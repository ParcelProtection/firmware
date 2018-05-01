/**
 * @file uart.c
 * @brief UART functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * EUSCI_A0 - on-board UART
 * EUSCI_A2 - Bluetooth UART
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */

#include "msp.h"
#include "uart.h"

void uart_init()
{
  /* UART0 for logging */
  P1->SEL0 |= BIT3 | BIT2; /* UART mode */
  P1->SEL1 &= ~(BIT3 | BIT2);
  EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; /* disable */
  EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK | /* SMCLK as source */
                    EUSCI_A_CTLW0_SWRST; /* keep disabled */
  EUSCI_A0->BRW = 0x14; /* 9600 BAUD */
  EUSCI_A0->MCTLW = 0x22 << EUSCI_A_MCTLW_BRS_OFS |
                    0x4 << EUSCI_A_MCTLW_BRF_OFS |
                    EUSCI_A_MCTLW_OS16; /* enable oversampling */
  EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST); /* enable */

  /* UART1 for Bluetooth */
  P3->SEL0 |= BIT3 | BIT2; /* UART mode */
  P3->SEL1 &= ~(BIT3 | BIT2);
  EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; /* disable */
  EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK | /* SMCLK as source */
                    EUSCI_A_CTLW0_SWRST; /* stay disabled */
  EUSCI_A2->BRW = 0x14; /* 9600 BAUD */
  EUSCI_A2->MCTLW = 0x22 << EUSCI_A_MCTLW_BRS_OFS |
                    0x4 << EUSCI_A_MCTLW_BRF_OFS |
                    EUSCI_A_MCTLW_OS16; /* enable oversampling */
  EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST); /* enable */
}

void uart0_send(uint8_t data)
{
  /* wait for UART to be idle */
  while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

  EUSCI_A0->TXBUF = data;
}

void uart1_send(uint8_t data)
{
  /* wait for UART to be idle */
  while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));

  EUSCI_A2->TXBUF = data;
}
