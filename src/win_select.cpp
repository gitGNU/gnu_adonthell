/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_border.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"
#include "win_image.h"
#include "win_container.h"
#include "win_select.h"


Type_win_select::Type_win_select(win_image * p,win_select * ws,u_int8 m)
{
  img=p;
  img->attach_select(ws);
  img->set_select_mode(m);
  img->save_position();
  wselect=ws;
  id=WIN_OBJ_IMAGE;
  
  if(m==WIN_SELECT_MODE_BORDER)
    n_border=img->get_border();
  else n_border=NULL;
}


Type_win_select::Type_win_select(win_label * p,win_select * ws,u_int8 m)
{
  lab=p;
  lab->attach_select(ws);
  lab->set_select_mode(m);
  lab->save_position();
  
  wselect=ws;
  id=WIN_OBJ_LABEL;
  if(m==WIN_SELECT_MODE_BORDER)
    n_border=lab->get_border(); 
  else n_border=NULL;
  
  if(m==WIN_SELECT_MODE_CURSOR)
    if(ws->cursor)
      lab->x_pad_l=ws->cursor->cursor->length;

}

Type_win_select::Type_win_select(win_write * p,win_select * ws,u_int8 m)
{
  wri=p;
  wri->wselect=ws;
  wri->set_select_mode(m);
  wri->save_position();
  
  wselect=ws;
  id=WIN_OBJ_WRITE;
  if(m==WIN_SELECT_MODE_BORDER)
    n_border=wri->get_border();
  else n_border=NULL;
}

Type_win_select::Type_win_select(win_container * p,win_select * ws,u_int8 m)
{
  con=p;
  con->attach_select(ws);
  con->set_select_mode(m);
  con->save_position();

  wselect=ws;
  id=WIN_OBJ_CONTAINER;
  if(m==WIN_SELECT_MODE_BORDER)
    n_border=p->get_border();
  else n_border=NULL;
}

void Type_win_select::select()
{
  switch(id)
    {
    case WIN_OBJ_LABEL:
      if(lab->get_select_mode()==WIN_SELECT_MODE_BORDER) lab->set_border(wselect->cur_border); 
      if(lab->get_select_mode()==WIN_SELECT_MODE_CURSOR) lab->set_cursor(wselect->cursor);
      lab->select();
      break;
    case WIN_OBJ_WRITE:
      if(wri->get_select_mode()==WIN_SELECT_MODE_BORDER) wri->set_border(wselect->cur_border);
      wri->select();
      break;
    case WIN_OBJ_IMAGE:
      if(img->get_select_mode()==WIN_SELECT_MODE_BORDER) img->set_border(wselect->cur_border);
      img->select();
      break;
    case WIN_OBJ_CONTAINER:
      if(con->get_select_mode()==WIN_SELECT_MODE_BORDER) con->set_border(wselect->cur_border); 
      con->select();
      break;
    }
}

void Type_win_select::unselect()
{
  switch(id)
    {
    case WIN_OBJ_LABEL:
      if(lab->get_select_mode()==WIN_SELECT_MODE_BORDER) lab->set_border(NULL);
      if(lab->get_select_mode()==WIN_SELECT_MODE_CURSOR) lab->set_cursor(NULL);
      lab->unselect();
      break;
    case WIN_OBJ_WRITE:
      if(wri->get_select_mode()==WIN_SELECT_MODE_BORDER) wri->set_border(NULL);
      wri->unselect();
      break;
    case WIN_OBJ_IMAGE:
      if(img->get_select_mode()==WIN_SELECT_MODE_BORDER) img->set_border(NULL);
      img->unselect();
      break;
    case WIN_OBJ_CONTAINER:
      if(con->get_select_mode()==WIN_SELECT_MODE_BORDER) con->set_border(NULL);
      con->unselect();
      break;
    }
}

void * Type_win_select::get()
{
  switch(id)
    {
    case WIN_OBJ_LABEL:
      return(lab);
      break;
    case WIN_OBJ_WRITE:
      return(wri);
      break;
    case WIN_OBJ_IMAGE:
      return(img);
      break;
    case WIN_OBJ_CONTAINER:
      return(con);
      break;
    }
  return(NULL);
}

Type_win_select::~Type_win_select()
{
  switch(id)
    {
    case WIN_OBJ_LABEL:
      lab->dettach_select();
      lab->set_border(n_border);
      break;
    case WIN_OBJ_WRITE:
      wri->wselect=NULL;
      wri->set_border(n_border);
      break;
    case WIN_OBJ_IMAGE:
      img->dettach_select();
      img->set_border(n_border);
      break;
    case WIN_OBJ_CONTAINER:
      con->dettach_select();
      con->set_border(n_border);
      break;
    }
}


s_int16 Type_win_select::get_y_move(win_container * wc)
{
  switch(id)
   {
   case WIN_OBJ_LABEL:
     if(lab->real_y == wc->real_y || (lab->real_y>=wc->real_y && lab->real_y+lab->height <= wc->real_y + wc->height))
       return 0;
     else
       if(lab->real_y < wc->real_y) 
	 return (wc->real_y - lab->real_y);
       else
	 if(lab->real_y > wc->real_y+wc->height || lab->real_y+lab->height > wc->real_y+wc->height) 
	   return ((wc->real_y+wc->height) - (lab->real_y+lab->height));
     break;

    case WIN_OBJ_WRITE:
       if(wri->real_y == wc->real_y || (wri->real_y>=wc->real_y && wri->real_y+wri->height <= wc->real_y + wc->height))
       return 0;
     else
       if(wri->real_y < wc->real_y) 
	 return (wc->real_y - wri->real_y);
       else
	 if(wri->real_y > wc->real_y+wc->height || wri->real_y+wri->height > wc->real_y+wc->height) 
	   return ((wc->real_y+wc->height) - (wri->real_y+wri->height));

      break;
   case WIN_OBJ_IMAGE:
     if(img->real_y == wc->real_y || (img->real_y>=wc->real_y && img->real_y+img->height <= wc->real_y + wc->height))
       return 0;
     else
       if(img->real_y < wc->real_y) 
	 return (wc->real_y - img->real_y);
       else
	 if(img->real_y > wc->real_y+wc->height || img->real_y+img->height > wc->real_y+wc->height) 
	   return ((wc->real_y+wc->height) - (img->real_y+img->height));

     break;
   case WIN_OBJ_CONTAINER:
     if(con->real_y == wc->real_y || (con->real_y>=wc->real_y && con->real_y+con->height <= wc->real_y + wc->height))
       return 0;
     else
       if(con->real_y < wc->real_y) 
	 return (wc->real_y - con->real_y);
       else
	 if(con->real_y > wc->real_y+wc->height || con->real_y+con->height > wc->real_y+wc->height) 
	   return ((wc->real_y+wc->height) - (con->real_y+con->height));

      break;
   }
  return 0;
}

void Type_win_select::activate()
{
switch(id)
   {
    case WIN_OBJ_LABEL:
      lab->activate();
      break;
    case WIN_OBJ_WRITE:
      wri->activate();
      break;
    case WIN_OBJ_IMAGE:
      img->activate();
      break;
    case WIN_OBJ_CONTAINER:
      con->activate();
      break;
    }
}

void Type_win_select::y_move(s_int16 tmp)
{
switch(id)
   {
    case WIN_OBJ_LABEL:
      lab->move(lab->x,lab->y+tmp);
      break;
    case WIN_OBJ_WRITE:
      wri->move(wri->x,wri->y+tmp);;
      break;
    case WIN_OBJ_IMAGE:
      img->move(img->x,img->y+tmp);;
      break;
    case WIN_OBJ_CONTAINER:
      con->move(con->x,con->y+tmp);;
      break;
    }
}

/*************************************************************************/
/*************************** WIN_SELECT *******************************/
/*************************************************************************/
void win_select::activate_select()
{
  if(ite_list!=l_list.end()) 
    {
      ite_list->activate();
    }
}

win_select::win_select(win_container * tmp)
{
  wc=tmp;
  l_list.clear();
  cur_border=NULL;
  cursor=NULL;
  ite_list=l_list.begin();
}

win_select::~win_select()
{
  l_list.clear();
}


void win_select::adjust_visible()
{
  if(ite_list!=NULL)
    {
      s_int16 y_move=ite_list->get_y_move(wc);
      if(y_move)
	{
	  list<Type_win_select>::iterator i=l_list.begin();
	  while(i!=l_list.end())
	    {
	      i->y_move(y_move);
	      i++;
	    }  
	}
    }
}



void win_select::add(win_label *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
  ite_list=l_list.begin();
}

void win_select::add(win_write *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
  ite_list=l_list.begin();
}

void win_select::add(win_image *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
  ite_list=l_list.begin();
}

void win_select::add(win_container *p,u_int8 m=1)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
  ite_list=l_list.begin();
}

void win_select::remove(win_label * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) l_list.erase(i);
}

void win_select::remove(win_write * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) l_list.erase(i);
}

void win_select::remove(win_image * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) l_list.erase(i);
}

void win_select::set_default_obj(void * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  ite_list->unselect();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) ite_list=i;
  ite_list->select();
}

void win_select::set_cursor(win_cursor * tmp)
{
  cursor=tmp;
}

void win_select::set_border(win_border * tmp)
{
  cur_border=tmp;
}

void win_select::remove(win_container * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) l_list.erase(i);
}


//get next object
void * win_select::next()
{
  ite_list->unselect();
  ite_list++;
  if(ite_list==l_list.end())
    {
      ite_list--;
    }
  ite_list->select();
  adjust_visible();
  return(ite_list->get());
}


//get next object
void * win_select::previous()
{
  ite_list->unselect();
  if(ite_list!=l_list.begin())
    { 
      ite_list--;    
    }
  ite_list->select();
  adjust_visible();
  return(ite_list->get());  
}

//get cur object
void * win_select::get()
{
  if(ite_list==NULL) return NULL;
  return(ite_list->get());
}


u_int16 win_select::get_pos()
{
  u_int16 pos=1;
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && i!=ite_list) {i++;pos++;};
  if(i!=l_list.end())  return pos;
  else return 0;
}











