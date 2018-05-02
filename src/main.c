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
#include <stdlib.h>
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
static uint8_t track_drops_f = 0;
static uint8_t track_flips_f = 0;
static uint8_t tracking_len;
static uint8_t * tracking = NULL;
uint8_t pkts_received = 0;


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
  P4->IE |= BIT4; /* enable interrupt generation */
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

void begin_tracking()
{
  if(track_drops_f)
  {
    NVIC_EnableIRQ(PORT4_IRQn);
    __enable_interrupts();
  }
  dev_status = STATUS_TRACKING;
}


/* Packet Sending Functions */

void send_ack_pkt(ack_e ack)
{
  pkt_t pkt;
  pkt.type = (ack == ACK) ? PKT_RES_ACK : PKT_RES_NAK;
  pkt.checksum = pkt.type;
  pkt.pkt_len = 0;
  pkt.checksum ^= pkt.pkt_len;

  bt_send_pkt(&pkt);
}

void send_status_pkt()
{
}

void send_dump_pkt()
{
}


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
  ack_e ack;
  uint8_t pkt_type, pkt_len, pkt_crc, crc_check;
  uint8_t * pkt = NULL;
  int16_t acc_z;
  uint32_t flip_count = 0, i;
  cmd_init_t * ptr_init_pkt = NULL;

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
    if(pkts_received)
    {
      pkts_received--;

      /* first byte is the packet type */
      cb_remove_item(ptr_uart_rx_buf, &pkt_type);
      crc_check = pkt_type;

      /* second byte is the payload len */
      cb_remove_item(ptr_uart_rx_buf, &pkt_len);
      crc_check ^= pkt_len;

      /* get payload */
      pkt = (uint8_t *)malloc(pkt_len);
      for(i = 0; i < pkt_len; i++)
      {
        cb_remove_item(ptr_uart_rx_buf, &pkt[i]);
        crc_check ^= pkt[i];
      }

      /* get and check crc */
      cb_remove_item(ptr_uart_rx_buf, &pkt_crc);
#ifdef CRC_CHECK
      ack = (crc_check == pkt_crc) ? ACK : NAK;
#else
      ack = ACK;
#endif /* CRC_CHECK */
      
      /* handle packets */
      if(ack == ACK)
      {
        switch(pkt_type)
        {
          case PKT_CMD_STATUS:
            send_status_pkt();
            break;
          case PKT_CMD_INIT:
            ptr_init_pkt = (cmd_init_t *)pkt;

            /* populate package parameters */
            package_id = ptr_init_pkt->package_id;
            carrier_access_code = ptr_init_pkt->carrier_access_code;
            user_access_code = ptr_init_pkt->user_access_code;
            track_drops_f = ptr_init_pkt->track_drops;
            track_flips_f = ptr_init_pkt->track_flips;
            tracking_len = ptr_init_pkt->tracking_len;

            if(tracking) free( (void *)tracking );
            tracking = (uint8_t *)malloc(tracking_len);
            memcpy(tracking, &ptr_init_pkt->tracking, tracking_len);

            rtc_init(ptr_init_pkt->time);

            begin_tracking();
            send_ack_pkt(ack);
            break;
          case PKT_CMD_DUMP:
            send_dump_pkt();
            break;
          default:
            send_ack_pkt(NAK);
            break;
        } /* switch(pkt_type) */
      }
      else
      {
        send_ack_pkt(ack);
      } /* if(ack == ACK) */

      free( (void *)pkt );

    } /* if(pkt_received_f) */
  }
}
