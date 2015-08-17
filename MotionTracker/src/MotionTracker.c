#include "MotionTracker.h"

int main(void)
{
  init();
  app_event_loop();
  deinit();
}

void init(void)
{
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers)
  {
    .load = window_load,
    .unload = window_unload
  });

  window_stack_push(main_window, true);

  accel_data_service_subscribe(, accel_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
}

void deinit(void)
{
  window_destroy(main_window);
}

void window_load(Window* window)
{
  Layer* layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(layer);

  text_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds_size.h));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(text_layer, "Data not acquired");
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
  layer_add_child(layer, text_layer_get_layer(text_layer));
}

void window_unload(Window* window)
{
  text_layer_destroy(text_layer);
}
