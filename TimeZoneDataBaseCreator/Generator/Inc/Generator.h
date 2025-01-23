#ifndef GENERATOR_H
#define GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Parser/Inc/Parser.h"

    typedef struct
    {
        COUNTER Time_Zone_ID;
        CHAR* Time_Zone_Identifier;
        CHAR* Country_Code;
        CHAR* Country_Name;
        LOCATION Latitude;
        LOCATION Longitude;
        BOOL Has_Data;
        CHAR* Linked_TZ_Identifier;
        COUNTER Data_Count;
        YEAR Year_Begin;
        YEAR Year_End;
        CHAR* Comments;
    }Zone_Info_t;

    typedef struct
    {
        COUNTER Rule_ID;
        CHAR* Name;
        COUNTER Years_Count;
        YEAR Year_Begin;
        YEAR Year_End;
    } Rule_Info_t;


    typedef struct
    {
        COUNTER Zones_Count;
        COUNTER Zones_Data_Count;
        COUNTER Rules_Count;
        COUNTER Rules_Data_Count;
        Zone_Info_t* Zones_Info;
        Zone_Data_t* Zones_Data;
        Rule_Info_t* Rules_Info;
        Rule_Data_t* Rules_Data;
    }Time_Zones_t;

    Time_Zones_t* Generate_Data(CONST CHAR** data_folder_path);
    VOID Generate_Time_Zones_Info(Parse_Data_t* parse_data, Zone_Info_t** zone_info_list, CONST COUNTER* zones_count);
    VOID Generate_Time_Zones_Data(Parse_Data_t* parse_data, Zone_Data_t** zones_data_list, COUNTER* zones_data_count, Time_Zones_t** tz_list);
    VOID Generate_Rules_Info(Parse_Data_t* parse_data, Rule_Info_t** rules_info_list);
    VOID Generate_Rules_Data(Parse_Data_t* parse_data, Rule_Data_t** rules_data_list, COUNTER* rules_data_count, Time_Zones_t** tz_list);
    COUNTER Get_Country_Name(CONST Parse_Data_t* parse_data, CONST CHAR** country_code);


    BOOL TZ_Check_Data(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifier);
    COUNTER TZ_Get_Linked_Identifier(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    YEAR Get_Zone_Year_Begin(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    YEAR Get_Zone_Year_End(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    COUNTER Get_Zone_Data_Count(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    COUNTER Get_Zone_ID(CONST Zone_Info_t** tz_info_list, CONST COUNTER* tz_info_count, CONST CHAR** tz_identifier);
    COUNTER Get_Rule_ID(CONST Rule_Info_t** rule_info_list, CONST COUNTER* rule_info_count, CONST CHAR** rule_name);


    INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b);
    VOID Sort_Zone_Info_By_Identifier(Zone_Info_t** zone_info, COUNTER* info_count);
    INT Compare_Zones_Data_Identifier(CONST VOID* a, CONST VOID* b);
    VOID Sort_Zone_Data_By_Identifier(Zone_Entry_t** zone_data, COUNTER* data_count);
    INT Compare_Rules_Name(CONST VOID* a, CONST VOID* b);
    VOID Sort_Rules_By_Name(Rule_Info_t** rules_info, COUNTER* info_count);
    
   

#ifdef __cplusplus
}
#endif

#endif