# Flu Metrics Collector

## Overview

`flu-metrics-collector` is an embedded application designed to run on an ESP32 microcontroller. It collects health metrics such as heart rate, oxygen saturation (SpO2), and temperature using a MAX30100 pulse oximeter and a DS18B20 temperature sensor. Upon user initiation via a button press, it records the readings and sends the collected data to a specified API endpoint over Wi-Fi. LEDs are used to signal each step of the process, providing visual feedback.

This project is part of a college assignment aimed at collecting health data during specific times of the year and in regions identified by FIOCRUZ (Info Gripe) where flu outbreaks are common.

## Prerequisites - Use

To use `flu-metrics-collector`, you need:

- **Hardware:**

  - An **ESP32** development board.
  - A **MAX30100** pulse oximeter sensor module.
  - A **DS18B20** temperature sensor.
  - A **button** connected to a GPIO pin for user input.
  - **LEDs** connected to GPIO pins for step indication.

- **Software:**
  - A computer with **PlatformIO** installed.
  - **Python** installed on your system (required by PlatformIO scripts).
  - Access to a **Wi-Fi network**.

## Configuration

The application is configured via the `Config.h` file, which contains Wi-Fi credentials, API endpoint, and pin definitions.

### `Config.h` File

```cpp
#pragma once

// Wi-Fi settings
constexpr char WIFI_SSID[] = "YourWiFiSSID";
constexpr char WIFI_PASSWORD[] = "YourWiFiPassword";

// API settings
constexpr char SERVER_NAME[] = "https://yourapi.com/api/v1/data";

// Pin definitions
constexpr int ONE_WIRE_BUS = 2;       // DS18B20 data pin
constexpr int BUTTON_PIN = 4;         // Button pin
constexpr int LED_WIFI = 16;          // Wi-Fi status LED
constexpr int LED_SENSORS = 17;       // Sensors status LED
constexpr int LED_SENDING = 5;        // Data sending status LED
```

**Note:** Replace the placeholder values with your actual Wi-Fi credentials and API endpoint.

## Usage

1. **Set Up the Hardware:**

   - **MAX30100 Pulse Oximeter:**

     - Connect `SDA` to GPIO 21 (default I2C SDA on ESP32).
     - Connect `SCL` to GPIO 22 (default I2C SCL on ESP32).
     - Connect `VIN` to 3.3V.
     - Connect `GND` to Ground.

   - **DS18B20 Temperature Sensor:**

     - Connect the data line to the pin defined as `ONE_WIRE_BUS` (default is GPIO 2).
     - Connect a 4.7kΩ resistor between the data line and 3.3V (pull-up resistor).

   - **Button:**

     - Connect one side of the button to the `BUTTON_PIN` (default is GPIO 4).
     - Connect the other side to Ground.
     - Enable internal pull-up resistor in the code (already handled).

   - **LEDs:**
     - Connect LEDs to the pins defined for `LED_WIFI`, `LED_SENSORS`, and `LED_SENDING`.
     - Use current-limiting resistors (e.g., 220Ω) for each LED.

2. **Configure the Application:**

   - Open `Config.h` and update:
     - `WIFI_SSID` and `WIFI_PASSWORD` with your Wi-Fi credentials.
     - `SERVER_NAME` with your API endpoint.
     - Pin definitions if your hardware setup differs.

3. **Build and Upload the Application:**

   - **Generate Compile Commands for Code Completion (Optional):**

     To enable code completion in your IDE, run:

     ```sh
     platformio run -t compiledb
     ```

     This will generate a `compile_commands.json` file using the `gen_compile_commands.py` script

   - **Build and Upload:**

     Connect your ESP32 board to the computer via USB and run:

     ```sh
     platformio run -t upload
     ```

     This command compiles the code and uploads it to the ESP32.

4. **Operation:**

   - **Startup:**

     - The ESP32 initializes and attempts to connect to the specified Wi-Fi network.
     - The `LED_WIFI` turns on once connected.

   - **Sensor Initialization:**

     - Sensors are initialized.
     - The `LED_SENSORS` turns on to indicate readiness.

   - **Data Collection:**

     - The device waits for the user to press the button.
     - Upon button press:
       - The `LED_SENSORS` turns off, and `LED_SENDING` turns on.
       - Heart rate and SpO2 data are collected over 10 seconds.
       - Temperature is read from the DS18B20 sensor.
       - Collected data is sent to the API endpoint.
       - LEDs update to reflect each step.

   - **Post-Operation:**
     - After sending data, the `LED_SENDING` turns off, and `LED_SENSORS` turns back on.
     - The device returns to waiting for the next button press.

## Building

1. **Install PlatformIO:**

   - Follow the instructions at [PlatformIO Installation](https://docs.platformio.org/en/latest/core/installation.html) to install PlatformIO Core.

2. **Clone the Repository:**

   ```sh
   git clone https://github.com/RafaelBahiense/flu-metrics-collector.git
   ```

3. **Navigate to the Project Directory:**

   ```sh
   cd flu-metrics-collector
   ```

4. **Install Dependencies:**

   - PlatformIO will automatically handle library dependencies specified in the `platformio.ini` file.

5. **Build the Project:**

   ```sh
   platformio run
   ```

6. **Upload to ESP32:**

   ```sh
   platformio run -t upload
   ```

## Dependencies

The project relies on the following libraries:

- **Arduino Framework:** Core framework for ESP32 development.
- **DallasTemperature:** For interacting with the DS18B20 temperature sensor.
- **OneWire:** Required by DallasTemperature for one-wire communication.
- **MAX30100 Pulse Oximeter Library:** For interfacing with the MAX30100 sensor.
- **WiFi.h and HTTPClient.h:** For Wi-Fi connectivity and HTTP communication.

These libraries are specified in the `platformio.ini` file

## Contributing

Contributions to the project are welcome! Please follow the standard Git workflow:

1. **Fork** the repository.
2. **Create** a new branch for your feature or bugfix.
3. **Make** your changes.
4. **Submit** a pull request with a clear description of your changes.

## License

`flu-metrics-collector` is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

**Disclaimer:** Ensure you comply with all local regulations and ethical guidelines when collecting health data. The use of this device should respect privacy laws and obtain necessary consents.
