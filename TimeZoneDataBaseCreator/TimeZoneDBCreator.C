
//#include "../Inc/TimeZoneDBCreator.h"
#include "Generator/Inc/Generator.h"

int main()
{
    CHAR* path= (CHAR*)calloc(20,20*sizeof(CHAR));
    sprintf(path,"%s","../tzdata2025a");
    Generate_Data(&path);

    while (1);
    return 0;
}

