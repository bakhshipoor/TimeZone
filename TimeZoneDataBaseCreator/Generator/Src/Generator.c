
#include "../Inc/Generator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BOOL Generate_Data(CONST CHAR** data_folder_path)
{
    Parse_Data_t* parse_data = Parse_Data(data_folder_path);
    if (parse_data == NULL)
    {
        return FALSE;
    }

    Time_Zones_t* time_zones = calloc(1, sizeof(Time_Zones_t));
    if (time_zones == NULL)
    {
        return FALSE;
    }
    
    time_zones->Zones_Count = parse_data->Zonetab_Count;
    Generate_Time_Zones_Info(parse_data, &time_zones->Zones_Info, &parse_data->Zonetab_Count);
    
     

    return TRUE;
}

VOID Generate_Time_Zones_Info(Parse_Data_t* parse_data, Time_Zone_Info_t** zone_info_list,CONST COUNTER* zones_count)
{
    if (parse_data==NULL || zone_info_list==NULL || zones_count==NULL)
    {
        return;
    }
    COUNTER zone_index = 0;
    for (zone_index = 0; zone_index < *zones_count; zone_index++)
    {
        Time_Zone_Info_t* z_info;
        if (zone_index == 0)
        {
            z_info = (Time_Zone_Info_t*)malloc((zone_index + 1) * sizeof(Time_Zone_Info_t));
        }
        else
        {
            z_info = (Time_Zone_Info_t*)realloc(*zone_info_list, (zone_index + 1) * sizeof(Time_Zone_Info_t));
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
        
        (*zone_info_list)[zone_index].TZ_Identifier = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].TZ_Identifier) + 1) * sizeof(CHAR));
        if ((*zone_info_list)[zone_index].TZ_Identifier != NULL)
        {
            sprintf((*zone_info_list)[zone_index].TZ_Identifier, "%s", parse_data->Zonetab[zone_index].TZ_Identifier);
        }
        
        (*zone_info_list)[zone_index].Latitude = parse_data->Zonetab[zone_index].Latitude;

        (*zone_info_list)[zone_index].Longitude = parse_data->Zonetab[zone_index].Longitude;

        (*zone_info_list)[zone_index].Comments = (CHAR*)malloc((strlen(parse_data->Zonetab[zone_index].Comments) + 1) * sizeof(CHAR));
        if ((*zone_info_list)[zone_index].Comments != NULL)
        {
            sprintf((*zone_info_list)[zone_index].Comments, "%s", parse_data->Zonetab[zone_index].Comments);
        }

        (*zone_info_list)[zone_index].Has_Data = TZ_Check_Data(parse_data, &(*zone_info_list)[zone_index].TZ_Identifier);

        if ((*zone_info_list)[zone_index].Has_Data == FALSE)
        {
            COUNTER link_index =  TZ_Get_Linked_Identifier(parse_data, &(*zone_info_list)[zone_index].TZ_Identifier);
            if (link_index != INDEX_NOT_FOUND)
            {
                (*zone_info_list)[zone_index].Linked_TZ_Identifier = (CHAR*)calloc(strlen(parse_data->Links[link_index].Target) + 1, sizeof(CHAR));
                (*zone_info_list)[zone_index].Linked_TZ_Identifier = _strdup(parse_data->Links[link_index].Target);
            }
        }
        else
        {
            (*zone_info_list)[zone_index].Linked_TZ_Identifier = (CHAR*)calloc(1,sizeof(CHAR));
        }
        
    }

    Sort_Zone_Info_By_Identifier(zone_info_list, zones_count);

   
    for (COUNTER tz_index = 0; tz_index < *zones_count; tz_index++)
    {
        (*zone_info_list)[tz_index].TZ_ID = tz_index + 1;
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


COUNTER Get_TZ_ID(CONST Time_Zone_Info_t** tz_info_list, CONST COUNTER* tz_info_count, CONST CHAR** tz_identifier)
{
    COUNTER tz_id = 0;
    for (COUNTER tz_info_index = 0; tz_info_index < *tz_info_count; tz_info_index++)
    {
        if (strcmp((*tz_info_list)[tz_info_index].TZ_Identifier, *tz_identifier) == 0)
        {
            return (*tz_info_list)[tz_info_index].TZ_ID;
        }
    }
    return tz_id;
}
















INT Compare_TZ_Identifier(CONST VOID* a, CONST VOID* b) 
{
    Time_Zone_Info_t* structA = (Time_Zone_Info_t*)a;
    Time_Zone_Info_t* structB = (Time_Zone_Info_t*)b;
    return strcmp(structA->TZ_Identifier, structB->TZ_Identifier);
}

VOID Sort_Zone_Info_By_Identifier(Time_Zone_Info_t** zone_info, COUNTER* info_count)
{
    qsort(*zone_info, *info_count, sizeof(Time_Zone_Info_t), Compare_TZ_Identifier);
}




//INT Compare_Standard_Offset(CONST VOID* a, CONST VOID* b) 
//{
//    Time_Zone_Info_t* pa = (Time_Zone_Info_t*)a;
//    Time_Zone_Info_t* pb = (Time_Zone_Info_t*)b;
//    return pa->Standard_Offset - pb->Standard_Offset;
//}

//VOID Sort_Zone_Info_By_STD_Offset(Time_Zone_Info_t** zone_info, COUNTER* info_count)
//{
//    qsort(*zone_info, *info_count, sizeof(Time_Zone_Info_t), Compare_Standard_Offset);
//}

//INT Compare_Zones_Standard_Offset_Then_Identifier(CONST VOID* a, CONST VOID* b)
//{
//    Time_Zone_Info_t* pa = (Time_Zone_Info_t*)a;
//    Time_Zone_Info_t* pb = (Time_Zone_Info_t*)b;
//
//    char temp1[100], temp2[100];
//    strcpy(temp1, pa->TZ_Identifier);
//    strcpy(temp2, pb->TZ_Identifier);
//    for (int i = 0; temp1[i]; i++) {
//        temp1[i] = tolower(temp1[i]);
//    }
//    for (int i = 0; temp2[i]; i++) {
//        temp2[i] = tolower(temp2[i]);
//    }
//
//    // ابتدا بر اساس Standard_Offset مقایسه می‌کنیم
//    int result = pa->Standard_Offset - pb->Standard_Offset;
//    if (result != 0) {
//        return result;
//    }
//
//    // اگر Standard_Offset برابر بود، بر اساس TZ_Identifier مقایسه می‌کنیم
//    return strcmp(temp1, temp2);
//}

//VOID Sort_Zone_Info_By_STD_Offset_Then_Identifier(Time_Zone_Info_t** zone_info, COUNTER* info_count)
//{
//    qsort(*zone_info, *info_count, sizeof(Time_Zone_Info_t), Compare_Zones_Standard_Offset_Then_Identifier);
//}











//FILE* temp = fopen("../temp.txt", "w");
   //for (zone_index = 0; zone_index < zones_counts; zone_index++)
   //{
   //    /*fprintf(temp, "%s\t%s\t%s\t%.6lf\t%.6lf\t%s\t%lld\t%s\t%d\t%d\n",
   //        time_zones->Zone[zone_index].Country_Code,
   //        time_zones->Zone[zone_index].Country_Name,
   //        time_zones->Zone[zone_index].TZ_Identifier,
   //        time_zones->Zone[zone_index].Latitude,
   //        time_zones->Zone[zone_index].Longitude,
   //        time_zones->Zone[zone_index].Comments,
   //        time_zones->Zone[zone_index].Standard_Offset,
   //        time_zones->Zone[zone_index].Standard_Offset_Text,
   //        time_zones->Zone[zone_index].Year_Begin,
   //        time_zones->Zone[zone_index].Year_End);*/
   //    if (time_zones->Zone[zone_index].DST_Effect == TRUE)
   //    {
   //        fprintf(temp, "%s | %s\n(UTC %s - DST %s)\n\n",
   //            time_zones->Zone[zone_index].TZ_Identifier,
   //            time_zones->Zone[zone_index].Country_Name,
   //            time_zones->Zone[zone_index].Standard_Offset_Text,
   //            time_zones->Zone[zone_index].DST_Offset_Text);
   //    }
   //    else
   //    {
   //        fprintf(temp, "%s | %s\n(UTC %s)\n\n",
   //            time_zones->Zone[zone_index].TZ_Identifier,
   //            time_zones->Zone[zone_index].Country_Name,
   //            time_zones->Zone[zone_index].Standard_Offset_Text);
   //    }
   //    
   //}
   //fclose(temp);