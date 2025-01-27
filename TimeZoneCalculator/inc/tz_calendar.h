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


    uint8_t calculate_days_in_month(int32_t* year, uint8_t* month);
    uint32_t calculate_JDN(int32_t* year, uint8_t* month, uint8_t* day);



#ifdef __cplusplus
}
#endif

#endif