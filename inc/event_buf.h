/**
 * @file event_buf.h
 * @brief Linked list buffer for events
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
  EB_NULL_PTR,
  EB_MALLOC_FAILED,
  EB_EMPTY
} eb_e;

typedef struct ll_event_t
{
  event_t event;
  struct ll_event_t * prev;
  struct ll_event_t * next;
} ll_event_t;

typedef struct
{
  ll_event_t * head;
  ll_event_t * tail;
} eb_t;

/**
 * @brief Initialize event buffer
 * 
 * Allocate memory for event buffer and setup the structure
 * 
 * @param ptr_buf A pointer to the event buffer pointer to be allocated
 *
 * @return An event buffer status code
 */
eb_e eb_init(eb_t ** ptr_buf);

/**
 * @brief Destroy event buffer
 *
 * Free memory used by event buffer.
 * 
 * @param ptr_buf A pointer to the buffer to be freed
 *
 * @return An event buffer status code
 */
eb_e eb_free(eb_t ** ptr_buf);

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
eb_e eb_add_item(eb_t * buf, event_t * ptr_data);

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
eb_e eb_remove_item(eb_t * buf, event_t * ptr_data);

/**
 * @brief Get an event by its index
 *
 * Get the nth event in the buffer
 *
 * @param buf Pointer to the event buffer
 * @param ptr_data Pointer to where the event will be stored
 * @param idx Index of th requested event
 *
 * @return An event buffer status code
 */
eb_e eb_seek_item(eb_t * buf, event_t * ptr_data, uint32_t idx);

/**
 * @brief Add an event to the buffer
 *
 * @param buf Pointer to an event buffer
 * @param event_type The type of event
 *
 * @return An event buffer status code
 */
__attribute__((always_inline)) inline eb_e eb_new_event(eb_t * buf, event_type_e event_type, uint32_t data)
{
  event_t event;
  event.event_type = event_type;
  event.time = rtc_get_time();
  event.data = data;

  return eb_add_item(buf, &event);
}

#endif /* __EVENT_BUF_H__ */
