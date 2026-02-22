# Logitech Z906 ESPHome Integration

Logitech Z906 Control Library for ESPHome

This project enables control of the Z906 unit without the console, using any device with a programmable serial port, eg a ESP32 microcontrollers. Which then can be connected to home assistent via the ESPHome integration.

# DE-15 Console Connector

The communication between the Digital Audio Processor and the console is done through TTL serial communication at 3.3V.

The following table illustrates the pinout.

## Pin out

| Pin | Amplifier                | ESP32 (connection point) |
| --- | ------------------------ | ------------------------ |
| 1   | Headphones Right         |                          |
| 2   | Headphones Left          |                          |
| 3   | Audio Ground             | AUX gnd (optional)       |
| 4   | AUX R                    | AUX R (optional)         |
| 5   | AUX L                    | AUX L (optional)         |
| 6   | Ground                   | Ground                   |
| 7   | Unknown (not required)   |                          |
| 8   |                          |                          |
| 9   | Unused                   |                          |
| 10  | Unused                   |                          |
| 11  | 3.3V                     | 3.3V                     |
| 12  | Tx                       | GPIO16                   |
| 13  | Rx                       | GPIO17                   |
| 14  | Unused                   |                          |
| 15  | Console presence Pull-up | Ground                   |

D2-71583 is **NOT** 5V tolerant. If you have Arduino UNO, MEGA, etc use a **Logic Level Converter.**

| Serial settings |       |
| --------------- | ----- |
| Baud rate       | 57600 |
| Data            | 8 bit |
| Parity          | Odd   |
| Stop            | 1 bit |

## Credits

This project builds upon the excellent work of:

- **[zarpli/Logitech-Z906](https://github.com/zarpli/Logitech-Z906)** - Original Z906 control library
- **[nomis/logitech-z906](https://github.com/nomis/logitech-z906)** - Z906 hardware and protocol documentation

This fork integrates the Z906 control library with ESPHome for easy home automation integration.
