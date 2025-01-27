#if 1

#include "time_zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void clear_screen(void)
{
    printf("\033[2J");
    printf("\033[H");
}

int main()
{
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

    int32_t year = 0;
    int32_t month = 0;
    int32_t day = 0;

    int32_t hour = 0;
    int32_t minute = 0;
    int32_t second = 0;

    while (1)
    {
        printf("\nPlease enter zone identifier: ");
        len = scanf_s("%s", buf, TZDB_MAX_LENGHT_IDENTIFIER);
        if (len > 0)
        {
            if (!tz_set_zone(&buf))
            {
                return -1;
            }
        }

        printf("\nPlease enter date [ yyyy/mm/dd ]: ");
        len = scanf_s("%4d/%2d/%2d", &year, &month, &day);
        if (len != 3)
        {
            clear_screen();
            continue;
        }

        if (!tz_set_date(&year, &month, &day))
        {
            clear_screen();
            continue;
        }

        printf("\nPlease enter time [ hh:mm:ss ]: ");
        len = scanf_s("%2d:%2d:%2d", &hour, &minute, &second);
        if (len < 2 || len > 3)
        {
            clear_screen();
            continue;
        }

        if (!tz_set_time(&hour, &minute, &second))
        {
            clear_screen();
            continue;
        }

        printf("\n\n\n");
        system("pause");
        clear_screen();
    }
    return 0;
}

#endif
