#include <iostream.h>
#include <list>
#include "types.h"
#include "win_types.h"
#include "image.h"
#include "win_font.h"
#include "win_base.h"
#include "win_write.h"
#include "win_label.h"
#include "win_container.h"
#include "win_select.h"


win_select::win_select()
{
  l_list.clear();
}

win_select::~win_select()
{
  l_list.clear();
}

void win_select::add(void * tmp)
{
  l_list.push_back(tmp);
}

void win_select::remove(void * tmp)
{
  list<void *>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=*i)
    i++;
  if(i!=l_list.end())
    l_list.erase(i);
}


void * win_select::get_select()
{
  
}


void win_select::set_select(void *)
{
  
}


