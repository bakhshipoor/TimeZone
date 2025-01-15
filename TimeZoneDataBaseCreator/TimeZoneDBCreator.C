
#include "TimeZoneDBCreator.h"

const char Data_Folder_Name[] = "tzdata2024b";

const Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "Sun", "Sunday",      (uint8_t)TZDB_WEEKDAY_SUNDAY },
    { "Mon", "Monday",      (uint8_t)TZDB_WEEKDAY_MONDAY },
    { "Tue", "Tuesday",     (uint8_t)TZDB_WEEKDAY_TUESDAY },
    { "Wed", "Wednesday",   (uint8_t)TZDB_WEEKDAY_WEDNESDAY },
    { "Thu", "Thursday",    (uint8_t)TZDB_WEEKDAY_THURSDAY },
    { "Fri", "Friday",      (uint8_t)TZDB_WEEKDAY_FRIDAY },
    { "Sat", "Saturday",    (uint8_t)TZDB_WEEKDAY_SATURDAY },
};

const Last_Weekday_Lookup_t Last_Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "lastSun", (uint8_t)TZDB_WEEKDAY_SUNDAY },
    { "lastMon", (uint8_t)TZDB_WEEKDAY_MONDAY },
    { "lastTue", (uint8_t)TZDB_WEEKDAY_TUESDAY },
    { "lastWed", (uint8_t)TZDB_WEEKDAY_WEDNESDAY },
    { "lastThu", (uint8_t)TZDB_WEEKDAY_THURSDAY },
    { "lastFri", (uint8_t)TZDB_WEEKDAY_FRIDAY },
    { "lastSat", (uint8_t)TZDB_WEEKDAY_SATURDAY },
};

const Month_Lookup_t Month_Names[] = {
    { "Jan", "January",     (uint8_t)TZDB_MONTH_JANUARY },
    { "Feb", "February",    (uint8_t)TZDB_MONTH_FEBRUARY },
    { "Mar", "March",       (uint8_t)TZDB_MONTH_MARCH },
    { "Apr", "April",       (uint8_t)TZDB_MONTH_APRIL },
    { "May", "May",         (uint8_t)TZDB_MONTH_MAY },
    { "Jun", "June",        (uint8_t)TZDB_MONTH_JUNE },
    { "Jul", "July",        (uint8_t)TZDB_MONTH_JULY },
    { "Aug", "August",      (uint8_t)TZDB_MONTH_AUGUST },
    { "Sep", "September",   (uint8_t)TZDB_MONTH_SEPTEMBER },
    { "Oct", "October",     (uint8_t)TZDB_MONTH_OCTOBER },
    { "Nov", "November",    (uint8_t)TZDB_MONTH_NOVEMBER },
    { "Dec", "December",    (uint8_t)TZDB_MONTH_DECEMBER },
};

uint8_t version_FileName[MAX_LENGHT_FILE_NAME]; 
uint8_t iso3166tab_FileName[MAX_LENGHT_FILE_NAME]; 
uint8_t zonetab_FileName[MAX_LENGHT_FILE_NAME]; 
// Data Files
uint8_t Data_FileName[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];


void Initial_FileNames(void)
{
    sprintf_s(version_FileName,             MAX_LENGHT_FILE_NAME,   "../%s/version",          Data_Folder_Name);
    sprintf_s(iso3166tab_FileName,          MAX_LENGHT_FILE_NAME,   "../%s/iso3166.tab",      Data_Folder_Name);
    sprintf_s(zonetab_FileName,             MAX_LENGHT_FILE_NAME,   "../%s/zone.tab",         Data_Folder_Name);
    // Data Files
    sprintf_s(Data_FileName[0], MAX_LENGHT_FILE_NAME, "../%s/africa",           Data_Folder_Name);
    sprintf_s(Data_FileName[1], MAX_LENGHT_FILE_NAME, "../%s/antarctica",       Data_Folder_Name);
    sprintf_s(Data_FileName[2], MAX_LENGHT_FILE_NAME, "../%s/asia",             Data_Folder_Name);
    sprintf_s(Data_FileName[3], MAX_LENGHT_FILE_NAME, "../%s/australasia",      Data_Folder_Name);
    sprintf_s(Data_FileName[4], MAX_LENGHT_FILE_NAME, "../%s/backward",         Data_Folder_Name);
    sprintf_s(Data_FileName[5], MAX_LENGHT_FILE_NAME, "../%s/backzone",         Data_Folder_Name);
    sprintf_s(Data_FileName[6], MAX_LENGHT_FILE_NAME, "../%s/etcetera",         Data_Folder_Name);
    sprintf_s(Data_FileName[7], MAX_LENGHT_FILE_NAME, "../%s/europe",           Data_Folder_Name);
    sprintf_s(Data_FileName[8], MAX_LENGHT_FILE_NAME, "../%s/northamerica",     Data_Folder_Name);
    sprintf_s(Data_FileName[9], MAX_LENGHT_FILE_NAME, "../%s/southamerica",     Data_Folder_Name);

}

char* Parse_Version()
{
    FILE* version_File = fopen(version_FileName, "r");
    if (!version_File) {
        perror("Failed to open version");
        return NULL;
    }
    char version[256];
    memset(version, '\0', 256);
    fgets(version, 256, version_File); 
    sprintf(version, "%s", version);
    size_t version_lenght = strlen(version);
    if (version[version_lenght - 1] == '\n')
    {
        version[version_lenght - 1] = '\0';
    }
    fclose(version_File);
    return version;
}


ISO3166_Entry_t* Parse_ISO3166Tab(int* iso3166_Count)
{
    FILE* iso3166_File = fopen(iso3166tab_FileName, "r");
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
        uint8_t country_code[MAX_LENGHT_COUNTRY_CODE];
        uint8_t country_name[MAX_LENGHT_COUNTRY_NAME];
        
        memset(country_code, '\0', MAX_LENGHT_COUNTRY_CODE);
        memset(country_name, '\0', MAX_LENGHT_COUNTRY_NAME);

        sscanf_lenght = sscanf(line, "%s\t%[^\n]", country_code, country_name);

        if (sscanf_lenght < 2)
        {
            continue;
        }

        sprintf_s(country_code, MAX_LENGHT_COUNTRY_CODE, "%s", country_code);
        sprintf_s(country_name, MAX_LENGHT_COUNTRY_NAME, "%s", country_name);

        ISO3166_Entry_t entry;

        entry.Country_Code = (uint8_t*)malloc((strlen(country_code) + 1) * sizeof(uint8_t));
        entry.Country_Name = (uint8_t*)malloc((strlen(country_name) + 1) * sizeof(uint8_t));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(country_code)+1, "%s", country_code);
        }
        if (entry.Country_Name != NULL)
        {
            sprintf_s(entry.Country_Name, strlen(country_name)+1, "%s", country_name);
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

ZoneTab_Entry_t* Parse_ZoneTab(int* zoneCount) 
{
    FILE* zonetab_File = fopen(zonetab_FileName, "r");
    if (!zonetab_File) {
        perror("Failed to open file");
        return NULL;
    }

    ZoneTab_Entry_t* TimeZone_List = NULL;
    *zoneCount = 0;
    char line[2048];

    while (fgets(line, sizeof(line), zonetab_File)) 
    {
        if (strlen(line) < 5 || line[0] == '#') 
        {
            continue;
        }

        
        int sscanf_lenght;
        uint8_t code[MAX_LENGHT_COUNTRY_CODE];
        uint8_t coordinates[MAX_LENGHT_COORDINATES];
        uint8_t tzid[MAX_LENGHT_TIMEZONEID];
        uint8_t comment[MAX_LENGHT_COMMENT];
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

        memset(code, '\0', MAX_LENGHT_COUNTRY_CODE);
        memset(coordinates, '\0', MAX_LENGHT_COORDINATES);
        memset(tzid, '\0', MAX_LENGHT_TIMEZONEID);
        memset(comment, '\0', MAX_LENGHT_COMMENT);

        sscanf_lenght = sscanf(line, "%s\t%s\t%s\t%[^\n]", code, coordinates, tzid, comment);

        if (sscanf_lenght < 3)
        {
            continue;
        }
        sprintf_s(code, MAX_LENGHT_COUNTRY_CODE, "%s", code);
        sprintf_s(coordinates, MAX_LENGHT_COORDINATES, "%s", coordinates);
        sprintf_s(tzid, MAX_LENGHT_TIMEZONEID, "%s", tzid);
        sprintf_s(comment, MAX_LENGHT_COMMENT, "%s", comment);

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
            sprintf_s(entry.Country_Code, strlen(code)+1, "%s", code);
        }
        if (entry.TZ_Identifier != NULL)
        {
            sprintf_s(entry.TZ_Identifier, strlen(tzid)+1, "%s", tzid);
        }
        if (entry.Comments != NULL)
        {
            sprintf_s(entry.Comments, strlen(comment)+1, "%s", comment);
        }
        
        entry.Latitude = latitude;
        entry.Longitude = longitude;

        ZoneTab_Entry_t* tz_list = realloc(TimeZone_List, (*zoneCount + 1) * sizeof(ZoneTab_Entry_t));
        if (tz_list == NULL)
        {
            return NULL;
        }
        TimeZone_List = tz_list;
        TimeZone_List[*zoneCount] = entry;
        (*zoneCount)++;
    }

    fclose(zonetab_File);
    return TimeZone_List;
}

Rule_Entry_t* Parse_Rules(int* rules_Count)
{
    Rule_Entry_t* Rules_List = NULL;
    *rules_Count = 0;
    char line[2048];
    uint16_t dataFile_index = 0;
    const uint8_t rule[] = "Rule";
    bool rule_exist = false;
    uint32_t rule_exist_index = 0;

    FILE* temp_File = fopen("../temp.txt", "w");

    for (dataFile_index = 0; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        FILE* data_File = fopen(Data_FileName[dataFile_index], "r");
        if (!data_File) {
            perror("Failed to open file");
            continue;
        }

        while (fgets(line, sizeof(line), data_File))
        {
            if (strlen(line) < 5 || line[0] == '#' || line[0] != 'R')
            {
                continue;
            }
            int sscanf_lenght;

            uint8_t field[MAX_LENGHT_DATA_FIELD];
            uint8_t name[MAX_LENGHT_DATA_FIELD];
            uint8_t from[MAX_LENGHT_DATA_FIELD];
            uint8_t to[MAX_LENGHT_DATA_FIELD];
            uint8_t reserved[MAX_LENGHT_DATA_FIELD];
            uint8_t in[MAX_LENGHT_DATA_FIELD];
            uint8_t on[MAX_LENGHT_DATA_FIELD];
            uint8_t at[MAX_LENGHT_DATA_FIELD];
            uint8_t save[MAX_LENGHT_DATA_FIELD];
            uint8_t letter[MAX_LENGHT_DATA_FIELD];
            uint8_t comment[MAX_LENGHT_DATA_FIELD];

            memset(field, '\0', MAX_LENGHT_DATA_FIELD);
            memset(name, '\0', MAX_LENGHT_DATA_FIELD);
            memset(from, '\0', MAX_LENGHT_DATA_FIELD);
            memset(to, '\0', MAX_LENGHT_DATA_FIELD);
            memset(reserved, '\0', MAX_LENGHT_DATA_FIELD);
            memset(in, '\0', MAX_LENGHT_DATA_FIELD);
            memset(on, '\0', MAX_LENGHT_DATA_FIELD);
            memset(at, '\0', MAX_LENGHT_DATA_FIELD);
            memset(save, '\0', MAX_LENGHT_DATA_FIELD);
            memset(letter, '\0', MAX_LENGHT_DATA_FIELD);
            memset(comment, '\0', MAX_LENGHT_DATA_FIELD);

            sscanf_lenght = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%[^\r\n]", field, name, from, to, reserved, in, on, at, save, letter, comment);

            sprintf(field, "%s", field);
            sprintf(name, "%s", name);
            sprintf(from, "%s", from);
            sprintf(to, "%s", to);
            sprintf(reserved, "%s", reserved);
            sprintf(in, "%s", in);
            sprintf(on, "%s", on);
            sprintf(at, "%s", at);
            sprintf(save, "%s", save);
            sprintf(letter, "%s", letter);
            sprintf(comment, "%s", comment);

            fprintf(temp_File, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", field, name, from, to, reserved, in, on, at, save, letter, comment);

            if ( strcmp(rule, field) != 0)
            {
                continue;
            }

            if (sscanf_lenght < 10)
            {
                continue;
            }

            rule_exist = false;
            if (Rules_List != NULL && ((*rules_Count) > 0))
            {
                for (int rule_index = 0; rule_index < (*rules_Count); rule_index++)
                {
                    if (strcmp(Rules_List[rule_index].Name, name) == 0)
                    {
                        rule_exist = true;
                        rule_exist_index = rule_index;
                        break;
                    }
                }
            }

            if (rule_exist)
            {
                Rule_Data_t* data = realloc(Rules_List[rule_exist_index].Data, (Rules_List[rule_exist_index].Data_Count + 1) * sizeof(Rule_Data_t));
                if (data != NULL)
                {
                    Rules_List[rule_exist_index].Data = data;
                    Rules_List[rule_exist_index].Data[Rules_List[rule_exist_index].Data_Count] = Parse_Rule_Data(from, to, reserved, in, on, at, save, letter, comment);
                    Rules_List[rule_exist_index].Data_Count++;
                }
            }
            else
            {
                Rule_Entry_t entry = { 0 };
                entry.Name = (uint8_t*)malloc((strlen(name) + 1) * sizeof(uint8_t));
                if (entry.Name != NULL)
                {
                    sprintf(entry.Name, "%s", name);
                }

                entry.Data_Count = 0;
                Rule_Data_t* data = realloc(entry.Data, (entry.Data_Count + 1) * sizeof(Rule_Data_t));
                if (data != NULL)
                {
                    entry.Data = data;
                    entry.Data[entry.Data_Count] = Parse_Rule_Data(from, to, reserved, in, on, at, save, letter, comment);
                    entry.Data_Count++;
                }

                Rule_Entry_t* rule_list = realloc(Rules_List, (*rules_Count + 1) * sizeof(Rule_Entry_t));
                if (rule_list == NULL)
                {
                    return NULL;
                }
                Rules_List = rule_list;
                Rules_List[*rules_Count] = entry;
                (*rules_Count)++;
            }

            
        }

        fclose(data_File);
    }
    fclose(temp_File);
    return Rules_List;
}

int main() 
{
    Initial_FileNames();



    char* version = Parse_Version();

    int iso3166_Count=0;
    ISO3166_Entry_t* iso3166_list = Parse_ISO3166Tab(&iso3166_Count);

    int zoneCount = 0;
    ZoneTab_Entry_t* tab = Parse_ZoneTab(&zoneCount);

    int rules_Count = 0;
    Rule_Entry_t* rules_list = Parse_Rules(&rules_Count);
   
    
    
    
    while (1);
    return 0;
}

Rule_Data_t Parse_Rule_Data(const char* from, const char* to, const char* reserved, const char* in, const char* on, const char* at, const char* save, const char* letter, const char* comment)
{
    Rule_Data_t rule_data = { 0 };
    int sscanf_lenght;

    rule_data.From = atoi(from);
    if (strcmp(to, "only") == 0)
    {
        rule_data.To = rule_data.From;
    }
    else if (strcmp(to, "max") == 0)
    {
        rule_data.To = RULE_MAX_VALUE;
    }
    else
    {
        rule_data.To = atoi(to);
    }

    rule_data.Reserved = (uint8_t*)malloc((strlen(reserved) + 1) * sizeof(uint8_t));
    if (rule_data.Reserved != NULL)
    {
        if (reserved[0] == '-')
        {
            sprintf(rule_data.Reserved, "%c", '\0');
        }
        else
        {
            sprintf(rule_data.Reserved, "%s", reserved);
        }
    }

    for (int month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(in, Month_Names[month_index].Abbr) == 0 || strcmp(in, Month_Names[month_index].Full) == 0)
        {
            rule_data.Month = Month_Names[month_index].Number;
        }
    }

    if (strlen(on) <= 3)
    {
        rule_data.Day = atoi(on);
        rule_data.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
        rule_data.Wday_After_Or_Before_Day = false;

    }
    else
    {
        uint8_t dow[10], d[10];
        sscanf_lenght = sscanf(on, "%[^=]=%s", dow, d);
        sprintf(dow, "%s", dow);
        sprintf(d, "%s", d);
        if (sscanf_lenght == 1)
        {
            for (int last_week_index = 0; last_week_index < TZDB_WEEKDAY_TOTAL; last_week_index++)
            {
                if (strcmp(dow, Last_Weekday_Names[last_week_index].Full) == 0)
                {
                    rule_data.Weekday = Last_Weekday_Names[last_week_index].Number;
                    rule_data.Day = 0; // Last Week
                }
            }
        }
        else
        {
            int len = strlen(dow);
            if (dow[len - 1] == '>')
            {
                rule_data.Wday_After_Or_Before_Day = true;
            }
            else if (dow[len - 1] == '<')
            {
                rule_data.Wday_After_Or_Before_Day = false;
            }
            dow[len - 1] = '\0';
            for (int week_index = 0; week_index < TZDB_WEEKDAY_TOTAL; week_index++)
            {
                if (strcmp(dow, Weekday_Names[week_index].Abbr) == 0 || strcmp(dow, Weekday_Names[week_index].Full) == 0)
                {
                    rule_data.Weekday = Weekday_Names[week_index].Number;
                    rule_data.Day = atoi(d);
                }
            }
        }
    }

    uint8_t base=0;
    uint32_t hour=0, minute=0;
    if (sscanf(at, "%d:%d%c", &hour, &minute, &base) == 3)
    {
        rule_data.Hour = (hour * 3600) + (minute * 60);
        if (base == 'u')
        {
            rule_data.UTC_Or_Local_Hour = true;
        }
        else if (base == 's')
        {
            rule_data.UTC_Or_Local_Hour = false;
        }
    }
    else if (sscanf(at, "%d:%d", &hour, &minute) == 2)
    {
        rule_data.Hour = (hour * 3600) + (minute * 60);
        rule_data.UTC_Or_Local_Hour = false;
    }

    uint8_t sign = 0;
    if (sscanf(save, "%c%d:%d", &sign, &hour, &minute) == 3)
    {
        rule_data.Save_Hour = (hour * 3600) + (minute * 60);
        if (sign == '-')
        {
            rule_data.Save_Hour *= -1;
        }
    }
    else if (sscanf(save, "%d:%d", &hour, &minute) == 2)
    {
        rule_data.Save_Hour = (hour * 3600) + (minute * 60);
    }
    else if (sscanf(save, "%d", &hour) == 1)
    {
        rule_data.Save_Hour = (hour * 3600);
    }
    
    rule_data.Letter = (uint8_t*)malloc((strlen(letter) + 1) * sizeof(uint8_t));
    if (rule_data.Letter != NULL)
    {
        if (letter[0] == '-')
        {
            sprintf(rule_data.Letter, "%c", '\0');
        }
        else
        {
            sprintf(rule_data.Letter, "%s", letter);
        }
        
    }

    rule_data.Comment = (uint8_t*)malloc((strlen(comment) + 1) * sizeof(uint8_t));
    if (rule_data.Comment != NULL)
    {
        sprintf(rule_data.Comment, "%s", comment);
    }
    
    return rule_data;
}