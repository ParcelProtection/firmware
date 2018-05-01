/**
 * @file circbuf.h
 * @brief Circular buffer functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */
#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

/**
 * @brief circular buffer structure
 */
typedef struct
{
  void * base;
  void * head;
  void * tail;
  uint32_t size;
  uint32_t count;
  uint32_t item_size;
} cb_t;

/**
 * Circular buffer status code
 */
typedef enum
{
  CB_SUCCESS,
  CB_NULL_PTR,
  CB_INVALID_PARAM,
  CB_MALLOC_FAILED,
  CB_FULL,
  CB_EMPTY
} cb_e;

/**
 * @brief Initialize circular buffer
 * 
 * Allocate memory for circular buffer and setup the structure
 * 
 * @param ptr_buf A pointer to the circular buffer pointer to be allocated
 * @param len The length of the buffer to allocate in memory
 * @param item_size The size of each item in the buffer
 *
 * @return A circular buffer status code
 */
cb_e cb_init(cb_t ** ptr_buf, uint32_t len, uint32_t item_size);

/**
 * @brief Destroy circular buffer
 *
 * Free memory used by circular buffer.
 * 
 * @param ptr_buf A pointer to the buffer to be freed
 *
 * @return A circular buffer status code
 */
cb_e cb_free(cb_t ** ptr_buf);

/**
 * @brief Add item to circular buffer
 * 
 * Add data to a circular buffer
 * 
 * @param buf Pointer to the circular buffer
 * @param ptr_data Data to add to buffer
 *
 * @return A circular buffer status code
 */
cb_e cb_add_item(cb_t * buf, void * ptr_data);

/**
 * @brief Remove item from circular buffer
 *
 * Remove data from a circular buffer
 * 
 * @param buf Pointer to the circular buffer
 * @param ptr_data Pointer to where data removed from buffer will be stored
 *
 * @return A circular buffer status code
 */
cb_e cb_remove_item(cb_t * buf, void * ptr_data);

#endif /* __CIRCBUF_H__ */
