#ifndef PARSE_VERSION_H
#define PARSE_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        YEAR Major;
        CHAR* Minor;
    }Version_t;

    VOID Parse_Version(CONST CHAR** line, Version_t** version);
    

#ifdef __cplusplus
}
#endif

#endif