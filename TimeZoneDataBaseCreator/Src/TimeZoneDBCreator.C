
#include "../Inc/TimeZoneDBCreator.h"
#include "../Inc/Parse_Version.h"
#include "../Inc/Parse_ISO3166tab.h"
#include "../Inc/Parse_Zonetab.h"
#include "../Inc/Parse_Rules.h"
#include "../Inc/Parse_Zones.h"

// Data Files
uint8_t Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

void Initial_FileNames(void)
{
    for (int file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        sprintf_s(Data_File[file_index], MAX_LENGHT_FILE_NAME, "../%s/%s", Data_Folder_Name, Data_Files_Name[file_index]);
    }
}

int main()
{
    Initial_FileNames();

    Version_t version = Parse_Version();

    int32_t iso3166_Count = 0;
    ISO3166_Entry_t* iso3166_list = Parse_ISO3166Tab(&iso3166_Count);

    int32_t zoneCount = 0;
    ZoneTab_Entry_t* tab = Parse_ZoneTab(&zoneCount);

    int32_t rules_Count = 0;
    Rule_Entry_t* rules_list = Parse_Rules(&rules_Count);

    int32_t zone_Count = 0;
    Zone_Entry_t* zones_list = Parse_Zones(&zone_Count);


    while (1);
    return 0;
}

