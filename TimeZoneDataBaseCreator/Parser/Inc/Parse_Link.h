#ifndef PARSE_LINK_H
#define PARSE_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Common.h"

    typedef struct
    {
        uint8_t Field[MAX_LENGHT_DATA_FIELD];
        uint8_t Target[MAX_LENGHT_DATA_FIELD];
        uint8_t Link_Name[MAX_LENGHT_DATA_FIELD];
        uint8_t Target1[MAX_LENGHT_DATA_FIELD];
    } Link_Data_t;

    typedef struct
    {
        uint8_t* Target;
        uint8_t* Link_Name;
        uint8_t* Target1;
    }Link_Entry_t;

    Link_Entry_t* Parse_Links(int32_t* links_Count);
    Link_Data_t Parse_Link_Data(const char* line);

#ifdef __cplusplus
}
#endif

#endif