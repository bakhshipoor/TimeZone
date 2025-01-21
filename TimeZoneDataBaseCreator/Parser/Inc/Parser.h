#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/Parse_Version.h"
#include "../Inc/Parse_ISO3166tab.h"
#include "../Inc/Parse_Zonetab.h"
#include "../Inc/Parse_Rules.h"
#include "../Inc/Parse_Zones.h"
#include "../Inc/Parse_Link.h"


    typedef struct
    {
        Version_t* Version;
        COUNTER ISO3166_Count;
        ISO3166_Entry_t* ISO3166;
        COUNTER Zonetab_Count;
        ZoneTab_Entry_t* Zonetab;
        COUNTER Rules_Count;
        Rule_Entry_t* Rules;
        COUNTER Zones_Count;
        Zone_Entry_t* Zones;
        COUNTER Links_Count;
        Link_Entry_t* Links;
    }Parse_Data_t;

    

    BOOL Initial_FileNames(CONST CHAR** data_folder_path);

    Parse_Data_t* Parse_Data(CONST CHAR** data_folder_path);

#ifdef __cplusplus
}
#endif

#endif