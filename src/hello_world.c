#include <pebble.h>  

// ****** THIS APP WILL SWITCH BETWEEN WINDOWS, PUSHING A NEW WINDOW IN BEFORE POPPING THE PREVIOUS ONE ******
  
static int numOfWindows = 3;  // change number of windows if expanding window array
  
Window *window[3];    // window pointer array
int windowIndex = 0;  // global window array index to keep track of what is on top of stack; default is 0

// Declare layers and layer elements to be used in different windows
TextLayer *text_layer, *index_layer;
static GBitmap *globe_bitmap, *logo_bitmap;
static BitmapLayer *globe_bitmap_layer, *logo_bitmap_layer;


// Prototyping User-Defined Function
void createWindow(int index);
void destroyWindow(int index);
void pushWindow(int index);
void popWindow(int index);
void pressUp(void);
void pressHome(void);
void pressDown(void);
void cleanUpImage(GBitmap *bitmap, BitmapLayer *bitmaplayer);

// ----------------  Click Handler Functions and Provider  ----------------------
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  pressUp();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  pressHome();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  pressDown();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
// ------------------------------------------------------------------------------

// ------------------------- Window Operations ----------------------------------
void createWindow(int index){
  // Create window with corresponding index
  window[index] = window_create();
  
  switch(index){
    case 0:
      // Set the text layer, text, font, and text alignment
      text_layer = text_layer_create(GRect(0, 0, 144, 154));
      text_layer_set_text(text_layer, "Hi, I'm a Pebble! Press Up or Down to Scroll Through Windows!");
	    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
      // Add text layer to window[0]
      layer_add_child(window_get_root_layer(window[0]), text_layer_get_layer(text_layer));
      break;
    
    case 1:
      // Set Gbitmap, BitmapLayer pointer
      globe_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GLOBE);
      globe_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
      bitmap_layer_set_bitmap(globe_bitmap_layer, globe_bitmap);
      // Add BitmapLayer to window[1]
      layer_add_child(window_get_root_layer(window[1]), bitmap_layer_get_layer(globe_bitmap_layer));
      break;
    
    case 2:
      // Set Gbitmap, BitmapLayer pointer
      logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHERRY_PICKS);
      logo_bitmap_layer = bitmap_layer_create(GRect(0, 58, 144, 86));
      bitmap_layer_set_bitmap(logo_bitmap_layer, logo_bitmap);
      // Add BitmapLayer to window[2]
      layer_add_child(window_get_root_layer(window[2]), bitmap_layer_get_layer(logo_bitmap_layer));
      break;
  }
}

void destroyWindow(int index){
  // Clean up data from indexed Window and its layer & elements
  window_destroy(window[index]);
  switch(index){
    case 0:
      text_layer_destroy(text_layer);
      break;
    case 1:
      cleanUpImage(logo_bitmap, logo_bitmap_layer);
      break;
    case 2:
      cleanUpImage(logo_bitmap, logo_bitmap_layer);
      break;
  }
}

void pushWindow(int index){
  // Set Click Config Provider to indexed Window
  window_set_click_config_provider(window[index], click_config_provider); 
  // Push indexed Window
  window_stack_push(window[index], true);
  // App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed window!" );
}

void popWindow(int index){
  // Remove specific indexed Window from stack
  window_stack_remove(window[index], true);
}

void cleanUpImage(GBitmap *bitmap, BitmapLayer *bitmap_layer){
    // Destroy Bitmap and Bitmap layer
    gbitmap_destroy(bitmap);
    bitmap_layer_destroy(bitmap_layer);
}
// -----------------------------------------------------------------------------

// -------------------------- Key Press Operations -----------------------------
void pressUp(void){
  // Go to next Window if not at last Window, otherwise return to first Window
  if(windowIndex != numOfWindows - 1){
    windowIndex++;
    pushWindow(windowIndex);
    popWindow(windowIndex - 1);  // Make sure at least one Window is in stack to keep app running
  }else{
    pushWindow(0);
    popWindow(windowIndex);
    windowIndex = 0;
  }
}

void pressHome(void){
  // Go to first Window, if not already
  if(windowIndex != 0){
    pushWindow(0);
    popWindow(windowIndex);
    windowIndex = 0;
  }
}

void pressDown(void){
  // Go to previous Window if not at first Window, otherwise go to last Window
  if(windowIndex != 0){
    pushWindow(windowIndex - 1);
    popWindow(windowIndex);
    windowIndex--;
  }else{
    pushWindow(numOfWindows - 1);
    popWindow(0);
    windowIndex = numOfWindows - 1;
  }
}
// -----------------------------------------------------------------------

// -------------- Main Application Operations (DO NOT REMOVE) ------------
void handle_init(void){
	// create all windows & elements in background
  for(int c = 0; c < numOfWindows; c++){
    createWindow(c);
  }
  pushWindow(windowIndex);
}

void handle_deinit(void){
	// destrory all windows & elements in background
	for(int c = 0; c < numOfWindows; c++){
    destroyWindow(c);
  }
}

int main(void){
	handle_init();
	app_event_loop();
	handle_deinit();
}
// -----------------------------------------------------------------------