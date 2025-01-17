#ifndef PARSE_VERSION_H
#define PARSE_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"



    typedef struct
    {
        uint32_t Major;
        uint8_t* Minor;
    }Version_t;

    Version_t Parse_Version(void);

#ifdef __cplusplus
}
#endif

#endif