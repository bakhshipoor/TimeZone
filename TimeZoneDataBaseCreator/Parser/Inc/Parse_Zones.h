#ifndef PARSE_ZONES_H
#define PARSE_ZONES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Field;   
        CHAR* Name;   
        CHAR* Standard_Offset;    
        CHAR* Rules;      
        CHAR* Format;
        CHAR* Until;      
        CHAR* Comment;
    } Zone_Data_t;

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
        CHAR* File;
    } Zone_Entry_t;

    Zone_Entry_t* Parse_Zones(COUNTER* rules_Count);
    Zone_Data_t* Parse_Zone_Data(CONST CHAR** line);
    VOID Parse_Free_Zone_Data(Zone_Data_t** zone_data);
    HOUR* Parse_Zone_Data_Standard_Offset(CONST Zone_Data_t** zone_data);
    Zone_Info_Rule_t* Parse_Zone_Data_Rules(CONST Zone_Data_t** zone_data);
    Zone_Info_Until_t* Parse_Zone_Info_Until(CONST Zone_Data_t** zone_data);
    BOOL Zone_isExist(CONST Zone_Entry_t** zone_list, CONST COUNTER* zones_count, CONST CHAR** zone_name, COUNTER* find_Index);
    BOOL Zone_Create(Zone_Entry_t** zone_list, CONST COUNTER* zones_Count, CONST CHAR** zone_name, CONST CHAR** file_name);
    VOID Parse_Zone_Year_Range(Zone_Entry_t** zone_list, CONST COUNTER* zone_index, CONST Zone_Data_t** zone_data);
    VOID Parse_Zone_Info(Zone_Entry_t** zone_list, CONST COUNTER* zones_Count);

#ifdef __cplusplus
}
#endif

#endif