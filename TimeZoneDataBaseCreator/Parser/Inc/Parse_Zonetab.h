#ifndef PARSE_ZONETAB_H
#define PARSE_ZONETAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* Coordinates;
        CHAR* TZ_Identifier;
        CHAR* Comments;
    } ZoneTab_Data_t;

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* TZ_Identifier;
        CHAR* Comments;
        LOCATION Latitude;
        LOCATION Longitude;
    } ZoneTab_Entry_t;

    ZoneTab_Entry_t* Parse_ZoneTab(COUNTER* zonetab_count);
    ZoneTab_Data_t* Parse_ZoneTab_Data(CONST CHAR** line);
    VOID Parse_ZoneTab_Data_Cordinates(ZoneTab_Entry_t** zone_tab, ZoneTab_Data_t** zonetab_data);
    VOID Parse_Free_ZoneTab_Data(ZoneTab_Data_t** zonetab_data);

#ifdef __cplusplus
}
#endif

#endif#pragma once
