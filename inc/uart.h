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

/**
 * @brief initializes UART
 *
 * UART0 is the on-board UART, initialized to 9600 BAUD
 * UARTi is the Bluetooth UART, initialized to 9600 BAUD
 *
 * @return none
 */
void uart_init();

/**
 * @brief sends a byte over UART0
 *
 * @param data The byte to send
 *
 * @return none
 */
void uart0_send(uint8_t data);

/**
 * @brief sends a byte over UART1
 *
 * @param data The byte to send
 *
 * @return none
 */
void uart1_send(uint8_t data);

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

#endif /* __UART_H__ */
