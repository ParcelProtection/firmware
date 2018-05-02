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

#define UART_RX_BUF_LEN (64)

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
 * @brief sends a byte over UART0
 *
 * @param data The byte to send
 *
 * @return none
 */
void uart0_send(uint8_t data);

/**
 * @brief sends a string over UART0
 *
 * @param data The string to send
 *
 * @return none
 */
void uart0_send_str(uint8_t * data);

/**
 * @brief sends a byte over UART1
 *
 * @param data The byte to send
 *
 * @return none
 */
void uart1_send(uint8_t data);

/**
 * @brief sends a string over UART1
 *
 * @param data The string to send
 *
 * @return none
 */
void uart1_send_str(uint8_t * data);

/**
 * @brief sends an integer over UART1
 *
 * @param data The integer to send
 *
 * @return none
 */
void uart1_send_int(int32_t data);

/**
 * @brief sends a byte over the on-chip UART
 *
 * @param data The byte to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void uart_send(uint8_t data)
{
  uart0_send(data);
}

/**
 * @brief sends a string over the on-chip UART
 *
 * @param data The string to send
 *
 * @return none
 */
__attribute__((always_inline)) inline void uart_send_str(uint8_t * data)
{
  uart0_send_str(data);
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
  uart1_send(data);
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
  uart1_send_str(data);
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
  uart1_send_int(data);
}

#endif /* __UART_H__ */
