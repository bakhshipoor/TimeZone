#ifdef _WIN32

#include "time_zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

int main()
{
    SYSTEMTIME st;
    tz_time_t* local_time;
    tz_get_offset_t* offset;

    int32_t year = 0;
    int32_t month = 0;
    int32_t day = 0;

    int32_t hour = 0;
    int32_t minute = 0;
    int32_t second = 0;

    if (!tz_init())
    {
        return -1;
    }

    uint8_t* buf = (uint8_t*)calloc(TZDB_MAX_LENGHT_IDENTIFIER, sizeof(uint8_t));
    if (buf == NULL)
    {
        return -1;
    }
    int len = 0;

    sprintf_s(buf, TZDB_MAX_LENGHT_IDENTIFIER, "%s", "America/New_York");

    if (!tz_set_zone(&buf))
    {
        return -1;
    }

    while (1)
    {

        GetSystemTime(&st);

        year = st.wYear;
        month = st.wMonth;
        day = st.wDay;

        hour = st.wHour;
        minute = st.wMinute;
        second = st.wSecond;

        if (!tz_set_date(&year, &month, &day))
        {
            system("cls");
            continue;
        }

        if (!tz_set_time(&hour, &minute, &second))
        {
            system("cls");
            continue;
        }

        tz_calculate();

        printf("\n\n\n");

        local_time = tz_get_local_time();
        if (local_time != NULL)
        {
            printf("Time Zone:\t\t%s\n\n", buf);
            printf("UTC Time:\t\t%02d:%02d:%02d\n", hour, minute, second);
            printf("Local Time:\t\t%02d:%02d:%02d\n\n", local_time->hour, local_time->minute, local_time->second);
        }

        offset = tz_get_offset();
        if (offset != NULL)
        {
            printf("Standard Offset:\t%02d:%02d:%02d\n", offset->std_offset.hour, offset->std_offset.minute, offset->std_offset.second);
            printf("DST Effect:\t\t%s\n", offset->dst_effect == true ? "True" : "False");
            printf("DST Offset:\t\t%02d:%02d:%02d\n", offset->dst_offset.hour, offset->dst_offset.minute, offset->dst_offset.second);
            printf("Total Offset:\t\t%02d:%02d:%02d\n", offset->total_offset.hour, offset->total_offset.minute, offset->total_offset.second);
        }

        printf("\n\n\n");
        Sleep(1000);
        system("cls");
    }
    return 0;
}

#endif