#ifndef TIME_ZONE_DATABASE_CREATOR_H
#define TIME_ZONE_DATABASE_CREATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define DATA_FILES_COUNT 10

#define MAX_LENGHT_FILE_NAME 256
#define MAX_LENGHT_COUNTRY_CODE 10
#define MAX_LENGHT_COUNTRY_NAME 256
#define MAX_LENGHT_COORDINATES 50
#define MAX_LENGHT_TIMEZONEID 256
#define MAX_LENGHT_COMMENT 1024


#define MAX_LENGHT_DATA_FIELD 256

#define RULE_MAX_VALUE 10000

    typedef enum {
        TZDB_WEEKDAY_SUNDAY=0,
        TZDB_WEEKDAY_MONDAY,
        TZDB_WEEKDAY_TUESDAY,
        TZDB_WEEKDAY_WEDNESDAY,
        TZDB_WEEKDAY_THURSDAY,
        TZDB_WEEKDAY_FRIDAY,
        TZDB_WEEKDAY_SATURDAY,

        TZDB_WEEKDAY_TOTAL,
        TZDB_WEEKDAY_NONE=255,
    }Weekday_Number_e;

    typedef enum {
        TZDB_MONTH_JANUARY=0,
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


    typedef struct
    {
        uint8_t* Country_Code;
        uint8_t* Country_Name;
    } ISO3166_Entry_t;

    typedef struct
    {
        uint8_t* Country_Code;
        uint8_t* Country_Name;
        uint8_t* TZ_Identifier;
        uint8_t* Comments;
        double Latitude;
        double Longitude;
        
    } ZoneTab_Entry_t;

    typedef struct
    {
        uint16_t From;
        uint16_t To;
        uint8_t* Reserved;
        uint8_t Month;
        uint8_t Day;
        uint8_t Weekday;
        bool Wday_After_Or_Before_Day; // After or Equal: True  | Before or Equal: False
        uint32_t Hour;
        bool UTC_Or_Local_Hour; // Hour based on Local (Standard): False | Hour based on UTC: True
        int32_t Save_Hour;
        uint8_t* Letter;
        uint8_t* Comment;
    }Rule_Data_t;

    typedef struct
    {
        uint8_t* Name;
        uint32_t Data_Count;
        Rule_Data_t* Data;
    }Rule_Entry_t;

    



    typedef struct
    {
        uint8_t Version;
        uint16_t ISO3166_Count;
        ISO3166_Entry_t* ISO3166_Data;
        uint16_t ZoneTab_Count;
        ZoneTab_Entry_t* ZoneTab_Data;
    }TimeZone_DB_t;


    void Initial_FileNames(void);
    char* Parse_Version();
    ISO3166_Entry_t* Parse_ISO3166Tab(int* iso3166_Count);
    ZoneTab_Entry_t* Parse_ZoneTab(int* size);
    Rule_Entry_t* Parse_Rules(int* rules_Count);
    Rule_Data_t Parse_Rule_Data(const char* from, const char* to, const char* reserved, const char* in, const char* on, const char* at, const char* save, const char* letter, const char* comment);

#ifdef __cplusplus
}
#endif

#endif