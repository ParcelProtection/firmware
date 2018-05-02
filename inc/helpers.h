/**
 * @file helpers.h
 * @brief Helper functions and macros
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */
#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "msp.h"

#define UNUSED(x) if(x)

#define BEGIN_CRITICAL_SECTION() __disable_irq()
#define END_CRITICAL_SECTION() __enable_irq()

#define BASE_2 (2)
#define BASE_8 (8)
#define BASE_10 (10)
#define BASE_16 (16)

/**
 * @brief Reverses a block of memory
 *
 * Given a pointer to a source memory location and its size,
 * reverses the order of the bytes in the block
 *
 * @param src Pointer to the source block
 * @param length The length of the memory block
 *
 * @return A pointer to the source memory block
 */
uint8_t * my_reverse(uint8_t * src, uint32_t length);

/**
 * @brief Converts integers to ASCII strings
 *
 * Converts an integer of a given base to an ASCII string,
 * and stores the string at a given address.
 *
 * @param data The integer to convert
 * @param ptr Pointer to the location for the string
 * @param base The base of the given integer
 *
 * @return The length of the ASCII string
 */
uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base);

#endif /* __HELPERS_H__ */
