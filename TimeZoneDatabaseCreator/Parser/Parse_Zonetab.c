#include "Parse_Zonetab.h"

typedef struct
{
    CHAR* Country_Code;
    CHAR* Coordinates;
    CHAR* TZ_Identifier;
    CHAR* Comments;
} ZoneTab_Data_t;

STATIC ZoneTab_Data_t* Parse_ZoneTab_Data(CONST CHAR** line);
STATIC VOID Parse_ZoneTab_Data_Cordinates(ZoneTab_Entry_t* zone_tab, ZoneTab_Data_t* zonetab_data);
STATIC VOID Free_ZoneTab_Data(ZoneTab_Data_t* zonetab_data);
STATIC VOID Free_Zone_Tab(ZoneTab_Entry_t* zone_tab);

VOID Parse_ZoneTab(CHAR** line, ZoneTab_Entry_t** zonetab_list, COUNTER* zonetab_count)
{
    ZoneTab_Data_t* zonetab_data = Parse_ZoneTab_Data(line);
    if (zonetab_data == NULL)
    {
        return;
    }

    ZoneTab_Entry_t* zone_tab = (ZoneTab_Entry_t*)calloc(1, sizeof(ZoneTab_Entry_t));
    if (zone_tab == NULL)
    {
        Free_ZoneTab_Data(zonetab_data);
        return;
    }

    zone_tab->Country_Code = _strdup(zonetab_data->Country_Code);
    zone_tab->TZ_Identifier = _strdup(zonetab_data->TZ_Identifier);
    zone_tab->Comments = _strdup(zonetab_data->Comments);

    if (zone_tab->Country_Code == NULL || zone_tab->TZ_Identifier == NULL || zone_tab->Comments == NULL)
    {
        Free_Zone_Tab(zone_tab);
        Free_ZoneTab_Data(zonetab_data);
        return;
    }

    Parse_ZoneTab_Data_Cordinates(zone_tab, zonetab_data);

    ZoneTab_Entry_t* tz_list = realloc(*zonetab_list, (*zonetab_count + 1) * sizeof(ZoneTab_Entry_t));
    if (tz_list == NULL)
    {
        Free_Zone_Tab(zone_tab);
        Free_ZoneTab_Data(zonetab_data);
        return;
    }

    *zonetab_list = tz_list;
    (*zonetab_list)[*zonetab_count] = *zone_tab;
    (*zonetab_count)++;

    free(zone_tab);
    Free_ZoneTab_Data(zonetab_data);

    return;
}

STATIC ZoneTab_Data_t* Parse_ZoneTab_Data(CONST CHAR** line)
{
    ZoneTab_Data_t* zonetab_data = (ZoneTab_Data_t*)malloc(sizeof(ZoneTab_Data_t));
    if (zonetab_data == NULL)
    {
        return NULL;
    }

    zonetab_data->Country_Code = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    zonetab_data->Coordinates = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    zonetab_data->TZ_Identifier = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    zonetab_data->Comments = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));

    if (zonetab_data->Country_Code == NULL ||
        zonetab_data->Coordinates == NULL ||
        zonetab_data->TZ_Identifier == NULL ||
        zonetab_data->Comments == NULL)
    {
        Free_ZoneTab_Data(zonetab_data);
        return NULL;
    }

    if (sscanf(*line, "%s\t%s\t%s\t%[^\n]",
        zonetab_data->Country_Code,
        zonetab_data->Coordinates,
        zonetab_data->TZ_Identifier,
        zonetab_data->Comments) < 3)
    {
        Free_ZoneTab_Data(zonetab_data);
        return NULL;
    }

    return zonetab_data;
}

STATIC VOID Parse_ZoneTab_Data_Cordinates(ZoneTab_Entry_t* zone_tab, ZoneTab_Data_t* zonetab_data)
{
    CHAR lat_Sign = 0, long_Sign = 0;
    INT lat_Hour = 0, lat_Minute = 0, lat_Second = 0;
    INT long_Hour = 0, long_Minute = 0, long_Second = 0;
    LOCATION latitude = 0.0, longitude = 0.0;

    if (sscanf(zonetab_data->Coordinates, "%c%2d%2d%2d%c%3d%2d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &lat_Second, &long_Sign, &long_Hour, &long_Minute, &long_Second) != 8)
    {
        if (sscanf(zonetab_data->Coordinates, "%c%2d%2d%c%3d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &long_Sign, &long_Hour, &long_Minute) != 6)
        {
            lat_Sign = '+';
            lat_Hour = lat_Minute = lat_Second = 0;
            long_Sign = '+';
            long_Hour = long_Minute = long_Second = 0;
        }
    }

    latitude = lat_Hour + lat_Minute / 60.0 + lat_Second / 3600.0;
    longitude = long_Hour + long_Minute / 60.0 + long_Second / 3600.0;
    latitude = (lat_Sign == '+') ? latitude : -latitude;
    longitude = (long_Sign == '+') ? longitude : -longitude;

    zone_tab->Latitude = latitude;
    zone_tab->Longitude = longitude;

}

STATIC VOID Free_ZoneTab_Data(ZoneTab_Data_t* zonetab_data)
{
    if (zonetab_data != NULL)
    {
        if (zonetab_data->Country_Code != NULL)
        {
            free(zonetab_data->Country_Code);
            zonetab_data->Country_Code = NULL;
        }
        if (zonetab_data->Coordinates != NULL)
        {
            free(zonetab_data->Coordinates);
            zonetab_data->Coordinates = NULL;
        }
        if (zonetab_data->TZ_Identifier != NULL)
        {
            free(zonetab_data->TZ_Identifier);
            zonetab_data->TZ_Identifier = NULL;
        }
        if (zonetab_data->Comments != NULL)
        {
            free(zonetab_data->Comments);
            zonetab_data->Comments = NULL;
        }
        
        free(zonetab_data);
        zonetab_data = NULL;
    }
}

STATIC VOID Free_Zone_Tab(ZoneTab_Entry_t* zone_tab)
{
    if (zone_tab != NULL)
    {
        CHAR* fields[] = { zone_tab->Country_Code, zone_tab->TZ_Identifier, zone_tab->Comments };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
            }
        }
        free(zone_tab);
    }
}