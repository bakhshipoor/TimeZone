
//#include "../Inc/TimeZoneDBCreator.h"
#include "Creator/Inc/Creator.h"

const char* sss[] = {

    //dfd
    { "dddd"},
};

int main()
{
    CHAR* path= "../tzdata2025a";

    

    Create_Database(&path);

    while (1);
    return 0;
}

