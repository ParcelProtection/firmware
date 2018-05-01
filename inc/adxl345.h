/**
 * @file adxl345.h
 * @brief Functions and macros for the ADXL345 board
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */
#ifndef __ADXL345_H__
#define __ADXL345_H__

#include "spi.h"

/* device registers */
#define ADDR_DEVID          (0x00)
#define ADDR_THRESH_TAP     (0x1D)
#define ADDR_OFSX           (0x1E)
#define ADDR_OFSY           (0x1F)
#define ADDR_OFSZ           (0x20)
#define ADDR_DUR            (0x21)
#define ADDR_LATENT         (0x22)
#define ADDR_WINDOW         (0x23)
#define ADDR_THRESH_ACT     (0x24)
#define ADDR_THRESH_INACT   (0x25)
#define ADDR_TIME_INACT     (0x26)
#define ADDR_ACT_INACT_CTL  (0x27)
#define ADDR_THRESH_FF      (0x28)
#define ADDR_TIME_FF        (0x29)
#define ADDR_TAP_AXES       (0x2A)
#define ADDR_ACT_TAP_STATUS (0x2B)
#define ADDR_BW_RATE        (0x2C)
#define ADDR_POWER_CTL      (0x2D)
#define ADDR_INT_ENABLE     (0x2E)
#define ADDR_INT_MAP        (0x2F)
#define ADDR_INT_SOURCE     (0x30)
#define ADDR_DATA_FORMAT    (0x31)
#define ADDR_DATAX0         (0x32)
#define ADDR_DATAX1         (0x33)
#define ADDR_DATAY0         (0x34)
#define ADDR_DATAY1         (0x35)
#define ADDR_DATAZ0         (0x36)
#define ADDR_DATAZ1         (0x37)
#define ADDR_FIFO_CTL       (0x38)
#define ADDR_FIFO_STATUS    (0x39)

/**
 * @brief get the device ID
 *
 * @return device ID
 */
__attribute__((always_inline)) inline uint8_t adxl_get_dev_id()
{
  return spi_read(ADDR_DEVID);
}

/**
 * @brief get x-axis
 *
 * @return x-axis
 */
__attribute__((always_inline)) inline int16_t adxl_get_x()
{
  return spi_read_double(ADDR_DATAX0);
}

/**
 * @brief get y-axis
 *
 * @return y-axis
 */
__attribute__((always_inline)) inline int16_t adxl_get_y()
{
  return spi_read_double(ADDR_DATAY0);
}

/**
 * @brief get z-axis
 *
 * @return z-axis
 */
__attribute__((always_inline)) inline int16_t adzl_get_x()
{
  return spi_read_double(ADDR_DATAZ0);
}

#endif /* __ADXL345_H__ */
