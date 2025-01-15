#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"

    typedef struct
    {
        uint8_t Month;
        uint8_t Day;
        uint8_t Weekday;
        bool Weekday_isAfterOrEqual_Day; // true if the specified weekday is on or after the reference day. If false, the specified weekday is on or before the reference day.
        uint32_t Hour;
        bool Hour_isUTC; // true if time is set based on UTC, false otherwise (local standard time)
        int32_t Save_Hour;
        uint8_t* Letter;
        uint8_t* Comment;
    }Rule_Year_Data_t;

    typedef struct
    {
        uint16_t From;
        uint16_t To;
        uint8_t* Reserved;
        bool DST_Effect;
        uint16_t Standard_Count;
        Rule_Year_Data_t* Standard;
        uint16_t DST_Count;
        Rule_Year_Data_t* DST;
    }Rule_Year_t;

    typedef struct
    {
        uint8_t* Name;
        uint32_t Years_Count;
        Rule_Year_t* Years;
    }Rule_Entry_t;

    Rule_Entry_t* Parse_Rules(int* rules_Count);
    Rule_Year_t Parse_Rule_Year(const char* from, const char* to, const char* reserved, const char* in, const char* on, const char* at, const char* save, const char* letter, const char* comment);

#ifdef __cplusplus
}
#endif

#endif