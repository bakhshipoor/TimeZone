#ifndef PARSE_ZONETAB_H
#define PARSE_ZONETAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        uint8_t* Country_Code;
        uint8_t* TZ_Identifier;
        uint8_t* Comments;
        double Latitude;
        double Longitude;

    } ZoneTab_Entry_t;

    ZoneTab_Entry_t* Parse_ZoneTab(int32_t* zonetab_count);

#ifdef __cplusplus
}
#endif

#endif#pragma once
