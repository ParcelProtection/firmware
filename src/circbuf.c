/**
 * @file circbuf.c
 * @brief Circular buffer functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */

#include <stdlib.h>
#include "helpers.h"
#include "circbuf.h"

cb_e cb_init(cb_t ** ptr_buf, size_t len, size_t item_size)
{
  /* check inputs */
  if(!ptr_buf) return CB_NULL_PTR;
  if(len < 1) return CB_INVALID_PARAM;
  if(item_size < 1) return CB_INVALID_PARAM;

  /* allocate memory */
  *ptr_buf = (cb_t *)malloc(sizeof(cb_t));
  if(!ptr_buf) return CB_MALLOC_FAILED;

  (*ptr_buf)->base = (cb_t *)malloc(item_size * len);
  if( !(*ptr_buf)->base ) return CB_MALLOC_FAILED;

  /* initialize */
  (*ptr_buf)->count = 0;
  (*ptr_buf)->size = len;
  (*ptr_buf)->item_size = item_size;
  (*ptr_buf)->head = (*ptr_buf)->base;
  (*ptr_buf)->tail = (*ptr_buf)->base;

  return CB_SUCCESS;
}

cb_e cb_free(cb_t ** ptr_buf)
{
  /* check inputs */
  if(!ptr_buf || !(*ptr_buf)) return CB_NULL_PTR;

  if( (*ptr_buf)->base ) free((*ptr_buf)->base);
  free(*ptr_buf);
  *ptr_buf = NULL;

  return CB_SUCCESS;
}

cb_e cb_add_item(cb_t * buf, void * ptr_data)
{
  /* check inputs */
  if(!buf || !ptr_data) return CB_NULL_PTR;
  /* check full */
  if(buf->count == buf->size) return CB_FULL;

  BEGIN_CRITICAL_SECTION();

  /* place new data at head */
  memcpy(buf->head, ptr_data, buf->item_size);

  /* move head */
  if( (buf->head += buf->item_size) >= buf->head + buf->size * buf->item_size )
  {
    buf->head = buf->base;
  }
  buf->count++;

  END_CRITICAL_SECTION();

  return CB_SUCCESS;
}

cb_e cb_remove_item(cb_t * buf, void * ptr_data)
{
  /* check inputs */
  if(!buf) return CB_NULL_PTR;
  /* check empty */
  if(buf->count == 0) return CB_EMPTY;

  BEGIN_CRITICAL_SECTION();

  /* copy from tail */
  memcpy(ptr_data, buf->tail, buf->item_size);

  /* move tail */
  if( (buf->tail += buf->item_size) >= buf->head + buf->size * buf->item_size )
  {
    buf->tail = buf->base;
  }
  buf->count--;

  END_CRITICAL_SECTION();

  return CB_SUCCESS;
}

cb_e cb_clear(cb_t * buf)
{
  /* check inputs */
  if(!buf) return CB_NULL_PTR;

  BEGIN_CRITICAL_SECTION();

  buf->tail = buf->head;
  buf->count = 0;

  END_CRITICAL_SECTION();

  return CB_SUCCESS;
}
