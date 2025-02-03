#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../timezone_database/timezone_database.h"

    typedef struct
    {
        int32_t year;
        int8_t month;
        int8_t day;
        int8_t hour;
        int8_t minute;
        int8_t second;
    } tz_time_t;

    typedef struct
    {
        int64_t std_offset_seconds;
        tz_time_t std_offset;

        bool dst_effect;
        int64_t dst_offset_seconds;
        tz_time_t dst_offset;

        int64_t total_offset_seconds;
        tz_time_t total_offset;
    } tz_get_offset_t;

    bool tz_init(void);

    bool tz_set_zone(const uint8_t** tz_identifier);
    bool tz_set_date(int32_t* g_year, uint8_t* g_month, uint8_t* g_day);
    bool tz_set_time(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second);

    void tz_calculate(void);

    void tz_get_local_time(tz_time_t** local_time);
    void tz_get_offset(tz_get_offset_t** offsets);


#ifdef __cplusplus
}
#endif

#endif