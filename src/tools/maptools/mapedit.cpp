#include "landmap.h"
#include <unistd.h>

int main(int argc, char * argv[])
{
    config myconfig ("");
  
    // try to read adonthellrc
    if (!myconfig.read_adonthellrc ())
	return 1;
  
    // try to change into data directory
    if (chdir (myconfig.datadir.c_str ()))
	{
	    printf ("\nSeems like %s is no valid data directory.", myconfig.datadir.c_str ());
	    printf ("\nIf you have installed the Adonthell data files into a different location,");
	    printf ("\nplease make sure to update the $HOME/.adonthell/adonthellrc file\n");
	    return 1;
	}  
    screen::set_video_mode(320,240);
    input::init();

    u_int16 i,j;

    landmap lmap;
    mapview mview;

    mapobject mobj;
    lmap.add_submap(30,30);
    mobj.load("grass.obj");
    lmap.add_object(mobj);
    mobj.load("flag.obj");
    lmap.add_object(mobj);
    mobj.load("terry.obj");
    lmap.add_object(mobj);
    for(i=0;i<30;i++)
      for(j=0;j<30;j++)
	lmap.set_square_pattern(0,i,j,0);
    lmap.set_square_pattern(0,16,13,1);
    lmap.set_square_pattern(0,15,15,1);
    lmap.set_square_pattern(0,14,17,1);
    lmap.set_square_pattern(0,13,12,1);
    mview.attach_map(&lmap);
    mview.set_current_submap(0);
    mview.resize(11,11);
    mview.set_pos(0,0);

    mview.editor();

    /*
    while(1)
      {

    win_theme * th;
    win_font * font;
    win_container * cont;
    win_container * c2;
    
    win_label * question;
    win_label * l;
    win_select * sel;

    th=new win_theme(WIN_THEME_ORIGINAL);
    font=new win_font(WIN_THEME_ORIGINAL);

    cont=new win_container(40,40,200,150,th);
    c2=new win_container(100,10,90,150,th);
    cont->add(c2);
    
    question=new win_label(5,5,80,30,th,font);
    question->set_text("Oki ca marche!!");
    question->set_auto_height(true);
    cont->add(question);
    
    sel=new win_select(5,20,80,130,th);
    c2->add(sel);

    l=new win_label(5,5,70,15,th,font);
    l->set_text("Premier texte");
    sel->add(l);

    l=new win_label(5,35,70,15,th,font);
    l->set_text("Deuxieme texte");
    sel->add(l);

    l=new win_label(5,65,70,15,th,font);
    l->set_text("Troisieme texte");
    sel->add(l);

    sel->set_default(1);
    sel->set_visible_all(true);

    c2->set_visible_all(true);

    cont->set_visible_all(true);
    cont->set_border_visible(true);
    cont->set_background_visible(true);

    sel->set_activated(true);
    sel->set_activate_keyboard(true);

    while(!input::has_been_pushed(SDLK_ESCAPE))
      {
	cout << "ark\n";
	input::update();
	screen::drawbox(0,0,320,240,0);
	cont->update();
	//	sel->update();
	cont->draw();
	screen::show();
      }
   
    delete cont;
    delete font;
    delete th;
    }*/

    input::shutdown();
    return 0;
}
