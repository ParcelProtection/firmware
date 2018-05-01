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

void gpio_init()
{
  P4->SEL0 &= !(BIT5 | BIT4 | BIT0); /* GPIO mode */
  P4->SEL1 &= !(BIT5 | BIT4 | BIT0);
  P4->DIR &= !(BIT5 | BIT4 | BIT0); /* configure as inputs */
  P4->REN |= BIT0; /* enable pullup/down resistor */
  P4->OUT |= BIT0; /* pullup resistor */
  P4->IES &= ~(BIT5 | BIT4); /* interrupt on low to high transition */
  P4->IES |= BIT0; /* interrupt on high to low transition */
  P4->IE |= 0; /* enable interrupts */
  NVIC_EnableIRQ(PORT4_IRQn);
  __enable_interrupts();
}

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; /* stop watchdog timer */

  uart_init();
  spi_init();
  gpio_init();

  while(1);
}

void PORT4_IRQHandler()
{
  if(P4->IFG & BIT0)
  {
    P4->IFG &= ~(BIT0);
  }
  if(P4->IFG & BIT4)
  {
    P4->IFG &= ~(BIT4);
  }
  if(P4->IFG & BIT5)
  {
    P4->IFG &= ~(BIT5);
  }
}
