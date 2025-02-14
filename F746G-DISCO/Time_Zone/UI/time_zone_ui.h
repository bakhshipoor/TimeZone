#ifndef TIME_ZONE_UI_H
#define TIME_ZONE_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "../timezone_calculator/time_zone.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

    extern lv_obj_t* scr_Main;
    extern lv_style_t style_scr_Main;

    extern int32_t Year;
    extern uint8_t Month;
    extern uint8_t Day;

    extern uint8_t Hours;
    extern uint8_t Minutes;
    extern uint8_t Seconds;

    extern char* Zone_Identifier;

    extern bool TZ_Init;
    
    extern lv_timer_t* timer_update_data;

    void tz_ui(void);
    void update_data(void);

#ifdef __cplusplus
}
#endif

#endif
