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


win_border::win_border(char * rep)
{
  h_border_template=NULL;
  v_border_template=NULL;
  corner=NULL;
  win_border::load(rep);
}


/*
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
*/

win_border::~win_border()
{
 
  if(h_border_template) delete h_border_template;
  if(v_border_template)  delete v_border_template;
  if(corner) delete corner;
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
}




win_background::win_background(char *rep)
{
  background_template=NULL;
  win_background::load(rep);
}

win_background::~win_background()
{
  delete background_template;
}

void win_background::load(char *rep)
{
  if(background_template) delete background_template;
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BACKGROUND_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_BACKGROUND_FILE);
  background_template=new image();
  background_template->load(path);
}

