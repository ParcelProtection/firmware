/**
 * @file event_buf.h
 * @brief Circular buffer for events
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/05/01
 */
#ifndef __EVENT_BUF_H__
#define __EVENT_BUF_H__

#include "packets.h"
#include "rtc.h"

/*
 * @brief Event buffer status code
 */
typedef enum
{
  EB_SUCCESS,
  EB_ERROR,
  EB_FULL,
  EB_EMPTY
} eb_e;

typedef cb_t eb_t;

/**
 * @brief Convert cb_e to eb_e
 *
 * @param val The CB return code
 *
 * @return The corresponding EB return code
 */
__attribute__((always_inline)) static inline eb_e _eb_ret(cb_e val)
{
  switch(val)
  {
    case CB_SUCCESS:
      return EB_SUCCESS;
    case CB_FULL:
      return EB_FULL;
    case CB_EMPTY:
      return EB_EMPTY;
    default:
      return EB_ERROR;
  }
}

/**
 * @brief Initialize event buffer
 * 
 * Allocate memory for event buffer and setup the structure
 * 
 * @param ptr_buf A pointer to the event buffer pointer to be allocated
 * @param len The length of the buffer to allocate in memory
 *
 * @return An event buffer status code
 */
__attribute__((always_inline)) inline eb_e eb_init(eb_t ** ptr_buf, uint32_t len)
{
  cb_e ret = cb_init(ptr_buf, len, sizeof(event_t));
  return _eb_ret(ret);
}

/**
 * @brief Destroy event buffer
 *
 * Free memory used by event buffer.
 * 
 * @param ptr_buf A pointer to the buffer to be freed
 *
 * @return An event buffer status code
 */
eb_e eb_free(eb_t ** ptr_buf)
{
  cb_e ret = cb_free(ptr_buf);
  return _eb_ret(ret);
}

/**
 * @brief Add item to event buffer
 * 
 * Add data to an event buffer
 * 
 * @param buf Pointer to the event buffer
 * @param ptr_data Event to add to buffer
 *
 * @return An event buffer status code
 */
eb_e eb_add_item(eb_t * buf, event_t * ptr_data)
{
  cb_e ret = cb_add_item(buf, (void *)ptr_data);
  return _eb_ret(ret);
}

/**
 * @brief Remove item from event buffer
 *
 * Remove data from an event buffer
 * 
 * @param buf Pointer to the event buffer
 * @param ptr_data Pointer to where data removed from buffer will be stored
 *
 * @return An event buffer status code
 */
eb_e eb_remove_item(eb_t * buf, void * ptr_data)
{
  cb_e ret = cb_remove_item(buf, ptr_data);
  return _eb_ret(ret);
}

/**
 * @brief Add an event to the buffer
 *
 * @param buf Pointer to an event buffer
 * @param event_type The type of event
 *
 * @return An event buffer status code
 */
__attribute__((always_inline)) inline eb_e eb_add_event(eb_t * buf, event_type_e event_type)
{
  event_t event;
  event.event_type = event_type;
  event.time = rtc_get_time();
  event.data = 0;

  return eb_add_item(buf, &event);
}

#endif /* __EVENT_BUF_H__ */
