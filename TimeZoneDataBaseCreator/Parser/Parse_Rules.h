#ifndef PARSE_RULES_H
#define PARSE_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    
    typedef struct
    {
        HOUR Hour;
        BOOL Hour_isUTC;
    } Rule_Data_Hour_t;

    typedef struct
    {
        DAY Day;
        WEEKDAY Weekday;
        BOOL Weekday_isAfterOrEqual_Day;
    } Rule_Data_Day_t;

    typedef struct
    {
        COUNTER Rule_ID;
        YEAR From;
        YEAR To;
        CHAR* Reserved;
        MONTH Month;
        Rule_Data_Day_t Day;
        Rule_Data_Hour_t Hour;
        HOUR Save_Hour;
        CHAR* Letter;
        CHAR* Comment;
    } Rule_Data_t;

    typedef struct
    {
        CHAR* Name;
        COUNTER Years_Count;
        Rule_Data_t* Years;
        YEAR Year_Begin;
        YEAR Year_End;
    } Rule_Entry_t;

    VOID Parse_Rules(CONST CHAR** line, Rule_Entry_t** rule_list, COUNTER* rules_count);

    VOID Free_Rules(Rule_Entry_t** rules, COUNTER* rules_count);




    //VOID test(Rule_Entry_t* rules, COUNTER* rules_count);

#ifdef __cplusplus
}
#endif

#endif
