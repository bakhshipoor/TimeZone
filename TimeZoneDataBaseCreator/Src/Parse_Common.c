#include "../Inc/TimeZoneDBCreator.h"

int32_t Parse_Hour(const char* hour)
{
    int32_t std_offset = 0;
    uint8_t sign = 0;
    uint32_t h = 0, m = 0, s = 0;

    if (hour[0] == '-')
    {
        sign = '-';
        strcpy(hour, hour + 1);
    }
    else if (hour[0] == '+')
    {
        sign = '+';
        strcpy(hour, hour + 1);
    }

    if (sscanf(hour, "%d:%d:%d", &h, &m, &s) == 3)
    {
        std_offset = (h * 3600) + (m * 60) + s;
    }
    else if (sscanf(hour, "%d:%d", &h, &m) == 2)
    {
        std_offset = (h * 3600) + (m * 60);
    }
    else if (sscanf(hour, "%d", &h) == 1)
    {
        std_offset = (h * 3600);
    }

    if (sign == '-')
    {
        std_offset *= -1;
    }

    return std_offset;
}