#include "../Inc/Parse_Links.h"

typedef struct
{
    CHAR* Field;
    CHAR* Target;
    CHAR* Link_Name;
    CHAR* Target1;
} Link_Data_t;


STATIC BOOL Target_isExist(CONST Link_Entry_t** link_list, CONST COUNTER* links_count, CONST CHAR** target, COUNTER* find_Index);
STATIC VOID Link_Create(Link_Entry_t** link_list, COUNTER* links_count, CONST CHAR** target);
STATIC BOOL Link_Name_Create(Link_Name_t** link_name_list, CONST COUNTER* link_names_count, CONST Link_Data_t* link_data);
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

    /* COUNTER find_index = INDEX_NOT_FOUND;
    if (!Target_isExist(links_list, links_count, &link_data->Target, &find_index))
    {
        Link_Create(links_list, links_count, &link_data->Target);
        find_index = (*links_count) - 1;
    }

    if (find_index != INDEX_NOT_FOUND)
    {
        if (Link_Name_Create(&(*links_list)[find_index].Link_Names, &(*links_list)[find_index].Link_Name_Counts, link_data))
        {
            (*links_list)[find_index].Link_Name_Counts++;
        }
    }*/

    Link_Entry_t* link = (Link_Entry_t*)malloc(sizeof(Link_Entry_t));
    if (link == NULL)
    {
        Free_Link_Data(link_data);
        return NULL;
    }

    link->Target = _strdup(link_data->Target);
    link->Link_Name = _strdup(link_data->Link_Name);
    //link->Target1 = _strdup(link_data->Target1);

    if (link->Target == NULL || link->Link_Name == NULL /*|| link->Target1 == NULL*/)
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

STATIC BOOL Target_isExist(CONST Link_Entry_t** link_list, CONST COUNTER* links_count, CONST CHAR** target, COUNTER* find_Index)
{
    if (link_list != NULL && links_count != NULL && *links_count > 0)
    {
        for (COUNTER rule_index = 0; rule_index < *links_count; rule_index++)
        {
            if (strcmp((*link_list)[rule_index].Target, *target) == 0)
            {
                *find_Index = rule_index;
                return TRUE;
            }
        }
    }
    return FALSE;
}

STATIC VOID Link_Create(Link_Entry_t** link_list, COUNTER* links_count, CONST CHAR** target)
{
    if (links_count == NULL || target == NULL || *target == NULL)
    {
        return;
    }

    Link_Entry_t* link = (Link_Entry_t*)calloc(1, sizeof(Link_Entry_t));
    if (link == NULL)
    {
        return;
    }

    link->Target = (CHAR*)calloc((strlen(*target) + 1), sizeof(CHAR));
    if (link->Target == NULL)
    {
        Free_Link(link);
        return;
    }

    strcpy(link->Target, *target);

    Link_Entry_t* rules = realloc(*link_list, (*links_count + 1) * sizeof(Link_Entry_t));
    if (rules == NULL)
    {
        Free_Link(link);
        return;
    }

    *link_list = rules;
    (*link_list)[*links_count] = *link;
    (*links_count)++;

    free(link);
}

STATIC BOOL Link_Name_Create(Link_Name_t** link_name_list, CONST COUNTER* link_names_count, CONST Link_Data_t* link_data)
{
    if (link_names_count == NULL || link_data == NULL)
    {
        return FALSE;
    }

    Link_Name_t link_name = { 0 };
    
    
    link_name.Link_Name = (CHAR*)calloc((strlen(link_data->Link_Name) + 1), sizeof(CHAR));
    if (link_name.Link_Name != NULL)
    {
        link_name.Link_Name = _strdup(link_data->Link_Name);
    }

    Link_Name_t* ln_list;
    if (*link_names_count == 0)
    {
        ln_list = (Link_Name_t*)malloc(sizeof(Link_Name_t));
    }
    else
    {
        ln_list = (Link_Name_t*)realloc(*link_name_list, (*link_names_count + 1) * sizeof(Link_Name_t));
    }
    if (ln_list == NULL)
    {
        free(ln_list);
        ln_list = NULL;
        return FALSE;
    }
    *link_name_list = ln_list;
    (*link_name_list)[*link_names_count] = link_name;


    return TRUE;
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
    /*if (link != NULL)
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
    }*/
}