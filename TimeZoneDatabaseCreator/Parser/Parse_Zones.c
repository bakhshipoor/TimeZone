#include "Parse_Zones.h"

typedef struct
{
    CHAR* Field;
    CHAR* Name;
    CHAR* Standard_Offset;
    CHAR* Rules;
    CHAR* Format;
    CHAR* Until;
    CHAR* Comment;
} Zone_Details_t;

CHAR Last_Zone_Name[MAX_LENGTH_DATA_FIELD] = "\0";


STATIC _BOOL Zone_isExist(CONST Zone_Entry_t** zone_list, CONST COUNTER* zones_count, CONST CHAR** zone_name, COUNTER* find_Index);
STATIC _BOOL Zone_Create(Zone_Entry_t** zone_list, COUNTER* zones_count, CONST CHAR** zone_name);
STATIC VOID Parse_Zone_Year_Range(Zone_Entry_t** zone_list, CONST COUNTER* zone_index, CONST Zone_Details_t* zone_data);
STATIC _BOOL Zone_Info_Create(Zone_Data_t** info_list, CONST COUNTER* info_count, CONST Zone_Details_t* zone_data);
STATIC Zone_Details_t* Parse_Zone_Data(CONST CHAR** line);
STATIC HOUR Parse_Zone_Data_Standard_Offset(CONST Zone_Details_t* zone_data);
STATIC Zone_Data_Rule_t Parse_Zone_Data_Rules(CONST Zone_Details_t* zone_data);
STATIC Zone_Data_Until_t Parse_Zone_Info_Until(CONST Zone_Details_t* zone_data);
STATIC VOID Free_Zone_Data(Zone_Details_t* zone_data);


VOID Parse_Zones(CONST CHAR** line, Zone_Entry_t** zones_list, COUNTER* zones_count)
{
    Zone_Details_t* zone_data = Parse_Zone_Data(line);
    if (zone_data == NULL)
    {
        return;
    }

    COUNTER find_index = INDEX_NOT_FOUND;

    if (!Zone_isExist(zones_list, zones_count, &zone_data->Name, &find_index))
    {
        if (Zone_Create(zones_list, zones_count, &zone_data->Name))
        {
            find_index = (*zones_count) - 1;
        }
    }
    if (find_index != INDEX_NOT_FOUND)
    {
        Parse_Zone_Year_Range(zones_list, &find_index, zone_data);
        if (Zone_Info_Create(&(*zones_list)[find_index].Info, &(*zones_list)[find_index].Info_Count, zone_data))
        {
            (*zones_list)[find_index].Info_Count++;
        }
    }
    Free_Zone_Data(zone_data);
}

STATIC _BOOL Zone_isExist(CONST Zone_Entry_t** zone_list, CONST COUNTER* zones_count, CONST CHAR** zone_name, COUNTER* find_Index)
{
    if (zone_list == NULL || zones_count == NULL || zone_name == NULL || find_Index == NULL)
    {
        return _FALSE;
    }

    if (*zone_list != NULL && *zones_count > 0)
    {
        for (int zone_index = 0; zone_index < *zones_count; zone_index++)
        {
            if (strcmp((*zone_list)[zone_index].Name, *zone_name) == 0)
            {
                *find_Index = zone_index;
                return _TRUE;
            }
        }
    }
    return _FALSE;
}

STATIC _BOOL Zone_Create(Zone_Entry_t** zone_list, COUNTER* zones_count, CONST CHAR** zone_name)
{
    if (zone_list == NULL || zones_count == NULL || zone_name == NULL || *zone_name == NULL)
    {
        return _FALSE;
    }

    Zone_Entry_t* zone = (Zone_Entry_t*)calloc(1, sizeof(Zone_Entry_t));
    if (zone == NULL)
    {
        return _FALSE;
    }

    zone->Name = (CHAR*)calloc(utf8_strlen(*zone_name) + 1, sizeof(CHAR));
    if (zone->Name == NULL)
    {
        free(zone);
        return _FALSE;
    }
    strcpy(zone->Name, *zone_name);

    zone->Info_Count = 0;
    zone->Year_Begin = 0;
    zone->Year_End = 0;

    Zone_Entry_t* zones = realloc(*zone_list, (*zones_count + 1) * sizeof(Zone_Entry_t));
    if (zones == NULL)
    {
        free(zone->Name);
        free(zone);
        return _FALSE;
    }

    *zone_list = zones;
    (*zone_list)[*zones_count] = *zone;
    (*zones_count)++;
    free(zone);
    return _TRUE;
}

STATIC VOID Parse_Zone_Year_Range(Zone_Entry_t** zone_list, CONST COUNTER* zone_index, CONST Zone_Details_t* zone_data)
{
    if (zone_list == NULL || zone_index == NULL || zone_data == NULL || *zone_list == NULL)
    {
        return;
    }

    Zone_Data_Until_t until = Parse_Zone_Info_Until(zone_data);
    YEAR year = until.Year;

    Zone_Entry_t* zone = &(*zone_list)[*zone_index];

    if (zone->Year_Begin == 0 || (year != -1 && zone->Year_Begin > year))
    {
        zone->Year_Begin = year;
    }

    if (zone->Year_End == 0 || year == -1 || (year != -1 && zone->Year_End < year))
    {
        zone->Year_End = year;
    }
}

STATIC _BOOL Zone_Info_Create(Zone_Data_t** info_list, CONST COUNTER* info_count, CONST Zone_Details_t* zone_data)
{
    Zone_Data_t* info = (Zone_Data_t*)malloc(sizeof(Zone_Data_t));
    if (info == NULL)
    {
        return _FALSE;
    }
    info->Standard_Offset = Parse_Zone_Data_Standard_Offset(zone_data);
    info->Rule = Parse_Zone_Data_Rules(zone_data);

    info->Format = (CHAR*)malloc((utf8_strlen(zone_data->Format) + 1) * sizeof(CHAR));
    if (info->Format != NULL)
    {
        sprintf(info->Format, "%s", zone_data->Format);
    }
    info->Until = Parse_Zone_Info_Until(zone_data);
    info->Comment = (CHAR*)malloc((utf8_strlen(zone_data->Comment) + 1) * sizeof(CHAR));
    if (info->Comment != NULL)
    {
        if (utf8_strlen(zone_data->Comment) > 0 && zone_data->Comment[0] == ' ')
        {
            strcpy(zone_data->Comment, zone_data->Comment + 1);
        }
        sprintf(info->Comment, "%s", zone_data->Comment);
    }
    Zone_Data_t* z_info;
    if (*info_count == 0)
    {
        z_info = (Zone_Data_t*)malloc(sizeof(Zone_Data_t));
    }
    else
    {
        z_info = (Zone_Data_t*)realloc((*info_list), (*info_count + 1) * sizeof(Zone_Data_t));
    }
    if (z_info != NULL)
    {
        (*info_list) = z_info;
        (*info_list)[*info_count] = *info;
    }
    free(info);
    return _TRUE;
}

STATIC Zone_Details_t* Parse_Zone_Data(CONST CHAR** line)
{
    Zone_Details_t* zone_data = (Zone_Details_t*)calloc(1,sizeof(Zone_Details_t));
    if (zone_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    zone_data->Field = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Name = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Standard_Offset = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Rules = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Format = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Until = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    zone_data->Comment = (CHAR*)calloc( MAX_LENGTH_DATA_FIELD , sizeof(CHAR));

    if (zone_data->Field == NULL ||
        zone_data->Name == NULL ||
        zone_data->Standard_Offset == NULL ||
        zone_data->Rules == NULL ||
        zone_data->Format == NULL ||
        zone_data->Until == NULL ||
        zone_data->Comment == NULL)
    {
        Free_Zone_Data(zone_data);
        return NULL;
    }

    if (strncmp(*line, "\t\t\t", 3) == 0 && (*line)[3] != '#')
    {
        sprintf(zone_data->Field, "%s", "Zone");
        sprintf(zone_data->Name, "%s", Last_Zone_Name);
        scan_lenght = sscanf(*line, "%s\t%s\t%s\t%[^#\n]#%[^\n]",
            zone_data->Standard_Offset,
            zone_data->Rules,
            zone_data->Format,
            zone_data->Until,
            zone_data->Comment);
    }
    else if ((*line)[0] == 'Z')
    {
        scan_lenght = sscanf(*line, "%s\t%s\t%s\t%s\t%s\t%[^#\n]#%[^\n]",
            zone_data->Field,
            zone_data->Name,
            zone_data->Standard_Offset,
            zone_data->Rules,
            zone_data->Format,
            zone_data->Until,
            zone_data->Comment);

        sprintf(Last_Zone_Name, "%s", zone_data->Name);
    }

    sprintf(zone_data->Field, "%s", zone_data->Field);
    sprintf(zone_data->Name, "%s", zone_data->Name);
    sprintf(zone_data->Standard_Offset, "%s", zone_data->Standard_Offset);
    sprintf(zone_data->Rules, "%s", zone_data->Rules);
    sprintf(zone_data->Format, "%s", zone_data->Format);
    sprintf(zone_data->Until, "%s", zone_data->Until);
    sprintf(zone_data->Comment, "%s", zone_data->Comment);

    if (strcmp("Zone", zone_data->Field) != 0 /*|| scan_lenght < 3 || scan_lenght>7*/)
    {
        Free_Zone_Data(zone_data);
        return NULL;
    }

    return zone_data;
}

STATIC HOUR Parse_Zone_Data_Standard_Offset(CONST Zone_Details_t* zone_data)
{
    CHAR s;
    return Parse_Hour((CONST CHAR**) &zone_data->Standard_Offset, &s);
}

STATIC Zone_Data_Rule_t Parse_Zone_Data_Rules(CONST Zone_Details_t* zone_data)
{
    Zone_Data_Rule_t rule = { 0 };

    if (zone_data == NULL || zone_data->Rules == NULL)
    {
        return rule;
    }

    if (strcmp(zone_data->Rules, "-") == 0)
    {
        rule.Has_Rule = _FALSE;
        rule.Rule_Name = (CHAR*)calloc(1, sizeof(CHAR));
        rule.Save_Hour = 0;
    }
    else if (!isalpha(zone_data->Rules[0]))
    {
        rule.Has_Rule = _TRUE;
        rule.Rule_Name = (CHAR*)calloc(1, sizeof(CHAR));
        CHAR s;
        rule.Save_Hour = Parse_Hour((CONST CHAR**) &zone_data->Rules, &s);
    }
    else
    {
        rule.Has_Rule = _TRUE;
        size_t rule_name_length = utf8_strlen(zone_data->Rules) + 1;
        rule.Rule_Name = (CHAR*)malloc(rule_name_length * sizeof(CHAR));
        if (rule.Rule_Name != NULL)
        {
            strcpy(rule.Rule_Name, zone_data->Rules);
        }
        rule.Save_Hour = 0;
    }

    if (rule.Rule_Name == NULL)
    {
        rule.Has_Rule = _FALSE;
        rule.Save_Hour = 0;
    }

    return rule;
}

STATIC Zone_Data_Until_t Parse_Zone_Info_Until(CONST Zone_Details_t* zone_data)
{
    Zone_Data_Until_t until = { 0 };

    if (zone_data == NULL || zone_data->Until == NULL)
    {
        return until;
    }

    //CHAR until_data[4][MAX_LENGTH_DATA_FIELD] = { { 0 } };
    CHAR* until_data_year = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
    CHAR* until_data_month = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
    CHAR* until_data_day = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
    CHAR* until_data_hour = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
    if (until_data_year == NULL ||
        until_data_month == NULL ||
        until_data_day == NULL ||
        until_data_hour == NULL)
    {
        if (until_data_year != NULL)
        {
            free(until_data_year);
        }
        if (until_data_month != NULL)
        {
            free(until_data_month);
        }
        if (until_data_day != NULL)
        {
            free(until_data_day);
        }
        if (until_data_hour != NULL)
        {
            free(until_data_hour);
        }
        return until;
    }



    until.Year = -1;
    until.Month = (MONTH)TZDB_MONTH_NONE;
    until.Day.Day = (DAY)TZDB_DAY_NONE;
    until.Day.Weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
    until.Day.Weekday_isAfterOrEqual_Day = _FALSE;
    until.Hour.Hour = INVALID_HOUR;
    until.Hour.Hour_isUTC = _FALSE;

    int scan_lenght = sscanf(zone_data->Until, "%s\t%s\t%s\t%s", until_data_year, until_data_month, until_data_day, until_data_hour);

    if (scan_lenght >= 1)
    {
        until.Year = atoi(until_data_year);
    }
    if (scan_lenght >= 2)
    {
        until.Month = Parse_Month(&until_data_month);
    }
    if (scan_lenght >= 3)
    {
        Parse_Day_Of_Month(&until_data_day, &until.Day.Day, &until.Day.Weekday, &until.Day.Weekday_isAfterOrEqual_Day);
    }
    if (scan_lenght == 4)
    {
        CHAR s;
        until.Hour.Hour = Parse_Hour(&until_data_hour, &s);
        until.Hour.Hour_isUTC = (s == 'u') ? _TRUE : _FALSE;
    }

    if (until_data_year != NULL)
    {
        free(until_data_year);
    }
    if (until_data_month != NULL)
    {
        free(until_data_month);
    }
    if (until_data_day != NULL)
    {
        free(until_data_day);
    }
    if (until_data_hour != NULL)
    {
        free(until_data_hour);
    }
    return until;
}

STATIC VOID Free_Zone_Data(Zone_Details_t* zone_data)
{
    if (zone_data != NULL)
    {
        CHAR* fields[] = {
            zone_data->Field,
            zone_data->Name,
            zone_data->Standard_Offset,
            zone_data->Rules,
            zone_data->Format,
            zone_data->Until,
            zone_data->Comment
        };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
                fields[i] = NULL;
            }
        }
        free(zone_data);
        zone_data = NULL;
    }
}