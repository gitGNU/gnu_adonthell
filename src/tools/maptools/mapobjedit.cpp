#include "mapobject.h"
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

  
    mapobject mobj;
    if(argc==2) mobj.load(argv[1]);
    mobj.editor();

    input::shutdown();
    return 0;
}
