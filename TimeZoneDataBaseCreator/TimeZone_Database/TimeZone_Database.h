
#ifndef TIME_ZONE_DATABASE_H
#define  TIME_ZONE_DATABASE_H
#ifdef __cplusplus
extern "C" {
#endif

    // Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
    // E.g. __attribute__((section("ExtFlashSection"))) __attribute__((aligned(4)))
#define ATTRIBUTE_MEM_ALIGN    

#define ZONES_INFO_COUNT       417
#define ZONES_DATA_COUNT       1942
#define RULES_INFO_COUNT       134
#define RULES_DATA_COUNT       2101


    typedef struct
    {
        char* Time_Zone_ID;
        char* Time_Zone_Identifier;
        char* Country_Code;
        char* Country_Name;
        char* Latitude;
        char* Longitude;
        char* Has_Data;
        char* Linked_TZ_Identifier;
        char* Data_Count;
        char* Year_Begin;
        char* Year_End;
        char* Comments;
    } TZDB_Zone_Info_t;





    extern const ATTRIBUTE_MEM_ALIGN TZDB_Zone_Info_t TZDB_Zones_Info[ZONES_INFO_COUNT];
    extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Zones_Data[ZONES_DATA_COUNT];
    extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Rules_Info[RULES_INFO_COUNT];
    extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Rules_Data[RULES_DATA_COUNT];

#ifdef __cplusplus
}
#endif
#endif /* TIME_ZONE_DATABASE_H */
