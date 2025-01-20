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

    LENGHT sscanf_lenght;
    CHAR* country_code = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    CHAR* country_name = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    if (country_code == NULL || country_name == NULL)
    {
        close(iso3166_File);
        return ISO3166_List;
    }

    while (fgets(line, MAX_LENGTH_LINE, iso3166_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }

        sscanf_lenght = sscanf(line, "%s\t%[^\n]", country_code, country_name);

        if (sscanf_lenght < 2)
        {
            continue;
        }

        sprintf_s(country_code, MAX_LENGTH_DATA_FIELD, "%s", country_code);
        sprintf_s(country_name, MAX_LENGTH_DATA_FIELD, "%s", country_name);

        ISO3166_Entry_t entry;

        entry.Country_Code = (CHAR*)malloc((strlen(country_code) + 1) * sizeof(CHAR));
        entry.Country_Name = (CHAR*)malloc((strlen(country_name) + 1) * sizeof(CHAR));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(country_code) + 1, "%s", country_code);
        }
        if (entry.Country_Name != NULL)
        {
            sprintf_s(entry.Country_Name, strlen(country_name) + 1, "%s", country_name);
        }

        if ((*iso3166_Count) == 0)
        {
            ISO3166_Entry_t* tz_list = malloc(sizeof(ISO3166_Entry_t));
            if (tz_list == NULL)
            {
                return NULL;
            }
            ISO3166_List = tz_list;
        }
        else
        {
            ISO3166_Entry_t* tz_list = realloc(ISO3166_List, (*iso3166_Count + 1) * sizeof(ISO3166_Entry_t));
            if (tz_list == NULL)
            {
                return NULL;
            }
            ISO3166_List = tz_list;
        }
        
        ISO3166_List[*iso3166_Count] = entry;
        (*iso3166_Count)++;


    }
    free(country_code);
    country_code = NULL;
    free(country_name);
    country_name = NULL;
    fclose(iso3166_File);
    return ISO3166_List;
}