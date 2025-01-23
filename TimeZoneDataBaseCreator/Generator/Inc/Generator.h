#ifndef GENERATOR_H
#define GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Parser/Inc/Parser.h"

    typedef struct
    {
        COUNTER TZ_ID;
        CHAR* TZ_Identifier;
        CHAR* Country_Code;
        CHAR* Country_Name;
        LOCATION Latitude;
        LOCATION Longitude;
        CHAR* Comments;
        // TZ Linked
        BOOL Has_Data;
        CHAR* Linked_TZ_Identifier;

    }Time_Zone_Info_t;

    typedef struct
    {
        COUNTER Link_Name_TZ_ID;
    }Time_Zone_Link_Name_t;

    typedef struct
    {
        COUNTER Target_TZ_ID;
        COUNTER Link_Name_Count;
        Time_Zone_Link_Name_t* Link_Name_List;
    }Time_Zone_Links_t;

    typedef struct
    {
        COUNTER Zones_Count;
        Time_Zone_Info_t* Zones_Info;
        COUNTER Links_Count;
        Time_Zone_Links_t* Links;
    }Time_Zones_t;

    BOOL Generate_Data(CONST CHAR** data_folder_path);
    VOID Generate_Time_Zones_Info(Parse_Data_t* parse_data, Time_Zone_Info_t** zone_info_list, CONST COUNTER* zones_count);
    COUNTER Get_Country_Name(CONST Parse_Data_t* parse_data, CONST CHAR** country_code);
    BOOL TZ_Check_Data(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifier);
    COUNTER TZ_Get_Linked_Identifier(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire);
    COUNTER Get_TZ_ID(CONST Time_Zone_Info_t** tz_info_list, CONST COUNTER* tz_info_count, CONST CHAR** tz_identifier);







    INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b);
    VOID Sort_Zone_Info_By_Identifier(Time_Zone_Info_t** zone_info, COUNTER* info_count);


    
   

#ifdef __cplusplus
}
#endif

#endif