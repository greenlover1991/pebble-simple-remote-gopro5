/*
 * Simple Pebble watchapp to remotely control GoPro Hero 5
 *
 * 4 Windows:
 *   - (this) splash screen
 *   - main camera status viewer (3 modes)
 *   - Settings per mode
 *   - camera settings
 *
 * @author Steven Go
 * @since 2017-08-27
 */

#include <pebble.h>
#include "utils/comms.h"
#include "windows/main_camera.h"

static Window *s_window;
static TextLayer *s_hello_text_layer;

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "local inbox received handler");
  
  if(dict_find(iter, MESSAGE_KEY_JS_READY) && comms_is_js_ready()) {
    // FIXME sgo correct the message or use progress dialog
    text_layer_set_text(s_hello_text_layer, "JS Ready! Checking camera status...");
    comms_send(REQ_CAM_STATUS);
    return;
  }
  
  // FIXME sgo warp these app message 
  int req_code = dict_find(iter, MESSAGE_KEY_REQ_CODE)->value->int32;
  int resp_code = dict_find(iter, MESSAGE_KEY_RESP_CODE)->value->int32;
  char *resp_msg = dict_find(iter, MESSAGE_KEY_RESP_MESSAGE)->value->cstring;
  
  if (req_code == REQ_CAM_STATUS) {
    // FIXME need to convert the status JSON to a flat dictionary
    // FIXME MESSAGE_KEY_STATUS_ARR, same for settings
    // FIXME sgo correct the message
    
    if (resp_code == 200) {
      text_layer_set_text(s_hello_text_layer, "CAMERA READY!");
      main_camera_window_push();  
    } else {
      text_layer_set_text(s_hello_text_layer, "CAMERA responded non-200");
    }
  } else {
    // TODO sgo correct message
    text_layer_set_text(s_hello_text_layer, "Unknown request");
  }
}

static void window_load(Window *window) {
  // FIXME sgo temporary text info check connection status
  // if successful, pop this window,
  // then push main_camera
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_hello_text_layer = text_layer_create((GRect) {
    .origin = { 0, 72 },
    .size = { bounds.size.w, 20 }
  });
  text_layer_set_text(s_hello_text_layer, "Hello GoPro5! Checking connection...");
  layer_add_child(window_layer, text_layer_get_layer(s_hello_text_layer));
  comms_init(inbox_received_handler, NULL, // FIXME sgo handle no app inbox connection
             NULL, NULL);
}

static void window_unload(Window *window) {
  comms_deinit();
  text_layer_destroy(s_hello_text_layer);
}

static void init() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  window_stack_push(s_window, true);
}

static void deinit() {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}