#include "Parse_Version.h"

typedef struct
{
    CHAR* Major;
    CHAR* Minor;
} Version_Data_t;

static Version_Data_t* Parse_Version_Data(CONST CHAR** line);
static VOID Free_Version_Data(Version_Data_t* version_data);

VOID Parse_Version(CONST CHAR** line, Version_t** version)
{
    *version = (Version_t*)calloc(1, sizeof(Version_t));
    if (version == NULL)
    {
        return;
    }

    Version_Data_t* version_data = Parse_Version_Data(line);
    if (version_data == NULL)
    {
        return;
    }

    if (strlen(version_data->Major) == 4)
    {
        (*version)->Major = atoi(version_data->Major);
    }
    else
    {
        (*version)->Major = 0;
    }
    LENGHT size = (LENGHT)strlen(version_data->Minor) + 1;
    (*version)->Minor = (CHAR*)calloc(size, size * sizeof(CHAR));
    if ((*version)->Minor != NULL)
    {
        strcpy((*version)->Minor, version_data->Minor);
    }
    Free_Version_Data(version_data);
    return;
}

STATIC Version_Data_t* Parse_Version_Data(CONST CHAR** line)
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
        Free_Version_Data(version_data);
        return NULL;
    }

    scan_lenght = sscanf(*line, "%4s%s",
        version_data->Major,
        version_data->Minor);

    sprintf(version_data->Major, "%s", version_data->Major);
    sprintf(version_data->Minor, "%s", version_data->Minor);
 
    return version_data;
}

STATIC VOID Free_Version_Data(Version_Data_t* version_data)
{
    if (version_data != NULL)
    {
        if (version_data->Major != NULL)
        {
            free(version_data->Major);
            version_data->Major = NULL;
        }
        if (version_data->Minor != NULL)
        {
            free(version_data->Minor);
            version_data->Minor = NULL;
        }
        free(version_data);
        version_data = NULL;
    }
}