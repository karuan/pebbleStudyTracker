#include <pebble.h>
#include "messaging.h"


Window* window;
TextLayer *text_layer;
char* passedString=NULL;
char** listOfNames=NULL;
int numOfNames=0;
uint32_t keyTemp=0;

void up_click_handler(ClickRecognizerRef recognizer, void *context){
	text_layer_set_text(text_layer, "You pressed UP!");
}

void down_click_handler(ClickRecognizerRef recognizer, void *context){
	text_layer_set_text(text_layer, "You pressed DOWN!");

}

void select_click_handler(ClickRecognizerRef recognizer, void *context){
	char* totalList="";
  for (int i=0;i<numOfNames; i++){
    keyTemp=(uint32_t)i;
    char buffer[32];
    persist_read_string(keyTemp, buffer, sizeof(buffer));
    strcat(totalList, buffer);
  }
  
  
  
    text_layer_set_text(text_layer, totalList);
  
}


void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

 





/* Load all Window sub-elements */
void window_load(Window *window)
{
    text_layer = text_layer_create(GRect(0, 0, 144, 168));
    text_layer_set_background_color(text_layer, GColorClear);
    text_layer_set_text_color(text_layer, GColorBlack);
 
    layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
    text_layer_set_text(text_layer, "My first watchapp!");
}

/* Un-load all Window sub-elements */
void window_unload(Window *window)
{
    text_layer_destroy(text_layer);
}

/* Initialize the main app elements */
void init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };
  
  window_set_window_handlers(window, (WindowHandlers) handlers);
     window_set_click_config_provider(window, click_config_provider);
  
    window_stack_push(window, true);
}
 
/* De-initialize the main app elements */
void deinit()
{
    window_destroy(window);
}
 
/* Main app lifecycle */
int main(void)
{
    messaging_init(NULL);
    init();
    app_event_loop();
    deinit();
}

