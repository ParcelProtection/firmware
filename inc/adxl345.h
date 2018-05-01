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
#define ADXL_DEVID          (0x00)
#define ADXL_THRESH_TAP     (0x1D)
#define ADXL_OFSX           (0x1E)
#define ADXL_OFSY           (0x1F)
#define ADXL_OFSZ           (0x20)
#define ADXL_DUR            (0x21)
#define ADXL_LATENT         (0x22)
#define ADXL_WINDOW         (0x23)
#define ADXL_THRESH_ACT     (0x24)
#define ADXL_THRESH_INACT   (0x25)
#define ADXL_TIME_INACT     (0x26)
#define ADXL_ACT_INACT_CTL  (0x27)
#define ADXL_THRESH_FF      (0x28)
#define ADXL_TIME_FF        (0x29)
#define ADXL_TAP_AXES       (0x2A)
#define ADXL_ACT_TAP_STATUS (0x2B)
#define ADXL_BW_RATE        (0x2C)
#define ADXL_POWER_CTL      (0x2D)
#define ADXL_INT_ENABLE     (0x2E)
#define ADXL_INT_MAP        (0x2F)
#define ADXL_INT_SOURCE     (0x30)
#define ADXL_DATA_FORMAT    (0x31)
#define ADXL_DATAX0         (0x32)
#define ADXL_DATAX1         (0x33)
#define ADXL_DATAY0         (0x34)
#define ADXL_DATAY1         (0x35)
#define ADXL_DATAZ0         (0x36)
#define ADXL_DATAZ1         (0x37)
#define ADXL_FIFO_CTL       (0x38)
#define ADXL_FIFO_STATUS    (0x39)

#define ADXL_INT_OVERRUN    (1 << 0)
#define ADXL_INT_WATERMARK  (1 << 1)
#define ADXL_INT_FREE_FALL  (1 << 2)
#define ADXL_INT_INACTIVITY (1 << 3)
#define ADXL_INT_ACTIVITY   (1 << 4)
#define ADXL_INT_DOUBLE_TAP (1 << 5)
#define ADXL_INT_SINGLE_TAP (1 << 6)
#define ADXL_INT_DATA_READY (1 << 7)

#define ADXL_ACT_INACT_CTRL_ACT_AC_DC (1 << 7)
#define ADXL_ACT_INACT_CTRL_ACT_X (1 << 6)
#define ADXL_ACT_INACT_CTRL_ACT_Y (1 << 5)
#define ADXL_ACT_INACT_CTRL_ACT_Z (1 << 4)
#define ADXL_ACT_INACT_CTRL_INACT_AC_DC (1 << 3)
#define ADXL_ACT_INACT_CTRL_INACT_X (1 << 2)
#define ADXL_ACT_INACT_CTRL_INACT_Y (1 << 1)
#define ADXL_ACT_INACT_CTRL_INACT_Z (1 << 0)

/**
 * @brief get the device ID
 *
 * @return device ID
 */
__attribute__((always_inline)) inline uint8_t adxl_get_dev_id()
{
  return spi_read(ADXL_DEVID);
}

/**
 * @brief get x-axis
 *
 * @return x-axis
 */
__attribute__((always_inline)) inline int16_t adxl_get_x()
{
  return spi_read_double(ADXL_DATAX0);
}

/**
 * @brief get y-axis
 *
 * @return y-axis
 */
__attribute__((always_inline)) inline int16_t adxl_get_y()
{
  return spi_read_double(ADXL_DATAY0);
}

/**
 * @brief get z-axis
 *
 * @return z-axis
 */
__attribute__((always_inline)) inline int16_t adxl_get_z()
{
  return spi_read_double(ADXL_DATAZ0);
}

/**
 * @brief disable interrupts
 *
 * @return none
 */
__attribute__((always_inline)) inline void adxl_disable_interrupts()
{
  spi_write(ADXL_INT_ENABLE, 0x00);
}

#endif /* __ADXL345_H__ */
