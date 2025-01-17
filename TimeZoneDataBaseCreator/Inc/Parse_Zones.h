#ifndef PARSE_ZONES_H
#define PARSE_ZONES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"
#include "Parse_Common.h"

    typedef struct
    {
        uint8_t Field[MAX_LENGHT_DATA_FIELD];   
        uint8_t Name[MAX_LENGHT_DATA_FIELD];   
        uint8_t Standard_Offset[MAX_LENGHT_DATA_FIELD];    
        uint8_t Rules[MAX_LENGHT_DATA_FIELD];      
        uint8_t Format[MAX_LENGHT_DATA_FIELD];
        uint8_t Until[MAX_LENGHT_DATA_FIELD];      
        uint8_t Comment[MAX_LENGHT_DATA_FIELD];
    } Zone_Data_t;

    typedef struct
    {
        int32_t Hour;          
        bool Hour_isUTC;       
    } Zone_Info_Until_Hour_t;

    typedef struct
    {
        uint8_t Day;
        uint8_t Weekday;
        bool Weekday_isAfterOrEqual_Day;
    } Zone_Info_Until_Day_t;

    typedef struct
    {
        int16_t Year;
        uint8_t Month;
        Zone_Info_Until_Day_t Day;
        Zone_Info_Until_Hour_t Hour;
    } Zone_Info_Until_t;

    typedef struct
    {
        bool Has_Rule;
        uint8_t* Rule_Name;
        int32_t Save_Hour;
    } Zone_Info_Rule_t;

    typedef struct
    {
        int32_t Standard_Offset;
        Zone_Info_Rule_t Rule;
        uint8_t* Format;
        Zone_Info_Until_t Until;
        uint8_t* Comment;
    } Zone_Info_t;

    typedef struct
    {
        uint8_t* Name;            
        uint32_t Info_Count;    
        Zone_Info_t* Info;
        int16_t Year_Begin;      
        int16_t Year_End;      
    } Zone_Entry_t;

    Zone_Entry_t* Parse_Zones(int32_t* rules_Count);
    Zone_Data_t Parse_Zone_Data(const char* line);
    int32_t Parse_Zone_Data_Standard_Offset(const Zone_Data_t zone_data);
    Zone_Info_Rule_t Parse_Zone_Data_Rules(const Zone_Data_t zone_data);
    Zone_Info_Until_t Parse_Zone_Info_Until(const Zone_Data_t zone_data);
    bool Zone_isExist(const Zone_Entry_t* zone_list, const int32_t* zones_count, const char* zone_name, int32_t* find_Index);
    bool Zone_Create(Zone_Entry_t** zone_list, const int32_t zones_Count, const char* zone_name);
    void Parse_Zone_Year_Range(Zone_Entry_t* zone_list, const int32_t zone_index, const Zone_Data_t zone_data);
    void Parse_Zone_Info(Zone_Entry_t* zone_list, const int32_t* zones_Count);

#ifdef __cplusplus
}
#endif

#endif