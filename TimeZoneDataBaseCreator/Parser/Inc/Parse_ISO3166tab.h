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
    } ISO3166_Data_t;

    typedef struct
    {
        CHAR* Country_Code;
        CHAR* Country_Name;
    } ISO3166_Entry_t;

    ISO3166_Entry_t* Parse_ISO3166Tab(COUNTER* iso3166_Count);

    ISO3166_Data_t* Parse_ISO3166_Data(CONST CHAR** line);
    VOID Parse_Free_ISO3166_Data(ISO3166_Data_t** iso_data);

#ifdef __cplusplus
}
#endif

#endif#pragma once
