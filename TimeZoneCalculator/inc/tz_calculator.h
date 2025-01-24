#ifndef TIME_ZONE_CALCULATOR_H
#define TIME_ZONE_CALCULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../time_zone.h"

    typedef struct
    {
        uint8_t tz_identifier[TZDB_MAX_LENGHT_IDENTIFIER];
        int32_t tz_id;
    }tz_init_t;

#ifdef __cplusplus
}
#endif

#endif