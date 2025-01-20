#ifndef PARSE_LINK_H
#define PARSE_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        CHAR* Field;
        CHAR* Target;
        CHAR* Link_Name;
        CHAR* Target1;
    } Link_Data_t;

    typedef struct
    {
        CHAR* Target;
        CHAR* Link_Name;
        CHAR* Target1;
    }Link_Entry_t;

    Link_Entry_t* Parse_Links(COUNTER* links_Count);
    Link_Data_t* Parse_Link_Data(CONST CHAR* line);
    VOID Parse_Free_Link_Data(Link_Data_t** link_data);

#ifdef __cplusplus
}
#endif

#endif