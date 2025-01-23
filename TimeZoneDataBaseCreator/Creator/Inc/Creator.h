#ifndef CREATOR_H
#define CREATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Generator/Inc/Generator.h"

    VOID Create_Database(CONST CHAR** data_folder_path);
    VOID Create_Time_Zone_Database_Header_File(Time_Zones_t** tz);


#ifdef __cplusplus
}
#endif

#endif /* CREATOR_H */