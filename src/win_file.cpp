#include <list>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_scrollbar.h"
#include "win_theme.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"
#include "win_file.h"

win_file::win_file(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * theme,win_font * wf):win_container(tx,ty,tl,th,theme)
{
  font_=wf;
  
  getcwd(cur_dir_,255*sizeof(char));
  
  title_=new win_label(0,0,0,0,theme_,font_);
  title_->set_text("Select file or directory");
  title_->set_border_size(WIN_SIZE_MINI);

  button_ok_=new win_label(0,0,0,0,theme_,font_);
  button_ok_->set_text("Ok");
  button_ok_->set_border_visible(true);
  button_ok_->set_border_size(WIN_SIZE_MINI);
  
  button_cancel_=new win_label(0,0,0,0,theme_,font_);
  button_cancel_->set_text("Cancel");
  button_cancel_->set_border_visible(true);
  button_cancel_->set_border_size(WIN_SIZE_MINI);
  
  entry_=new win_write(0,0,0,0,theme_,font_);
  entry_->set_border_visible(true);
  entry_->set_border_size(WIN_SIZE_MINI);
  
  combo_file_=new win_select(0,0,0,0,theme_);
  combo_file_->set_border_visible(true);
  combo_file_->set_border_size(WIN_SIZE_MINI);
  combo_file_->set_space_between_border(5);
  combo_file_->set_justify(WIN_JUSTIFY_LEFT);
  combo_file_->set_layout(WIN_LAYOUT_LIST);
  combo_file_->set_scrollbar_visible(true);
  
  combo_=new win_select(0,0,0,0,theme_);
  combo_->set_border_visible(false);
  combo_->add(combo_file_);
  combo_->add(button_ok_);
  combo_->add(button_cancel_);
  combo_->add(entry_);
  combo_->set_default(combo_file_);
  combo_->set_select_mode(WIN_SELECT_MODE_BRIGHTNESS);
  combo_->set_visible_all(true);
  
  space_between_object_=10;
  
  add(title_);
  add(combo_);
  resize(length_,height_);
  set_visible_all(true);
  set_border_visible(true);
  set_background_visible(true);
  set_border_visible(true);
  set_visible(false);
  
  //set now the callback
  set_signal_connect(makeFunctor(*this,&win_file::_on_activate__),WIN_SIG_ACTIVATE);
  combo_file_->set_signal_connect(makeFunctor(*this,&win_file::_on_activate_combo_file__),WIN_SIG_ACTIVATE_KEY);
  entry_->set_signal_connect(makeFunctor(*this,&win_file::_on_activate_key_entry__),WIN_SIG_ACTIVATE_KEY);
  list_directory();
}

void win_file::_on_activate_key_entry__()
{
  struct stat buf;
  char *p=entry_->get_text();
  lstat(p,&buf);
  if(S_ISDIR(buf.st_mode))
    {
      strcpy(cur_dir_,p);
      list_directory();
    }
  else
    {
      if(S_ISREG(buf.st_mode))
	{
	  int i=strlen(p)-1; 
	  while(i>=0 && p[i]!='/') i--;
	  strcpy(cur_file_,(p+i+1));
	}
    }
  combo_->set_default(button_ok_);
  entry_->set_activated(false);
  combo_->set_activated(true);
}

void win_file::_on_activate_combo_file__()
{ 
  win_label * lab_=(win_label*)combo_file_->get();
  char * p=lab_->get_text();
  if(!strncmp(p,"<D>",3))
    {
      if(!strcmp((p+4),".")) return;
      if(!strcmp((p+4),"..") && strcmp(cur_dir_,"/"))
	{
	  int i=strlen(cur_dir_)-1;
	  while(i>=0 && cur_dir_[i]!='/') i--;
	  if(i==0) strcpy(cur_dir_,"/");
	  else cur_dir_[i]='\0';
	}
      else
	{
	  if(cur_dir_[strlen(cur_dir_)-1]!='/') strcat(cur_dir_,"/");
	  strcat(cur_dir_,(p+4));
	}
      list_directory();
    }
  else
  {
    char tmp[500];
    //directory
    strcpy(tmp,cur_dir_);
    if(cur_dir_[strlen(cur_dir_)-1]!='/')  strcat(tmp,"/");
    strcat(tmp,(p+4));
    strcpy(cur_file_,(p+4));
    entry_->set_text(tmp);
  }
}

//function called by on_activate
void win_file::_on_activate__()
{
  combo_->set_activated(true);
  win_select::set_cur_select(combo_);
}

void win_file::resize(u_int16 tl,u_int16 th)
{
  win_container::resize(tl,th);
  title_->set_auto_size(true);
  title_->move(0,space_between_border_);
  set_justify(title_,WIN_JUSTIFY_CENTER);
  
  combo_->resize(length_,height_-space_between_border_-title_->height());
  combo_->move(0,height_-combo_->height());
 
  entry_->resize(combo_->length()-(space_between_border_<<1),font_->height());
  entry_->move(0,combo_->height()-space_between_border_-entry_->height());
  set_justify(entry_,WIN_JUSTIFY_CENTER);
  
  combo_file_->resize(((combo_->length()*3)>>2)-space_between_border_-(space_between_object_>>1),
		      combo_->height()-entry_->height()-space_between_border_-(space_between_object_<<1));
  combo_file_->move(space_between_border_,space_between_object_);
  
  
  button_ok_->resize((combo_->length()>>2)-space_between_border_-(space_between_object_>>1),font_->height());
  button_ok_->move(combo_->length()-button_ok_->length()-space_between_border_,space_between_object_);
  
  button_cancel_->resize((combo_->length()>>2)-space_between_border_-(space_between_object_>>1),
			 font_->height());
  button_cancel_->move(combo_->length()-button_ok_->length()-space_between_border_,
		       button_ok_->height()+(space_between_object_<<1));
}

void win_file::set_title(char * til)
{
  title_->set_text(til);
  resize(length_,height_);
}

void win_file::set_curdirectory(char * dir)
{
  strcpy(cur_dir_,dir);
  list_directory();
}

void win_file::list_directory()
{
  char tmppath[255];
  char tmpfile[255];
  DIR *cwd;
  struct dirent *dirlist;
  struct stat buf;
  combo_file_->destroy();
  win_label * tmplabel=NULL;
  if(chdir(cur_dir_)==0)
    {
      if((cwd=opendir(cur_dir_)))
	{
	  while((dirlist=readdir(cwd)))
	    {
	      strcpy(tmppath,cur_dir_);
	      strcat(tmppath,"/");
	      strcat(tmppath,dirlist->d_name);
	      lstat(tmppath,&buf);
	      if(!S_ISDIR(buf.st_mode)) strcpy(tmpfile,"<F>");
	      else strcpy(tmpfile,"<D>");
	      strcat(tmpfile," ");
	      strcat(tmpfile,dirlist->d_name);
	      tmplabel=new win_label(0,0,combo_file_->length()-space_between_border_-theme_->scrollbar->back->length(),font_->height(),theme_,font_);
	      tmplabel->set_text(tmpfile);
	      combo_file_->add(tmplabel);
	      tmplabel=NULL;
	    }
	}
    }
  entry_->set_text(cur_dir_);
  combo_file_->set_default(1);
  combo_file_->set_visible_all(true);
}

win_file::~win_file()
{
  
}

void win_file::set_sig_activate_button_ok(Functor0 &func)
{
  if(button_ok_) 
    button_ok_->set_signal_connect(func,WIN_SIG_ACTIVATE);
}

void win_file::set_sig_activate_button_cancel(Functor0 &func)
{
  if(button_cancel_) 
    button_cancel_->set_signal_connect(func,WIN_SIG_ACTIVATE);
}







