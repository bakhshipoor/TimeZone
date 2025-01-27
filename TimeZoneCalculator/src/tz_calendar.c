#include "../inc/tz_calendar.h"

uint8_t calculate_days_in_month(int32_t* year, uint8_t* month)
{
    static const uint8_t days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    /*if (*month < 1 || *month > 12)
    {
        return 0;
    }*/

    uint8_t days = days_in_month[*month - 1];
    if (*month == 2 && ((*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0)))
    {
        days = 29;
    }

    return days;
}

uint32_t calculate_JDN(int32_t* year, uint8_t* month, uint8_t* day)
{
    uint32_t a = (14 - (*month)) / 12;
    uint32_t y = (*year) + 4800 - a;
    uint32_t m = (*month) + 12 * a - 3;

    return (*day + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045);
}

double calculate_JD(uint32_t* jdn, int64_t* second)
{
    double jd = (((double)(*jdn) * 1.0) + (-12.0 / 24.0) + (((double)(*second) * 1.0) / 86400.0));
    return jd;
}