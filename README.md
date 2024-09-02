# Arduino Nano ESP32 ESP-IDF template
This template makes it easy to set up an ESP-IDF project for Arduino Nano ESP32 including [Arduino Core for ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/) and VS Code support via the ESP-IDF VS Code extension. Benefits:

- Work with the latest Arduino Core for ESP32 version (+3.0), something not currently possible with PlatformIO. Sadly, there appears to be [massive amounts of beef](https://github.com/platformio/platform-espressif32/issues/1225) between PlatformIO and Espressif, so this might not be resolved.
- A [CMake based build-system](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html).
- No need to use Arduino IDE or its CLI, which aren't great for more complex projects.
- Source-level debugging in VS Code via the built-in JTAG interface of the Arduino Nano ESP32.
- **MUCH** faster builds and flashing compared to Arduino IDE.

## Prerequisits
### Installing ESP-IDF
[ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v5.1.4/esp32/index.html) is Espressif's SDK for developing firmware for all their ESP32 SoCs. The simplest way to install it is via the VS Code extension.

1. Install the [ESP-IDF VS Code extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension). Ensure you have all its [prerequisits installed for your operating system](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#step-1-install-prerequisites).
2. Once installed, open the command palette in VS Code (`CTRL+P`/`CMD+P`) and run `ESP-IDF: Configure ESP-IDF extension`.
3. Select `Express`, then set `Select ESP-IDF Version:` to `v5.1.4`. You **MUST** use this version, as Arduino Core for ESP32 is not compatible with newer ESP-IDF versions. Click `Install` and wait for everything to complete. The ESP-IDF SDK is about 1.4GB. If the download is slow, abort and redo the installation using the Espressif server instead of the pre-configured GitHub server.

This process will install ESP-IDF as well as all the toolchains (GCC, GDB for each ESP32 CPU type). I.e. on Linux and macOS, ESP-IDF can be found in `$HOME/esp/esp-idf`, while the toolchains will be installed to `$HOME/.espressif`. This can also be done via the CLI, but it's not for the faint of heart.

> Note: the extension appears to fail to install the GCC toolchains seems to fail on macOS. After installing and configuring the ESP-IDF VS Code extension, check if the folder `~/.espressif/tools/xtensa-esp32s3-elf` exists. If it does not, open a terminal and execute `~/esp/esp-idf/install.sh`. That should properly unpack the GCC toolchains. I've filed an [issue](https://github.com/espressif/vscode-esp-idf-extension/issues/1301) on the ESP-IDF VS Code extension repository.

### Preparing the Arduino Nano ESP32
By default, the Nano ESP32 come's with Arduino's custom bootloader. ESP-IDF will not be able to flash or debug factory Nano ESP32s. Here's how to fix that.

1. Connect your Arduino Nano ESP32 via USB to your computer.
1. Connect pin B1 with the GND pin via a jumper wire. The LED should turn green.
1. While the LED is green, press the reset button on the Nano ESP32 (next to the LED).
1. Remove the wire, the LED should turn purple.

That's it. The Arduino Nano ESP32 is now in ROM boot mode. **The first time you flash it (as described in the next section), you have to manually press the reset button on the Nano ESP32 again for the flashed app to start. Subsequent flashing (and debugging) will not require you to press the reset button, as the ESP-IDF bootloader will already be installed and running.**

You can reset your Arduino Nano ESP32 to the factory bootloader as described [here](https://support.arduino.cc/hc/en-us/articles/9810414060188-Reset-the-Arduino-bootloader-on-the-Nano-ESP32).

## Using the Template
You have multiple alternatives to get the template
1. [Download](https://github.com/badlogic/arduino-nano-esp32-esp-idf-template/archive/refs/heads/main.zip) the repository as a ZIP file and unpack it
2. Clone the Git repository via `git clone https://github.com/badlogic/arduino-nano-esp32-esp-idf-template`
3. On the [GitHub repository page](https://github.com/badlogic/arduino-nano-esp32-esp-idf-template), click the green `Use this template` button. This will clone the repo into your GitHub user account. Then `git clone https://github.com/<username>/arduino-nano-esp32-esp-idf-template`.

Once you've downloaded the template, open it in VS Code, and connect your Arduino Nano ESP32 via USB. Common operations:
- Build & flash: press `CTRL + SHIFT + B` (or `CMD + SHIFT + B` on macOS) to build and flash the app to your connected Arduino Nano ESP32. A new terminal will pop up that shows the build and flashing log output. **If this is the first time you flash the app to your Nano ESP32, don't forget to press the reset button on the Nano!**
- Monitoring: press `CTRL + P` (or `CMD + SHIFT + P` on macOS), select `ESP-IDF: Monitor your device`. A new terminal will pop up that shows the incoming serial data, e.g. whatever you `printf()`.
- Debug: press `F5` to start a debugging session. The first time you run this, you'll be asked if an OpenOCD server should be started. Confirm. Each debugging session then restarts the firmware and halts in `app_main()`. Debugging will **NOT** rebuild your project! Rebuild and flash as described above first if necessary.

## Notes
- `Serial.begin()`, `Serial.println()`, and consorts won't work. I haven't figured out why, but I assume it's because we use the built-in JTAG/Serial interface. Use `printf()` instead.
- [Flashing via JTAG/OpenOCD is slow](https://github.com/espressif/openocd-esp32/issues/259). Sadly, even flashing via UART is faster. Couldn't find a way to make it faster. The default build task (`CTRL+SHIFT+B` or `CMD+SHIFT+B`) uses UART for flashing via `idf.py flash`.
- OpenOCD is a bit flaky at times. I found stopping and restarting it via `CTRL + P` (or `CMD + P` on macOS) followed by `ESP-IDF: OpenOCD Manager` can alleviate some of the pain.
- Debugging is slow. The ESP32-S3 only has 2 hardware breakpoints. Don't set more than 2 breakpoints, or stepping becomes unbearably slow. In fact, 1 breakpoint gives you the best stepping performance. I've added launch configurations for the espidf debugger, cppdbg, and native debug. YMMV. To use a debugger != espidf, you have to start the OpenOCD debugger manually via `ESP-IDF: OpenOCD Manager` from the command palette. Use the [Native Debug](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) extension if possible. You can't set breakpoints while the app is running. Use the pause button, then set the breakpoint, then resume. Native Debug sometimes fail to attach to the process. Just launch it again.
- By default, ESP-IDF compiles code with -Og. This will allow debugging, but will also introduce some optimizations in the code. Some of your stepping may look weird, some variables may be missing. Don't worry about it. -O0 won't work, as the resulting binary will be too large usually.
- Arduino Core for ESP32 uses GPIO pin numbers. What you see on your Arduino Nano ESP32 is wrong. Use the mappings from Arduino pin numbers to GPIO [here](https://docs.arduino.cc/tutorials/nano-esp32/pin-setup/).