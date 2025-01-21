#include "../Inc/Parse_Zonetab.h"

ZoneTab_Entry_t* Parse_ZoneTab(COUNTER* zonetab_count)
{
    FILE* zonetab_File = fopen(Data_File[2], "r");
    if (!zonetab_File) 
    {
        return NULL;
    }

    ZoneTab_Entry_t* TimeZone_List = NULL;
    *zonetab_count = 0;

    ZoneTab_Data_t* zonetab_data = NULL;

    while (fgets(line, MAX_LENGTH_LINE, zonetab_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }

        ZoneTab_Entry_t* zone_tab = (ZoneTab_Entry_t*)malloc(sizeof(ZoneTab_Entry_t));
        if (zone_tab == NULL)
        {
            continue;
        }

        Parse_Free_ZoneTab_Data(&zonetab_data);
        zonetab_data = Parse_ZoneTab_Data(&line);
        if (zonetab_data == NULL)
        {
            continue;
        }

        zone_tab->Country_Code = (CHAR*)malloc((strlen(zonetab_data->Country_Code) + 1) * sizeof(CHAR));
        zone_tab->TZ_Identifier = (CHAR*)malloc((strlen(zonetab_data->TZ_Identifier) + 1) * sizeof(CHAR));
        zone_tab->Comments = (CHAR*)malloc((strlen(zonetab_data->Comments) + 1) * sizeof(CHAR));

        if (zone_tab->Country_Code != NULL)
        {
            sprintf(zone_tab->Country_Code, "%s", zonetab_data->Country_Code);
        }
        if (zone_tab->TZ_Identifier != NULL)
        {
            sprintf(zone_tab->TZ_Identifier, "%s", zonetab_data->TZ_Identifier);
        }
        if (zone_tab->Comments != NULL)
        {
            sprintf(zone_tab->Comments, "%s", zonetab_data->Comments);
        }

        Parse_ZoneTab_Data_Cordinates(&zone_tab, &zonetab_data);

        ZoneTab_Entry_t* tz_list;
        if ((*zonetab_count) == 0)
        {
            tz_list = malloc(sizeof(ZoneTab_Entry_t));
        }
        else
        {
            tz_list = realloc(TimeZone_List, (*zonetab_count + 1) * sizeof(ZoneTab_Entry_t));
        }
        if (tz_list != NULL)
        {
            TimeZone_List = tz_list;
            TimeZone_List[*zonetab_count] = *zone_tab;
            (*zonetab_count)++;
        }
    }

    Parse_Free_ZoneTab_Data(&zonetab_data);
    fclose(zonetab_File);
    return TimeZone_List;
}

ZoneTab_Data_t* Parse_ZoneTab_Data(CONST CHAR** line)
{
    ZoneTab_Data_t* zonetab_data = (ZoneTab_Data_t*)malloc(sizeof(ZoneTab_Data_t));
    if (zonetab_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    zonetab_data->Country_Code = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    zonetab_data->Coordinates = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    zonetab_data->TZ_Identifier = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    zonetab_data->Comments = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    

    if (zonetab_data->Country_Code == NULL ||
        zonetab_data->Coordinates == NULL ||
        zonetab_data->TZ_Identifier == NULL ||
        zonetab_data->Comments == NULL)
    {
        Parse_Free_ISO3166_Data(&zonetab_data);
        return NULL;
    }

    scan_lenght = sscanf(*line, "%s\t%s\t%s\t%[^\n]",
        zonetab_data->Country_Code,
        zonetab_data->Coordinates,
        zonetab_data->TZ_Identifier,
        zonetab_data->Comments);

    sprintf(zonetab_data->Country_Code, "%s", zonetab_data->Country_Code);
    sprintf(zonetab_data->Comments, "%s", zonetab_data->Coordinates);
    sprintf(zonetab_data->TZ_Identifier, "%s", zonetab_data->TZ_Identifier);
    sprintf(zonetab_data->Comments, "%s", zonetab_data->Comments);

    return zonetab_data;
}

VOID Parse_ZoneTab_Data_Cordinates(ZoneTab_Entry_t** zone_tab, ZoneTab_Data_t** zonetab_data)
{

    LENGHT sscanf_lenght;
    LENGHT coordinates_lenght = 0;

    CHAR lat_Sign = 0;
    INT lat_Hour = 0;
    INT lat_Minute = 0;
    INT lat_Second = 0;

    CHAR long_Sign = 0;
    INT long_Hour = 0;
    INT long_Minute = 0;
    INT long_Second = 0;

    LOCATION latitude = 0.0;
    LOCATION longitude = 0.0;


    coordinates_lenght = (LENGHT)strlen((*zonetab_data)->Coordinates);
    if (sscanf((*zonetab_data)->Coordinates, "%c%2d%2d%c%3d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &long_Sign, &long_Hour, &long_Minute) == 6)
    {
        lat_Second = 0;
        long_Second = 0;
    }
    else if (sscanf((*zonetab_data)->Coordinates, "%c%2d%2d%2d%c%3d%2d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &lat_Second, &long_Sign, &long_Hour, &long_Minute, &long_Second)==8)
    {
        
    }
    else
    {
        lat_Sign = '+';
        lat_Hour = 0;
        lat_Minute = 0;
        lat_Second = 0;

        long_Sign = '+';
        long_Hour = 0;
        long_Minute = 0;
        long_Second = 0;
    }

    latitude = (lat_Hour * 1.0) + (lat_Minute / 60.0) + (lat_Second / 3600.0);
    longitude = (long_Hour * 1.0) + (long_Minute / 60.0) + (long_Second / 3600.0);
    latitude = lat_Sign == '+' ? latitude : -latitude;
    longitude = long_Sign == '+' ? longitude : -longitude;




    (*zone_tab)->Latitude = latitude;
    (*zone_tab)->Longitude = longitude;

}

VOID Parse_Free_ZoneTab_Data(ZoneTab_Data_t** zonetab_data)
{
    if (*zonetab_data != NULL)
    {
        if ((*zonetab_data)->Country_Code != NULL)
        {
            free((*zonetab_data)->Country_Code);
            (*zonetab_data)->Country_Code = NULL;
        }
        if ((*zonetab_data)->Coordinates != NULL)
        {
            free((*zonetab_data)->Coordinates);
            (*zonetab_data)->Coordinates = NULL;
        }
        if ((*zonetab_data)->TZ_Identifier != NULL)
        {
            free((*zonetab_data)->TZ_Identifier);
            (*zonetab_data)->TZ_Identifier = NULL;
        }
        if ((*zonetab_data)->Comments != NULL)
        {
            free((*zonetab_data)->Comments);
            (*zonetab_data)->Comments = NULL;
        }
        
        free((*zonetab_data));
        (*zonetab_data) = NULL;
    }
}