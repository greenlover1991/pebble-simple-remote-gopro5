/**
 * Main window and camera mode status viewer with modes:
 * 1. Camera
 * 2. Video
 * 3. Timelapse
 */

#include <pebble.h>
#include "../utils/comms.h"
#include "main_camera.h"
#include "settings.h"

static Window* s_window;
static TextLayer* s_hello_text_layer;

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  if(!dict_find(iter, MESSAGE_KEY_JS_READY)) {
    int req_code = dict_find(iter, MESSAGE_KEY_REQ_CODE)->value->int32;
    int resp_code = dict_find(iter, MESSAGE_KEY_RESP_CODE)->value->int32;
    char *resp_msg = dict_find(iter, MESSAGE_KEY_RESP_MESSAGE)->value->cstring;
    if (resp_code == 200) {
      text_layer_set_text(s_hello_text_layer, "Captured success");
    } else {
      text_layer_set_text(s_hello_text_layer, "Capturing failed.");
    }
  }
}


/* On click of up button, open Settings screen */
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {  
  settings_window_push();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (comms_is_js_ready()) {
    text_layer_set_text(s_hello_text_layer, "Capturing...");
    comms_send(REQ_TRIGGER_SHUTTER);
  } else {
    text_layer_set_text(s_hello_text_layer, "JS NOT READY...");
  } 
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)down_click_handler);
}

static void window_load(Window *window) {
  // FIXME sgo temporary text info and other info
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_hello_text_layer = text_layer_create((GRect) {
    .origin = { 0, 72 },
    .size = { bounds.size.w, 20 }
  });
  text_layer_set_text(s_hello_text_layer, "Up=Settings, select to capture,"
    " down to switch modes");
  layer_add_child(window_layer, text_layer_get_layer(s_hello_text_layer));
  
  window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);
  
  comms_init(inbox_received_handler, NULL, // FIXME sgo handle no app inbox connection
             NULL, NULL);
}

static void window_unload(Window *window) {
  text_layer_destroy(s_hello_text_layer);
  window_destroy(window);
  s_window = NULL;
}

void main_camera_window_push() {
  if(!s_window) {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_window, true);
}