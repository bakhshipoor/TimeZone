#ifndef PARSE_LINK_H
#define PARSE_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

   

    typedef struct
    {
        CHAR* Target;
        CHAR* Link_Name;
        CHAR* Target1;
    }Link_Entry_t;

    Link_Entry_t* Parse_Links(CONST CHAR** line, Link_Entry_t** links_list, COUNTER* links_count);
    

#ifdef __cplusplus
}
#endif

#endif