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
#include "win_background.h"

win_background::win_background(char *rep)
{
  background_template=NULL;
  background=NULL;
  win_background::load(rep);
}

win_background::win_background(win_background & wb)
{
  background_template=NULL;
  background=NULL;
  *this=wb;
}

win_background::~win_background()
{
  if(background_template)delete background_template;
  if(background) delete background;
}

win_background & win_background::operator=(win_background & wb)
{
  if(background_template) delete background_template;
  if(background) delete background;
  background_template=new image();
  *background_template=*(wb.background_template);
  background=new image();
  return *this;
}



void win_background::load(char *rep)
{
  if(background_template) delete background_template;
  if(background) delete background;
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BACKGROUND_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_BACKGROUND_FILE);
  background_template=new image();
  background_template->load_pnm(path);//new
  background=new image();
}

void win_background::update(win_base * wb)
{
  if(wb)
    {
      background->resize(wb->length(),wb->height());
      background->tile (*background_template); 
//       background->putbox_tile_img(background_template);
    }
}


