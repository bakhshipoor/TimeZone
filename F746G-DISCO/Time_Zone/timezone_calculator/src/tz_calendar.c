#include "../inc/tz_calendar.h"

uint8_t calculate_days_in_month(int32_t* year, int8_t* month)
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

uint32_t calculate_JDN(int32_t* year, int8_t* month, int8_t* day)
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

void convert_JD_to_Gregorian(double* JD, int* year, int* month, int* day, int* hour, int* minute, int* second) {
    double Z, F, A, alpha, B, C, D, E;
    double dayFraction, h, m, s;

    Z = floor(*JD + 0.5);
    F = (*JD + 0.5) - Z;

    if (Z < 2299161) {
        A = Z;
    }
    else {
        alpha = floor((Z - 1867216.25) / 36524.25);
        A = Z + 1 + alpha - floor(alpha / 4);
    }

    B = A + 1524;
    C = floor((B - 122.1) / 365.25);
    D = floor(365.25 * C);
    E = floor((B - D) / 30.6001);

    *day = (int)(B - D - floor(30.6001 * E));
    *month = (int)((E < 14) ? E - 1 : E - 13);
    *year = (int)((*month > 2) ? C - 4716 : C - 4715);

    dayFraction = F * 24.0;
    h = floor(dayFraction);
    m = floor((dayFraction - h) * 60.0);
    s = round(((dayFraction - h) * 60.0 - m) * 60.0);

    *hour = (int)h;
    *minute = (int)m;
    *second = (int)s;
}

uint8_t calculate_last_weekday_day_in_month(int32_t* year, int8_t* month, int8_t* weekday)
{
    uint8_t result_day = calculate_days_in_month(year, month);
    while (((calculate_JDN(year, month, &result_day) + 1) % 7) != (*weekday))
    {
        result_day--;
    }
    return result_day;
}

uint8_t calculate_first_weekday_after_day_in_month(int32_t* year, int8_t* month, int8_t* day, int8_t* weekday)
{
    uint32_t jdn = calculate_JDN(year, month, day);
    uint8_t current_weekday = ((jdn + 1) % 7);

    if (current_weekday == (*weekday))
    {
        return (*day);
    }

    uint8_t days_to_next_weekday = ((*weekday) - current_weekday + 7) % 7;
    uint8_t result_day = (*day) + days_to_next_weekday;

    if (result_day > calculate_days_in_month(year, month))
    {
        (*month)++;
        if (*month > 12) {
            *month = 1;
            (*year)++;
        }
        result_day -= calculate_days_in_month(year, month);
    }

    return result_day;
}

uint8_t calculate_first_weekday_before_day_in_month(int32_t* year, int8_t* month, int8_t* day, int8_t* weekday)
{
    uint32_t jdn = calculate_JDN(year, month, day);
    uint8_t current_weekday = ((jdn + 1) % 7);

    if (current_weekday == (*weekday))
    {
        return (*day);
    }

    uint8_t days_to_prev_weekday = (current_weekday - (*weekday) + 7) % 7;
    uint8_t result_day = (*day) - days_to_prev_weekday;

    if (result_day < 1)
    {
        (*month)--;
        if (*month < 1) {
            *month = 12;
            (*year)--;
        }
        result_day += calculate_days_in_month(year, month);
    }

    return result_day;
}

void subtract_or_add_seconds(int32_t* year, int8_t* month, int8_t* day, int64_t* second, const int64_t seconds_to_add)
{
    int64_t total_seconds = (*second) + seconds_to_add;

    while (total_seconds < 0)
    {
        total_seconds += 86400;
        (*day)--;
        if ((*day) < 1)
        {
            (*month)--;
            if ((*month) < 1)
            {
                (*month) = 12;
                (*year)--;
            }
            (*day) = calculate_days_in_month(year, month);
        }
    }

    while (total_seconds >= 86400)
    {
        total_seconds -= 86400;
        (*day)++;
        if ((*day) > calculate_days_in_month(year, month))
        {
            (*day) = 1;
            (*month)++;
            if ((*month) > 12)
            {
                (*month) = 1;
                (*year)++;
            }
        }
    }

    *second = total_seconds;
}