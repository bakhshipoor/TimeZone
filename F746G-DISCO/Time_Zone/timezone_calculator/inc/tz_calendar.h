#ifndef TZ_CALENDAR_H
#define TZ_CALENDAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


    uint8_t calculate_days_in_month(int32_t* year, int8_t* month);
    uint32_t calculate_JDN(int32_t* year, int8_t* month, int8_t* day);
    double calculate_JD(uint32_t* jdn, int64_t* second);
    void convert_JD_to_Gregorian(double* JD, int* year, int* month, int* day, int* hour, int* minute, int* second);
    uint8_t calculate_last_weekday_day_in_month(int32_t* year, int8_t* month, int8_t* weekday);
    uint8_t calculate_first_weekday_after_day_in_month(int32_t* year, int8_t* month, int8_t* day, int8_t* weekday);
    uint8_t calculate_first_weekday_before_day_in_month(int32_t* year, int8_t* month, int8_t* day, int8_t* weekday);
    void subtract_or_add_seconds(int32_t* year, int8_t* month, int8_t* day, int64_t* second, const int64_t seconds_to_add);


#ifdef __cplusplus
}
#endif

#endif