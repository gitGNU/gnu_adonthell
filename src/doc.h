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


/**
 * @page page1 Introduction for new programmers
 * @section cpp C++
 *
 * Adonthell makes intensive use of C++ advantages, when they can make the code
 * clear and easier and do not slow things down too much. Adonthell tries to
 * respect as much as possible the Object Oriented Programming concepts. In
 * Adonthell, everything are objects, inheritance and templates are used where usefull.
 *
 * Also, Adonthell heavily uses some templates from the STL Library
 * (http://www.sgi.com/tech/stl/) , especially containers like lists and hash maps,
 * so you'll certainly want to give it a look.
 *
 * @section python Python 
 * In many kind of games, including RPGs, a script language is necessary to command
 * characters, build complex actions, cutscenes, etc... As we want modularity and
 * reusability, in-game actions must be real-time interpreted and executed from a
 * script language, which interacts with the C++ interface and can of course share
 * variables with it. Python (http://www.python.org) has proven it was very
 * efficient at this - moreover it's a fully object-oriented language which takes
 * into account things like inheritance, and there is a great utility, SWIG
 * (http://www.swig.org), which can automate the process of building Python include
 * files from C++ ones. Thanks to the great design of Python, everything in this
 * document that applies to the C++ interface also applies to the Python one,
 * making script building much easier. The only difference is that from Python you
 * can only use one constructor - the one that is listed first in the class methods
 * listing. Also, operator redefinition isn't supported.
 *
 * @section scorg Source code organisation 
 * Adonthell make use of autoconf and automake to be built. Each subdirectory has
 * a Makefile.am file that describes building rules. Running "automake" in the root
 * directory build Makefile.in files from Makefile.am in each subdirectory.
 * "autoconf" builds the configure script from configure.in. Finally, running
 * "./configure" generates all the Makefiles from Makefile.ins, making the package
 * ready for a "make".
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
 * Each class that is documented here is generally implemented by classname.h and
 * classname.cpp.
 *
 * @section datatypes Data types 
 * Adonthell can run on several platforms, which all have different characteristics.
 * One of these differences can reside in the way the basic C types (char, ints, ...)
 * are encoded. A 32 bits operating system will code it's ints with 32 bits, while a
 * 64 bits operating system will use 64 bits for ints. For several operations (like
 * reading an int from a file) this can result in different behavior, and catastrophic
 * consequences (most likely protection fault). That's why some of the most basic
 * types have been redifined according to the architecture in types.h:
 * - u_int8: unsigned 8 bits integer
 * - s_int8: signed 8 bits integer
 * - u_int16: unsigned 16 bits integer
 * - s_int16: signed 16 bits integer
 * - u_int32: unsigned 32 bits integer
 * - s_int32: signed 32 bits integer
 *
 * @section gamedyn Game dynamic
 * As we display animated things, we must know when they must change. A game that
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
 * catch the time back by telling to the other objects that we must perform 2 games
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
 * @section indent Indenting
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
 * Most often you will work with objects that you or someone else have created.
 * Passing such objects to methods by value must absolutely be avoided,  for
 * performances and bug issues. Passing a big object by value to a function
 * requires memory to be allocated for the function's object, and of course the
 * copy-constructor and destructor to be called. Needless to say, that without
 * a copy-constructor most complicated objects won't be correctly passed, and
 * this is a source of useless bug tracking.
 *
 * Instead of passing your objects by value, you'll pass them by reference.
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
 * And, to make sure nobody will ever pass one of your objects by value,
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
 * This will cause a compilation error if someone ever try to pass an object of
 * your class by value.
 *
 */
