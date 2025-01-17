#include "../Inc/Parse_Zones.h"

char Last_Zone_Name[MAX_LENGHT_DATA_FIELD];

Zone_Entry_t* Parse_Zones(int32_t* zones_Count)
{
    Zone_Entry_t* Zones_List = NULL;
    *zones_Count = 0;
    char line[2048]; 
    uint16_t dataFile_index = 0; 
    FILE* temp = fopen("../temp.txt", "w");
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

            if (strlen(zone_data.Name) > 0)
            {
                /*continue;*/
                int x = 0;
                x++;
                
    
                fprintf(temp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", zone_data.Field, zone_data.Name, zone_data.Standard_Offset, zone_data.Rules, zone_data.Format, zone_data.Until, zone_data.Comment);
    
    
            }

            
        }

        fclose(data_File);
    }

    fclose(temp);
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
    if (strcmp(zone_data.Rules, '-') == 0)
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