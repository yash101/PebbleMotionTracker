#include "MotionTracker.h"
#include <math.h>

int main(void)
{
  init();
  app_event_loop();
  deinit();
}

void down_single_click_handler(ClickRecognizerRef recog, void* contect)
{
  force -= 100;
  if(force < 0) force = 0;
}

void up_single_click_handler(ClickRecognizerRef recog, void* context)
{
  force += 100;
  if(force > 4080) force = 4080;
}

void click_config_handler(Window* window)
{
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
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

  accel_data_service_subscribe(N_SAMPLES_PER_PROCESS, accel_data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);

  window_set_click_config_provider(main_window, (ClickConfigProvider) click_config_handler);
}

void deinit(void)
{
  window_destroy(main_window);
}

void window_load(Window* window)
{
  Layer* layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(layer);

  text_layer = text_layer_create(GRect(5, 0, bounds.size.w - 10, bounds.size.h));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(text_layer, "Data not acquired");
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
  layer_add_child(layer, text_layer_get_layer(text_layer));
}

void window_unload(Window* window)
{
  text_layer_destroy(text_layer);
}

int d_abs(int num)
{
  if(num < 0) return 0 - num;
  return num;
}

void accel_data_handler(AccelData* data, uint32_t n)
{
  bool vibrate = false;

  int mx = 0;

  for(uint32_t i = 0; i < n; i++)
  {
    if(d_abs(data[i].x) > mx)
    {
      mx = data[i].x;
    }

    if(d_abs(data[i].x) >= force)
    {
      vibrate = true;
    }
  }

  if(vibrate == true)
  {
    vibes_short_pulse();
    num_heavy++;
  }

  else
  {
    vibes_cancel();
  }

  static char buffer[2048];
  snprintf(buffer, sizeof(buffer), "Vibrating: %s\nHard Hits: %d\nThreshold: +/-%d mg\nMax: %d",
    vibrate ? "True" : "False",
    num_heavy,
    force,
    mx
  );

  text_layer_set_text(text_layer, buffer);
}

