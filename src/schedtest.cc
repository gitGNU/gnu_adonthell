#include <iostream.h>

#include "python_class.h"
#include "event_handler.h"
#include "input_manager.h"
#include "character.h"
#include "gametime.h"
#include "gamedate.h"
#include "schedule.h"
#include "screen.h"

extern "C"
{
    void initbasec (void);
}

// our global test scbedule
schedule myschedule;
character myself;

// tell the program to quit
int quit = 0;

// update time and schedule
void mainloop ()
{
    while (!quit)
    {
        // perform operations to keep the game's speed constant
        gametime::update ();

        // update the internal clock
        gamedate::update ();

        // run the schedule
        for (int i = 0; i < gametime::frames_to_skip (); i++)
        {
            input_manager::update ();
            myschedule.update ();
        }
    }
}

// catch escape key
int escape_pushed (input_event *e)
{
    keyboard_event *ev = (keyboard_event *) e;

    if (ev->type() == keyboard_event::KEY_PUSHED &&
        ev->key () == keyboard_event::ESCAPE_KEY)
        quit = 1;

    return 0;
}

int main (int argc, char* argv[])
{
    // gfx
    screen::init ();
    screen::set_video_mode (640, 480);

    // python system
    python::init ();
    python::insert_path ("adontest");
    initbasec ();
    PyObject *module = python::import_module ("base");
    data::globals = PyModule_GetDict (module);

    // input system
    input_manager::init ();
    input_listener listener;
    input_manager::add (&listener);

    Functor1wRet<input_event *, int> ftor;
    ftor = makeFunctor (&ftor, &escape_pushed);
    listener.connect_function (input_event::KEYBOARD_EVENT, ftor);

    // event system
    event_handler::init ();

    // gametime system
    // (2 realtime minutes make approx. 1 gametime day)
    gametime::init (2);
    gametime::start_action ();

    cout << "A game minute takes " << gametime::minute () << " cycles" << endl;

    // a clock telling every hour
    time_event clock ("1h");
    clock.set_repeat ("1h");
    clock.set_script ("clock");
    event_handler::register_event (&clock);

    // create a character for the schedule
    myself.set_name ("Kai Sterker");
    myself.set_schedule (&myschedule);
    data::characters[myself.get_id ().c_str ()] = &myself;

    // all we need to do is setting the manager script;
    PyObject *args = Py_BuildValue ("(s)", myself.get_id ().c_str ());
    myschedule.set_manager ("student", args);
    myschedule.set_active (true);

    // mainloop
    mainloop ();

    // cleanup
    event_handler::cleanup ();
    input_manager::cleanup ();
    python::cleanup ();
    // screen::cleanup ();

    return 0;
}
