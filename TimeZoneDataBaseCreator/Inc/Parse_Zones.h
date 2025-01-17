#ifndef PARSE_ZONES_H
#define PARSE_ZONES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TimeZoneDBCreator.h"
#include "Parse_Common.h"

#define MAX_LENGHT_DATA_FIELD 256

    typedef struct
    {
        uint8_t Field[MAX_LENGHT_DATA_FIELD];   
        uint8_t Name[MAX_LENGHT_DATA_FIELD];   
        uint8_t Standard_Offset[MAX_LENGHT_DATA_FIELD];    
        uint8_t Rules[MAX_LENGHT_DATA_FIELD];      
        uint8_t Format[MAX_LENGHT_DATA_FIELD];
        uint8_t Until[MAX_LENGHT_DATA_FIELD];      
        uint8_t Comment[MAX_LENGHT_DATA_FIELD];
    } Zone_Data_t;

    typedef struct
    {
        uint32_t Hour;          
        bool Hour_isUTC;       
    } Zone_Info_Until_Hour_t;

    typedef struct
    {
        int16_t Year;
        uint8_t Month;
        uint8_t Day;
        Zone_Info_Until_Hour_t Hour;
    } Zone_Info_Until_t;

    typedef struct
    {
        bool Has_Rule;
        uint8_t* Rule_Name;
        int32_t Save_Hour;
    } Zone_Info_Rule_t;

    typedef struct
    {
        int32_t Standard_Offset;
        Zone_Info_Rule_t Rule;
        uint8_t* Format;
        Zone_Info_Until_Hour_t Until;
        uint8_t* Comment;
    } Zone_Info_t;

    typedef struct
    {
        uint8_t* Name;            
        uint32_t Info_Count;    
        Zone_Info_t* Info;
        int16_t Year_Begin;      
        int16_t Year_End;      
    } Zone_Entry_t;

    Zone_Entry_t* Parse_Zones(int32_t* rules_Count);
    Zone_Data_t Parse_Zone_Data(const char* line);


#ifdef __cplusplus
}
#endif

#endif