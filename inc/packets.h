/**
 * @file packets.h
 * @brief packets to communicate with app
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/05/01
 */

#ifndef __PACKETS_H__
#define __PACKETS_H__

#include "msp.h"

/*
 * @brief Device status
 */
typedef enum
{
  STATUS_UNINITIALIZED = 0,
  STATUS_TRACKING,
  STATUS_ERROR
} dev_status_e;

/*
 * @brief Packet type
 */
typedef enum
{
  PKT_CMD_STATUS = 0x00,
  PKT_CMD_INIT,
  PKT_CMD_DUMP,
  PKT_RES_ACK = 0x80,
  PKT_RES_STATUS,
  PKT_RES_DUMP,
  PKT_RES_NAK = 0x8F
} pkt_type_e;

/*
 * @brief Event type
 */
typedef enum
{
  EVENT_DROP = 0,
  EVENT_FLIP
} event_type_e;

/*
 * @brief ACK
 */
typedef enum
{
  NAK = 0,
  ACK
} ack_e;

/*
 * @brief time structure
 */
typedef struct
{
  uint16_t year;
  uint8_t month;
  uint8_t dow; /* day of the week */
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} rtc_t;

/*
 * @brief Event structure
 */
typedef struct
{
  uint8_t event_type; /* 0x00 - drop
                         0x01 - flip */
  uint8_t reserved[3];
  rtc_t time; /* time of event */
  uint32_t data; /* extra data (acceleration value) */
} event_t;

/*
 * @brief Initialization command structure
 */
typedef struct
{
  uint16_t package_id; /* internal package id */
  uint8_t reserved_0[2];
  rtc_t time; /* current time */
  uint8_t carrier_access_code; /* access code for carriers */
  uint8_t user_access_code; /* access code for users */
  uint8_t track_drops :1; /* track package drops; 0: false, 1: true */
  uint8_t track_flips :1; /* track package flips; 0: false, 1: true */
  uint32_t reserved_1 :6;
  uint8_t tracking_len; /* length of tracking number in bytes */
  uint8_t * tracking; /* tracking number */
} cmd_init_t;

/*
 * @brief Dump command structure
 */
typedef struct
{
  uint8_t access_code; /* carrier or user, determines what data to dump */
} cmd_dump_t;

/*
 * @brief Status response structure
 */
typedef struct
{
  uint16_t package_id; /* internal package id */
  uint8_t status_code; /* 0x00 - uninitialized
                          0x01 - initialized and tracking
                          0x02 - error */
} res_status_t;

/*
 * @brief Dump response structure
 */
typedef struct
{
  uint16_t package_id; /* internal package id */
  uint8_t num_events; /* number of events in log */
  uint8_t reserved;
  event_t * events; /* events */
} res_dump_t;

/*
 * @brief Acknowledge response structure
 */
typedef struct
{
  uint8_t acknowledge; /* 0x01 to acknowledge */
} res_ack_t;

/*
 * @brief Packet structure
 */
typedef struct
{
  uint8_t type; /* 0x00 - status command
                   0x01 - init command
                   0x02 - dump command
                   0x80 - acknowledge
                   0x81 - status response
                   0x82 - dump response 
                   0x8F - non-acknowledge */
  uint8_t pkt_len;
  uint8_t * ptr_pkt;
  uint8_t checksum; /* running XOR of all bytes in packet */
} pkt_t;

#endif /* __PACKETS_H__ */
