#ifndef PARSE_ISO3166TAB_H
#define PARSE_ISO3166TAB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* Country_Name;
    } ISO3166_Entry_t;

    ISO3166_Entry_t* Parse_ISO3166Tab(COUNTER* iso3166_Count);

#ifdef __cplusplus
}
#endif

#endif#pragma once
