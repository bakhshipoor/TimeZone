#include "Creator.h"

CONST CHAR* folderPath = "timezone_database";
CHAR* header_file_name = "timezone_database/timezone_database.h";
CHAR* c_file_name = "timezone_database/timezone_database.c";

VOID Create_Database(CONST CHAR** data_folder_path)
{
    if (!CreateDirectoryA(folderPath, NULL))
    {
        if (ERROR_ALREADY_EXISTS != GetLastError()) 
        {
            printf("Error when Create Directory!");
            return;
        }
    }

    Time_Zones_t* tz = Generate_Data(data_folder_path);
    if (tz == NULL)
    {
        printf("Error when Generatin Data!");
        return;
    }

    Create_Time_Zone_Database_Header_File(tz);
    Create_Time_Zone_Database_C_File(tz);
}

VOID Create_Time_Zone_Database_Header_File(Time_Zones_t* tz)
{
    FILE* header_file = fopen(header_file_name, "w");
    if (header_file == NULL) 
    {
        printf("Error when Create Header File!");
        return;
    }

    fprintf(header_file, "// This code uses the IANA Time Zone Database for accurate time zone handling.\n");
    fprintf(header_file, "// \n");
    fprintf(header_file, "// References: \n");
    fprintf(header_file, "// - Database: https://www.iana.org/time-zones \n");
    fprintf(header_file, "// - Library: https://github.com/eggert/tz \n");
    fprintf(header_file, "// - For more details: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones \n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifndef TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "#define  TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "extern \"C\" {\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#include <stdint.h>\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_VERSION_MAJOR                 %d\n",tz->Version.Major);
    fprintf(header_file, "#define TZDB_VERSION_MINOR                 \"%s\"\n",tz->Version.Minor);
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "// Enforces strict memory alignment for data, preventing potential performance issues \n");
    fprintf(header_file, "// and unexpected behavior in microcontroller environments. \n");
    fprintf(header_file, "// When using compiler optimizations like -Os, data may not be naturally aligned \n");
    fprintf(header_file, "// to word boundaries, leading to inefficient memory access. \n");
    fprintf(header_file, "// This macro provides a convenient way to specify alignment attributes \n");
    fprintf(header_file, "// for variables and data structures. \n");
    fprintf(header_file, "// For information on memory layout and section placement, see LinkerScript.ld. \n");
    fprintf(header_file, "// Example: \n");
    fprintf(header_file, "//   __attribute__((section(\"ExtFlashSection\"))) __attribute__((aligned(0x4))) uint64_t myAlignedData; \n");
    fprintf(header_file, "#define ATTRIBUTE_MEM_ALIGN           \n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_ZONES_INFO_COUNT         %d\n", tz->Zones_Count);
    fprintf(header_file, "#define TZDB_ZONES_DATA_COUNT         %d\n", tz->Zones_Data_Count);
    fprintf(header_file, "#define TZDB_RULES_INFO_COUNT         %d\n", tz->Rules_Count);
    fprintf(header_file, "#define TZDB_RULES_DATA_COUNT         %d\n", tz->Rules_Data_Count);
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef struct\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        uint8_t* time_zone_id;\n");
    fprintf(header_file, "        uint8_t* time_zone_identifier;\n");
    fprintf(header_file, "        uint8_t* country_code;\n");
    fprintf(header_file, "        uint8_t* country_name;\n");
    fprintf(header_file, "        uint8_t* latitude;\n");
    fprintf(header_file, "        uint8_t* longitude;\n");
    fprintf(header_file, "        uint8_t* has_data;\n");
    fprintf(header_file, "        uint8_t* linked_tz_identifier;\n");
    fprintf(header_file, "        uint8_t* data_count;\n");
    fprintf(header_file, "        uint8_t* year_begin;\n");
    fprintf(header_file, "        uint8_t* year_end;\n");
    fprintf(header_file, "        uint8_t* comments;\n");
    fprintf(header_file, "    } tzdb_zone_info_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    extern const ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_info[TZDB_ZONES_INFO_COUNT];\n");
    fprintf(header_file, "    extern const ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_data[TZDB_ZONES_DATA_COUNT];\n");
    fprintf(header_file, "    extern const ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_rules_info[TZDB_RULES_INFO_COUNT];\n");
    fprintf(header_file, "    extern const ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_rules_data[TZDB_RULES_DATA_COUNT];\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "}\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "#endif /* TIME_ZONE_DATABASE_H */\n");

    fclose(header_file);
    
}

VOID Create_Time_Zone_Database_C_File(Time_Zones_t* tz)
{
    Zones_Info_Lenght_t tz_info_lenght = { 0 };
    Zones_Info_String_t* tz_info = Convert_Time_Zone_Info_To_String(tz, &tz_info_lenght);
    
    FILE* c_file = fopen(c_file_name, "w");
    if (c_file == NULL)
    {
        printf("Error when Create C File!");
        return;
    }

    fprintf(c_file, "\n");
    fprintf(c_file, "#include \"TimeZone_Database.h\"\n");
    fprintf(c_file, "\n");
    fprintf(c_file, "const ATTRIBUTE_MEM_ALIGN TZDB_Zone_Info_t TZDB_Zones_Info[ZONES_INFO_COUNT] = {\n");
    for (COUNTER tz_info_index = 0; tz_info_index < tz->Zones_Count; tz_info_index++)
    {
        fprintf(c_file, "    { ");
        for (COUNTER field_index = 0; field_index < 12; field_index++)
        {
            fprintf(c_file, "\"%s\",%s",
                tz_info[tz_info_index].Time_Zone_Info[field_index],
                Print_Space(strlen(tz_info[tz_info_index].Time_Zone_Info[field_index]),tz_info_lenght.Time_Zone_Info[field_index])
            );
        }
        fprintf(c_file, " },\n");
    }
    fprintf(c_file, "};\n");




    fclose(c_file);

}

Zones_Info_String_t* Convert_Time_Zone_Info_To_String(Time_Zones_t* tz, Zones_Info_Lenght_t* tz_info_lenght)
{
    for (COUNTER field_index = 0; field_index < 12; field_index++)
    {
        tz_info_lenght->Time_Zone_Info[field_index] = 0;
    }

    Zones_Info_String_t* tz_info = (Zones_Info_String_t*)calloc(tz->Zones_Count + 1, sizeof(Zones_Info_String_t));
    if (tz_info == NULL)
    {
        return NULL;
    }

    for (COUNTER tz_info_index = 0; tz_info_index < tz->Zones_Count; tz_info_index++)
    {
        for (COUNTER field_index = 0; field_index < 12; field_index++)
        {
            tz_info[tz_info_index].Time_Zone_Info[field_index] = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
            if (tz_info[tz_info_index].Time_Zone_Info[field_index] == NULL)
            {
                return NULL;
            }
        }

        sprintf(tz_info[tz_info_index].Time_Zone_Info[0], "%d", tz->Zones_Info[tz_info_index].Time_Zone_ID);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[1], "%s", tz->Zones_Info[tz_info_index].Time_Zone_Identifier);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[2], "%s", tz->Zones_Info[tz_info_index].Country_Code);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[3], "%s", tz->Zones_Info[tz_info_index].Country_Name);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[4], "%.06llf", tz->Zones_Info[tz_info_index].Latitude);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[5], "%.06llf", tz->Zones_Info[tz_info_index].Longitude);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[6], "%d", tz->Zones_Info[tz_info_index].Has_Data);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[7], "%s", tz->Zones_Info[tz_info_index].Linked_TZ_Identifier);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[8], "%d", tz->Zones_Info[tz_info_index].Data_Count);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[9], "%d", tz->Zones_Info[tz_info_index].Year_Begin);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[10], "%d", tz->Zones_Info[tz_info_index].Year_End);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[11], "%s", tz->Zones_Info[tz_info_index].Comments);

        for (COUNTER field_index = 0; field_index < 12; field_index++)
        {
            if (strlen(tz_info[tz_info_index].Time_Zone_Info[field_index]) > tz_info_lenght->Time_Zone_Info[field_index])
            {
                tz_info_lenght->Time_Zone_Info[field_index] = strlen(tz_info[tz_info_index].Time_Zone_Info[field_index]);
            }
        }
    }
    return tz_info;
}

CHAR* Print_Space(COUNTER str_lenght, COUNTER max_lenght)
{
    COUNTER count = max_lenght - str_lenght + 2;
    CHAR* space = (CHAR*)calloc(count + 1, sizeof(CHAR));
    if (space == NULL)
    {
        return " ";
    }
    for (COUNTER ch_index = 0; ch_index < count; ch_index++)
    {
        space[ch_index] = ' ';
    }
    return space;
}