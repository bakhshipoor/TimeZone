#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    

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

    VOID Parse_Rules(CONST CHAR** line, Rule_Entry_t** rule_list, COUNTER* rules_count);
    
   /* VOID Parse_Rule_Year_Range(Rule_Entry_t** rule_list, CONST COUNTER* rule_index, CONST Rule_Data_t** rule_data);
    VOID Parse_Rule_Years(Rule_Entry_t** rule_list, COUNTER* rules_count);
    BOOL Rule_Year_isExist(CONST Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, COUNTER* find_index);
    BOOL Rule_Year_Create(Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, CONST Rule_Data_t** rule_data);
    BOOL Rule_Year_Add_Data(Rule_Year_Data_t** year_data, CONST COUNTER* count, CONST Rule_Data_t** rule_data);*/
    
    


   /* 
    YEAR* Parse_Rule_Data_From(CONST Rule_Data_t** rule_data);
    YEAR* Parse_Rule_Data_To(CONST Rule_Data_t** rule_data, YEAR* max_value);
    MONTH* Parse_Rule_Data_In(CONST Rule_Data_t** rule_data);
    Rule_Year_Day_t* Parse_Rule_Data_On(CONST Rule_Data_t** rule_data);
    Rule_Year_Hour_t* Parse_Rule_Data_At(CONST Rule_Data_t** rule_data);
    HOUR* Parse_Rule_Data_Save(CONST Rule_Data_t** rule_data);*/

    VOID Free_Rules(Rule_Entry_t** rules, COUNTER* rules_count);




    //VOID test(Rule_Entry_t* rules, COUNTER* rules_count);

#ifdef __cplusplus
}
#endif

#endif
