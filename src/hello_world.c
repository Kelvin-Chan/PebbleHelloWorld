#include <pebble.h>

Window *window, *window2;
TextLayer *text_layer;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;


void pushText(void){
  // Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 154));
  
  // Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Hi, I'm a Pebble! Press Any Button For Globe!");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  
  // Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  
  // Push the window
  window_stack_push(window, true);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed Window!");
}

void pushImage(void){
  
  // Create Bitmap Window
  window2 = window_create();
	
  // Assign Gbitmap pointer
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GLOBE);
  
  // Assign BitmapLayer pointer
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  
  // Add BitmapLayer to window2
  layer_add_child(window_get_root_layer(window2), bitmap_layer_get_layer(s_bitmap_layer));
  
  // Push window2
  window_stack_push(window2, true);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed Window2!");
}

void switchScreen(void){
  if(!window_stack_contains_window(window2)){
    pushImage();
  }else{
  window_stack_pop(true);
  }
}

void handle_init(void) {
	pushText();
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
  
  // Destroy Bitmap layer
  gbitmap_destroy(s_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
	
	// Destroy the window
	window_destroy(window);
  window_destroy(window2);
}

// ----------------  Click Handler Functions and Provider  ----------------------
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "UP");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "SELECT");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "DOWN");
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
// -------------------------------------------------------------------------

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}