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

    landmap lmap;
    mapview mview;

    /*    mapobject mobj;
    lmap.add_submap(30,30);
    mobj.load("grass.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"grass.obj");
    mobj.load("flag.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"flag.obj");
    mobj.load("terry.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"terry.obj");
    for(i=0;i<30;i++)
      for(j=0;j<30;j++)
	lmap.set_square_pattern(0,i,j,0);
    lmap.set_square_pattern(0,16,13,1);
    lmap.set_square_pattern(0,15,15,1);
    lmap.set_square_pattern(0,14,17,1);
    lmap.set_square_pattern(0,13,12,1);
    */

    lmap.load("mymap.map");
    mview.attach_map(&lmap);
    mview.set_current_submap(0);
    mview.resize(11,11);
    mview.set_pos(0,0);

    mview.editor();
    //    lmap.save("mymap.map");

    input::shutdown();
    return 0;
}
