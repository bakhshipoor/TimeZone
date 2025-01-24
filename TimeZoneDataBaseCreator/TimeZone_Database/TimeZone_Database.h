// This code uses the IANA Time Zone Database for accurate time zone handling.
// 
// References: 
// - Database: https://www.iana.org/time-zones 
// - Library: https://github.com/eggert/tz 
// - For more details: https://en.wikipedia.org/wiki/List_of_zones_database_time_zones 

#ifndef TIME_ZONE_DATABASE_H
#define  TIME_ZONE_DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define TZDB_VERSION_MAJOR                 2025
#define TZDB_VERSION_MINOR                 "a"


// Enforces strict memory alignment for data, preventing potential performance issues 
// and unexpected behavior in microcontroller environments. 
// When using compiler optimizations like -Os, data may not be naturally aligned 
// to word boundaries, leading to inefficient memory access. 
// This macro provides a convenient way to specify alignment attributes 
// for variables and data structures. 
// For information on memory layout and section placement, see LinkerScript.ld. 
// Example: 
//   __attribute__((section("ExtFlashSection"))) __attribute__((aligned(0x4))) uint64_t myAlignedData; 
#define TZDB_ATTRIBUTE_MEM_ALIGN      

#define TZDB_ZONES_INFO_COUNT         417
#define TZDB_ZONES_DATA_COUNT         1942
#define TZDB_RULES_INFO_COUNT         134
#define TZDB_RULES_DATA_COUNT         2101


    typedef struct
    {
        int32_t time_zone_id;
        uint8_t time_zone_identifier[31];
        uint8_t country_code[3];
        uint8_t country_name[43];
        double latitude;
        double longitude;
        bool has_data;
        uint8_t linked_tz_identifier[21];
        int32_t data_count;
        int32_t year_begin;
        int32_t year_end;
        uint8_t comments[74];
    } tzdb_zone_info_t;

    typedef struct
    {
        int32_t time_zone_id;
        int64_t standard_offset;
        bool has_rule;
        uint8_t* rule_name[13];
        int64_t save_hour;
        uint8_t* format[10];
        int32_t year;
        uint8_t month;
        uint8_t day;
        uint8_t weekday;
        bool weekday_isafterorequal_day;
        int64_t hour;
        bool hour_isUTC;
        uint8_t* comments[24];
    } tzdb_zone_data_t;

    typedef struct
    {
        uint8_t* rule_id;
        uint8_t* name;
        uint8_t* years_count;
        uint8_t* year_begin;
        uint8_t* year_end;
    } tzdb_rule_info_t;


    typedef struct
    {
        uint8_t* rule_id;
        uint8_t* from;
        uint8_t* to;
        uint8_t* month;
        uint8_t* day;
        uint8_t* weekday;
        uint8_t* weekday_isafterorequal_day;
        uint8_t* hour;
        uint8_t* hour_isUTC;
        uint8_t* save_hour;
        uint8_t* letter;
        uint8_t* comment;
    } tzdb_rule_data_t;

    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_info[TZDB_ZONES_INFO_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_data_t tzdb_zones_data[TZDB_ZONES_DATA_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_info_t tzdb_rules_info[TZDB_RULES_INFO_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_data_t tzdb_rules_data[TZDB_RULES_DATA_COUNT];

#ifdef __cplusplus
}
#endif
#endif /* TIME_ZONE_DATABASE_H */
