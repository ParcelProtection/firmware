/**
 * @file uart.h
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
#ifndef __UART_H__
#define __UART_H__

#include "circbuf.h"
#include "packets.h"

#define UART_RX_BUF_LEN (64)
#define UART_NUM_LOG (0)
#define UART_NUM_BT (1)

/**
 * @brief initializes UART
 *
 * UART0 is the on-board UART, initialized to 9600 BAUD
 * UARTi is the Bluetooth UART, initialized to 9600 BAUD
 *
 * @param ptr_uart_rx_buf The RX buffer to initialize
 *
 * @return none
 */
void uart_init(cb_t ** ptr_uart_rx_buf);

/**
 * @brief sends a byte over UART
 *
 * @param uart_num 0 or 1
 * @param data The byte to send
 *
 * @return none
 */
void uart_send_blocking(uint8_t uart_num, uint8_t data);

/**
 * @brief sends a block of data over UART
 *
 * @param uart_num 0 or 1
 * @param ptr_data A pointer to the block to send
 * @param len The length of the block in bytes
 *
 * @return none
 */
void uart_send_n_blocking(uint8_t uart_num, uint8_t * ptr_data, uint8_t len);

/**
 * @brief sends a string over UART
 *
 * @param uart_num 0 or 1
 * @param data The string to send
 *
 * @return none
 */
void uart_send_str_blocking(uint8_t uart_num, uint8_t * data);

/**
 * @brief sends an integer over UART
 *
 * @param uart_num 0 or 1
 * @param data The integer to send
 *
 * @return none
 */
void uart_send_int_blocking(uint8_t uart_num, int32_t data);

/**
 * @brief sends a packet over UART
 *
 * @param uart_num 0 or 1
 * @param ptr_pkt A pointer to the packet
 *
 * @return none
 */
void uart_send_pkt(uint8_t uart_num, pkt_t * ptr_pkt);

/**
 * @brief sends a byte over the on-chip UART
 *
 * @param data The byte to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void log_send(uint8_t data)
{
  uart_send_blocking(UART_NUM_LOG, data);
}

/**
 * @brief sends a block of data over the on-chip UART
 *
 * @param data The block to send
 * @param len The length of the block in bytes
 *
 * @return none
 */
__attribute__((always_inline)) inline void log_send_n(uint8_t * ptr_data, uint8_t len)
{
  uart_send_n_blocking(UART_NUM_LOG, ptr_data, len);
}

/**
 * @brief sends a string over the on-chip UART
 *
 * @param data The string to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void log_send_str(uint8_t * data)
{
  uart_send_str_blocking(UART_NUM_LOG, data);
}

/**
 * @brief sends an integer over the on-chip UART
 *
 * @param data The integer to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void log_send_int(int32_t data)
{
  uart_send_int_blocking(UART_NUM_LOG, data);
}

/**
 * @brief sends a packet over the on-chip UART
 *
 * @param ptr_pkt A pointer to the packet
 *
 * @return none
 */
__attribute__((always_inline)) inline void log_send_pkt(pkt_t * ptr_pkt)
{
  uart_send_pkt(UART_NUM_LOG, ptr_pkt);
}

/**
 * @brief sends a byte over Bluetooth
 *
 * @param data The byte to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void bt_send(uint8_t data)
{
  uart_send_blocking(UART_NUM_BT, data);
}

/**
 * @brief sends a block of data over Bluetooth
 *
 * @param data The block to send
 * @param len The length of the block in bytes
 *
 * @return none
 */
__attribute__((always_inline)) inline void bt_send_n(uint8_t * ptr_data, uint8_t len)
{
  uart_send_n_blocking(UART_NUM_BT, ptr_data, len);
}

/**
 * @brief sends a string over Bluetooth
 *
 * @param data The string to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void bt_send_str(uint8_t * data)
{
  uart_send_str_blocking(UART_NUM_BT, data);
}

/**
 * @brief sends an integer over Bluetooth
 *
 * @param data The integer to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void bt_send_int(int32_t data)
{
  uart_send_int_blocking(UART_NUM_BT, data);
}

/**
 * @brief sends a packet over Bluetooth
 *
 * @param ptr_pkt A pointer to the packet
 *
 * @return none
 */
__attribute__((always_inline)) inline void bt_send_pkt(pkt_t * ptr_pkt)
{
  uart_send_pkt(UART_NUM_BT, ptr_pkt);
}

#endif /* __UART_H__ */
