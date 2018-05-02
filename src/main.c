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
#include <stddef.h>
#include "adxl345.h"
#include "circbuf.h"
#include "event_buf.h"
#include "helpers.h"
#include "packets.h"
#include "rtc.h"
#include "spi.h"
#include "uart.h"

#define ACC_FLIP_THRESH (0x400)
#define ACC_TAP_THRESH (0xFF)
#define ACC_TAP_TIME (0x40)

static eb_t * ptr_event_buf = NULL;
static cb_t * ptr_uart_rx_buf = NULL;
static dev_status_e dev_status = STATUS_UNINITIALIZED;
static uint16_t package_id;
static uint8_t carrier_access_code;
static uint8_t user_access_code;
static uint8_t track_drops_f;
static uint8_t track_flips_f;
static uint8_t tracking_len;
static uint8_t * tracking = NULL;
uint8_t pkt_received_f = 0;


/* Initialization Functions */

void gpio_init()
{
  P4->SEL0 &= !(BIT5 | BIT4 | BIT0); /* GPIO mode */
  P4->SEL1 &= !(BIT5 | BIT4 | BIT0);
  P4->DIR &= !(BIT5 | BIT4 | BIT0); /* configure as inputs */
  P4->REN |= BIT0; /* enable pullup/down resistor */
  P4->OUT |= BIT0; /* pullup resistor */
  P4->IES &= ~(BIT5 | BIT4); /* interrupt on low to high transition */
  P4->IES |= BIT0; /* interrupt on high to low transition */
  P4->IE |= BIT4; /* enable interrupts */
  NVIC_EnableIRQ(PORT4_IRQn);
  __enable_interrupts();
}

void adxl_init()
{
  spi_write(ADXL_POWER_CTL, 0x08); /* enable measurements */
  spi_write(ADXL_INT_ENABLE, 0x00); /* disable interrupts */
  spi_write(ADXL_INT_MAP, 0x00); /* map all interrupts to INT1 */
  spi_write(ADXL_THRESH_TAP, ACC_TAP_THRESH); /* set tap threshold */
  spi_write(ADXL_DUR, ACC_TAP_TIME); /* set tap time */
  spi_write(ADXL_TAP_AXES, 0x07); /* enable tap detection for all axes */
  spi_write(ADXL_DATA_FORMAT, 0x00); /* set range to 2G */
  spi_write(ADXL_INT_ENABLE, 0x40); /* enable tap interrupts */
  spi_read(ADXL_INT_SOURCE); /* clear all interrupts */
}


/* Packet Sending Functions */

void send_ack_pkt(ack_e ack);

void send_status_pkt();

void send_dump_pkt();


/* Interrupt Handlers */

void PORT4_IRQHandler()
{
  if(P4->IFG & BIT0)
  {
    P4->IFG &= ~(BIT0);
  }
  if(P4->IFG & BIT4)
  {
    /* clear interrupt */
    P4->IFG &= ~(BIT4);
    spi_read(ADXL_INT_SOURCE);

    if(track_drops_f) eb_add_event(ptr_event_buf, EVENT_DROP);
  }
  if(P4->IFG & BIT5)
  {
    P4->IFG &= ~(BIT5);
  }
}

void EUSCIA2_IRQHandler();


/* Main */

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; /* stop watchdog timer */

  uart_init(&ptr_event_buf);
  spi_init();
  gpio_init();
  adxl_init();

  /* main control loop */
  uint8_t val;
  int16_t acc_z;
  uint32_t flip_count = 0;

  while(1)
  {
    /* sense flips */
    if(track_flips_f)
    {
      acc_z = adxl_get_z();
      if(acc_z > 0)
      {
        flip_count++;
        if(flip_count > ACC_FLIP_THRESH)
        {
          eb_add_event(ptr_event_buf, EVENT_FLIP);
        }
      }
      else
      {
        flip_count = 0;
      }
    } /* if(track_flips_f) */

    /* handle received packets */
    if(pkt_received_f)
    {
      /* pull packets from RX buffer */

      /* handle packets */

    } /* if(pkt_received_f) */
  }
}
