#include <windows.h>
#include "../Inc/Creator.h"

CONST CHAR* folderPath = "TimeZone_Database";
CHAR* header_file_name = "TimeZone_Database/TimeZone_Database.h";

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

    Create_Time_Zone_Database_Header_File(&tz);

}

VOID Create_Time_Zone_Database_Header_File(Time_Zones_t** tz)
{
    FILE* header_file = fopen(header_file_name, "w");
    if (header_file == NULL) 
    {
        printf("Error when Create Header File!");
        return;
    }

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
    fprintf(header_file, "#define ZONES_INFO_COUNT       %d\n", (*tz)->Zones_Count);
    fprintf(header_file, "#define ZONES_DATA_COUNT       %d\n", (*tz)->Zones_Data_Count);
    fprintf(header_file, "#define RULES_INFO_COUNT       %d\n", (*tz)->Rules_Count);
    fprintf(header_file, "#define RULES_DATA_COUNT       %d\n", (*tz)->Rules_Data_Count);
    fprintf(header_file, "\n");
    fprintf(header_file, "extern const ATTRIBUTE_MEM_ALIGN char* Zones_Info[ZONES_INFO_COUNT];\n");
    fprintf(header_file, "extern const ATTRIBUTE_MEM_ALIGN char* Zones_Data[ZONES_DATA_COUNT];\n");
    fprintf(header_file, "extern const ATTRIBUTE_MEM_ALIGN char* Rules_Info[RULES_INFO_COUNT];\n");
    fprintf(header_file, "extern const ATTRIBUTE_MEM_ALIGN char* Rules_Data[RULES_DATA_COUNT];\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "}\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "#endif /* TIME_ZONE_DATABASE_H */\n");

    fclose(header_file);
    
}