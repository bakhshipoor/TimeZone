
//#include "../Inc/TimeZoneDBCreator.h"
#include "Creator/Inc/Creator.h"

int main()
{
    CHAR* path= "../tzdata2025a";
    Create_Database(&path);

    while (1);
    return 0;
}

