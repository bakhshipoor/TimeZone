#ifdef _WIN32



#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "lvgl.h"
#include "UI/time_zone_ui.h"
#include "LvglWindowsIconResource/LvglWindowsIconResource.h"

void scr_Main_Event(lv_event_t* e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_UNLOAD_START)
    {
        lv_timer_delete(timer_update_data);
    }

}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{

    lv_init();

#if LV_TXT_ENC == LV_TXT_ENC_UTF8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    int32_t zoom_level = 100;
    bool allow_dpi_override = true;
    bool simulator_mode = true;
    lv_display_t* display = lv_windows_create_display(
        L"Time Zone",
        480,
        272,
        zoom_level,
        allow_dpi_override,
        simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display);
    if (!window_handle)
    {
        return -1;
    }

    HICON icon_handle = LoadIconW(
        GetModuleHandleW(NULL),
        MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        SendMessageW(
            window_handle,
            WM_SETICON,
            TRUE,
            (LPARAM)icon_handle);
        SendMessageW(
            window_handle,
            WM_SETICON,
            FALSE,
            (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev)
    {
        return -1;
    }

    lv_indev_t* encoder_indev = lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev)
    {
        return -1;
    }
    lv_task_handler();
    lv_theme_t* theme = lv_theme_default_init(display, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);
    lv_obj_t* scr_Main;
    scr_Main = lv_obj_create(NULL);
    
    time_t now;
    struct tm* timeinfo;

    time(&now);
    timeinfo = gmtime(&now);

    Year = timeinfo->tm_year - 100;
    Month = timeinfo->tm_mon+1;
    Day = timeinfo->tm_mday;

    Hours = timeinfo->tm_hour;
    Minutes = timeinfo->tm_min;
    Seconds = timeinfo->tm_sec;

    TZ_Init = tz_init();

    lv_obj_set_style_bg_color(scr_Main, lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_text_color(scr_Main, lv_color_hex(0x000000),0);
    lv_obj_add_event_cb(scr_Main, scr_Main_Event, LV_EVENT_ALL, NULL);
    lv_screen_load(scr_Main);


    tz_ui();
   
    while (1)
    {
        
        time(&now);
        timeinfo = gmtime(&now);

        Year =  timeinfo->tm_year - 100 + 2000;
        Month = timeinfo->tm_mon+1;
        Day = timeinfo->tm_mday;

        Hours = timeinfo->tm_hour;
        Minutes = timeinfo->tm_min;
        Seconds = timeinfo->tm_sec;
        
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
        //lv_task_handler();
    }

    return 0;
}

#endif