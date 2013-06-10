#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "hebwatch2.h"


#define MY_UUID { 0x8D, 0x01, 0xF1, 0x43, 0x2E, 0x0F, 0x4B, 0x2D, 0x84, 0xC7, 0xA8, 0x6D, 0x09, 0x0B, 0x97, 0xF4 }
PBL_APP_INFO(MY_UUID,
             "Hebwatch", "Josh Rosenberg",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer normaltime;

char s_time_str_buffer[32];


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&normaltime, window.layer.frame);
  strcpy(s_time_str_buffer, "");
  text_layer_set_text(&normaltime, s_time_str_buffer);
  layer_add_child(&window.layer, &normaltime.layer);

  init_sunrises_sunsets(2456453.523657405749, TIMEZONE, LONGITUDE, LATITUDE, 0);
  init_hour_count_start_end(2456453.523657405749);
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *event) {
    float fracthour = event->tick_time->tm_hour + (event->tick_time->tm_min * 1.0 / 60) + (event->tick_time->tm_sec * 1.0 / 3600) - 1;
    double jc = julian_century(event->tick_time->tm_year + 1900, event->tick_time->tm_mon+1, event->tick_time->tm_mday, fracthour);
  /*  h_m_chelek_string(jc, hour_count_start, hour_count_end, s_time_str_buffer);*/
  string_format_time(s_time_str_buffer, 32, "%I:%M:%S %p", event->tick_time);
  text_layer_set_text(&normaltime, s_time_str_buffer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init, 
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
