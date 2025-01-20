#include "../Inc/Parse_Link.h"

Link_Entry_t* Parse_Links(COUNTER* links_Count)
{
    Link_Entry_t* Links_List = NULL;
    *links_Count = 0;
    
    COUNTER dataFile_index = 0;
    FILE* data_File;
    Link_Data_t* link_data = NULL;
    for (dataFile_index = 3; dataFile_index < DATA_FILES_COUNT; dataFile_index++)
    {

        data_File = fopen(Data_File[dataFile_index], "r");
        if (!data_File)
        {
            return NULL;
        }

        while (fgets(line, MAX_LENGTH_LINE, data_File))
        {
            if (strlen(line) < 5 || line[0] == '#')
            {
                continue;
            }

            Parse_Free_Link_Data(&link_data);
            link_data = Parse_Link_Data(line);

            if (link_data == NULL)
            {
                continue;
            }

            Link_Entry_t* entry = (Link_Entry_t*)malloc(sizeof(Link_Entry_t));

            entry->Target = (CHAR*)malloc((strlen(link_data->Target) + 1) * sizeof(CHAR));
            if (entry->Target != NULL)
            {
                sprintf(entry->Target, "%s", link_data->Target);
            }

            entry->Link_Name = (CHAR*)malloc((strlen(link_data->Link_Name) + 1) * sizeof(CHAR));
            if (entry->Link_Name != NULL)
            {
                sprintf(entry->Link_Name, "%s", link_data->Link_Name);
            }

            entry->Target1 = (CHAR*)malloc((strlen(link_data->Target1) + 1) * sizeof(CHAR));
            if (entry->Target1 != NULL)
            {
                sprintf(entry->Target1, "%s", link_data->Target1);
            }
            Link_Entry_t* links_list;
            if ((*links_Count) == 0)
            {
                links_list = (Link_Entry_t*)malloc(sizeof(Link_Entry_t));
            }
            else
            {
                links_list = (Link_Entry_t*)realloc(Links_List, ((*links_Count) + 1) * sizeof(Link_Entry_t));
            }
            if (links_list == NULL)
            {
                return NULL;
            }
            Links_List = links_list;
            Links_List[*links_Count] = *entry;
            (*links_Count)++;
        }

        fclose(data_File);
    }
    Parse_Free_Link_Data(&link_data);
    return Links_List;
}

Link_Data_t* Parse_Link_Data(CONST CHAR* line)
{
    Link_Data_t* link_data=(Link_Data_t*)malloc(sizeof(Link_Data_t));
    if (link_data == NULL)
    {
        return NULL;
    }
    LENGHT scan_lenght = 0;

    link_data->Field = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    link_data->Target = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    link_data->Link_Name = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));
    link_data->Target1 = (CHAR*)calloc(1, MAX_LENGTH_DATA_FIELD * sizeof(char));

    if (link_data->Field == NULL ||
        link_data->Target == NULL ||
        link_data->Link_Name == NULL ||
        link_data->Target1 == NULL)
    {
        return NULL;
    }
    

    scan_lenght = sscanf(line, "%s\t%s\t%[^#= \t\n]\t#= %[^\n]",
        link_data->Field,
        link_data->Target,
        link_data->Link_Name,
        link_data->Target1);



    sprintf(link_data->Field, "%s", link_data->Field);
    sprintf(link_data->Target, "%s", link_data->Target);
    sprintf(link_data->Link_Name, "%s", link_data->Link_Name);
    sprintf(link_data->Target1, "%s", link_data->Target1);

    if (strcmp("Link", link_data->Field) != 0 /*|| scan_lenght < 3 || scan_lenght>7*/)
    {
        Parse_Free_Link_Data(&link_data);
    }

    return link_data;
}

VOID Parse_Free_Link_Data(Link_Data_t** link_data)
{
    if (*link_data != NULL)
    {
        if ((*link_data)->Field != NULL)
        {
            free((*link_data)->Field);
            (*link_data)->Field = NULL;
        }
        if ((*link_data)->Target != NULL)
        {
            free((*link_data)->Target);
            (*link_data)->Target = NULL;
        }
        if ((*link_data)->Link_Name != NULL)
        {
            free((*link_data)->Link_Name);
            (*link_data)->Link_Name = NULL;
        }
        if ((*link_data)->Target1 != NULL)
        {
            free((*link_data)->Target1);
            (*link_data)->Target1 = NULL;
        }
        
        free((*link_data));
        (*link_data) = NULL;
    }
}