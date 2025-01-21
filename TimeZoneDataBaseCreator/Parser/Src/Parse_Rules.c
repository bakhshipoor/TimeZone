#include "../Inc/Parse_Rules.h"

Rule_Entry_t* Parse_Rules(COUNTER* rules_Count)
{

    Rule_Entry_t* Rules_List = NULL;
    *rules_Count = 0;
    FILE* data_File = NULL;
    COUNTER dataFile_index = 0;
    COUNTER find_index;
    Rule_Data_t* rule_data = NULL;

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


            Parse_Free_Rule_Data(&rule_data);
            rule_data = Parse_Rule_Data(&line);


            if (rule_data == NULL)
            {
                continue;
            }

            find_index = -1;


            if (!Rule_isExist(&Rules_List, rules_Count, &rule_data->Name, &find_index))
            {

                if (Rule_Create(&Rules_List, rules_Count, &rule_data->Name))
                {
                    find_index = (*rules_Count);
                    (*rules_Count)++;
                }
            }

            if (find_index != -1)
            {

                Parse_Rule_Year_Range(&Rules_List, &find_index, &rule_data);
            }
        }


        fclose(data_File);
    }


    Parse_Free_Rule_Data(&rule_data);

    Parse_Rule_Years(&Rules_List, rules_Count);

    test(Rules_List, rules_Count);


    return Rules_List;
}

BOOL Rule_isExist(CONST Rule_Entry_t** rule_list, CONST COUNTER* rules_count, CONST CHAR** rule_name, COUNTER* find_Index)
{

    if (*rule_list != NULL && ((*rules_count) > 0))
    {

        for (COUNTER rule_index = 0; rule_index < (*rules_count); rule_index++)
        {

            if (strcmp((*rule_list)[rule_index].Name, *rule_name) == 0)
            {

                *find_Index = rule_index;
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL Rule_Create(Rule_Entry_t** rule_list, CONST COUNTER* rules_Count, CONST CHAR** rule_name)
{
    if (rules_Count == NULL || *rule_name == NULL)
    {
        return FALSE;
    }

    Rule_Entry_t* rule = (Rule_Entry_t*)calloc(1,sizeof(Rule_Entry_t));
    if (rule == NULL)
    {
        return FALSE;
    }
    
    rule->Name = (CHAR*)malloc((strlen(*rule_name) + 1) * sizeof(CHAR));
    if (rule->Name != NULL)
    {

        sprintf(rule->Name, "%s", *rule_name);
    }


    rule->Years_Count = 0;
    rule->Year_Begin = 0;
    rule->Year_End = 0;


    Rule_Entry_t* rules;
    if (rules_Count == 0)
    {
        rules = malloc(sizeof(Rule_Entry_t));
    }
    else
    {
        rules = realloc(*rule_list, (*rules_Count + 1) * sizeof(Rule_Entry_t));
    }
    if (rules == NULL)
    {
        free(rule);
        rule = NULL;
        return FALSE;
    }
    *rule_list = rules;


    (*rule_list)[(*rules_Count)] = *rule;


    return TRUE;
}

VOID Parse_Rule_Year_Range(Rule_Entry_t** rule_list, CONST COUNTER* rule_index, CONST Rule_Data_t** rule_data)
{

    YEAR* year_from = Parse_Rule_Data_From(rule_data);
    YEAR max = -1;
    YEAR* year_to = Parse_Rule_Data_To(rule_data, &max);
    if (year_from == NULL || year_to == NULL)
    {
        return;
    }


    if ((*rule_list)[*rule_index].Year_Begin == 0)
    {

        (*rule_list)[*rule_index].Year_Begin = *year_from;
    }

    else if ((*rule_list)[*rule_index].Year_Begin > *year_from)
    {
        (*rule_list)[*rule_index].Year_Begin = *year_from;
    }


    if ((*rule_list)[*rule_index].Year_End == 0)
    {

        (*rule_list)[*rule_index].Year_End = *year_to;
    }

    else if (*year_to == -1)
    {
        (*rule_list)[*rule_index].Year_End = * year_to;
    }

    else if ((*rule_list)[*rule_index].Year_End < *year_to)
    {
        (*rule_list)[*rule_index].Year_End = *year_to;
    }
}

VOID Parse_Rule_Years(Rule_Entry_t** rule_list, COUNTER* rules_Count)
{
    FILE* data_File;
    COUNTER dataFile_index = 0;
    COUNTER rule_find_index;
    YEAR* year_from = (YEAR*)calloc(1, sizeof(YEAR));
    YEAR* year_to= (YEAR*)calloc(1, sizeof(YEAR));
    COUNTER years_count;
    COUNTER year_find_index;
    COUNTER year_index;
    HOUR* save_hour = NULL;
    Rule_Data_t* rule_data = NULL;

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

            Parse_Free_Rule_Data(&rule_data);
            rule_data = Parse_Rule_Data(&line);
            if (rule_data == NULL)
            {
                continue;
            }

            rule_find_index = -1;

            if (Rule_isExist(rule_list, rules_Count, &rule_data->Name, &rule_find_index))
            {
                if (rule_find_index >= 0)
                {
                    year_from = Parse_Rule_Data_From(&rule_data);
                    year_to = Parse_Rule_Data_To(&rule_data, &(*rule_list)[rule_find_index].Year_End);
                    years_count = 0;
                    year_find_index = -1;
                    if (year_from == NULL || year_to == NULL)
                    {
                        continue;
                    }

                    if (*year_to > *year_from)
                    {
                        years_count = (*year_to - *year_from) + 1;

                        for (year_index = 0; year_index <= years_count; year_index++)
                        {
                            year_find_index = -1;

                            if (!Rule_Year_isExist(&(*rule_list)[rule_find_index].Years, &(*rule_list)[rule_find_index].Years_Count, year_from, year_from, &year_find_index))
                            {

                                if (Rule_Year_Create(&(*rule_list)[rule_find_index].Years, &(*rule_list)[rule_find_index].Years_Count, year_from, year_from, &rule_data))
                                {
                                    (*rule_list)[rule_find_index].Years_Count++;
                                }
                            }
                            else
                            {

                                save_hour = Parse_Rule_Data_Save(&rule_data);
                                if (save_hour != NULL)
                                {

                                    if (*save_hour == 0)
                                    {
                                        if (Rule_Year_Add_Data(&((*rule_list)[rule_find_index].Years[year_find_index].Standard), &(*rule_list)[rule_find_index].Years[year_find_index].Standard_Count, &rule_data))
                                        {
                                            (*rule_list)[rule_find_index].Years[year_find_index].Standard_Count++;
                                        }
                                    }
                                    else
                                    {
                                        if (Rule_Year_Add_Data(&((*rule_list)[rule_find_index].Years[year_find_index].DST), &(*rule_list)[rule_find_index].Years[year_find_index].DST_Count, &rule_data))
                                        {
                                            (*rule_list)[rule_find_index].Years[year_find_index].DST_Count++;
                                        }
                                    }
                                    free(save_hour);
                                    save_hour = NULL;
                                }
                            }
                            year_from++;
                        }
                    }
                    else
                    {

                        year_find_index = -1;

                        if (!Rule_Year_isExist(&(*rule_list)[rule_find_index].Years, &(*rule_list)[rule_find_index].Years_Count, year_from, year_to, &year_find_index))
                        {

                            if (Rule_Year_Create(&(*rule_list)[rule_find_index].Years, &(*rule_list)[rule_find_index].Years_Count, year_from, year_to, &rule_data))
                            {
                                (*rule_list)[rule_find_index].Years_Count++;
                            }
                        }
                        else
                        {

                            save_hour = Parse_Rule_Data_Save(rule_data);
                            if (save_hour != NULL)
                            {

                                if (*save_hour == 0)
                                {
                                    if (Rule_Year_Add_Data(&(*rule_list)[rule_find_index].Years[year_find_index].Standard, &(*rule_list)[rule_find_index].Years[year_find_index].Standard_Count, &rule_data))
                                    {
                                        (*rule_list)[rule_find_index].Years[year_find_index].Standard_Count++;
                                    }
                                }
                                else
                                {
                                    if (Rule_Year_Add_Data(&(*rule_list)[rule_find_index].Years[year_find_index].DST, &(*rule_list)[rule_find_index].Years[year_find_index].DST_Count, &rule_data))
                                    {
                                        (*rule_list)[rule_find_index].Years[year_find_index].DST_Count++;
                                    }
                                }
                                free(save_hour);
                                save_hour = NULL;
                            }
                        }
                    }
                }
            }
        }
        fclose(data_File);
    }
    Parse_Free_Rule_Data(&rule_data);
}

BOOL Rule_Year_isExist(CONST Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, COUNTER* find_index)
{

    if (*year_list != NULL && (*years_count > 0))
    {
        for (int year_index = 0; year_index < *years_count; year_index++)
        {
            if (((*year_list)[year_index].From == (*year_from)) && ((*year_list)[year_index].To == (*year_to)))
            {
                (*find_index) = year_index;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL Rule_Year_Create(Rule_Year_t** year_list, CONST COUNTER* years_count, CONST YEAR* year_from, CONST YEAR* year_to, CONST Rule_Data_t** rule_data)
{

    Rule_Year_t* rule_year = (Rule_Year_t*)calloc(1, sizeof(Rule_Year_t));
    if (rule_year == NULL)
    {
        return false;
    }


    rule_year->From = *year_from;
    rule_year->To = *year_to;


    rule_year->Reserved = (CHAR*)malloc((strlen((*rule_data)->Reserved) + 1) * sizeof(CHAR));
    if (rule_year->Reserved != NULL)
    {

        if ((*rule_data)->Reserved[0] == '-')
        {
            sprintf(rule_year->Reserved, "%c", '\0');
        }

        else
        {
            sprintf(rule_year->Reserved, "%s", (*rule_data)->Reserved);
        }
    }


    rule_year->Standard_Count = 0;
    rule_year->DST_Count = 0;


    HOUR* save_hour = Parse_Rule_Data_Save(rule_data);
    if (save_hour != NULL)
    {

        if (*save_hour == 0)
        {
            if (Rule_Year_Add_Data(&rule_year->Standard, &rule_year->Standard_Count, rule_data))
            {
                rule_year->Standard_Count++;
            }
        }
        else
        {
            if (Rule_Year_Add_Data(&rule_year->DST, &rule_year->DST_Count, rule_data))
            {
                rule_year->DST_Count++;
            }
        }
        /*free(save_hour);
        save_hour = NULL;*/
    }

    Rule_Year_t* year;
    if (*years_count == 0)
    {
        year = (Rule_Year_t*)malloc(sizeof(Rule_Year_t));
    }
    else
    {
        year = (Rule_Year_t*)realloc(*year_list, (*years_count + 1) * sizeof(Rule_Year_t));
    }
    if (year == NULL)
    {
        free(rule_year);
        rule_year = NULL;
        return FALSE;
    }
    *year_list = year;
    (*year_list)[*years_count] = *rule_year;

    return TRUE;
}

BOOL Rule_Year_Add_Data(Rule_Year_Data_t** year_data, CONST COUNTER* count, CONST Rule_Data_t** rule_data)
{

    Rule_Year_Data_t* rule_year_data = (Rule_Year_Data_t*)malloc(sizeof(Rule_Year_Data_t));
    if (rule_year_data == NULL)
    {
        return FALSE;
    }


    MONTH* m = Parse_Rule_Data_In(rule_data);
    if (m != NULL)
    {
        rule_year_data->Month = *m;
    }
    Rule_Year_Day_t* day = Parse_Rule_Data_On(rule_data);
    if (day != NULL)
    {
        rule_year_data->Day = *day;
    }
    Rule_Year_Hour_t* hour = Parse_Rule_Data_At(rule_data);
    if (hour != NULL)
    {
        rule_year_data->Hour = *hour;
    }
    HOUR* s_h = Parse_Rule_Data_Save(rule_data);
    if (s_h != NULL)
    {
        rule_year_data->Save_Hour = *s_h;
    }


    rule_year_data->Letter = (CHAR*)malloc((strlen((*rule_data)->Letter) + 1) * sizeof(CHAR));
    if ((*rule_data)->Letter != NULL)
    {

        if ((*rule_data)->Letter[0] == '-')
        {
            sprintf(rule_year_data->Letter, "%c", '\0');
        }

        else
        {
            sprintf(rule_year_data->Letter, "%s", (*rule_data)->Letter);
        }
    }


    rule_year_data->Comment = (CHAR*)malloc((strlen((*rule_data)->Comment) + 1) * sizeof(CHAR));
    if (rule_year_data->Comment != NULL)
    {
        sprintf(rule_year_data->Comment, "%s", (*rule_data)->Comment);
    }


    Rule_Year_Data_t* data;
    if (*count == 0)
    {
        data = (Rule_Year_Data_t*)malloc(sizeof(Rule_Year_Data_t));
    }
    else
    {
        data = (Rule_Year_Data_t*)realloc(*year_data, (*count + 1) * sizeof(Rule_Year_Data_t));
    }
    if (data != NULL)
    {

        *year_data = data;
        (*year_data)[*count] = *rule_year_data;
        return TRUE;
    }

    return FALSE;
}



Rule_Data_t* Parse_Rule_Data(CONST CHAR** line)
{
    Rule_Data_t* rule_data = (Rule_Data_t*)malloc(sizeof(Rule_Data_t));
    if (rule_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;


    rule_data->Field = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->Name = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->From = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->To = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->Reserved = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->In = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->On = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->At = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->Save = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->Letter = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    rule_data->Comment = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));


    scan_lenght = sscanf(*line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%[^\r\n]",
        rule_data->Field,
        rule_data->Name,
        rule_data->From,
        rule_data->To,
        rule_data->Reserved,
        rule_data->In,
        rule_data->On,
        rule_data->At,
        rule_data->Save,
        rule_data->Letter,
        rule_data->Comment);


    sprintf(rule_data->Field, "%s", rule_data->Field);
    sprintf(rule_data->Name, "%s", rule_data->Name);
    sprintf(rule_data->From, "%s", rule_data->From);
    sprintf(rule_data->To, "%s", rule_data->To);
    sprintf(rule_data->Reserved, "%s", rule_data->Reserved);
    sprintf(rule_data->In, "%s", rule_data->In);
    sprintf(rule_data->On, "%s", rule_data->On);
    sprintf(rule_data->At, "%s", rule_data->At);
    sprintf(rule_data->Save, "%s", rule_data->Save);
    sprintf(rule_data->Letter, "%s", rule_data->Letter);
    sprintf(rule_data->Comment, "%s", rule_data->Comment);


    if (strcmp("Rule", rule_data->Field) != 0 || scan_lenght < 10)
    {
        Parse_Free_Rule_Data(&rule_data);
    }


    return rule_data;
}

YEAR* Parse_Rule_Data_From(CONST Rule_Data_t** rule_data)
{
    YEAR* yf = (YEAR*)calloc(1, sizeof(YEAR));
    if (yf == NULL)
    {
        return NULL;
    }
    *yf = 0;
    *yf = atoi((*rule_data)->From);
    return yf;
}

YEAR* Parse_Rule_Data_To(CONST Rule_Data_t** rule_data, YEAR* max_value)
{
    YEAR* y = (YEAR*)malloc(sizeof(YEAR));
    if (y == NULL)
    {
        return NULL;
    }    

    if (strcmp((*rule_data)->To, "only") == 0)
    {
        *y= atoi((*rule_data)->From);
    }
    else if (strcmp((*rule_data)->To, "max") == 0)
    {
        y = max_value;
    }
    else
    {
        *y = atoi((*rule_data)->To);
    }

    return y;
}

MONTH* Parse_Rule_Data_In(CONST Rule_Data_t** rule_data)
{
    MONTH* m = (MONTH*)malloc(sizeof(MONTH));
    if (m == NULL)
    {
        return NULL;
    }
    m = Parse_Month(&(*rule_data)->In);
    return m;
}

Rule_Year_Day_t* Parse_Rule_Data_On(CONST Rule_Data_t** rule_data)
{
    Rule_Year_Day_t* day = (Rule_Year_Day_t*)malloc(sizeof(Rule_Year_Day_t));
    if (day != NULL)
    {
        Parse_Day_Of_Month(&(*rule_data)->On, &day->Day, &day->Weekday, &day->Weekday_isAfterOrEqual_Day);
    }
    return day;
}

Rule_Year_Hour_t* Parse_Rule_Data_At(CONST Rule_Data_t** rule_data)
{

    Rule_Year_Hour_t* hour = (Rule_Year_Hour_t*)malloc(sizeof(Rule_Year_Hour_t));
    if (hour == NULL)
    {
        return NULL;
    }
    CHAR s;
    HOUR* h = Parse_Hour(&(*rule_data)->At, &s);
    if (h != NULL)
    {
        hour->Hour = *h;
        hour->Hour_isUTC = FALSE;
        if (s == 'u')
        {
            hour->Hour_isUTC = TRUE;
        }
    }


    return hour;
}

HOUR* Parse_Rule_Data_Save(CONST Rule_Data_t** rule_data)
{
    CHAR s;
    HOUR* h = Parse_Hour(&(*rule_data)->Save, &s);
    return h;
}



VOID Parse_Free_Rule_Data(Rule_Data_t** rule_data)
{
    if (*rule_data != NULL)
    {
        if ((*rule_data)->Field != NULL)
        {
            free((*rule_data)->Field);
            (*rule_data)->Field = NULL;
        }
        if ((*rule_data)->Name != NULL)
        {
            free((*rule_data)->Name);
            (*rule_data)->Name = NULL;
        }
        if ((*rule_data)->From != NULL)
        {
            free((*rule_data)->From);
            (*rule_data)->From = NULL;
        }
        if ((*rule_data)->To != NULL)
        {
            free((*rule_data)->To);
            (*rule_data)->To = NULL;
        }
        if ((*rule_data)->Reserved != NULL)
        {
            free((*rule_data)->Reserved);
            (*rule_data)->Reserved = NULL;
        }
        if ((*rule_data)->In != NULL)
        {
            free((*rule_data)->In);
            (*rule_data)->In = NULL;
        }
        if ((*rule_data)->On != NULL)
        {
            free((*rule_data)->On);
            (*rule_data)->On = NULL;
        }
        if ((*rule_data)->At != NULL)
        {
            free((*rule_data)->At);
            (*rule_data)->At = NULL;
        }
        if ((*rule_data)->Save != NULL)
        {
            free((*rule_data)->Save);
            (*rule_data)->Save = NULL;
        }
        if ((*rule_data)->Letter != NULL)
        {
            free((*rule_data)->Letter);
            (*rule_data)->Letter = NULL;
        }
        if ((*rule_data)->Comment != NULL)
        {
            free((*rule_data)->Comment);
            (*rule_data)->Comment = NULL;
        }
        free((*rule_data));
        (*rule_data) = NULL;
    }
}

VOID Free_Standard_Entries(Rule_Year_Data_t* standards, COUNTER count)
{
    for (COUNTER i = 0; i < count; i++)
    {
        if (standards[i].Letter != NULL)
        {
            free(standards[i].Letter);
            standards[i].Letter = NULL;
        }
        if (standards[i].Comment != NULL)
        {
            free(standards[i].Comment);
            standards[i].Comment = NULL;
        }
    }
    free(standards);
}

VOID Free_DST_Entries(Rule_Year_Data_t* dst_entries, COUNTER count)
{
    for (COUNTER i = 0; i < count; i++)
    {
        if (dst_entries[i].Letter != NULL)
        {
            free(dst_entries[i].Letter);
            dst_entries[i].Letter = NULL;
        }
        if (dst_entries[i].Comment != NULL)
        {
            free(dst_entries[i].Comment);
            dst_entries[i].Comment = NULL;
        }
    }
    free(dst_entries);
}

VOID Parse_Free_Rules(Rule_Entry_t* rules, COUNTER rules_count)
{
    if (rules != NULL && rules_count > 0)
    {
        for (COUNTER rule_index = 0; rule_index < rules_count; rule_index++)
        {
            Rule_Entry_t* rule = &rules[rule_index];
            if (rule != NULL && rule->Years != NULL && rule->Years_Count > 0)
            {
                for (COUNTER year_index = 0; year_index < rule->Years_Count; year_index++)
                {
                    Rule_Year_t* year = &rule->Years[year_index];
                    if (year != NULL) {
                        if (year->Standard != NULL && year->Standard_Count > 0)
                        {
                            Free_Standard_Entries(year->Standard, year->Standard_Count);
                        }
                        if (year->DST != NULL && year->DST_Count > 0)
                        {
                            Free_DST_Entries(year->DST, year->DST_Count);
                        }
                    }
                }
                free(rule->Years);
                rule->Years = NULL;
            }
        }
        free(rules);
        rules = NULL;
    }
}



VOID test(Rule_Entry_t* rules, COUNTER* rules_count)
{
    uint64_t cc = 0;
    long long size = sizeof(rules);
    for (int i = 0; i < *rules_count; i++)
    {
        size += sizeof(rules);
        size += sizeof(rules->Name);
        size += (sizeof(CHAR) * strlen(rules->Name));
        size += sizeof(rules->Years_Count);
        size += sizeof(rules->Years);
        for (int j = 0; j < rules->Years_Count; j++)
        {
            size += sizeof(rules->Years->From);
            size += sizeof(rules->Years->To);
            size += sizeof(rules->Years->Reserved);
            size += (sizeof(CHAR) * strlen(rules->Years->Reserved));
            size += sizeof(rules->Years->Standard_Count);
            size += sizeof(rules->Years->Standard);
            for (int k = 0; k < rules->Years->Standard_Count; k++)
            {
                size += sizeof(rules->Years->Standard->Month);
                size += sizeof(rules->Years->Standard->Save_Hour);
                size += sizeof(rules->Years->Standard->Letter);
                size += (sizeof(CHAR) * strlen(rules->Years->Standard->Letter));
                size += sizeof(rules->Years->Standard->Comment);
                size += (sizeof(CHAR) * strlen(rules->Years->Standard->Comment));
                size += sizeof(rules->Years->Standard->Day);
                size += sizeof(rules->Years->Standard->Day.Day);
                size += sizeof(rules->Years->Standard->Day.Weekday);
                size += sizeof(rules->Years->Standard->Day.Weekday_isAfterOrEqual_Day);
                size += sizeof(rules->Years->Standard->Hour);
                size += sizeof(rules->Years->Standard->Hour.Hour);
                size += sizeof(rules->Years->Standard->Hour.Hour_isUTC);
                rules->Years->Standard++;
            }
            size += sizeof(rules->Years->DST_Count);
            size += sizeof(rules->Years->DST);
            for (int k = 0; k < rules->Years->DST_Count; k++)
            {
                size += sizeof(rules->Years->DST->Month);
                size += sizeof(rules->Years->DST->Save_Hour);
                size += sizeof(rules->Years->DST->Letter);
                size += (sizeof(CHAR) * strlen(rules->Years->DST->Letter));
                size += sizeof(rules->Years->DST->Comment);
                size += (sizeof(CHAR) * strlen(rules->Years->DST->Comment));
                size += sizeof(rules->Years->DST->Day);
                size += sizeof(rules->Years->DST->Day.Day);
                size += sizeof(rules->Years->DST->Day.Weekday);
                size += sizeof(rules->Years->DST->Day.Weekday_isAfterOrEqual_Day);
                size += sizeof(rules->Years->DST->Hour);
                size += sizeof(rules->Years->DST->Hour.Hour);
                size += sizeof(rules->Years->DST->Hour.Hour_isUTC);
                rules->Years->DST++;
            }
            rules->Years++;
        }
        size += sizeof(rules->Year_Begin);
        size += sizeof(rules->Year_End);
        rules++;
    }
}
