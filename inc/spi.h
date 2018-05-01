/**
 * @file spi.h
 * @brief SPI functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * EUSCI_B0 - SPI to accelerometer
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */
#ifndef __SPI_H__
#define __SPI_H__

/**
 * @brief initializes SPI
 *
 * @return none
 */
void spi_init();

/**
 * @brief Writes to a register on a SPI device
 *
 * @param addr The address of the register
 * @param data The byte to write to the register
 *
 * @return none
 */
void spi_write(uint8_t addr, uint8_t data);

/**
 * @brief Reads from a register on a SPI device
 *
 * @param addr The address of the register
 * @param data A pointer to store the returned data
 *
 * @return none
 */
void spi_read(uint8_t addr, uint8_t * data);

#endif /* __SPI_H__ */
