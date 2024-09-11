/*!
 * @file      main_ASFS_App.c
 *
 * @brief     Example of Adaptive Spreading Factor Selection (ASFS) for the SX126x chip
 *
 * This code demonstrates a basic application of the Adaptive Spreading Factor Selection (ASFS) algorithm
 * using the SX126x LoRa transceiver. The primary purpose of this program is to receive a message transmitted 
 * by a node without prior knowledge of the spreading factor (SF). The ASFS algorithm dynamically adjusts 
 * the spreading factor based on the detection of channel activity, optimizing communication performance in LoRa networks.
 *
 * The Clear BSD License
                             ___  ________  ___  ________  ________     
                            |\  \|\   __  \|\  \|\   ____\|\   __  \    
                            \ \  \ \  \|\  \ \  \ \  \___|\ \  \|\  \   
                             \ \  \ \   _  _\ \  \ \_____  \ \   __  \  
                              \ \  \ \  \\  \\ \  \|____|\  \ \  \ \  \ 
                               \ \__\ \__\\ _\\ \__\____\_\  \ \__\ \__\
                                \|__|\|__|\|__|\|__|\_________\|__|\|__|
                                                   \|_________|         
                   (c) IRISA Corporation 2024. All rights reserved.
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "apps_common.h"
#include "apps_utilities.h"

#include "sx126x.h"
#include "main_ASFS_App.h"
#include "sx126x_str.h"
#include "smtc_hal_mcu.h"
#include "smtc_hal_dbg_trace.h"
#include "uart_init.h"
#include "stm32l4xx_ll_utils.h"


static sx126x_hal_context_t* context;

/* Definition of global variables for ASFS */
sx126x_lora_sf_t LORA_SPREADING_FACTOR_t = SX126X_LORA_SF7;
sx126x_cad_exit_modes_t CAD_EXIT_MODE = SX126X_CAD_ONLY;
sx126x_mod_params_lora_t lora_mod_params;
sx126x_cad_params_t cad_params;


/* Function definitions */
void change_LORA_SPREADING_FACTOR_t(sx126x_lora_sf_t sf) {
    LORA_SPREADING_FACTOR_t = sf;
}
void change_CAD_EXIT_MODE(sx126x_cad_exit_modes_t mode) {
    CAD_EXIT_MODE = mode;
}

void change_lora_mod_params(sx126x_lora_sf_t sf) {
    lora_mod_params.sf   = sf;
    lora_mod_params.bw   = LORA_BANDWIDTH;
    lora_mod_params.cr   = LORA_CODING_RATE;
    lora_mod_params.ldro = 0;  // Will be initialized during radio init
}

void change_cad_params(sx126x_cad_exit_modes_t mode)
	{
    cad_params.cad_symb_nb     = CAD_SYMBOL_NUM;
    cad_params.cad_detect_peak = CAD_DETECT_PEAK;
    cad_params.cad_detect_min  = CAD_DETECT_MIN;
    cad_params.cad_exit_mode   = mode;
    cad_params.cad_timeout     = 0;
}

sx126x_lora_sf_t LORA_SF=SX126X_LORA_SF7;

static uint8_t  buffer[PAYLOAD_LENGTH];
static uint32_t iteration_number        = 0;
static uint32_t detection_counter       = 0;
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static void start_cad_after_delay( uint16_t delay_ms );

static void optimize_cad_parameters( sx126x_lora_sf_t sf, sx126x_cad_params_t* cad_params );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */
void init_app(sx126x_lora_sf_t sf, sx126x_cad_exit_modes_t mode)__attribute__( ( weak ) );

 
/*
 * @brief : This application configures and initializes the SX126x LoRa radio for communication.
 *        It sets up the LoRa spreading factor (SF) and Channel Activity Detection (CAD) modes,
 *        optimizes the radio parameters, and starts the CAD process. The application handles
 *        interrupts to process events such as data reception, transmission, and errors.
 */ 
int main(void)
{
    // Initialize the MCU (Microcontroller Unit)
    smtc_hal_mcu_init();
    // Initialize UART (Universal Asynchronous Receiver-Transmitter)
    uart_init();
    // Initialize the shield (hardware component that the system relies on)
    apps_common_shield_init();
    // Get the context for the SX126x (radio chip for LoRa communication)
    context = apps_common_sx126x_get_context();
    // Initialize the SX126x with the retrieved context
    apps_common_sx126x_init((void*)context);
    // Set the IRQ (Interrupt Request) parameters for the SX126x.
    // This configures which interrupts the chip will trigger, such as CAD (Channel Activity Detection),
    // RX (Receive), TX (Transmit), and various error interrupts.
    sx126x_set_dio_irq_params(context, 
                              SX126X_IRQ_ALL,  // All possible IRQs are set
                              SX126X_IRQ_CAD_DETECTED | SX126X_IRQ_CAD_DONE | SX126X_IRQ_TX_DONE | 
                              SX126X_IRQ_RX_DONE | SX126X_IRQ_TIMEOUT | SX126X_IRQ_HEADER_ERROR | 
                              SX126X_IRQ_CRC_ERROR,  // IRQ sources being enabled
                              SX126X_IRQ_NONE,  // No DIO1 interrupts are enabled
                              SX126X_IRQ_NONE); // No DIO2 interrupts are enabled
    // Clear all pending IRQs for the SX126x
    sx126x_clear_irq_status(context, SX126X_IRQ_ALL);
    // Initialize the application with specific parameters:
    // SX126X_LORA_SF7 refers to spreading factor 7 for LoRa communication,
    // and SX126X_CAD_RX indicates it's operating in CAD (Channel Activity Detection) mode.
    init_app(SX126X_LORA_SF7, SX126X_CAD_RX);
    // Print version information for the SX126x chip
    apps_common_sx126x_print_version_info();
    // Print current configuration of the SX126x chip
    apps_common_sx126x_print_config();
    // Main loop: Continuously process interrupts from the SX126x
    while(1)
    {
        apps_common_sx126x_irq_process((void*)context);  // Handle IRQs (interrupts) in an infinite loop
    }
}

/*
 * @brief: Initializes the application with a specified LoRa spreading factor and CAD mode.
 *        Optimizes the CAD parameters and starts the CAD process after a defined delay.
 */

void init_app(sx126x_lora_sf_t sf, sx126x_cad_exit_modes_t mode)
{
    // Initialize the detection counter to 0
    detection_counter = 0;
    // Change the LoRa spreading factor based on the provided value
    change_LORA_SPREADING_FACTOR_t(sf);
    // Adjust the LoRa modulation parameters to match the new spreading factor
    change_lora_mod_params(sf);
    // Change the CAD (Channel Activity Detection) exit mode based on the provided mode
    change_CAD_EXIT_MODE(mode);
    // Adjust the CAD parameters according to the new exit mode
    change_cad_params(mode);
    // Initialize the radio with the current context
    apps_common_sx126x_radio_init((void*)context);
    // Optimize the CAD parameters based on the LoRa spreading factor
    optimize_cad_parameters(LORA_SPREADING_FACTOR_t, &cad_params);
    // If the CAD exit mode is set to switch to RX (receive mode) after detection
    if(cad_params.cad_exit_mode == SX126X_CAD_RX)
    {
        // Convert the CAD timeout value from milliseconds to RTC (Real-Time Clock) steps
        cad_params.cad_timeout = sx126x_convert_timeout_in_ms_to_rtc_step(CAD_TIMEOUT_MS);
    }
    // Set the CAD parameters for the SX126x, ensuring no errors during the process
    ASSERT_SX126X_RC(sx126x_set_cad_params(context, &cad_params));
    // Start the CAD process after a specified delay in milliseconds
    start_cad_after_delay(DELAY_MS_BEFORE_CAD);
}

	
// Callback function triggered when CAD (Channel Activity Detection) detects activity
void on_cad_done_detected(void)
{
    // Increment the detection counter
    detection_counter++;
    // Handle the CAD exit mode based on the current configuration
    switch(cad_params.cad_exit_mode)
    {
        // If CAD mode is set to only detect and stop (no RX mode)
        case SX126X_CAD_ONLY:
            // Restart CAD detection after a specified delay
            start_cad_after_delay(DELAY_MS_BEFORE_CAD);
            break;
        // If CAD mode is set to switch to RX (receive mode) after detection
        case SX126X_CAD_RX:
            // Handle the pre-RX setup (ready the system for receiving data)
            apps_common_sx126x_handle_pre_rx();
            break;
        // If CAD mode is set for LBT (Listen Before Talk), restart CAD after delay
        case SX126X_CAD_LBT:
            // Restart CAD detection after a specified delay
            start_cad_after_delay(DELAY_MS_BEFORE_CAD);
            break;
        // Handle unknown CAD exit modes (error logging)
        default:
            // Print an error message with the unknown CAD exit mode
            HAL_DBG_TRACE_ERROR("Unknown CAD exit mode: 0x%02x\n", cad_params.cad_exit_mode);
            break;
    }
}

// Callback function triggered when a preamble is detected
void on_preamble_detected(void)
{
    // Increment the detection counter
    detection_counter++;
}

// Callback function triggered when no preamble is detected
void on_preamble_undetected(void)
{
    // If no preamble is detected, restart the CAD process
    ASSERT_SX126X_RC(sx126x_set_cad(context));
}

/*
 * @brief: This function is called when CAD (Channel Activity Detection) fails to detect activity.
 *        It adjusts the LoRa spreading factor (SF) based on the detection counter. If too many 
 *        undetected attempts occur, it resets or adjusts the spreading factor to improve signal sensitivity.
 */
void on_cad_done_undetected(void)
{			
    // If more than 5 unsuccessful detections, reset spreading factor to SF7
    if(detection_counter > 5)
    {
        LORA_SF = SX126X_LORA_SF7;  // Set spreading factor to SF7 (default/lowest)
        detection_counter = 0;      // Reset detection counter
    } 
    else 
    {
        // If detection failed fewer than 5 times, adjust the spreading factor
        // If SF is currently SF11, reset to SF7 for faster transmission
        if (LORA_SF == SX126X_LORA_SF11)
        {
            LORA_SF = SX126X_LORA_SF7;  // Reset to SF7 (lowest spreading factor)
        }
        else
        {
            // Otherwise, increase the spreading factor to improve sensitivity
            LORA_SF++;  // Increment SF to next higher level (e.g., SF7 -> SF8, etc.)
        }
    }
    // Re-initialize the application with the adjusted spreading factor
    init_app(LORA_SF, SX126X_CAD_RX);
}

/*
 * @brief: This function is called when the SX126x radio successfully receives a packet.
 *        It handles post-reception processing, reads the received data into a buffer,
 *        and prepares the radio to receive the next packet after a random delay.
 */
void on_rx_done(void)
{
    uint8_t size;
    // Clear the buffer to prepare for incoming data
    memset(buffer, 0, PAYLOAD_LENGTH);
    // Handle post-reception processes such as clearing interrupts
    apps_common_sx126x_handle_post_rx();
    // Retrieve the received data from the radio into the buffer
    apps_common_sx126x_receive((void*)context, buffer, &size, PAYLOAD_LENGTH);
    // Prepare for the next reception
    apps_common_sx126x_handle_pre_rx();
    // Set the radio to receive mode again, with a random delay
    sx126x_set_rx(context, get_time_on_air_in_ms() + RX_TIMEOUT_VALUE + rand() % 500);
}

/*
 * @brief: This function is called when there is an error during the reception of a packet.
 *        It handles post-reception error processing and reinitializes the radio to listen
 *        for the next packet after a random delay.
 */
void on_rx_error(void)
{
    // Handle post-reception processes such as clearing interrupts, even after an error
    apps_common_sx126x_handle_post_rx();
    // Set the radio to receive mode again, with a random delay to avoid collisions
    sx126x_set_rx(context, get_time_on_air_in_ms() + RX_TIMEOUT_VALUE + rand() % 500);
}

/*
 * @brief: This function starts the CAD (Channel Activity Detection) process after a delay.
 *        It waits for the specified time (in milliseconds) and then initiates CAD to check
 *        for channel activity.
 */
static void start_cad_after_delay(uint16_t delay_ms)
{
    // Wait for the specified delay in milliseconds
    LL_mDelay(delay_ms);
    // Start the CAD process and check for any errors during the setup
    ASSERT_SX126X_RC(sx126x_set_cad(context));
}



#if USER_PROVIDED_CAD_PARAMETERS == False
static void optimize_cad_parameters( sx126x_lora_sf_t sf, sx126x_cad_params_t* cad_params )
{
    switch( sf )
    {
    case SX126X_LORA_SF7:
        cad_params->cad_detect_min  = 10;
        cad_params->cad_detect_peak = 22;
        cad_params->cad_symb_nb     = 2;
        break;
    case SX126X_LORA_SF8:
        cad_params->cad_detect_min  = 10;
        cad_params->cad_detect_peak = 22;
        cad_params->cad_symb_nb     = 2;
        break;
    case SX126X_LORA_SF9:
        cad_params->cad_detect_min  = 10;
        cad_params->cad_detect_peak = 23;
        cad_params->cad_symb_nb     = 4;
        break;
    case SX126X_LORA_SF10:
        cad_params->cad_detect_min  = 10;
        cad_params->cad_detect_peak = 24;
        cad_params->cad_symb_nb     = 4;
        break;
    case SX126X_LORA_SF11:
        cad_params->cad_detect_min  = 10;
        cad_params->cad_detect_peak = 25;
        cad_params->cad_symb_nb     = 4;
        break;
    default:
        HAL_DBG_TRACE_WARNING( "CAD may not function properly while using these radio parameters\n" );
        break;
    }
}
#else
static void optimize_cad_parameters( sx126x_lora_sf_t sf, sx126x_cad_params_t* cad_params )
{
}
#endif