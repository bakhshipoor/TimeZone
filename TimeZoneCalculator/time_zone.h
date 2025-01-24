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
        uint8_t offset_hour;
        uint8_t offset_minute;
        uint8_t offset_second;
        double offset;
        bool dst_effect;
    } tz_get_offset_t;

    bool tz_set_zone(const char* tz_identifier);
    bool tz_set_zone_ptr(const char** tz_identifier);

    void tz_set_date(int32_t g_year,uint8_t  g_month, uint8_t  g_day);
    void tz_set_date_ptr(int32_t* g_year, uint8_t* g_month, uint8_t* g_day);

    tz_get_time_t* tz_get_time(uint8_t utc_hour, uint8_t utc_minute, uint8_t utc_second);
    tz_get_time_t* tz_get_time_ptr(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second);

    tz_get_offset_t* tz_get_offset(uint8_t utc_hour, uint8_t utc_minute, uint8_t utc_second);
    tz_get_offset_t* tz_get_offset_ptr(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second);


#ifdef __cplusplus
}
#endif

#endif