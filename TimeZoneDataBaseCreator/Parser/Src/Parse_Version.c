#include "../Inc/Parse_Version.h"

Version_t* Parse_Version(VOID)
{
    FILE* data_File = fopen(Data_File[0], "r");
    if (!data_File)
    {
        return NULL;
    }

    Version_t* version = (Version_t*)malloc(sizeof(Version_t));
    if (version == NULL)
    {
        fclose(data_File);
        return NULL;
    }
    
    CHAR* minor = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(CHAR));
    if (minor == NULL)
    {
        free(version);
        version = NULL;
        fclose(data_File);
        return version;
    }
    version->Major = 0;
    fgets(line, sizeof(line), data_File);

    if (sscanf(line, "%d%s", &version->Major, minor)==2)
    {
        sprintf(minor, "%s", minor);
        version->Minor = (CHAR*)malloc((strlen(minor) + 1) * sizeof(CHAR));
        if (version->Minor != NULL)
        {
            sprintf(version->Minor, minor);
        }
    }
    else if (sscanf(line, "%d", &version->Major) == 1)
    {
        
    }
    else
    {
        version->Major = 0;

    }
    free(minor);
    minor = NULL;
    
    fclose(data_File);
    return version;
}