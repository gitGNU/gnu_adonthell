#include "object_template.h"

using namespace gui;


std::string object_template::get_name () const
{
    return my_name; 
}


void object_template::set_name (const std::string & name)
{
    my_name =  name; 
}



