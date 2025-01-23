#include "../Inc/Creator.h"

CONST CHAR* folderPath = "TimeZone_Database";
CHAR* header_file_name = "TimeZone_Database/TimeZone_Database.h";
CHAR* c_file_name = "TimeZone_Database/TimeZone_Database.c";

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
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifndef TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "#define  TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "extern \"C\" {\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "// Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).\n");
    fprintf(header_file, "// E.g. __attribute__((section(\"ExtFlashSection\"))) __attribute__((aligned(4)))\n");
    fprintf(header_file, "#define ATTRIBUTE_MEM_ALIGN    \n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#define ZONES_INFO_COUNT       %d\n", tz->Zones_Count);
    fprintf(header_file, "#define ZONES_DATA_COUNT       %d\n", tz->Zones_Data_Count);
    fprintf(header_file, "#define RULES_INFO_COUNT       %d\n", tz->Rules_Count);
    fprintf(header_file, "#define RULES_DATA_COUNT       %d\n", tz->Rules_Data_Count);
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "  typedef struct\n");
    fprintf(header_file, "  {\n");
    fprintf(header_file, "      char* Time_Zone_ID;\n");
    fprintf(header_file, "      char* Time_Zone_Identifier\n");
    fprintf(header_file, "      char* Country_Code;\n");
    fprintf(header_file, "      char* Country_Name;\n");
    fprintf(header_file, "      char* Latitude;\n");
    fprintf(header_file, "      char* Longitude;\n");
    fprintf(header_file, "      char* Has_Data;\n");
    fprintf(header_file, "      char* Linked_TZ_Identifier;\n");
    fprintf(header_file, "      char* Data_Count;\n");
    fprintf(header_file, "      char* Year_Begin;\n");
    fprintf(header_file, "      char* Year_End;\n");
    fprintf(header_file, "      char* Comments;\n");
    fprintf(header_file, "  } TZDB_Zone_Info_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "  extern const ATTRIBUTE_MEM_ALIGN TZDB_Zone_Info_t TZDB_Zones_Info[ZONES_INFO_COUNT];\n");
    fprintf(header_file, "  extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Zones_Data[ZONES_DATA_COUNT];\n");
    fprintf(header_file, "  extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Rules_Info[RULES_INFO_COUNT];\n");
    fprintf(header_file, "  extern const ATTRIBUTE_MEM_ALIGN char* TZDB_Rules_Data[RULES_DATA_COUNT];\n");
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
        sprintf(tz_info[tz_info_index].Time_Zone_Info[4], "%.010llf", tz->Zones_Info[tz_info_index].Latitude);
        sprintf(tz_info[tz_info_index].Time_Zone_Info[5], "%.010llf", tz->Zones_Info[tz_info_index].Longitude);
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