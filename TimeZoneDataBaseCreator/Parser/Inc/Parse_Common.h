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
#include <stdBOOL.h>
#include <ctype.h>


#define DATA_FILES_COUNT 12
#define MAX_LENGHT_FILE_NAME 256
#define MAX_LENGHT_DATA_FIELD 256

#define VOID void
#define CONST const
#define EXTERN extern

#define COUNTER int32_t
#define LENGHT int32_t
#define INT int32_t
#define CHAR uint8_t
#define LOCATION double

#define YEAR int32_t
#define MONTH uint8_t
#define DAY uint8_t
#define WEEKDAY uint8_t
#define HOUR int64_t

#define BOOL bool
#define TRUE true
#define FALSE false

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
        CHAR* Abbr;
        CHAR* Full;
        CHAR* Last_Abbr;
        CHAR* Last_Full;
        WEEKDAY Number;
    }Weekday_Lookup_t;

    typedef struct
    {
        CHAR* Abbr;
        CHAR* Full;
        MONTH Number;
    }Month_Lookup_t;

    EXTERN CONST CHAR* Data_Files_Name[DATA_FILES_COUNT];
    EXTERN CONST Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL];
    EXTERN CONST Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL];
    EXTERN CHAR Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

    HOUR Parse_Hour(CONST CHAR* hour, CHAR** suffix);
    WEEKDAY Parse_Weekday(CONST CHAR* weekday);
    MONTH Parse_Month(CONST CHAR* month);
    VOID Parse_Day_Of_Month(CONST CHAR* on, DAY* day, WEEKDAY* weekday, BOOL* weekday_after);
    

#ifdef __cplusplus
}
#endif

#endif