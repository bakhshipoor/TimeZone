#include "../Inc/Parse_Version.h"

Version_t* Parse_Version(void)
{
    Version_t* version = (Version_t*)malloc(sizeof(Version_t));
    if (version == NULL)
    {
        return NULL;
    }
    version->Major = 0;
    uint8_t line[2048];
    uint32_t major;
    uint8_t minor[MAX_LENGHT_DATA_FIELD];

    FILE* data_File = fopen(Data_File[0], "r");
    if (!data_File)
    {
        return version;
    }
    
    fgets(line, sizeof(line), data_File);

    if (sscanf(line, "%d%s", &major, minor)==2)
    {
        version->Major = major;
        sprintf(minor, "%s", minor);
        version->Minor = (uint8_t*)malloc((strlen(minor) + 1) * sizeof(uint8_t));
        if (version->Minor != NULL)
        {
            sprintf(version->Minor, minor);
        }
    }
    else if (sscanf(line, "%d", &major) == 1)
    {
        version->Major = major;
    }

    fclose(data_File);
    return version;
}