#include "Creator/Creator.h"

int main()
{
    CHAR* path = (CHAR*)calloc(500, sizeof(CHAR));//"tzdata2025a";
    if (path == NULL)
    {
        printf("Error Initial Path Variable\n");
        return -1;
    }
    printf("Please visit https://www.iana.org/time-zones\n");
    printf(" and find the latest version of the database,\n");
    printf("then download the 'Data Only Distribution' option.\n");
    printf("After downloading, please extract it.\n");
    printf("After extract, please enter the folder path below.\n");
    printf("\n");
    printf("Please Enter Folder Path: ");
    char temp[500];
    int len = scanf("%s", temp);
    if (len == 1)
    {
        sprintf(path,"%s", temp);
        Create_Database(&path);
    }
    else
    {
        printf("Error Initial Path Variable\n");
        return -1;
    }
    return 0;
}

