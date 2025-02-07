#ifndef PARSE_ZONETAB_H
#define PARSE_ZONETAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

   

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* TZ_Identifier;
        CHAR* Comments;
        LOCATION Latitude;
        LOCATION Longitude;
    } ZoneTab_Entry_t;

    VOID Parse_ZoneTab(CHAR** line, ZoneTab_Entry_t** zonetab_list, COUNTER* zonetab_count);
    

#ifdef __cplusplus
}
#endif

#endif#pragma once
