#include "Creator.h"
#include "math.h"


CONST CHAR* folderPath = "timezone_database";
CHAR* header_file_name = "timezone_database/timezone_database.h";
CHAR* c_file_name = "timezone_database/timezone_database.c";
CHAR* zone_info_bin_file_name = "timezone_database/tz_zi.txt";
CHAR* zone_data_bin_file_name = "timezone_database/tz_zd.txt";
CHAR* rule_info_bin_file_name = "timezone_database/tz_ri.txt";
CHAR* rule_data_bin_file_name = "timezone_database/tz_rd.txt";


Zones_Info_Lenght_t zones_info_lenght; 
Zones_Info_String_t* zones_info;
CONST CHAR ZI_Comments[ZONE_INFO_FIELDS_COUNT][25] = {
    "Zone_ID",
    "Zone_Identifier",
    "Current_STD_Offset",
    "Current_DST_Offset",
    "Country_Code",
    "Country_Name",
    "Latitude",
    "Longitude",
    "Linked_Zone_ID",
    "Data_Count",
    "Year_Begin",
    "Year_End",
    "Comments"
};
COUNTER ZI_Comments_Lenght[ZONE_INFO_FIELDS_COUNT];

Zones_Data_Lenght_t zones_data_lenght;
Zones_Data_String_t* zones_data;
CONST CHAR ZD_Comments[ZONE_DATA_FIELDS_COUNT][25] = {
    "Zone_ID",
    "Standard_Offset",
    "Rule_ID",
    "Save_Hour",
    "Format",
    "Until_JD",
    "Comments"
};
COUNTER ZD_Comments_Lenght[ZONE_DATA_FIELDS_COUNT];

Rules_Info_Lenght_t rules_info_lenght;
Rules_Info_String_t* rules_info;
CONST CHAR RI_Comments[RULE_INFO_FIELDS_COUNT][25] = {
    "Rule_ID",
    "Rule_Name",
    "Data_Count",
    "Year_Begin",
    "Year_End"
};
COUNTER RI_Comments_Lenght[RULE_INFO_FIELDS_COUNT];

Rules_Data_Lenght_t rules_data_lenght;
Rules_Data_String_t* rules_data;
CONST CHAR RD_Comments[RULE_DATA_FIELDS_COUNT][35] = {
    "Rule_ID",
    "From",
    "To",
    "Month",
    "Day",
    "Weekday",
    "Weekday_IsAfterOrEqual_Day",
    "Hour",
    "Hour_isUTC",
    "Save_Hour",
    "Letter",
    "Comments"
};
COUNTER RD_Comments_Lenght[RULE_DATA_FIELDS_COUNT];

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

    zones_info = Convert_Zones_Info_To_String(tz, &zones_info_lenght);

    zones_data = Convert_Zones_Data_To_String(tz, &zones_data_lenght);

    rules_info = Convert_Rules_Info_To_String(tz, &rules_info_lenght);

    rules_data = Convert_Rules_Data_To_String(tz, &rules_data_lenght);

    Create_Time_Zone_Database_Header_File(tz);
    Create_Time_Zone_Database_C_File(tz);

    Create_Time_Zone_Database_Bin_Files(tz);

    Free_TZ_Data(tz);
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
    fprintf(header_file, "// - For more details: https://en.wikipedia.org/wiki/List_of_zones_database_time_zones \n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifndef TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "#define  TIME_ZONE_DATABASE_H\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "extern \"C\" {\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#include <stdint.h>\n");
    fprintf(header_file, "#include <stdbool.h>\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_VERSION_MAJOR                    %d\n",tz->Version.Major);
    fprintf(header_file, "#define TZDB_VERSION_MINOR                    \"%s\"\n",tz->Version.Minor);
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
    fprintf(header_file, "#define TZDB_ATTRIBUTE_MEM_ALIGN              \n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_ZONES_INFO_COUNT                 %d\n", tz->Zones_Count);
    fprintf(header_file, "#define TZDB_ZONES_DATA_COUNT                 %d\n", tz->Zones_Data_Count);
    fprintf(header_file, "#define TZDB_RULES_INFO_COUNT                 %d\n", tz->Rules_Count);
    fprintf(header_file, "#define TZDB_RULES_DATA_COUNT                 %d\n", tz->Rules_Data_Count);
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZI_IDENTIFIER         %d\n", zones_info_lenght.Zones_Info[1] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZI_COUNTRY_CODE       %d\n", zones_info_lenght.Zones_Info[4] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZI_COUNTRY_NAME       %d\n", zones_info_lenght.Zones_Info[5] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZI_COMMENTS           %d\n", zones_info_lenght.Zones_Info[12] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZD_FORMAT             %d\n", zones_data_lenght.Zones_Data[4] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_ZD_COMMENTS           %d\n", zones_data_lenght.Zones_Data[6] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_RI_NAME               %d\n", rules_info_lenght.Rules_Info[1] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_RD_LETTER             %d\n", rules_data_lenght.Rules_Data[10] + 1);
    fprintf(header_file, "#define TZDB_MAX_LENGHT_RD_COMMENTS           %d\n", rules_data_lenght.Rules_Data[11] + 1);
    fprintf(header_file, "\n");
    fprintf(header_file, "#define TZDB_YEAR_END_MAX                     -1\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef enum\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        TZDB_WEEKDAY_SUNDAY = 0,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_MONDAY,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_TUESDAY,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_WEDNESDAY,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_THURSDAY,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_FRIDAY,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_SATURDAY,\n");
    fprintf(header_file, "        \n");
    fprintf(header_file, "        TZDB_WEEKDAY_TOTAL,\n");
    fprintf(header_file, "        TZDB_WEEKDAY_NONE = 255,\n");
    fprintf(header_file, "    } tzdb_weekday_number_e;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef enum\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        TZDB_MONTH_NONE = 0,\n");
    fprintf(header_file, "        TZDB_MONTH_JANUARY,\n");
    fprintf(header_file, "        TZDB_MONTH_FEBRUARY,\n");
    fprintf(header_file, "        TZDB_MONTH_MARCH,\n");
    fprintf(header_file, "        TZDB_MONTH_APRIL,\n");
    fprintf(header_file, "        TZDB_MONTH_MAY,\n");
    fprintf(header_file, "        TZDB_MONTH_JUNE,\n");
    fprintf(header_file, "        TZDB_MONTH_JULY,\n");
    fprintf(header_file, "        TZDB_MONTH_AUGUST,\n");
    fprintf(header_file, "        TZDB_MONTH_SEPTEMBER,\n");
    fprintf(header_file, "        TZDB_MONTH_OCTOBER,\n");
    fprintf(header_file, "        TZDB_MONTH_NOVEMBER,\n");
    fprintf(header_file, "        TZDB_MONTH_DECEMBER,\n");
    fprintf(header_file, "        \n");
    fprintf(header_file, "        TZDB_MONTH_TOTAL = TZDB_MONTH_DECEMBER,\n");
    fprintf(header_file, "    } tzdb_month_number_e;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef struct\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        int32_t       zone_id;\n");
    fprintf(header_file, "        uint8_t       zone_identifier[TZDB_MAX_LENGHT_ZI_IDENTIFIER];\n");
    fprintf(header_file, "        int64_t       std_offset;\n");
    fprintf(header_file, "        int64_t       dst_offset;\n");
    fprintf(header_file, "        uint8_t       country_code[TZDB_MAX_LENGHT_ZI_COUNTRY_CODE];\n");
    fprintf(header_file, "        uint8_t       country_name[TZDB_MAX_LENGHT_ZI_COUNTRY_NAME];\n");
    fprintf(header_file, "        double        latitude;\n");
    fprintf(header_file, "        double        longitude;\n");
    fprintf(header_file, "        int32_t       linked_zone_id;\n");
    fprintf(header_file, "        int32_t       data_count;\n");
    fprintf(header_file, "        int32_t       year_begin;\n");
    fprintf(header_file, "        int32_t       year_end;\n");
    fprintf(header_file, "        uint8_t       comments[TZDB_MAX_LENGHT_ZI_COMMENTS];\n");
    fprintf(header_file, "    } tzdb_zone_info_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef struct\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        int32_t       zone_id;\n");
    fprintf(header_file, "        int64_t       standard_offset;\n");
    fprintf(header_file, "        int32_t       rule_id;\n");
    fprintf(header_file, "        int64_t       save_hour;\n");
    fprintf(header_file, "        uint8_t       format[TZDB_MAX_LENGHT_ZD_FORMAT];\n");
    fprintf(header_file, "        double        until_jd;\n");
    fprintf(header_file, "        uint8_t       comments[TZDB_MAX_LENGHT_ZD_COMMENTS];\n");
    fprintf(header_file, "    } tzdb_zone_data_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef struct\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        int32_t       rule_id;\n");
    fprintf(header_file, "        uint8_t       rule_name[TZDB_MAX_LENGHT_RI_NAME];\n");
    fprintf(header_file, "        int32_t       data_count;\n");
    fprintf(header_file, "        int32_t       year_begin;\n");
    fprintf(header_file, "        int32_t       year_end;\n");
    fprintf(header_file, "    } tzdb_rule_info_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    typedef struct\n");
    fprintf(header_file, "    {\n");
    fprintf(header_file, "        int32_t       rule_id;\n");
    fprintf(header_file, "        int32_t       from;\n");
    fprintf(header_file, "        int32_t       to;\n");
    fprintf(header_file, "        uint8_t       month;\n");
    fprintf(header_file, "        uint8_t       day;\n");
    fprintf(header_file, "        uint8_t       weekday;\n");
    fprintf(header_file, "        bool          weekday_isafterorequal_day;\n");
    fprintf(header_file, "        int64_t       hour;\n");
    fprintf(header_file, "        bool          hour_isUTC;\n");
    fprintf(header_file, "        int64_t       save_hour;\n");
    fprintf(header_file, "        uint8_t       letter[TZDB_MAX_LENGHT_RD_LETTER];\n");
    fprintf(header_file, "        uint8_t       comments[TZDB_MAX_LENGHT_RD_COMMENTS];\n");
    fprintf(header_file, "    } tzdb_rule_data_t;\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_info[TZDB_ZONES_INFO_COUNT];\n");
    fprintf(header_file, "    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_data_t tzdb_zones_data[TZDB_ZONES_DATA_COUNT];\n");
    fprintf(header_file, "    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_info_t tzdb_rules_info[TZDB_RULES_INFO_COUNT];\n");
    fprintf(header_file, "    extern const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_data_t tzdb_rules_data[TZDB_RULES_DATA_COUNT];\n");
    fprintf(header_file, "\n");
    fprintf(header_file, "#ifdef __cplusplus\n");
    fprintf(header_file, "}\n");
    fprintf(header_file, "#endif\n");
    fprintf(header_file, "#endif /* TIME_ZONE_DATABASE_H */\n");

    fclose(header_file);
    
}

VOID Create_Time_Zone_Database_C_File(Time_Zones_t* tz)
{
    
    
    FILE* c_file = fopen(c_file_name, "w");
    if (c_file == NULL)
    {
        printf("Error when Create C File!");
        return;
    }

    int index = 0;
    for (index = 0; index < ZONE_INFO_FIELDS_COUNT; index++)
    {
        ZI_Comments_Lenght[index] = utf8_strlen(ZI_Comments[index]);
    }
    for (index = 0; index < ZONE_DATA_FIELDS_COUNT; index++)
    {
        ZD_Comments_Lenght[index] = utf8_strlen(ZD_Comments[index]);
    }
    for (index = 0; index < RULE_INFO_FIELDS_COUNT; index++)
    {
        RI_Comments_Lenght[index] = utf8_strlen(RI_Comments[index]);
    }
    for (index = 0; index < RULE_DATA_FIELDS_COUNT; index++)
    {
        RD_Comments_Lenght[index] = utf8_strlen(RD_Comments[index]);
    }


    fprintf(c_file, "\n");
    fprintf(c_file, "#include \"timezone_database.h\"\n");
    fprintf(c_file, "\n");

    
    // Zones Info
    fprintf(c_file, "const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_info_t tzdb_zones_info[TZDB_ZONES_INFO_COUNT] = {\n");
    fprintf(c_file, "    /*");
    for (COUNTER filed_index = 0; filed_index < ZONE_INFO_FIELDS_COUNT; filed_index++)
    {
        if (filed_index == 1 || filed_index == 4 || filed_index == 5 || filed_index == 12)
        {
            fprintf(c_file, "%s   %s", ZI_Comments[filed_index], Print_Space(utf8_strlen(ZI_Comments[filed_index]), max(zones_info_lenght.Zones_Info[filed_index], ZI_Comments_Lenght[filed_index])));
        }
        else
        {
            fprintf(c_file, "%s %s", ZI_Comments[filed_index], Print_Space(utf8_strlen(ZI_Comments[filed_index]), max(zones_info_lenght.Zones_Info[filed_index], ZI_Comments_Lenght[filed_index])));
        }
    }
    fprintf(c_file, " */\n");
    for (COUNTER zones_info_index = 0; zones_info_index < tz->Zones_Count; zones_info_index++)
    {
        fprintf(c_file, "    { ");
        // Field 0: time_zone_id
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[0]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[0]), max(zones_info_lenght.Zones_Info[0], ZI_Comments_Lenght[0]))
        );
        // Field 1: time_zone_identifier
        fprintf(c_file, "\"%s\",%s",
            zones_info[zones_info_index].Zones_Info[1],
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[1]), max(zones_info_lenght.Zones_Info[1], ZI_Comments_Lenght[1]))
        );
        // Field 2: std_offset
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[2]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[2]), max(zones_info_lenght.Zones_Info[2], ZI_Comments_Lenght[2]))
        );
        // Field 3: dst_offset
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[3]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[3]), max(zones_info_lenght.Zones_Info[3], ZI_Comments_Lenght[3]))
        );
        // Field 4: country_code
        fprintf(c_file, "\"%s\",%s",
            zones_info[zones_info_index].Zones_Info[4],
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[4]), max(zones_info_lenght.Zones_Info[4], ZI_Comments_Lenght[4]))
        );
        // Field 5: country_name
        fprintf(c_file, "\"%s\",%s",
            zones_info[zones_info_index].Zones_Info[5],
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[5]), max(zones_info_lenght.Zones_Info[5], ZI_Comments_Lenght[5]))
        );
        // Field 6: latitude
        fprintf(c_file, "%.06lf,%s",
            atof(zones_info[zones_info_index].Zones_Info[6]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[6]), max(zones_info_lenght.Zones_Info[6], ZI_Comments_Lenght[6]))
        );
        // Field 7: longitude
        fprintf(c_file, "%.06lf,%s",
            atof(zones_info[zones_info_index].Zones_Info[7]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[7]), max(zones_info_lenght.Zones_Info[7], ZI_Comments_Lenght[7]))
        );
        // Field 8: linked_tz_identifier
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[8]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[8]), max(zones_info_lenght.Zones_Info[8], ZI_Comments_Lenght[8]))
        );
        // Field 9: data_count
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[9]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[9]), max(zones_info_lenght.Zones_Info[9], ZI_Comments_Lenght[9]))
        );
        // Field 10: year_begin
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[10]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[10]), max(zones_info_lenght.Zones_Info[10], ZI_Comments_Lenght[10]))
        );
        // Field 11: year_end
        fprintf(c_file, "%d,%s",
            atoi(zones_info[zones_info_index].Zones_Info[11]),
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[11]), max(zones_info_lenght.Zones_Info[11], ZI_Comments_Lenght[11]))
        );
        // Field 12: comments
        fprintf(c_file, "\"%s\"%s",
            zones_info[zones_info_index].Zones_Info[12],
            Print_Space(utf8_strlen(zones_info[zones_info_index].Zones_Info[12]), max(zones_info_lenght.Zones_Info[12], ZI_Comments_Lenght[12]))
        );
        if (zones_info_index == (tz->Zones_Count - 1))
        {
            fprintf(c_file, "}\n");
        }
        else
        {
            fprintf(c_file, "},\n");
        }
    }
    fprintf(c_file, "};\n");
    fprintf(c_file, "\n");
    fprintf(c_file, "\n");

    // Zones Data
    fprintf(c_file, "const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_zone_data_t tzdb_zones_data[TZDB_ZONES_DATA_COUNT] = {\n");
    fprintf(c_file, "    /*");
    for (COUNTER filed_index = 0; filed_index < ZONE_DATA_FIELDS_COUNT; filed_index++)
    {
        if (filed_index == 4 || filed_index == 6)
        {
            fprintf(c_file, "%s   %s", ZD_Comments[filed_index], Print_Space(utf8_strlen(ZD_Comments[filed_index]), max(zones_data_lenght.Zones_Data[filed_index], ZD_Comments_Lenght[filed_index])));
        }
        else
        {
            fprintf(c_file, "%s %s", ZD_Comments[filed_index], Print_Space(utf8_strlen(ZD_Comments[filed_index]), max(zones_data_lenght.Zones_Data[filed_index], ZD_Comments_Lenght[filed_index])));
        }
    }
    fprintf(c_file, " */\n");
    for (COUNTER zones_data_index = 0; zones_data_index < tz->Zones_Data_Count; zones_data_index++)
    {
        fprintf(c_file, "    { ");
        // Field 0: time_zone_id
        fprintf(c_file, "%d,%s",
            atoi(zones_data[zones_data_index].Zones_Data[0]),
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[0]), max(zones_data_lenght.Zones_Data[0], ZD_Comments_Lenght[0]))
        );
        // Field 1: standard_offset
        fprintf(c_file, "%lld,%s",
            _atoi64(zones_data[zones_data_index].Zones_Data[1]),
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[1]), max(zones_data_lenght.Zones_Data[1], ZD_Comments_Lenght[1]))
        );
        // Field 2: rule_id
        fprintf(c_file, "%d,%s",
            atoi(zones_data[zones_data_index].Zones_Data[2]),
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[2]), max(zones_data_lenght.Zones_Data[2], ZD_Comments_Lenght[2]))
        );
        // Field 3: save_hour
        fprintf(c_file, "%lld,%s",
            _atoi64(zones_data[zones_data_index].Zones_Data[3]),
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[3]), max(zones_data_lenght.Zones_Data[3], ZD_Comments_Lenght[3]))
        );
        // Field 4: format
        fprintf(c_file, "\"%s\",%s",
            zones_data[zones_data_index].Zones_Data[4],
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[4]), max(zones_data_lenght.Zones_Data[4], ZD_Comments_Lenght[4]))
        );
        // Field 5: until_jd
        fprintf(c_file, "%.010lf,%s",
            atof(zones_data[zones_data_index].Zones_Data[5]),
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[5]), max(zones_data_lenght.Zones_Data[5], ZD_Comments_Lenght[5]))
        );
        // Field 6: comments
        fprintf(c_file, "\"%s\"%s",
            zones_data[zones_data_index].Zones_Data[6],
            Print_Space(utf8_strlen(zones_data[zones_data_index].Zones_Data[6]), max(zones_data_lenght.Zones_Data[6], ZD_Comments_Lenght[6]))
        );
        if (zones_data_index == (tz->Zones_Data_Count - 1))
        {
            fprintf(c_file, "}\n");
        }
        else
        {
            fprintf(c_file, "},\n");
        }
    }
    fprintf(c_file, "};\n");
    fprintf(c_file, "\n");
    fprintf(c_file, "\n");

    // Rules Info
    fprintf(c_file, "const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_info_t tzdb_rules_info[TZDB_RULES_INFO_COUNT] = {\n");
    fprintf(c_file, "    /*");
    for (COUNTER filed_index = 0; filed_index < RULE_INFO_FIELDS_COUNT; filed_index++)
    {
        if (filed_index == 1)
        {
            fprintf(c_file, "%s   %s", RI_Comments[filed_index], Print_Space(utf8_strlen(RI_Comments[filed_index]), max(rules_info_lenght.Rules_Info[filed_index], RI_Comments_Lenght[filed_index])));
        }
        else
        {
            fprintf(c_file, "%s %s", RI_Comments[filed_index], Print_Space(utf8_strlen(RI_Comments[filed_index]), max(rules_info_lenght.Rules_Info[filed_index], RI_Comments_Lenght[filed_index])));
        }
    }
    fprintf(c_file, " */\n");
    for (COUNTER rules_info_index = 0; rules_info_index < tz->Rules_Count; rules_info_index++)
    {
        fprintf(c_file, "    { ");
        // Field 0: rule_id
        fprintf(c_file, "%d,%s",
            atoi(rules_info[rules_info_index].Rules_Info[0]),
            Print_Space(utf8_strlen(rules_info[rules_info_index].Rules_Info[0]), max(rules_info_lenght.Rules_Info[0], RI_Comments_Lenght[0]))
        );
        // Field 1: name
        fprintf(c_file, "\"%s\",%s",
            rules_info[rules_info_index].Rules_Info[1],
            Print_Space(utf8_strlen(rules_info[rules_info_index].Rules_Info[1]), max(rules_info_lenght.Rules_Info[1], RI_Comments_Lenght[1]))
        );
        // Field 2: years_count
        fprintf(c_file, "%d,%s",
            atoi(rules_info[rules_info_index].Rules_Info[2]),
            Print_Space(utf8_strlen(rules_info[rules_info_index].Rules_Info[2]), max(rules_info_lenght.Rules_Info[2], RI_Comments_Lenght[2]))
        );
        // Field 3: year_begin
        fprintf(c_file, "%d,%s",
            atoi(rules_info[rules_info_index].Rules_Info[3]),
            Print_Space(utf8_strlen(rules_info[rules_info_index].Rules_Info[3]), max(rules_info_lenght.Rules_Info[3], RI_Comments_Lenght[3]))
        );
        // Field 4: year_end
        fprintf(c_file, "%d%s",
            atoi(rules_info[rules_info_index].Rules_Info[4]),
            Print_Space(utf8_strlen(rules_info[rules_info_index].Rules_Info[4]), max(rules_info_lenght.Rules_Info[4], RI_Comments_Lenght[4]))
        );
        if (rules_info_index == (tz->Rules_Count - 1))
        {
            fprintf(c_file, "}\n");
        }
        else
        {
            fprintf(c_file, "},\n");
        }
    }
    fprintf(c_file, "};\n");
    fprintf(c_file, "\n");
    fprintf(c_file, "\n");

    // Rules Data
    fprintf(c_file, "const TZDB_ATTRIBUTE_MEM_ALIGN tzdb_rule_data_t tzdb_rules_data[TZDB_RULES_DATA_COUNT] = {\n");
    fprintf(c_file, "    /*");
    for (COUNTER filed_index = 0; filed_index < RULE_DATA_FIELDS_COUNT; filed_index++)
    {
        if (filed_index == 10 || filed_index == 11)
        {
            fprintf(c_file, "%s   %s", RD_Comments[filed_index], Print_Space(utf8_strlen(RD_Comments[filed_index]), max(rules_data_lenght.Rules_Data[filed_index], RD_Comments_Lenght[filed_index])));
        }
        else
        {
            fprintf(c_file, "%s %s", RD_Comments[filed_index], Print_Space(utf8_strlen(RD_Comments[filed_index]), max(rules_data_lenght.Rules_Data[filed_index], RD_Comments_Lenght[filed_index])));
        }
    }
    fprintf(c_file, " */\n");
    for (COUNTER rules_data_index = 0; rules_data_index < tz->Rules_Data_Count; rules_data_index++)
    {
        fprintf(c_file, "    { ");
        // Field 0: rule_id
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[0]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[0]), max(rules_data_lenght.Rules_Data[0], RD_Comments_Lenght[0]))
        );
        // Field 1: from
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[1]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[1]), max(rules_data_lenght.Rules_Data[1], RD_Comments_Lenght[1]))
        );
        // Field 2: to
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[2]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[2]), max(rules_data_lenght.Rules_Data[2], RD_Comments_Lenght[2]))
        );
        // Field 3: month
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[3]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[3]), max(rules_data_lenght.Rules_Data[3], RD_Comments_Lenght[3]))
        );
        // Field 4: day
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[4]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[4]), max(rules_data_lenght.Rules_Data[4], RD_Comments_Lenght[4]))
        );
        // Field 5: weekday
        fprintf(c_file, "%d,%s",
            atoi(rules_data[rules_data_index].Rules_Data[5]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[5]), max(rules_data_lenght.Rules_Data[5], RD_Comments_Lenght[5]))
        );
        // Field 6: weekday_isafterorequal_day
        LENGHT bool_len = atoi(rules_data[rules_data_index].Rules_Data[6]) <= 0 ? 5 : 4;
        fprintf(c_file, "%s,%s",
            atoi(rules_data[rules_data_index].Rules_Data[6]) <= 0 ? "false" : "true",
            Print_Space(bool_len, max(RD_Comments_Lenght[6], bool_len))
        );
        // Field 7: hour
        fprintf(c_file, "%lld,%s",
            _atoi64(rules_data[rules_data_index].Rules_Data[7]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[7]), max(rules_data_lenght.Rules_Data[7], RD_Comments_Lenght[7]))
        );
        // Field 8: hour_isUTC
        bool_len = atoi(rules_data[rules_data_index].Rules_Data[8]) <= 0 ? 5 : 4;
        fprintf(c_file, "%s,%s",
            atoi(rules_data[rules_data_index].Rules_Data[8]) <= 0 ? "false" : "true",
            Print_Space(bool_len, max(RD_Comments_Lenght[8], bool_len))
        );
        // Field 9: save_hour
        fprintf(c_file, "%lld,%s",
            _atoi64(rules_data[rules_data_index].Rules_Data[9]),
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[9]), max(rules_data_lenght.Rules_Data[9], RD_Comments_Lenght[9]))
        );
        // Field 10: letter
        fprintf(c_file, "\"%s\",%s",
            rules_data[rules_data_index].Rules_Data[10],
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[10]), max(rules_data_lenght.Rules_Data[10], RD_Comments_Lenght[10]))
        );
        // Field 11: comment
        fprintf(c_file, "\"%s\"%s",
            rules_data[rules_data_index].Rules_Data[11],
            Print_Space(utf8_strlen(rules_data[rules_data_index].Rules_Data[11]), max(rules_data_lenght.Rules_Data[11], RD_Comments_Lenght[11]))
        );
        if (rules_data_index == (tz->Rules_Data_Count-1))
        {
            fprintf(c_file, "}\n");
        }
        else
        {
            fprintf(c_file, "},\n");
        }
    }
    fprintf(c_file, "};\n");

    fprintf(c_file, "\n");
    fprintf(c_file, "\n");

    fclose(c_file);

}

VOID Create_Time_Zone_Database_Bin_Files(Time_Zones_t* tz)
{
    COUNTER index = 0;

    FILE* zone_info_bin_file = fopen(zone_info_bin_file_name, "w");
    if (zone_info_bin_file == NULL)
    {
        printf("Error when Create bin File!");
        return;
    }
    for (index = 0; index < tz->Zones_Count; index++)
    {
        fprintf(zone_info_bin_file,
            "%d\t%s\t%lld\t%lld\t%s\t%s\t%.06lf\t%.06lf\t%d\t%d\t%d\t%d\t%s\n",
            tz->Zones_Info[index].Time_Zone_ID,
            utf8_strlen(tz->Zones_Info[index].Time_Zone_Identifier) == 0 ? "-" : tz->Zones_Info[index].Time_Zone_Identifier,
            tz->Zones_Info[index].STD_Offset,
            tz->Zones_Info[index].DST_Offset,
            utf8_strlen(tz->Zones_Info[index].Country_Code) == 0 ? "-" : tz->Zones_Info[index].Country_Code,
            utf8_strlen(tz->Zones_Info[index].Country_Name) == 0 ? "-" : tz->Zones_Info[index].Country_Name,
            tz->Zones_Info[index].Latitude,
            tz->Zones_Info[index].Longitude,
            tz->Zones_Info[index].Linked_Zone_ID,
            tz->Zones_Info[index].Data_Count,
            tz->Zones_Info[index].Year_Begin,
            tz->Zones_Info[index].Year_End,
            utf8_strlen(tz->Zones_Info[index].Comments) == 0 ? "-" : tz->Zones_Info[index].Comments
        );
        
    }
    fclose(zone_info_bin_file);

    FILE* zone_data_bin_file = fopen(zone_data_bin_file_name, "w");
    if (zone_info_bin_file == NULL)
    {
        printf("Error when Create bin File!");
        return;
    }
    for (index = 0; index < tz->Zones_Data_Count; index++)
    {
        fprintf(zone_data_bin_file,
            "%d\t%lld\t%d\t%lld\t%s\t%.010lf\t%s\n",
            tz->Zones_Data[index].Time_Zone_ID,
            tz->Zones_Data[index].Standard_Offset,
            tz->Zones_Data[index].Rule_ID,
            tz->Zones_Data[index].Save_Hour,
            utf8_strlen(tz->Zones_Data[index].Format) == 0 ? "-" : tz->Zones_Data[index].Format,
            tz->Zones_Data[index].Until_JD,
            utf8_strlen(tz->Zones_Data[index].Comments) == 0 ? "-" : tz->Zones_Data[index].Comments
        );

    }
    fclose(zone_data_bin_file);

    FILE* rule_info_bin_file = fopen(rule_info_bin_file_name, "w");
    if (rule_info_bin_file == NULL)
    {
        printf("Error when Create bin File!");
        return;
    }
    for (index = 0; index < tz->Rules_Count; index++)
    {
        fprintf(rule_info_bin_file,
            "%d\t%s\t%d\t%d\t%d\n",
            tz->Rules_Info[index].Rule_ID,
            utf8_strlen(tz->Rules_Info[index].Name) == 0 ? "-" : tz->Rules_Info[index].Name,
            tz->Rules_Info[index].Data_Count,
            tz->Rules_Info[index].Year_Begin,
            tz->Rules_Info[index].Year_End
        );

    }
    fclose(rule_info_bin_file);

    FILE* rule_data_bin_file = fopen(rule_data_bin_file_name, "w");
    if (rule_data_bin_file == NULL)
    {
        printf("Error when Create bin File!");
        return;
    }
    for (index = 0; index < tz->Rules_Data_Count; index++)
    {
        fprintf(rule_data_bin_file,
            "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%lld\t%d\t%lld\t%s\t%s\n",
            tz->Rules_Data[index].Rule_ID,
            tz->Rules_Data[index].From,
            tz->Rules_Data[index].To,
            tz->Rules_Data[index].Month,
            tz->Rules_Data[index].Day,
            tz->Rules_Data[index].Weekday,
            tz->Rules_Data[index].Weekday_IsAfterOrEqual_Day,
            tz->Rules_Data[index].Hour,
            tz->Rules_Data[index].Hour_isUTC,
            tz->Rules_Data[index].Save_Hour,
            utf8_strlen(tz->Rules_Data[index].Letter) == 0 ? "-" : tz->Rules_Data[index].Letter,
            utf8_strlen(tz->Rules_Data[index].Comments) == 0 ? "-" : tz->Rules_Data[index].Comments
        );

    }
    fclose(rule_data_bin_file);
}

Zones_Info_String_t* Convert_Zones_Info_To_String(Time_Zones_t* tz, Zones_Info_Lenght_t* zones_info_lenght)
{
    for (COUNTER field_index = 0; field_index < ZONE_INFO_FIELDS_COUNT; field_index++)
    {
        zones_info_lenght->Zones_Info[field_index] = 0;
    }

    Zones_Info_String_t* zones_info = (Zones_Info_String_t*)calloc(tz->Zones_Count + 1, sizeof(Zones_Info_String_t));
    if (zones_info == NULL)
    {
        return NULL;
    }

    for (COUNTER zones_info_index = 0; zones_info_index < tz->Zones_Count; zones_info_index++)
    {
        for (COUNTER field_index = 0; field_index < ZONE_INFO_FIELDS_COUNT; field_index++)
        {
            zones_info[zones_info_index].Zones_Info[field_index] = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
            if (zones_info[zones_info_index].Zones_Info[field_index] == NULL)
            {
                return NULL;
            }
        }

        sprintf(zones_info[zones_info_index].Zones_Info[0], "%d", tz->Zones_Info[zones_info_index].Time_Zone_ID);
        sprintf(zones_info[zones_info_index].Zones_Info[1], "%s", tz->Zones_Info[zones_info_index].Time_Zone_Identifier);
        sprintf(zones_info[zones_info_index].Zones_Info[2], "%lld", tz->Zones_Info[zones_info_index].STD_Offset);
        sprintf(zones_info[zones_info_index].Zones_Info[3], "%lld", tz->Zones_Info[zones_info_index].DST_Offset);
        sprintf(zones_info[zones_info_index].Zones_Info[4], "%s", tz->Zones_Info[zones_info_index].Country_Code);
        sprintf(zones_info[zones_info_index].Zones_Info[5], "%s", tz->Zones_Info[zones_info_index].Country_Name);
        sprintf(zones_info[zones_info_index].Zones_Info[6], "%.06lf", tz->Zones_Info[zones_info_index].Latitude);
        sprintf(zones_info[zones_info_index].Zones_Info[7], "%.06lf", tz->Zones_Info[zones_info_index].Longitude);
        sprintf(zones_info[zones_info_index].Zones_Info[8], "%d", tz->Zones_Info[zones_info_index].Linked_Zone_ID);
        sprintf(zones_info[zones_info_index].Zones_Info[9], "%d", tz->Zones_Info[zones_info_index].Data_Count);
        sprintf(zones_info[zones_info_index].Zones_Info[10], "%d", tz->Zones_Info[zones_info_index].Year_Begin);
        sprintf(zones_info[zones_info_index].Zones_Info[11], "%d", tz->Zones_Info[zones_info_index].Year_End);
        sprintf(zones_info[zones_info_index].Zones_Info[12], "%s", tz->Zones_Info[zones_info_index].Comments);

        for (COUNTER field_index = 0; field_index < ZONE_INFO_FIELDS_COUNT; field_index++)
        {
            COUNTER str_len = utf8_strlen(zones_info[zones_info_index].Zones_Info[field_index]);
            if (str_len  > zones_info_lenght->Zones_Info[field_index])
            {
                zones_info_lenght->Zones_Info[field_index] = str_len;
            }
        }
    }
    return zones_info;
}

Zones_Data_String_t* Convert_Zones_Data_To_String(Time_Zones_t* tz, Zones_Data_Lenght_t* zones_data_lenght)
{
    for (COUNTER field_index = 0; field_index < ZONE_DATA_FIELDS_COUNT; field_index++)
    {
        zones_data_lenght->Zones_Data[field_index] = 0;
    }

    Zones_Data_String_t* zones_data = (Zones_Data_String_t*)calloc(tz->Zones_Data_Count + 1, sizeof(Zones_Data_String_t));
    if (zones_data == NULL)
    {
        return NULL;
    }

    for (COUNTER zones_data_index = 0; zones_data_index < tz->Zones_Data_Count; zones_data_index++)
    {
        for (COUNTER field_index = 0; field_index < ZONE_DATA_FIELDS_COUNT; field_index++)
        {
            zones_data[zones_data_index].Zones_Data[field_index] = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
            if (zones_data[zones_data_index].Zones_Data[field_index] == NULL)
            {
                return NULL;
            }
        }

        sprintf(zones_data[zones_data_index].Zones_Data[0], "%d", tz->Zones_Data[zones_data_index].Time_Zone_ID);
        sprintf(zones_data[zones_data_index].Zones_Data[1], "%lld", tz->Zones_Data[zones_data_index].Standard_Offset);
        sprintf(zones_data[zones_data_index].Zones_Data[2], "%d", tz->Zones_Data[zones_data_index].Rule_ID);
        sprintf(zones_data[zones_data_index].Zones_Data[3], "%lld", tz->Zones_Data[zones_data_index].Save_Hour);
        sprintf(zones_data[zones_data_index].Zones_Data[4], "%s", tz->Zones_Data[zones_data_index].Format);
        sprintf(zones_data[zones_data_index].Zones_Data[5], "%.010lf", tz->Zones_Data[zones_data_index].Until_JD);
        sprintf(zones_data[zones_data_index].Zones_Data[6], "%s", tz->Zones_Data[zones_data_index].Comments);

        for (COUNTER field_index = 0; field_index < ZONE_DATA_FIELDS_COUNT; field_index++)
        {
            if (utf8_strlen(zones_data[zones_data_index].Zones_Data[field_index]) > zones_data_lenght->Zones_Data[field_index])
            {
                zones_data_lenght->Zones_Data[field_index] = utf8_strlen(zones_data[zones_data_index].Zones_Data[field_index]);
            }
        }
    }
    return zones_data;
}

Rules_Info_String_t* Convert_Rules_Info_To_String(Time_Zones_t* tz, Rules_Info_Lenght_t* rules_info_lenght)
{
    for (COUNTER field_index = 0; field_index < RULE_INFO_FIELDS_COUNT; field_index++)
    {
        rules_info_lenght->Rules_Info[field_index] = 0;
    }

    Rules_Info_String_t* rules_info = (Rules_Info_String_t*)calloc(tz->Rules_Count + 1, sizeof(Rules_Info_String_t));
    if (rules_info == NULL)
    {
        return NULL;
    }

    for (COUNTER rules_info_index = 0; rules_info_index < tz->Rules_Count; rules_info_index++)
    {
        for (COUNTER field_index = 0; field_index < RULE_INFO_FIELDS_COUNT; field_index++)
        {
            rules_info[rules_info_index].Rules_Info[field_index] = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
            if (rules_info[rules_info_index].Rules_Info[field_index] == NULL)
            {
                return NULL;
            }
        }

        sprintf(rules_info[rules_info_index].Rules_Info[0], "%d", tz->Rules_Info[rules_info_index].Rule_ID);
        sprintf(rules_info[rules_info_index].Rules_Info[1], "%s", tz->Rules_Info[rules_info_index].Name);
        sprintf(rules_info[rules_info_index].Rules_Info[2], "%d", tz->Rules_Info[rules_info_index].Data_Count);
        sprintf(rules_info[rules_info_index].Rules_Info[3], "%d", tz->Rules_Info[rules_info_index].Year_Begin);
        sprintf(rules_info[rules_info_index].Rules_Info[4], "%d", tz->Rules_Info[rules_info_index].Year_End);

        for (COUNTER field_index = 0; field_index < RULE_INFO_FIELDS_COUNT; field_index++)
        {
            if (utf8_strlen(rules_info[rules_info_index].Rules_Info[field_index]) > rules_info_lenght->Rules_Info[field_index])
            {
                rules_info_lenght->Rules_Info[field_index] = utf8_strlen(rules_info[rules_info_index].Rules_Info[field_index]);
            }
        }
    }
    return rules_info;
}

Rules_Data_String_t* Convert_Rules_Data_To_String(Time_Zones_t* tz, Rules_Data_Lenght_t* rules_data_lenght)
{
    for (COUNTER field_index = 0; field_index < RULE_DATA_FIELDS_COUNT; field_index++)
    {
        rules_data_lenght->Rules_Data[field_index] = 0;
    }

    Rules_Data_String_t* rules_data = (Rules_Data_String_t*)calloc(tz->Rules_Data_Count + 1, sizeof(Rules_Data_String_t));
    if (rules_data == NULL)
    {
        return NULL;
    }

    for (COUNTER rules_data_index = 0; rules_data_index < tz->Rules_Data_Count; rules_data_index++)
    {
        for (COUNTER field_index = 0; field_index < RULE_DATA_FIELDS_COUNT; field_index++)
        {
            rules_data[rules_data_index].Rules_Data[field_index] = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD, sizeof(CHAR));
            if (rules_data[rules_data_index].Rules_Data[field_index] == NULL)
            {
                return NULL;
            }
        }

        sprintf(rules_data[rules_data_index].Rules_Data[0], "%d", tz->Rules_Data[rules_data_index].Rule_ID);
        sprintf(rules_data[rules_data_index].Rules_Data[1], "%d", tz->Rules_Data[rules_data_index].From);
        sprintf(rules_data[rules_data_index].Rules_Data[2], "%d", tz->Rules_Data[rules_data_index].To);
        sprintf(rules_data[rules_data_index].Rules_Data[3], "%d", tz->Rules_Data[rules_data_index].Month);
        sprintf(rules_data[rules_data_index].Rules_Data[4], "%d", tz->Rules_Data[rules_data_index].Day);
        sprintf(rules_data[rules_data_index].Rules_Data[5], "%d", tz->Rules_Data[rules_data_index].Weekday);
        sprintf(rules_data[rules_data_index].Rules_Data[6], "%d", tz->Rules_Data[rules_data_index].Weekday_IsAfterOrEqual_Day);
        sprintf(rules_data[rules_data_index].Rules_Data[7], "%lld", tz->Rules_Data[rules_data_index].Hour);
        sprintf(rules_data[rules_data_index].Rules_Data[8], "%d", tz->Rules_Data[rules_data_index].Hour_isUTC);
        sprintf(rules_data[rules_data_index].Rules_Data[9], "%lld", tz->Rules_Data[rules_data_index].Save_Hour);
        sprintf(rules_data[rules_data_index].Rules_Data[10], "%s", tz->Rules_Data[rules_data_index].Letter);
        sprintf(rules_data[rules_data_index].Rules_Data[11], "%s", tz->Rules_Data[rules_data_index].Comments);

        for (COUNTER field_index = 0; field_index < RULE_DATA_FIELDS_COUNT; field_index++)
        {
            if (utf8_strlen(rules_data[rules_data_index].Rules_Data[field_index]) > rules_data_lenght->Rules_Data[field_index])
            {
                rules_data_lenght->Rules_Data[field_index] = utf8_strlen(rules_data[rules_data_index].Rules_Data[field_index]);
            }
        }
    }
    return rules_data;
}


CHAR* Print_Space(COUNTER str_lenght, COUNTER max_lenght)
{
    COUNTER count = max_lenght - str_lenght + 1;
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

VOID Free_TZ_Data(Time_Zones_t* tz)
{
    for (COUNTER zones_info_index = 0; zones_info_index < tz->Zones_Count; zones_info_index++)
    {
        for (COUNTER field_index = 0; field_index < ZONE_INFO_FIELDS_COUNT; field_index++)
        {
            if (zones_info[zones_info_index].Zones_Info[field_index] != NULL)
            {
                free(zones_info[zones_info_index].Zones_Info[field_index]);
            }
        }
    }

    for (COUNTER zones_data_index = 0; zones_data_index < tz->Zones_Data_Count; zones_data_index++)
    {
        for (COUNTER field_index = 0; field_index < ZONE_DATA_FIELDS_COUNT; field_index++)
        {
            if (zones_data[zones_data_index].Zones_Data[field_index] != NULL)
            {
                free(zones_data[zones_data_index].Zones_Data[field_index]);
            }
        }
    }

    for (COUNTER rules_info_index = 0; rules_info_index < tz->Rules_Count; rules_info_index++)
    {
        for (COUNTER field_index = 0; field_index < RULE_INFO_FIELDS_COUNT; field_index++)
        {
            if (rules_info[rules_info_index].Rules_Info[field_index] != NULL)
            {
                free(rules_info[rules_info_index].Rules_Info[field_index]);
            }
        }
    }

    for (COUNTER rules_data_index = 0; rules_data_index < tz->Rules_Data_Count; rules_data_index++)
    {
        for (COUNTER field_index = 0; field_index < RULE_DATA_FIELDS_COUNT; field_index++)
        {
            if (rules_data[rules_data_index].Rules_Data[field_index] != NULL)
            {
                free(rules_data[rules_data_index].Rules_Data[field_index]);
            }
        }
    }

    free(tz);

}

