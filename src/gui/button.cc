#include "input/mouse_event.h"

#include "label.h"
#include "button_ui.h"
#include "button.h"

using namespace gui;

button::button (): pressed_ (false)
{
  set_sensible (true);
  set_border_width (0);
}


bool button::get_pressed() const
{
  return pressed_;
}


void button::set_button_ui (button_template * but)
{
  if (my_object_ui) delete my_object_ui;
  
  my_object_ui = new button_ui (this);
  ((button_ui*)my_object_ui)->set_button (but);
}


int button::input_update (input::event * ev)
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


 
button::~button ()
{ 
   
} 

void button::set_text (const std::string & text, font * font)
{
  /* destroy child */
  clear();
  
  /*  label * lab = new label;
  lab->set_font (*font);
  lab->set_text (text);
  lab->set_visible (true);
  //  lab->set_alignment ( misc::CENTER, misc::CENTER );
  add ( lab );
  lab->realize();*/
}


