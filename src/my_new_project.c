#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xF5, 0x43, 0x47, 0xE7, 0xD9, 0x45, 0x4B, 0x59, 0x9A, 0x5C, 0x37, 0xEF, 0xDC, 0xF0, 0x08, 0x59 }
PBL_APP_INFO(MY_UUID,
             "Template App", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
