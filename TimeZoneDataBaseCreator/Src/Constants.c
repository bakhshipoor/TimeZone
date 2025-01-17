#include "../Inc/TimeZoneDBCreator.h"

const char Data_Folder_Name[] = "tzdata2024b";

const Weekday_Lookup_t Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "Sun", "Sunday",      (uint8_t)TZDB_WEEKDAY_SUNDAY },
    { "Mon", "Monday",      (uint8_t)TZDB_WEEKDAY_MONDAY },
    { "Tue", "Tuesday",     (uint8_t)TZDB_WEEKDAY_TUESDAY },
    { "Wed", "Wednesday",   (uint8_t)TZDB_WEEKDAY_WEDNESDAY },
    { "Thu", "Thursday",    (uint8_t)TZDB_WEEKDAY_THURSDAY },
    { "Fri", "Friday",      (uint8_t)TZDB_WEEKDAY_FRIDAY },
    { "Sat", "Saturday",    (uint8_t)TZDB_WEEKDAY_SATURDAY },
};

const Last_Weekday_Lookup_t Last_Weekday_Names[TZDB_WEEKDAY_TOTAL] = {
    { "lastSun", (uint8_t)TZDB_WEEKDAY_SUNDAY },
    { "lastMon", (uint8_t)TZDB_WEEKDAY_MONDAY },
    { "lastTue", (uint8_t)TZDB_WEEKDAY_TUESDAY },
    { "lastWed", (uint8_t)TZDB_WEEKDAY_WEDNESDAY },
    { "lastThu", (uint8_t)TZDB_WEEKDAY_THURSDAY },
    { "lastFri", (uint8_t)TZDB_WEEKDAY_FRIDAY },
    { "lastSat", (uint8_t)TZDB_WEEKDAY_SATURDAY },
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