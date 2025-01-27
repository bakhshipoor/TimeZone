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
        int32_t* zone_data_count;
        int32_t zone_rules_count;
        int32_t* zone_rules_id;
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
        int32_t* zone_data_count;
        tzdb_zone_data_t** zone_data;
    }tz_zone_data_t;

    typedef struct
    {
        int32_t rules_data_count;
        tzdb_rule_data_t** rule_data;
    }tz_rule_data_t;

    typedef struct
    {
        int64_t now_seconds;
        uint32_t now_jdn;
        double now_jd;
        int64_t std_offset;
        bool dst_effect;
        int64_t dst_offset;
        int64_t total_offset;
    }tz_calculated_data_t;
    



#ifdef __cplusplus
}
#endif

#endif