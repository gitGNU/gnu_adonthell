/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <string.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"

win_border::win_border(char * rep,char * size=WIN_BORDER_NORMAL_SIZE)
{
  h_border_template=NULL;
  v_border_template=NULL;
  h_border=NULL;
  v_border=NULL;
  corner_top_left=NULL;
  corner_top_right=NULL;
  corner_bottom_left=NULL;
  corner_bottom_right=NULL;
  win_border::load(rep,size);
}

win_border::win_border(win_border & wb)
{
  h_border=NULL;
  v_border=NULL;
  h_border_template=NULL;
  v_border_template=NULL;
  corner_top_left=NULL;
  corner_top_right=NULL;
  corner_bottom_left=NULL;
  corner_bottom_right=NULL;
  *this=wb;
}

win_border::~win_border()
{
  destroy();
}

win_border & win_border::operator=(win_border & wb)
{
  destroy();
  h_border_template=new image();
  *h_border_template=*(wb.h_border_template);
  v_border_template=new image();
  *v_border_template=*(wb.v_border_template);
  corner_bottom_left=new image();
  *corner_bottom_left=*(wb.corner_bottom_left);
  corner_bottom_right=new image();
  *corner_bottom_right=*(wb.corner_bottom_right);
  corner_top_left=new image();
  *corner_top_left=*(wb.corner_top_left);
  corner_top_right=new image();
  *corner_top_right=*(wb.corner_top_right);
  h_border=new image();
  v_border=new image();
  return *this;
}

void win_border::destroy()
{
  if(h_border_template) delete h_border_template;
  if(v_border_template)  delete v_border_template;
  if(corner_bottom_left) delete corner_bottom_left;
  if(corner_bottom_right) delete corner_bottom_right;
  if(corner_top_left) delete corner_top_left;
  if(corner_top_right) delete corner_top_right;
  if(h_border) delete h_border;
  if(v_border)  delete v_border;
  h_border=NULL;
  v_border=NULL;
  h_border_template=NULL;
  v_border_template=NULL;
  corner_bottom_left=NULL;
  corner_bottom_right=NULL;
  corner_top_left=NULL;
  corner_top_right=NULL;
}

void win_border::load(char * rep,char *size)
{
  destroy();
  char path[255];char tmp[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BORDER_DIRECTORY);
  strcat(path,rep);
  strcat(path,size);
  h_border_template=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_H_BORDER_TEMPLATE_FILE);
  h_border_template->load_pnm(tmp);//new
  
  v_border_template=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_V_BORDER_TEMPLATE_FILE);
  v_border_template->load_pnm(tmp);//new
  
  corner_top_left=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_CORNER_TOP_LEFT_FILE);
  corner_top_left->load_pnm(tmp);//new
  
  corner_top_right=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_CORNER_TOP_RIGHT_FILE);
  corner_top_right->load_pnm(tmp);//new

  corner_bottom_left=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_CORNER_BOTTOM_LEFT_FILE);
  corner_bottom_left->load_pnm(tmp);//new

  corner_bottom_right=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_CORNER_BOTTOM_RIGHT_FILE);
  corner_bottom_right->load_pnm(tmp);//new

  v_border=new image();
  h_border=new image();
}

void win_border::update(win_base *wb)
{
  if(v_border && h_border && wb)
    {
      v_border->resize(v_border_template->length(),wb->height());
      v_border->putbox_tile_img(v_border_template);
      h_border->resize(wb->length(),h_border_template->height());
      h_border->putbox_tile_img(h_border_template); 
    }
}














