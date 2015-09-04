#ifndef MOTION_TRACKER_H
#define MOTION_TRACKER_H
#include <pebble.h>
#define N_SAMPLES_PER_PROCESS 10
#define true 1
#define false 0

extern Window* main_window;
extern TextLayer* text_layer;
extern int force;
extern int num_heavy;

int main(void);
void init(void);
void deinit(void);

void window_load(Window* window);
void window_unload(Window* window);

void accel_data_handler(AccelData* data, uint32_t n_samples);
#endif
