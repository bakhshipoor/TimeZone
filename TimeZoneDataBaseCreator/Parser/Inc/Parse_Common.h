#ifndef PARSE_COMMON_H
#define PARSE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define DATA_FILES_COUNT 13
#define MAX_LENGHT_FILE_NAME 256
#define MAX_LENGHT_DATA_FIELD 256

    typedef enum
    {
        TZDB_WEEKDAY_SUNDAY = 0,
        TZDB_WEEKDAY_MONDAY,
        TZDB_WEEKDAY_TUESDAY,
        TZDB_WEEKDAY_WEDNESDAY,
        TZDB_WEEKDAY_THURSDAY,
        TZDB_WEEKDAY_FRIDAY,
        TZDB_WEEKDAY_SATURDAY,

        TZDB_WEEKDAY_TOTAL,
        TZDB_WEEKDAY_NONE = 255,
    }Weekday_Number_e;

    typedef enum
    {
        TZDB_MONTH_JANUARY = 0,
        TZDB_MONTH_FEBRUARY,
        TZDB_MONTH_MARCH,
        TZDB_MONTH_APRIL,
        TZDB_MONTH_MAY,
        TZDB_MONTH_JUNE,
        TZDB_MONTH_JULY,
        TZDB_MONTH_AUGUST,
        TZDB_MONTH_SEPTEMBER,
        TZDB_MONTH_OCTOBER,
        TZDB_MONTH_NOVEMBER,
        TZDB_MONTH_DECEMBER,

        TZDB_MONTH_TOTAL
    }Month_Number_e;

    typedef struct
    {
        uint8_t* Abbr;
        uint8_t* Full;
        uint8_t* Last_Abbr;
        uint8_t* Last_Full;
        uint8_t Number;
    }Weekday_Lookup_t;

    typedef struct
    {
        uint8_t* Full;
        uint8_t Number;
    }Last_Weekday_Lookup_t;

    typedef struct
    {
        uint8_t* Abbr;
        uint8_t* Full;
        uint8_t Number;
    }Month_Lookup_t;

    extern const char* Data_Files_Name[DATA_FILES_COUNT];
    extern const Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL];
    extern const Last_Weekday_Lookup_t Last_Weekday_Names[TZDB_WEEKDAY_TOTAL];
    extern const Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL];
    extern uint8_t Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

    int32_t Parse_Hour(const char* hour, char** suffix);
    uint8_t Parse_Weekday(const char* weekday);
    uint8_t Parse_Month(const char* month);
    void Parse_Day_Of_Month(const char* on, uint8_t* day, uint8_t* weekday, bool* weekday_after);
    

#ifdef __cplusplus
}
#endif

#endif