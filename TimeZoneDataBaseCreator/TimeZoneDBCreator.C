
//#include "../Inc/TimeZoneDBCreator.h"
#include "Generator/Inc/Generator.h"

int main()
{
    CHAR* path= "../tzdata2025a";
    Generate_Data(&path);

    while (1);
    return 0;
}

