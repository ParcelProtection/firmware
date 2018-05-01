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

#endif /* __HELPERS_H__ */
