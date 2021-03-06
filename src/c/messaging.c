#include <pebble.h>
#include <string.h>
#include <pebble.h>
#include <stdbool.h>
#include "messaging.h"
#include "main.h"



static char s_items_to_add_buffer[512];

uint32_t key=0;
void (*message_processed_callback)(void);

void messaging_init(void (*processed_callback)(void)) {
  // register my custom callback
 // message_processed_callback = processed_callback;

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Watch messaging is started!");
  app_message_register_inbox_received(inbox_received_callback);
}



void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // does this message contain weather information?
  Tuple *items_to_add_tuple = dict_find(iterator, projectName);

  if(items_to_add_tuple != NULL) {
    strncpy(s_items_to_add_buffer, items_to_add_tuple->value->cstring, sizeof(s_items_to_add_buffer) - 1);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, items_to_add_tuple->value->cstring);
    //text_layer_set_text(text_layer, items_to_add_tuple->value->cstring);
    
    
    /**
     char spliting function goes here
    
    **/ 
    passedString = items_to_add_tuple->value->cstring; 
    char* best="";
    int counter=0;
    while (strlen(&passedString[counter])!=0){
      if (passedString[counter] == ','){
        
          strcpy(best,passedString);
        
          best[counter]='\0' ;
        
         key =(uint32_t)(3* numOfNames);
    
        persist_write_string(key, best);
        key = (uint32_t)(3* numOfNames + 1);
        int times[20];
        for (int i=0;i<20;i++){
        times[i]=0;
        }
    times[0]=-1;
    persist_write_data(key, &times, sizeof(int[20]));
    
    /**
     key = (uint32_t)(3* numOfNames + 2);
    struct tm** dates = calloc(20, sizeof(struct tm*[20]));
    persist_write_data(key, &dates, sizeof(struct tm*[20]));
    **/
  
    numOfNames=numOfNames+1;
    persist_write_int((uint32_t)(-1), numOfNames);
        
        passedString=&passedString[counter+1];
        counter = 0;
        
      }
      counter++;
      
    }
    
    
    
      strcpy(best,passedString);
        
          best[counter]='\0' ;
        
         key =(uint32_t)(3* numOfNames);
    
        persist_write_string(key, best);
        key = (uint32_t)(3* numOfNames + 1);
        int times[20];
        for (int i=0;i<20;i++){
        times[i]=0;
        }
    times[0]=-1;
    persist_write_data(key, &times, sizeof(int[20]));
    
    
     key = (uint32_t)(3* numOfNames + 2);
    struct tm** dates = calloc(20, sizeof(struct tm*[20]));
    persist_write_data(key, &dates, sizeof(struct tm*[20]));
    
  
    numOfNames=numOfNames+1;
    persist_write_int((uint32_t)(-1), numOfNames);
    
    
    
/**
    // Read the string
char buffer[32];
persist_read_string(key, buffer, sizeof(buffer));
    
    
    passedString = items_to_add_tuple->value->cstring;
    **/
    
  }

  // notify the main screen, in case something changed
 // message_processed_callback();
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! %d %d %d", reason, APP_MSG_SEND_TIMEOUT, APP_MSG_SEND_REJECTED);

}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
