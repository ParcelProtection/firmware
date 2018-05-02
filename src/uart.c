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
#include "circbuf.h"
#include "helpers.h"
#include "packets.h"
#include "uart.h"

void uart_init(cb_t ** ptr_uart_rx_buf)
{
  /* initialize RX buffer */
  if(cb_init(ptr_uart_rx_buf, UART_RX_BUF_LEN, sizeof(uint8_t)) != CB_SUCCESS) while(1);

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

void uart_send_blocking(uint8_t uart_num, uint8_t data)
{
  if(uart_num == 0)
  {
    /* wait for UART to be idle */
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    EUSCI_A0->TXBUF = data;
  }
  else if(uart_num == 1)
  {
    /* wait for UART to be idle */
    while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));

    EUSCI_A2->TXBUF = data;
  }
}

void uart_send_n_blocking(uint8_t uart_num, uint8_t * ptr_data, uint8_t len)
{
  uint8_t i;
  for(i = 0; i < len; i++)
  {
    uart_send_blocking(uart_num, ptr_data[i]);
  }
}

void uart_send_str_blocking(uint8_t uart_num, uint8_t * data)
{
  uint32_t i;
  if(uart_num == 0)
  {
    for(i = 0; *(data + i); i++)
    {
      /* wait for UART to be idle */
      while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

      EUSCI_A0->TXBUF = *(data + i);
    }
  }
  else if(uart_num == 1)
  {
    for(i = 0; *(data + i); i++)
    {
      /* wait for UART to be idle */
      while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));

      EUSCI_A2->TXBUF = *(data + i);
    }
  }
}

void uart_send_int_blocking(uint8_t uart_num, int32_t data)
{
  uint8_t conversion_buf[12];
  my_itoa(data, conversion_buf, BASE_16);
  uart_send_str_blocking(uart_num, conversion_buf);
}

void uart_send_pkt(uint8_t uart_num, pkt_t * ptr_pkt)
{
  uart_send_blocking(uart_num, ptr_pkt->type);
  uart_send_blocking(uart_num, ptr_pkt->pkt_len);

  uint8_t i;
  for(i = 0; i < ptr_pkt->pkt_len; i++)
  {
    uart_send_blocking(uart_num, ptr_pkt->ptr_pkt[i]);
  }

  uart_send_blocking(uart_num, ptr_pkt->checksum);
}
