# Plant Automation System

This Arduino-based project automates plant watering and environmental control based on user-selected plant types. The system monitors temperature, humidity, soil moisture, light levels, and distinguishes between natural and artificial light using an IR sensor. It controls a fan and a servo motor for watering, providing optimal conditions for different types of plants.

## Features

- **Plant Type Selection**: Choose between fruits and vegetables, herbs, cacti, and flowers using a keypad.
- **Environmental Monitoring**: Continuously measures temperature, humidity and soil moisture
- **Automatic Watering**: Activates the watering system based on soil moisture levels specific to the plant type.
- **LCD Display**: Displays real-time temperature, humidity, and system status.

## Components Used

- Arduino board
- Temperature and humidity sensor (DHT11)
- Soil moisture sensor
- Servo motor
- I2C LCD display
- Keypad


## Usage

1. **Select Plant Type**:
    - On startup, use the keypad to select the plant type: 1 for Fruits, 2 for Herbs, 3 for Cacti, 4 for Flowers.

2. **Monitor and Control**:
    - The system will display the temperature and humidity on the LCD.
    - The watering system will automatically functioning according to the sensor readings and thresholds.

## Configuration

- Adjust the thresholds for soil moisture,in the code as needed to suit your specific requirements.

## Contributing

- This is a private project.

## Acknowledgments

- Special thanks to the creators of the libraries used in this project.


