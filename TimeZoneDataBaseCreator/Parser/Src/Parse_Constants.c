#include "../Inc/Parse_Common.h"

CONST Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "Sun", "Sunday",      "lastSun", "lastSunday",        (WEEKDAY)TZDB_WEEKDAY_SUNDAY },
    { "Mon", "Monday",      "lastMon", "lastMonday",        (WEEKDAY)TZDB_WEEKDAY_MONDAY },
    { "Tue", "Tuesday",     "lastTue", "lastTuesday",       (WEEKDAY)TZDB_WEEKDAY_TUESDAY },
    { "Wed", "Wednesday",   "lastWed", "lastWednesday",     (WEEKDAY)TZDB_WEEKDAY_WEDNESDAY },
    { "Thu", "Thursday",    "lastThu", "lastThursday",      (WEEKDAY)TZDB_WEEKDAY_THURSDAY },
    { "Fri", "Friday",      "lastFri", "lastFriday",        (WEEKDAY)TZDB_WEEKDAY_FRIDAY },
    { "Sat", "Saturday",    "lastSat", "lastSaturday" ,     (WEEKDAY)TZDB_WEEKDAY_SATURDAY },
};

CONST Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL] = {
    { "Jan", "January",     (MONTH)TZDB_MONTH_JANUARY },
    { "Feb", "February",    (MONTH)TZDB_MONTH_FEBRUARY },
    { "Mar", "March",       (MONTH)TZDB_MONTH_MARCH },
    { "Apr", "April",       (MONTH)TZDB_MONTH_APRIL },
    { "May", "May",         (MONTH)TZDB_MONTH_MAY },
    { "Jun", "June",        (MONTH)TZDB_MONTH_JUNE },
    { "Jul", "July",        (MONTH)TZDB_MONTH_JULY },
    { "Aug", "August",      (MONTH)TZDB_MONTH_AUGUST },
    { "Sep", "September",   (MONTH)TZDB_MONTH_SEPTEMBER },
    { "Oct", "October",     (MONTH)TZDB_MONTH_OCTOBER },
    { "Nov", "November",    (MONTH)TZDB_MONTH_NOVEMBER },
    { "Dec", "December",    (MONTH)TZDB_MONTH_DECEMBER },
};

CONST CHAR* Data_Files_Name[DATA_FILES_COUNT] = {
    "version",
    "iso3166.tab",
    "zone.tab",
    "africa",
    "antarctica",
    "asia",
    "australasia",
    "backward",
    "etcetera",
    "europe",
    "northamerica",
    "southamerica"
};