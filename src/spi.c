/**
 * @file spi.c
 * @brief SPI functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * EUSCI_B0 - SPI to accelerometer
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */

#include "msp.h"
#include "spi.h"

void spi_init()
{
  /* SPI for accelerometer */
  P3->SEL0 &= ~(BIT0); /* CS as STE for SPI mode */
  P3->SEL1 &= ~(BIT0);
  P3->DIR |= BIT0;
  P3->OUT |= BIT0;
  P1->SEL0 |= BIT7 | BIT6 | BIT5; /* MISO, MISO, CLK in SPI mode */
  P1->SEL1 &= ~(BIT7 | BIT6 | BIT5);
  EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; /* disable */
  EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPL | /* clock inactive high */
                    EUSCI_B_CTLW0_MSB | /* MSB first */
                    EUSCI_B_CTLW0_MST | /* master mode */
                    EUSCI_B_CTLW0_MODE_2 | /* 4-wire with CS active low */
                    EUSCI_B_CTLW0_SSEL__SMCLK | /* SMCLK as source */
                    EUSCI_B_CTLW0_STEM | /* CS used to enable */
                    EUSCI_B_CTLW0_SWRST; /* stay disabled */
  EUSCI_B0->BRW = 0x10; /* bring the clock down a bit */
  EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST); /* enable */
}

void spi_write(uint8_t addr, uint8_t data)
{
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  P3->OUT &= ~(BIT0);
  EUSCI_B0->TXBUF = ~BIT7 & addr;
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  EUSCI_B0->TXBUF = data;
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  P3->OUT |= BIT0;
}

void spi_read(uint8_t addr, uint8_t * data)
{
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  P3->OUT &= ~(BIT0);
  EUSCI_B0->TXBUF = BIT7 | addr;
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  EUSCI_B0->TXBUF = 0;
  while(EUSCI_B0->STATW & EUSCI_B_STATW_SPI_BUSY);
  P3->OUT |= BIT0;
  *data = EUSCI_B0->RXBUF;
}
