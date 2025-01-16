#include "../Inc/Parse_Rules.h"

Rule_Entry_t* Parse_Rules(int32_t* rules_Count)
{
    Rule_Entry_t* Rules_List = NULL;
    *rules_Count = 0;
    char line[2048];
    uint16_t dataFile_index = 0;
   

    for (dataFile_index = 0; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        FILE* data_File = fopen(Data_FileName[dataFile_index], "r");
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
            
            Rule_Data_t rule_data= Parse_Rule_Data(line);
            if (strlen(rule_data.Name)<=0)
            {
                continue;
            }

            int32_t find_index = -1;
            if (!Rule_isExist(Rules_List, rules_Count, rule_data.Name, &find_index))
            {
                if (Rule_Create(&Rules_List, *rules_Count, rule_data.Name))
                {
                    (*rules_Count)++;
                }
                
            }

            Parse_Rule_Year_Range(Rules_List, find_index, rule_data);
        }

        fclose(data_File);
    }
    Parse_Rule_Years(Rules_List, rules_Count);
    return Rules_List;
}

Rule_Data_t Parse_Rule_Data(const char* line)
{
    Rule_Data_t rule_data;
    int scan_lenght = 0;
    memset(rule_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.From, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.To, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Reserved, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.In, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.On, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.At, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Save, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Letter, '\0', MAX_LENGHT_DATA_FIELD);
    memset(rule_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);

    scan_lenght = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%[^\r\n]",
        rule_data.Field,
        rule_data.Name,
        rule_data.From,
        rule_data.To,
        rule_data.Reserved,
        rule_data.In,
        rule_data.On,
        rule_data.At,
        rule_data.Save,
        rule_data.Letter,
        rule_data.Comment);

    sprintf(rule_data.Field, "%s", rule_data.Field);
    sprintf(rule_data.Name, "%s", rule_data.Name);
    sprintf(rule_data.From, "%s", rule_data.From);
    sprintf(rule_data.To, "%s", rule_data.To);
    sprintf(rule_data.Reserved, "%s", rule_data.Reserved);
    sprintf(rule_data.In, "%s", rule_data.In);
    sprintf(rule_data.On, "%s", rule_data.On);
    sprintf(rule_data.At, "%s", rule_data.At);
    sprintf(rule_data.Save, "%s", rule_data.Save);
    sprintf(rule_data.Letter, "%s", rule_data.Letter);
    sprintf(rule_data.Comment, "%s", rule_data.Comment);

    if (strcmp("Rule", rule_data.Field) != 0 || scan_lenght<10)
    {
        memset(rule_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Name, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.From, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.To, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Reserved, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.In, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.On, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.At, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Save, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Letter, '\0', MAX_LENGHT_DATA_FIELD);
        memset(rule_data.Comment, '\0', MAX_LENGHT_DATA_FIELD);
    }
    return rule_data;
}

int16_t Parse_Rule_Data_From(const Rule_Data_t rule_data)
{
    return atoi(rule_data.From);
}

int16_t Parse_Rule_Data_To(const Rule_Data_t rule_data, int16_t max_value)
{
    if (strcmp(rule_data.To, "only") == 0)
    {
        return atoi(rule_data.From);
    }
    else if (strcmp(rule_data.To, "max") == 0)
    {
        return max_value;
    }
    else
    {
        return atoi(rule_data.To);
    }
    return 0;
}

uint8_t Parse_Rule_Data_In(const Rule_Data_t rule_data)
{
    for (int month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(rule_data.In, Month_Names[month_index].Abbr) == 0 || strcmp(rule_data.In, Month_Names[month_index].Full) == 0)
        {
            return Month_Names[month_index].Number + 1;
        }
    }
    return 0;
}

Rule_Year_Day_t Parse_Rule_Data_On(const Rule_Data_t rule_data)
{
    Rule_Year_Day_t day = { 0 };
    day.Day = 0;
    day.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
    day.Weekday_isAfterOrEqual_Day = false;

    if (strlen(rule_data.On) <= 3)
    {
        day.Day = atoi(rule_data.On);
        day.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
        day.Weekday_isAfterOrEqual_Day = false;
    }
    else
    {
        uint8_t dow[10], d[10];
        int sscanf_lenght = sscanf(rule_data.On, "%[^=]=%s", dow, d);
        sprintf(dow, "%s", dow);
        sprintf(d, "%s", d);
        if (sscanf_lenght == 1)
        {
            for (int last_week_index = 0; last_week_index < TZDB_WEEKDAY_TOTAL; last_week_index++)
            {
                if (strcmp(dow, Last_Weekday_Names[last_week_index].Full) == 0)
                {
                    day.Day = 0; // Last Week
                    day.Weekday = Last_Weekday_Names[last_week_index].Number;
                    day.Weekday_isAfterOrEqual_Day = false;
                    return day;
                }
            }
        }
        else
        {
            int len = strlen(dow);
            if (dow[len - 1] == '>')
            {
                day.Weekday_isAfterOrEqual_Day = true;
            }
            else if (dow[len - 1] == '<')
            {
                day.Weekday_isAfterOrEqual_Day = false;
            }
            dow[len - 1] = '\0';
            for (int week_index = 0; week_index < TZDB_WEEKDAY_TOTAL; week_index++)
            {
                if (strcmp(dow, Weekday_Names[week_index].Abbr) == 0 || strcmp(dow, Weekday_Names[week_index].Full) == 0)
                {
                    day.Day = atoi(d);
                    day.Weekday = Weekday_Names[week_index].Number;
                }
            }
        }
    }
    return day;
}

Rule_Year_Hour_t Parse_Rule_Data_At(const Rule_Data_t rule_data)
{
    Rule_Year_Hour_t hour = { 0 };
    hour.Hour = 0;
    hour.Hour_isUTC = false;

    uint8_t base = 0;
    uint32_t h = 0, m = 0;
    if (sscanf(rule_data.At, "%d:%d%c", &h, &m, &base) == 3)
    {
        hour.Hour = (h * 3600) + (m * 60);
        if (base == 'u')
        {
            hour.Hour_isUTC = true;
        }
        else if (base == 's')
        {
            hour.Hour_isUTC = false;
        }
    }
    else if (sscanf(rule_data.At, "%d:%d", &h, &m) == 2)
    {
        hour.Hour = (h * 3600) + (m * 60);
        hour.Hour_isUTC = false;
    }

    return hour;
}

int32_t Parse_Rule_Data_Save(const Rule_Data_t rule_data)
{
    int32_t save = 0;
    uint8_t sign = 0;
    uint32_t h = 0, m = 0;
    if (sscanf(rule_data.Save, "%c%d:%d", &sign, &h, &m) == 3)
    {
        save = (h * 3600) + (m * 60);
        if (sign == '-')
        {
            save *= -1;
        }
    }
    else if (sscanf(rule_data.Save, "%d:%d", &h, &m) == 2)
    {
        save = (h * 3600) + (m * 60);
    }
    else if (sscanf(rule_data.Save, "%d", &h) == 1)
    {
        save = (h * 3600);
    }
    return save;
}

bool Rule_isExist(const Rule_Entry_t* rule_list, const int32_t* rules_count, const char* rule_name, int32_t* find_Index)
{
    if (rule_list != NULL && ((*rules_count) > 0))
    {
        for (int rule_index = 0; rule_index < (*rules_count); rule_index++)
        {
            if (strcmp(rule_list[rule_index].Name, rule_name) == 0)
            {
                *find_Index = rule_index;
                return true;
            }
        }
    }
    return false;
}

bool Rule_Create(Rule_Entry_t** rule_list, const int32_t rules_Count, const char* rule_name)
{
    Rule_Entry_t rule = { 0 };
    rule.Name = (uint8_t*)malloc((strlen(rule_name) + 1) * sizeof(uint8_t));
    if (rule.Name != NULL)
    {
        sprintf(rule.Name, "%s", rule_name);
    }
    rule.Years_Count = 0;
    rule.Year_Begin = 0;
    rule.Year_End = 0;

    Rule_Entry_t* rules = realloc(*rule_list, (rules_Count + 1) * sizeof(Rule_Entry_t));
    if (rules == NULL)
    {
        return false;
    }
    *rule_list = rules;
    (*rule_list)[rules_Count] = rule;
    return true;
}

void Parse_Rule_Year_Range(Rule_Entry_t* rule_list, const int32_t rule_index, const Rule_Data_t rule_data)
{
    int16_t year_from = Parse_Rule_Data_From(rule_data);
    int16_t year_to = Parse_Rule_Data_To(rule_data, -1);
    
    if (rule_list[rule_index].Year_Begin == 0)
    {
        rule_list[rule_index].Year_Begin = year_from;
    }
    else if (rule_list[rule_index].Year_Begin > year_from)
    {
        rule_list[rule_index].Year_Begin = year_from;
    }

    if (rule_list[rule_index].Year_End == 0)
    {
        rule_list[rule_index].Year_End = year_to;
    }
    else if (year_to == -1)
    {
        rule_list[rule_index].Year_End = year_to;
    }
    else if (rule_list[rule_index].Year_End < year_to)
    {
        rule_list[rule_index].Year_End = year_to;
    }
}

void Parse_Rule_Years(Rule_Entry_t* rule_list, int32_t* rules_Count)
{
    uint8_t line[2048];
    uint16_t dataFile_index = 0;


    for (dataFile_index = 0; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {
        FILE* data_File = fopen(Data_FileName[dataFile_index], "r");
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

            Rule_Data_t rule_data = Parse_Rule_Data(line);
            if (strlen(rule_data.Name) <= 0)
            {
                continue;
            }

            int32_t rule_find_index;
            rule_find_index = -1;
            if (Rule_isExist(rule_list, rules_Count, rule_data.Name, &rule_find_index))
            {
                if (rule_find_index >= 0)
                {
                    int16_t year_from = Parse_Rule_Data_From(rule_data);
                    int16_t year_to = Parse_Rule_Data_To(rule_data, rule_list[rule_find_index].Year_End);
                    int16_t years_count = 0;
                    int32_t year_find_index;
                    if (year_to > year_from)
                    {
                        years_count = (year_to - year_from) + 1;
                        for (int year_index = 0; year_index <= years_count; year_index++)
                        {
                            year_find_index = -1;
                            if (!Rule_Year_isExist(rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_from, &year_find_index))
                            {
                                if (Rule_Year_Create(&rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_from, rule_data))
                                {
                                    rule_list[rule_find_index].Years_Count++;
                                }
                            }
                            else
                            {
                                int32_t save_hour = Parse_Rule_Data_Save(rule_data);
                                if (save_hour == 0)
                                {
                                    if (Rule_Year_Add_Data(&(rule_list[rule_find_index].Years[year_find_index].Standard), rule_list[rule_find_index].Years[year_find_index].Standard_Count, rule_data))
                                    {
                                        rule_list[rule_find_index].Years[year_find_index].Standard_Count++;
                                    }
                                }
                                else
                                {
                                    if (Rule_Year_Add_Data(&(rule_list[rule_find_index].Years[year_find_index].DST), rule_list[rule_find_index].Years[year_find_index].DST_Count, rule_data))
                                    {
                                        rule_list[rule_find_index].Years[year_find_index].DST_Count++;
                                    }
                                }
                            }
                            year_from++;
                        }
                    }
                    else
                    {
                        year_find_index = -1;
                        if (!Rule_Year_isExist(rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_to, &year_find_index))
                        {
                            if (Rule_Year_Create(&rule_list[rule_find_index].Years, rule_list[rule_find_index].Years_Count, year_from, year_to, rule_data))
                            {
                                rule_list[rule_find_index].Years_Count++;
                            }
                        }
                        else
                        {
                            int32_t save_hour = Parse_Rule_Data_Save(rule_data);
                            if (save_hour == 0)
                            {
                                if (Rule_Year_Add_Data(&rule_list[rule_find_index].Years[year_find_index].Standard, rule_list[rule_find_index].Years[year_find_index].Standard_Count, rule_data))
                                {
                                    rule_list[rule_find_index].Years[year_find_index].Standard_Count++;
                                }
                            }
                            else
                            {
                                if (Rule_Year_Add_Data(&rule_list[rule_find_index].Years[year_find_index].DST, rule_list[rule_find_index].Years[year_find_index].DST_Count, rule_data))
                                {
                                    rule_list[rule_find_index].Years[year_find_index].DST_Count++;
                                }
                            }
                        }
                    }
                    
                }
            }
        }
        fclose(data_File);
    }
}

bool Rule_Year_isExist(const Rule_Year_t* year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, int32_t* find_index)
{
    if (year_list != NULL && (years_count > 0))
    {
        for (int year_index = 0; year_index < years_count; year_index++)
        {
            if (year_list[year_index].From == year_from && year_list[year_index].To == year_to)
            {
                (*find_index) = year_index;
                return true;
            }
        }
    }
    return false;
}

bool Rule_Year_Create(Rule_Year_t** year_list, const uint32_t years_count, const int16_t year_from, const int16_t year_to, const Rule_Data_t rule_data)
{
    Rule_Year_t rule_year = { 0 };
    
    rule_year.From = year_from;
    rule_year.To = year_to;
    rule_year.Reserved = (uint8_t*)malloc((strlen(rule_data.Reserved) + 1) * sizeof(uint8_t));
    if (rule_year.Reserved != NULL)
    {
        if (rule_data.Reserved[0] == '-')
        {
            sprintf(rule_year.Reserved, "%c", '\0');
        }
        else
        {
            sprintf(rule_year.Reserved, "%s", rule_data.Reserved);
        }
    }
    rule_year.Standard_Count = 0;
    rule_year.DST_Count = 0;

    int32_t save_hour = Parse_Rule_Data_Save(rule_data);
    if (save_hour == 0)
    {
        if (Rule_Year_Add_Data(&rule_year.Standard, rule_year.Standard_Count, rule_data))
        {
            rule_year.Standard_Count++;
        }
    }
    else
    {
        if (Rule_Year_Add_Data(&rule_year.DST, rule_year.DST_Count, rule_data))
        {
            rule_year.DST_Count++;
        }
    }
   

    Rule_Year_t* year = (Rule_Year_t*)realloc(*year_list, (years_count + 1) * sizeof(Rule_Year_t));
    if (year != NULL)
    {
        *year_list = year;
        (*year_list)[years_count] = rule_year;
        return true;
    }
    return false;
}

bool Rule_Year_Add_Data(Rule_Year_Data_t** year_data, const uint16_t count,  const Rule_Data_t rule_data)
{
    Rule_Year_Data_t rule_year_data = { 0 };
    rule_year_data.Month = Parse_Rule_Data_In(rule_data);
    rule_year_data.Day = Parse_Rule_Data_On(rule_data);
    rule_year_data.Hour = Parse_Rule_Data_At(rule_data);
    rule_year_data.Save_Hour = Parse_Rule_Data_Save(rule_data);
    rule_year_data.Letter = (uint8_t*)malloc((strlen(rule_data.Letter) + 1) * sizeof(uint8_t));
    if (rule_data.Letter != NULL)
    {
        if (rule_data.Letter[0] == '-')
        {
            sprintf(rule_year_data.Letter, "%c", '\0');
        }
        else
        {
            sprintf(rule_year_data.Letter, "%s", rule_data.Letter);
        }
    }
    rule_year_data.Comment = (uint8_t*)malloc((strlen(rule_data.Comment) + 1) * sizeof(uint8_t));
    if (rule_year_data.Comment != NULL)
    {
        sprintf(rule_year_data.Comment, "%s", rule_data.Comment);
    }

    Rule_Year_Data_t* data = (Rule_Year_Data_t*)realloc(*year_data, (count + 1) * sizeof(Rule_Year_Data_t));
    if (data != NULL)
    {
        *year_data = data;
        (*year_data)[count] = rule_year_data;
        return true;
    }
    return false;
}
