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

#include <pebble.h>

/* Number of camera settings supported */
#define NUM_SETTINGS 4

/* Structs */
typedef struct {
  char *name;
  int idx;
  int *opts;
  char **optLabels;
} CameraSetting;

/* Data */
static CameraSetting CAMERA_SETTINGS[] = {
  // FIXME setup properly in initialized method
  { .name = "On Camera Voice Control", .idx = 0 }, // .opts = { 0, 1 }, .optLabels = { "ON", "OFF" } },
  { .name = "Beep Volume", .idx = 0}, // .opts = { 0, 1, 3, 4 }, .optLabels = { "HIGH", "MEDIUM", "LOW", "MUTE" } },
  { .name = "Brightness", .idx = 0}, // .opts = { 25, 50, 75, 100 }, .optLabels = { "25", "50", "75", "100" } },
  { .name = "GPS", .idx = 0}, // .opts = { 0, 1 }, .optLabels = { "ON", "OFF" } },
};


/* Main screen */
static Window *s_main_window;
static TextLayer *s_main_hello_text_layer;

/* Settings screen */
static Window *s_settings_window;
static MenuLayer *s_settings_menu_layer;


/* On click of main menu select button, open Settings screen */
static void main_up_click_handler(ClickRecognizerRef recognizer, void *context) {  
  window_stack_push(s_settings_window, true);
}

static void main_click_config_provider(void *context) {
  //window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) main_up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)down_click_handler);
}

static void main_window_load(Window *window) {
  // FIXME sgo temporary text info
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_main_hello_text_layer = text_layer_create((GRect) {
    .origin = { 0, 72 },
    .size = { bounds.size.w, 20 }
  });
  text_layer_set_text(s_main_hello_text_layer, "Hello GoPro5! Press Up for settings,"
    " down to switch modes");
  layer_add_child(window_layer, text_layer_get_layer(s_main_hello_text_layer));
  
  // setup click action for main window
  window_set_click_config_provider(window, (ClickConfigProvider) main_click_config_provider);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_main_hello_text_layer);
}

static uint16_t get_num_rows_settings(struct MenuLayer* menu_layer, uint16_t section_index, void *callback_context) {
  return NUM_SETTINGS;
}

static void draw_row_settings(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
  CameraSetting *setting_array = (CameraSetting*) callback_context;
  CameraSetting *selected = &setting_array[cell_index->row];
  // FIXME print properly
  char optLabel[128];// = itoa(selected->idx); //"wassup"; //selected->optLabels[selected->idx];
  snprintf(optLabel, sizeof(optLabel), "%d", selected->idx);
  
  menu_cell_basic_draw(ctx, cell_layer, selected->name, optLabel, NULL);
}

static void select_click_settings(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  CameraSetting *setting_array = (CameraSetting*) callback_context;
  CameraSetting *selected = &setting_array[cell_index->row];
  selected->idx++;
  menu_layer_reload_data(menu_layer);
  // FIXME sgo toggle settings to the next
}

static void settings_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  s_settings_menu_layer = menu_layer_create(layer_get_bounds(window_layer));
  menu_layer_set_callbacks(s_settings_menu_layer, CAMERA_SETTINGS, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows_settings,
    .draw_row = draw_row_settings,
    .select_click = select_click_settings
  });
  menu_layer_set_click_config_onto_window(s_settings_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_settings_menu_layer));
}

static void settings_window_unload(Window *window) {
  menu_layer_destroy(s_settings_menu_layer);
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