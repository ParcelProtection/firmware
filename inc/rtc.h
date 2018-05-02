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

#include "packets.h"

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
