#include "../Inc/Parser.h"


// Data Files
CHAR Data_File[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];

BOOL Initial_FileNames(CONST CHAR* data_folder_path)
{
    if (data_folder_path == NULL)
    {
        return FALSE;
    }
    LENGHT path_lenght = (LENGHT)strlen(data_folder_path);
    CHAR* folder_path = (CHAR*)malloc((path_lenght + 1) * sizeof(CHAR));
    if (folder_path == NULL || path_lenght == 0)
    {
        return FALSE;
    }
    sprintf(folder_path, "%s", data_folder_path);
    path_lenght = (LENGHT)strlen(folder_path);
    if (folder_path[path_lenght - 1] == '/' || folder_path[path_lenght - 1] == '\\')
    {
        folder_path[path_lenght - 1] = '\0';
    }

    COUNTER file_index = 0;

    for (file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        sprintf_s(Data_File[file_index], MAX_LENGHT_FILE_NAME, "%s/%s", folder_path, Data_Files_Name[file_index]);
    }

    for (file_index = 0; file_index < DATA_FILES_COUNT; file_index++)
    {
        FILE* data_File = fopen(Data_File[file_index], "r");
        if (!data_File)
        {
            return FALSE;
        }
        fclose(data_File);
    }
    return TRUE;
}

Parse_Data_t* Parse_Data(CONST CHAR* data_folder_path)
{
    Parse_Data_t* data = (Parse_Data_t*)malloc(sizeof(Parse_Data_t));
    if (data == NULL)
    {
        return NULL;
    }

    if (!Initial_FileNames(data_folder_path))
    {
        return NULL;
    }

    data->Version = Parse_Version();
    data->ISO3166 = Parse_ISO3166Tab(&data->ISO3166_Count);
    data->Zonetab = Parse_ZoneTab(&data->Zonetab_Count);
    data->Rules = Parse_Rules(&data->Rules_Count);
    data->Zones = Parse_Zones(&data->Zones_Count);
    data->Links = Parse_Links(&data->Links_Count);

    return data;
}