
//#include "../Inc/TimeZoneDBCreator.h"
#include "Creator/Creator.h"


int main()
{
    CHAR* path= "../tzdata2025a";

    

    Create_Database(&path);

    while (1);
    return 0;
}

