#include "../Inc/Parse_Common.h"

HOUR* Parse_Hour(CONST CHAR** hour, CHAR* suffix)
{
    CHAR* new_hour=(CHAR*)calloc(1,(strlen(hour)+1)*sizeof(CHAR));
    if (new_hour == NULL)
    {
        return NULL;
    }

    HOUR* std_offset = (HOUR*)calloc(1,sizeof(HOUR));
    if (std_offset == NULL)
    {
        return NULL;
    }
    CHAR sign = 0;
    HOUR h = 0, m = 0, s = 0;

    sprintf(new_hour, "%s", *hour);

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

    *suffix = '\0';
    if (isalpha(ch))
    {
        *suffix = ch;
        new_hour[strlen(new_hour) - 1] = '\0';
    }

    if (sscanf(new_hour, "%lld:%lld:%lld", &h, &m, &s) == 3)
    {
        *std_offset = (h * 3600) + (m * 60) + s;
    }
    else if (sscanf(new_hour, "%lld:%lld", &h, &m) == 2)
    {
        *std_offset = (h * 3600) + (m * 60);
    }
    else if (sscanf(new_hour, "%lld", &h) == 1)
    {
        *std_offset = (h * 3600);
    }

    if (sign == '-')
    {
        *std_offset *= -1;
    }

    return std_offset;
}

WEEKDAY* Parse_Weekday(CONST CHAR** weekday)
{
    WEEKDAY* wd = (WEEKDAY*)malloc(sizeof(WEEKDAY*));
    if (wd == NULL)
    {
        return NULL;
    }
    *wd = (WEEKDAY)TZDB_WEEKDAY_NONE;
    for (COUNTER weekday_index = 0; weekday_index < TZDB_WEEKDAY_TOTAL; weekday_index++)
    {
        if (strcmp(*weekday, Weekday_Names[weekday_index].Abbr) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Full) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Last_Abbr) == 0 ||
            strcmp(*weekday, Weekday_Names[weekday_index].Last_Full) == 0)
        {
            *wd = Weekday_Names[weekday_index].Number;
        }
    }
    return wd;
}

MONTH* Parse_Month(CONST CHAR** month)
{
    MONTH* m = (MONTH*)malloc(sizeof(MONTH));
    if (m == NULL)
    {
        return NULL;
    }
    *m = 0;
    for (COUNTER month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(*month, Month_Names[month_index].Abbr) == 0 || strcmp(*month, Month_Names[month_index].Full) == 0)
        {
            *m = (Month_Names[month_index].Number + 1);
        }
    }
    return m;
}

VOID Parse_Day_Of_Month(CONST CHAR** on, DAY* day, WEEKDAY* weekday, BOOL* weekday_after)
{
    if (strlen(*on) <= 3)
    {
        *day = atoi(on);
        *weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
        *weekday_after = FALSE;
    }
    else
    {
        CHAR* dow = (CHAR*)malloc(10 * sizeof(CHAR));
        CHAR* d = (CHAR*)malloc(10 * sizeof(CHAR));
        if (dow == NULL || d == NULL)
        {
            *day = 0;
            *weekday = (WEEKDAY)TZDB_WEEKDAY_NONE;
            *weekday_after = FALSE;
            return;
        }
        LENGHT sscanf_lenght = sscanf(*on, "%[^=]=%s", dow, d);
        sprintf(dow, "%s", dow); 
        sprintf(d, "%s", d);

        if (sscanf_lenght == 1)
        {
            *day = 0; // Last week
            *weekday = Parse_Weekday(&dow);
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
            weekday = Parse_Weekday(&dow);
        }
    }
}
