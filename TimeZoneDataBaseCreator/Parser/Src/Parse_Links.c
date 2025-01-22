#include "../Inc/Parse_Links.h"

typedef struct
{
    CHAR* Field;
    CHAR* Target;
    CHAR* Link_Name;
    CHAR* Target1;
} Link_Data_t;

STATIC Link_Data_t* Parse_Link_Data(CONST CHAR** line);
STATIC VOID Free_Link_Data(Link_Data_t* link_data);
STATIC VOID Free_Link(Link_Entry_t* link);

Link_Entry_t* Parse_Links(CONST CHAR** line, Link_Entry_t** links_list, COUNTER* links_count)
{
    Link_Data_t* link_data = Parse_Link_Data(line);
    if (link_data == NULL)
    {
        return NULL;
    }

    Link_Entry_t* link = (Link_Entry_t*)malloc(sizeof(Link_Entry_t));
    if (link == NULL)
    {
        Free_Link_Data(link_data);
        return NULL;
    }

    link->Target = _strdup(link_data->Target);
    link->Link_Name = _strdup(link_data->Link_Name);
    link->Target1 = _strdup(link_data->Target1);

    if (link->Target == NULL || link->Link_Name == NULL || link->Target1 == NULL)
    {
        Free_Link_Data(link_data);
        Free_Link(link);
        return NULL;
    }

    Link_Entry_t* list = (Link_Entry_t*)realloc(*links_list, ((*links_count) + 1) * sizeof(Link_Entry_t));
    if (list == NULL)
    {
        Free_Link_Data(link_data);
        Free_Link(link);
        return NULL;
    }

    *links_list = list;
    (*links_list)[*links_count] = *link;
    (*links_count)++;

    free(link);
    Free_Link_Data(link_data);
    return;
}

STATIC Link_Data_t* Parse_Link_Data(CONST CHAR** line)
{
    Link_Data_t* link_data = (Link_Data_t*)malloc(sizeof(Link_Data_t));
    if (link_data == NULL)
    {
        return NULL;
    }

    link_data->Field = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    link_data->Target = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    link_data->Link_Name = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD , sizeof(CHAR));
    link_data->Target1 = (CHAR*)calloc(MAX_LENGTH_DATA_FIELD , sizeof(CHAR));

    if (link_data->Field == NULL ||
        link_data->Target == NULL ||
        link_data->Link_Name == NULL ||
        link_data->Target1 == NULL)
    {
        Free_Link_Data(link_data);
        return NULL;
    }

    LENGHT scan_lenght = sscanf(*line, "%s\t%s\t%[^#= \t\n]\t#= %[^\n]",
        link_data->Field,
        link_data->Target,
        link_data->Link_Name,
        link_data->Target1);

    if (scan_lenght < 3 || strcmp("Link", link_data->Field) != 0)
    {
        Free_Link_Data(link_data);
        return NULL;
    }

    return link_data;
}

STATIC VOID Free_Link_Data(Link_Data_t* link_data)
{
    if (link_data != NULL)
    {
        CHAR* fields[] = { link_data->Field, link_data->Target, link_data->Link_Name, link_data->Target1 };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
            }
        }
        free(link_data);
    }
}

STATIC VOID Free_Link(Link_Entry_t* link)
{
    if (link != NULL)
    {
        CHAR* fields[] = { link->Target, link->Link_Name, link->Target1 };
        for (int i = 0; i < sizeof(fields) / sizeof(fields[0]); i++)
        {
            if (fields[i] != NULL)
            {
                free(fields[i]);
            }
        }
        free(link);
    }
}