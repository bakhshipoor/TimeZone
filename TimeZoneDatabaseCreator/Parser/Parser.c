#include "Parser.h"

CHAR* line;
// Data Files
CHAR Data_File[FILE_TOTAL][MAX_LENGTH_FILE_NAME];

BOOL Initial_FileNames(CONST CHAR** data_folder_path)
{
    if (*data_folder_path == NULL)
    {
        return FALSE;
    }
    LENGHT path_lenght = (LENGHT)strlen(*data_folder_path);
    CHAR* folder_path = (CHAR*)malloc((path_lenght + 1) * sizeof(CHAR));
    if (folder_path == NULL || path_lenght == 0)
    {
        return FALSE;
    }
    sprintf(folder_path, "%s", *data_folder_path);
    path_lenght = (LENGHT)strlen(folder_path);
    if (folder_path[path_lenght - 1] == '/' || folder_path[path_lenght - 1] == '\\')
    {
        folder_path[path_lenght - 1] = '\0';
    }

    COUNTER file_index = 0;

    for (file_index = 0; file_index < FILE_TOTAL; file_index++)
    {
        sprintf_s(Data_File[file_index], MAX_LENGTH_FILE_NAME, "%s/%s", folder_path, Data_Files_Name[file_index]);
    }

    for (file_index = 0; file_index < FILE_TOTAL; file_index++)
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

Parse_Data_t* Parse_Data(CONST CHAR** data_folder_path)
{
    if (!Initial_FileNames(data_folder_path))
    {
        return NULL;
    }

    Parse_Data_t* data = (Parse_Data_t*)calloc(1, sizeof(Parse_Data_t));
    if (data == NULL)
    {
        return NULL;
    }

    line = (CHAR*)calloc(MAX_LENGTH_LINE, sizeof(CHAR));
    if (line == NULL)
    {
        free(data);
        data = NULL;
        return NULL;
    }

    FILE* data_File;
    for (COUNTER dataFile_index = FILE_VERSION; dataFile_index < FILE_TOTAL; dataFile_index++)
    {
        data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue;
        }

        while (fgets(line, MAX_LENGTH_LINE, data_File))
        {
            if (strlen(line) < 4 || line[0] == '#')
            {
                continue;
            }

            if (dataFile_index == FILE_VERSION)
            {
                Parse_Version(&line, &data->Version);
                break;
            }
            else if (dataFile_index == FILE_ISO3166TAB)
            {
                Parse_ISO3166Tab(&line, &data->ISO3166, &data->ISO3166_Count);
            }
            else if (dataFile_index == FILE_ZONETAB)
            {
                Parse_ZoneTab(&line, &data->Zonetab, &data->Zonetab_Count);
            }
            /*else if (dataFile_index == FILE_BACKWARD)
            {
                
            }*/
            else
            {
                Parse_Links(&line, &data->Links, &data->Links_Count);
                Parse_Rules(&line, &data->Rules, &data->Rules_Count);
                Parse_Zones(&line, &data->Zones, &data->Zones_Count);
            }
        }
        fclose(data_File);
    }

    free(line);
    line = NULL;

    return data;
}