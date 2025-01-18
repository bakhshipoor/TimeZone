#include "../Inc/Parser.h"


// Data Files
uint8_t Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

bool Initial_FileNames(const char* data_folder_path)
{
    if (data_folder_path == NULL)
    {
        return false;
    }
    int32_t path_lenght = strlen(data_folder_path);
    char* folder_path = (char*)malloc((path_lenght + 1) * sizeof(char));
    if (folder_path == NULL || path_lenght == 0)
    {
        return false;
    }
    sprintf(folder_path, "%s", data_folder_path);
    path_lenght = strlen(folder_path);
    if (folder_path[path_lenght - 1] == '/' || folder_path[path_lenght - 1] == '\\')
    {
        folder_path[path_lenght - 1] = '\0';
    }

    int file_index = 0;

    for (file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        sprintf_s(Data_File[file_index], MAX_LENGHT_FILE_NAME, "%s/%s", folder_path, Data_Files_Name[file_index]);
    }

    for (file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        FILE* data_File = fopen(Data_File[file_index], "r");
        if (!data_File)
        {
            return false;
        }
        fclose(data_File);
    }
}

Parse_Data_t* Parse_Data(const char* data_folder_path)
{
    Parse_Data_t data = { 0 };

    if (!Initial_FileNames(data_folder_path))
    {
        return NULL;
    }

    data.Version = Parse_Version();
    data.ISO3166 = Parse_ISO3166Tab(&data.ISO3166_Count);
    data.Zonetab = Parse_ZoneTab(&data.Zonetab_Count);
    data.Rules = Parse_Rules(&data.Rules_Count);
    data.Zones = Parse_Zones(&data.Zones_Count);

    return &data;
}