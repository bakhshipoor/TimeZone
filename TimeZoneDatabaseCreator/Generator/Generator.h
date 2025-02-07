#ifndef GENERATOR_H
#define GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../Parser/Parser.h"

    typedef struct
    {
        COUNTER Time_Zone_ID;
        CHAR* Time_Zone_Identifier;
        HOUR STD_Offset;
        HOUR DST_Offset;
        CHAR* Country_Code;
        CHAR* Country_Name;
        LOCATION Latitude;
        LOCATION Longitude;
        COUNTER Linked_Zone_ID;
        COUNTER Data_Count;
        YEAR Year_Begin;
        YEAR Year_End;
        CHAR* Comments;
    }Zone_Info_t;

    typedef struct
    {
        COUNTER Time_Zone_ID;
        HOUR Standard_Offset;
        COUNTER Rule_ID;
        HOUR Save_Hour;
        CHAR* Format;
        JD Until_JD; 
        CHAR* Comments;
    } ZoneData_t;

    typedef struct
    {
        COUNTER Rule_ID;
        CHAR* Name;
        COUNTER Data_Count;
        YEAR Year_Begin;
        YEAR Year_End;
    } Rule_Info_t;

    typedef struct
    {
        JD Rule_JD;
        HOUR Offset;
    }Rule_Data_Changes_t;

    typedef struct
    {
        COUNTER Rule_ID;
        YEAR From;
        YEAR To;
        MONTH Month;
        DAY Day;
        WEEKDAY Weekday;
        bool Weekday_IsAfterOrEqual_Day;
        HOUR Hour;
        bool Hour_isUTC;
        HOUR Save_Hour;
        CHAR* Letter;
        CHAR* Comments;
    } RuleData_t;


    typedef struct
    {
        Version_t Version;
        COUNTER Rules_Count;
        COUNTER Rules_Data_Count;
        COUNTER Zones_Count;
        COUNTER Zones_Data_Count;
        Rule_Info_t* Rules_Info;
        RuleData_t* Rules_Data;
        Zone_Info_t* Zones_Info;
        ZoneData_t* Zones_Data;
    }Time_Zones_t;

    Time_Zones_t* Generate_Data(CONST CHAR** data_folder_path);
    
    VOID Generate_Time_Zones_Info(Parse_Data_t* parse_data, Zone_Info_t** zone_info_list, CONST COUNTER* zones_count);
    VOID Generate_Time_Zones_Data(Parse_Data_t* parse_data, ZoneData_t** zones_data_list, COUNTER* zones_data_count, Time_Zones_t** tz_list);
    VOID Generate_Rules_Info(Parse_Data_t* parse_data, Rule_Info_t** rules_info_list);
    VOID Generate_Rules_Data(Parse_Data_t* parse_data, RuleData_t** rules_data_list, COUNTER* rules_data_count, Time_Zones_t** tz_list);
    
    COUNTER Get_Country_Name(CONST Parse_Data_t* parse_data, CONST CHAR** country_code);


    _BOOL TZ_Check_Data(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifier);
    COUNTER TZ_Get_Linked_Identifier(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    YEAR Get_Zone_Year_Begin(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    YEAR Get_Zone_Year_End(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    COUNTER Get_Zone_Data_Count(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    COUNTER Get_Zone_ID(CONST Zone_Info_t** tz_info_list, CONST COUNTER* tz_info_count, CONST CHAR** tz_identifier);
    COUNTER Get_Rule_ID(CONST Rule_Info_t** rule_info_list, CONST COUNTER* rule_info_count, CONST CHAR** rule_name);


    JD Get_Zone_Data_Until(CONST Zone_Data_Until_t* until, CONST HOUR* utc_offset);
    DAY Calculate_Days_In_Month(YEAR* year, MONTH* month);
    JD Calculate_JD(JDN* jdn, HOUR* second);
    JDN Calculate_JDN(YEAR* year, MONTH* month, DAY* day);
    DAY Calculate_Last_Weekday_Day_In_Month(YEAR* year, MONTH* month, WEEKDAY* weekday);
    DAY Calculate_First_Weekday_After_Day_In_Month(YEAR* year, MONTH* month, DAY* day, WEEKDAY* weekday);
    DAY Calculate_First_Weekday_Before_Day_In_Month(YEAR* year, MONTH* month, DAY* day, WEEKDAY* weekday);
    VOID Subtract_Or_Add_Seconds(YEAR* year, MONTH* month, DAY* day, HOUR* second, CONST HOUR seconds_to_add);

    VOID Calculate_Current_Offsets(Time_Zones_t** tz);
    COUNTER Find_Zone_Data_Index_JD(COUNTER* tz_id, Time_Zones_t** tz, JD* jd);
    VOID Get_Rules_Data_JD(CONST Time_Zones_t** tz, Rule_Data_Changes_t** ch_list, COUNTER* ch_count, CONST COUNTER* rule_id, CONST YEAR* year);
    DAY Calculate_Day(YEAR* _y, MONTH* _m, DAY* _d, WEEKDAY* _weekday, _BOOL* _is_after);

    INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b);
    VOID Sort_Zone_Info_By_Identifier(Zone_Info_t** zone_info, CONST COUNTER* info_count);
    INT Compare_Zones_Data_Identifier(CONST VOID* a, CONST VOID* b);
    VOID Sort_Zone_Data_By_Identifier(Zone_Entry_t** zone_data, COUNTER* data_count);
    INT Compare_Rules_Name(CONST VOID* a, CONST VOID* b);
    VOID Sort_Rules_By_Name(Rule_Entry_t** rules_info, CONST COUNTER* info_count);
    
   
   

#ifdef __cplusplus
}
#endif

#endif