
#include "../Inc/TimeZoneDBCreator.h"
#include "../Inc/Parse_Rules.h"

uint8_t version_FileName[MAX_LENGHT_FILE_NAME]; 
uint8_t iso3166tab_FileName[MAX_LENGHT_FILE_NAME]; 
uint8_t zonetab_FileName[MAX_LENGHT_FILE_NAME]; 
// Data Files
uint8_t Data_FileName[DATA_FILES_COUNT][MAX_LENGHT_FILE_NAME];


void Initial_FileNames(void)
{
    sprintf_s(version_FileName,             MAX_LENGHT_FILE_NAME,   "../%s/version",          Data_Folder_Name);
    sprintf_s(iso3166tab_FileName,          MAX_LENGHT_FILE_NAME,   "../%s/iso3166.tab",      Data_Folder_Name);
    sprintf_s(zonetab_FileName,             MAX_LENGHT_FILE_NAME,   "../%s/zone.tab",         Data_Folder_Name);
    // Data Files
    sprintf_s(Data_FileName[0], MAX_LENGHT_FILE_NAME, "../%s/africa",           Data_Folder_Name);
    sprintf_s(Data_FileName[1], MAX_LENGHT_FILE_NAME, "../%s/antarctica",       Data_Folder_Name);
    sprintf_s(Data_FileName[2], MAX_LENGHT_FILE_NAME, "../%s/asia",             Data_Folder_Name);
    sprintf_s(Data_FileName[3], MAX_LENGHT_FILE_NAME, "../%s/australasia",      Data_Folder_Name);
    sprintf_s(Data_FileName[4], MAX_LENGHT_FILE_NAME, "../%s/backward",         Data_Folder_Name);
    sprintf_s(Data_FileName[5], MAX_LENGHT_FILE_NAME, "../%s/backzone",         Data_Folder_Name);
    sprintf_s(Data_FileName[6], MAX_LENGHT_FILE_NAME, "../%s/etcetera",         Data_Folder_Name);
    sprintf_s(Data_FileName[7], MAX_LENGHT_FILE_NAME, "../%s/europe",           Data_Folder_Name);
    sprintf_s(Data_FileName[8], MAX_LENGHT_FILE_NAME, "../%s/northamerica",     Data_Folder_Name);
    sprintf_s(Data_FileName[9], MAX_LENGHT_FILE_NAME, "../%s/southamerica",     Data_Folder_Name);

}

char* Parse_Version()
{
    FILE* version_File = fopen(version_FileName, "r");
    if (!version_File) {
        perror("Failed to open version");
        return NULL;
    }
    char version[256];
    memset(version, '\0', 256);
    fgets(version, 256, version_File); 
    sprintf(version, "%s", version);
    size_t version_lenght = strlen(version);
    if (version[version_lenght - 1] == '\n')
    {
        version[version_lenght - 1] = '\0';
    }
    fclose(version_File);
    return version;
}


ISO3166_Entry_t* Parse_ISO3166Tab(int* iso3166_Count)
{
    FILE* iso3166_File = fopen(iso3166tab_FileName, "r");
    if (!iso3166_File) {
        perror("Failed to open file");
        return NULL;
    }

    ISO3166_Entry_t* ISO3166_List = NULL;
    *iso3166_Count = 0;
    char line[2048];

    while (fgets(line, sizeof(line), iso3166_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }

        
        int sscanf_lenght;
        uint8_t country_code[MAX_LENGHT_COUNTRY_CODE];
        uint8_t country_name[MAX_LENGHT_COUNTRY_NAME];
        
        memset(country_code, '\0', MAX_LENGHT_COUNTRY_CODE);
        memset(country_name, '\0', MAX_LENGHT_COUNTRY_NAME);

        sscanf_lenght = sscanf(line, "%s\t%[^\n]", country_code, country_name);

        if (sscanf_lenght < 2)
        {
            continue;
        }

        sprintf_s(country_code, MAX_LENGHT_COUNTRY_CODE, "%s", country_code);
        sprintf_s(country_name, MAX_LENGHT_COUNTRY_NAME, "%s", country_name);

        ISO3166_Entry_t entry;

        entry.Country_Code = (uint8_t*)malloc((strlen(country_code) + 1) * sizeof(uint8_t));
        entry.Country_Name = (uint8_t*)malloc((strlen(country_name) + 1) * sizeof(uint8_t));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(country_code)+1, "%s", country_code);
        }
        if (entry.Country_Name != NULL)
        {
            sprintf_s(entry.Country_Name, strlen(country_name)+1, "%s", country_name);
        }

        ISO3166_Entry_t* tz_list = realloc(ISO3166_List, (*iso3166_Count + 1) * sizeof(ISO3166_Entry_t));
        if (tz_list == NULL)
        {
            return NULL;
        }
        ISO3166_List = tz_list;
        ISO3166_List[*iso3166_Count] = entry;
        (*iso3166_Count)++;
    }

    fclose(iso3166_File);
    return ISO3166_List;
}

ZoneTab_Entry_t* Parse_ZoneTab(int* zoneCount) 
{
    FILE* zonetab_File = fopen(zonetab_FileName, "r");
    if (!zonetab_File) {
        perror("Failed to open file");
        return NULL;
    }

    ZoneTab_Entry_t* TimeZone_List = NULL;
    *zoneCount = 0;
    char line[2048];

    while (fgets(line, sizeof(line), zonetab_File)) 
    {
        if (strlen(line) < 5 || line[0] == '#') 
        {
            continue;
        }

        
        int sscanf_lenght;
        uint8_t code[MAX_LENGHT_COUNTRY_CODE];
        uint8_t coordinates[MAX_LENGHT_COORDINATES];
        uint8_t tzid[MAX_LENGHT_TIMEZONEID];
        uint8_t comment[MAX_LENGHT_COMMENT];
        size_t coordinates_lenght = 0;
        char lat_Sign = 0;
        int lat_Hour = 0;
        int lat_Minute = 0;
        int lat_Second = 0;
        char long_Sign = 0;
        int long_Hour = 0;
        int long_Minute = 0;
        int long_Second = 0;
        double latitude = 0.0;
        double longitude = 0.0;

        memset(code, '\0', MAX_LENGHT_COUNTRY_CODE);
        memset(coordinates, '\0', MAX_LENGHT_COORDINATES);
        memset(tzid, '\0', MAX_LENGHT_TIMEZONEID);
        memset(comment, '\0', MAX_LENGHT_COMMENT);

        sscanf_lenght = sscanf(line, "%s\t%s\t%s\t%[^\n]", code, coordinates, tzid, comment);

        if (sscanf_lenght < 3)
        {
            continue;
        }
        sprintf_s(code, MAX_LENGHT_COUNTRY_CODE, "%s", code);
        sprintf_s(coordinates, MAX_LENGHT_COORDINATES, "%s", coordinates);
        sprintf_s(tzid, MAX_LENGHT_TIMEZONEID, "%s", tzid);
        sprintf_s(comment, MAX_LENGHT_COMMENT, "%s", comment);

        coordinates_lenght = strlen(coordinates);
        if (coordinates_lenght == 11) 
        {
            sscanf_lenght = sscanf(coordinates, "%c%2d%2d%c%3d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &long_Sign, &long_Hour, &long_Minute);
            if (sscanf_lenght != 6)
            {
                continue;
            }
        }
        else {
            sscanf_lenght = sscanf(coordinates, "%c%2d%2d%2d%c%3d%2d%2d", &lat_Sign, &lat_Hour, &lat_Minute, &lat_Second, &long_Sign, &long_Hour, &long_Minute, &long_Second);
            if (sscanf_lenght != 8)
            {
                continue;
            }
        }
        latitude = (lat_Hour * 1.0) + (lat_Minute / 60.0) + (lat_Second / 3600.0);
        longitude = (long_Hour * 1.0) + (long_Minute / 60.0) + (long_Second / 3600.0);
        latitude = lat_Sign == '+' ? latitude : -latitude;
        longitude = long_Sign == '+' ? longitude : -longitude;

        ZoneTab_Entry_t entry;
        entry.Country_Code = (uint8_t*)malloc((strlen(code) + 1) * sizeof(uint8_t));
        entry.TZ_Identifier = (uint8_t*)malloc((strlen(tzid) + 1) * sizeof(uint8_t));
        entry.Comments = (uint8_t*)malloc((strlen(comment) + 1) * sizeof(uint8_t));

        if (entry.Country_Code != NULL)
        {
            sprintf_s(entry.Country_Code, strlen(code)+1, "%s", code);
        }
        if (entry.TZ_Identifier != NULL)
        {
            sprintf_s(entry.TZ_Identifier, strlen(tzid)+1, "%s", tzid);
        }
        if (entry.Comments != NULL)
        {
            sprintf_s(entry.Comments, strlen(comment)+1, "%s", comment);
        }
        
        entry.Latitude = latitude;
        entry.Longitude = longitude;

        ZoneTab_Entry_t* tz_list = realloc(TimeZone_List, (*zoneCount + 1) * sizeof(ZoneTab_Entry_t));
        if (tz_list == NULL)
        {
            return NULL;
        }
        TimeZone_List = tz_list;
        TimeZone_List[*zoneCount] = entry;
        (*zoneCount)++;
    }

    fclose(zonetab_File);
    return TimeZone_List;
}


int main() 
{
    Initial_FileNames();



    char* version = Parse_Version();

    int iso3166_Count=0;
    ISO3166_Entry_t* iso3166_list = Parse_ISO3166Tab(&iso3166_Count);

    int zoneCount = 0;
    ZoneTab_Entry_t* tab = Parse_ZoneTab(&zoneCount);

    int rules_Count = 0;
    Rule_Entry_t* rules_list = Parse_Rules(&rules_Count);
   
    
    
    
    while (1);
    return 0;
}

