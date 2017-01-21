#include <pebble.h>
#include <stdbool.h>
#include <string.h>
#include "messaging.h"
#include "main.h"


Window* window;
Window* window2;
Window* window3;
TextLayer *text_layer;
//might not need this yet
TextLayer *text_layer3;
char* passedString=NULL;
MenuLayer* menu_layer;
MenuLayer* menu_layer2;
char** listOfNames=NULL;
int numOfNames=0;
uint32_t keyTemp=0;
struct Project* tempProj;
int whichProject=0;
int numRows=0;


/** STOPWATCH STUFF 
char* message=NULL;
char* totalMessage=NULL;
 //struct tm *currentTime;
//struct tm *diffTime;
char buffer[] = "00:00:00";

AppTimer *timer;
bool isRunning = false;
char* currentTime = NULL;
 END STOPWATCH STUFF **/



void seeTimeBlocks(){
  window_stack_push(window2, true);
  
}


void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
  //  for (int i = 0; i < numOfNames; i++) {
       // if (cell_index->row == i) {
            char buffer[32];
            persist_read_string((uint32_t) (2*cell_index->row), buffer, sizeof(buffer));
            menu_cell_basic_draw(ctx, cell_layer, buffer, "", NULL); // change to pers storage
        //    break;  
      //  }
  //  } 
}

void draw_row_callback2(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
      int times[20];
      persist_read_data((uint32_t)(2*whichProject+1), &times, sizeof(int[20]));
        char buffer[] = "00:00:00";
        struct tm *dTime = malloc(sizeof(struct tm));
        int seconds;
        if (cell_index->row == 0 ) {   //note: if row==0; make it the add time option
            menu_cell_basic_draw(ctx, cell_layer, "add new block" , "", NULL);
        }
        else if (cell_index->row == numRows-1){
            menu_cell_basic_draw(ctx, cell_layer, "finished assignment" , "", NULL);
        }
        
        else {
            seconds=times[cell_index->row];
            dTime->tm_hour=(int)(seconds/3600);
            seconds=seconds-(dTime->tm_hour*3600);
            dTime->tm_min = (int)(seconds/60);
            seconds=seconds-(dTime->tm_min*60);
            dTime->tm_sec = seconds;
            strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", dTime);
            menu_cell_basic_draw(ctx, cell_layer, buffer , "", NULL); // change to pers storage
           
        }
    
}
  
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return (uint16_t)persist_read_int((uint32_t)(-1)); 
}

uint16_t num_rows_callback2(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  int times[20];
  persist_read_data((uint32_t)(2*whichProject+1), &times, sizeof(int[20]));
  int counter = 0;
  while (times[counter]!=0){
    counter++;
  }
  numRows=counter+1;
  return counter+1; 
}

 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
//Get which row
    int which = cell_index->row;
 
   
    whichProject=which;
    //PUSHING WINDOW ONTO THE STACK
    seeTimeBlocks();
    
}


void select_click_callback2(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context, int index)
{

  if (cell_index->row == 0){
    
    
  }
  else if (cell_index->row == numRows-1){
    
    
  }
    
}



/**

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

**/

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



void window_load2(Window *window2){
  //menulayer stuff
    //Create it - 12 is approx height of the top bar
    menu_layer2 = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    //Let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer2, window2);
 
    //Give it its callbacks
    MenuLayerCallbacks callbacks2 = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback2,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback2,
        .select_click = (MenuLayerSelectCallback) select_click_callback2
    };
    menu_layer_set_callbacks(menu_layer2, NULL, callbacks2);
 
    //Add to Window
    layer_add_child(window_get_root_layer(window2), menu_layer_get_layer(menu_layer2));
  
}



void window_load3(Window *window2){
  //menulayer stuff
  // TIMER STUFF
  
}


/* Un-load all Window sub-elements */
void window_unload(Window *window)
{  menu_layer_destroy(menu_layer); 
    text_layer_destroy(text_layer);
}

void window_unload2(Window *window2){
   menu_layer_destroy(menu_layer2); 
  
}


void window_unload3(Window *window3){
   //menu_layer_destroy(menu_layer2); 
  
}

/* Initialize the main app elements */
void init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };  
  
    window2=window_create();
    WindowHandlers handlers2 = {
        .load = window_load2,
        .unload = window_unload2
    };
  
    window3 = window_create();
    WindowHandlers handlers3 = {
        .load = window_load3,
        .unload = window_unload3
    };  
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_set_window_handlers(window2, (WindowHandlers) handlers2);
     window_set_window_handlers(window3, (WindowHandlers) handlers3);
    //IN ORDER TO USE CLICK, NEED TO USE CLICK CONFIG PROVIDER
    //window_set_click_config_provider(window, click_config_provider);
    
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

