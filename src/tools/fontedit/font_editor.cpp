#include <stdio.h>
#include <unistd.h>
#include <list>
#include <stdlib.h>
#include "SDL_thread.h"
#include "types.h"
#include "screen.h"
#include "image.h"
#include "input.h"
#include "prefs.h"
#include "win_types.h"
#include "win_background.h"
#include "win_border.h"
#include "win_theme.h"
#include "win_font.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"
#include "win_image.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"
#include "win_font_editor.h"

int main(int argc,char **argv)
{
  if(argc!=4)
    {
      cout << argv[0] << "[font.pnm] [cursor.pnm] [dest_file.font]\n";  
      exit(1);
    }
  
  config myconfig (argc > 1 ? argv[1] : "");
  // try to read adonthellrc
  myconfig.read_adonthellrc ();
  // change into data directory
  chdir (myconfig.datadir.data ());
  //clavier
  screen::set_video_mode(320,240);
  input::init();


  //load theme and font 
  win_theme theme(WIN_THEME_ORIGINAL);
 

  win_font font(WIN_THEME_ORIGINAL);
 
  //create a font editor
  win_font_editor test(20,20,200,200,&theme,&font); 
 

  test.set_visible(true);
  
  //set file for font editor
  test.set_file(argv[1],argv[2],argv[3]);
  
  
  while(test.update() && !(input::has_been_pushed(SDLK_ESCAPE)))
    {
      screen::drawbox(0,0,320,240,0x00A100);	
      test.update();
      input::update();
      test.draw();
      screen::show();
    }
 

  test.write();

  
  /*
  gzFile f=gzopen("/home/jol/essai.font","rb");
  image a;
  a.get(f);
  a.get(f);
  char c;
  u_int16 p,l;
  while(!gzeof(f))
    {
      gzread(f,&c,sizeof(char));
      gzread(f,&p,sizeof(u_int16));
      gzread(f,&l,sizeof(u_int16));
      printf("%c --- %d --- %d\n",c,p,l);
    }

  gzclose(f);

  */
}








