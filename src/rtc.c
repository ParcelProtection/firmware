/**
 * @file rtc.c
 * @brief RTC functions
 *
 * For CSCI 4830-019 Wireless X final project
 *
 * @author Christopher Morroni
 * @date 2018/05/01
 */

#include "msp.h"
#include "rtc.h"

void rtc_init(rtc_t curr_time)
{
  RTC_C->CTL0 = (0xA5 << RTC_C_CTL0_KEY_OFS);
  RTC_C->CTL13 |= RTC_C_CTL13_HOLD;
  RTC_C->YEAR = curr_time.year;
  RTC_C->DATE = (curr_time.month << RTC_C_DATE_MON_OFS) | (curr_time.day << RTC_C_DATE_DAY_OFS);
  RTC_C->TIM1 = (curr_time.dow << RTC_C_TIM1_DOW_OFS) | (curr_time.hour << RTC_C_TIM1_HOUR_OFS);
  RTC_C->TIM0 = (curr_time.minute << RTC_C_TIM0_MIN_OFS) | (curr_time.second << RTC_C_TIM0_SEC_OFS);
  RTC_C->CTL13 &= ~RTC_C_CTL13_HOLD;
  RTC_C->CTL0 = 0;
}

rtc_t rtc_get_time()
{
  rtc_t ret;
  ret.year = RTC_C->YEAR;
  ret.month = (RTC_C->DATE & RTC_C_DATE_MON_MASK) >> RTC_C_DATE_MON_OFS;
  ret.day = (RTC_C->DATE & RTC_C_DATE_DAY_MASK) >> RTC_C_DATE_DAY_OFS;
  ret.dow = (RTC_C->TIM1 & RTC_C_TIM1_DOW_MASK) >> RTC_C_TIM1_DOW_OFS;
  ret.hour = (RTC_C->TIM1 & RTC_C_TIM1_HOUR_MASK) >> RTC_C_TIM1_HOUR_OFS;
  ret.minute = (RTC_C->TIM0 & RTC_C_TIM0_MIN_MASK) >> RTC_C_TIM0_MIN_OFS;
  ret.second = (RTC_C->TIM0 & RTC_C_TIM0_SEC_MASK) >> RTC_C_TIM0_SEC_OFS;
  return ret;
}
