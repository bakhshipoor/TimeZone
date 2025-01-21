#ifndef PARSE_VERSION_H
#define PARSE_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Major;
        CHAR* Minor;
    } Version_Data_t;

    typedef struct
    {
        YEAR Major;
        CHAR* Minor;
    }Version_t;

    Version_t* Parse_Version(VOID);
    Version_Data_t* Parse_Version_Data(CONST CHAR** line);
    VOID Parse_Free_Version_Data(Version_Data_t** version_data);

#ifdef __cplusplus
}
#endif

#endif