#include <list>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "types.h"
#include "image.h"
#include "input.h"
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
#include "win_file_select.h"

win_file_select::win_file_select(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * theme,win_font * wf, char * fext, char * bdir=NULL):win_container(tx,ty,tl,th,theme)
{
  font_=wf;
  
  getcwd(cur_dir_,255*sizeof(char));
  strcpy(base_dir_,cur_dir_);
  if(bdir)
    {
      if(cur_dir_[strlen(base_dir_)-1]!='/') strcat(base_dir_,"/");
      strcat(base_dir_,bdir);
      strcpy(cur_dir_,base_dir_);
    }
  strcpy(ext,fext);

  curdir_=new win_label(0,0,0,0,theme_,font_);
  curdir_->set_border_size(WIN_SIZE_MINI);
  
  combo_file_=new win_select(0,0,0,0,theme_);
  combo_file_->set_border_visible(true);
  combo_file_->set_border_size(WIN_SIZE_MINI);
  combo_file_->set_space_between_border(5);
  combo_file_->set_align_all(WIN_ALIGN_LEFT);
  combo_file_->set_layout(WIN_LAYOUT_LIST);
  combo_file_->set_scrollbar_visible(true);
  
  space_between_object_=10;
  
  add(curdir_);
  add(combo_file_);
  resize(length_,height_);
  set_visible_all(true);
  set_border_visible(true);
  set_background_visible(true);
  set_border_visible(true);
  set_visible(false);
  
  /*
  //set now the callback
  set_signal_connect(makeFunctor(*this,&win_file::_on_activate__),WIN_SIG_ACTIVATE);
  combo_file_->set_signal_connect(makeFunctor(*this,&win_file::_on_activate_combo_file__),WIN_SIG_ACTIVATE_KEY);*/
  list_directory(ext);
}

void win_file_select::resize(u_int16 tl,u_int16 th)
{
  win_container::resize(tl,th);
  curdir_->set_auto_size(true);
  curdir_->move(10,10);
  //  set_justify(curdir_,WIN_JUSTIFY_CENTER);
  
  combo_file_->resize(length_-10,height_-40);
  combo_file_->move(5,30);
}


void win_file_select::set_curdirectory(char * dir)
{
  if(cur_dir_[strlen(cur_dir_)-1]!='/') strcat(cur_dir_,"/");
  strcat(cur_dir_,dir);
  list_directory(ext);
}

void win_file_select::list_directory(char * ext)
{
  char tmppath[255];
  char tmpfile[255];
  DIR *cwd;
  struct dirent *dirlist;
  struct stat buf;
  static char wdsave[500];
  getcwd(wdsave,500);
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
	      if(!S_ISDIR(buf.st_mode)) 
		{
		  bool ok=true;
		  u_int16 i;
		  u_int16 st=strlen(dirlist->d_name)-strlen(ext);
		  for(i=0;i<strlen(ext);i++)
		    {
		      if(ext[i]!=dirlist->d_name[i+st]) ok=false;
		    }
		  if(!ok) continue;
		  strcpy(tmpfile,"<F>");
		}
	      else strcpy(tmpfile,"<D>");
	      strcat(tmpfile," ");
	      strcat(tmpfile,dirlist->d_name);
	      tmplabel=new win_label(0,0,combo_file_->length()-space_between_border_-theme_->scrollbar->back->length(),font_->height(),theme_,font_);
	      tmplabel->set_text(tmpfile);
	      combo_file_->add(tmplabel);
	      tmplabel=NULL;
	    }
	}
      chdir(wdsave);
    }
  curdir_->set_text(cur_dir_);
  combo_file_->set_default(1);
  combo_file_->set_visible_all(true);
}

win_file_select::~win_file_select()
{
  
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

char * win_file_select::wait_for_select(const Functor0 & updatefunc,
					const Functor0 & drawfunc)
{
  u_int16 i;
  char *s;
  set_visible(true);
  set_activated(true);
  set_signal_connect(updatefunc,WIN_SIG_UPDATE);
  set_signal_connect(drawfunc,WIN_SIG_DRAW);

  input::clear_keys_queue();
  input::set_key_repeat();
  combo_file_->set_activated(true);
  win_select::set_cur_select(combo_file_);  
  while(1)
    {
      input::update();
      if(testkey(SDLK_DOWN)) 
	{
	  combo_file_->next();
	}
      if(testkey(SDLK_UP)) 
	{
	  combo_file_->previous();
	}
      if(input::has_been_pushed(SDLK_ESCAPE))
      {
	return NULL;
      }
      if(input::has_been_pushed(SDLK_RETURN))
      {
	win_label * lab_=(win_label*)combo_file_->get();
	char * p=lab_->get_text();
	if(!strncmp(p,"<D>",3))
	  {
	    if(strcmp((p+4),".")) 
	      if(!strcmp((p+4),".."))
		{
		  if(strcmp(cur_dir_,base_dir_))
		    {
		      int i=strlen(cur_dir_)-1;
		      while(i>=0 && cur_dir_[i]!='/') i--;
		      if(i==0) strcpy(cur_dir_,"/");
		      else cur_dir_[i]='\0';
		    }
		}
	      else
		{
		  if(cur_dir_[strlen(cur_dir_)-1]!='/') strcat(cur_dir_,"/");
		  strcat(cur_dir_,(p+4));
		}
	    list_directory(ext);
	  }
	else
	  {
	    char tmp[500];
	    strcpy(tmp,cur_dir_);
	    if(cur_dir_[strlen(cur_dir_)-1]!='/')  strcat(tmp,"/");
	    strcat(tmp,(p+4));
	    input::set_key_repeat(0);
	    input::clear_keys_queue();
	    s=strdup(tmp+strlen(base_dir_));
	    return s;
	  }
      }
      for(i=0;i<screen::frames_to_do();i++) update();
      draw();
      screen::show();
    }
}
