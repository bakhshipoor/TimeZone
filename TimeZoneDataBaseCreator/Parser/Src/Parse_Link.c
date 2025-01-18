#include "../Inc/Parse_Link.h"

Link_Entry_t* Parse_Links(int32_t* links_Count)
{
    Link_Entry_t* Links_List = NULL;
    *links_Count = 0;
    char line[2048];
    uint16_t dataFile_index = 0;

    FILE* data_File = fopen(Data_File[7], "r");
    if (!data_File)
    {
        return NULL;
    }

    while (fgets(line, sizeof(line), data_File))
    {
        if (strlen(line) < 5 || line[0] == '#')
        {
            continue;
        }

        Link_Data_t link_data = Parse_Link_Data(line);

        if (strlen(link_data.Field) <= 0)
        {
            continue;
        }

        Link_Entry_t entry = { 0 };

        entry.Target = (uint8_t*)malloc((strlen(link_data.Target) + 1) * sizeof(uint8_t));
        if (entry.Target != NULL)
        {
            sprintf(entry.Target, "%s", link_data.Target);
        }

        entry.Link_Name = (uint8_t*)malloc((strlen(link_data.Link_Name) + 1) * sizeof(uint8_t));
        if (entry.Link_Name != NULL)
        {
            sprintf(entry.Link_Name, "%s", link_data.Link_Name);
        }

        entry.Target1 = (uint8_t*)malloc((strlen(link_data.Target1) + 1) * sizeof(uint8_t));
        if (entry.Target1 != NULL)
        {
            sprintf(entry.Target1, "%s", link_data.Target1);
        }

        if ((*links_Count) == 0)
        {
            Link_Entry_t* links_list = (Link_Entry_t*)malloc(sizeof(Link_Entry_t));
            if (links_list == NULL)
            {
                return NULL;
            }
            Links_List = links_list;
        }
        else
        {
            Link_Entry_t* links_list = (Link_Entry_t*)realloc(Links_List, ((*links_Count) + 1) * sizeof(Link_Entry_t));
            if (links_list == NULL)
            {
                return NULL;
            }
            Links_List = links_list;
        }
        Links_List[*links_Count] = entry;
        (*links_Count)++;
    }

    fclose(data_File);
    return Links_List;
}

Link_Data_t Parse_Link_Data(const char* line)
{
    Link_Data_t link_data;
    int scan_lenght = 0;

    memset(link_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
    memset(link_data.Target, '\0', MAX_LENGHT_DATA_FIELD);
    memset(link_data.Link_Name, '\0', MAX_LENGHT_DATA_FIELD);
    memset(link_data.Target1, '\0', MAX_LENGHT_DATA_FIELD);


    scan_lenght = sscanf(line, "%s\t%s\t%[^#= \t\n]\t#= %[^\n]",
        link_data.Field,
        link_data.Target,
        link_data.Link_Name,
        link_data.Target1);



    sprintf(link_data.Field, "%s", link_data.Field);
    sprintf(link_data.Target, "%s", link_data.Target);
    sprintf(link_data.Link_Name, "%s", link_data.Link_Name);
    sprintf(link_data.Target1, "%s", link_data.Target1);

    if (strcmp("Link", link_data.Field) != 0 /*|| scan_lenght < 3 || scan_lenght>7*/)
    {
        memset(link_data.Field, '\0', MAX_LENGHT_DATA_FIELD);
        memset(link_data.Target, '\0', MAX_LENGHT_DATA_FIELD);
        memset(link_data.Link_Name, '\0', MAX_LENGHT_DATA_FIELD);
        memset(link_data.Target1, '\0', MAX_LENGHT_DATA_FIELD);
    }

    return link_data;
}
