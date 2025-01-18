#ifndef GENERATOR_H
#define GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Parser/Inc/Parser.h"


    typedef struct
    {
        uint8_t Month;
        uint8_t Day;
        uint8_t Weekday;
        int32_t Hour;
        bool Hour_isUTC;
    }G_Effect_Time_t;

    typedef struct
    {
        G_Effect_Time_t Start;
        G_Effect_Time_t End;
    }G_Effects_t;

    typedef struct
    {
        uint32_t Year;
        int32_t Standard_Offset;
        int32_t Standard_Effects_Coutnt;
        G_Effects_t* Standard_Effects;
        bool DST_Effect;
        int32_t DST_Offset;
        int32_t DST_Effects_Coutnt;
        G_Effects_t* DST_Effects;
        uint8_t* Format;
    }G_Yeaars_t;

    typedef struct
    {
        uint8_t* Country_Code;
        uint8_t* Country_Name;
        uint8_t* TZ_Identifier;
        double Latitude;
        double Longitude;
        uint8_t* Comments;
        int32_t Standard_Offset;
        int8_t Standard_Offset_Text[10];
        int16_t Year_Begin;
        int16_t Year_End;
        int32_t Years_Count;
        G_Yeaars_t* Years;
    }G_Zone_Info_t;

    typedef struct
    {
        int32_t Zones_Count;
        G_Zone_Info_t* Zone;
    }G_Time_Zones_t;

    bool Generate_Data(const char* data_folder_path);
    bool Generate_Full_List(const char* data_folder_path);
    int32_t Get_Country_Name(const Parse_Data_t* parse_data, const uint8_t* country_code);
    int32_t Get_Zone_Last_Standard_Offset(const Parse_Data_t* parse_data, const uint8_t* tz_identifire);
    int16_t Get_Zone_Year_Begin(const Parse_Data_t* parse_data, const uint8_t* tz_identifire);
    int16_t Get_Zone_Year_End(const Parse_Data_t* parse_data, const uint8_t* tz_identifire);
    int16_t Get_Rule_Year_End(const Parse_Data_t* parse_data, const uint8_t* rule_name);

#ifdef __cplusplus
}
#endif

#endif