#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "timezone_database/timezone_database.h"

    typedef struct
    {
        uint8_t local_hour;
        uint8_t local_minute;
        uint8_t local_second;
    } tz_get_time_t;

    typedef struct
    {
        uint64_t std_offset_seconds;
        uint64_t dst_offset_seconds;
        bool dst_effect;
        uint64_t total_offset_seconds;
    } tz_get_offset_t;

    bool tz_init(void);

    bool tz_set_zone(const uint8_t** tz_identifier);
    bool tz_set_date(int32_t* g_year, uint8_t* g_month, uint8_t* g_day);
    bool tz_set_time(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second);

    tz_get_time_t* tz_get_time(void);
    tz_get_offset_t* tz_get_offset(void);


#ifdef __cplusplus
}
#endif

#endif