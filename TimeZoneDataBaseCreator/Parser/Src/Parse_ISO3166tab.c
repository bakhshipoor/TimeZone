#include "../Inc/Parse_ISO3166tab.h"

ISO3166_Entry_t* Parse_ISO3166Tab(int32_t* iso3166_Count)
{
    FILE* iso3166_File = fopen(Data_File[1], "r");
    if (!iso3166_File) {
        perror("Failed to open file");
        return NULL;
    }

    ISO3166_Entry_t* ISO3166_List = NULL;
    *iso3166_Count = 0;
    char line[2048];

    while (fgets(line, sizeof(line), iso3166_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }


        int sscanf_lenght;
        uint8_t country_code[MAX_LENGHT_DATA_FIELD];
        uint8_t country_name[MAX_LENGHT_DATA_FIELD];

        memset(country_code, '\0', MAX_LENGHT_DATA_FIELD);
        memset(country_name, '\0', MAX_LENGHT_DATA_FIELD);

        sscanf_lenght = sscanf(line, "%s\t%[^\n]", country_code, country_name);

        if (sscanf_lenght < 2)
        {
            continue;
        }

        sprintf_s(country_code, MAX_LENGHT_DATA_FIELD, "%s", country_code);
        sprintf_s(country_name, MAX_LENGHT_DATA_FIELD, "%s", country_name);

        ISO3166_Entry_t entry;

        entry.Country_Code = (uint8_t*)malloc((strlen(country_code) + 1) * sizeof(uint8_t));
        entry.Country_Name = (uint8_t*)malloc((strlen(country_name) + 1) * sizeof(uint8_t));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(country_code) + 1, "%s", country_code);
        }
        if (entry.Country_Name != NULL)
        {
            sprintf_s(entry.Country_Name, strlen(country_name) + 1, "%s", country_name);
        }

        ISO3166_Entry_t* tz_list = realloc(ISO3166_List, (*iso3166_Count + 1) * sizeof(ISO3166_Entry_t));
        if (tz_list == NULL)
        {
            return NULL;
        }
        ISO3166_List = tz_list;
        ISO3166_List[*iso3166_Count] = entry;
        (*iso3166_Count)++;
    }

    fclose(iso3166_File);
    return ISO3166_List;
}