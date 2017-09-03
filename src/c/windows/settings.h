#pragma once

/* Number of camera settings supported */
#define NUM_SETTINGS 4

/* Structs */
typedef struct {
  char *name;
  int idx;
  int *opts;
  char **optLabels;
} CameraSetting;

void settings_window_push();