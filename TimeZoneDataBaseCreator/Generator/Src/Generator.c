
#include "../Inc/Generator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BOOL Generate_Data(CONST CHAR** data_folder_path)
{
    if (!Generate_Full_List(data_folder_path))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL Generate_Full_List(CONST CHAR** data_folder_path)
{
    G_Time_Zones_t* time_zones = (G_Time_Zones_t*)malloc(sizeof(G_Time_Zones_t));
    if (time_zones == NULL)
    {
        return FALSE;
    }

    Parse_Data_t* parse_data = Parse_Data(data_folder_path);
    if (parse_data == NULL)
    {
        return FALSE;
    }

    COUNTER zones_counts = 0;
    zones_counts = parse_data->Zonetab_Count;
    time_zones->Zones_Count = zones_counts;

    COUNTER zone_index = 0;
    for (zone_index = 0; zone_index < zones_counts; zone_index++)
    {
        if (zone_index == 0)
        {
            G_Zone_Info_t* z_info = (G_Zone_Info_t*)malloc((zone_index + 1) * sizeof(G_Zone_Info_t));
            if (z_info == NULL)
            {
                return FALSE;
            }
            time_zones->Zone = z_info;
        }
        else
        {

            G_Zone_Info_t* z_info = (G_Zone_Info_t*)realloc(time_zones->Zone, (zone_index + 1) * sizeof(G_Zone_Info_t));
            if (z_info == NULL)
            {
                return FALSE;
            }
            time_zones->Zone = z_info;
        }

        time_zones->Zone[zone_index].Country_Code = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].Country_Code) + 1) * sizeof(CHAR));
        if (time_zones->Zone[zone_index].Country_Code != NULL)
        {
            sprintf(time_zones->Zone[zone_index].Country_Code, "%s", parse_data->Zonetab[zone_index].Country_Code);
        }

        COUNTER country_index = Get_Country_Name(&parse_data, &parse_data->Zonetab[zone_index].Country_Code);
        if (country_index != -1)
        {
            time_zones->Zone[zone_index].Country_Name = (CHAR*)malloc((strlen(parse_data->ISO3166[country_index].Country_Name) + 1) * sizeof(CHAR));
            if (time_zones->Zone[zone_index].Country_Name != NULL)
            {
                sprintf(time_zones->Zone[zone_index].Country_Name, "%s", parse_data->ISO3166[country_index].Country_Name);
            }
        }
        
        time_zones->Zone[zone_index].TZ_Identifier = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].TZ_Identifier) + 1) * sizeof(CHAR));
        if (time_zones->Zone[zone_index].TZ_Identifier != NULL)
        {
            sprintf(time_zones->Zone[zone_index].TZ_Identifier, "%s", parse_data->Zonetab[zone_index].TZ_Identifier);
        }
        
        time_zones->Zone[zone_index].Latitude = parse_data->Zonetab[zone_index].Latitude;

        time_zones->Zone[zone_index].Longitude = parse_data->Zonetab[zone_index].Longitude;

        time_zones->Zone[zone_index].Comments = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].Comments) + 1) * sizeof(CHAR));
        if (time_zones->Zone[zone_index].Comments != NULL)
        {
            sprintf(time_zones->Zone[zone_index].Comments, "%s", parse_data->Zonetab[zone_index].Comments);
        }

        time_zones->Zone[zone_index].Standard_Offset = Get_Zone_Last_Standard_Offset(&parse_data, &time_zones->Zone[zone_index].TZ_Identifier);
        HOUR h=0,m=0;
        CHAR sign = '+';
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
        
        sprintf(time_zones->Zone[zone_index].Standard_Offset_Text, "%c%02lld:%02lld", sign, h, m);

        BOOL dst_effect; 
        HOUR save_hour;
        Get_Zone_DST_Effect(&parse_data, &time_zones->Zone[zone_index].TZ_Identifier, &dst_effect, &save_hour);
        time_zones->Zone[zone_index].DST_Effect = dst_effect;
        time_zones->Zone[zone_index].DST_Offset = save_hour;
        h = time_zones->Zone[zone_index].DST_Offset / 3600;
        sign = '+';
        if (h < 0)
        {
            h *= -1;
            sign = '-';
        }
        m = (time_zones->Zone[zone_index].DST_Offset % 3600) / 60;
        if (m < 0)
        {
            m *= -1;
        }
        sprintf(time_zones->Zone[zone_index].DST_Offset_Text, "%c%02lld:%02lld", sign, h, m);


        time_zones->Zone[zone_index].Year_Begin = Get_Zone_Year_Begin(&parse_data, &time_zones->Zone[zone_index].TZ_Identifier);
        time_zones->Zone[zone_index].Year_End = Get_Zone_Year_End(&parse_data, &time_zones->Zone[zone_index].TZ_Identifier);
        
    }

    Sort_Zone_Info_By_Identifier(&time_zones->Zone, &time_zones->Zones_Count);
    Sort_Zone_Info_By_STD_Offset_Then_Identifier(&time_zones->Zone, &time_zones->Zones_Count);

    FILE* temp = fopen("../temp.txt", "w");
    for (zone_index = 0; zone_index < zones_counts; zone_index++)
    {
        /*fprintf(temp, "%s\t%s\t%s\t%.6lf\t%.6lf\t%s\t%lld\t%s\t%d\t%d\n",
            time_zones->Zone[zone_index].Country_Code,
            time_zones->Zone[zone_index].Country_Name,
            time_zones->Zone[zone_index].TZ_Identifier,
            time_zones->Zone[zone_index].Latitude,
            time_zones->Zone[zone_index].Longitude,
            time_zones->Zone[zone_index].Comments,
            time_zones->Zone[zone_index].Standard_Offset,
            time_zones->Zone[zone_index].Standard_Offset_Text,
            time_zones->Zone[zone_index].Year_Begin,
            time_zones->Zone[zone_index].Year_End);*/
        if (time_zones->Zone[zone_index].DST_Effect == TRUE)
        {
            fprintf(temp, "%s | %s\n(UTC %s - DST %s)\n\n",
                time_zones->Zone[zone_index].TZ_Identifier,
                time_zones->Zone[zone_index].Country_Name,
                time_zones->Zone[zone_index].Standard_Offset_Text,
                time_zones->Zone[zone_index].DST_Offset_Text);
        }
        else
        {
            fprintf(temp, "%s | %s\n(UTC %s)\n\n",
                time_zones->Zone[zone_index].TZ_Identifier,
                time_zones->Zone[zone_index].Country_Name,
                time_zones->Zone[zone_index].Standard_Offset_Text);
        }
        
    }
    fclose(temp);

    //Parse_Free_Rules(parse_data->Rules, parse_data->Rules_Count);
    
    return TRUE;
}

COUNTER Get_Country_Name(CONST Parse_Data_t** parse_data, CONST CHAR** country_code)
{
    if (*country_code == NULL)
    {
        return -1;
    }
    COUNTER country_count = (*parse_data)->ISO3166_Count;
    COUNTER country_index = 0;
    for (country_index = 0; country_index < country_count; country_index++)
    {
        if (strcmp((*parse_data)->ISO3166[country_index].Country_Code, *country_code) == 0)
        {
            return country_index;
        }
    }
    return -1;
}

HOUR Get_Zone_Last_Standard_Offset(CONST Parse_Data_t** parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INVALID_HOUR;
    }

    COUNTER link_index = 0;
    COUNTER find_index = 0;
    CHAR tz[MAX_LENGTH_DATA_FIELD];
    sprintf(tz, "%s", *tz_identifire);
    COUNTER tz_index = 0;
___START:
    for (tz_index = 0; tz_index < (*parse_data)->Zones_Count; tz_index++)
    {
        if (strcmp((*parse_data)->Zones[tz_index].Name, tz) == 0)
        {
            if ((*parse_data)->Zones[tz_index].Info_Count > 0)
            {
                return (*parse_data)->Zones[tz_index].Info[(*parse_data)->Zones[tz_index].Info_Count - 1].Standard_Offset;
            }
        }
    }

    
    for (link_index = find_index; link_index < (*parse_data)->Links_Count; link_index++)
    {
        if (strcmp((*parse_data)->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", (*parse_data)->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }
    return INVALID_HOUR;
}

VOID Get_Zone_DST_Effect(CONST Parse_Data_t** parse_data, CONST CHAR** tz_identifire, BOOL* dst_effect, HOUR* save_hour)
{
    *dst_effect = false;
    *save_hour = 0;
    if (*tz_identifire == NULL)
    {
        return;
    }
    COUNTER tz_index = 0;
    COUNTER link_index = 0;
    COUNTER find_index = 0;
    CHAR tz[MAX_LENGTH_DATA_FIELD];
    sprintf(tz, "%s", *tz_identifire);
___START:
    for (tz_index = 0; tz_index < (*parse_data)->Zones_Count; tz_index++)
    {
        if (strcmp((*parse_data)->Zones[tz_index].Name, tz) == 0)
        {
            if ((*parse_data)->Zones[tz_index].Info_Count > 0)
            {
                if ((*parse_data)->Zones[tz_index].Info[(*parse_data)->Zones[tz_index].Info_Count - 1].Rule.Has_Rule)
                {
                    if (strlen((*parse_data)->Zones[tz_index].Info[(*parse_data)->Zones[tz_index].Info_Count - 1].Rule.Rule_Name) > 0)
                    {
                        
                        YEAR rule_end_year = Get_Rule_Year_End(parse_data, &(*parse_data)->Zones[tz_index].Info[(*parse_data)->Zones[tz_index].Info_Count - 1].Rule.Rule_Name);
                        if ((rule_end_year != -1) && (rule_end_year < (*parse_data)->Version->Major))
                        {
                            return;
                        }
                        else if ((rule_end_year == -1) || (rule_end_year >= (*parse_data)->Version->Major))
                        {
                            Get_Rule_Year_End_DST_Data(parse_data, &(*parse_data)->Zones[tz_index].Info[(*parse_data)->Zones[tz_index].Info_Count - 1].Rule.Rule_Name, dst_effect, save_hour);
                            return;
                        }
                    }
                }
            }
            return;
        }
    }

    for (link_index = find_index; link_index < (*parse_data)->Links_Count; link_index++)
    {
        if (strcmp((*parse_data)->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", (*parse_data)->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }
}

YEAR Get_Zone_Year_Begin(CONST Parse_Data_t** parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INVALID_YEAR;
    }

    COUNTER tz_index = 0;
    COUNTER link_index = 0;
    COUNTER find_index = 0;
    CHAR tz[MAX_LENGTH_DATA_FIELD];
    sprintf(tz, "%s", *tz_identifire);
___START:
    for (tz_index = 0; tz_index < (*parse_data)->Zones_Count; tz_index++)
    {
        if (strcmp((*parse_data)->Zones[tz_index].Name, tz) == 0)
        {
            return (*parse_data)->Zones[tz_index].Year_Begin;
        }
    }

    for (link_index = find_index; link_index < (*parse_data)->Links_Count; link_index++)
    {
        if (strcmp((*parse_data)->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", (*parse_data)->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }

    return INVALID_YEAR;
}

YEAR Get_Zone_Year_End(CONST Parse_Data_t** parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INVALID_YEAR;
    }

    COUNTER tz_index = 0;
    COUNTER link_index = 0;
    COUNTER find_index = 0;
    CHAR tz[MAX_LENGTH_DATA_FIELD];
    sprintf(tz, "%s", *tz_identifire);
___START:
    for (tz_index = 0; tz_index < (*parse_data)->Zones_Count; tz_index++)
    {
        if (strcmp((*parse_data)->Zones[tz_index].Name, tz) == 0)
        {
            return (*parse_data)->Zones[tz_index].Year_End;
        }
    }

    for (link_index = find_index; link_index < (*parse_data)->Links_Count; link_index++)
    {
        if (strcmp((*parse_data)->Links[link_index].Link_Name, tz) == 0)
        {
            sprintf(tz, "%s", (*parse_data)->Links[link_index].Target);
            find_index = link_index + 1;
            goto ___START;
        }
    }

    return INVALID_YEAR;
}

YEAR Get_Rule_Year_End(CONST Parse_Data_t** parse_data, CONST CHAR** rule_name)
{
    if (*rule_name == NULL)
    {
        return INVALID_YEAR;
    }
    COUNTER rule_index = 0;
    for (rule_index = 0; rule_index < (*parse_data)->Rules_Count; rule_index++)
    {
        if (strcmp((*parse_data)->Rules[rule_index].Name, *rule_name) == 0)
        {
            return (*parse_data)->Rules[rule_index].Year_End;
        }
    }
    return INVALID_YEAR;
}

VOID Get_Rule_Year_End_DST_Data(CONST Parse_Data_t** parse_data, CONST CHAR** rule_name, BOOL* dst_effect, HOUR* save_hour)
{
    /*if (*rule_name == NULL)
    {
        return INVALID_YEAR;
    }
    COUNTER rule_index = 0;
    for (rule_index = 0; rule_index < (*parse_data)->Rules_Count; rule_index++)
    {
        if (strcmp((*parse_data)->Rules[rule_index].Name, *rule_name) == 0)
        {
            if ((*parse_data)->Rules[rule_index].Years_Count > 0)
            {
                if ((*parse_data)->Rules[rule_index].Years[(*parse_data)->Rules[rule_index].Years_Count - 1].DST_Count > 0)
                {
                    *dst_effect = TRUE;
                    *save_hour = (*parse_data)->Rules[rule_index].Years[(*parse_data)->Rules[rule_index].Years_Count - 1].DST[0].Save_Hour;
                }
                else
                {
                    *dst_effect = false;
                    *save_hour = 0;
                }
            }
        }
    }*/
    return INVALID_YEAR;
}

INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b) 
{
    G_Zone_Info_t* structA = (G_Zone_Info_t*)a;
    G_Zone_Info_t* structB = (G_Zone_Info_t*)b;
    return strcmp(structA->TZ_Identifier, structB->TZ_Identifier);
}

VOID Sort_Zone_Info_By_Identifier(G_Zone_Info_t** zone_info, COUNTER* info_count)
{
    qsort(*zone_info, *info_count, sizeof(G_Zone_Info_t), Compare_TZ_Identifier);
}

INT Compare_Standard_Offset(CONST VOID* a, CONST VOID* b) 
{
    G_Zone_Info_t* pa = (G_Zone_Info_t*)a;
    G_Zone_Info_t* pb = (G_Zone_Info_t*)b;
    return pa->Standard_Offset - pb->Standard_Offset;
}

VOID Sort_Zone_Info_By_STD_Offset(G_Zone_Info_t** zone_info, COUNTER* info_count)
{
    qsort(*zone_info, *info_count, sizeof(G_Zone_Info_t), Compare_Standard_Offset);
}

INT Compare_Zones_Standard_Offset_Then_Identifier(CONST VOID* a, CONST VOID* b)
{
    G_Zone_Info_t* pa = (G_Zone_Info_t*)a;
    G_Zone_Info_t* pb = (G_Zone_Info_t*)b;

    char temp1[100], temp2[100];
    strcpy(temp1, pa->TZ_Identifier);
    strcpy(temp2, pb->TZ_Identifier);
    for (int i = 0; temp1[i]; i++) {
        temp1[i] = tolower(temp1[i]);
    }
    for (int i = 0; temp2[i]; i++) {
        temp2[i] = tolower(temp2[i]);
    }

    // ابتدا بر اساس Standard_Offset مقایسه می‌کنیم
    int result = pa->Standard_Offset - pb->Standard_Offset;
    if (result != 0) {
        return result;
    }

    // اگر Standard_Offset برابر بود، بر اساس TZ_Identifier مقایسه می‌کنیم
    return strcmp(temp1, temp2);
}

VOID Sort_Zone_Info_By_STD_Offset_Then_Identifier(G_Zone_Info_t** zone_info, COUNTER* info_count)
{
    qsort(*zone_info, *info_count, sizeof(G_Zone_Info_t), Compare_Zones_Standard_Offset_Then_Identifier);
}