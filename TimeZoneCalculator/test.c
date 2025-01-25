#if 1
#include <stdio.h>
// تابع برای تبدیل تاریخ میلادی به JDN
double gregorian_to_jdn(int year, int month, int day, int hour, int minute, int second) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;

    // محاسبه JDN بدون زمان
    int JDN = day + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045;

    // محاسبه زمان در روز ژولیانی
    double JD = JDN + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;

    return JD;
}


int main()
{
   
    int year = 1999;
    int month = 12;
    int day = 31;
    int hour = 0;
    int minute = 0;
    int second = 86400;

    double JD = gregorian_to_jdn(year, month, day, hour, minute, second);
    printf("Julian Date: %.6f\n", JD);
    return 0;
}

#endif
