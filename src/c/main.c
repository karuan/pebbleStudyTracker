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


/** STOPWATCH STUFF **/
char* message=NULL;
char* totalMessage=NULL;
bool firstClick=false;
bool isPaused=false;
struct tm *diffTime;
char buffer2[] = "00:00:00";

AppTimer *timer;
bool isRunning = false;
char* currentTime = NULL;
/** END STOPWATCH STUFF **/


/* ~~~  START OF STOPWATCH FUNCTION ~~~~*/


void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	

 //make a time structure with the difference in time
  if (diffTime->tm_sec==59){
    diffTime->tm_sec=0;
    diffTime->tm_min=diffTime->tm_min + 1;
  }
  else{
    diffTime->tm_sec = diffTime->tm_sec + 1;
  }
  if (diffTime->tm_min == 60){
    diffTime->tm_min = 0;
    diffTime->tm_hour=diffTime->tm_hour + 1;
  }
 
 
  	strftime(buffer2, sizeof("00:00:00"), "%H:%M:%S", diffTime);
  
   
        //Change the TextLayer text to show the new time!
        text_layer_set_text(text_layer3, buffer2);
	
}





void up_click_handler3(ClickRecognizerRef recognizer, void *context){
	//text_layer_set_text(text_layer, "You pressed UP!");
  if (isPaused){
    int seconds = diffTime->tm_sec + diffTime->tm_min*60 + diffTime->tm_hour*3600;
    int times[20];
    persist_read_data((uint32_t)(3*whichProject+1),&times, sizeof(int[20]));
    int counter=0;
    while (times[counter]!=0){
      counter++;
    }
    times[counter]=seconds;
    persist_write_data((uint32_t)(3*whichProject + 1), &times, sizeof(int[20]));
    
    struct tm *currentTime;
	  time_t currentTemp;
	  currentTemp = time(NULL);
	  currentTime = localtime(&currentTemp);
    struct tm** dates;
    persist_read_data((uint32_t)(3*whichProject + 2), &dates, sizeof(struct tm*[20]));
    dates[counter]= currentTime;
    persist_write_data((uint32_t)(3*whichProject + 2), &dates, sizeof(struct tm*[20]));
    
    
    
    text_layer_set_text(text_layer3, "committed your time");
    firstClick=true;
    currentTime=NULL;
    isPaused=false;
    isRunning=false;
  }
}

void down_click_handler3(ClickRecognizerRef recognizer, void *context){
if (isPaused){

    text_layer_set_text(text_layer3, "deleted your time");
    firstClick=true;
    currentTime=NULL;
    isPaused=false;
    isRunning=false;
  }

}

void select_click_handler3(ClickRecognizerRef recognizer, void *context){
  
  struct tm *t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);
  
  if (isRunning == false && isPaused == true){
    
    
    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);	

   tick_handler (t, SECOND_UNIT);
    isPaused=false;
    isRunning=true;
    
    
    
  } 
  
  else if (isRunning == false && isPaused == false){
     if (firstClick == true){
    text_layer_set_text(text_layer3,  "press select to start timer");
      firstClick=false;
      }
  else if (firstClick==false){
  firstClick=true;


/**
  //make a time structure with the current time
  currentTime = malloc(sizeof(struct tm));
  currentTime->tm_sec = t->tm_sec;
  currentTime->tm_min = t->tm_min;
  currentTime->tm_hour = t->tm_hour;**/
  
  diffTime = malloc(sizeof(struct tm));
  
diffTime->tm_sec = -1;
  diffTime->tm_min =0;
  diffTime->tm_hour = 0;
	//manually call the tick handler when the window is loading
	  //don't actually understand why passing in SECOND_UNIT bc not used
    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);	

   tick_handler (t, SECOND_UNIT);
    
    isRunning = true;
  }
  }
  else if (isRunning == true){
    isRunning=false;
    isPaused = true;
    currentTime=buffer2;
    tick_timer_service_unsubscribe(); 
  }
    
    
}

/** ~~~~ END OF STOPWATCH FUNCTIONS ~~~~ */

void click_config_provider3(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler3);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler3);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler3);
}



void seeTimeBlocks(){
  window_stack_push(window2, true);
  
}


void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
 
            char buffer[32];
            persist_read_string((uint32_t) (3*cell_index->row), buffer, sizeof(buffer));
            int times[20];
            persist_read_data((uint32_t)(3*cell_index->row + 1), &times, sizeof(int[20]));
              if (times[0]==-1){
                 menu_cell_basic_draw(ctx, cell_layer, buffer, "[in progress]", NULL); // change to pers storage
              }
              else{
                menu_cell_basic_draw(ctx, cell_layer, buffer, "[completed]", NULL); 
   
  } 
}

void draw_row_callback2(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
      int times[20];
      persist_read_data((uint32_t)(3*whichProject+1), &times, sizeof(int[20]));
        char buffer[] = "00:00:00";
        struct tm *dTime = malloc(sizeof(struct tm));
        int seconds;
        if (cell_index->row == 0 ) {   //note: if row==0; make it the add time option
            int times[20];
          persist_read_data((uint32_t)(3*whichProject+1),&times, sizeof(int[20]));
          if (times[0]==1){
            menu_cell_basic_draw(ctx, cell_layer, "add new block" , "[disabled]", NULL);
          }
         else {
            menu_cell_basic_draw(ctx, cell_layer, "add new block" , "", NULL);
          }
        }
        else if (cell_index->row == numRows-1){
          int times[20];
          persist_read_data((uint32_t)(3*whichProject+1),&times, sizeof(int[20]));
          if (times[0]==1){
            menu_cell_basic_draw(ctx, cell_layer, "FINISHED" , "", NULL);
          }
          else{
            menu_cell_basic_draw(ctx, cell_layer, "finish" , "", NULL);
          }
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
           
        
           struct tm** dates;
            persist_read_data((uint32_t)(3*whichProject + 2), &dates, sizeof(struct tm*[20]));
            char dateBuffer[26];
            strftime(dateBuffer, 26, "%Y-%m-%d", dates[cell_index->row]);
            menu_cell_basic_draw(ctx, cell_layer, buffer , dateBuffer, NULL); 
        
        }
    
}
  
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return (uint16_t)persist_read_int((uint32_t)(-1)); 
}

uint16_t num_rows_callback2(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  int times[20];
  persist_read_data((uint32_t)(3*whichProject+1), &times, sizeof(int[20]));
  int counter = 0;
  while (times[counter]!=0){
    counter++;
  }
  numRows=counter+1;
 // if (times[0]==-1){
  
    return counter+1; 
 /**
  }
  else{
    return counter;
  }**/
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
int times[20];
    persist_read_data((uint32_t)(3*whichProject+1),&times, sizeof(int[20]));
  if (cell_index->row == 0 && times[0]==-1){
    firstClick=false;
    isPaused=false;
    isRunning = false;
     window_stack_push(window3,true);
  }
  else if (cell_index->row == numRows-1){
    int times[20];
    persist_read_data((uint32_t)(3*whichProject+1),&times, sizeof(int[20]));
     times[0]=1;
    persist_write_data((uint32_t)(3*whichProject + 1), &times, sizeof(int[20]));
     ///////////////////
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
  
   //We will add the creation of the Window's elements here soon!
	text_layer3 = text_layer_create(GRect(0, 53, 132, 168));
   text_layer_set_background_color(text_layer3, GColorClear);
   text_layer_set_text_color(text_layer3, GColorBlack);

	text_layer_set_text_alignment(text_layer3, GTextAlignmentCenter);
  text_layer_set_text(text_layer3, "press select to start timer");
  text_layer_set_font(text_layer3, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  layer_add_child(window_get_root_layer(window3), text_layer_get_layer(text_layer3));


//get a time structure so that the face doesn’t start blank
  
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
   //We will safely destroy the Window's elements here!
	text_layer_destroy(text_layer3);

	//cancel timer
	//app_timer_cancel(timer);
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
  
 
   window_set_click_config_provider(window3, click_config_provider3);
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
  window_destroy(window2);
  window_destroy(window3);
}
 
/* Main app lifecycle */
int main(void)
{
    messaging_init(NULL);
    init();
    app_event_loop();
    deinit();
}

