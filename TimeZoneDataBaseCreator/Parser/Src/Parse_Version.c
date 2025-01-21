#include "../Inc/Parse_Version.h"

Version_t* Parse_Version(VOID)
{
    Version_t* version = (Version_t*)malloc(sizeof(Version_t));
    if (version == NULL)
    {
        return NULL;
    }

    FILE* data_File = fopen(Data_File[0], "r");
    if (!data_File)
    {
        free(version);
        version = NULL;
        return version;
    }
    
    fgets(line, sizeof(line), data_File);

    Version_Data_t* version_data = Parse_Version_Data(&line);
    if (version_data == NULL)
    {
        free(version);
        version = NULL;
    }
    
    if (strlen(version_data->Major) == 4)
    {
        version->Major = atoi(version_data->Major);
    }
    else
    {
        version->Major = 0;
    }
    version->Minor = (CHAR*)malloc((strlen(version_data->Minor) + 1) * sizeof(CHAR));
    if (version->Minor != NULL)
    {
        sprintf(version->Minor, "%s", version_data->Minor);
    }
    Parse_Free_Version_Data(&version_data);
    fclose(data_File);
    return version;
}

Version_Data_t* Parse_Version_Data(CONST CHAR** line)
{
    Version_Data_t* version_data = (Version_Data_t*)malloc(sizeof(Version_Data_t));
    if (version_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    version_data->Major = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    version_data->Minor = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    if (version_data->Major == NULL || version_data->Minor == NULL)
    {
        return NULL;
    }

    scan_lenght = sscanf(*line, "%4s%s",
        version_data->Major,
        version_data->Minor);

    sprintf(version_data->Major, "%s", version_data->Major);
    sprintf(version_data->Minor, "%s", version_data->Minor);
 
    return version_data;
}

VOID Parse_Free_Version_Data(Version_Data_t** version_data)
{
    if (*version_data != NULL)
    {
        if ((*version_data)->Major != NULL)
        {
            free((*version_data)->Major);
            (*version_data)->Major = NULL;
        }
        if ((*version_data)->Minor != NULL)
        {
            free((*version_data)->Minor);
            (*version_data)->Minor = NULL;
        }
        free((*version_data));
        (*version_data) = NULL;
    }
}