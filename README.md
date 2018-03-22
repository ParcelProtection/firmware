# Parcel Protection Firmware
###### firmware for the Parcel Protection IoT project for CSCI 4830

## Hardware

#### MSP432P401R Launchpad
* TI MSP432 microcontroller
* ARM Cortex-M4 core
* UART, SPI, and I2C peripherals for communication
* RTC peripheral for time-keeping

#### Sparkfun ADXL345 Breakout Board
* 3-axis 13-bit accelerometer
* SPI or I2C communication
* Interrupt generation capable
* Free-fall detection
* Low power modes
* 32-member FIFO buffer for measurements

#### Adafruit PN532 NFC Breakout Board
* NFC chip with antenna
* UART, SPI, or I2C communication
* Bi-directional NFC connection capable
* Low power modes

## Target Features

#### Package Damage Detection
* Accelerometer-based fall and orientation change detection
* RTC to track current date/time
* Event logging to internal memory
* NFC communication with Android app
* Battery-powered operation from low-power modes

#### NFC Communication with Android Application
* Initial setup with acceleration thresholds
* Dump event data to be interpreted by application
