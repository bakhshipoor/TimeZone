#include "../Inc/Parse_Zones.h"

CHAR* Last_Zone_Name = NULL;

Zone_Entry_t* Parse_Zones(COUNTER* zones_Count)
{
    
    Zone_Entry_t* Zones_List = NULL;
    *zones_Count = 0;

    Last_Zone_Name = (CHAR*)malloc((MAX_LENGTH_DATA_FIELD) * sizeof(CHAR));
    if (Last_Zone_Name == NULL)
    {
        return NULL;
    }
    
    COUNTER dataFile_index = 0;
    FILE* data_File;
    COUNTER find_index;
    Zone_Data_t* zone_data=NULL;
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue; 
        }

        while (fgets(line, MAX_LENGTH_LINE, data_File))
        {
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            Parse_Free_Zone_Data(&zone_data);
            zone_data = Parse_Zone_Data(&line);

            if (zone_data == NULL)
            {
                continue;
            }

            find_index = -1;

            if (!Zone_isExist(&Zones_List, zones_Count, &zone_data->Name, &find_index))
            {
                if (Zone_Create(&Zones_List, zones_Count, &zone_data->Name, &Data_Files_Name[dataFile_index]))
                {
                    find_index = (*zones_Count);
                    (*zones_Count)++;
                }
            }
            if (find_index != -1)
            {
                Parse_Zone_Year_Range(&Zones_List, &find_index, &zone_data);
            }
            
            
        }

        fclose(data_File);
    }
    Parse_Free_Zone_Data(&zone_data);
    Parse_Zone_Info(&Zones_List, zones_Count);
    
    free(Last_Zone_Name);
    Last_Zone_Name = NULL;

    return Zones_List;
}

BOOL Zone_isExist(CONST Zone_Entry_t** zone_list, CONST COUNTER* zones_count, CONST CHAR** zone_name, COUNTER* find_Index)
{
    if (*zone_list != NULL && ((*zones_count) > 0))
    {
        for (int zone_index = 0; zone_index < (*zones_count); zone_index++)
        {
            if (strcmp((*zone_list)[zone_index].Name, *zone_name) == 0)
            {
                *find_Index = zone_index;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL Zone_Create(Zone_Entry_t** zone_list, CONST COUNTER* zones_Count, CONST CHAR** zone_name, CONST CHAR** file_name)
{
    Zone_Entry_t* zone = (Zone_Entry_t*)malloc(sizeof(Zone_Entry_t));
    if (zone == NULL)
    {
        return FALSE;
    }

    if (zones_Count == NULL )
    {
        return FALSE;
    }

    zone->Name = (CHAR*)malloc((strlen(*zone_name) + 1) * sizeof(CHAR));
    if (zone->Name != NULL)
    {
        sprintf(zone->Name, "%s", *zone_name);
    }

    zone->Info_Count = 0;
    zone->Year_Begin = 0;
    zone->Year_End = 0;

    zone->File = (CHAR*)malloc((strlen(*file_name) + 1) * sizeof(CHAR));
    if (zone->File != NULL)
    {
        sprintf(zone->File, "%s", *file_name);
    }
    Zone_Entry_t* zones;
    if (zones_Count == 0)
    {
        zones = malloc(sizeof(Zone_Entry_t));
    }
    else
    {
        zones = realloc(*zone_list, (*zones_Count + 1) * sizeof(Zone_Entry_t));
    }
    if (zones == NULL)
    {
        return FALSE;
    }
    *zone_list = zones;
    (*zone_list)[*zones_Count] = *zone;
    return TRUE;
}

VOID Parse_Zone_Year_Range(Zone_Entry_t** zone_list, CONST COUNTER* zone_index, CONST Zone_Data_t** zone_data)
{
    Zone_Info_Until_t* until = Parse_Zone_Info_Until(zone_data);
    if (until == NULL)
    {
        return;
    }
    YEAR year = until->Year;
    
    if ((*zone_list)[*zone_index].Year_Begin == 0)
    {
        (*zone_list)[*zone_index].Year_Begin = year;
    }
    else if (year != -1 && (*zone_list)[*zone_index].Year_Begin > year)
    {
        (*zone_list)[*zone_index].Year_Begin = year;
    }

    if ((*zone_list)[*zone_index].Year_End == 0)
    {
        (*zone_list)[*zone_index].Year_End = year;
    }
    else if (year == -1)
    {
        (*zone_list)[*zone_index].Year_End = year;
    }
    else if (year != -1 && (*zone_list)[*zone_index].Year_End < year)
    {
        (*zone_list)[*zone_index].Year_End = year;
    }
    free(until);
    until = NULL;
}

VOID Parse_Zone_Info(Zone_Entry_t** zone_list, CONST COUNTER* zones_Count)
{
    COUNTER dataFile_index = 0;
    FILE* data_File;
    COUNTER find_index;
    Zone_Data_t* zone_data=NULL;
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue;
        }
        int size = sizeof(line);
        while (fgets(line, MAX_LENGTH_LINE, data_File))
        {
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }
            Parse_Free_Zone_Data(&zone_data);
            zone_data = Parse_Zone_Data(&line);
            if (zone_data == NULL)
            {
                continue;
            }

            find_index = -1;
            if (Zone_isExist(zone_list, zones_Count, &zone_data->Name, &find_index))
            {
                Zone_Info_t* info = (Zone_Info_t*)malloc(sizeof(Zone_Info_t));
                if (info == NULL)
                {
                    continue;
                }
                HOUR* hour = Parse_Zone_Data_Standard_Offset(&zone_data);
                if (hour != NULL)
                {
                    info->Standard_Offset = *hour;
                }
                Zone_Info_Rule_t* rule= Parse_Zone_Data_Rules(&zone_data);
                if (rule != NULL)
                {
                    info->Rule = *rule;
                }
                info->Format = (CHAR*)malloc((strlen(zone_data->Format) + 1) * sizeof(CHAR));
                if (info->Format != NULL)
                {
                    sprintf(info->Format, "%s", zone_data->Format);
                }
                Zone_Info_Until_t* until = Parse_Zone_Info_Until(&zone_data);
                if (until != NULL)
                {
                    info->Until = *until;
                }
                info->Comment = (CHAR*)malloc((strlen(zone_data->Comment) + 1) * sizeof(CHAR));
                if (info->Comment != NULL)
                {
                    sprintf(info->Comment, "%s", zone_data->Comment);
                }
                Zone_Info_t* z_info;
                if ((*zone_list)[find_index].Info_Count==0)
                {
                    z_info = (Zone_Info_t*)malloc(sizeof(Zone_Info_t));
                }
                else
                {
                    z_info = (Zone_Info_t*)realloc((*zone_list)[find_index].Info, ((*zone_list)[find_index].Info_Count + 1) * sizeof(Zone_Info_t));
                }
                if (z_info != NULL)
                {
                    (*zone_list)[find_index].Info = z_info;
                    (*zone_list)[find_index].Info[(*zone_list)[find_index].Info_Count] = *info;
                    (*zone_list)[find_index].Info_Count++;
                }
            }
        }
        fclose(data_File);
    }

    Parse_Free_Zone_Data(&zone_data);
}




Zone_Data_t* Parse_Zone_Data(CONST CHAR** line)
{
    Zone_Data_t* zone_data = (Zone_Data_t*)malloc(sizeof(Zone_Data_t));
    if (zone_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    zone_data->Field = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Name = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Standard_Offset = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Rules = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Format = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Until = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    zone_data->Comment = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));

    if (zone_data->Field == NULL ||
        zone_data->Name == NULL ||
        zone_data->Standard_Offset == NULL ||
        zone_data->Rules == NULL ||
        zone_data->Format == NULL ||
        zone_data->Until == NULL ||
        zone_data->Comment == NULL)
    {
        Parse_Free_Zone_Data(&zone_data);
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
        Parse_Free_Zone_Data(&zone_data);
    }

    return zone_data;
}


HOUR* Parse_Zone_Data_Standard_Offset(CONST Zone_Data_t** zone_data)
{
    CHAR s;
    HOUR* hour = Parse_Hour(&(*zone_data)->Standard_Offset, &s);
    return hour;
}

Zone_Info_Rule_t* Parse_Zone_Data_Rules(CONST Zone_Data_t** zone_data)
{
    Zone_Info_Rule_t* rule = (Zone_Info_Rule_t*)malloc(sizeof(Zone_Info_Rule_t));
    if (rule == NULL)
    {
        return NULL;
    }
    if (strcmp((*zone_data)->Rules, "-") == 0)
    {
        rule->Has_Rule = FALSE;
        rule->Rule_Name = (CHAR*)malloc(sizeof(CHAR));
        if (rule->Rule_Name != NULL)
        {
            rule->Rule_Name[0] = '\0';
        }
        rule->Save_Hour = 0;
    }
    else if (!isalpha((*zone_data)->Rules[0]))
    {
        rule->Has_Rule = TRUE;
        rule->Rule_Name = (CHAR*)malloc(sizeof(CHAR));
        if (rule->Rule_Name != NULL)
        {
            rule->Rule_Name[0] = '\0';
        }
        CHAR* s = { 0 };
        HOUR* h = Parse_Hour(&(*zone_data)->Rules, &s);
        if (h != NULL)
        {
            rule->Save_Hour = *h;
        }
        free(s);
        s = NULL;
    }
    else
    {
        rule->Has_Rule = TRUE;
        rule->Rule_Name = (CHAR*)malloc((strlen((*zone_data)->Rules) + 1) * sizeof(CHAR));
        if (rule->Rule_Name != NULL)
        {
            sprintf(rule->Rule_Name, "%s", (*zone_data)->Rules);
        }
        rule->Save_Hour = 0;
    }
    return rule;
}

Zone_Info_Until_t* Parse_Zone_Info_Until(CONST Zone_Data_t** zone_data)
{
    Zone_Info_Until_t* until = (Zone_Info_Until_t*)calloc(1,sizeof(Zone_Info_Until_t));
    if (until == NULL)
    {
        return NULL;
    }
    CHAR** until_data = (CHAR**)malloc(4 * sizeof(CHAR*));
    if (until_data == NULL)
    {
        free(until);
        until = NULL;
        return NULL;
    }
    for (COUNTER ud_index = 0; ud_index < 4; ud_index++)
    {
        until_data[ud_index] = (CHAR*)malloc(MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
        if (until_data[ud_index] == NULL)
        {
            free(until);
            until = NULL;
            free(until_data);
            until_data = NULL;
            return until;
        }
    }
    if (strlen((*zone_data)->Until) == 0)
    {
        until->Year = -1;
        until->Month = 0;
        until->Day.Day = 0;
        until->Day.Weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
        until->Day.Weekday_isAfterOrEqual_Day = FALSE;
        until->Hour.Hour = 0;
        until->Hour.Hour_isUTC = FALSE;
    }
    else
    {
        if (sscanf((*zone_data)->Until, "%s\t%s\t%s\t%s", until_data[0], until_data[1], until_data[2], until_data[3]) == 4)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            sprintf(until_data[2], "%s", until_data[2]);
            sprintf(until_data[3], "%s", until_data[3]);
            until->Year = atoi(until_data[0]);
            MONTH* m = Parse_Month(&until_data[1]);
            if (m != NULL)
            {
                until->Month = *m;
            }
            Parse_Day_Of_Month(&until_data[2], &until[0].Day.Day, &until[0].Day.Weekday, &until[0].Day.Weekday_isAfterOrEqual_Day);
            CHAR s;
            HOUR* h = Parse_Hour(&until_data[3], &s);
            if (h != NULL)
            {
                until->Hour.Hour = *h;
                until->Hour.Hour_isUTC = FALSE;
                if (s == 'u')
                {
                    until->Hour.Hour_isUTC = TRUE;
                }
            }
        }
        else if (sscanf((*zone_data)->Until, "%s\t%s\t%s", until_data[0], until_data[1], until_data[2]) == 3)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            sprintf(until_data[2], "%s", until_data[2]);
            until->Year = atoi(until_data[0]);
            MONTH* m = Parse_Month(&until_data[1]);
            if (m != NULL)
            {
                until->Month = *m;
            }
            Parse_Day_Of_Month(&until_data[2], &until->Day.Day, &until->Day.Weekday, &until->Day.Weekday_isAfterOrEqual_Day);
        }
        else if (sscanf((*zone_data)->Until, "%s\t%s", until_data[0], until_data[1]) == 2)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            until->Year = atoi(until_data[0]);
            MONTH* m = Parse_Month(&until_data[1]);
            if (m != NULL)
            {
                until->Month = *m;
            }
        }
        else if (sscanf((*zone_data)->Until, "%s", until_data[0]) == 1)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            until->Year = atoi(until_data[0]);
        }
    }

    for (COUNTER ud_index = 0; ud_index < 4; ud_index++)
    {
        free(until_data[ud_index]);
        until_data[ud_index] = NULL;
    }
    free(until_data);
    until_data = NULL;

    return until;
}



VOID Parse_Free_Zone_Data(Zone_Data_t** zone_data)
{
    if (*zone_data != NULL)
    {
        if ((*zone_data)->Field != NULL)
        {
            free((*zone_data)->Field);
            (*zone_data)->Field = NULL;
        }
        if ((*zone_data)->Name != NULL)
        {
            free((*zone_data)->Name);
            (*zone_data)->Name = NULL;
        }
        if ((*zone_data)->Standard_Offset != NULL)
        {
            free((*zone_data)->Standard_Offset);
            (*zone_data)->Standard_Offset = NULL;
        }
        if ((*zone_data)->Rules != NULL)
        {
            free((*zone_data)->Rules);
            (*zone_data)->Rules = NULL;
        }
        if ((*zone_data)->Format != NULL)
        {
            free((*zone_data)->Format);
            (*zone_data)->Format = NULL;
        }
        if ((*zone_data)->Until != NULL)
        {
            free((*zone_data)->Until);
            (*zone_data)->Until = NULL;
        }
        if ((*zone_data)->Comment != NULL)
        {
            free((*zone_data)->Comment);
            (*zone_data)->Comment = NULL;
        }
        free((*zone_data));
        (*zone_data) = NULL;
    }
}