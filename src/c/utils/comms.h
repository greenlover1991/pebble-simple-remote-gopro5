#pragma once

/** Request Codes */
#define REQ_CAM_STATUS 0
#define REQ_TRIGGER_SHUTTER 1
#define REQ_STOP_SHUTTER 2


/** True with PebbleKit JS is ready*/
bool comms_is_js_ready();

// User Preferences through settings
bool comms_prefs_flick_to_capture();
bool comms_prefs_vibrate_on_capture();

void comms_send(int request_code);

void comms_init(AppMessageInboxReceived inbox_received_handler,
    AppMessageInboxDropped inbox_dropped_handler,
    AppMessageOutboxSent outbox_sent_handler,
    AppMessageOutboxFailed outbox_failed_handler);
void comms_deinit();