#include "../Inc/Parser.h"


// Data Files
uint8_t Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

void Initial_FileNames(void)
{
    for (int file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        sprintf_s(Data_File[file_index], MAX_LENGHT_FILE_NAME, "%s/%s", Data_Folder_Path, Data_Files_Name[file_index]);
    }
}

void Parse_Data(void)
{
    Parse_Data_t data = { 0 };

    Initial_FileNames();

    data.Version = Parse_Version();
    data.ISO3166 = Parse_ISO3166Tab(&data.ISO3166_Count);
    data.Zonetab = Parse_ZoneTab(&data.Zonetab_Count);
    data.Rules = Parse_Rules(&data.Rules_Count);
    data.Zones = Parse_Zones(&data.Zones_Count);
}