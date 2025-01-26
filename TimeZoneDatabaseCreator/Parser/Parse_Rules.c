#include "Parse_Rules.h"

typedef struct
{
    CHAR* Field;
    CHAR* Name;
    CHAR* From;
    CHAR* To;
    CHAR* Reserved;
    CHAR* In;
    CHAR* On;
    CHAR* At;
    CHAR* Save;
    CHAR* Letter;
    CHAR* Comment;
} Rule_Details_t;

STATIC BOOL Rule_isExist(CONST Rule_Entry_t** rule_list, CONST COUNTER* rules_count, CONST CHAR** rule_name, COUNTER* find_Index);
STATIC VOID Rule_Create(Rule_Entry_t** rule_list, COUNTER* rules_count, CONST CHAR** rule_name);
STATIC VOID Parse_Rule_Year_Range(Rule_Entry_t** rule_list, CONST COUNTER* rule_index, CONST Rule_Details_t* rule_data);
STATIC BOOL Rule_Year_Create(Rule_Data_t** year_list, CONST COUNTER* years_count, CONST Rule_Details_t* rule_data);
STATIC Rule_Details_t* Parse_Rule_Data(CONST CHAR** line);
STATIC YEAR Parse_Rule_Data_From(CONST Rule_Details_t* rule_data);
STATIC YEAR Parse_Rule_Data_To(CONST Rule_Details_t* rule_data, YEAR max_value);
STATIC MONTH Parse_Rule_Data_In(CONST Rule_Details_t* rule_data);
STATIC Rule_Data_Day_t Parse_Rule_Data_On(CONST Rule_Details_t* rule_data);
STATIC Rule_Data_Hour_t Parse_Rule_Data_At(CONST Rule_Details_t* rule_data);
STATIC HOUR Parse_Rule_Data_Save(CONST Rule_Details_t* rule_data);
STATIC VOID Free_Rule_Data(Rule_Details_t* rule_data);
STATIC VOID Free_Rule(Rule_Entry_t* rule);


VOID Parse_Rules(CONST CHAR** line, Rule_Entry_t** rule_list, COUNTER* rules_count)
{
    Rule_Details_t* rule_data = Parse_Rule_Data(line);
    if (rule_data == NULL)
    {
        return;
    }

    COUNTER find_index = INDEX_NOT_FOUND;
    if (!Rule_isExist(rule_list, rules_count, &rule_data->Name, &find_index))
    {
        Rule_Create(rule_list, rules_count, &rule_data->Name);
        find_index = (*rules_count) - 1;
    }

    if (find_index != INDEX_NOT_FOUND)
    {
        Parse_Rule_Year_Range(rule_list, &find_index, rule_data);
        if (Rule_Year_Create(&(*rule_list)[find_index].Years, &(*rule_list)[find_index].Years_Count, rule_data))
        {
            (*rule_list)[find_index].Years_Count++;
        }
    }
    Free_Rule_Data(rule_data);
    return;
}

STATIC BOOL Rule_isExist(CONST Rule_Entry_t** rule_list, CONST COUNTER* rules_count, CONST CHAR** rule_name, COUNTER* find_Index)
{
    if (rule_list != NULL && rules_count != NULL && *rules_count > 0)
    {
        for (COUNTER rule_index = 0; rule_index < *rules_count; rule_index++)
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

STATIC VOID Rule_Create(Rule_Entry_t** rule_list, COUNTER* rules_count, CONST CHAR** rule_name)
{
    if (rules_count == NULL || rule_name == NULL || *rule_name == NULL)
    {
        return FALSE;
    }

    Rule_Entry_t* rule = (Rule_Entry_t*)calloc(1, sizeof(Rule_Entry_t));
    if (rule == NULL)
    {
        return FALSE;
    }

    rule->Name = (CHAR*)calloc((utf8_strlen(*rule_name) + 1), sizeof(CHAR));
    if (rule->Name == NULL)
    {
        Free_Rule(rule);
        return FALSE;
    }

    strcpy(rule->Name, *rule_name);

    Rule_Entry_t* rules = realloc(*rule_list, (*rules_count + 1) * sizeof(Rule_Entry_t));
    if (rules == NULL)
    {
        Free_Rule(rule);
        return FALSE;
    }

    *rule_list = rules;
    (*rule_list)[*rules_count] = *rule;
    (*rules_count)++;

    free(rule);

    return TRUE;
}

STATIC VOID Parse_Rule_Year_Range(Rule_Entry_t** rule_list, CONST COUNTER* rule_index, CONST Rule_Details_t* rule_data)
{
    YEAR year_from = Parse_Rule_Data_From(rule_data);
    YEAR year_to = Parse_Rule_Data_To(rule_data, YEAR_END_MAX);
    
    if ((*rule_list)[*rule_index].Year_Begin == 0)
    {
        (*rule_list)[*rule_index].Year_Begin = year_from;
    }
    else if ((*rule_list)[*rule_index].Year_Begin > year_from)
    {
        (*rule_list)[*rule_index].Year_Begin = year_from;
    }

    if ((*rule_list)[*rule_index].Year_End == 0)
    {
        (*rule_list)[*rule_index].Year_End = year_to;
    }
    else if (year_to == -1)
    {
        (*rule_list)[*rule_index].Year_End = year_to;
    }
    else if ((*rule_list)[*rule_index].Year_End < year_to)
    {
        (*rule_list)[*rule_index].Year_End = year_to;
    }
}

STATIC BOOL Rule_Year_Create(Rule_Data_t** year_list, CONST COUNTER* years_count, CONST Rule_Details_t* rule_data)
{
    if (years_count == NULL || rule_data == NULL)
    {
        return FALSE;
    }

    Rule_Data_t* rule_year = (Rule_Data_t*)calloc(1, sizeof(Rule_Data_t));
    if (rule_year == NULL)
    {
        return false;
    }

    rule_year->From = Parse_Rule_Data_From(rule_data);
    rule_year->To = Parse_Rule_Data_To(rule_data, YEAR_END_MAX);
    rule_year->Reserved = (CHAR*)calloc((utf8_strlen(rule_data->Reserved) + 1), sizeof(CHAR));
    if (rule_year->Reserved != NULL && strcmp(rule_data->Reserved, "-") != 0)
    {
        strcpy(rule_year->Reserved, rule_data->Reserved);
    }
    rule_year->Month = Parse_Rule_Data_In(rule_data);
    rule_year->Day = Parse_Rule_Data_On(rule_data);
    rule_year->Hour = Parse_Rule_Data_At(rule_data);
    rule_year->Save_Hour = Parse_Rule_Data_Save(rule_data);
    rule_year->Letter = (CHAR*)calloc((utf8_strlen(rule_data->Letter) + 1), sizeof(CHAR));
    if (rule_year->Letter != NULL && strcmp(rule_data->Letter, "-") != 0)
    {
        strcpy(rule_year->Letter, rule_data->Letter);
    }
    rule_year->Comment = (CHAR*)calloc((utf8_strlen(rule_data->Comment) + 1), sizeof(CHAR));
    if (rule_year->Comment != NULL)
    {
        strcpy(rule_year->Comment, rule_data->Comment);
    }

    Rule_Data_t* year;
    if (*years_count == 0)
    {
        year = (Rule_Data_t*)malloc(sizeof(Rule_Data_t));
    }
    else
    {
        year = (Rule_Data_t*)realloc(*year_list, (*years_count + 1) * sizeof(Rule_Data_t));
    }
    if (year == NULL)
    {
        free(rule_year);
        rule_year = NULL;
        return FALSE;
    }
    *year_list = year;
    (*year_list)[*years_count] = *rule_year;

    free(rule_year);

    return TRUE;
}

STATIC Rule_Details_t* Parse_Rule_Data(CONST CHAR** line)
{
    Rule_Details_t* rule_data = (Rule_Details_t*)malloc(sizeof(Rule_Details_t));
    if (rule_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    rule_data->Field = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->Name = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->From = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->To = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->Reserved = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->In = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->On = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->At = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->Save = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->Letter = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));
    rule_data->Comment = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(char));

    if (rule_data->Field == NULL ||
        rule_data->Name == NULL ||
        rule_data->From == NULL ||
        rule_data->To == NULL ||
        rule_data->Reserved == NULL ||
        rule_data->In == NULL ||
        rule_data->On == NULL ||
        rule_data->At == NULL ||
        rule_data->Save == NULL ||
        rule_data->Letter == NULL ||
        rule_data->Comment == NULL)
    {
        Free_Rule_Data(rule_data);
        return NULL;
    }
    //scan_lenght = sscanf(*line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%[^#\32\n]# %[^\n]",
    scan_lenght = sscanf(*line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t# %s",
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

    if (strcmp("Rule", rule_data->Field) != 0 || scan_lenght < 10)
    {
        Free_Rule_Data(rule_data);
        return NULL;
    }

    return rule_data;
}

STATIC YEAR Parse_Rule_Data_From(CONST Rule_Details_t* rule_data)
{
    return atoi(rule_data->From);
}

STATIC YEAR Parse_Rule_Data_To(CONST Rule_Details_t* rule_data, YEAR max_value)
{
    if (strcmp(rule_data->To, "only") == 0)
    {
        return atoi(rule_data->From);
    }
    else if (strcmp(rule_data->To, "max") == 0)
    {
        return max_value;
    }
    else
    {
        return atoi(rule_data->To);
    }

    return INVALID_YEAR;
}

STATIC MONTH Parse_Rule_Data_In(CONST Rule_Details_t* rule_data)
{
    return Parse_Month(&rule_data->In);
}

STATIC Rule_Data_Day_t Parse_Rule_Data_On(CONST Rule_Details_t* rule_data)
{
    Rule_Data_Day_t day = { 0 };
    Parse_Day_Of_Month(&rule_data->On, &day.Day, &day.Weekday, &day.Weekday_isAfterOrEqual_Day);
    return day;
}

STATIC Rule_Data_Hour_t Parse_Rule_Data_At(CONST Rule_Details_t* rule_data)
{
    Rule_Data_Hour_t hour = { 0 };
    CHAR s;
    hour.Hour = Parse_Hour(&rule_data->At, &s);
    hour.Hour_isUTC = FALSE;
    if (s == 'u')
    {
        hour.Hour_isUTC = TRUE;
    }
    return hour;
}

STATIC HOUR Parse_Rule_Data_Save(CONST Rule_Details_t* rule_data)
{
    CHAR s;
    return Parse_Hour(&rule_data->Save, &s);
}

STATIC VOID Free_Rule_Data(Rule_Details_t* rule_data)
{
    if (rule_data != NULL)
    {
        CHAR* fields[] = {
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
            rule_data->Comment
        };

        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
            }
        }

        free(rule_data);
    }
}

STATIC VOID Free_Rule(Rule_Entry_t* rule)
{
    if (rule != NULL)
    {
        if (rule->Name != NULL)
        {
            free(rule->Name);
        }
        if (rule->Years != NULL && rule->Years_Count > 0)
        {
            for (COUNTER year_index = 0; year_index < rule->Years_Count; year_index++)
            {
                Rule_Data_t* year = &rule->Years[year_index];
                if (year->Reserved != NULL)
                {
                    free(year->Reserved);
                }
                if (year->Letter != NULL)
                {
                    free(year->Letter);
                }
                if (year->Comment != NULL)
                {
                    free(year->Comment);
                }
                year++;
            }
            free(rule->Years);
        }
        free(rule);
    }
}

VOID Free_Rules(Rule_Entry_t** rules, COUNTER* rules_count)
{
    if (rules != NULL && rules_count != NULL && *rules_count > 0)
    {
        for (COUNTER rule_index = 0; rule_index < *rules_count; rule_index++)
        {
            Free_Rule(&(*rules)[rule_index]);
        }
        free(*rules);
        *rules = NULL;
        *rules_count = 0;
    }
}
