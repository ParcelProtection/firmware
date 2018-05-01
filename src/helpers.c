/**
 * @file helpers.c
 * @brief Helper functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/04/30
 */

#include "helpers.h"

uint8_t * my_reverse(uint8_t * src, uint32_t length)
{
  if (!src) {
    return 0;
  }

  uint8_t temp;
  int32_t i;
  for(i = 0; i < length / 2; i++)
  {
    temp = *(src + i);
    *(src + i) = *(src + length - 1 - i);
    *(src + length - 1 - i) = temp;
  }

  return src;
}

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base)
{
  if (!ptr || base < 2 || base > 16) {
    return 0;
  }

  uint8_t len = 0, val;

  /* check for sign */
  if(data < 0)
  {
    data *= -1;
    *ptr = '-';
    len++;
  }

  /* this uses modulus and divide to find the value in each place */
  do
  {
    val = data % base;
    *(ptr + len++) = val < 10 ? '0' + val : 'A' + val - 10;
    data /= base;
  } while(data != 0);

  /* reverse the numbers */
  if(*ptr == '-')
  {
    my_reverse(ptr + 1, len - 1);
  }
  else
  {
    my_reverse(ptr, len);
  }

  *(ptr + len++) = '\0';

  return len;
}
