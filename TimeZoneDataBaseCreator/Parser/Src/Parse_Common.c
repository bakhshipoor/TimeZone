#include "../Inc/Parse_Common.h"

int32_t Parse_Hour(const char* hour, char** suffix)
{
    char* new_hour=(char*)malloc((strlen(hour)+1)*sizeof(char));
    if (new_hour == NULL)
    {
        return 0;
    }

    char* suf = (char*)realloc(*suffix, 2 * sizeof(char));
    if (suf != NULL)
    {
        *suffix = suf;
        (*suffix)[0] = '\0';
        (*suffix)[1] = '\0';
    }

    int32_t std_offset = 0;
    uint8_t sign = 0;
    uint32_t h = 0, m = 0, s = 0;

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

    uint8_t ch = new_hour[strlen(new_hour) - 1];

    if (isalpha(ch))
    {
        (*suffix)[0] = new_hour[strlen(new_hour) - 1];
        new_hour[strlen(new_hour) - 1] = '\0';
    }

    if (sscanf(new_hour, "%d:%d:%d", &h, &m, &s) == 3)
    {
        std_offset = (h * 3600) + (m * 60) + s;
    }
    else if (sscanf(new_hour, "%d:%d", &h, &m) == 2)
    {
        std_offset = (h * 3600) + (m * 60);
    }
    else if (sscanf(new_hour, "%d", &h) == 1)
    {
        std_offset = (h * 3600);
    }

    if (sign == '-')
    {
        std_offset *= -1;
    }

    return std_offset;
}

uint8_t Parse_Weekday(const char* weekday)
{
    for (int weekday_index = 0; weekday_index < TZDB_WEEKDAY_TOTAL; weekday_index++)
    {
        if (strcmp(weekday, Weekday_Names[weekday_index].Abbr) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Full) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Last_Abbr) == 0 ||
            strcmp(weekday, Weekday_Names[weekday_index].Last_Full) == 0)
        {
            return Weekday_Names[weekday_index].Number;
        }
    }
    return (uint8_t)TZDB_WEEKDAY_NONE;
}

uint8_t Parse_Month(const char* month)
{
    for (int month_index = 0; month_index < TZDB_MONTH_TOTAL; month_index++)
    {
        if (strcmp(month, Month_Names[month_index].Abbr) == 0 || strcmp(month, Month_Names[month_index].Full) == 0)
        {
            return Month_Names[month_index].Number + 1;
        }
    }
    return 0;
}

void Parse_Day_Of_Month(const char* on, uint8_t* day, uint8_t* weekday, bool* weekday_after)
{
    if (strlen(on) <= 3)
    {
        *day = atoi(on);
        *weekday = (uint8_t)TZDB_WEEKDAY_NONE;
        *weekday_after = false;
    }
    else
    {
        uint8_t dow[10], d[10]; 
        int sscanf_lenght = sscanf(on, "%[^=]=%s", dow, d); 
        sprintf(dow, "%s", dow); 
        sprintf(d, "%s", d);

        if (sscanf_lenght == 1)
        {
            *day = 0; // Last week
            *weekday = Parse_Weekday(dow);
            *weekday_after = false;
        }
        else
        {
            int len = strlen(dow); 

            if (dow[len - 1] == '>')
            {
                *weekday_after = true;
            }
            else if (dow[len - 1] == '<')
            {
                *weekday_after = false;
            }
            dow[len - 1] = '\0'; 

            *day = atoi(d);
            *weekday = Parse_Weekday(dow);
        }
    }
}
