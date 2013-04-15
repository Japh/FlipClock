#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0xF5, 0x43, 0x47, 0xE7, 0xD9, 0x45, 0x4B, 0x59, 0x9A, 0x5C, 0x37, 0xEF, 0xDC, 0xF0, 0x08, 0x59 }
PBL_APP_INFO(MY_UUID,
	     "Flip Clock", "Japh",
	     0, 1, /* App major/minor version */
	     RESOURCE_ID_IMAGE_MENU_ICON,
	     APP_INFO_WATCH_FACE);

Window window;


BmpContainer background_image;


const int MONTH_NAME_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_MONTH_NAME_JAN,
  RESOURCE_ID_IMAGE_MONTH_NAME_FEB,
  RESOURCE_ID_IMAGE_MONTH_NAME_MAR,
  RESOURCE_ID_IMAGE_MONTH_NAME_APR,
  RESOURCE_ID_IMAGE_MONTH_NAME_MAY,
  RESOURCE_ID_IMAGE_MONTH_NAME_JUN,
  RESOURCE_ID_IMAGE_MONTH_NAME_JUL,
  RESOURCE_ID_IMAGE_MONTH_NAME_AUG,
  RESOURCE_ID_IMAGE_MONTH_NAME_SEP,
  RESOURCE_ID_IMAGE_MONTH_NAME_OCT,
  RESOURCE_ID_IMAGE_MONTH_NAME_NOV,
  RESOURCE_ID_IMAGE_MONTH_NAME_DEC
};

BmpContainer month_name_image;


#define TOTAL_DATE_DIGITS 2
BmpContainer date_digits_images[TOTAL_DATE_DIGITS];


const int BIG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_NUM_0,
  RESOURCE_ID_IMAGE_NUM_1,
  RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3,
  RESOURCE_ID_IMAGE_NUM_4,
  RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6,
  RESOURCE_ID_IMAGE_NUM_7,
  RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9
};


#define TOTAL_TIME_DIGITS 4
BmpContainer time_digits_images[TOTAL_TIME_DIGITS];


void set_container_image(BmpContainer *bmp_container, const int resource_id, GPoint origin) {

  layer_remove_from_parent(&bmp_container->layer.layer);
  bmp_deinit_container(bmp_container);

  bmp_init_container(resource_id, bmp_container);

  GRect frame = layer_get_frame(&bmp_container->layer.layer);
  frame.origin.x = origin.x;
  frame.origin.y = origin.y;
  layer_set_frame(&bmp_container->layer.layer, frame);

  layer_add_child(&window.layer, &bmp_container->layer.layer);
}


unsigned short get_display_hour(unsigned short hour) {

  if (clock_is_24h_style()) {
    return hour;
  }

  unsigned short display_hour = hour % 12;

  // Converts "0" to "12"
  return display_hour ? display_hour : 12;

}


void update_display(PblTm *current_time) {
  // TODO: Only update changed values?

  set_container_image(&month_name_image, MONTH_NAME_IMAGE_RESOURCE_IDS[current_time->tm_mon], GPoint(74, 0));

  // TODO: Remove leading zero?
  set_container_image(&date_digits_images[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_mday/10], GPoint(1, 0));
  set_container_image(&date_digits_images[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_mday%10], GPoint(36, 0));


  unsigned short display_hour = get_display_hour(current_time->tm_hour);

  // TODO: Remove leading zero?
  set_container_image(&time_digits_images[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(1, 84));
  set_container_image(&time_digits_images[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(36, 84));

  set_container_image(&time_digits_images[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_min/10], GPoint(74, 84));
  set_container_image(&time_digits_images[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_min%10], GPoint(109, 84));

}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  update_display(t->tick_time);
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Flip Clock");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&APP_RESOURCES);

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image);
  layer_add_child(&window.layer, &background_image.layer.layer);


  // Avoids a blank screen on watch start.
  PblTm tick_time;

  get_time(&tick_time);
  update_display(&tick_time);

}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);
  bmp_deinit_container(&month_name_image);

  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    bmp_deinit_container(&date_digits_images[i]);
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    bmp_deinit_container(&time_digits_images[i]);
  }

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
