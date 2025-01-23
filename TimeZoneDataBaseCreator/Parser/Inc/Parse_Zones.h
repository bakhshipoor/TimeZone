#ifndef PARSE_ZONES_H
#define PARSE_ZONES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        HOUR Hour;
        BOOL Hour_isUTC;       
    } Zone_Info_Until_Hour_t;

    typedef struct
    {
        DAY Day;
        WEEKDAY Weekday;
        BOOL Weekday_isAfterOrEqual_Day;
    } Zone_Info_Until_Day_t;

    typedef struct
    {
        YEAR Year;
        MONTH Month;
        Zone_Info_Until_Day_t Day;
        Zone_Info_Until_Hour_t Hour;
    } Zone_Info_Until_t;

    typedef struct
    {
        BOOL Has_Rule;
        CHAR* Rule_Name;
        HOUR Save_Hour;
    } Zone_Info_Rule_t;

    typedef struct
    {
        HOUR Standard_Offset;
        Zone_Info_Rule_t Rule;
        CHAR* Format;
        Zone_Info_Until_t Until;
        CHAR* Comment;
    } Zone_Info_t;

    typedef struct
    {
        CHAR* Name;            
        COUNTER Info_Count;
        Zone_Info_t* Info;
        YEAR Year_Begin;
        YEAR Year_End;
    } Zone_Entry_t;

    VOID Parse_Zones(CONST CHAR** line, Zone_Entry_t** zones_list, COUNTER* zones_count);
    

#ifdef __cplusplus
}
#endif

#endif