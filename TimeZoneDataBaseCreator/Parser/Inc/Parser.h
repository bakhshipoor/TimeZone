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


    typedef struct
    {
        Version_t Version;
        int32_t ISO3166_Count;
        ISO3166_Entry_t* ISO3166;
        int32_t Zonetab_Count;
        ZoneTab_Entry_t* Zonetab;
        int32_t Rules_Count;
        Rule_Entry_t* Rules;
        int32_t Zones_Count;
        Zone_Entry_t* Zones;
    }Parse_Data_t;

    

    void Initial_FileNames(void);

    void Parse_Data(void);

#ifdef __cplusplus
}
#endif

#endif