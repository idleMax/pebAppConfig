#include <pebble.h>
  
static Window *window;
static TextLayer *tLayer;
  
#define KEY_BGMIN 0
#define KEY_FGMIN 1
#define KEY_BGHOUR 2
#define KEY_FGHOUR 3
#define KEY_CRIM 4
  
int bgMin;
int fgMin;
int bgHour;
int fgHour;
int fgRim;


static void window_load(Window *window) {
  tLayer = text_layer_create(GRect(0, 0, 144, 168));
 	text_layer_set_font(tLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tLayer));
}


static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  Tuple *t = dict_read_first(iterator);
  if(t)
  {
    switch(t->key)
    {
    case KEY_BGMIN:      
      bgMin = (int)t->value->int16;             
      break;
    case KEY_FGMIN:
      fgMin = (int)t->value->int16;      
      break;
    case KEY_BGHOUR:
      bgHour = (int)t->value->int16;      
      break;      
    case KEY_FGHOUR:
      fgHour = (int)t->value->int16;      
      break;  
    case KEY_CRIM:
      fgRim = (int)t->value->int16;      
      break;  
    }
  }
  
  //Update text
  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%d, %d, %d, %d, %d", bgMin, fgMin, bgHour, fgHour, fgRim);
  text_layer_set_text_color(tLayer, GColorBlack);
  text_layer_set_text(tLayer, buffer);  
}

  
static void window_unload(Window *window) {
  text_layer_destroy(tLayer);
}


static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
    
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum()); 
}


static void deinit(void) {
  window_destroy(window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}

  