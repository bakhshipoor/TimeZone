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



    typedef enum 
    {
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

    typedef enum 
    {
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



    extern const char Data_Folder_Name[];
    extern const Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL];
    extern const Last_Weekday_Lookup_t Last_Weekday_Names[TZDB_WEEKDAY_TOTAL];
    extern const Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL];

    extern uint8_t Data_FileName[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

    void Initial_FileNames(void);
    char* Parse_Version();
    ISO3166_Entry_t* Parse_ISO3166Tab(int* iso3166_Count);
    ZoneTab_Entry_t* Parse_ZoneTab(int* size);
   

#ifdef __cplusplus
}
#endif

#endif