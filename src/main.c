/**
 * @file main.c
 * @brief Main file for Parcel Protection Firmware
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * Hardware connections:
 *   UART - Bluetooth
 *     P3.2 - Tx
 *     P3.3 - Rx
 *   SPI - Accelerometer
 *     P3.0 - CS
 *     P1.6 - SDA
 *     P1.7 - SDO
 *     P1.5 - SCL
 *   GPIO
 *     P4.4 - Acc INT1
 *     P4.5 - Acc INT2
 *     P4.0 - Button
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */

#include "msp.h"
#include "uart.h"
#include "spi.h"
#include "circbuf.h"

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; /* stop watchdog timer */

  uart_init();
  spi_init();

  while(1);
}
