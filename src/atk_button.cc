#include "input/mouse_event.h"

#include "atk_label.h"
#include "atk_button_ui.h"
#include "atk_button.h"


atk_button::atk_button (): pressed_ (false)
{
  set_sensible (true);
  set_border_width (0);
}


bool atk_button::get_pressed() const
{
  return pressed_;
}


void atk_button::set_button_ui (atk_button_template * but)
{
  if (object_ui_) delete object_ui_;
  
  object_ui_ = new atk_button_ui (this);
  ((atk_button_ui*)object_ui_)->set_button (but);
}


int atk_button::input_update (input::event * ev)
{
  if (!is_sensible () ) return 0; 
  
  input::mouse_event * me = (input::mouse_event *) ev; 
  
/* test if mouse position is inside this button */
  if (!point_belong (me->x (), me->y ())) 
    {
      if (pressed_) pressed_ = false;
      return 0;
    }
  
/* the mouse is inside the button */
  if (me->type () == input::mouse_event::BUTTON_PUSHED)
    {
      on_pressed();
      pressed_ = true;
    } 
  else if (me->type () == input::mouse_event::BUTTON_RELEASED)
    {
      on_released();
      if( pressed_ ) on_clicked();
      pressed_ = false;
    }
  return 1;
} 


 
atk_button::~atk_button ()
{ 
   
} 

void atk_button::set_text (const std::string & text, atk_font * font)
{
  /* destroy child */
  clear();
  
  atk_label * lab = new atk_label;
  lab->set_font (*font);
  lab->set_text (text);
  lab->set_visible (true);
  //  lab->set_alignment ( atk_misc::CENTER, atk_misc::CENTER );
  add ( lab );
  lab->realize();
}


