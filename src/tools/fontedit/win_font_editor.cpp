#include<list>
#include "types.h"
#include "image.h"
#include "input.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_border.h"
#include "win_background.h"
#include "win_label.h"
#include "win_image.h"
#include "win_write.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"
#include "win_file.h"
#include "win_font_editor.h"

win_font_editor::win_font_editor(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth,win_font * twf):win_container(tx,ty,tl,th,wth)
{
  //no font no cursor
  cursor_=newfont_=NULL;
  lastpos_=0;
  curpos_=1;

  //create a win_image
  img=new win_image(20,20,80,80,wth);
  
  //stretch to false, i would like  to set true, but an error localized with a use of putbox_part_img && zoom: WARNING ALEX
  img->set_stretch(false);
  
  //add to the container 
  add(img);

  //create a temporary image
  tmp=new image();
}


void win_font_editor::set_file(char * font,char * cursor,char * dest)
{
  //load font pnm
  newfont_=new image();
  newfont_->load_pnm(font);
  
  //load cursor pnm
  cursor_=new image();
  cursor_->load_pnm(cursor);
  
  //set the destination file
  destfile_=dest;
  
  //img set to visible
  img->set_visible(true);

  //get the first letter
  update_next_letter();
  
  //initialise the keys queue
  input::clear_keys_queue();
}


bool win_font_editor::update()
{
  static s_int32 c;
    
  if(win_container::update())
    {
      while(curpos_<=newfont_->length() && (c=input::get_next_unicode())>0)
	{
	  //create new inode to store part of image
	  Tinode_ * t = new Tinode_;
	  t->letter=(char)c;
	  t->pos=lastpos_;
	  t->length=curpos_-1-lastpos_;
	  l_.push_back(t);

	  //if not a end of font file, go to the next letter
	  if(curpos_!=newfont_->length())
	    {
	      lastpos_=curpos_++;
	      update_next_letter();
	    }
	}
  
      if(curpos_>=newfont_->length()) return false;
      
      return true;
    }
  return false;
}


void win_font_editor::write()
{

  //open a gzFILE
  gzFile dest=gzopen(destfile_,"wb");
  //put font file
  newfont_->put(dest);
  //put cursor
  cursor_->put(dest);
  //put font's information
  for(list<Tinode_*> ::iterator i=l_.begin();i!=l_.end();i++)
    {
      gzwrite(dest,&((*i)->letter),sizeof(char));
      gzwrite(dest,&((*i)->pos),sizeof(u_int16));
      gzwrite(dest,&((*i)->length),sizeof(u_int16));
    }
  gzclose(dest);
}

bool win_font_editor::update_next_letter()
{
  
  while(curpos_<=newfont_->length() && newfont_->get_pix(curpos_,newfont_->height()-1)!=0) curpos_++;
  
  tmp->resize(curpos_-lastpos_,newfont_->height()-1);
  tmp->putbox_part_img(newfont_,0,0,curpos_-lastpos_,newfont_->height()-1,lastpos_,0);
  img->set_image(tmp);
  return true;
}
















/*
win_font_editor::win_font_editor(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth,win_font * twf):win_container(tx,ty,tl,th,wth)
{
  wfile_=NULL;
  new_font_=NULL;
  cursor_=NULL;
  font__=twf;
  init();
}


void win_font_editor::init()
{
  wlab_=new win_label(0,0,0,0,theme_,font__);
  wlab_->set_text("Please, do your choice:");
  wlab_->set_auto_size(true);
  set_justify(wlab_,WIN_JUSTIFY_LEFT);
  add(wlab_);
  
  wfile_=new win_file(20,20,200,200,theme_,font__);
  
  select_menu_=new win_select(0,30,200,200,theme_);
  select_menu_->set_layout(WIN_LAYOUT_LIST);
  select_menu_->set_justify(WIN_JUSTIFY_LEFT);
  add(select_menu_);
  set_justify(select_menu_,WIN_JUSTIFY_CENTER);
  set_visible_all(true);
  set_visible(false);
  set_border_visible(true);
  set_menu1();

  //signal def
  set_signal_connect(makeFunctor(*this,&win_font_editor::_on_activate__),WIN_SIG_ACTIVATE);
  set_signal_connect(makeFunctor(*this,&win_font_editor::_on_draw__),WIN_SIG_DRAW);
  set_signal_connect(makeFunctor(*this,&win_font_editor::_on_update__),WIN_SIG_UPDATE);

  imgletter_=new win_image(0,0,40,40,theme_);
  imgletter_->set_stretch(true);
  imgletter_->set_visible(false);
  
}


void win_font_editor::set_menu1()
{
  if(!select_menu_) return;
  select_menu_->destroy();
  win_label * item;
  
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU1_C1);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu1_c1_activate__),WIN_SIG_ACTIVATE);
  
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU1_C2);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu1_c2_activate__),WIN_SIG_ACTIVATE);

  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU1_C3);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu1_c3_activate__),WIN_SIG_ACTIVATE);
  
  select_menu_->set_visible_all(true);
  select_menu_->set_default(1);
}


void win_font_editor::set_edit_font_menu()
{
  wlab_->set_text("Push the equivalent key:");
  select_menu_->destroy();
  imgletter_->move(20,20);
  imgletter_->set_visible(true);
}



void win_font_editor::set_menu2()
{
  if(!select_menu_) return;
  select_menu_->destroy();
  win_label * item;
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU2_C1);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu2_c1_activate__),WIN_SIG_ACTIVATE);

  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU2_C2);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu2_c2_activate__),WIN_SIG_ACTIVATE);
  
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU2_C3);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu2_c3_activate__),WIN_SIG_ACTIVATE);
  
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU2_C4);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu2_c4_activate__),WIN_SIG_ACTIVATE);
  
  item=new win_label(0,0,0,0,theme_,font__);
  item->set_text(WIN_FONT_EDITOR_MENU2_C5);
  item->set_auto_size(true);
  select_menu_->add(item);
  item->set_signal_connect(makeFunctor(*this,&win_font_editor::_on_menu2_c5_activate__),WIN_SIG_ACTIVATE);

  select_menu_->set_visible_all(true);
  select_menu_->set_default(1);
}


void win_font_editor::_on_activate__()
{
  win_select::set_cur_select(select_menu_);
  select_menu_->set_activated(true);
}


bool win_font_editor::update()
{
  if(win_container::update())
    {
      if(wfile_->is_visible()) wfile_->update();

      if(imgletter_)
	{
	  
	  
	  
	}
      return true;
    }
  return false;
}

void win_font_editor::_on_update__()
{
  //select_menu_->update();
  
  
}


void win_font_editor::_on_draw__()
{
  //select_menu_->draw();
  if(wfile_->is_visible()) wfile_->draw();
  if(imgletter_->is_visible()) imgletter_->draw();
}


void win_font_editor::_on_menu1_c1_activate__()
{
  set_menu2();
}


void win_font_editor::_on_menu1_c2_activate__()
{
  
}


void win_font_editor::_on_menu1_c3_activate__()
{
  exit(0);
}

void win_font_editor::_on_menu2_c1_activate__()
{
  set_visible(false);
  set_activated(false);
  
  wfile_->set_title("Select PNM font");
  wfile_->set_activated(true);
  wfile_->set_visible(true);
  
  Functor0 F1_=makeFunctor(*this,&win_font_editor::_on_wf_bt_ok_load_pnm__);
  Functor0 F2_=makeFunctor(*this,&win_font_editor::_on_wf_bt_cancel_load_pnm__);
  wfile_->set_sig_activate_button_ok(F1_);
  wfile_->set_sig_activate_button_cancel(F2_);
  
  if(new_font_) 
    {
      delete new_font_;
      new_font_=NULL;
    }
}


void win_font_editor::_on_menu2_c2_activate__()
{
  set_visible(false);
  set_activated(false);
  
  wfile_->set_title("Select PNM Cursor");
  wfile_->set_activated(true);
  wfile_->set_visible(true);
  
  Functor0 F1_=makeFunctor(*this,&win_font_editor::_on_wf_bt_ok_load_pnm__);
  Functor0 F2_=makeFunctor(*this,&win_font_editor::_on_wf_bt_cancel_load_pnm__);
  wfile_->set_sig_activate_button_ok(F1_);
  wfile_->set_sig_activate_button_cancel(F2_);
  
  if(cursor_) 
    {
      delete cursor_;
      cursor_=NULL;
    }
}



void win_font_editor::_on_menu2_c3_activate__()
{
  if(new_font_ && cursor_)
    {
      win_select::set_activate_keyboard(false);
      nbletter_=0;
      lastpos_=0;
      curpos_=1;
    }
}

bool win_font_editor::next_letter()
{
  while(curpos_<new_font_->length && new_font_->get_pix(curpos_,new_font_->height-1)!=0) i++;  
  if(curpos_==new_font_->length) return false;
  return true;
}

void win_font_editor::update_font()
{
  static s_int32 c;
  while((c=input::get_next_unicode())>0 && curpos_<new_font_->length)
    {
      
      
    }
}


void win_font_editor::_on_menu2_c4_activate__()
{
  
}

void win_font_editor::_on_menu2_c5_activate__()
{
  set_menu1();
}

void win_font_editor::_on_wf_bt_cancel_load_pnm__()
{ 
  wfile_->set_activated(false);
  wfile_->set_visible(false);
  set_menu2();
  win_select::set_cur_select(select_menu_);
  set_activated(true);
  set_visible(true);
}


void win_font_editor::_on_wf_bt_ok_load_pnm__()
{
  wfile_->set_activated(false);
  wfile_->set_visible(false);
  
  set_menu2();
  win_select::set_cur_select(select_menu_);
  
  set_activated(true);
  set_visible(true);
  
  if(!new_font_)
    {
      new_font_=new image();
      new_font_->load_pnm(wfile_->get_text());
    }
  else {
    if(!cursor_)
      {
	cursor_=new image();
	cursor_->load_pnm(wfile_->get_text());
      }
  }
}




*/







