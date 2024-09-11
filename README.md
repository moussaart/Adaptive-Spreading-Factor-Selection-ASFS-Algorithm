# Adaptive Spreading Factor Selection (ASFS) for SX126x LoRa Transceiver 📡

## Description

This project implements an Adaptive Spreading Factor Selection (ASFS) algorithm for the SX126x LoRa transceiver. The ASFS algorithm dynamically adjusts the spreading factor (SF) based on channel activity, optimizing communication performance in LoRa networks. The implementation is tailored for the SX1261MB2BAS shield and the STM32 Nucleo L476RG microcontroller.

## Hardware 🛠️

- **Transceiver Shield**: [SX126X readme](sx126x/README.md)
- **Microcontroller**: [STM32 Nucleo L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)

## Software 💻

- **Development Environment**: Keil uVision 5
- **Libraries and Dependencies**: Utilizes the `sx126x` library for interfacing with the LoRa transceiver, and other utility libraries for hardware abstraction and debugging.

## Files 📂

- `main_ASFS_App.c`: Contains the main application code implementing the ASFS algorithm.
- `apps_common.h`, `apps_utilities.h`: Utility header files for common functions and definitions.
- `sx126x.h`, `sx126x_str.h`: Header files for the SX126x library.
- `smtc_hal_mcu.h`, `smtc_hal_dbg_trace.h`: Hardware abstraction and debugging header files.
- `uart_init.h`: Header file for UART initialization.
- `stm32l4xx_ll_utils.h`: STM32 low-layer utilities header file.

## `ASFS_app.uvprojx` Description

**Path:** `core/sx126x/ASFS/MDK-ARM/ASFS_app.uvprojx`

**Description:**

The file `ASFS_app.uvprojx` is a Keil uVision 5 project file used to configure and manage the development of the Adaptive Spreading Factor Selection (ASFS) algorithm for the SX126x LoRa transceiver. This project file is essential for building, debugging, and deploying the ASFS application on the STM32 Nucleo L476RG microcontroller.

### Key Features:

- **Project Configuration:** Defines project settings, including compiler options, linker settings, and debug configurations.
- **Source Management:** Manages source files and directories associated with the ASFS application.
- **Build Process:** Integrates with Keil’s build tools to compile the ASFS algorithm and generate the executable code for the STM32 microcontroller.

### Usage ⚙️

1. **Open Keil uVision 5:**
   - Launch Keil uVision 5 on your development machine.

2. **Open the Project:**
   - Go to `File` > `Open Project...` in the menu.
   - Navigate to the `core/sx126x/ASFS/MDK-ARM/` directory.
   - Select the `ASFS_app.uvprojx` file and click `Open`.

3. **Build the Project:**
   - After opening the project, go to `Project` > `Build Target` to compile the source code and generate the binary.

4. **Debug and Deploy:**
   - Connect your STM32 Nucleo L476RG microcontroller to your development environment.
   - Go to `Debug` > `Start/Stop Debug Session` to begin debugging the application.

5. **Configuration and Customization:**
   - You can modify project settings, add or remove source files, and adjust compiler/linker options through the project’s configuration dialogs in Keil uVision 5.

**Note:** Ensure that you have all necessary hardware and software prerequisites installed, including the STM32 HAL drivers and any required libraries for the SX126x transceiver.

For more detailed instructions on working with Keil uVision 5, refer to the Keil documentation or online resources.

## Installation 🚀

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/your-repository.git
    ```
2. Open the project in Keil uVision 5.
3. Configure the project settings to match your hardware setup (SX1261MB2BAS shield and STM32 Nucleo L476RG).
4. Build the project and upload the firmware to the microcontroller.


## Adaptive Spreading Factor Selection (ASFS) 🔍

### Context and Importance

In a LoRa Mesh Network, nodes communicate using different Spreading Factors (SFs), which determine the data transmission rate and signal range. Each SF corresponds to a distinct symbol duration, directly affecting the time needed to detect the preamble (initial synchronization sequence) during message reception.

When multiple clusters use different SFs, a node belonging to multiple clusters must be able to receive messages on various channels. This complicates synchronization, as the node needs to detect and align with the SF used by each transmitter within a limited time. Efficient detection and alignment are crucial for maintaining reliable communication.


### Problem to Address

The main challenge is to minimize the time required to detect the SF used by a transmitter, as the receiver must inspect several possible SFs before synchronizing with the correct one. The order in which SFs are inspected significantly affects the total detection time (CAD time). Inefficient inspection can lead to delays or communication failures if the preamble is not detected in time.

### ASFS Algorithm 🧩

To improve SF detection speed and reliability, the ASFS algorithm dynamically adjusts the inspection order of SFs based on network conditions and the node's communication history. The key steps are:

<img src="image\Algorithme de sélection SF. (1).png" alt="ASFS Algorithm">
![Example Image](image\Algorithme de sélection SF. (1).png)

1. **Iterative Inspection**: The receiver inspects each SF multiple times during preamble reception. This approach reduces false detection chances but increases detection time.
2. **Probability-Based Selection**: If an SF is consistently detected across multiple iterations, it is selected for synchronization. If a lower SF (SF7 or SF8) is detected, inspection is stopped early, as the probability of false detection is negligible at these levels.
3. **Conditional Detection for SF9 to SF12**: For higher SFs (SF9 to SF12), the algorithm continues inspection until a neighboring SF fails to detect the preamble. This confirms the SF with increased confidence, even in noisy or high-interference environments.

### Benefits of ASFS 🌟

The ASFS algorithm significantly reduces SF detection time while enhancing communication reliability. Although it is more complex to implement, its performance benefits make it preferable, especially in scenarios where nodes must communicate with multiple clusters using different SFs.



## License 📜

This project is licensed under the Clear BSD License. See the [LICENSE](LICENSE.txt) file for more details.

## References 📚

- Kim, J., & Kim, J. (2020). Adaptive Spreading Factor Selection in LoRa Networks. [Link to Paper](https://www.mdpi.com/1424-8220/20/4/1008)

## Contact 📧

For any questions or issues, please contact [MOUSSA Taha](https://mot4373.wixsite.com/taha-moussa-cv/contact) .