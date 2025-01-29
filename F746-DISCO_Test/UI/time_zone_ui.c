#include "time_zone_ui.h"


uint8_t Year = 0;
uint8_t Month = 0;
uint8_t Day = 0;

uint8_t Hours = 0;
uint8_t Minutes = 0;
uint8_t Seconds = 0;

uint8_t* Zone_Identifier;

bool TZ_Init = false;

static tz_time_t* tz_time = NULL;
static tz_get_offset_t* tz_offset = NULL;


static lv_style_t style_list_zone = { 0 };
static lv_style_t style_lbl_zone_1 = { 0 };
static lv_style_t style_lbl_zone_2 = { 0 };

static lv_obj_t * list_zone = NULL;
static lv_obj_t * selected_zone = NULL;
static lv_obj_t* lbl_selected_zone = NULL;
static lv_obj_t* lbl_date = NULL;
static lv_obj_t* lbl_time = NULL;
static lv_obj_t* lbl_local = NULL;
static lv_obj_t* lbl_dst_effect = NULL;
static lv_obj_t* lbl_std_offset = NULL;
static lv_obj_t* lbl_dst_offset = NULL;
static lv_obj_t* lbl_total_offset = NULL;

static void zone_list_label_event(lv_event_t* e);

static void zone_list_label_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED)
    {
        selected_zone = obj;
        if (lbl_selected_zone != NULL)
        {
            lv_label_set_text(lbl_selected_zone, lv_label_get_text(obj));
            sprintf(Zone_Identifier, "%s", lv_label_get_text(obj));
        }
        
        /*if(selected_zone == obj)
        {
            selected_zone = NULL;
        }
        else
        {
            selected_zone = obj;
        }*/
        lv_obj_t * parent = lv_obj_get_parent(obj);
        uint32_t i;
        for(i = 0; i < lv_obj_get_child_count(parent); i++)
        {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            if(child == selected_zone)
            {
            	lv_obj_add_style(child, &style_lbl_zone_2, 0);
            }
            else
            {
            	lv_obj_add_style(child, &style_lbl_zone_1, 0);
            }
        }
    }
}

static void update_data(lv_timer_t* timer)
{
    if (TZ_Init == true)
    {

        tz_set_zone(&Zone_Identifier);
        tz_set_date(&Year, &Month, &Day);
        tz_set_time(&Hours, &Minutes, &Seconds);
        tz_calculate();

        tz_time = tz_get_local_time();
        tz_offset = tz_get_offset();

        lv_label_set_text_fmt(lbl_date, "Date:          20%02d/%02d/%02d", Year, Month, Day);
        lv_label_set_text_fmt(lbl_time, "UTC:           %02d:%02d:%02d", Hours, Minutes, Seconds);
        lv_label_set_text_fmt(lbl_local, "Local:         %02d:%02d:%02d", tz_time->hour, tz_time->minute, tz_time->second);
        lv_label_set_text_fmt(lbl_dst_effect, "DST Effect:    %s", tz_offset->dst_effect == true ? "True" : "False");
        lv_label_set_text_fmt(lbl_std_offset, "STD Offset:    %02d:%02d:%02d", tz_offset->std_offset.hour, tz_offset->std_offset.minute, tz_offset->std_offset.second);
        lv_label_set_text_fmt(lbl_dst_offset, "DST Offset:    %02d:%02d:%02d", tz_offset->dst_offset.hour, tz_offset->dst_offset.minute, tz_offset->dst_offset.second);
        lv_label_set_text_fmt(lbl_total_offset, "Total Offset:  %02d:%02d:%02d", tz_offset->total_offset.hour, tz_offset->total_offset.minute, tz_offset->total_offset.second);
    }
}

static void style_init(void)
{
    lv_style_init(&style_list_zone);
    lv_style_set_size(&style_list_zone, lv_pct(100), lv_pct(100));
    lv_style_set_pad_row(&style_list_zone, 5);
    lv_style_set_pad_top(&style_list_zone, 5);
    lv_style_set_pad_bottom(&style_list_zone, 5);
    lv_style_set_bg_opa(&style_list_zone, LV_OPA_40);
    lv_style_set_bg_color(&style_list_zone, lv_color_hex(0xFDFDFD));
    lv_style_set_margin_all(&style_list_zone, 2);
    lv_style_set_align(&style_list_zone, LV_ALIGN_LEFT_MID);
    lv_style_set_x(&style_list_zone, 2);

    lv_style_init(&style_lbl_zone_1);
    lv_style_set_bg_opa(&style_lbl_zone_1, LV_OPA_40);
    lv_style_set_bg_color(&style_lbl_zone_1, lv_color_hex(0xEEEEEE));
    lv_style_set_pad_left(&style_lbl_zone_1, 12);
    lv_style_set_pad_right(&style_lbl_zone_1, 12);
    lv_style_set_pad_top(&style_lbl_zone_1, 12);
    lv_style_set_size(&style_lbl_zone_1, lv_pct(100), 40);
    lv_style_set_text_align(&style_lbl_zone_1, LV_TEXT_ALIGN_LEFT);
    lv_style_set_radius(&style_lbl_zone_1, 12);

    lv_style_init(&style_lbl_zone_2);
    lv_style_set_bg_color(&style_lbl_zone_2, lv_color_hex(0xFF0000));
}

void tz_ui(void)
{
    
    style_init();

    Zone_Identifier = (uint8_t*)calloc(TZDB_MAX_LENGHT_IDENTIFIER, sizeof(uint8_t));


    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };


    /*Create a container with grid*/
    lv_obj_t* cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_center(cont);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

    /*Create a zones list*/
    list_zone = lv_list_create(cont);
    lv_obj_add_style(list_zone, &style_list_zone, 0);
    lv_obj_set_grid_cell(list_zone, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 10);

    /*Add labels to the list*/
    lv_obj_t* lbl_zone;
    int i;
    for (i = 0; i < TZDB_ZONES_INFO_COUNT; i++)
    {
        lbl_zone = lv_label_create(list_zone);
        lv_obj_add_flag(lbl_zone, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(lbl_zone, zone_list_label_event, LV_EVENT_CLICKED, NULL);
        lv_label_set_text_fmt(lbl_zone, "%s", tzdb_zones_info[i].zone_identifier);
        lv_label_set_long_mode(lbl_zone, LV_LABEL_LONG_MODE_SCROLL);
        lv_obj_add_style(lbl_zone, &style_lbl_zone_1, 0);
        if (i == 0)
        {
            selected_zone = lbl_zone;
            lv_obj_add_style(lbl_zone, &style_lbl_zone_2, 0);
            sprintf(Zone_Identifier, "%s", lv_label_get_text(lbl_zone));
        }
    }

    lbl_selected_zone = lv_label_create(cont);
    lv_obj_set_size(lbl_selected_zone, lv_pct(100), 20);
    lv_label_set_text(lbl_selected_zone, lv_label_get_text(selected_zone));
    lv_obj_set_style_text_align(lbl_selected_zone, LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_long_mode(lbl_selected_zone, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_obj_set_grid_cell(lbl_selected_zone, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_style_border_color(lbl_selected_zone, lv_color_hex(0x666666), 0);
    lv_obj_set_style_border_width(lbl_selected_zone, 2,0);
    lv_obj_set_style_border_side(lbl_selected_zone, LV_BORDER_SIDE_BOTTOM,0);



    lv_obj_t*  lbl_2 = lv_label_create(cont);
    lv_obj_set_size(lbl_2, lv_pct(100), 20);
    lv_label_set_text(lbl_2, " ");
    lv_obj_set_grid_cell(lbl_2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

    lbl_date = lv_label_create(cont);
    lv_obj_set_size(lbl_date, lv_pct(100), 20);
    lv_label_set_text(lbl_date, "Date:");
    lv_obj_set_grid_cell(lbl_date, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 2, 1);

    lbl_time = lv_label_create(cont);
    lv_obj_set_size(lbl_time, lv_pct(100), 20);
    lv_label_set_text(lbl_time, "UTC:");
    lv_obj_set_grid_cell(lbl_time, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 3, 1);

    lbl_local = lv_label_create(cont);
    lv_obj_set_size(lbl_local, lv_pct(100), 20);
    lv_label_set_text(lbl_local, "Local:");
    lv_obj_set_grid_cell(lbl_local, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 4, 1);

    lbl_dst_effect = lv_label_create(cont);
    lv_obj_set_size(lbl_dst_effect, lv_pct(100), 20);
    lv_label_set_text(lbl_dst_effect, "DST Effect:");
    lv_obj_set_grid_cell(lbl_dst_effect, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 5, 1);

    lv_obj_t* lbl_7 = lv_label_create(cont);
    lv_obj_set_size(lbl_7, lv_pct(100), 20);
    lv_label_set_text(lbl_7, " ");
    lv_obj_set_grid_cell(lbl_7, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 6, 1);

    lbl_std_offset = lv_label_create(cont);
    lv_obj_set_size(lbl_std_offset, lv_pct(100), 20);
    lv_label_set_text(lbl_std_offset, "STD Offset:");
    lv_obj_set_grid_cell(lbl_std_offset, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 7, 1);

    lbl_dst_offset = lv_label_create(cont);
    lv_obj_set_size(lbl_dst_offset, lv_pct(100), 20);
    lv_label_set_text(lbl_dst_offset, "DST Offset:");
    lv_obj_set_grid_cell(lbl_dst_offset, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 8, 1);

    lbl_total_offset = lv_label_create(cont);
    lv_obj_set_size(lbl_total_offset, lv_pct(100), 20);
    lv_label_set_text(lbl_total_offset, "Total Offset:");
    lv_obj_set_grid_cell(lbl_total_offset, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 9, 1);

    lv_timer_create(update_data, 500, NULL);
   
}




