
#include "../Inc/Generator.h"


bool Generate_Data(const char* data_folder_path)
{
    if (!Generate_Full_List(data_folder_path))
    {
        return false;
    }
    return true;
}

bool Generate_Full_List(const char* data_folder_path)
{
    G_Time_Zones_t* time_zones = (G_Time_Zones_t*)malloc(sizeof(G_Time_Zones_t));
    if (time_zones == NULL)
    {
        return false;
    }

     Parse_Data_t* parse_data = Parse_Data(data_folder_path);
    if (parse_data == NULL)
    {
        return false;
    }

    FILE* temp = fopen("../temp.txt", "w");
    
    int32_t zones_counts = 0;
    zones_counts = parse_data->Zonetab_Count;
    time_zones->Zones_Count = zones_counts;

    int32_t zone_index = 0;
    for (zone_index = 0; zone_index < zones_counts; zone_index++)
    {
        if (zone_index == 0)
        {
            G_Zone_Info_t* z_info = (G_Zone_Info_t*)malloc((zone_index + 1) * sizeof(G_Zone_Info_t));
            if (z_info == NULL)
            {
                return false;
            }
            time_zones->Zone = z_info;
        }
        else
        {

            G_Zone_Info_t* z_info = (G_Zone_Info_t*)realloc(time_zones->Zone, (zone_index + 1) * sizeof(G_Zone_Info_t));
            if (z_info == NULL)
            {
                return false;
            }
            time_zones->Zone = z_info;
        }

        time_zones->Zone[zone_index].Country_Code = (uint8_t*)malloc((strlen(parse_data->Zonetab[zone_index].Country_Code) + 1) * sizeof(uint8_t));
        if (time_zones->Zone[zone_index].Country_Code != NULL)
        {
            sprintf(time_zones->Zone[zone_index].Country_Code, "%s", parse_data->Zonetab[zone_index].Country_Code);
        }

        int32_t country_index = Get_Country_Name(parse_data, parse_data->Zonetab[zone_index].Country_Code);
        if (country_index != -1)
        {
            time_zones->Zone[zone_index].Country_Name = (uint8_t*)malloc((strlen(parse_data->ISO3166[country_index].Country_Name) + 1) * sizeof(uint8_t));
            if (time_zones->Zone[zone_index].Country_Name != NULL)
            {
                sprintf(time_zones->Zone[zone_index].Country_Name, "%s", parse_data->ISO3166[country_index].Country_Name);
            }
        }
        
        time_zones->Zone[zone_index].TZ_Identifier = (uint8_t*)malloc((strlen(parse_data->Zonetab[zone_index].TZ_Identifier) + 1) * sizeof(uint8_t));
        if (time_zones->Zone[zone_index].TZ_Identifier != NULL)
        {
            sprintf(time_zones->Zone[zone_index].TZ_Identifier, "%s", parse_data->Zonetab[zone_index].TZ_Identifier);
        }
        
        time_zones->Zone[zone_index].Latitude = parse_data->Zonetab[zone_index].Latitude;

        time_zones->Zone[zone_index].Longitude = parse_data->Zonetab[zone_index].Longitude;

        time_zones->Zone[zone_index].Comments = (uint8_t*)malloc((strlen(parse_data->Zonetab[zone_index].Comments) + 1) * sizeof(uint8_t));
        if (time_zones->Zone[zone_index].Comments != NULL)
        {
            sprintf(time_zones->Zone[zone_index].Comments, "%s", parse_data->Zonetab[zone_index].Comments);
        }

        time_zones->Zone[zone_index].Standard_Offset = Get_Zone_Last_Standard_Offset(parse_data, time_zones->Zone[zone_index].TZ_Identifier);
        int16_t h=0,m=0;
        uint8_t sign = '+';
        h = time_zones->Zone[zone_index].Standard_Offset / 3600;
        if (h < 0)
        {
            h*=-1;
            sign = '-';
        }
        m = (time_zones->Zone[zone_index].Standard_Offset % 3600) / 60;
        if (m < 0)
        {
            m*=-1;
        }
        
        sprintf(time_zones->Zone[zone_index].Standard_Offset_Text, "%c%02d:%02d", sign, h, m);

        time_zones->Zone[zone_index].Year_Begin = Get_Zone_Year_Begin(parse_data, time_zones->Zone[zone_index].TZ_Identifier);
        time_zones->Zone[zone_index].Year_End = Get_Zone_Year_End(parse_data, time_zones->Zone[zone_index].TZ_Identifier);

        fprintf(temp, "%s\t%s\t%s\t%f\t%f\t%s\t%d\t%s\t%d\t%d\n", 
            time_zones->Zone[zone_index].Country_Code, 
            time_zones->Zone[zone_index].Country_Name,
            time_zones->Zone[zone_index].TZ_Identifier,
            time_zones->Zone[zone_index].Latitude,
            time_zones->Zone[zone_index].Longitude,
            time_zones->Zone[zone_index].Comments,
            time_zones->Zone[zone_index].Standard_Offset,
            time_zones->Zone[zone_index].Standard_Offset_Text,
            time_zones->Zone[zone_index].Year_Begin,
            time_zones->Zone[zone_index].Year_End);
    }

    
    
    fclose(temp);
    
}

int32_t Get_Country_Name(const Parse_Data_t* parse_data, const uint8_t* country_code)
{
    if (country_code == NULL)
    {
        return -1;
    }
    uint32_t country_count = parse_data->ISO3166_Count;
    uint32_t country_index = 0;
    for (country_index = 0; country_index < country_count; country_index++)
    {
        if (strcmp(parse_data->ISO3166[country_index].Country_Code, country_code) == 0)
        {
            return country_index;
        }
    }
    return -1;
}

int32_t Get_Zone_Last_Standard_Offset(const Parse_Data_t* parse_data, const uint8_t* tz_identifire)
{
    if (tz_identifire == NULL)
    {
        return 0;
    }

    uint32_t link_index = 0;
    uint32_t find_index = 0;
    uint8_t tz[MAX_LENGHT_DATA_FIELD];
    sprintf(tz, "%s", tz_identifire);
    uint32_t tz_index = 0;
___START:
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, tz) == 0)
        {
            if (parse_data->Zones[tz_index].Info_Count > 0)
            {
                return parse_data->Zones[tz_index].Info[parse_data->Zones[tz_index].Info_Count - 1].Standard_Offset;
            }
        }
    }

    
    for (link_index = find_index; link_index < parse_data->Links_Count; link_index++)
    {
        if (strcmp(parse_data->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", parse_data->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }
    return 0;
}

int16_t Get_Zone_Year_Begin(const Parse_Data_t* parse_data, const uint8_t* tz_identifire)
{
    if (tz_identifire == NULL)
    {
        return -1;
    }

    uint32_t tz_index = 0;
    uint32_t link_index = 0;
    uint32_t find_index = 0;
    uint8_t tz[MAX_LENGHT_DATA_FIELD];
    sprintf(tz, "%s", tz_identifire);
___START:
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, tz) == 0)
        {
            return parse_data->Zones[tz_index].Year_Begin;
        }
    }

    for (link_index = find_index; link_index < parse_data->Links_Count; link_index++)
    {
        if (strcmp(parse_data->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", parse_data->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }

    return -1;
}

int16_t Get_Zone_Year_End(const Parse_Data_t* parse_data, const uint8_t* tz_identifire)
{
    if (tz_identifire == NULL)
    {
        return -1;
    }
    uint32_t tz_index = 0;
    uint32_t link_index = 0;
    uint32_t find_index = 0;
    uint8_t tz[MAX_LENGHT_DATA_FIELD];
    sprintf(tz, "%s", tz_identifire);
___START:
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, tz) == 0)
        {
            if (parse_data->Zones[tz_index].Info_Count > 0)
            {
                if (parse_data->Zones[tz_index].Info[parse_data->Zones[tz_index].Info_Count - 1].Rule.Has_Rule)
                {
                    if (strlen(parse_data->Zones[tz_index].Info[parse_data->Zones[tz_index].Info_Count - 1].Rule.Rule_Name) > 0)
                    {
                        uint8_t rule_name[MAX_LENGHT_DATA_FIELD];
                        sprintf(rule_name, "%s", parse_data->Zones[tz_index].Info[parse_data->Zones[tz_index].Info_Count - 1].Rule.Rule_Name);
                        return Get_Rule_Year_End(parse_data, rule_name);
                    }
                }
            }
            return parse_data->Zones[tz_index].Year_End;
        }
    }

    for (link_index = find_index; link_index < parse_data->Links_Count; link_index++)
    {
        if (strcmp(parse_data->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", parse_data->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }
    return -1;
}

int16_t Get_Rule_Year_End(const Parse_Data_t* parse_data, const uint8_t* rule_name)
{
    if (rule_name == NULL)
    {
        return -1;
    }
    uint32_t rule_index = 0;
    for (rule_index = 0; rule_index < parse_data->Rules_Count; rule_index++)
    {
        if (strcmp(parse_data->Rules[rule_index].Name, rule_name) == 0)
        {
            return parse_data->Rules[rule_index].Year_End;
        }
    }
    return -1;
}