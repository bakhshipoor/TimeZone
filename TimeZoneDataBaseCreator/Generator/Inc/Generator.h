#ifndef GENERATOR_H
#define GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Parser/Inc/Parser.h"


    typedef struct
    {
        MONTH Month;
        DAY Day;
        WEEKDAY Weekday;
        HOUR Hour;
        BOOL Hour_isUTC;
    }G_Effect_Time_t;

    typedef struct
    {
        G_Effect_Time_t Start;
        G_Effect_Time_t End;
    }G_Effects_t;

    typedef struct
    {
        YEAR Year;
        HOUR Standard_Offset;
        COUNTER Standard_Effects_Coutnt;
        G_Effects_t* Standard_Effects;
        BOOL DST_Effect;
        HOUR DST_Offset;
        COUNTER DST_Effects_Coutnt;
        G_Effects_t* DST_Effects;
        CHAR* Format;
    }G_Yeaars_t;

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* Country_Name;
        CHAR* TZ_Identifier;
        LOCATION Latitude;
        LOCATION Longitude;
        CHAR* Comments;
        HOUR Standard_Offset;
        CHAR Standard_Offset_Text[10];
        YEAR Year_Begin;
        YEAR Year_End;
        COUNTER Years_Count;
        G_Yeaars_t* Years;
    }G_Zone_Info_t;

    typedef struct
    {
        COUNTER Zones_Count;
        G_Zone_Info_t* Zone;
    }G_Time_Zones_t;

    BOOL Generate_Data(CONST CHAR* data_folder_path);
    BOOL Generate_Full_List(CONST CHAR* data_folder_path);
    COUNTER Get_Country_Name(CONST Parse_Data_t* parse_data, CONST CHAR* country_code);
    HOUR Get_Zone_Last_Standard_Offset(CONST Parse_Data_t* parse_data, CONST CHAR* tz_identifire);
    YEAR Get_Zone_Year_Begin(CONST Parse_Data_t* parse_data, CONST CHAR* tz_identifire);
    YEAR Get_Zone_Year_End(CONST Parse_Data_t* parse_data, CONST CHAR* tz_identifire);
    YEAR Get_Rule_Year_End(CONST Parse_Data_t* parse_data, CONST CHAR* rule_name);

#ifdef __cplusplus
}
#endif

#endif