/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_manager.h"

list<win_base*> win_manager::lmanage;
win_base * win_manager::wc=NULL;

hash_map<string, win_theme *> win_manager::theme;
hash_map<string, win_font *> win_manager::font; 

using namespace std; 


void win_manager::init () 
{
}

void win_manager::cleanup () 
{
    // Close all windows
    destroy ();
    
    // Cleaning up themes
    for (hash_map <string, win_theme *>::iterator it = theme.begin ();
         it != theme.end (); it++)
        delete it->second;
    theme.clear (); 

    // Cleaning up fonts
    for (hash_map <string, win_font *>::iterator ifo = font.begin ();
         ifo != font.end (); ifo++)
        delete ifo->second;
    font.clear (); 
}

//add a container 
void win_manager::add(win_base * tmp)
{
    lmanage.push_back(tmp);
}

void win_manager::add_after (win_base * toadd, win_base * after)
{
    for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
        if (*i == after) 
        {
            i++; 
            lmanage.insert (i, toadd);
            break; 
        }
    }
}

bool win_manager::exist(win_base * tmp)
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
      if(*i==tmp) return true;
    }
  return false;
}

//Remove base 
void win_manager::remove(win_base * tmp)
{
  //if tmp has focus on, we set focus off and put ilm at nothing
  if(tmp->is_focus()) 
    {
      tmp->set_focus(false);
      wc=NULL;
    }
  lmanage.remove(tmp);
  if (!wc) set_focus (lmanage.back ());
}


void win_manager::destroy()
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    delete *i;
  lmanage.clear();
  wc = NULL;
}

void win_manager::draw()
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
      (*i)->draw();
}


void win_manager::input_update()
{  
  if(wc) {wc->input_update();} 
}

void win_manager::update()
{
    list<win_base *>::iterator i=lmanage.begin();
    while (i!=lmanage.end())
    {
        if(!(*i)->update())
        {
            if(*i==wc) wc=NULL;
            
            if((*i)->is_focus()) 
            {
                (*i)->set_focus(false);
                wc=NULL;
            }
            win_base * s = *i; 
            i = lmanage.erase(i);
            delete s; 
            if (!wc) set_focus (lmanage.back ());
        }
        else i++; 
    }
    if (!wc) set_focus (lmanage.back ());
}

void win_manager::set_focus(win_base * tmp)
{
    if(!lmanage.empty())
    {
        if(wc) wc->set_focus(false);
        wc=tmp;
        wc->set_focus(true);
    }   
}

void win_manager::add_theme (string name)
{
    theme[name] = new win_theme ((char *) name.c_str ()); 
}

bool win_manager::remove_theme (string name)
{
    hash_map <string, win_theme *>::iterator it = theme.find (name);
    if (it == theme.end ()) return false;

    delete it->second;
    theme.erase (it);
    return true; 
}

win_theme * win_manager::get_theme (string name)
{
    hash_map <string, win_theme *>::iterator it = theme.find (name); 
    if (it == theme.end ()) return NULL;
    else return it->second; 
}

void win_manager::add_font (string name)
{
    font[name] = new win_font ((char *) name.c_str ());
}

bool win_manager::remove_font (string name)
{
    hash_map <string, win_font *>::iterator it = font.find (name);
    if (it == font.end ()) return false;

    delete it->second;
    font.erase (it);
    return true; 
}

win_font * win_manager::get_font (string name)
{
    hash_map <string, win_font *>::iterator it = font.find (name);
    if (it == font.end ()) return NULL;
    else return it->second; 
}
