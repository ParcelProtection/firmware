/**
 * @file event_buf.c
 * @brief Linked list buffer for events
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/05/02
 */

#include <stdlib.h>
#include "helpers.h"
#include "packets.h"
#include "event_buf.h"

eb_e eb_init(eb_t ** ptr_buf)
{
  /* check inputs */
  if(!ptr_buf) return EB_NULL_PTR;

  /* allocate memory */
  *ptr_buf = (eb_t *)malloc(sizeof(eb_t));
  if(!ptr_buf) return EB_MALLOC_FAILED;

  /* initialize */
  (*ptr_buf)->head = NULL;
  (*ptr_buf)->tail = NULL;
  (*ptr_buf)->count = 0;

  return EB_SUCCESS;
}

eb_e eb_free(eb_t ** ptr_buf)
{
  /* check inputs */
  if(!ptr_buf) return EB_NULL_PTR;

  /* delete members */
  event_t temp;
  while((*ptr_buf)->tail) eb_remove_item(*ptr_buf, &temp);

  free(*ptr_buf);
  *ptr_buf = NULL;

  return EB_SUCCESS;
}

eb_e eb_add_item(eb_t * buf, event_t * ptr_data)
{
  /* check inputs */
  if(!buf) return EB_NULL_PTR;

  /* create member */
  ll_event_t * ptr_event = (ll_event_t *)malloc(sizeof(ll_event_t));
  if(!ptr_event) return EB_MALLOC_FAILED;

  memcpy(&ptr_event->event, ptr_data, sizeof(event_t));
  ptr_event->next = NULL;

  BEGIN_CRITICAL_SECTION();

  /* place new data at head */
  ptr_event->prev = buf->head;
  buf->head->next = ptr_event;
  buf->head = ptr_event;

  if(!buf->tail) buf->tail = ptr_event;

  buf->count++;

  END_CRITICAL_SECTION();

  return EB_SUCCESS;
}

eb_e eb_remove_item(eb_t * buf, event_t * ptr_data)
{
  /* check inputs */
  if(!buf) return EB_NULL_PTR;
  /* check empty */
  if(!buf->tail) return EB_EMPTY;

  BEGIN_CRITICAL_SECTION();

  /* copy from tail */
  memcpy(ptr_data, &buf->tail->event, sizeof(event_t));

  /* remove member */
  ll_event_t * rem = buf->tail;
  buf->tail = rem->next;

  if(rem->next) rem->next->prev = NULL;
  else buf->head = NULL;

  buf->count--;

  END_CRITICAL_SECTION();

  free( (void *)rem );

  return EB_SUCCESS;
}

eb_e eb_seek_item(eb_t * buf, event_t * ptr_data, uint32_t idx)
{
  /* check inputs */
  if(!buf || !ptr_data) return EB_NULL_PTR;
  /* check empty */
  if(!buf->tail) return EB_EMPTY;

  BEGIN_CRITICAL_SECTION();

  uint32_t i;
  ll_event_t * curr = buf->tail;
  for(i = 0; i < idx && curr->next; curr = curr->next, i++);

  if(i != idx) return EB_EMPTY;

  memcpy(ptr_data, &curr->event, sizeof(event_t));

  END_CRITICAL_SECTION();

  return EB_SUCCESS;
}
