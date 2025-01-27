#include "../inc/tz_calculator.h"

static tz_inititial_t* tz_inititial;
static tz_calculated_data_t  tz_calculated_data = { 0 };

static int32_t* get_zone_id_identifier(uint8_t** tz_identifier);
static int32_t* get_zone_data_count_identifier(uint8_t** tz_identifier);
static bool validate_data(void);
static int32_t find_zone_data_index_jd(int32_t* tz_id, double* jd);

static void convert_second_to_time(int64_t* seconds, tz_time_t* time);
static void free_tz_initial(tz_inititial_t* tz_inititial);

bool tz_init(void)
{
    tz_inititial = (tz_inititial_t*)calloc(1, sizeof(tz_inititial_t));
    if (tz_inititial == NULL)
    {
        return false;
    }

    tz_inititial->tz_id = (int32_t*)calloc(1, sizeof(int32_t));
    tz_inititial->tz_identifier = (uint8_t*)calloc(TZDB_MAX_LENGHT_IDENTIFIER, sizeof(uint8_t));

    tz_inititial->g_year = (int32_t*)calloc(1, sizeof(int32_t));
    tz_inititial->g_month = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->g_day = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_hour = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_minute = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_second = (uint8_t*)calloc(1, sizeof(uint8_t));

    if (tz_inititial->tz_id == NULL ||
        tz_inititial->tz_identifier == NULL ||
        tz_inititial->g_year == NULL ||
        tz_inititial->g_month == NULL ||
        tz_inititial->g_day == NULL ||
        tz_inititial->utc_hour == NULL ||
        tz_inititial->utc_minute == NULL ||
        tz_inititial->utc_second == NULL)
    {
        free_tz_initial(tz_inititial);
        return false;
    }


    tz_calculated_data.offsets = (tz_get_offset_t*)calloc(1, sizeof(tz_get_offset_t));
    if (tz_calculated_data.offsets == NULL)
    {
        free_tz_initial(tz_inititial);
        return false;
    }

    tz_inititial->tz_is_init = true;
    return true;
}

bool tz_set_zone(const uint8_t** tz_identifier)
{
    if (tz_identifier == NULL || *tz_identifier == NULL)
    {
        return false;
    }

    tz_inititial->tz_id = get_zone_id_identifier(tz_identifier);
    if (tz_inititial->tz_id == NULL)
    {
        return false;
    }
    tz_inititial->tz_identifier = tz_identifier;

    tz_inititial->zone_is_init = true;
    return true;
}

bool tz_set_date(int32_t* g_year, uint8_t* g_month, uint8_t* g_day)
{
    if (*g_year < 0)
    {
        return false;
    }
    else if (*g_month < 1 || *g_month > 12)
    {
        return false;
    }
    else if (*g_day < 1 || *g_day > calculate_days_in_month(g_year, g_month))
    {
        return false;
    }

    tz_inititial->g_year = g_year;
    tz_inititial->g_month = g_month;
    tz_inititial->g_day = g_day;

    tz_inititial->date_is_init = true;
    return true;
}

bool tz_set_time(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second)
{
    if (*utc_hour > 23 || *utc_hour < 0)
    {
        return false;
    }
    else if (*utc_minute > 59 || *utc_minute < 0)
    {
        return false;
    }
    else if (*utc_second > 59 || *utc_second < 0)
    {
        return false;
    }

    tz_inititial->utc_hour = utc_hour;
    tz_inititial->utc_minute = utc_minute;
    tz_inititial->utc_second = utc_second;

    tz_inititial->time_is_init = true;
    return true;
}


tz_time_t* tz_get_local_time(void)
{

}

tz_get_offset_t* tz_get_offset(void)
{
    if (!validate_data())
    {
        return NULL;
    }
    tz_calculated_data.now_jdn = 0;
    tz_calculated_data.now_utc_jd = 0;
    tz_calculated_data.now_std_jd = 0;
    tz_calculated_data.offsets->std_offset_seconds = 0;
    tz_calculated_data.offsets->dst_effect = false;
    tz_calculated_data.offsets->dst_offset_seconds = 0;
    tz_calculated_data.offsets->total_offset_seconds = 0;

    tz_calculated_data.now_seconds = ((*tz_inititial->utc_hour) * 3600) + ((*tz_inititial->utc_minute) * 60) + (*tz_inititial->utc_second);

    tz_calculated_data.now_jdn = calculate_JDN(tz_inititial->g_year, tz_inititial->g_month, tz_inititial->g_day);
    tz_calculated_data.now_utc_jd = calculate_JD(&tz_calculated_data.now_jdn, &tz_calculated_data.now_seconds);

    int32_t year_data_index = find_zone_data_index_jd(tz_inititial->tz_id, &tz_calculated_data.now_utc_jd);
    if (year_data_index > -1)
    {
        tz_calculated_data.offsets->std_offset_seconds = tzdb_zones_data[year_data_index].standard_offset + tzdb_zones_data[year_data_index].save_hour;
        tz_calculated_data.now_std_jd = calculate_JD(&tz_calculated_data.now_jdn, &tz_calculated_data.offsets->std_offset_seconds);
        convert_second_to_time(&tz_calculated_data.offsets->std_offset_seconds, &tz_calculated_data.offsets->std_offset);

        tz_calculated_data.offsets->total_offset_seconds = tz_calculated_data.offsets->std_offset_seconds;
        convert_second_to_time(&tz_calculated_data.offsets->total_offset_seconds, &tz_calculated_data.offsets->total_offset);

        if (tzdb_zones_data[year_data_index].rule_id != 0)
        {

        }
    }

    return tz_calculated_data.offsets;
}


static int32_t* get_zone_id_identifier(uint8_t** tz_identifier)
{
    for (int32_t tz_index = 0; tz_index < TZDB_ZONES_INFO_COUNT; tz_index++)
    {
        if (strcmp(tzdb_zones_info[tz_index].zone_identifier, *tz_identifier) == 0)
        {
            return tzdb_zones_info[tz_index].linked_zone_id != 0
                ? &tzdb_zones_info[tz_index].linked_zone_id
                : &tzdb_zones_info[tz_index].zone_id;
        }
    }
    return NULL;
}

static int32_t* get_zone_data_count_identifier(uint8_t** tz_identifier)
{
    for (int32_t tz_index = 0; tz_index < TZDB_ZONES_INFO_COUNT; tz_index++)
    {
        if (strcmp(tzdb_zones_info[tz_index].zone_identifier, *tz_identifier) == 0)
        {
            return &tzdb_zones_info[tz_index].data_count;
        }
    }
    return NULL;
}



static bool validate_data(void)
{
    if (tz_inititial == NULL)
    {
        return false;
    }

    if (!tz_inititial->tz_is_init)
    {
        return false;
    }

    if (tz_inititial->tz_identifier == NULL || (*tz_inititial->tz_identifier) == NULL)
    {
        return false;
    }
    
    if (!tz_inititial->zone_is_init)
    {
        return false;
    }

    if (!tz_inititial->date_is_init)
    {
        return false;
    }

    if (tz_inititial->g_year == NULL || *tz_inititial->g_year < 1)
    {
        return false;
    }

    if (tz_inititial->g_month == NULL || *tz_inititial->g_month < 1 || *tz_inititial->g_month > 12)
    {
        return false;
    }

    if (tz_inititial->g_day == NULL || *tz_inititial->g_day < 1 || *tz_inititial->g_day > calculate_days_in_month(tz_inititial->g_year, tz_inititial->g_month))
    {
        return false;
    }

    if (!tz_inititial->time_is_init)
    {
        return false;
    }

    if (tz_inititial->utc_hour == NULL || *tz_inititial->utc_hour < 0 || *tz_inititial->utc_hour > 23)
    {
        return false;
    }

    if (tz_inititial->utc_minute == NULL || *tz_inititial->utc_minute < 0 || *tz_inititial->utc_minute > 59)
    {
        return false;
    }

    if (tz_inititial->utc_second == NULL || *tz_inititial->utc_second < 0 || *tz_inititial->utc_second > 59)
    {
        return false;
    }
    return true;
}

static int32_t find_zone_data_index_jd(int32_t* tz_id, double* jd)
{
    int32_t max_year_index = -1;
    for (int32_t zd_index = 0; zd_index < TZDB_ZONES_DATA_COUNT; zd_index++)
    {
        if (tzdb_zones_data[zd_index].zone_id == *tz_id)
        {
            if (tzdb_zones_data[zd_index].until_jd == -1.0)
            {
                max_year_index = zd_index;
                continue;
            }
            if (*jd < tzdb_zones_data[zd_index].until_jd)
            {
                return zd_index;
            }
        }
    }
    return max_year_index;
}


static void convert_second_to_time(int64_t* seconds, tz_time_t* time)
{
    if ((*seconds) < 0)
    {
        time->hour = (*seconds) / 3600;
        time->minute = -((*seconds) % 3600) / 60;
        time->second = -(*seconds) % 60;
    }
    else
    {
        time->hour = (*seconds) / 3600;
        time->minute = ((*seconds) % 3600) / 60;
        time->second = (*seconds) % 60;
    }
}

static void free_tz_initial(tz_inititial_t* tz_inititial)
{
    if (tz_inititial != NULL)
    {
        void* fields[] = {
            tz_inititial->tz_id,
            tz_inititial->tz_identifier,
            tz_inititial->g_year,
            tz_inititial->g_month,
            tz_inititial->g_day,
            tz_inititial->utc_hour,
            tz_inititial->utc_minute,
            tz_inititial->utc_second
        };

        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
                fields[i] = NULL;
            }
        }

        free(tz_inititial);
        tz_inititial = NULL;
    }
}