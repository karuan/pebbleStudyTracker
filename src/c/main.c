#include <pebble.h>
#include <stdbool.h>
#include <string.h>
#include "messaging.h"
#include "main.h"


Window* window;
TextLayer *text_layer;
char* passedString=NULL;
MenuLayer* menu_layer;
char** listOfNames=NULL;
int numOfNames=0;
uint32_t keyTemp=0;
struct Project* tempProj;



void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    for (int i = 0; i < numOfNames; i++) {
        if (cell_index->row == i) {
            char buffer[32];
            persist_read_string((uint32_t) (2*i), buffer, sizeof(buffer));
            menu_cell_basic_draw(ctx, cell_layer, buffer, "", NULL); // change to pers storage
            break;  
        }
    } 
}
  
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return (uint16_t)persist_read_int((uint32_t)(-1)); 
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
//Get which row
    int which = cell_index->row;
 
    //The array that will hold the on/off vibration times
    uint32_t segments[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 
    //Build the pattern (milliseconds on and off in alternating positions)
    for(int i = 0; i < which + 1; i++)
    {
        segments[2 * i] = 200;
        segments[(2 * i) + 1] = 100;
    }
 
    //Create a VibePattern data structure
    VibePattern pattern = {
        .durations = segments,
        .num_segments = 16
    };
 
    //Do the vibration pattern!
    vibes_enqueue_custom_pattern(pattern); 
}
 







void up_click_handler(ClickRecognizerRef recognizer, void *context){
	text_layer_set_text(text_layer, "You pressed UP!");
}

void down_click_handler(ClickRecognizerRef recognizer, void *context){
	text_layer_set_text(text_layer, "You pressed DOWN!");
}

void select_click_handler(ClickRecognizerRef recognizer, void *context){
	char* totalList="";
  for (int i=0;i<numOfNames; i++){
    keyTemp=(uint32_t)(2*i);
    char buffer[32] ;
    persist_read_string(keyTemp, buffer, sizeof(buffer));
    //buffer=get_name(tempProj);
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


    //menulayer stuff
    //Create it - 12 is approx height of the top bar
    menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    //Let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer, window);
 
    //Give it its callbacks
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_click_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
 
    //Add to Window
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

/* Un-load all Window sub-elements */
void window_unload(Window *window)
{  menu_layer_destroy(menu_layer); 
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
    
    //data initialization
    numOfNames=persist_read_int((uint32_t)(-1));
  


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

