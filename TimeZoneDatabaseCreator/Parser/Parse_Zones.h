#ifndef PARSE_ZONES_H
#define PARSE_ZONES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        HOUR Hour;
        _BOOL Hour_isUTC;       
    } Zone_Data_Until_Hour_t;

    typedef struct
    {
        DAY Day;
        WEEKDAY Weekday;
        _BOOL Weekday_isAfterOrEqual_Day;
    } Zone_Data_Until_Day_t;

    typedef struct
    {
        YEAR Year;
        MONTH Month;
        Zone_Data_Until_Day_t Day;
        Zone_Data_Until_Hour_t Hour;
    } Zone_Data_Until_t;

    typedef struct
    {
        _BOOL Has_Rule;
        CHAR* Rule_Name;
        HOUR Save_Hour;
    } Zone_Data_Rule_t;

    typedef struct
    {
        COUNTER Time_Zone_ID;
        HOUR Standard_Offset;
        Zone_Data_Rule_t Rule;
        CHAR* Format;
        Zone_Data_Until_t Until;
        CHAR* Comment;
    } Zone_Data_t;

    typedef struct
    {
        CHAR* Name;            
        COUNTER Info_Count;
        Zone_Data_t* Info;
        YEAR Year_Begin;
        YEAR Year_End;
    } Zone_Entry_t;

    VOID Parse_Zones(CONST CHAR** line, Zone_Entry_t** zones_list, COUNTER* zones_count);
    

#ifdef __cplusplus
}
#endif

#endif