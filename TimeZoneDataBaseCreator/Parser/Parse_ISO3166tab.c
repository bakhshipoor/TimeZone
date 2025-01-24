#include "Parse_ISO3166tab.h"

typedef struct
{
    CHAR* Country_Code;
    CHAR* Country_Name;
} ISO3166_Data_t;

STATIC ISO3166_Data_t* Parse_ISO3166_Data(CONST CHAR** line);
STATIC VOID Free_ISO3166_Data(ISO3166_Data_t* iso_data);
STATIC VOID Free_ISO3166_Tab(ISO3166_Entry_t* iso_tab);

VOID Parse_ISO3166Tab(CONST CHAR** line, ISO3166_Entry_t** iso3166_list, COUNTER* iso3166_count)
{
    ISO3166_Data_t* iso_data = Parse_ISO3166_Data(line);
    if (iso_data == NULL)
    {
        return;
    }

    ISO3166_Entry_t* iso = (ISO3166_Entry_t*)malloc(sizeof(ISO3166_Entry_t));
    if (iso == NULL)
    {
        Free_ISO3166_Data(iso_data);
        return;
    }

    iso->Country_Code = _strdup(iso_data->Country_Code);
    iso->Country_Name = _strdup(iso_data->Country_Name);

    if (iso->Country_Code == NULL || iso->Country_Name == NULL)
    {
        Free_ISO3166_Tab(iso);
        Free_ISO3166_Data(iso_data);
        return;
    }

    ISO3166_Entry_t* iso_list = realloc(*iso3166_list, (*iso3166_count + 1) * sizeof(ISO3166_Entry_t));
    if (iso_list == NULL)
    {
        Free_ISO3166_Tab(iso);
        Free_ISO3166_Data(iso_data);
        return;
    }

    *iso3166_list = iso_list;
    (*iso3166_list)[*iso3166_count] = *iso;
    (*iso3166_count)++;

    free(iso);
    Free_ISO3166_Data(iso_data);

    return;
}

STATIC ISO3166_Data_t* Parse_ISO3166_Data(CONST CHAR** line)
{
    ISO3166_Data_t* iso_data = (ISO3166_Data_t*)malloc(sizeof(ISO3166_Data_t));
    if (iso_data == NULL)
    {
        return NULL;
    }

    iso_data->Country_Code = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    iso_data->Country_Name = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    if (iso_data->Country_Code == NULL || iso_data->Country_Name == NULL)
    {
        Free_ISO3166_Data(&iso_data);
        return NULL;
    }

    if (sscanf(*line, "%s\t%[^\n]", iso_data->Country_Code, iso_data->Country_Name) != 2)
    {
        Free_ISO3166_Data(&iso_data);
        return NULL;
    }

    return iso_data;
}

STATIC VOID Free_ISO3166_Data(ISO3166_Data_t* iso_data)
{
    if (iso_data != NULL)
    {
        CHAR* fields[] = { iso_data->Country_Code, iso_data->Country_Name };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            free(fields[i]);
        }
        free(iso_data);
    }
}

STATIC VOID Free_ISO3166_Tab(ISO3166_Entry_t* iso_tab)
{
    if (iso_tab != NULL)
    {
        CHAR* fields[] = { iso_tab->Country_Code, iso_tab->Country_Name };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
            }
        }
        free(iso_tab);
    }
}