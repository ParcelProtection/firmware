/**
 * @file rtc.h
 * @brief RTC functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/05/01
 */
#ifndef __RTC_H__
#define __RTC_H__

typedef struct
{
  uint16_t year;
  uint8_t month;
  uint8_t dow;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t reserved[2];
} rtc_t;

/**
 * @brief initializes RTC
 *
 * @param curr_time the current epoch time
 *
 * @return none
 */
void rtc_init(rtc_t curr_time);

/**
 * @brief gets current time
 *
 * @return current epoch time
 */
rtc_t rtc_get_time();

#endif /* __RTC_H__ */
