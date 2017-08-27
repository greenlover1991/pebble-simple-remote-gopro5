#include <pebble.h>

/*
 * Simple Pebble watchapp to remotely control GoPro Hero 5
 *
 * 4 Windows:
 *   - Splash screen
 *   - main camera status viewer
 *   - Settings per mode
 *   - camera settings
 *
 * @author Steven Go
 * @since 2017-08-27
 */

/* Main screen */
static Window *s_main_window;
static TextLayer *s_main_hello_text_layer;

/* Settings screen */
static Window *s_settings_window;
static MenuLayer *s_settings_menu_layer;

static void main_window_load(Window *window) {
//   text_layer_create((GRect) {
//     .x = 0, 
//     .y = 72, 
//     .w, h)
//   });
}

static void main_window_unload(Window *window) {
  
}

static void settings_window_load(Window *window) {
  
}

static void settings_window_unload(Window *window) {
  
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  s_settings_window = window_create(); 
  window_set_window_handlers(s_settings_window, (WindowHandlers) {
    .load = settings_window_load,
    .unload = settings_window_unload
  });
  
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_settings_window);
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}