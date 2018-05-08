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

#### HV-06 Bluetooth Board
* Bluetooth chip with antenna
* UART communication
* Bi-directional Bluetooth over serial connection
___

## Features

#### Package Damage Detection
* Accelerometer-based fall and orientation change detection
* RTC to track current date/time
* Event logging to internal memory
* Bluetooth communication with Android app
* Battery-powered operation from low-power modes

#### Bluetooth Communication with Android Application
* Initial setup with acceleration thresholds
* Dump event data to be interpreted by application
___

## Hardware Connections

MSP432 Launchpad | HV-06
---------------- | -----
5V               | 5V
GND              | GND
P3.2             | Tx
P3.3             | Rx

MSP432 Launchpad | ADXL432
---------------- | -------
3.3V             | VCC
GND              | GND
P3.0             | CS
P4.4             | INT1
P4.5             | INT2
P1.7             | SDO
P1.6             | SDA
P1.5             | SCL
___

## Build Instructions

* Import source code into Code Composer Studio
* Add inc/ as a path for include files
* Build and debug using CCS
