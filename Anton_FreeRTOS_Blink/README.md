# Anton_FreeRTOS_Blink

This project demonstrates the use of FreeRTOS on the Raspberry Pi Pico W by implementing a simple LED blink application. The application utilizes FreeRTOS tasks to manage the LED blinking behavior.

## Features

- Initialization of hardware components, including the standard output over USB.
- Creation of a FreeRTOS task to handle different LED blinking.
- Multithreaded loops/tasks that individually turn on and off three LEDs.

## Requirements

- Raspberry Pi Pico W
- CMake build system
- FreeRTOS
- Pico SDK

## Project Structure

- `main.c`: Contains the main application code, including hardware initialization and FreeRTOS task creation.
- `CMakeLists.txt`: Build configuration file for the project.

## Setup and Compilation

1. **Set Up the Pico SDK**:
   - Ensure that the Pico SDK is correctly set up on your system.
   - Set the `PICO_SDK_PATH` environment variable to point to the Pico SDK directory.

2. **Clone the Repository**:
   - Clone this repository to your local machine.

3. **Create a Build Directory**:
   - Navigate to the project directory.
   - Create a directory named `build` and navigate into it:
     ```bash
     mkdir build
     cd build
     ```

4. **Configure the Project with CMake**:
   - Run the following command to configure the project:
     ```bash
     cmake -DPICO_BOARD=pico_w ..
     ```

5. **Build the Project**:
   - Compile the project using the following command:
     ```bash
     make
     ```

6. **Upload the Firmware**:
   - After a successful build, upload the generated `.uf2` file to your Raspberry Pi Pico W by copying it to the Pico's storage.

## Usage

Once the firmware is uploaded and the Raspberry Pi Pico W is running:

- The onboard LED will blink with a variation of intervals, indicating that the FreeRTOS task is operating correctly.

## License

This project is licensed under the MIT License. See the [LICENSE](../LICENSE) file for details.

For more information, visit the [Raspberry Pi Pico SDK documentation](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html) and the [FreeRTOS documentation](https://www.freertos.org/).

