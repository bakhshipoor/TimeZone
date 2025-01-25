#ifndef CREATOR_H
#define CREATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../Generator/Generator.h"

#define ZONE_INFO_FIELDS_COUNT 11
#define ZONE_DATA_FIELDS_COUNT 8
#define RULE_INFO_FIELDS_COUNT 5
#define RULE_DATA_FIELDS_COUNT 12

    typedef struct
    {
        CHAR* Zones_Info[ZONE_INFO_FIELDS_COUNT];
    }Zones_Info_String_t;

    typedef struct
    {
        LENGHT Zones_Info[ZONE_INFO_FIELDS_COUNT];
    }Zones_Info_Lenght_t;

    typedef struct
    {
        CHAR* Zones_Data[ZONE_DATA_FIELDS_COUNT];
    }Zones_Data_String_t;

    typedef struct
    {
        LENGHT Zones_Data[ZONE_DATA_FIELDS_COUNT];
    }Zones_Data_Lenght_t;

    typedef struct
    {
        CHAR* Rules_Info[RULE_INFO_FIELDS_COUNT];
    }Rules_Info_String_t;

    typedef struct
    {
        LENGHT Rules_Info[RULE_INFO_FIELDS_COUNT];
    }Rules_Info_Lenght_t;

    typedef struct
    {
        CHAR* Rules_Data[RULE_DATA_FIELDS_COUNT];
    }Rules_Data_String_t;

    typedef struct
    {
        LENGHT Rules_Data[RULE_DATA_FIELDS_COUNT];
    }Rules_Data_Lenght_t;


    VOID Create_Database(CONST CHAR** data_folder_path);
    VOID Create_Time_Zone_Database_Header_File(Time_Zones_t* tz);
    VOID Create_Time_Zone_Database_C_File(Time_Zones_t* tz);

    Zones_Info_String_t* Convert_Zones_Info_To_String(Time_Zones_t* tz, Zones_Info_Lenght_t* tz_info_lenght);
    Zones_Data_String_t* Convert_Zones_Data_To_String(Time_Zones_t* tz, Zones_Data_Lenght_t* tz_data_lenght);
    Rules_Info_String_t* Convert_Rules_Info_To_String(Time_Zones_t* tz, Rules_Info_Lenght_t* rules_info_lenght);
    Rules_Data_String_t* Convert_Rules_Data_To_String(Time_Zones_t* tz, Rules_Data_Lenght_t* rules_data_lenght);
    
    CHAR* Print_Space(COUNTER str_lenght, COUNTER max_lenght);


#ifdef __cplusplus
}
#endif

#endif /* CREATOR_H */


