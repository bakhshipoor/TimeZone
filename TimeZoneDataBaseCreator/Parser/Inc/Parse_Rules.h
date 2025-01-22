#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Field;
        CHAR* Name;
        CHAR* From;
        CHAR* To;
        CHAR* Reserved;
        CHAR* In;
        CHAR* On;
        CHAR* At;
        CHAR* Save;
        CHAR* Letter;
        CHAR* Comment;
    } Rule_Data_t;

    typedef struct
    {
        DAY Day;
        WEEKDAY Weekday;
        BOOL Weekday_isAfterOrEqual_Day;
    } Rule_Year_Day_t;

    typedef struct
    {
        HOUR Hour;
        BOOL Hour_isUTC;
    } Rule_Year_Hour_t;

    typedef struct
    {
        MONTH Month;
        Rule_Year_Day_t Day;
        Rule_Year_Hour_t Hour;
        HOUR Save_Hour;
        CHAR* Letter;
        CHAR* Comment;
    } Rule_Year_Data_t;

    typedef struct
    {
        YEAR From;
        YEAR To;
        CHAR* Reserved;

        COUNTER Standard_Count;
        Rule_Year_Data_t* Standard;
        COUNTER DST_Count;
        Rule_Year_Data_t* DST;
    } Rule_Year_t;

    typedef struct
    {
        CHAR* Name;
        COUNTER Years_Count;
        Rule_Year_t* Years;
        YEAR Year_Begin;
        YEAR Year_End;
    } Rule_Entry_t;

    Rule_Entry_t* Parse_Rules(COUNTER* rules_Count);
    BOOL Rule_isExist(CONST Rule_Entry_t** rule_list, CONST COUNTER* rules_count, CONST CHAR** rule_name, COUNTER* find_Index);
    BOOL Rule_Create(Rule_Entry_t** rule_list, CONST COUNTER* rules_Count, CONST CHAR** rule_name);
    
    VOID Parse_Rule_Year_Range(Rule_Entry_t** rule_list, CONST COUNTER* rule_index, CONST Rule_Data_t** rule_data);
    VOID Parse_Rule_Years(Rule_Entry_t** rule_list, COUNTER* rules_Count);
    BOOL Rule_Year_isExist(CONST Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, COUNTER* find_index);
    BOOL Rule_Year_Create(Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, CONST Rule_Data_t** rule_data);
    BOOL Rule_Year_Add_Data(Rule_Year_Data_t** year_data, CONST COUNTER* count, CONST Rule_Data_t** rule_data);
    
    


    Rule_Data_t* Parse_Rule_Data(CONST CHAR** line);
    YEAR* Parse_Rule_Data_From(CONST Rule_Data_t** rule_data);
    YEAR* Parse_Rule_Data_To(CONST Rule_Data_t** rule_data, YEAR* max_value);
    MONTH* Parse_Rule_Data_In(CONST Rule_Data_t** rule_data);
    Rule_Year_Day_t* Parse_Rule_Data_On(CONST Rule_Data_t** rule_data);
    Rule_Year_Hour_t* Parse_Rule_Data_At(CONST Rule_Data_t** rule_data);
    HOUR* Parse_Rule_Data_Save(CONST Rule_Data_t** rule_data);

    VOID Parse_Free_Rule_Data(Rule_Data_t** rule_data);
    VOID Free_Standard_Entries(Rule_Year_Data_t* standards, COUNTER count);
    VOID Free_DST_Entries(Rule_Year_Data_t* dst_entries, COUNTER count);
    VOID Parse_Free_Rules(Rule_Entry_t* rules, COUNTER rules_count);




    VOID test(Rule_Entry_t* rules, COUNTER* rules_count);

#ifdef __cplusplus
}
#endif

#endif
