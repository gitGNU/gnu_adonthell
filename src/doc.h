/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/*! \mainpage
 \section intro Welcome to the Adonthell Programmer's documentation
 Writing a Role Playing Game from scratch is a huge undertaking and requires the
 active help of quite a few people. But the further we progress in coding, the
 harder it will be for new programmers to join in, simply because of the large
 amount of code that has accumulated over the years. This is the point where 
 this manual comes in: its purpose is to show you the way around Adonthell's 
 code base and some of the important underlying concepts.
 
 In case you are not so much interested in programming for Adonthell but rather in
 creating game content you should take a look at the Designer's documentation
 instead.
 
 \section overview Overview
 Apart from the pure API documentation, which contains the interface description
 of the different classes and shows how they interact, there are a few chapters
 about general issues.
 
 The \ref page1 gives an overview about the engine's foundamentals. Programming
 languages, source code organization and such. Before you dive into the code,
 have a look at this for the basics.
 
 Of similar importance -- if you want to lend a hand to the coding -- are the 
 \ref page2 . They include indentation rules, naming conventions and a few 
 other odds and ends that shall help to give the code of different programmers
 a similar shape to make it easier to read.

 Once you understand the basics, you might want to explore specific parts of the
 engine in more detail. This chapter explains how the \ref page3 works, and how
 new events can be defined.
*/

/**
 * @page page1 Introduction for new programmers
 * @section cpp C++
 *
 * Adonthell makes intense use of the features of C++ whenever they make the code
 * clearer and easier and do not slow things down too much. Adonthell tries to
 * respect the concepts of Object Oriented Programming as much as possible. In
 * Adonthell, everything is an %object and inheritance and templates are used 
 * where appropriate. Attributes are usually hidden and may only be accessed
 * through an %object's methods.
 *
 * Further, Adonthell makes heavy use of the Standard Template Library (STL)
 * (http://www.sgi.com/tech/stl/), especially of strings and containers 
 * like lists and hash maps. So you'll certainly want to give it a look.
 *
 * @section python Python 
 * In many kinds of computer games, including RPGs, a script language is necessary to command
 * characters, build complex actions, cutscenes, etc... As we want modularity and
 * reusability, in-game actions must be real-time interpreted. Scripts need to
 * interact with the C++ interface and of course they have to share variables with it.
 * Python (http://www.python.org) has proven to be very efficient at both - 
 * moreover it is an object-oriented language and therefore fits well with C++.
 * And with SWIG (http://www.swig.org), a great tool is available to automate the 
 * process of building the Python interface to our C++ classes.
 * Basically, each class and method described in this document is also available
 * from Python scripts, with only a few exceptions:
 * Python allows no method and operator overloading, so only the first of 
 * overloaded methods or constructors and no operators are accessible from
 * Python.
 *
 * @section scorg Source code organisation 
 * Adonthell makes use of autoconf and automake to be built. In each subdirectory  
 * resides a Makefile.am file that containes the building rules for the files inside
 * that directory as well as its subdirectories. Running "automake" in the root
 * directory creates a Makefile.in from each Makefile.am. "autoconf" builds the 
 * configure script from configure.in. Finally, running "./configure" generates
 * all the Makefiles from the Makefile.ins, making the package ready for 
 * compilation via "make".
 *
 * Here is what the source tree does look like:
 *
 * - doc The user and developer documentation  
 * - src Source code for Adonthell engine - this is where the adonthell executable is built
 *   - tools Various development tools
 *      - dlgedit The dialogue editor (requires GTK+)
 *      - charedit The character editor (requires GTK+)
 *      - questedit The quest editor (requires GTK+)
 *      - maptools The map building tools
 *      - pydonthell A custom Python interpreter with Adonthell Python modules inside
 *      - oggloop Ogg music looping utility
 *
 * Each class that is documented here is usually defined by classname.h and
 * implemented by classname.cc.
 *
 * @section datatypes Data types 
 * Adonthell can run on several platforms, which all have different characteristics.
 * One of these differences can reside in the way the basic C types (char, ints, ...)
 * are encoded. A 32 bit operating system will code it's ints with 32 bits, while a
 * 64 bits operating system will use 64 bits for ints. For several operations (like
 * reading an int from a file) this can result in different behavior, and catastrophic
 * consequences (most likely a protection fault). That's why some of the most basic
 * types have been redifined according to the architecture in types.h:
 * - u_int8: unsigned 8 bit integer
 * - s_int8: signed 8 bit integer
 * - u_int16: unsigned 16 bit integer
 * - s_int16: signed 16 bit integer
 * - u_int32: unsigned 32 bit integer
 * - s_int32: signed 32 bit integer
 *
 * @section gamedyn Game dynamic
 * As we display animated things, we need to know when they have to change. A game that
 * runs at a different speed on various machines has nearly no interest, as only
 * a few configurations can make it run at the right speed. So it's very important
 * to have a timing system built into the game engine.
 * 
 * Adonthell uses it's own timing system. The time unit is the game cycle, which
 * corresponds to approximatively 1/70 of second. When the game runs, it performs
 * a loop which looks like this:
 *
 * @code
 * while(<condition to quit the engine>) 
 * { 
 *     gametime::update();
 *
 *     for(i=0;i<gametime::get_frames_to_do();i++) 
 *     {
 *         <update the game status (character positions,...)> 
 *     }
 *
 *     <perform drawing operations>
 * }
 * @endcode
 *
 * Explanations:
 *
 * This loop performs what is necessary to update the %screen. Depending on the speed
 * of the CPU, this can take more or less time. You've seen that a game cycle durate
 * 1/70 of a second. For some machines, this is not enough to perform the entire loop.
 *
 * As you've seen, there are two kinds of operations that are in the loop:
 *
 * @li Update operations, which actually update the state of the game, according to
 *     user input, previous game state, etc... These operations are very fast to
 *     perform.
 *
 * @li Drawing operations, that is, update the %screen. This is what may slow
 *     things down. Some graphic boards simply can't redraw the entire %screen 70
 *     times per second.
 *
 * So the solution to keep the game running at the same speed on every machine is to
 * draw less frames per second on slow machines (instead of drawing 1 frame every game
 * cycle, we'll draw one frame for 2 games cycles, for example). This is where
 * gametime is usefull: The gametime::update() method calculates the delay between
 * the last call and the current call. It can then calculate if we've been late, and
 * catch the time back by telling to the other %objects that we must perform 2 games
 * cycles instead of 1 to be sync (this is the result of the gametime::get_frames_to_do()
 * method). For example, if the last loop took 1/35 of a second to be completed,
 * gametime::get_frames_to_do() will return 2, so the loop will perform 2 game updates
 * before drawing the %screen. On the contrary, if the machine is too fast (if it can
 * draw 2 frames for each game cycle, for example), it will usleep() to stay in sync.
 *
 * In a more general manner, every class that get's updated and draw something on
 * the %screen MUST have an update() method, that updates it's state once, and a
 * draw() method to draw it on the %screen.
 * 
 */

/** 
 * @page page2 Programming rules
 *
 * @section indent Indentation
 *
 * @section names Methods naming
 * @subsection namgen General naming conventions
 * @subsection namcons Constructor selection for Python
 * As Python can be given only one constructor per class, you have to choose
 * which one will be available.
 * The default constructor often make sense ; but if your class is designed so
 * a constructor without argument have no sense, then you can transgress this
 * rule. In any case, be aware that you must be able to reach the state of ANY
 * of your C++ available constructor from Python by using the available constructor
 * plus one or some of your class methods.
 * To select which constructor is available, embed the others with a ifndef SWIG
 * directive.
 * @code
 * class image
 * {
 * public:
 *     // Default constructor (the one available from Python) 
 *     image ();
 *    
 * #ifndef SWIG
 *     // Creates an image of size (l, h)
 *     image (u_int16 l, u_int16 h); 
 * #endif
 *
 *     // This method make it possible to reach the state of the second constructor
 *     // from Python by doing: im = image (); im.resize (l, h); 
 *     // Moreover, it's also usefull in other cases.
 *     void resize (u_int16 l, u_int16 h);
 *   ...
 * };
 * @endcode
 *
 *
 * @subsection nampy Making overloaded methods and operators available for Python
 * 
 * 
 *
 * @section args Arguments passing conventions
 *
 * @subsection objcts Objects passing
 * Most often you will work with %objects that you or someone else have created.
 * Passing such %objects to methods by value must absolutely be avoided,  for
 * performances and bug issues. Passing a big object by value to a function
 * requires memory to be allocated for the function's object, and of course the
 * copy-constructor and destructor to be called. Needless to say, that without
 * a copy-constructor most complicated %objects won't be correctly passed, and
 * this is a source of useless bug tracking.
 *
 * Instead of passing your %objects by value, you'll pass them by reference.
 * That way, no memory is allocated, and actions are performed directly on your
 * object. To make it obvious to guess which method modify the object you've
 * passed and which ones doesn't, the following conventions has been set up:
 * @li When your function requires an object and doesn't modify it, pass it by
 *     const reference.
 *     @code
 *     void doesntmodify (const myclass & myobject);
 *     @endcode
 *
 * @li When your function requires an object and modify it, pass it by adress.
 *     @code
 *     void modify (myclass * myobject);
 *     @endcode
 *
 * @note Of course, this doesn't apply to your operator overloading functions
 *       which are obviously explicit.
 *
 * And, to make sure nobody will ever pass one of your %objects by value,
 * declare the copy-constructor as private:
 * @code
 * class myclass
 * {
 *
 * ...
 *
 * private:
 *     myclass (myclass & src);
 * };
 * @endcode
 * This will cause a compilation error if someone ever tries to pass an object 
 * of your class by value.
 *
 */

/*! \page page3 Event System
 
 The %event system is divided into three parts. The %event handler
 keeps track of all registered %event scripts. Whenever an
 %event occurs, the %event handler is notified and executes
 all scripts registered for that particular %event. The %event
 list keeps track of the %events registered by a certain %object,
 (e.g. a NPC, a maptile or item) and automatically unregisters
 these %events when this %object is deleted. Finally, there
 are the %events themself, used both as message sent to the
 %event handler whenever an %event occurs and to register an
 %event script. Each %event has its own data structure with
 parameters corresponding to its type. These parameters are
 passed to the %event script, so all infomation regarding
 an %event is available from within the script as well. The
 parameters can further be used to specialize the script so it
 reacts to a smaller range of %events.

 \section event_usage Using the Event System
 
 The %event handler is implemented in the event_handler class.
 It totally consists of static members and methods, to make it
 easily accessible from any part of the code. Just include
 the event.h file. To register a script with the handler that,
 for example, is executed whenever the player arrives at the
 coordinates (20, 20) of a map, you'd write:

 \code
// Create the filter and set it's parameters
event *filter = new enter_event;
 
filter->x = 20;
filter->y = 20;
filter->c = data::the_player;

// Set the script to be executed when the event occurs
filter->set_script ("a_script.py");
 
// Finally add the filter to the event list. This will register it with the event handler
add_event (filter);
 \endcode
 
 For a list of available events with their corresponding parameters
 see the \link event API documentation \endlink .

 Now we have registered an %event with the %event handler. But that alone
 won't get the %event triggered. So depending on its type, you'll have to
 notify the %event handler of an %event's occurance. In case of the \link
 enter_event enter event \endlink , you'll want to send a message to the
 %event handler whenever a character reaches a new position on the map.
         
 \code
// Event structures are also used to pass messages to the event_handler
enter_event message;

// Fill in the parameters
message.x = mapcharacter::posx ();
message.y = mapcharacter::posy ();
message.submap = mapcharacter::submap ();
message.c = this;

// Notify the event handler
event_handler::raise_event (message);
\endcode

The %event handler will then compare all %events of the given type with the
message it recieved and execute the %event script of the matching %events.         

\section event_new Defining new Events        
Now that you know how events principly work, you might want to define your own.
Doing so is quite easy. Take event as the base class, override it's methods with
your own, and you're nearly done.

There is only one problem remaining: loading your %event from a file. The %event
list is taking care of that. But to avoid additional dependencies between the
%event system and your code, the %event list cannot know about your %event at
compile time. Otherwise, each part of the engine using the %event system had to
#include every other part doing so, which we'd like to avoid. Basically, this is
no restriction, since the %event list knows the base class and, thanks to virtual
methods, can handle any derived %event without problem. The only situation where
this fails is when a serialized %event list needs to be loaded from disk again.
To do so, the \link event_list::load loader \endlink needs to be able to
instanciate every possible %event. The way to go is pretty clear now: we need
a function that returns a newly allocated %event structure, and we have to
pass that function to the %event list at runtime, before loading an %event of
that type. Since these steps are the same for each %event, two macros have been
defined:

\code
NEW_EVENT(evt)
REGISTER_EVENT(type,evt)
\endcode

%NEW_EVENT() provides the function that will return a newly allocated %event,
and %REGISTER_EVENT() will pass this function to the %event list. The only
information the %event system needs to know apart from that is the numerical
%event ID, i.e. its type. 
*/
