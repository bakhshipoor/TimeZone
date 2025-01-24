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

    VOID Parse_ISO3166Tab(CONST CHAR** line, ISO3166_Entry_t** iso3166_list, COUNTER* iso3166_count);

    

#ifdef __cplusplus
}
#endif

#endif#pragma once
