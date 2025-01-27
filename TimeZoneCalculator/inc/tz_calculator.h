#ifndef TIME_ZONE_CALCULATOR_H
#define TIME_ZONE_CALCULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "../time_zone.h"
#include "../inc/tz_calendar.h"

    typedef struct
    {
        int32_t* tz_id;
        uint8_t** tz_identifier;
        int32_t* zone_data_count;
        int32_t zone_rules_count;
        int32_t* zone_rules_id;

        int32_t* g_year; 
        uint8_t* g_month; 
        uint8_t* g_day;
        uint8_t* utc_hour;
        uint8_t* utc_minute;
        uint8_t* utc_second;

        
        uint32_t now_jdn;
        double now_jd;
    }tz_inititial_t;

    typedef struct
    {
        int32_t* zone_data_count;
        tzdb_zone_data_t** zone_data;
    }tz_zone_data_t;

    typedef struct
    {
        int32_t rules_data_count;
        tzdb_rule_data_t** rule_data;
    }tz_rule_data_t;

    extern tz_inititial_t* tz_inititial;
    extern tz_zone_data_t* tz_zone_data;
    extern tz_rule_data_t* tz_rule_data;

    int32_t* get_zone_id_identifier(uint8_t** tz_identifier);
    int32_t* get_zone_data_count_identifier(uint8_t** tz_identifier);
    tz_zone_data_t* get_zone_data(int32_t* zone_data_count, int32_t* tz_id);
    tz_rule_data_t* get_rule_data(tz_inititial_t* tz_init);





    void free_tz_initial(tz_inititial_t* tz_inititial);
    void free_tz_zone_data(tz_zone_data_t* tz_zone_data);
    void free_tz_rule_data(tz_rule_data_t* rule_data);



#ifdef __cplusplus
}
#endif

#endif