#include "../Inc/Parse_Zones.h"

char Last_Zone_Name[MAX_LENGHT_DATA_FIELD];

Zone_Entry_t* Parse_Zones(int32_t* zones_Count)
{
    Zone_Entry_t* Zones_List = NULL;
    *zones_Count = 0;
    char line[2048]; 
    uint16_t dataFile_index = 0; 
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        FILE* data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue; 
        }

        while (fgets(line, sizeof(line), data_File))
        {
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            Zone_Data_t zone_data = Parse_Zone_Data(line);

            if (strlen(zone_data.Name) <= 0)
            {
                continue;
            }

            int32_t find_index = -1; 

            if (!Zone_isExist(Zones_List, zones_Count, zone_data.Name, &find_index))
            {
                if (Zone_Create(&Zones_List, *zones_Count, zone_data.Name))
                {
                    find_index = (*zones_Count);
                    (*zones_Count)++;
                }
            }
            if (find_index != -1)
            {
                Parse_Zone_Year_Range(Zones_List, find_index, zone_data);
            }
            
            
        }

        fclose(data_File);
    }

    Parse_Zone_Info(Zones_List, zones_Count);

    return Zones_List;
}

Zone_Data_t Parse_Zone_Data(const char* line)
{
    Zone_Data_t zone_data; 
    int scan_lenght = 0;

    memset(zone_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Standard_Offset, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Rules, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Format, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Until, '\0', MAX_LENGHT_DATA_FIELD);
    memset(zone_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);

    if (strncmp(line,"\t\t\t",3)==0 && line[3] != '#')
    {
        sprintf(zone_data.Field, "%s", "Zone");
        sprintf(zone_data.Name, "%s", Last_Zone_Name);
        scan_lenght = sscanf(line, "%s\t%s\t%s\t%[^#\n]#%[^\n]",
            zone_data.Standard_Offset,
            zone_data.Rules,
            zone_data.Format,
            zone_data.Until,
            zone_data.Comment);
    }
    else if (line[0]=='Z')
    {
        scan_lenght = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%[^#\n]#%[^\n]",
            zone_data.Field,
            zone_data.Name,
            zone_data.Standard_Offset,
            zone_data.Rules,
            zone_data.Format,
            zone_data.Until,
            zone_data.Comment);

        sprintf(Last_Zone_Name, "%s", zone_data.Name);
    }

    sprintf(zone_data.Field, "%s", zone_data.Field);
    sprintf(zone_data.Name, "%s", zone_data.Name);
    sprintf(zone_data.Standard_Offset, "%s", zone_data.Standard_Offset);
    sprintf(zone_data.Rules, "%s", zone_data.Rules);
    sprintf(zone_data.Format, "%s", zone_data.Format);
    sprintf(zone_data.Until, "%s", zone_data.Until);
    sprintf(zone_data.Comment, "%s", zone_data.Comment);

    if (strcmp("Zone", zone_data.Field) != 0 /*|| scan_lenght < 3 || scan_lenght>7*/)
    {
        memset(zone_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Standard_Offset, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Rules, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Format, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Until, '\0', MAX_LENGHT_DATA_FIELD);
        memset(zone_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);
    }
    
    return zone_data;
}

int32_t Parse_Zone_Data_Standard_Offset(const Zone_Data_t zone_data)
{
    char* s = { 0 };
    return Parse_Hour(zone_data.Standard_Offset, &s);
}

Zone_Info_Rule_t Parse_Zone_Data_Rules(const Zone_Data_t zone_data)
{
    Zone_Info_Rule_t rule = { 0 };
    if (strcmp(zone_data.Rules, "-") == 0)
    {
        rule.Has_Rule = false;
        rule.Rule_Name = (uint8_t*)malloc(sizeof(uint8_t));
        if (rule.Rule_Name != NULL)
        {
            rule.Rule_Name[0] = '\0';
        }
        rule.Save_Hour = 0;
    }
    else if (!isalpha(zone_data.Rules[0]))
    {
        rule.Has_Rule = true;
        rule.Rule_Name = (uint8_t*)malloc(sizeof(uint8_t));
        if (rule.Rule_Name != NULL)
        {
            rule.Rule_Name[0] = '\0';
        }
        char* s = { 0 };
        rule.Save_Hour = Parse_Hour(zone_data.Rules, &s);
    }
    else
    {
        rule.Has_Rule = true;
        rule.Rule_Name = (uint8_t*)malloc((strlen(zone_data.Rules) + 1) * sizeof(uint8_t));
        if (rule.Rule_Name != NULL)
        {
            sprintf(rule.Rule_Name, "%s", zone_data.Rules);
        }
        rule.Save_Hour = 0;
    }
    return rule;
}

Zone_Info_Until_t Parse_Zone_Info_Until(const Zone_Data_t zone_data)
{
    Zone_Info_Until_t until = { 0 };
    char* until_data[4];
    for (int ud_index = 0; ud_index < 4; ud_index++)
    {
        until_data[ud_index] = (char*)malloc(MAX_LENGHT_DATA_FIELD * sizeof(char));
        if (until_data[ud_index] == NULL)
        {
            return until;
        }
    }
    if (strlen(zone_data.Until) == 0)
    {
        until.Year = -1;
        until.Month = 0;
        until.Day.Day = 0;
        until.Day.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
        until.Day.Weekday_isAfterOrEqual_Day = false;
        until.Hour.Hour = 0;
        until.Hour.Hour_isUTC = false;
    }
    else
    {
        if (sscanf(zone_data.Until, "%s\t%s\t%s\t%s", until_data[0], until_data[1], until_data[2], until_data[3]) == 4)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            sprintf(until_data[2], "%s", until_data[2]);
            sprintf(until_data[3], "%s", until_data[3]);
            until.Year = atoi(until_data[0]);
            until.Month = Parse_Month(until_data[1]); 
            Parse_Day_Of_Month(until_data[2], &until.Day.Day, &until.Day.Weekday, &until.Day.Weekday_isAfterOrEqual_Day);
            char* s = { 0 };
            until.Hour.Hour = Parse_Hour(until_data[3], &s);
            until.Hour.Hour_isUTC = false;
            if (s[0] == 'u')
            {
                until.Hour.Hour_isUTC = true;
            }
        }
        else if (sscanf(zone_data.Until, "%s\t%s\t%s", until_data[0], until_data[1], until_data[2]) == 3)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            sprintf(until_data[2], "%s", until_data[2]);
            until.Year = atoi(until_data[0]);
            until.Month = Parse_Month(until_data[1]);
            Parse_Day_Of_Month(until_data[2], &until.Day.Day, &until.Day.Weekday, &until.Day.Weekday_isAfterOrEqual_Day);            
        }
        else if (sscanf(zone_data.Until, "%s\t%s", until_data[0], until_data[1]) == 2)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            sprintf(until_data[1], "%s", until_data[1]);
            until.Year = atoi(until_data[0]);
            until.Month = Parse_Month(until_data[1]);
        }
        else if (sscanf(zone_data.Until, "%s", until_data[0]) == 1)
        {
            sprintf(until_data[0], "%s", until_data[0]);
            until.Year = atoi(until_data[0]);
        }
    }
    return until;
}

bool Zone_isExist(const Zone_Entry_t* zone_list, const int32_t* zones_count, const char* zone_name, int32_t* find_Index)
{
    if (zone_list != NULL && ((*zones_count) > 0))
    {
        for (int zone_index = 0; zone_index < (*zones_count); zone_index++)
        {
            if (strcmp(zone_list[zone_index].Name, zone_name) == 0)
            {
                *find_Index = zone_index;
                return true;
            }
        }
    }
    return false;
}

bool Zone_Create(Zone_Entry_t** zone_list, const int32_t zones_Count, const char* zone_name)
{
    Zone_Entry_t zone = { 0 };

    zone.Name = (uint8_t*)malloc((strlen(zone_name) + 1) * sizeof(uint8_t));
    if (zone.Name != NULL)
    {
        sprintf(zone.Name, "%s", zone_name);
    }

    zone.Info_Count = 0;
    zone.Year_Begin = 0;
    zone.Year_End = 0;

    Zone_Entry_t* zones = realloc(*zone_list, (zones_Count + 1) * sizeof(Zone_Entry_t));
    if (zones == NULL)
    {
        return false;
    }

    *zone_list = zones;

    (*zone_list)[zones_Count] = zone;

    return true;
}

void Parse_Zone_Year_Range(Zone_Entry_t* zone_list, const int32_t zone_index, const Zone_Data_t zone_data)
{
    Zone_Info_Until_t until = Parse_Zone_Info_Until(zone_data);
    int16_t year = until.Year;
    
    if (zone_list[zone_index].Year_Begin == 0)
    {
        zone_list[zone_index].Year_Begin = year;
    }
    else if (year != -1 && zone_list[zone_index].Year_Begin > year)
    {
        zone_list[zone_index].Year_Begin = year;
    }

    if (zone_list[zone_index].Year_End == 0)
    {
        zone_list[zone_index].Year_End = year;
    }
    else if (year == -1)
    {
        zone_list[zone_index].Year_End = year;
    }
    else if (year != -1 && zone_list[zone_index].Year_End < year)
    {
        zone_list[zone_index].Year_End = year;
    }
}

void Parse_Zone_Info(Zone_Entry_t* zone_list, const int32_t* zones_Count)
{
    uint8_t line[2048]; 
    uint16_t dataFile_index = 0;

    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        FILE* data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            continue;
        }

        while (fgets(line, sizeof(line), data_File))
        {
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            Zone_Data_t zone_data = Parse_Zone_Data(line);
            if (strlen(zone_data.Name) <= 0)
            {
                continue;
            }

            int32_t find_index = -1;
            if (Zone_isExist(zone_list, zones_Count, zone_data.Name, &find_index))
            {
                Zone_Info_t info = { 0 };
                info.Standard_Offset = Parse_Zone_Data_Standard_Offset(zone_data);
                info.Rule = Parse_Zone_Data_Rules(zone_data);
                info.Format = (uint8_t*)malloc((strlen(zone_data.Format) + 1) * sizeof(uint8_t));
                if (info.Format != NULL)
                {
                    sprintf(info.Format, "%s", zone_data.Format);
                }
                info.Until = Parse_Zone_Info_Until(zone_data);
                info.Comment = (uint8_t*)malloc((strlen(zone_data.Comment) + 1) * sizeof(uint8_t));
                if (info.Comment != NULL)
                {
                    sprintf(info.Comment, "%s", zone_data.Comment);
                }
                Zone_Info_t* z_info = (Zone_Info_t*)realloc(zone_list[find_index].Info, (zone_list[find_index].Info_Count + 1) * sizeof(Zone_Info_t));
                if (z_info != NULL)
                {
                    zone_list[find_index].Info = z_info;
                    zone_list[find_index].Info[zone_list[find_index].Info_Count] = info;
                    zone_list[find_index].Info_Count++;
                }
            }
        }
        fclose(data_File);
    }
}