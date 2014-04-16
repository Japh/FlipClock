#include <pebble.h>

// Settings (bit) flags
enum {
  SETTING_INVERTED        = 1 << 0,
  SETTING_12H_TIME        = 1 << 1,
  SETTING_DATE_ABOVE_TIME = 1 << 2,
  SETTING_DATE_BELOW_TIME = 1 << 3,
  SETTING_TIME_ONLY       = 1 << 4,
};

// Settings AppSync keys; correspond to appKeys in appinfo.json
enum {
  SETTING_SYNC_KEY_INVERTED  = 0,
  SETTING_SYNC_KEY_12H_TIME  = 1,
  SETTING_SYNC_KEY_SHOW_DATE = 2,
};

// Numerical IDs for "digit" layers
enum {
  MINUTE_ONES = 0,
  MINUTE_TENS = 1,
  HOUR_ONES   = 2,
  HOUR_TENS   = 3,
  DAY_ONES    = 4,
  DAY_TENS    = 5,
  MONTH       = 6
};

/**
 *  Numerical IDs for bitmap layers
 *
 *  Each digit is displayed using two images: a top half and a bottom half.
 *  Each half of a digit also has an empty layer floating above it that is
 *  used for displaying the animations.
 */
enum {
  // months
  MONTH_TOP = 0,
  MONTH_BOT = 1,
  MONTH_TOP_ANIM = 2,
  MONTH_BOT_ANIM = 3,

  // digits
  MINUTE_ONES_TOP = 0,
  MINUTE_ONES_BOT = 1,
  MINUTE_TENS_TOP = 2,
  MINUTE_TENS_BOT = 3,
  HOUR_ONES_TOP = 4,
  HOUR_ONES_BOT = 5,
  HOUR_TENS_TOP = 6,
  HOUR_TENS_BOT = 7,
  DAY_ONES_TOP  = 8,
  DAY_ONES_BOT  = 9,
  DAY_TENS_TOP  = 10,
  DAY_TENS_BOT  = 11,
  MINUTE_ONES_TOP_ANIM = 12,
  MINUTE_ONES_BOT_ANIM = 13,
  MINUTE_TENS_TOP_ANIM = 14,
  MINUTE_TENS_BOT_ANIM = 15,
  HOUR_ONES_TOP_ANIM   = 16,
  HOUR_ONES_BOT_ANIM   = 17,
  HOUR_TENS_TOP_ANIM   = 18,
  HOUR_TENS_BOT_ANIM   = 19,
  DAY_ONES_TOP_ANIM    = 20,
  DAY_ONES_BOT_ANIM    = 21,
  DAY_TENS_TOP_ANIM    = 22,
  DAY_TENS_BOT_ANIM    = 23,
};

/**
 * Struct to hold information about an animation.
 *
 */
typedef struct AnimationContext {
  uint8_t layer_id;   /**< The numerical ID of the bitmap layer to be animated */
  uint8_t prev_value; /**< The old digit */
  uint8_t value;      /**< The new digit */
} AnimationContext;

// Array of resource IDs for the digit images.
static const uint8_t DIGITS[] = {
  RESOURCE_ID_IMAGE_ZERO_TOP_WHITE,    // 0
  RESOURCE_ID_IMAGE_ZERO_TOP_45_WHITE, // 1
  RESOURCE_ID_IMAGE_ZERO_TOP_60_WHITE, // 2
  RESOURCE_ID_IMAGE_ZERO_TOP_75_WHITE, // 3
  RESOURCE_ID_IMAGE_ZERO_BOT_WHITE,    // 4
  RESOURCE_ID_IMAGE_ZERO_BOT_75_WHITE, // 5
  RESOURCE_ID_IMAGE_ZERO_BOT_60_WHITE, // 6
  RESOURCE_ID_IMAGE_ZERO_BOT_45_WHITE, // 7
  RESOURCE_ID_IMAGE_ONE_TOP_WHITE,     // 8
  RESOURCE_ID_IMAGE_ONE_TOP_45_WHITE,  // 9
  RESOURCE_ID_IMAGE_ONE_TOP_60_WHITE,  // 10
  RESOURCE_ID_IMAGE_ONE_TOP_75_WHITE,  // 11
  RESOURCE_ID_IMAGE_ONE_BOT_WHITE,     // 12
  RESOURCE_ID_IMAGE_ONE_BOT_75_WHITE,  // 13
  RESOURCE_ID_IMAGE_ONE_BOT_60_WHITE,  // 14
  RESOURCE_ID_IMAGE_ONE_BOT_45_WHITE,  // 15
  RESOURCE_ID_IMAGE_TWO_TOP_WHITE,     // 16
  RESOURCE_ID_IMAGE_TWO_TOP_45_WHITE,  // 17
  RESOURCE_ID_IMAGE_TWO_TOP_60_WHITE,  // 18
  RESOURCE_ID_IMAGE_TWO_TOP_75_WHITE,  // 19
  RESOURCE_ID_IMAGE_TWO_BOT_WHITE,     // 20
  RESOURCE_ID_IMAGE_TWO_BOT_75_WHITE,  // 21
  RESOURCE_ID_IMAGE_TWO_BOT_60_WHITE,  // 22
  RESOURCE_ID_IMAGE_TWO_BOT_45_WHITE,  // 23
  RESOURCE_ID_IMAGE_THREE_TOP_WHITE,   // 24
  RESOURCE_ID_IMAGE_THREE_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_THREE_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_THREE_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_THREE_BOT_WHITE,
  RESOURCE_ID_IMAGE_THREE_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_THREE_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_THREE_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_FOUR_TOP_WHITE,
  RESOURCE_ID_IMAGE_FOUR_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_FOUR_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_FOUR_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_FOUR_BOT_WHITE,
  RESOURCE_ID_IMAGE_FOUR_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_FOUR_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_FOUR_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_FIVE_TOP_WHITE,
  RESOURCE_ID_IMAGE_FIVE_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_FIVE_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_FIVE_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_FIVE_BOT_WHITE,
  RESOURCE_ID_IMAGE_FIVE_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_FIVE_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_FIVE_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_SIX_TOP_WHITE,
  RESOURCE_ID_IMAGE_SIX_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_SIX_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_SIX_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_SIX_BOT_WHITE,
  RESOURCE_ID_IMAGE_SIX_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_SIX_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_SIX_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_TOP_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_BOT_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_SEVEN_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_TOP_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_BOT_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_EIGHT_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_NINE_TOP_WHITE,
  RESOURCE_ID_IMAGE_NINE_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_NINE_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_NINE_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_NINE_BOT_WHITE,
  RESOURCE_ID_IMAGE_NINE_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_NINE_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_NINE_BOT_45_WHITE,
};

// Array of resource IDs for the digit images.
/*static const uint16_t MONTHS[] = {
  RESOURCE_ID_IMAGE_MONTH_01_TOP_WHITE,    // 0
  RESOURCE_ID_IMAGE_MONTH_01_TOP_45_WHITE, // 1
  RESOURCE_ID_IMAGE_MONTH_01_TOP_60_WHITE, // 2
  RESOURCE_ID_IMAGE_MONTH_01_TOP_75_WHITE, // 3
  RESOURCE_ID_IMAGE_MONTH_01_BOT_WHITE,    // 4
  RESOURCE_ID_IMAGE_MONTH_01_BOT_75_WHITE, // 5
  RESOURCE_ID_IMAGE_MONTH_01_BOT_60_WHITE, // 6
  RESOURCE_ID_IMAGE_MONTH_01_BOT_45_WHITE, // 7
  RESOURCE_ID_IMAGE_MONTH_02_TOP_WHITE,    // 8
  RESOURCE_ID_IMAGE_MONTH_02_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_02_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_03_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_04_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_05_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_06_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_07_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_08_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_09_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_10_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_11_BOT_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_TOP_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_TOP_45_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_TOP_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_TOP_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_BOT_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_BOT_75_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_BOT_60_WHITE,
  RESOURCE_ID_IMAGE_MONTH_12_BOT_45_WHITE,
};
*/

#define ANIMATION_FRAMES 7

static Window *window;
static GBitmap *background_bitmap;
static GBitmap *digit_bitmap[12]; /**< Bitmaps for displaying the current time */
static GBitmap *month_bitmap[2]; /**< Bitmaps for displaying the current month */
static GBitmap *animation_bitmap[7][ANIMATION_FRAMES]; /**< Bitmaps for displaying animations */
static BitmapLayer *background_bitmap_layer;
static BitmapLayer *digit_bitmap_layer[24]; /**< Bitmap layers for displaying the current time and digit animations */
static BitmapLayer *month_bitmap_layer[4]; /**< Bitmap layers for displaying the current month */
static InverterLayer *inverter_layer;
static Animation *animation[7];
static AnimationImplementation animation_implementation;
static struct tm *prev_timeinfo, *timeinfo;
static AnimationContext context[7]; /**< Data to be passed to animation callbacks */
static AppSync settings_sync; /**< Keeps settings in sync between phone and watch */
static uint8_t settings_sync_buffer[34]; /**< Buffer used by settings sync */
static uint8_t settings; /**< Current settings (as bit flags) */

static void window_load(Window *window);
static void window_unload(Window *window);

/**
 * Prepares the animation for running. This callback is called when the animation is added to the scheduler.
 *
 * @param animation The animation that needs to be set up.
 */
static void setup_animation(struct Animation *animation) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "setup animation");
  const AnimationContext* const anim_context = (AnimationContext*)animation_get_context(animation);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  context: %d,%d,%d", anim_context->layer_id, anim_context->prev_value, anim_context->value);

  const uint8_t i = anim_context->prev_value * (ANIMATION_FRAMES + 1); // Index of top half of current digit in DIGITS array
  const uint8_t j = anim_context->value * (ANIMATION_FRAMES + 1); // Index of top half of next digit in DIGITS array

  // Load bitmaps needed for animation
  animation_bitmap[anim_context->layer_id][0] = gbitmap_create_with_resource(DIGITS[i+1]); // top half of current digit rotated 45°
  animation_bitmap[anim_context->layer_id][1] = gbitmap_create_with_resource(DIGITS[i+2]); // top half of current digit rotated 60°
  animation_bitmap[anim_context->layer_id][2] = gbitmap_create_with_resource(DIGITS[i+3]); // top half of current digit rotated 75°
  animation_bitmap[anim_context->layer_id][3] = NULL; // no animation bitmap is shown at 90° (just pretend it's there, sticking straight out of the screen)
  animation_bitmap[anim_context->layer_id][4] = gbitmap_create_with_resource(DIGITS[j+5]); // bottom half of next digit rotated 75°
  animation_bitmap[anim_context->layer_id][5] = gbitmap_create_with_resource(DIGITS[j+6]); // bottom half of next digit rotated 60°
  animation_bitmap[anim_context->layer_id][6] = gbitmap_create_with_resource(DIGITS[j+7]); // bottom half of next digit rotated 45°

  // Replace top bitmap with new number, e..g replace 1 with 2, replace 2 with 3, etc.
  gbitmap_destroy(digit_bitmap[anim_context->layer_id]);
  digit_bitmap[anim_context->layer_id] = gbitmap_create_with_resource(DIGITS[j]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id], digit_bitmap[anim_context->layer_id]);
}

/**
 * Updates the animation according to the given normalized time. This callback
 * will be called repeatedly by the animation scheduler whenever the animation
 * needs to be updated.
 *
 * @param animation       The animation that needs to update; gets passed in by
 *                        the animation framework.
 * @param time_normalized The current normalized time; gets passed in by the
 *                        animation framework for each animation frame
 */
static void update_animation(struct Animation *animation, const uint32_t time_normalized) {
  const uint16_t time = (uint16_t)(time_normalized * 1000 / ANIMATION_NORMALIZED_MAX);
  const uint8_t i = time / (1000 / ANIMATION_FRAMES);

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "update animation: %d, %d, %d", (int)time_normalized, time, i);
  const AnimationContext* const anim_context = (AnimationContext*)animation_get_context(animation);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "  context: %d,%d,%d", anim_context->layer_id, anim_context->prev_value, anim_context->value);
  const uint8_t j = anim_context->value * (ANIMATION_FRAMES + 1); // Index of top half of next digit in DIGITS array

  // Clear animation bitmaps
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+12], NULL);
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+13], NULL);

  if(time == 0) {
    // Replace top bitmap with new number, e..g replace 1 with 2, replace 2 with 3, etc.
  } else if(time == 1000) {
    // Replace bottom bitmap with new number, e.g. replace 1 with 2, replace 2 with 3, etc.
    gbitmap_destroy(digit_bitmap[anim_context->layer_id+1]);
    digit_bitmap[anim_context->layer_id+1] = gbitmap_create_with_resource(DIGITS[j+4]);
    bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+1], digit_bitmap[anim_context->layer_id+1]);
  } else {
    // Replace animation bitmap
    if(i < 3) {
      bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+12], animation_bitmap[anim_context->layer_id][i]);
    } else if(i > 3) {
      bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+13], animation_bitmap[anim_context->layer_id][i]);
    }
  }
}

/**
 * This callback is called when the animation is removed from the schedule.
 *
 * @param animation The animation that needs to be teared down.
 */
static void teardown_animation(struct Animation *animation) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "teardown animation");
  const AnimationContext* const anim_context = (AnimationContext*)animation_get_context(animation);
  const uint8_t j = anim_context->value * (ANIMATION_FRAMES + 1);
  uint8_t i = 0;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  context: %d,%d,%d", anim_context->layer_id, anim_context->prev_value, anim_context->value);

  // Replace bottom bitmap with new number, e.g. replace 1 with 2, replace 2 with 3, etc.
  // This should have happened in the last step of update_animation(...), but it's never guaranteed (the Pebble seems to stutter sometimes)
  gbitmap_destroy(digit_bitmap[anim_context->layer_id+1]);
  digit_bitmap[anim_context->layer_id+1] = gbitmap_create_with_resource(DIGITS[j+4]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+1], digit_bitmap[anim_context->layer_id+1]);

  // Clear animation layers
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+12], NULL);
  bitmap_layer_set_bitmap(digit_bitmap_layer[anim_context->layer_id+13], NULL);

  // Destroy animation bitmaps (they are no longer needed)
  for(i = 0; i < ANIMATION_FRAMES; i++) {
    gbitmap_destroy(animation_bitmap[anim_context->layer_id][i]);
  }
}

/**
 * Animate a single digit.
 *
 * @param layer_id ID of digit layer to be animated
 * @param old      Old digit
 * @param new      New digit
 */
static void animate_digit(const uint8_t layer_id, const uint8_t old, const uint8_t new) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "animate_digit(%d, %d, %d)", layer_id, old, new);

  // No animation for digit that has not changed
  if(new == old) { return; }

  // Determine the ID of the animation layer
  const uint8_t anim_layer_id = layer_id * 2;

  if(layer_id == DAY_ONES) { // 4
    //anim_layer_id = DAY_ONES_TOP;
    return;
  } else if(layer_id == DAY_TENS) { // 5
    //anim_layer_id = DAY_TENS_TOP;
    return;
  } else if(layer_id == MONTH) { // 6
    return;
  }

  // Set animation handlers - actually, all we care about is setting the
  // context so that we can pass values to the AnimationImplementation
  // callbacks. The .started and .stoped handlers are not used.
  AnimationHandlers handlers = { .started = NULL, .stopped = NULL };
  context[layer_id] = (AnimationContext){ .layer_id = anim_layer_id, .prev_value = old, .value = new };
  animation_set_handlers(animation[layer_id], handlers, &context[layer_id]);

  // Schedule the animation
  animation_schedule(animation[layer_id]);
}

/**
 * Called once per minute to update time display.
 *
 * If this funciton has been called, then we know that (at least) the minute
 * has changed. It's also possible that the hour has changed, the day, the
 * month, or even the year. `timeinfo` contains all of the latest time data we
 * need. `prev_timeinfo` contains the time data for the previous minute, i.e.
 * the last time this function was called.
 *
 * @param tick_time     The time at which the tick event was triggered
 * @param units_changes Which unit change triggered this tick event
 */
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_minute_tick - %d:%d", tick_time->tm_hour, tick_time->tm_min);

  // Store previous timeinfo, and update current timeinfo
  *prev_timeinfo = *timeinfo;
  *timeinfo = *tick_time;
    APP_LOG(APP_LOG_LEVEL_DEBUG, " prev_time - %d:%d", prev_timeinfo->tm_hour, prev_timeinfo->tm_min);
    APP_LOG(APP_LOG_LEVEL_DEBUG, " curr_time - %d:%d", timeinfo->tm_hour, timeinfo->tm_min);

  uint8_t hour = timeinfo->tm_hour, prev_hour = prev_timeinfo->tm_hour;
  const uint8_t min = timeinfo->tm_min, prev_min = prev_timeinfo->tm_min;

  // 12H time?
  if((settings & SETTING_12H_TIME) > 0) {
    if(hour == 0) { hour = 12; }
    else if(hour > 12) { hour -= 12; }

    if(prev_hour == 0) { prev_hour = 12; }
    else if(prev_hour > 12) { prev_hour -= 12; }
  }

  // Break hours and minutes into component digits
  const uint8_t min_tens = min / 10,
    min_ones = min % 10,
    hour_tens = hour / 10,
    hour_ones = hour % 10,
    prev_min_tens = prev_min / 10,
    prev_min_ones = prev_min % 10,
    prev_hour_tens = prev_hour / 10,
    prev_hour_ones = prev_hour % 10;

  // Schedule animations
  animate_digit(MINUTE_ONES, prev_min_ones, min_ones);
  if(min_tens != prev_min_tens) {
    animate_digit(MINUTE_TENS, prev_min_tens, min_tens);
  }
  if(hour_ones != prev_hour_ones) {
    animate_digit(HOUR_ONES, prev_hour_ones, hour_ones);
  }
  if(hour_tens != prev_hour_tens) {
    animate_digit(HOUR_TENS, prev_hour_tens, hour_tens);
  }
}

/**
 * Called when there is a settings sync error.
 *
 * @see https://developer.getpebble.com/2/api-reference/group___app_sync.html#ga144a1a8d8050f8f279b11cfb5d526212
 */
static void settings_sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Sync Error: %d", app_message_error);
}

/**
 * Called when a settings tuple has changed.
 *
 * @see https://developer.getpebble.com/2/api-reference/group___app_sync.html#ga448af36883189f6345cc7d5cd8a3cc29
 */
static void settings_sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  uint8_t new = (uint8_t) new_tuple->value->uint8;
  uint8_t old_settings = settings;

  switch(key) {
    case SETTING_SYNC_KEY_12H_TIME:
      if (0 == new) settings = settings & ~SETTING_12H_TIME;
      else settings = settings | SETTING_12H_TIME;
      break;
    case SETTING_SYNC_KEY_INVERTED:
      if (0 == new) settings = settings & ~SETTING_INVERTED;
      else settings = settings | SETTING_INVERTED;
      break;
    case SETTING_SYNC_KEY_SHOW_DATE:
      settings = settings & ~(SETTING_TIME_ONLY + SETTING_DATE_ABOVE_TIME + SETTING_DATE_BELOW_TIME);
      settings = settings | new;
      break;
  }

  if(settings != old_settings) {
    // Destroy and re-create window
    window_unload(window);
    window_load(window);
  }
}

/**
 * Called when the window is pushed to the screen when it's not loaded.
 *
 * Create layout.
 *
 * @param window Pointer to Window object
 */
static void window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window load");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GRect image_frame = (GRect) { .origin = GPoint(104, 85), .size = GSize(44, 42) };

  //const uint8_t n = sizeof(animation_bitmap) / sizeof(*animation_bitmap);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "size of animation_bitmap: %d, %d", sizeof(animation_bitmap), sizeof(*animation_bitmap));
  //memset(&animation_bitmap, 0, sizeof(animation_bitmap));
  //static GBitmap *animation_bitmap[7][ANIMATION_FRAMES]; /**< Bitmaps for displaying animations */

  // Load background
  background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  GRect bg_bounds = (GRect) { .origin = GPoint(0, 0), .size = background_bitmap->bounds.size };
  //background_bitmap_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  //background_bitmap_layer = bitmap_layer_create(GRect(0, 0, background_bitmap->bounds.size.w, background_bitmap->bounds.size.h));
  if((settings & SETTING_DATE_BELOW_TIME) > 0) {
    bg_bounds.origin.y = -84;
  } else if((settings & SETTING_TIME_ONLY) > 0) {
    bg_bounds.origin.y = -216;
  }
  background_bitmap_layer = bitmap_layer_create(bg_bounds);
  bitmap_layer_set_bitmap(background_bitmap_layer, background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_bitmap_layer));

  // Initialize graphics layer - Minute ones (top half)
  if((settings & SETTING_TIME_ONLY) > 0) {
    image_frame.origin.y = (bounds.size.h / 2) - 42;
  } else if((settings & SETTING_DATE_BELOW_TIME) > 0) {
    image_frame.origin.y = 0;
  }
  digit_bitmap_layer[MINUTE_ONES_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_ONES_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_ONES_TOP], GCompOpOr);
  //bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_ONES_TOP], digit_bitmap[MINUTE_ONES_TOP]);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_ONES_TOP]));

  // Initialize graphics layer - Minute ones (top half animation layer)
  digit_bitmap_layer[MINUTE_ONES_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_ONES_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_ONES_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_ONES_TOP_ANIM]));

  // Initialize graphics layer - Minute ones (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[MINUTE_ONES_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_ONES_BOT], GAlignTop);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_ONES_BOT], GCompOpOr);
  //bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_ONES_BOT], digit_bitmap[MINUTE_ONES_BOT]);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_ONES_BOT]));

  // Initialize graphics layer - Minute ones (bottom half animation layer)
  digit_bitmap_layer[MINUTE_ONES_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_ONES_BOT_ANIM], GAlignTop);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_ONES_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_ONES_BOT_ANIM]));

  // Initialize graphics layer - Minute tens (top half)
  image_frame.origin.x = 69;
  image_frame.origin.y -= 42;
  digit_bitmap_layer[MINUTE_TENS_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_TENS_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_TENS_TOP], GCompOpOr);
  //bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_TENS_TOP], digit_bitmap[MINUTE_TENS_TOP]);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_TENS_TOP]));

  // Initialize graphics layer - Minute tens (top half animation layer)
  digit_bitmap_layer[MINUTE_TENS_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_TENS_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_TENS_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_TENS_TOP_ANIM]));

  // Initialize graphics layer - Minute tens (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[MINUTE_TENS_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_TENS_BOT], GAlignTop);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_TENS_BOT], GCompOpOr);
  //bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_TENS_BOT], digit_bitmap[MINUTE_TENS_BOT]);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_TENS_BOT]));

  // Initialize graphics layer - Minute tens (bottom half animation layer)
  digit_bitmap_layer[MINUTE_TENS_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[MINUTE_TENS_BOT_ANIM], GAlignTop);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[MINUTE_TENS_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[MINUTE_TENS_BOT_ANIM]));

  // Initialize graphics layer - Hour ones (top half)
  image_frame.origin.x = 31;
  image_frame.origin.y -= 42;
  digit_bitmap_layer[HOUR_ONES_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_ONES_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_ONES_TOP], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_ONES_TOP]));

  // Initialize graphics layer - Hour ones (top half animation layer)
  digit_bitmap_layer[HOUR_ONES_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_ONES_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_ONES_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_ONES_TOP_ANIM]));

  // Initialize graphics layer - Hour ones (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[HOUR_ONES_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_ONES_BOT], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_ONES_BOT], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_ONES_BOT]));

  // Initialize graphics layer - Hour ones (bottom half animation layer)
  digit_bitmap_layer[HOUR_ONES_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_ONES_BOT_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_ONES_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_ONES_BOT_ANIM]));

  // Initialize graphics layer - Hour tens (top half)
  image_frame.origin.x = -4;
  image_frame.origin.y -= 42;
  digit_bitmap_layer[HOUR_TENS_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_TENS_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_TENS_TOP], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_TENS_TOP]));

  // Initialize graphics layer - Hour tens (top half animation layer)
  digit_bitmap_layer[HOUR_TENS_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_TENS_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_TENS_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_TENS_TOP_ANIM]));

  // Initialize graphics layer - Hour tens (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[HOUR_TENS_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_TENS_BOT], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_TENS_BOT], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_TENS_BOT]));

  // Initialize graphics layer - Hour tens (bottom half animation layer)
  digit_bitmap_layer[HOUR_TENS_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[HOUR_TENS_BOT_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[HOUR_TENS_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[HOUR_TENS_BOT_ANIM]));

  // Initialize graphics layer - Day ones (top half)
  image_frame.origin.x = 31;
  image_frame.origin.y = 0;
  if((settings & SETTING_DATE_BELOW_TIME) > 0) {
    image_frame.origin.y = 85;
  }
  digit_bitmap_layer[DAY_ONES_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_ONES_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_ONES_TOP], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_TOP]));

  // Initialize graphics layer - Day ones (top half animation layer)
  digit_bitmap_layer[DAY_ONES_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_ONES_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_ONES_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_TOP_ANIM]));

  // Initialize graphics layer - Day ones (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[DAY_ONES_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_ONES_BOT], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_ONES_BOT], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_BOT]));

  // Initialize graphics layer - Day ones (bottom half animation layer)
  digit_bitmap_layer[DAY_ONES_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_ONES_BOT_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_ONES_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_BOT_ANIM]));

  // Initialize graphics layer - Day tens (top half)
  image_frame.origin.x = -4;
  image_frame.origin.y -= 42;
  digit_bitmap_layer[DAY_TENS_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_TENS_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_TENS_TOP], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_TOP]));

  // Initialize graphics layer - Day tens (top half animation layer)
  digit_bitmap_layer[DAY_TENS_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_TENS_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_TENS_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_TOP_ANIM]));

  // Initialize graphics layer - Day tens (bottom half)
  image_frame.origin.y += 42;
  digit_bitmap_layer[DAY_TENS_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_TENS_BOT], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_TENS_BOT], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_BOT]));

  // Initialize graphics layer - Day tens (bottom half animation layer)
  digit_bitmap_layer[DAY_TENS_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(digit_bitmap_layer[DAY_TENS_BOT_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(digit_bitmap_layer[DAY_TENS_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_BOT_ANIM]));

  // Initialize graphics layer - Month (top half)
  image_frame.origin.x = 65;
  image_frame.origin.y -= 42;
  month_bitmap_layer[MONTH_TOP] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(month_bitmap_layer[MONTH_TOP], GAlignBottom);
  bitmap_layer_set_compositing_mode(month_bitmap_layer[MONTH_TOP], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(month_bitmap_layer[MONTH_TOP]));

  // Initialize graphics layer - Month (top half animation layer)
  month_bitmap_layer[MONTH_TOP_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(month_bitmap_layer[MONTH_TOP_ANIM], GAlignBottom);
  bitmap_layer_set_compositing_mode(month_bitmap_layer[MONTH_TOP_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(month_bitmap_layer[MONTH_TOP_ANIM]));

  // Initialize graphics layer - Month (bottom half)
  image_frame.origin.y += 42;
  month_bitmap_layer[MONTH_BOT] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(month_bitmap_layer[MONTH_TOP], GAlignTop);
  bitmap_layer_set_compositing_mode(month_bitmap_layer[MONTH_BOT], GCompOpOr);
  layer_add_child(window_layer, bitmap_layer_get_layer(month_bitmap_layer[MONTH_BOT]));

  // Initialize graphics layer - Month (bottom half animation layer)
  month_bitmap_layer[MONTH_BOT_ANIM] = bitmap_layer_create(image_frame);
  bitmap_layer_set_alignment(month_bitmap_layer[MONTH_BOT_ANIM], GAlignTop);
  bitmap_layer_set_compositing_mode(month_bitmap_layer[MONTH_BOT_ANIM], GCompOpAssign);
  layer_add_child(window_layer, bitmap_layer_get_layer(month_bitmap_layer[MONTH_BOT_ANIM]));

  if((settings & SETTING_TIME_ONLY) > 0) {
    // Hide date layers
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_TOP]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_BOT]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_TOP]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_BOT]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_TOP_ANIM]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_ONES_BOT_ANIM]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_TOP_ANIM]), true);
    layer_set_hidden(bitmap_layer_get_layer(digit_bitmap_layer[DAY_TENS_BOT_ANIM]), true);
    //layer_set_hidden(bitmap_layer_get_layer(month_bitmap_layer[MONTH_TOP]), true);
    //layer_set_hidden(bitmap_layer_get_layer(month_bitmap_layer[MONTH_BOT]), true);
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "size of digit_bitmap_layer: %d", sizeof(digit_bitmap_layer) / sizeof(*digit_bitmap_layer));

  // Store current timeinfo
  const time_t now = time(NULL);
  const struct tm *ti = localtime(&now);
  timeinfo = malloc(sizeof(*ti));
  prev_timeinfo = malloc(sizeof(*ti));
  *timeinfo = *ti;
  *prev_timeinfo = *ti;

  // Load bitmaps
  const uint8_t month = timeinfo->tm_mon;
  const uint8_t day = timeinfo->tm_mday;
  uint8_t hour = timeinfo->tm_hour;
  // 12H time?
  if((settings & SETTING_12H_TIME) > 0) {
    if(hour == 0) { hour = 12; }
    else if(hour > 12) { hour -= 12; }
  }
  const uint8_t min = timeinfo->tm_min;
  const uint8_t min_tens = min / 10;
  const uint8_t min_ones = min % 10;
  const uint8_t hour_tens = hour / 10;
  const uint8_t hour_ones = hour % 10;
  const uint8_t day_tens = day / 10;
  const uint8_t day_ones = day % 10;
  digit_bitmap[MINUTE_ONES_TOP] = gbitmap_create_with_resource(DIGITS[min_ones * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[MINUTE_ONES_BOT] = gbitmap_create_with_resource(DIGITS[min_ones * (ANIMATION_FRAMES + 1) + 4]);
  digit_bitmap[MINUTE_TENS_TOP] = gbitmap_create_with_resource(DIGITS[min_tens * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[MINUTE_TENS_BOT] = gbitmap_create_with_resource(DIGITS[min_tens * (ANIMATION_FRAMES + 1) + 4]);
  digit_bitmap[HOUR_ONES_TOP] = gbitmap_create_with_resource(DIGITS[hour_ones * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[HOUR_ONES_BOT] = gbitmap_create_with_resource(DIGITS[hour_ones * (ANIMATION_FRAMES + 1) + 4]);
  digit_bitmap[HOUR_TENS_TOP] = gbitmap_create_with_resource(DIGITS[hour_tens * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[HOUR_TENS_BOT] = gbitmap_create_with_resource(DIGITS[hour_tens * (ANIMATION_FRAMES + 1) + 4]);
  digit_bitmap[DAY_ONES_TOP] = gbitmap_create_with_resource(DIGITS[day_ones * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[DAY_ONES_BOT] = gbitmap_create_with_resource(DIGITS[day_ones * (ANIMATION_FRAMES + 1) + 4]);
  digit_bitmap[DAY_TENS_TOP] = gbitmap_create_with_resource(DIGITS[day_tens * (ANIMATION_FRAMES + 1)]);
  digit_bitmap[DAY_TENS_BOT] = gbitmap_create_with_resource(DIGITS[day_tens * (ANIMATION_FRAMES + 1) + 4]);
  //month_bitmap[MONTH_TOP] = gbitmap_create_with_resource(MONTHS[month * (ANIMATION_FRAMES +1)]);
  //month_bitmap[MONTH_BOT] = gbitmap_create_with_resource(MONTHS[month * (ANIMATION_FRAMES +1) + 4]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_ONES_TOP], digit_bitmap[MINUTE_ONES_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_ONES_BOT], digit_bitmap[MINUTE_ONES_BOT]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_TENS_TOP], digit_bitmap[MINUTE_TENS_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[MINUTE_TENS_BOT], digit_bitmap[MINUTE_TENS_BOT]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[HOUR_ONES_TOP], digit_bitmap[HOUR_ONES_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[HOUR_ONES_BOT], digit_bitmap[HOUR_ONES_BOT]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[HOUR_TENS_TOP], digit_bitmap[HOUR_TENS_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[HOUR_TENS_BOT], digit_bitmap[HOUR_TENS_BOT]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[DAY_ONES_TOP], digit_bitmap[DAY_ONES_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[DAY_ONES_BOT], digit_bitmap[DAY_ONES_BOT]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[DAY_TENS_TOP], digit_bitmap[DAY_TENS_TOP]);
  bitmap_layer_set_bitmap(digit_bitmap_layer[DAY_TENS_BOT], digit_bitmap[DAY_TENS_BOT]);

  // Inverter layer
  inverter_layer = inverter_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
  layer_set_hidden(inverter_layer_get_layer(inverter_layer), (settings & SETTING_INVERTED) == 0);

  // Set animation implementation
  animation_implementation = (AnimationImplementation) {
    .setup = &setup_animation, .teardown = &teardown_animation, .update = &update_animation
  };

  // Create animations
  uint8_t i = 0;
  for(; i < 4; i++) {
    animation[i] = animation_create();
    animation_set_duration(animation[i], 1000);
    animation_set_delay(animation[i], 2000);
    animation_set_implementation(animation[i], &animation_implementation);
  }
}

/**
 * Called when the window is de-initialized.
 *
 * Perform clean-up.
 *
 * @param window Pointer to Window object
 */
static void window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window unload");
  uint8_t i = 0, j = 0, n = sizeof(animation) / sizeof(*animation);

  animation_unschedule_all();

  for(; i < n; i++) {
    animation_destroy(animation[i]);
  }

  n = sizeof(digit_bitmap_layer) / sizeof(*digit_bitmap_layer);
  for(i = 0; i < n; i++) {
    bitmap_layer_destroy(digit_bitmap_layer[i]);
  }

  n = sizeof(month_bitmap_layer) / sizeof(*month_bitmap_layer);
  for(i = 0; i < n; i++) {
    bitmap_layer_destroy(month_bitmap_layer[i]);
  }

  n = sizeof(digit_bitmap) / sizeof(*digit_bitmap);
  for(i = 0; i < n; i++) {
    gbitmap_destroy(digit_bitmap[i]);
  }

  n = sizeof(month_bitmap) / sizeof(*month_bitmap);
  for(i = 0; i < n; i++) {
    gbitmap_destroy(month_bitmap[i]);
  }

  n = sizeof(animation_bitmap) / sizeof(*animation_bitmap);
  for(i = 0; i < n; i++) {
    for(j = 0; j < ANIMATION_FRAMES; j++) {
      gbitmap_destroy(animation_bitmap[i][j]);
    }
  }

  bitmap_layer_destroy(background_bitmap_layer);
  gbitmap_destroy(background_bitmap);

  inverter_layer_destroy(inverter_layer);
}

/**
 * Initialie the app
 *
 */
static void init(void) {
  settings = 0;

  // Load settings and init sync with JS app on phone
  Tuplet initial_settings[] = {
    TupletInteger(SETTING_SYNC_KEY_INVERTED, 0),
    TupletInteger(SETTING_SYNC_KEY_12H_TIME, 0),
    TupletInteger(SETTING_SYNC_KEY_SHOW_DATE, SETTING_DATE_ABOVE_TIME),
  };
  APP_LOG(APP_LOG_LEVEL_DEBUG, "size needed = %d", (int)dict_calc_buffer_size_from_tuplets(initial_settings, 3));

  // @FIXME this is causing a mem leak of 192B?
  app_sync_init(&settings_sync, settings_sync_buffer, sizeof(settings_sync_buffer), initial_settings, ARRAY_LENGTH(initial_settings),
    settings_sync_tuple_changed_callback, settings_sync_error_callback, NULL
  );

  // @FIXME this is causing a mem leak of 184B?
  app_message_open(1024, 1024);

  // Initialize the window
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // Subscribe to tick timer service to update watchface every minute 
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

/**
 * De-initialize the app
 *
 */
static void deinit(void) {
  app_sync_deinit(&settings_sync);
  tick_timer_service_unsubscribe();
  window_destroy(window);
  free(timeinfo);
  free(prev_timeinfo);
}

/**
 * App entry point.
 *
 */
int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
}
