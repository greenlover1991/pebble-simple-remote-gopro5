/**
 * Camera Settings screen
 */

#include <pebble.h>
#include "settings.h"

/* Data */
static CameraSetting CAMERA_SETTINGS[] = {
  // FIXME setup properly in initialized method
  { .name = "On Camera Voice Control", .idx = 0 }, // .opts = { 0, 1 }, .optLabels = { "ON", "OFF" } },
  { .name = "Beep Volume", .idx = 0}, // .opts = { 0, 1, 3, 4 }, .optLabels = { "HIGH", "MEDIUM", "LOW", "MUTE" } },
  { .name = "Brightness", .idx = 0}, // .opts = { 25, 50, 75, 100 }, .optLabels = { "25", "50", "75", "100" } },
  { .name = "GPS", .idx = 0}, // .opts = { 0, 1 }, .optLabels = { "ON", "OFF" } },
};

static Window *s_window;
static MenuLayer *s_menu_layer;

static uint16_t get_num_rows(struct MenuLayer* menu_layer, uint16_t section_index, void *callback_context) {
  return NUM_SETTINGS;
}

static void draw_row(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
  CameraSetting *setting_array = (CameraSetting*) callback_context;
  CameraSetting *selected = &setting_array[cell_index->row];
  // FIXME print properly
  char optLabel[128];// = itoa(selected->idx); //"wassup"; //selected->optLabels[selected->idx];
  snprintf(optLabel, sizeof(optLabel), "%d", selected->idx);
  
  menu_cell_basic_draw(ctx, cell_layer, selected->name, optLabel, NULL);
}

static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  CameraSetting *setting_array = (CameraSetting*) callback_context;
  CameraSetting *selected = &setting_array[cell_index->row];
  selected->idx++;
  menu_layer_reload_data(menu_layer);
  // FIXME sgo toggle settings to the next
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  s_menu_layer = menu_layer_create(layer_get_bounds(window_layer));
  menu_layer_set_callbacks(s_menu_layer, CAMERA_SETTINGS, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows,
    .draw_row = draw_row,
    .select_click = select_click
  });
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  
  window_destroy(window);
  s_window = NULL;
}

void settings_window_push() {
  if(!s_window) {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_window, true);
}