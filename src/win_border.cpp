#include <iostream.h>
#include <string.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"
#include "win_container.h"
#include "win_border.h"

win_border::win_border(win_container * tmpwc)
{
  wc=tmpwc;
  h_border_template=NULL;
  v_border_template=NULL;
  corner=NULL;
  h_border=NULL;
  v_border=NULL;
  visible=false;
}

win_border::win_border(char * rep,win_container * tmpwc)
{
  wc=tmpwc;
  h_border_template=NULL;
  v_border_template=NULL;
  corner=NULL;
  h_border=NULL;
  v_border=NULL;
  visible=false;
  win_border::load(rep);
}

void win_border::show()
{
  visible=true;
}

void win_border::hide()
{
  visible=false;
}

void win_border::update()
{
  if(v_border && h_border)
    {
      v_border->resize(v_border_template->length,wc->geth());
      v_border->putbox_tile_img(v_border_template);
      h_border->resize(wc->getl(),h_border_template->height);
      h_border->putbox_tile_img(h_border_template); 
    }
}

win_border::~win_border()
{
  delete h_border;
  delete v_border;
  delete h_border_template;
  delete v_border_template;
  delete corner;
}

void win_border::load(char * rep)
{
  if(h_border_template) delete h_border_template; 
  if(v_border_template) delete v_border_template; 
  if(corner) delete corner;
  char path[255];char tmp[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BORDER_DIRECTORY);
  strcat(path,rep);
  h_border_template=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_H_BORDER_TEMPLATE_FILE);
  h_border_template->load(tmp);
  
  v_border_template=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_V_BORDER_TEMPLATE_FILE);
  v_border_template->load(tmp);
  
  corner=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_CORNER_FILE);
  corner->load(tmp);

  h_border=new image();
  v_border=new image();

  //  delete [] tmp;
  //delete [] path;
}


void win_border::draw()
{
  if(visible)
    {
#define WIN_CORNER_RELX (corner->length>>1)
#define WIN_CORNER_RELY (corner->height>>1)
#define WIN_HBRELX (h_border_template->length>>1)
#define WIN_HBRELY (h_border_template->height>>1)
#define WIN_VBRELX (v_border_template->length>>1)
#define WIN_VBRELY (v_border_template->height>>1)
      

      h_border->putbox(wc->getx()-WIN_HBRELX,wc->gety()-WIN_HBRELY);
      h_border->putbox(wc->getx()-WIN_HBRELX,wc->gety()-WIN_HBRELY+wc->geth());
      v_border->putbox(wc->getx()-WIN_VBRELX,wc->gety()-WIN_VBRELY);
      v_border->putbox(wc->getx()+wc->getl()-WIN_VBRELX,wc->gety()-WIN_VBRELY);
      
      corner->putbox_mask(wc->getx()-WIN_CORNER_RELX,wc->gety()-WIN_CORNER_RELY);
      corner->putbox_mask(wc->getx()+wc->getl()-WIN_CORNER_RELX,wc->gety()-WIN_CORNER_RELY);
      corner->putbox_mask(wc->getx()-WIN_CORNER_RELX,wc->gety()+wc->geth()-WIN_CORNER_RELY);
      corner->putbox_mask(wc->getx()+wc->getl()-WIN_CORNER_RELX,wc->gety()+wc->geth()-WIN_CORNER_RELY);
      }
}



win_background::win_background(win_container * tmpwc)
{
  level_trans=180;
  wc=tmpwc;
  background=NULL;
  background_template=NULL;
  visible=false;
}

win_background::win_background(char *rep,win_container * tmpwc)
{
  visible=false;
  level_trans=180;
  wc=tmpwc;
  background=NULL;
  background_template=NULL;
  win_background::load(rep);
}

win_background::~win_background()
{
  wc=NULL;
  delete background;
  delete background_template;
}

void win_background::load(char *rep)
{
  if(background) delete background;
  if(background_template) delete background_template;
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BACKGROUND_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_BACKGROUND_FILE);
  background=new image();
  background_template=new image();
  background_template->load(path);
}

void win_background::update()
{
  if(background && wc) 
    {
      background->resize(wc->getl(),wc->geth());
      background->putbox_tile_img(background_template);
    }
}

void win_background::show()
{
  visible=true;
}

void win_background::hide()
{
  visible=false;
}

void win_background::draw()
{
  if(visible) background->putbox_trans(wc->getx(),wc->gety(),level_trans);
}

