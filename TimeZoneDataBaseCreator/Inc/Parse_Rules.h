#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"


#define MAX_LENGHT_DATA_FIELD 256

#define RULE_YEAR_MAX_VALUE 2025

    typedef struct
    {
        uint8_t Field[MAX_LENGHT_DATA_FIELD];
        uint8_t Name[MAX_LENGHT_DATA_FIELD];
        uint8_t From[MAX_LENGHT_DATA_FIELD];
        uint8_t To[MAX_LENGHT_DATA_FIELD];
        uint8_t Reserved[MAX_LENGHT_DATA_FIELD];
        uint8_t In[MAX_LENGHT_DATA_FIELD];
        uint8_t On[MAX_LENGHT_DATA_FIELD];
        uint8_t At[MAX_LENGHT_DATA_FIELD];
        uint8_t Save[MAX_LENGHT_DATA_FIELD];
        uint8_t Letter[MAX_LENGHT_DATA_FIELD];
        uint8_t Comment[MAX_LENGHT_DATA_FIELD];
    }Rule_Data_t;

    typedef struct
    {
        uint8_t Day;
        uint8_t Weekday;
        bool Weekday_isAfterOrEqual_Day; // true if the specified weekday is on or after the reference day. If false, the specified weekday is on or before the reference day.
    }Rule_Year_Day_t;

    typedef struct
    {
        uint32_t Hour;
        bool Hour_isUTC; // true if time is set based on UTC, false otherwise (local standard time)
    }Rule_Year_Hour_t;

    typedef struct
    {
        uint8_t Month;
        Rule_Year_Day_t Day;
        Rule_Year_Hour_t Hour;
        int32_t Save_Hour;
        uint8_t* Letter;
        uint8_t* Comment;
    }Rule_Year_Data_t;

    typedef struct
    {
        int16_t From;
        int16_t To;
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
        int16_t Year_Begin;
        int16_t Year_End;
    }Rule_Entry_t;

    
    Rule_Entry_t* Parse_Rules(int32_t* rules_Count);
    Rule_Data_t Parse_Rule_Data(const char* line);
    int16_t Parse_Rule_Data_From(const Rule_Data_t rule_data);
    int16_t Parse_Rule_Data_To(const Rule_Data_t rule_data);
    uint8_t Parse_Rule_Data_In(const Rule_Data_t rule_data);
    Rule_Year_Day_t Parse_Rule_Data_On(const Rule_Data_t rule_data);
    Rule_Year_Hour_t Parse_Rule_Data_At(const Rule_Data_t rule_data);
    bool Rule_isExist(const Rule_Entry_t* rule_list, const int* rules_count, const char* rule_name, int32_t* find_Index);
    Rule_Entry_t Rule_Create(const char* rule_name);
    void Parse_Rule_Year_Range(Rule_Entry_t* rule_list, const int32_t rule_index, const Rule_Data_t rule_data);
    bool Rule_Year_isExist(const Rule_Year_t* year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, int32_t* find_index);
    Rule_Year_t Rule_Year_Create(const int16_t year_from, const int16_t year_to);

#ifdef __cplusplus
}
#endif

#endif