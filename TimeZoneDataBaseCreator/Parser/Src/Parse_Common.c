#include "../Inc/Parse_Common.h"

HOUR Parse_Hour(CONST CHAR* hour, CHAR** suffix)
{
    CHAR* new_hour=(CHAR*)malloc((strlen(hour)+1)*sizeof(CHAR));
    if (new_hour == NULL)
    {
        return 0;
    }

    CHAR* suf = (CHAR*)realloc(*suffix, 2 * sizeof(CHAR));
    if (suf != NULL)
    {
        *suffix = suf;
        (*suffix)[0] = '\0';
        (*suffix)[1] = '\0';
    }

    HOUR std_offset = 0;
    CHAR sign = 0;
    HOUR h = 0, m = 0, s = 0;

    sprintf(new_hour, "%s", hour);

    if (new_hour[0] == '-')
    {
        sign = '-';
        strcpy(new_hour, new_hour + 1);
    }
    else if (new_hour[0] == '+')
    {
        sign = '+';
        strcpy(new_hour, new_hour + 1);
    }

    CHAR ch = new_hour[strlen(new_hour) - 1];

    if (isalpha(ch))
    {
        (*suffix)[0] = new_hour[strlen(new_hour) - 1];
        new_hour[strlen(new_hour) - 1] = '\0';
    }

    if (sscanf(new_hour, "%lld:%lld:%lld", &h, &m, &s) == 3)
    {
        std_offset = (h * 3600) + (m * 60) + s;
    }
    else if (sscanf(new_hour, "%lld:%lld", &h, &m) == 2)
    {
        std_offset = (h * 3600) + (m * 60);
    }
    else if (sscanf(new_hour, "%lld", &h) == 1)
    {
        std_offset = (h * 3600);
    }

    if (sign == '-')
    {
        std_offset *= -1;
    }

    return std_offset;
}

WEEKDAY Parse_Weekday(CONST CHAR* weekday)
{
    for (COUNTER weekday_index = 0; weekday_index < TZDB_WEEKDAY_TOTAL; weekday_index++)
    {
        if (strcmp(weekday, Weekday_Names[weekday_index].Abbr) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Full) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Last_Abbr) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Last_Full) == 0)
        {
            return Weekday_Names[weekday_index].Number;
        }
    }
    return (WEEKDAY)TZDB_WEEKDAY_NONE;
}

MONTH Parse_Month(CONST CHAR* month)
{
    for (COUNTER month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(month, Month_Names[month_index].Abbr) == 0 || strcmp(month, Month_Names[month_index].Full) == 0)
        {
            return (Month_Names[month_index].Number + 1);
        }
    }
    return 0;
}

VOID Parse_Day_Of_Month(CONST CHAR* on, DAY* day, WEEKDAY* weekday, BOOL* weekday_after)
{
    if (strlen(on) <= 3)
    {
        *day = atoi(on);
        *weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
        *weekday_after = FALSE;
    }
    else
    {
        CHAR dow[10], d[10]; 
        LENGHT sscanf_lenght = sscanf(on, "%[^=]=%s", dow, d);
        sprintf(dow, "%s", dow); 
        sprintf(d, "%s", d);

        if (sscanf_lenght == 1)
        {
            *day = 0; // Last week
            *weekday = Parse_Weekday(dow);
            *weekday_after = FALSE;
        }
        else
        {
            LENGHT len = (LENGHT)strlen(dow);

            if (dow[len - 1] == '>')
            {
                *weekday_after = TRUE;
            }
            else if (dow[len - 1] == '<')
            {
                *weekday_after = FALSE;
            }
            dow[len - 1] = '\0'; 

            *day = atoi(d);
            *weekday = Parse_Weekday(dow);
        }
    }
}
