# PlantAutomationSystem
Arduino-based plant watering and environmental control system

# Plant Automation System

This Arduino-based project automates plant watering and environmental control based on user-selected plant types. The system monitors temperature, humidity, soil moisture, light levels, and distinguishes between natural and artificial light using an IR sensor. It controls a fan and a servo motor for watering, providing optimal conditions for different types of plants.

## Features

- **Plant Type Selection**: Choose between fruits and vegetables, herbs, cacti, and flowers using a keypad.
- **Environmental Monitoring**: Continuously measures temperature, humidity, soil moisture, and light levels.
- **Light Source Detection**: Differentiates between natural and artificial light using an IR sensor.
- **Automatic Watering**: Activates the watering system based on soil moisture levels specific to the plant type.
- **Temperature Control**: Turns on a fan when the temperature exceeds preset thresholds during the day or night.
- **LCD Display**: Displays real-time temperature, humidity, and system status.

## Components Used

- Arduino board
- Temperature and humidity sensor (DHT11)
- Soil moisture sensor
- Light-dependent resistor (LDR)
- IR receiver
- Servo motor
- Relay module
- I2C LCD display
- Keypad

## Schematic

(Include a schematic diagram of your circuit here)

## Installation

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/your-username/PlantAutomationSystem.git
    cd PlantAutomationSystem
    ```

2. **Install Required Libraries**:
    - DHT sensor library
    - LiquidCrystal_I2C library
    - Keypad library
    - IRremote library

3. **Upload the Code**:
    - Open `PlantAutomationSystem.ino` in the Arduino IDE.
    - Select your Arduino board and port.
    - Click on **Upload**.

## Usage

1. **Select Plant Type**:
    - On startup, use the keypad to select the plant type: 1 for Fruits, 2 for Herbs, 3 for Cacti, 4 for Flowers.

2. **Monitor and Control**:
    - The system will display the temperature and humidity on the LCD.
    - The fan and watering system will activate based on the monitored conditions.

## Configuration

- Adjust the thresholds for soil moisture, temperature, and light levels in the code as needed to suit your specific requirements.

## Contributing

Feel free to fork this repository, make changes, and submit pull requests. Contributions are welcome!

## Acknowledgments

- Special thanks to the creators of the libraries used in this project.


