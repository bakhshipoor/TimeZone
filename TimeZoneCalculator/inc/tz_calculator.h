#ifndef TIME_ZONE_CALCULATOR_H
#define TIME_ZONE_CALCULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "string.h"
#include "../time_zone.h"
#include "../inc/tz_calendar.h"

    typedef struct
    {
        bool tz_is_init;

        int32_t* tz_id;
        uint8_t** tz_identifier;
        bool zone_is_init;

        int32_t* g_year; 
        uint8_t* g_month; 
        uint8_t* g_day;
        bool date_is_init;

        uint8_t* utc_hour;
        uint8_t* utc_minute;
        uint8_t* utc_second;
        bool time_is_init;
    }tz_inititial_t;

    typedef struct
    {
        int32_t year_data_index;
        int64_t now_seconds;
        uint32_t now_jdn;
        double now_utc_jd;
        double now_std_jd;
        tz_get_offset_t* offsets;
    }tz_calculated_data_t;
    



#ifdef __cplusplus
}
#endif

#endif