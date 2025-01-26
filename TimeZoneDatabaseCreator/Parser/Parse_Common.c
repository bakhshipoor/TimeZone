#include "Parse_Common.h"

HOUR Parse_Hour(CONST CHAR** hour, CHAR* suffix)
{
    if (hour == NULL || *hour == NULL || suffix == NULL)
    {
        return INVALID_HOUR;
    }

    CHAR* input = *hour;
    HOUR h = 0, m = 0, s = 0;
    HOUR parsed_hour = INVALID_HOUR;
    CHAR sign = 0;

    if (input[0] == '-')
    {
        sign = '-';
        input++;
    }
    else if (input[0] == '+')
    {
        sign = '+';
        input++;
    }

    size_t length = utf8_strlen(input);
    *suffix = '\0';
    if (isalpha(input[length - 1]))
    {
        *suffix = input[length - 1];
        input[length - 1] = '\0';
    }

    if (sscanf(input, "%lld:%lld:%lld", &h, &m, &s) == 3)
    {
        parsed_hour = (h * 3600) + (m * 60) + s;
    }
    else if (sscanf(input, "%lld:%lld", &h, &m) == 2)
    {
        parsed_hour = (h * 3600) + (m * 60);
    }
    else if (sscanf(input, "%lld", &h) == 1)
    {
        parsed_hour = (h * 3600);
    }

    if (sign == '-')
    {
        parsed_hour *= -1;
    }

    return parsed_hour;
}

WEEKDAY Parse_Weekday(CONST CHAR** weekday)
{
    if (weekday == NULL || *weekday == NULL)
    {
        return (WEEKDAY)TZDB_WEEKDAY_NONE;
    }

    for (COUNTER weekday_index = 0; weekday_index < TZDB_WEEKDAY_TOTAL; weekday_index++)
    {
        if (strcmp(*weekday, Weekday_Names[weekday_index].Abbr) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Full) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Last_Abbr) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Last_Full) == 0)
        {
            return Weekday_Names[weekday_index].Number;
        }
    }
    return (WEEKDAY)TZDB_WEEKDAY_NONE;
}

MONTH Parse_Month(CONST CHAR** month)
{
    if (month == NULL || *month == NULL)
    {
        return INVALID_MONTH;
    }

    for (COUNTER month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(*month, Month_Names[month_index].Abbr) == 0 || 
            strcmp(*month, Month_Names[month_index].Full) == 0)
        {
            return Month_Names[month_index].Number;
        }
    }
    return INVALID_MONTH;
}

VOID Parse_Day_Of_Month(CONST CHAR** on, DAY* day, WEEKDAY* weekday, BOOL* weekday_after)
{
    if (on == NULL || *on == NULL || day == NULL || weekday == NULL || weekday_after == NULL)
    {
        return;
    }

    *day = (DAY)TZDB_DAY_NONE;
    *weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
    *weekday_after = FALSE;

    if (utf8_strlen(*on) <= 3)
    {
        *day = atoi(*on);
    }
    else
    {
        CHAR* dow = (CHAR*)calloc(10, sizeof(CHAR));
        CHAR* d = (CHAR*)calloc(10, sizeof(CHAR));
        if (dow != NULL && d != NULL)
        {
            if (sscanf(*on, "%[^=]=%s", dow, d) == 1)
            {
                *day = 0; // Last week
                *weekday = Parse_Weekday(&dow);
                *weekday_after = FALSE;
            }
            else
            {
                size_t len = utf8_strlen(dow);

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
                *weekday = Parse_Weekday(&dow);
            }
        }
    }
}

LENGHT utf8_strlen(CONST CHAR* s) 
{
    LENGHT length = 0;
    while (*s) 
    {
        if ((*s & 0xC0) != 0x80) 
        {
            length++;
        }
        s++;
    }
    return length;
}
