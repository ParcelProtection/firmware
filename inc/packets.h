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

#include "event_buf.h"

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
                   0x82 - dump response */
  uint8_t pkt_len;
  uint8_t * ptr_pkt;
  uint8_t checksum; /* running XOR of all bytes in packet */
} pkt_t;

/*
 * @brief Initialization command structure
 */
typedef struct
{
  uint8_t package_id; /* internal package id */
  uint8_t reserved_0[3];
  int32_t epoch_time; /* current time */
  uint8_t carrier_access_code; /* access code for carriers */
  uint8_t user_access_code; /* access code for users */
  uint8_t track_drops :1; /* track package drops; 0: false, 1: true */
  uint8_t track_flips :1; /* track package flips; 0: false, 1: true */
  uint32_t reserved :6;
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
  uint8_t package_id; /* internal package id */
  uint8_t status_code; /* 0x00 - uninitialized
                          0x01 - initialized and tracking
                          0x02 - error */
} res_status_t;

/*
 * @brief Dump response structure
 */
typedef struct
{
  uint8_t package_id; /* internal package id */
  uint8_t reserved[3];
  uint32_t num_events; /* number of events in log */
  event_t * events; /* events */
} res_dump_t;

/*
 * @brief Acknowledge response structure
 */
typedef struct
{
  uint8_t acknowledge; /* 0x01 to acknowledge */
} res_ack_t;

#ifndef __EVENT_BUF_H__
/*
 * @brief Event structure
 */
typedef struct
{
  uint8_t event_type; /* 0x00 - drop
                         0x01 - flip */
  uint8_t reserved[3];
  uint32_t time; /* time of event */
  uint32_t data; /* extra data (acceleration value) */
} event_t;
#endif /* __EVENT_BUF_H__ */

#endif /* __PACKETS_H__ */
