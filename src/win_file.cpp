#include <iostream.h>
#include <string.h>
#include <list>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_border.h"
#include "win_font.h"
#include "win_base.h"
#include "win_select.h"
#include "win_container.h"
#include "win_label.h"
#include "win_write.h"
#include "win_file.h"

win_file::win_file(s_int16 x,s_int16 y,u_int16 l,u_int16 h,char * theme,win_container * wc)
{
  font=NULL;
  bord_norm=NULL;
  bord_mini=NULL;
  background=NULL;
  getcwd(cur_dir,WIN_FILE_MAX_PATH*sizeof(char));
  wc_master=wc;
  if(wc_master)
    {
      set_theme(theme);
      //create the window
      window=wc_master->add_container(x,y,l,h);
      window->set_border(bord_norm);
      window->set_background(background);
      
      //create button Ok
      button_ok=window->add_label(0,0,0,0,font);
      button_ok->set_text(WIN_FILE_BUTTON_OK_TEXT);
      button_ok->set_border(bord_mini);
      
      //create button Cancel
      button_cancel=window->add_label(0,0,0,0,font);
      button_cancel->set_text(WIN_FILE_BUTTON_CANCEL_TEXT);
      button_cancel->set_border(bord_mini);
      
      //create title
      title=window->add_label(0,0,0,0,font);
      title->set_text(WIN_FILE_TITLE_TEXT);
      title->set_border(bord_mini);
      
      //create path
      entry=window->add_write(0,0,0,0,font);
      entry->set_border(bord_mini);
      
      //create combo
      combo=window->add_container(0,0,0,0);
      combo->set_border(bord_mini);
      
      resize(l,h);
      window->show_all();//show all
      hide();
      list_dir();
    }  
}

void win_file::set_theme(char * theme)
{
  if(bord_norm) delete bord_norm;
  if(bord_mini) delete bord_mini;
  if(font) delete font;
  if(background) delete background;
  
  bord_norm=new win_border(theme,WIN_BORDER_NORMAL_SIZE);
  bord_mini=new win_border(theme,WIN_BORDER_MINI_SIZE);
  font=new win_font(theme);
  background=new win_background(theme);
  
}

void win_file::show()
{
  if(window) window->show();
}

void win_file::hide()
{
  if(window) window->hide();
}

win_file::~win_file()
{
  if(wc_master) wc_master->remove(window);
  if(window) delete window;
  if(bord_norm) delete bord_norm;
  if(bord_mini) delete bord_mini;
  if(font) delete font;
  if(background) delete background;
}

void win_file::move(s_int16 x, s_int16 y)
{
  if(window) window->move(x,y);
}

void win_file::resize(u_int16 l,u_int16 h)
{
  if(! window) return; 
  window->resize(l,h); 
  //title
  title->resize(l-2*WIN_FILE_SPACE_BETWEEN_OBJECT,font->height);
  title->move(WIN_FILE_SPACE_BETWEEN_OBJECT,WIN_FILE_SPACE_BETWEEN_OBJECT);
  //entry
  entry->resize(l-2*WIN_FILE_SPACE_BETWEEN_OBJECT,font->height);
  entry->move(WIN_FILE_SPACE_BETWEEN_OBJECT,h-WIN_FILE_SPACE_BETWEEN_OBJECT-font->height);
  //combo
  combo->resize((u_int16) (title->length-WIN_FILE_SPACE_BETWEEN_OBJECT) * WIN_FILE_COMBO_BUTTON_RATIO,h-WIN_FILE_SPACE_BETWEEN_OBJECT*4-font->height*2);
  combo->move(WIN_FILE_SPACE_BETWEEN_OBJECT,WIN_FILE_SPACE_BETWEEN_OBJECT*2+title->height);

  //button ok
  button_ok->resize(title->length-WIN_FILE_SPACE_BETWEEN_OBJECT-combo->length,font->height);
  button_ok->move(combo->x+combo->length+WIN_FILE_SPACE_BETWEEN_OBJECT,2*WIN_FILE_SPACE_BETWEEN_OBJECT + title->height);
  //button cancel
  button_cancel->resize(title->length-WIN_FILE_SPACE_BETWEEN_OBJECT-combo->length,font->height);
  button_cancel->move(combo->x+combo->length+WIN_FILE_SPACE_BETWEEN_OBJECT,3*WIN_FILE_SPACE_BETWEEN_OBJECT + title->height+ button_ok->height);
}


void win_file::list_dir()
{
  char tmppath[WIN_FILE_MAX_PATH];
  char tmpfile[WIN_FILE_MAX_PATH];
  DIR *cwd;
  struct dirent *dirlist;
  struct stat buf;
  u_int16 nbfile=0;
  win_label * tmplabel=NULL;
  //  if(combo) combo->destroy_all_label();
  
  cout << cur_dir << " 3\n";
  if(chdir(cur_dir)==0)
    {
      cout << "2\n";
      if((cwd=opendir(cur_dir)))
	{
	  while((dirlist=readdir(cwd)))
	    {
	      strcpy(tmppath,cur_dir);
	      strcat(tmppath,"/");
	      strcat(tmppath,dirlist->d_name);
	      lstat(tmppath,&buf);
	      cout << "1\n";
	      if(!S_ISDIR(buf.st_mode))
		{
		  strcpy(tmpfile,WIN_FILE_FILE_NAME);
		  strcat(tmpfile," ");
		  strcat(tmpfile,dirlist->d_name);
		  //printf("FILE: %s\n",dirlist->d_name);
		}
	      else 
		{
		  //printf("DIRE: %s\n",dirlist->d_name);
		  strcpy(tmpfile,WIN_FILE_DIR_NAME);
		  strcat(tmpfile," ");
		  strcat(tmpfile,dirlist->d_name);
		}
	      tmplabel=combo->add_label(WIN_FILE_SPACE_BETWEEN_OBJECT,5+nbfile*(5+font->height),combo->height-2*WIN_FILE_SPACE_BETWEEN_OBJECT,font->height,font);
	      tmplabel->set_text(tmpfile);
	      tmplabel=NULL;
	      
	      nbfile++;
	    }
	  //next while
	}
    }
  if(combo) combo->show_all();
}








