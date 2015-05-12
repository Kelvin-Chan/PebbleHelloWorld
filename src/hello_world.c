#include <pebble.h>

Window *window, *window2, *window3;
TextLayer *text_layer;
static GBitmap *globe_bitmap, *logo_bitmap;
static BitmapLayer *globe_bitmap_layer, *logo_bitmap_layer;

// Prototyping User-Defined Function
void pushText(void);
void pushImage(void);
void switchUp(void);
void switchHome(void);
void switchDown(void);
void cleanUp(GBitmap *bitmap, BitmapLayer *bitmaplayer);

// ----------------  Click Handler Functions and Provider  ----------------------
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  switchUp();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  switchHome();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  switchDown();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
// -------------------------------------------------------------------------

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
  
  // Set Click Config Provider to Window
  window_set_click_config_provider(window, click_config_provider);
  
  // Push the window
  window_stack_push(window, true);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed Window!");
}

void pushImage(void){
  
  // Create Bitmap Window
  window2 = window_create();
	
  // Assign Gbitmap pointer
  globe_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GLOBE);
  
  // Assign BitmapLayer pointer
  globe_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
  bitmap_layer_set_bitmap(globe_bitmap_layer, globe_bitmap);
  
  // Add BitmapLayer to window2
  layer_add_child(window_get_root_layer(window2), bitmap_layer_get_layer(globe_bitmap_layer));
  
  // Set Click Config Provider to Window
  window_set_click_config_provider(window2, click_config_provider); 
  
  // Push window2
  window_stack_push(window2, true);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed Window2!");
}

void pushLogo(void){
  
  // Create Bitmap Window
  window3 = window_create();
	
  // Assign Gbitmap pointer
  logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHERRY_PICKS);
  
  // Assign BitmapLayer pointer
  logo_bitmap_layer = bitmap_layer_create(GRect(0, 58, 144, 86));
  bitmap_layer_set_bitmap(logo_bitmap_layer, logo_bitmap);
  
  // Add BitmapLayer to window2
  layer_add_child(window_get_root_layer(window3), bitmap_layer_get_layer(logo_bitmap_layer));
  
  // Set Click Config Provider to Window
  window_set_click_config_provider(window3, click_config_provider); 
  
  // Push window2
  window_stack_push(window3, true);
  
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed Window2!");
}

void switchUp(void){
  if(!window_stack_contains_window(window2)){
    pushImage();
  }else if(!window_stack_contains_window(window3)){
    pushLogo();
  }else{
    window_stack_pop(true);
    cleanUp(logo_bitmap, logo_bitmap_layer);
  }
}

void switchHome(void){
  while(window_stack_get_top_window() != window){
    Window *windowTemp = window_stack_pop(true);
    if(windowTemp == window3){
      cleanUp(logo_bitmap, logo_bitmap_layer);
    }else{
      cleanUp(globe_bitmap, globe_bitmap_layer);
    }
  }
}

void switchDown(void){
  if(window_stack_get_top_window() != window){
    Window *windowTemp = window_stack_pop(true);
    if(windowTemp == window3){
      cleanUp(logo_bitmap, logo_bitmap_layer);
    }else{
      cleanUp(globe_bitmap, globe_bitmap_layer);
    }
  }
}

void cleanUp(GBitmap *bitmap, BitmapLayer *bitmap_layer){
    // Destroy Bitmap and Bitmap layer
    gbitmap_destroy(bitmap);
    bitmap_layer_destroy(bitmap_layer);
}
  
void handle_init(void){
	pushText();
}

void handle_deinit(void){
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void){
	handle_init();
	app_event_loop();
	handle_deinit();
}