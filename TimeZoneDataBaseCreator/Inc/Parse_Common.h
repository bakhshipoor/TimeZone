#ifndef PARSE_COMMON_H
#define PARSE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"

    int32_t Parse_Hour(const char* hour, char** suffix);
    uint8_t Parse_Weekday(const char* weekday);
    uint8_t Parse_Month(const char* month);
    void Parse_Day_Of_Month(const char* on, uint8_t* day, uint8_t* weekday, bool* weekday_after);
    

#ifdef __cplusplus
}
#endif

#endif