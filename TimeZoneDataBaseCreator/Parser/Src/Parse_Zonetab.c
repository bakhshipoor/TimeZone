#include "../Inc/Parse_Zonetab.h"

ZoneTab_Entry_t* Parse_ZoneTab(int32_t* zonetab_count)
{
    FILE* zonetab_File = fopen(Data_File[2], "r");
    if (!zonetab_File) {
        perror("Failed to open file");
        return NULL;
    }

    ZoneTab_Entry_t* TimeZone_List = NULL;
    *zonetab_count = 0;
    char line[2048];

    while (fgets(line, sizeof(line), zonetab_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }


        int sscanf_lenght;
        uint8_t code[MAX_LENGHT_DATA_FIELD];
        uint8_t coordinates[MAX_LENGHT_DATA_FIELD];
        uint8_t tzid[MAX_LENGHT_DATA_FIELD];
        uint8_t comment[MAX_LENGHT_DATA_FIELD];
        size_t coordinates_lenght = 0;
        char lat_Sign = 0;
        int lat_Hour = 0;
        int lat_Minute = 0;
        int lat_Second = 0;
        char long_Sign = 0;
        int long_Hour = 0;
        int long_Minute = 0;
        int long_Second = 0;
        double latitude = 0.0;
        double longitude = 0.0;

        memset(code, '\0', MAX_LENGHT_DATA_FIELD);
        memset(coordinates, '\0', MAX_LENGHT_DATA_FIELD);
        memset(tzid, '\0', MAX_LENGHT_DATA_FIELD);
        memset(comment, '\0', MAX_LENGHT_DATA_FIELD);

        sscanf_lenght = sscanf(line, "%s\t%s\t%s\t%[^\n]", code, coordinates, tzid, comment);

        if (sscanf_lenght < 3)
        {
            continue;
        }
        sprintf_s(code, MAX_LENGHT_DATA_FIELD, "%s", code);
        sprintf_s(coordinates, MAX_LENGHT_DATA_FIELD, "%s", coordinates);
        sprintf_s(tzid, MAX_LENGHT_DATA_FIELD, "%s", tzid);
        sprintf_s(comment, MAX_LENGHT_DATA_FIELD, "%s", comment);

        coordinates_lenght = strlen(coordinates);
        if (coordinates_lenght == 11)
        {
            sscanf_lenght = sscanf(coordinates, "%c%2d%2d%c%3d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &long_Sign, &long_Hour, &long_Minute);
            if (sscanf_lenght != 6)
            {
                continue;
            }
        }
        else {
            sscanf_lenght = sscanf(coordinates, "%c%2d%2d%2d%c%3d%2d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &lat_Second, &long_Sign, &long_Hour, &long_Minute, &long_Second);
            if (sscanf_lenght != 8)
            {
                continue;
            }
        }
        latitude = (lat_Hour * 1.0) + (lat_Minute / 60.0) + (lat_Second / 3600.0);
        longitude = (long_Hour * 1.0) + (long_Minute / 60.0) + (long_Second / 3600.0);
        latitude = lat_Sign == '+' ? latitude : -latitude;
        longitude = long_Sign == '+' ? longitude : -longitude;

        ZoneTab_Entry_t entry;
        entry.Country_Code = (uint8_t*)malloc((strlen(code) + 1) * sizeof(uint8_t));
        entry.TZ_Identifier = (uint8_t*)malloc((strlen(tzid) + 1) * sizeof(uint8_t));
        entry.Comments = (uint8_t*)malloc((strlen(comment) + 1) * sizeof(uint8_t));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(code) + 1, "%s", code);
        }
        if (entry.TZ_Identifier != NULL)
        {
            sprintf_s(entry.TZ_Identifier, strlen(tzid) + 1, "%s", tzid);
        }
        if (entry.Comments != NULL)
        {
            sprintf_s(entry.Comments, strlen(comment) + 1, "%s", comment);
        }

        entry.Latitude = latitude;
        entry.Longitude = longitude;

        if ((*zonetab_count) == 0)
        {
            ZoneTab_Entry_t* tz_list = malloc(sizeof(ZoneTab_Entry_t));
            if (tz_list == NULL)
            {
                return NULL;
            }
            TimeZone_List = tz_list;
        }
        else
        {
            ZoneTab_Entry_t* tz_list = realloc(TimeZone_List, (*zonetab_count + 1) * sizeof(ZoneTab_Entry_t));
            if (tz_list == NULL)
            {
                return NULL;
            }
            TimeZone_List = tz_list;
        }
        TimeZone_List[*zonetab_count] = entry;
        (*zonetab_count)++;
    }

    fclose(zonetab_File);
    return TimeZone_List;
}