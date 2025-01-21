#include "../Inc/Parse_ISO3166tab.h"

ISO3166_Entry_t* Parse_ISO3166Tab(COUNTER* iso3166_Count)
{
    FILE* iso3166_File = fopen(Data_File[1], "r");
    if (!iso3166_File) 
    {
        return NULL;
    }

    ISO3166_Entry_t* ISO3166_List = NULL;
    *iso3166_Count = 0;

    ISO3166_Data_t* iso_data = NULL;

    while (fgets(line, MAX_LENGTH_LINE, iso3166_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }

        Parse_Free_ISO3166_Data(&iso_data);
        iso_data = Parse_ISO3166_Data(&line);
        if (iso_data == NULL)
        {
            continue;
        }

        ISO3166_Entry_t* iso = (ISO3166_Entry_t*)malloc(sizeof(ISO3166_Entry_t));
        if (iso == NULL)
        {
            continue;
        }

        iso->Country_Code = (CHAR*)malloc((strlen(iso_data->Country_Code) + 1) * sizeof(CHAR));
        iso->Country_Name = (CHAR*)malloc((strlen(iso_data->Country_Name) + 1) * sizeof(CHAR));

        if (iso->Country_Code != NULL)
        {
            sprintf(iso->Country_Code, "%s", iso_data->Country_Code);
        }
        if (iso->Country_Name != NULL)
        {
            sprintf(iso->Country_Name, "%s", iso_data->Country_Name);
        }

        ISO3166_Entry_t* iso_list;
        if ((*iso3166_Count) == 0)
        {
            iso_list = malloc(sizeof(ISO3166_Entry_t));
        }
        else
        {
            iso_list = realloc(ISO3166_List, (*iso3166_Count + 1) * sizeof(ISO3166_Entry_t));
            
        }
        if (iso_list != NULL)
        {
            ISO3166_List = iso_list;
            ISO3166_List[*iso3166_Count] = *iso;
            (*iso3166_Count)++;
        }
    }
    Parse_Free_ISO3166_Data(&iso_data);
    fclose(iso3166_File);
    return ISO3166_List;
}


ISO3166_Data_t* Parse_ISO3166_Data(CONST CHAR** line)
{
    ISO3166_Data_t* iso_data = (ISO3166_Data_t*)malloc(sizeof(ISO3166_Data_t));
    if (iso_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    iso_data->Country_Code = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    iso_data->Country_Name = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    if (iso_data->Country_Code == NULL || iso_data->Country_Name == NULL)
    {
        Parse_Free_ISO3166_Data(&iso_data);
        return NULL;
    }

    scan_lenght = sscanf(*line, "%s\t%[^\n]",
        iso_data->Country_Code,
        iso_data->Country_Name);

    sprintf(iso_data->Country_Code, "%s", iso_data->Country_Code);
    sprintf(iso_data->Country_Name, "%s", iso_data->Country_Name);

    return iso_data;
}

VOID Parse_Free_ISO3166_Data(ISO3166_Data_t** iso_data)
{
    if (*iso_data != NULL)
    {
        if ((*iso_data)->Country_Code != NULL)
        {
            free((*iso_data)->Country_Code);
            (*iso_data)->Country_Code = NULL;
        }
        if ((*iso_data)->Country_Name != NULL)
        {
            free((*iso_data)->Country_Name);
            (*iso_data)->Country_Name = NULL;
        }
        free((*iso_data));
        (*iso_data) = NULL;
    }
}