#include <iostream.h>
#include <list>
#include "types.h"
#include "win_types.h"
#include "image.h"
#include "win_font.h"
#include "win_base.h"
#include "win_write.h"
#include "win_label.h"
#include "win_image.h"
#include "win_container.h"
#include "win_select.h"


Type_win_select::Type_win_select(win_image * p,win_select * ws,u_int8 m)
{
  img=p;
  p->wselect=ws;
  wselect=ws;
  mode=m;
  id=2;
  p->select_mode=m;
  if(ws->cur_border)
    {
      n_border=p->get_border();
    }
}


Type_win_select::Type_win_select(win_label * p,win_select * ws,u_int8 m)
{
  lab=p;
  p->wselect=ws;
  wselect=ws;
  mode=m;
  p->select_mode=m;
  id=0;
  if(ws->cur_border)
    {
      n_border=p->get_border();
    }
}

Type_win_select::Type_win_select(win_write * p,win_select * ws,u_int8 m)
{
  wri=p;
  p->wselect=ws;
  wselect=ws;
  mode=m;
  p->select_mode=m;
  id=1;
  if(ws->cur_border)
    {
      n_border=p->get_border();
    }
}

Type_win_select::Type_win_select(win_container * p,win_select * ws,u_int8 m)
{
  con=p;
  wselect=ws;
  p->wselect=ws;
  mode=m;
  p->select_mode=m;
  id=3;
  if(ws->cur_border)
    {
      n_border=p->get_border();
    }
}

void Type_win_select::select()
{
  switch(id)
    {
    case 0:
      if(mode==WIN_SELECT_MODE_BORDER) lab->set_border(wselect->cur_border); 
      lab->select();
      break;
    case 1:
      if(mode==WIN_SELECT_MODE_BORDER) wri->set_border(wselect->cur_border); 
      wri->select();
      break;
    case 2:
      if(mode==WIN_SELECT_MODE_BORDER) img->set_border(wselect->cur_border); 
      img->select();
      break;
    case 3:
      if(mode==WIN_SELECT_MODE_BORDER) con->set_border(wselect->cur_border); 
      con->select();
      break;
    }
}

void Type_win_select::unselect()
{
  switch(id)
    {
    case 0:
      if(mode==WIN_SELECT_MODE_BORDER) lab->set_border(NULL);
      lab->unselect();
      break;
    case 1:
      if(mode==WIN_SELECT_MODE_BORDER) wri->set_border(NULL);
      wri->unselect();
      break;
    case 2:
      if(mode==WIN_SELECT_MODE_BORDER) img->set_border(NULL);
      img->unselect();
      break;
    case 3:
      if(mode==WIN_SELECT_MODE_BORDER) con->set_border(NULL);
      con->unselect();
      break;
    }
}

void * Type_win_select::get()
{
  switch(id)
    {
    case 0:
      return(lab);
      break;
    case 1:
      return(wri);
      break;
    case 2:
      return(img);
      break;
    case 3:
      return(con);
      break;
    }
  return(NULL);
}

Type_win_select::~Type_win_select()
{
 switch(id)
   {
    case 0:
      lab->wselect=NULL;
      lab->set_border(n_border);
      break;
    case 1:
      wri->wselect=NULL;
      wri->set_border(n_border);
      break;
    case 2:
      img->wselect=NULL;
      img->set_border(n_border);
      break;
    case 3:
      con->wselect=NULL;
      con->set_border(n_border);
      break;
    }
}

void Type_win_select::activate()
{
switch(id)
   {
    case 0:
      lab->activate();
      break;
    case 1:
      wri->activate();
      break;
    case 2:
      img->activate();
      break;
    case 3:
      con->activate();
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

win_select::win_select()
{
  l_list.clear();
  cur_border=NULL;
  ite_list=l_list.begin();
}

win_select::~win_select()
{
  l_list.clear();
}


void win_select::add(win_label *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  
  l_list.push_back(*tmp);
  tmp=NULL;
}

void win_select::add(win_write *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
}

void win_select::add(win_image *p, u_int8 m=0)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
}

void win_select::add(win_container *p,u_int8 m=1)
{
  Type_win_select * tmp=new Type_win_select(p,this,m);
  l_list.push_back(*tmp);
  tmp=NULL;
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

void win_select::set_default(void * tmp)
{
  list<Type_win_select>::iterator i=l_list.begin();
  while(i!=l_list.end() && tmp!=i->get()) i++;
  if(i!=l_list.end()) ite_list=i;
  ite_list->select();
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




void * win_select::next()
{
  ite_list->unselect();
  ite_list++;
  if(ite_list==l_list.end())
    {
      ite_list--;
    }
  ite_list->select();
  return(ite_list->get());
}


void * win_select::preview()
{
  ite_list->unselect();
  if(ite_list!=l_list.begin())
    { 
      ite_list--;    
    }
  ite_list->select();
  return(ite_list->get());  
}

void * win_select::get()
{
  if(ite_list==NULL) return NULL;
  return(ite_list->get());
}














