#include "../Inc/Parse_Common.h"

const char Data_Folder_Path[] = "../tzdata2025a";

const Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "Sun", "Sunday",      "lastSun", "lastSunday",        (uint8_t)TZDB_WEEKDAY_SUNDAY },
    { "Mon", "Monday",      "lastMon", "lastMonday",        (uint8_t)TZDB_WEEKDAY_MONDAY },
    { "Tue", "Tuesday",     "lastTue", "lastTuesday",       (uint8_t)TZDB_WEEKDAY_TUESDAY },
    { "Wed", "Wednesday",   "lastWed", "lastWednesday",     (uint8_t)TZDB_WEEKDAY_WEDNESDAY },
    { "Thu", "Thursday",    "lastThu", "lastThursday",      (uint8_t)TZDB_WEEKDAY_THURSDAY },
    { "Fri", "Friday",      "lastFri", "lastFriday",        (uint8_t)TZDB_WEEKDAY_FRIDAY },
    { "Sat", "Saturday",    "lastSat", "lastSaturday" ,     (uint8_t)TZDB_WEEKDAY_SATURDAY },
};

const Month_Lookup_t Month_Names[TZDB_MONTH_TOTAL] = {
    { "Jan", "January",     (uint8_t)TZDB_MONTH_JANUARY },
    { "Feb", "February",    (uint8_t)TZDB_MONTH_FEBRUARY },
    { "Mar", "March",       (uint8_t)TZDB_MONTH_MARCH },
    { "Apr", "April",       (uint8_t)TZDB_MONTH_APRIL },
    { "May", "May",         (uint8_t)TZDB_MONTH_MAY },
    { "Jun", "June",        (uint8_t)TZDB_MONTH_JUNE },
    { "Jul", "July",        (uint8_t)TZDB_MONTH_JULY },
    { "Aug", "August",      (uint8_t)TZDB_MONTH_AUGUST },
    { "Sep", "September",   (uint8_t)TZDB_MONTH_SEPTEMBER },
    { "Oct", "October",     (uint8_t)TZDB_MONTH_OCTOBER },
    { "Nov", "November",    (uint8_t)TZDB_MONTH_NOVEMBER },
    { "Dec", "December",    (uint8_t)TZDB_MONTH_DECEMBER },
};

const char* Data_Files_Name[DATA_FILES_COUNT] = {
    "version",
    "iso3166.tab",
    "zone.tab",
    "africa",
    "antarctica",
    "asia",
    "australasia",
    "backward",
    "backzone",
    "etcetera",
    "europe",
    "northamerica",
    "southamerica"
};