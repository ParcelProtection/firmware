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

/* functionality switches */
#undef CRC_CHECK
#undef AUTH_CHECK
#undef TESTING
#undef APP_TESTING
#define DEMO

typedef enum
{
  AUTH_UNAUTH,
  AUTH_USER,
  AUTH_CARRIER
} auth_e;

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
  P4->SEL0 &= ~(BIT5 | BIT4 | BIT0); /* GPIO mode */
  P4->SEL1 &= ~(BIT5 | BIT4 | BIT0);
  P4->DIR &= ~(BIT5 | BIT4 | BIT0); /* configure as inputs */
  P4->REN |= BIT0; /* enable pullup/down resistor */
  P4->OUT |= BIT0; /* pullup resistor */
  P4->IES &= ~(BIT5 | BIT4); /* interrupt on low to high transition */
  P4->IES |= BIT0; /* interrupt on high to low transition */
  P4->IE |= BIT4; /* enable interrupt generation */

#ifdef TESTING
  P1->SEL0 &= ~(BIT4 | BIT1); /* GPIO mode */
  P1->SEL1 &= ~(BIT4 | BIT1);
  P1->DIR &= ~(BIT4 | BIT1); /* configure as inputs */
  P1->REN |= BIT4 | BIT1; /* enable pullup/down resistor */
  P1->OUT |= BIT4 | BIT1; /* pullup resistor */
  P1->IES |= BIT4 | BIT1; /* interrupt on high to low transition */
  P1->IE |= BIT4 | BIT1; /* enable interrupt generation */
  P1->IFG = 0; /* clear interrupts */
  NVIC_EnableIRQ(PORT1_IRQn);
  __enable_interrupts();
#endif /* TESTING */
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
    spi_read(ADXL_INT_SOURCE); /* clear interrupts */
    P4->IFG &= ~(BIT4);
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
  pkt_t pkt;
  pkt.type = PKT_RES_STATUS;
  pkt.checksum = pkt.type;
  pkt.pkt_len = sizeof(res_status_t);
  pkt.checksum ^= pkt.pkt_len;

  res_status_t payload;
  payload.package_id = package_id;
  payload.status_code = dev_status;

  uint8_t i;
  for(i = 0; i < pkt.pkt_len; i++)
  {
    pkt.checksum ^= *((uint8_t *)(&payload) + i);
  }

  pkt.ptr_pkt = (uint8_t *)&payload;

  bt_send_pkt(&pkt);
}

void send_dump_pkt(auth_e auth)
{
  /* return a NAK if not authorized */
  if(auth == AUTH_UNAUTH)
  {
    send_ack_pkt(NAK);
    return;
  }

  uint8_t crc, data, pkt_len;
  uint32_t count, i, j;
  event_t event;
  res_dump_t payload;

  /* send header */
  bt_send(PKT_RES_DUMP);
  crc = PKT_RES_DUMP;

  eb_get_count(ptr_event_buf, &count);
  pkt_len = sizeof(res_dump_t) - sizeof(event_t *) + count * sizeof(event_t);
  bt_send(pkt_len);
  crc ^= pkt_len;

  /* send payload */
  bt_send(package_id);
  crc ^= package_id;
  bt_send(package_id >> 8);
  crc ^= package_id >> 8;
  bt_send(count);
  crc ^= count;
  bt_send(0x00);

  /* send events */
  for(i = 0; i < count; i++)
  {
    eb_seek_item(ptr_event_buf, &event, i);
    for(j = 0; j < sizeof(event_t); j++)
    {
      data = *((uint8_t *)&event + j);
      bt_send(data);
      crc ^= data;
    }
  }

  bt_send(crc);
}


/* Testing Functions */

#if defined TESTING | defined DEMO
void add_mock_events()
{
  rtc_t mock_time;
  mock_time.year = 2018;
  mock_time.month = 4;
  mock_time.dow = 0;
  mock_time.day = 22;
  mock_time.hour = 19;
  mock_time.minute = 40;
  mock_time.second = 17;

  rtc_init(mock_time);
  eb_new_event(ptr_event_buf, EVENT_FLIP, 0);

  mock_time.dow = 2;
  mock_time.day = 24;
  mock_time.hour = 8;
  mock_time.minute = 44;
  mock_time.second = 36;

  rtc_init(mock_time);
  eb_new_event(ptr_event_buf, EVENT_DROP, 0);

  mock_time.hour = 10;
  mock_time.minute = 48;
  mock_time.second = 11;

  rtc_init(mock_time);
  eb_new_event(ptr_event_buf, EVENT_FLIP, 0);

  mock_time.hour = 17;
  mock_time.minute = 5;
  mock_time.second = 57;

  rtc_init(mock_time);
  eb_new_event(ptr_event_buf, EVENT_DROP, 0);
}
#endif /* TESTING */


/* Interrupt Handlers */

#ifdef TESTING
void PORT1_IRQHandler()
{
  if(P1->IFG & BIT1)
  {
    /* artificially initialize */

    /* populate package parameters */
    package_id = 0xBEEF;
    carrier_access_code = 0x8A;
    user_access_code = 0xB2;
    track_drops_f = 1;
    track_flips_f = 1;
    tracking_len = 18;

    if(tracking) free( (void *)tracking );
    tracking = (uint8_t *)malloc(tracking_len);
    memcpy(tracking, "1ZA807T70336134832", tracking_len);

    rtc_t rtc;
    rtc.year = 2018;
    rtc.month = 5;
    rtc.dow = 4;
    rtc.day = 3;
    rtc.hour = 10;
    rtc.minute = 0;
    rtc.second = 0;
    rtc_init(rtc);

    begin_tracking();
    send_ack_pkt(ACK);

    /* debounce */
    uint32_t i;
    for(i = 0; i < 1000; i++);

    P1->IFG &= ~BIT1;
  }
  if(P1->IFG & BIT4)
  {
    /* spoof valid dump command */
    cb_clear(ptr_uart_rx_buf);
    uint8_t data = 0x02;
    cb_add_item(ptr_uart_rx_buf, &data);
    data = 0x01;
    cb_add_item(ptr_uart_rx_buf, &data);
    data = 0x8A;
    cb_add_item(ptr_uart_rx_buf, &data);
    data = 0x02 ^ 0x01 ^ 0x8A;
    cb_add_item(ptr_uart_rx_buf, &data);
    pkts_received++;

    /* debounce */
    uint32_t i;
    for(i = 0; i < 1000; i++);

    P1->IFG &= ~BIT4;
  }
}
#endif /* TESTING */

void PORT4_IRQHandler()
{
  if(P4->IFG & BIT0)
  {
    P4->IFG &= ~(BIT0);
  }
  if(P4->IFG & BIT4)
  {
    /* clear interrupt */
    uint32_t i;
    for(i = 0; i < 10000; i++);
    spi_read(ADXL_INT_SOURCE);
    P4->IFG &= ~(BIT4);

    if(track_drops_f) eb_new_event(ptr_event_buf, EVENT_DROP, 0);
  }
  if(P4->IFG & BIT5)
  {
    P4->IFG &= ~(BIT5);
  }
}

void EUSCIA2_IRQHandler()
{
  /* clear interrupt */
  EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;

#ifdef APP_TESTING
  uint8_t data = EUSCI_A2->RXBUF;

  bt_send(data);
#else
  static uint8_t mid_pkt = 0, byte_count, payload_len;

  uint8_t data = EUSCI_A2->RXBUF;
  cb_add_item(ptr_uart_rx_buf, &data);

  /* do a bit of processing to simplify main */
  if(!mid_pkt)
  {
    mid_pkt = 1;
    byte_count = 1;
  }
  else if(byte_count == 1)
  {
    payload_len = data;
    byte_count++;
  }
  else if(byte_count == 2 + payload_len)
  {
    mid_pkt = 0;
    pkts_received++;
  }
  else
  {
    byte_count++;
  }
#endif /* APP_TESTING */
}


/* Main */

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; /* stop watchdog timer */

  eb_init(&ptr_event_buf);
  uart_init(&ptr_uart_rx_buf);
  spi_init();
  gpio_init();
  adxl_init();

#if defined TESTING || defined DEMO
  add_mock_events();
#endif

#ifdef APP_TESTING
  while(1)
  {
    bt_send('A');
    uint32_t i;
    for(i = 0; i < 10000; i++);
  }
#endif

  /* main control loop */
  ack_e ack;
  auth_e auth;
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
        if(flip_count == ACC_FLIP_THRESH)
        {
          eb_new_event(ptr_event_buf, EVENT_FLIP, 0);
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
#ifdef AUTH_CHECK
            auth = ( ((cmd_dump_t *)pkt)->access_code == carrier_access_code ) ? AUTH_CARRIER :
                   ( ((cmd_dump_t *)pkt)->access_code == user_access_code ) ? AUTH_USER : AUTH_UNAUTH;
#else
            auth = AUTH_CARRIER;
#endif /* AUTH_CHECK */
            send_dump_pkt(auth);
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
