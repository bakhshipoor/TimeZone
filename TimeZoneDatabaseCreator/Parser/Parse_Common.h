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
#include <limits.h>
#include <windows.h>

#define MAX_LENGTH_FILE_NAME    256
#define MAX_LENGTH_DATA_FIELD   256
#define MAX_LENGTH_LINE         2048

#define STATIC                  static
#define VOID                    void
#define CONST                   const
#define EXTERN                  extern

#define COUNTER                 int32_t
#define LENGHT                  int32_t
#define INT                     int32_t
#define INT64                   int64_t
#define CHAR                    uint8_t
#define LOCATION                double

#define YEAR                    int32_t
#define MONTH                   uint8_t
#define DAY                     uint8_t
#define WEEKDAY                 uint8_t
#define HOUR                    int64_t

#define BOOL                    bool
#define TRUE                    true
#define FALSE                   false

#define INVALID_YEAR            _I32_MAX
#define INVALID_MONTH           _UI8_MAX
#define INVALID_DAY             _UI8_MAX
#define TZDB_DAY_NONE           255
#define INVALID_WEEKDAY         _UI8_MAX
#define INVALID_HOUR            0xFFFFF
#define INDEX_NOT_FOUND         -1i32
#define YEAR_END_MAX            -1i32

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
        TZDB_MONTH_JANUARY = 1,
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

        TZDB_MONTH_TOTAL = 12ui8,
        TZDB_MONTH_NONE = 255
    }Month_Number_e;

    typedef enum
    {
        FILE_VERSION = 0ui8,
        FILE_ISO3166TAB,
        FILE_ZONETAB,
        FILE_AFRICA,
        FILE_ANTARCTICA,
        FILE_ASIA,
        FILE_AUSTRALASIA,
        FILE_BACKWARD,
        FILE_ETCETERA,
        FILE_EUROPE,
        FILE_NORTHAMERICA,
        FILE_SOUTHAMERICA,

        FILE_TOTAL
    }Parse_Files_e;

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

    EXTERN CONST CHAR* Data_Files_Name[FILE_TOTAL];
    EXTERN CONST Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL];
    EXTERN CONST Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL];
    EXTERN CHAR Data_File[FILE_TOTAL][MAX_LENGTH_FILE_NAME];

    EXTERN CHAR* line;

    HOUR Parse_Hour(CONST CHAR** hour, CHAR* suffix);
    WEEKDAY Parse_Weekday(CONST CHAR** weekday);
    MONTH Parse_Month(CONST CHAR** month);
    VOID Parse_Day_Of_Month(CONST CHAR** on, DAY* day, WEEKDAY* weekday, BOOL* weekday_after);
    

#ifdef __cplusplus
}
#endif

#endif