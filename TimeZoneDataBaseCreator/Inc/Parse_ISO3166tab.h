#ifndef PARSE_ISO3166TAB_H
#define PARSE_ISO3166TAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"

    typedef struct
    {
        uint8_t* Country_Code;
        uint8_t* Country_Name;
    } ISO3166_Entry_t;

    ISO3166_Entry_t* Parse_ISO3166Tab(int32_t* iso3166_Count);

#ifdef __cplusplus
}
#endif

#endif#pragma once
