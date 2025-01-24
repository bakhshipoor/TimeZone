#ifndef CREATOR_H
#define CREATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../Generator/Generator.h"

    typedef struct
    {
        CHAR* Time_Zone_Info[12];
    }Zones_Info_String_t;

    typedef struct
    {
        LENGHT Time_Zone_Info[12];
    }Zones_Info_Lenght_t;


    VOID Create_Database(CONST CHAR** data_folder_path);
    VOID Create_Time_Zone_Database_Header_File(Time_Zones_t* tz);
    VOID Create_Time_Zone_Database_C_File(Time_Zones_t* tz);

    Zones_Info_String_t* Convert_Time_Zone_Info_To_String(Time_Zones_t* tz, Zones_Info_Lenght_t* tz_info_lenght);
    
    CHAR* Print_Space(COUNTER str_lenght, COUNTER max_lenght);


#ifdef __cplusplus
}
#endif

#endif /* CREATOR_H */