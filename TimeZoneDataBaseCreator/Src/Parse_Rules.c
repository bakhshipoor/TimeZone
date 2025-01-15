#include "../Inc/Parse_Rules.h"

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

            if (strcmp(rule, field) != 0)
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
                Rule_Year_t* data = realloc(Rules_List[rule_exist_index].Years, (Rules_List[rule_exist_index].Years_Count + 1) * sizeof(Rule_Year_t));
                if (data != NULL)
                {
                    Rules_List[rule_exist_index].Years = data;
                    Rules_List[rule_exist_index].Years[Rules_List[rule_exist_index].Years_Count] = Parse_Rule_Year(from, to, reserved, in, on, at, save, letter, comment);
                    Rules_List[rule_exist_index].Years_Count++;
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

                entry.Years_Count = 0;
                Rule_Year_t* data = realloc(entry.Years, (entry.Years_Count + 1) * sizeof(Rule_Year_t));
                if (data != NULL)
                {
                    entry.Years = data;
                    entry.Years[entry.Years_Count] = Parse_Rule_Year(from, to, reserved, in, on, at, save, letter, comment);
                    entry.Years_Count++;
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

Rule_Year_t Parse_Rule_Year(const char* from, const char* to, const char* reserved, const char* in, const char* on, const char* at, const char* save, const char* letter, const char* comment)
{
    Rule_Year_t rule_data = { 0 };
    //int sscanf_lenght;

    //rule_data.From = atoi(from);
    //if (strcmp(to, "only") == 0)
    //{
    //    rule_data.To = rule_data.From;
    //}
    //else if (strcmp(to, "max") == 0)
    //{
    //    rule_data.To = RULE_MAX_VALUE;
    //}
    //else
    //{
    //    rule_data.To = atoi(to);
    //}

    //rule_data.Reserved = (uint8_t*)malloc((strlen(reserved) + 1) * sizeof(uint8_t));
    //if (rule_data.Reserved != NULL)
    //{
    //    if (reserved[0] == '-')
    //    {
    //        sprintf(rule_data.Reserved, "%c", '\0');
    //    }
    //    else
    //    {
    //        sprintf(rule_data.Reserved, "%s", reserved);
    //    }
    //}

    //for (int month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    //{
    //    if (strcmp(in, Month_Names[month_index].Abbr) == 0 || strcmp(in, Month_Names[month_index].Full) == 0)
    //    {
    //        rule_data.Month = Month_Names[month_index].Number;
    //    }
    //}

    //if (strlen(on) <= 3)
    //{
    //    rule_data.Day = atoi(on);
    //    rule_data.Weekday = (uint8_t)TZDB_WEEKDAY_NONE;
    //    rule_data.Wday_After_Or_Before_Day = false;

    //}
    //else
    //{
    //    uint8_t dow[10], d[10];
    //    sscanf_lenght = sscanf(on, "%[^=]=%s", dow, d);
    //    sprintf(dow, "%s", dow);
    //    sprintf(d, "%s", d);
    //    if (sscanf_lenght == 1)
    //    {
    //        for (int last_week_index = 0; last_week_index < TZDB_WEEKDAY_TOTAL; last_week_index++)
    //        {
    //            if (strcmp(dow, Last_Weekday_Names[last_week_index].Full) == 0)
    //            {
    //                rule_data.Weekday = Last_Weekday_Names[last_week_index].Number;
    //                rule_data.Day = 0; // Last Week
    //            }
    //        }
    //    }
    //    else
    //    {
    //        int len = strlen(dow);
    //        if (dow[len - 1] == '>')
    //        {
    //            rule_data.Wday_After_Or_Before_Day = true;
    //        }
    //        else if (dow[len - 1] == '<')
    //        {
    //            rule_data.Wday_After_Or_Before_Day = false;
    //        }
    //        dow[len - 1] = '\0';
    //        for (int week_index = 0; week_index < TZDB_WEEKDAY_TOTAL; week_index++)
    //        {
    //            if (strcmp(dow, Weekday_Names[week_index].Abbr) == 0 || strcmp(dow, Weekday_Names[week_index].Full) == 0)
    //            {
    //                rule_data.Weekday = Weekday_Names[week_index].Number;
    //                rule_data.Day = atoi(d);
    //            }
    //        }
    //    }
    //}

    //uint8_t base=0;
    //uint32_t hour=0, minute=0;
    //if (sscanf(at, "%d:%d%c", &hour, &minute, &base) == 3)
    //{
    //    rule_data.Hour = (hour * 3600) + (minute * 60);
    //    if (base == 'u')
    //    {
    //        rule_data.UTC_Or_Local_Hour = true;
    //    }
    //    else if (base == 's')
    //    {
    //        rule_data.UTC_Or_Local_Hour = false;
    //    }
    //}
    //else if (sscanf(at, "%d:%d", &hour, &minute) == 2)
    //{
    //    rule_data.Hour = (hour * 3600) + (minute * 60);
    //    rule_data.UTC_Or_Local_Hour = false;
    //}

    //uint8_t sign = 0;
    //if (sscanf(save, "%c%d:%d", &sign, &hour, &minute) == 3)
    //{
    //    rule_data.Save_Hour = (hour * 3600) + (minute * 60);
    //    if (sign == '-')
    //    {
    //        rule_data.Save_Hour *= -1;
    //    }
    //}
    //else if (sscanf(save, "%d:%d", &hour, &minute) == 2)
    //{
    //    rule_data.Save_Hour = (hour * 3600) + (minute * 60);
    //}
    //else if (sscanf(save, "%d", &hour) == 1)
    //{
    //    rule_data.Save_Hour = (hour * 3600);
    //}
    //
    //rule_data.Letter = (uint8_t*)malloc((strlen(letter) + 1) * sizeof(uint8_t));
    //if (rule_data.Letter != NULL)
    //{
    //    if (letter[0] == '-')
    //    {
    //        sprintf(rule_data.Letter, "%c", '\0');
    //    }
    //    else
    //    {
    //        sprintf(rule_data.Letter, "%s", letter);
    //    }
    //    
    //}

    //rule_data.Comment = (uint8_t*)malloc((strlen(comment) + 1) * sizeof(uint8_t));
    //if (rule_data.Comment != NULL)
    //{
    //    sprintf(rule_data.Comment, "%s", comment);
    //}

    return rule_data;
}