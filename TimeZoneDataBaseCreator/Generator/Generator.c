
#include "Generator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Time_Zones_t* Generate_Data(CONST CHAR** data_folder_path)
{
    Parse_Data_t* parse_data = Parse_Data(data_folder_path);
    if (parse_data == NULL)
    {
        return NULL;
    }

    Time_Zones_t* time_zones = calloc(1, sizeof(Time_Zones_t));
    if (time_zones == NULL)
    {
        return NULL;
    }

    time_zones->Version = *parse_data->Version;
    
    time_zones->Zones_Count = parse_data->Zonetab_Count;
    Generate_Time_Zones_Info(parse_data, &time_zones->Zones_Info, &parse_data->Zonetab_Count);
    Generate_Time_Zones_Data(parse_data, &time_zones->Zones_Data,&time_zones->Zones_Data_Count, &time_zones);
    
    time_zones->Rules_Count = parse_data->Rules_Count;
    Generate_Rules_Info(parse_data, &time_zones->Rules_Info);
    Generate_Rules_Data(parse_data, &time_zones->Rules_Data, &time_zones->Rules_Data_Count, &time_zones);

    return time_zones;
}

VOID Generate_Time_Zones_Info(Parse_Data_t* parse_data, Zone_Info_t** zone_info_list,CONST COUNTER* zones_count)
{
    if (parse_data==NULL || zone_info_list==NULL || zones_count==NULL)
    {
        return;
    }
    COUNTER zone_index = 0;
    for (zone_index = 0; zone_index < *zones_count; zone_index++)
    {
        Zone_Info_t* z_info;
        if (zone_index == 0)
        {
            z_info = (Zone_Info_t*)malloc((zone_index + 1) * sizeof(Zone_Info_t));
        }
        else
        {
            z_info = (Zone_Info_t*)realloc(*zone_info_list, (zone_index + 1) * sizeof(Zone_Info_t));
        }
        if (z_info == NULL)
        {
            return;
        }
        *zone_info_list = z_info;

        (*zone_info_list)[zone_index].Country_Code = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].Country_Code) + 1) * sizeof(CHAR));
        if ((*zone_info_list)[zone_index].Country_Code != NULL)
        {
            sprintf((*zone_info_list)[zone_index].Country_Code, "%s", parse_data->Zonetab[zone_index].Country_Code);
        }

        COUNTER country_index = Get_Country_Name(parse_data, &parse_data->Zonetab[zone_index].Country_Code);
        if (country_index != -1)
        {
            (*zone_info_list)[zone_index].Country_Name = (CHAR*)malloc((strlen(parse_data->ISO3166[country_index].Country_Name) + 1) * sizeof(CHAR));
            if ((*zone_info_list)[zone_index].Country_Name != NULL)
            {
                sprintf((*zone_info_list)[zone_index].Country_Name, "%s", parse_data->ISO3166[country_index].Country_Name);
            }
        }
        
        (*zone_info_list)[zone_index].Time_Zone_Identifier = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].TZ_Identifier) + 1) * sizeof(CHAR));
        if ((*zone_info_list)[zone_index].Time_Zone_Identifier != NULL)
        {
            sprintf((*zone_info_list)[zone_index].Time_Zone_Identifier, "%s", parse_data->Zonetab[zone_index].TZ_Identifier);
        }
        
        (*zone_info_list)[zone_index].Latitude = parse_data->Zonetab[zone_index].Latitude;

        (*zone_info_list)[zone_index].Longitude = parse_data->Zonetab[zone_index].Longitude;

        (*zone_info_list)[zone_index].Comments = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].Comments) + 1) * sizeof(CHAR));
        if ((*zone_info_list)[zone_index].Comments != NULL)
        {
            sprintf((*zone_info_list)[zone_index].Comments, "%s", parse_data->Zonetab[zone_index].Comments);
        }

        (*zone_info_list)[zone_index].Has_Data = TZ_Check_Data(parse_data, &(*zone_info_list)[zone_index].Time_Zone_Identifier);

        if ((*zone_info_list)[zone_index].Has_Data == FALSE)
        {
            COUNTER link_index =  TZ_Get_Linked_Identifier(parse_data, &(*zone_info_list)[zone_index].Time_Zone_Identifier);
            if (link_index != INDEX_NOT_FOUND)
            {
                (*zone_info_list)[zone_index].Linked_TZ_Identifier = (CHAR*)calloc(strlen(parse_data->Links[link_index].Target) + 1, sizeof(CHAR));
                (*zone_info_list)[zone_index].Linked_TZ_Identifier = _strdup(parse_data->Links[link_index].Target);
            }
            (*zone_info_list)[zone_index].Data_Count = Get_Zone_Data_Count(parse_data, &(*zone_info_list)[zone_index].Linked_TZ_Identifier);
            (*zone_info_list)[zone_index].Year_Begin = Get_Zone_Year_Begin(parse_data,&(*zone_info_list)[zone_index].Linked_TZ_Identifier);
            (*zone_info_list)[zone_index].Year_End = Get_Zone_Year_End(parse_data, &(*zone_info_list)[zone_index].Linked_TZ_Identifier);
        }
        else
        {
            (*zone_info_list)[zone_index].Linked_TZ_Identifier = (CHAR*)calloc(1,sizeof(CHAR));
            (*zone_info_list)[zone_index].Data_Count = Get_Zone_Data_Count(parse_data, &(*zone_info_list)[zone_index].Time_Zone_Identifier);
            (*zone_info_list)[zone_index].Year_Begin = Get_Zone_Year_Begin(parse_data, &(*zone_info_list)[zone_index].Time_Zone_Identifier);
            (*zone_info_list)[zone_index].Year_End = Get_Zone_Year_End(parse_data, &(*zone_info_list)[zone_index].Time_Zone_Identifier);
        }
        
    }

    Sort_Zone_Info_By_Identifier(zone_info_list, zones_count);

   
    for (COUNTER tz_index = 0; tz_index < *zones_count; tz_index++)
    {
        (*zone_info_list)[tz_index].Time_Zone_ID = tz_index + 1;
    }
   
}

VOID Generate_Time_Zones_Data(Parse_Data_t* parse_data, Zone_Data_t** zones_data_list, COUNTER* zones_data_count, Time_Zones_t** tz_list)
{
    Sort_Zone_Data_By_Identifier(&parse_data->Zones, &parse_data->Zones_Count);
    *zones_data_count = 0;
    COUNTER zone_index = 0;
    COUNTER data_index = 0;
    for (zone_index = 0; zone_index < parse_data->Zones_Count; zone_index++)
    {
        for (data_index = 0; data_index< parse_data->Zones[zone_index].Info_Count; data_index++)
        {
            Zone_Data_t* z_data;
            if (*zones_data_count == 0)
            {
                z_data = (Zone_Data_t*)malloc(sizeof(Zone_Data_t));
            }
            else
            {
                z_data = (Zone_Data_t*)realloc(*zones_data_list, (*zones_data_count + 1) * sizeof(Zone_Data_t));
            }
            if (z_data == NULL)
            {
                return;
            }
            *zones_data_list = z_data;

            (*zones_data_list)[*zones_data_count] = parse_data->Zones[zone_index].Info[data_index];
            if (strcmp(parse_data->Zones[zone_index].Name, "Europe/Astrakhan") == 0)
            {
                int x = 0;
                x++;
            }
            (*zones_data_list)[*zones_data_count].Time_Zone_ID = Get_Zone_ID(&(*tz_list)->Zones_Info,&(*tz_list)->Zones_Count,&parse_data->Zones[zone_index].Name);
            (*zones_data_count)++;
            
        }
        
    }
}

VOID Generate_Rules_Info(Parse_Data_t* parse_data, Rule_Info_t** rules_info_list)
{
    Sort_Rules_By_Name(&parse_data->Rules, &parse_data->Rules_Count);
    COUNTER rule_index = 0;
    for (rule_index = 0; rule_index < parse_data->Rules_Count; rule_index++)
    {
        Rule_Info_t* r_info;
        if (rule_index == 0)
        {
            r_info = (Rule_Info_t*)malloc(sizeof(Rule_Info_t));
        }
        else
        {
            r_info = (Rule_Info_t*)realloc(*rules_info_list, (rule_index + 1) * sizeof(Rule_Info_t));
        }
        if (r_info == NULL)
        {
            return;
        }
        *rules_info_list = r_info;

        (*rules_info_list)[rule_index].Name = (CHAR*)calloc(strlen(parse_data->Rules[rule_index].Name) + 1, sizeof(CHAR));
        if ((*rules_info_list)[rule_index].Name != NULL)
        {
            (*rules_info_list)[rule_index].Name = _strdup(parse_data->Rules[rule_index].Name);
        }

        (*rules_info_list)[rule_index].Years_Count = parse_data->Rules[rule_index].Years_Count;
        (*rules_info_list)[rule_index].Year_Begin = parse_data->Rules[rule_index].Year_Begin;
        (*rules_info_list)[rule_index].Year_End = parse_data->Rules[rule_index].Year_End;
    }

    for (rule_index = 0; rule_index < parse_data->Rules_Count; rule_index++)
    {
        (*rules_info_list)[rule_index].Rule_ID = rule_index + 1;
    }

}

VOID Generate_Rules_Data(Parse_Data_t* parse_data, Rule_Data_t** rules_data_list, COUNTER* rules_data_count, Time_Zones_t** tz_list)
{
    *rules_data_count = 0;
    COUNTER reule_index = 0;
    COUNTER data_index = 0;
    for (reule_index = 0; reule_index < parse_data->Rules_Count; reule_index++)
    {
        for (data_index = 0; data_index < parse_data->Rules[reule_index].Years_Count; data_index++)
        {
            Rule_Data_t* r_data;
            if (*rules_data_count == 0)
            {
                r_data = (Rule_Data_t*)malloc(sizeof(Rule_Data_t));
            }
            else
            {
                r_data = (Rule_Data_t*)realloc(*rules_data_list, (*rules_data_count + 1) * sizeof(Rule_Data_t));
            }
            if (r_data == NULL)
            {
                return;
            }
            *rules_data_list = r_data;

            (*rules_data_list)[*rules_data_count] = parse_data->Rules[reule_index].Years[data_index];
            (*rules_data_list)[*rules_data_count].Rule_ID = Get_Rule_ID(&(*tz_list)->Rules_Info, &(*tz_list)->Rules_Count, &parse_data->Rules[reule_index].Name);
            (*rules_data_count)++;
        }
    }
}



COUNTER Get_Country_Name(CONST Parse_Data_t* parse_data, CONST CHAR** country_code)
{
    if (*country_code == NULL)
    {
        return -1;
    }
    COUNTER country_count = parse_data->ISO3166_Count;
    COUNTER country_index = 0;
    for (country_index = 0; country_index < country_count; country_index++)
    {
        if (strcmp(parse_data->ISO3166[country_index].Country_Code, *country_code) == 0)
        {
            return country_index;
        }
    }
    return -1;
}

BOOL TZ_Check_Data(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifier)
{
    for (COUNTER zone_index = 0; zone_index < parse_data->Zones_Count; zone_index++)
    {
        if (strcmp(parse_data->Zones[zone_index].Name, *tz_identifier) == 0)
        {
            return TRUE;
        }
    }
    return false;
}

COUNTER TZ_Get_Linked_Identifier(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INDEX_NOT_FOUND;
    }

    for (COUNTER link_index = 0; link_index < parse_data->Links_Count; link_index++)
    {
        if (strcmp(parse_data->Links[link_index].Link_Name, *tz_identifire) == 0)
        {
            if (TZ_Check_Data(parse_data, &parse_data->Links[link_index].Target) == TRUE)
            {
                return link_index;
            }
        }
    }
    return INDEX_NOT_FOUND;
}

YEAR Get_Zone_Year_Begin(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INVALID_YEAR;
    }

    COUNTER tz_index = 0;
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, *tz_identifire) == 0)
        {
            return parse_data->Zones[tz_index].Year_Begin;
        }
    }
    return INVALID_YEAR;
}

YEAR Get_Zone_Year_End(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire)
{
    if (*tz_identifire == NULL)
    {
        return INVALID_YEAR;
    }

    COUNTER tz_index = 0;
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, *tz_identifire) == 0)
        {
            return parse_data->Zones[tz_index].Year_End;
        }
    }

    return INVALID_YEAR;
}

COUNTER Get_Zone_Data_Count(CONST Parse_Data_t* parse_data, CONST CHAR** tz_identifire)
{
    COUNTER counts = 0;
    COUNTER tz_index = 0;
    COUNTER data_index = 0;
    for (tz_index = 0; tz_index < parse_data->Zones_Count; tz_index++)
    {
        if (strcmp(parse_data->Zones[tz_index].Name, *tz_identifire) == 0)
        {
            return parse_data->Zones[tz_index].Info_Count;
        }
    }
    return counts;
}

COUNTER Get_Zone_ID(CONST Zone_Info_t** tz_info_list, CONST COUNTER* tz_info_count, CONST CHAR** tz_identifier)
{
    COUNTER tz_id = 0;
    for (COUNTER tz_info_index = 0; tz_info_index < *tz_info_count; tz_info_index++)
    {
        if (strcmp((*tz_info_list)[tz_info_index].Time_Zone_Identifier, *tz_identifier) == 0)
        {
            return (*tz_info_list)[tz_info_index].Time_Zone_ID;
        }
    }
    return tz_id;
}

COUNTER Get_Rule_ID(CONST Rule_Info_t** rule_info_list, CONST COUNTER* rule_info_count, CONST CHAR** rule_name)
{
    COUNTER rule_id = 0;
    for (COUNTER rule_info_index = 0; rule_info_index < *rule_info_count; rule_info_index++)
    {
        if (strcmp((*rule_info_list)[rule_info_index].Name, *rule_name) == 0)
        {
            return (*rule_info_list)[rule_info_index].Rule_ID;
        }
    }
    return rule_id;
}



INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b) 
{
    Zone_Info_t* structA = (Zone_Info_t*)a;
    Zone_Info_t* structB = (Zone_Info_t*)b;
    return strcmp(structA->Time_Zone_Identifier, structB->Time_Zone_Identifier);
}

VOID Sort_Zone_Info_By_Identifier(Zone_Info_t** zone_info, COUNTER* info_count)
{
    qsort(*zone_info, *info_count, sizeof(Zone_Info_t), Compare_TZ_Identifier);
}

INT Compare_Zones_Data_Identifier(CONST VOID* a, CONST VOID* b)
{
    Zone_Entry_t* structA = (Zone_Entry_t*)a;
    Zone_Entry_t* structB = (Zone_Entry_t*)b;
    return strcmp(structA->Name, structB->Name);
}

VOID Sort_Zone_Data_By_Identifier(Zone_Entry_t** zone_data, COUNTER* data_count)
{
    qsort(*zone_data, *data_count, sizeof(Zone_Entry_t), Compare_Zones_Data_Identifier);
}

INT Compare_Rules_Name(CONST VOID* a, CONST VOID* b)
{
    Rule_Entry_t* structA = (Rule_Entry_t*)a;
    Rule_Entry_t* structB = (Rule_Entry_t*)b;
    return strcmp(structA->Name, structB->Name);
}

VOID Sort_Rules_By_Name(Rule_Info_t** rules_info, COUNTER* info_count)
{
    qsort(*rules_info, *info_count, sizeof(Rule_Entry_t), Compare_Rules_Name);
}
