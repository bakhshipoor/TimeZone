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
#include <stdbool.h>

#define TZDB_VERSION_MAJOR                    2025
#define TZDB_VERSION_MINOR                    "a"

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

#define TZDB_ZONES_INFO_COUNT                 417
#define TZDB_ZONES_DATA_COUNT                 1942
#define TZDB_RULES_INFO_COUNT                 134
#define TZDB_RULES_DATA_COUNT                 2101

#define TZDB_MAX_LENGHT_ZI_IDENTIFIER         31
#define TZDB_MAX_LENGHT_ZI_COUNTRY_CODE       3
#define TZDB_MAX_LENGHT_ZI_COUNTRY_NAME       43
#define TZDB_MAX_LENGHT_ZI_COMMENTS           74
#define TZDB_MAX_LENGHT_ZD_FORMAT             10
#define TZDB_MAX_LENGHT_ZD_COMMENTS           24
#define TZDB_MAX_LENGHT_RI_NAME               13
#define TZDB_MAX_LENGHT_RD_LETTER             6
#define TZDB_MAX_LENGHT_RD_COMMENTS           10

#define TZDB_YEAR_END_MAX                     -1

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
        TZDB_WEEKDAY_NONE = -1,
    } tzdb_weekday_number_e;

    typedef enum
    {
        TZDB_MONTH_NONE = 0,
        TZDB_MONTH_JANUARY,
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
        
        TZDB_MONTH_TOTAL = TZDB_MONTH_DECEMBER,
    } tzdb_month_number_e;

    typedef struct
    {
        int32_t       zone_id;
        uint8_t       zone_identifier[TZDB_MAX_LENGHT_ZI_IDENTIFIER];
        int64_t       std_offset;
        int64_t       dst_offset;
        char          country_code[TZDB_MAX_LENGHT_ZI_COUNTRY_CODE];
        char          country_name[TZDB_MAX_LENGHT_ZI_COUNTRY_NAME];
        double        latitude;
        double        longitude;
        int32_t       linked_zone_id;
        int32_t       data_count;
        int32_t       year_begin;
        int32_t       year_end;
        char          comments[TZDB_MAX_LENGHT_ZI_COMMENTS];
    } tzdb_zone_info_t;

    typedef struct
    {
        int32_t       zone_id;
        int64_t       standard_offset;
        int32_t       rule_id;
        int64_t       save_hour;
        char          format[TZDB_MAX_LENGHT_ZD_FORMAT];
        double        until_jd;
        char          comments[TZDB_MAX_LENGHT_ZD_COMMENTS];
    } tzdb_zone_data_t;

    typedef struct
    {
        int32_t       rule_id;
        char          rule_name[TZDB_MAX_LENGHT_RI_NAME];
        int32_t       data_count;
        int32_t       year_begin;
        int32_t       year_end;
    } tzdb_rule_info_t;

    typedef struct
    {
        int32_t       rule_id;
        int32_t       from;
        int32_t       to;
        int8_t        month;
        int8_t        day;
        int8_t        weekday;
        bool          weekday_isafterorequal_day;
        int64_t       hour;
        bool          hour_isUTC;
        int64_t       save_hour;
        char          letter[TZDB_MAX_LENGHT_RD_LETTER];
        char          comments[TZDB_MAX_LENGHT_RD_COMMENTS];
    } tzdb_rule_data_t;

// If you want to use external memory files instead of the built-in database,
// change the value of USE_EMBEDED_DATABASE to zero.
#define USE_EMBEDED_DATABASE                  1

#if USE_EMBEDED_DATABASE == 1

    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_info[TZDB_ZONES_INFO_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_data_t tzdb_zones_data[TZDB_ZONES_DATA_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_info_t tzdb_rules_info[TZDB_RULES_INFO_COUNT];
    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_data_t tzdb_rules_data[TZDB_RULES_DATA_COUNT];

#else

    extern const char* tzdb_zone_info_file_name;
    extern const char* tzdb_zone_data_file_name;
    extern const char* tzdb_rule_info_file_name;
    extern const char* tzdb_rule_data_file_name;

#endif

#ifdef __cplusplus
}
#endif
#endif /* TIME_ZONE_DATABASE_H */
