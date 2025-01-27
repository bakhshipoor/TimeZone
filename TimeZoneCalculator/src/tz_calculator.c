#include "../inc/tz_calculator.h"

tz_inititial_t* tz_inititial;
tz_zone_data_t* tz_zone_data;
tz_rule_data_t* tz_rule_data;

bool tz_init(void)
{
    tz_inititial = (tz_inititial_t*)calloc(1, sizeof(tz_inititial_t));
    if (tz_inititial == NULL)
    {
        return false;
    }

    tz_inititial->tz_id = (int32_t*)calloc(1, sizeof(int32_t));
    tz_inititial->tz_identifier = (uint8_t*)calloc(TZDB_MAX_LENGHT_IDENTIFIER, sizeof(uint8_t));
    tz_inititial->zone_data_count = (int32_t*)calloc(1, sizeof(int32_t));
    tz_inititial->zone_rules_count = 0;
    tz_inititial->zone_rules_id = NULL;

    tz_inititial->g_year = (int32_t*)calloc(1, sizeof(int32_t));
    tz_inititial->g_month = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->g_day = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_hour = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_minute = (uint8_t*)calloc(1, sizeof(uint8_t));
    tz_inititial->utc_second = (uint8_t*)calloc(1, sizeof(uint8_t));

    if (tz_inititial->tz_id == NULL ||
        tz_inititial->tz_identifier == NULL ||
        tz_inititial->zone_data_count == NULL ||
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

    tz_zone_data = (tz_zone_data_t*)calloc(1, sizeof(tz_zone_data_t));
    if (tz_zone_data == NULL)
    {
        free_tz_initial(tz_inititial);
        return false;
    }

    tz_rule_data = (tz_rule_data_t*)calloc(1, sizeof(tz_rule_data_t));
    if (tz_rule_data == NULL)
    {
        free_tz_initial(tz_inititial);
        free(tz_zone_data);
        return false;
    }

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
    tz_inititial->zone_data_count = get_zone_data_count_identifier(tz_identifier);

    tz_zone_data = get_zone_data(tz_inititial->zone_data_count, tz_inititial->tz_id);
    if (tz_zone_data == NULL)
    {
        return false;
    }

    if (tz_inititial->zone_rules_count > 0)
    {
        tz_rule_data = get_rule_data(tz_inititial);
        if (tz_zone_data == NULL)
        {
            return false;
        }
    }

    return true;
}

bool tz_set_date(int32_t* g_year, uint8_t* g_month, uint8_t* g_day)
{
    if (*g_year < 1000)
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

    return true;
}

bool tz_set_time(uint8_t* utc_hour, uint8_t* utc_minute, uint8_t* utc_second)
{
    if (*utc_hour > 24 || *utc_hour < 0)
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

    return true;
}

tz_get_time_t* tz_get_time(void)
{

}

tz_get_offset_t* tz_get_offset(void)
{

}






int32_t* get_zone_id_identifier(uint8_t** tz_identifier)
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

int32_t* get_zone_data_count_identifier(uint8_t** tz_identifier)
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

tz_zone_data_t* get_zone_data(int32_t* zone_data_count, int32_t* tz_id)
{
    if (zone_data_count == NULL || *zone_data_count == 0)
    {
        return NULL;
    }

    if (tz_id == NULL || *tz_id == 0)
    {
        return NULL;
    }

    tz_zone_data_t* zone_data = (tz_zone_data_t*)calloc(1, sizeof(tz_zone_data_t));
    if (zone_data == NULL)
    {
        return NULL;
    }

    zone_data->zone_data_count = (int32_t*)calloc(1, sizeof(int32_t));
    zone_data->zone_data = (tzdb_zone_data_t**)calloc(1, sizeof(tzdb_zone_data_t*));

    if (zone_data->zone_data_count == NULL || zone_data->zone_data == NULL)
    {
        free_tz_zone_data(zone_data);
        return NULL;
    }

    for (int32_t data_index = 0; data_index < TZDB_ZONES_DATA_COUNT; data_index++)
    {
        if (tzdb_zones_data[data_index].zone_id == *tz_id)
        {
            tzdb_zone_data_t** temp = (tzdb_zone_data_t**)realloc(zone_data->zone_data, (*(zone_data->zone_data_count) + 1) * sizeof(tzdb_zone_data_t*));
            if (temp == NULL)
            {
                free_tz_zone_data(zone_data);
                return NULL;
            }
            zone_data->zone_data = temp;
            zone_data->zone_data[*(zone_data->zone_data_count)] = &tzdb_zones_data[data_index];

            if (tzdb_zones_data[data_index].rule_id != 0)
            {
                bool find = false;
                if (tz_inititial->zone_rules_id != NULL)
                {
                    for (int32_t zri_index = 0; zri_index < tz_inititial->zone_rules_count; zri_index++)
                    {
                        if (tz_inititial->zone_rules_id[zri_index] == tzdb_zones_data[data_index].rule_id)
                        {
                            find = true;
                        }
                    }
                }
                if (find == false)
                {
                    int32_t* rc;
                    if (tz_inititial->zone_rules_count == 0)
                    {
                        rc = (int32_t*)calloc(1, sizeof(int32_t));
                    }
                    else
                    {
                        rc = (int32_t*)realloc(tz_inititial->zone_rules_id, (tz_inititial->zone_rules_count + 1) * sizeof(int32_t));
                    }
                    if (rc != NULL)
                    {
                        tz_inititial->zone_rules_id = rc;
                        tz_inititial->zone_rules_id[tz_inititial->zone_rules_count] = tzdb_zones_data[data_index].rule_id;
                        tz_inititial->zone_rules_count++;
                    }
                }
            }

            (*(zone_data->zone_data_count))++;
        }
    }

    if (*(zone_data->zone_data_count) != *zone_data_count)
    {
        free_tz_zone_data(zone_data);
        return NULL;
    }

    zone_data->zone_data_count = zone_data_count;
    return zone_data;
}

tz_rule_data_t* get_rule_data(tz_inititial_t* tz_init)
{
    if (tz_init == NULL)
    {
        return NULL;
    }

    tz_rule_data_t* rule_data = (tz_rule_data_t*)calloc(1, sizeof(tz_rule_data_t));
    if (rule_data == NULL)
    {
        return NULL;
    }

    rule_data->rules_data_count = 0;
    rule_data->rule_data = (tzdb_zone_data_t**)calloc(1, sizeof(tzdb_zone_data_t*));

    if (rule_data->rule_data == NULL)
    {
        free_tz_rule_data(rule_data);
        return NULL;
    }

    for (int32_t tz_init_rd_index = 0; tz_init_rd_index < tz_init->zone_rules_count; tz_init_rd_index++)
    {
        for (int32_t data_index = 0; data_index < TZDB_RULES_DATA_COUNT; data_index++)
        {
            if (tzdb_rules_data[data_index].rule_id == tz_init->zone_rules_id[tz_init_rd_index])
            {
                tzdb_rule_data_t** temp = (tzdb_rule_data_t**)realloc(rule_data->rule_data, (rule_data->rules_data_count + 1) * sizeof(tzdb_rule_data_t*));
                if (temp == NULL)
                {
                    free_tz_rule_data(rule_data);
                    return NULL;
                }
                rule_data->rule_data = temp;
                rule_data->rule_data[rule_data->rules_data_count] = &tzdb_rules_data[data_index];
                rule_data->rules_data_count++;
            }
        }
    }
    return rule_data;
}



void free_tz_initial(tz_inititial_t* tz_inititial)
{
    if (tz_inititial != NULL)
    {
        void* fields[] = {
            tz_inititial->tz_id,
            tz_inititial->tz_identifier,
            tz_inititial->zone_data_count,
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

void free_tz_zone_data(tz_zone_data_t* zone_data)
{
    if (zone_data != NULL)
    {
        if (zone_data->zone_data_count != NULL)
        {
            free(zone_data->zone_data_count);
            zone_data->zone_data_count = NULL;
            
        }
        if (zone_data->zone_data != NULL)
        {
            free(zone_data->zone_data);
            zone_data->zone_data = NULL;
        }

        free(zone_data);
        zone_data = NULL;
    }
}

void free_tz_rule_data(tz_rule_data_t* rule_data)
{
    if (rule_data != NULL)
    {
        if (rule_data->rule_data != NULL)
        {
            free(rule_data->rule_data);
            rule_data->rule_data = NULL;
        }

        free(rule_data);
        rule_data = NULL;
    }
}