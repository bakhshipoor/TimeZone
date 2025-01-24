#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Parse_Version.h"
#include "Parse_ISO3166tab.h"
#include "Parse_Zonetab.h"
#include "Parse_Links.h"
#include "Parse_Rules.h"
#include "Parse_Zones.h"



    typedef struct
    {
        Version_t* Version;
        COUNTER ISO3166_Count;
        ISO3166_Entry_t* ISO3166;
        COUNTER Zonetab_Count;
        ZoneTab_Entry_t* Zonetab;
        COUNTER Links_Count;
        Link_Entry_t* Links;
        COUNTER Rules_Count;
        Rule_Entry_t* Rules;
        COUNTER Zones_Count;
        Zone_Entry_t* Zones;
    }Parse_Data_t;

    

    BOOL Initial_FileNames(CONST CHAR** data_folder_path);

    Parse_Data_t* Parse_Data(CONST CHAR** data_folder_path);

#ifdef __cplusplus
}
#endif

#endif