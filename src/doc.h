/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Copyright (C) 2001   Kai Sterker
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   doc.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Extra documentation.
 * 
 * 
 */



/*! 
 \mainpage
 \section intro Welcome to the Adonthell Programmer's documentation
 Writing a Role Playing Game from scratch is a huge undertaking and requires the
 active help of quite a few people. But the further we progress in coding, the
 harder it will be for new programmers to join in, simply because of the large
 amount of code that has accumulated over the years. This is the point where 
 this manual comes in: its purpose is to show you the way around Adonthell's 
 code base and some of the important underlying concepts.
 
 In case you are not so much interested in programming for Adonthell but rather in
 creating %game content you should take a look at the Designer's documentation
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
 engine in more detail. As many parts of the %game are divided into several classes,
 these chapters are there to give a first overview how they work and interact.
 	- \ref page3
 	- \ref page4
 	- \ref page5
*/

/*!
 \page page1 Introduction for new programmers
 \section cpp C++

 Adonthell makes intense use of the features of C++ whenever they make the code
 clearer and easier and do not slow things down too much. Adonthell tries to
 respect the concepts of Object Oriented Programming as much as possible. In
 Adonthell, everything is an %object and inheritance and templates are used 
 where appropriate. Attributes are usually hidden and may only be accessed
 through an %object's methods.

 Further, Adonthell makes heavy use of the Standard Template Library (STL)
 (http://www.sgi.com/tech/stl/), especially of strings and containers 
 like lists and hash maps. So you'll certainly want to give it a look.

 \section python Python 
 In many kinds of computer games, including RPGs, a script language is necessary to command
 characters, build complex actions, cutscenes, etc... As we want modularity and
 reusability, in-%game actions must be real-time interpreted. Scripts need to
 interact with the C++ interface and of course they have to share variables with it.
 Python (http://www.python.org) has proven to be very efficient at both - 
 moreover it is an object-oriented language and therefore fits well with C++.
 And with SWIG (http://www.swig.org), a great tool is available to automate the 
 process of building the Python interface to our C++ classes.
 Basically, each class and method described in this document is also available
 from Python scripts, with only a few exceptions:
 Python allows no method and operator overloading, so only the first of 
 overloaded methods or constructors and no operators are accessible from
 Python.

 \section scorg Source code organisation 
 Adonthell makes use of autoconf and automake to be built. In each subdirectory  
 resides a Makefile.am file that containes the building rules for the files inside
 that directory as well as its subdirectories. Running "automake" in the root
 directory creates a Makefile.in from each Makefile.am. "autoconf" builds the 
 configure script from configure.in. Finally, running "./configure" generates
 all the Makefiles from the Makefile.ins, making the package ready for 
 compilation via "make".

 Here is what the source tree does look like:

 - doc The user and developer documentation  
 - src Source code for Adonthell engine - this is where the adonthell executable is built
   - tools Various development tools
      - dlgedit The dialogue editor (requires GTK+)
      - charedit The character editor (requires GTK+)
      - questedit The quest editor (requires GTK+)
      - maptools The map building tools
      - pydonthell A custom Python interpreter with Adonthell Python modules inside
      - oggloop Ogg music looping utility

 Each class that is documented here is usually defined by classname.h and
 implemented by classname.cc.

 \section datatypes Data types 
 Adonthell can run on several platforms, which all have different characteristics.
 One of these differences can reside in the way the basic C types (char, ints, ...)
 are encoded. A 32 bit operating system will code it's ints with 32 bits, while a
 64 bits operating system will use 64 bits for ints. For several operations (like
 reading an int from a file) this can result in different behavior, and catastrophic
 consequences (most likely a protection fault). That's why some of the most basic
 types have been redifined according to the architecture in types.h:
 - u_int8: unsigned 8 bit integer
 - s_int8: signed 8 bit integer
 - u_int16: unsigned 16 bit integer
 - s_int16: signed 16 bit integer
 - u_int32: unsigned 32 bit integer
 - s_int32: signed 32 bit integer

 \section gamedyn Game dynamic
 As we display animated things, we need to know when they have to change. A %game that
 runs at a different speed on various machines has nearly no interest, as only
 a few configurations can make it run at the right speed. So it's very important
 to have a timing system built into the %game engine.
 
 Adonthell uses it's own timing system. The time unit is the %game cycle, which
 corresponds to approximatively 1/70 of second. When the %game runs, it performs
 a loop which looks like this:

 \code
 while(<condition to quit the engine>) 
 { 
     gametime::update();

     for(i=0;i<gametime::get_frames_to_do();i++) 
     {
         <update the %game status (%character positions, etc...)> 
     }

     <perform drawing operations>
 }
 \endcode

 Explanations:

 This loop performs what is necessary to update the %screen. Depending on the speed
 of the CPU, this can take more or less time. You've seen that a %game cycle durate
 1/70 of a second. For some machines, this is not enough to perform the entire loop.

 As you've seen, there are two kinds of operations that are in the loop:

 \li Update operations, which actually update the state of the %game, according to
     user %input, previous %game state, etc... These operations are very fast to
     perform.

 \li Drawing operations, that is, update the %screen. This is what may slow
     things down. Some graphic boards simply can't redraw the entire %screen 70
     times per second. Moreover, even with high-end boards, hardware acceleration may
     not be used depending on the SDL target used. x11 is know to be unable to use 
     hardware acceleration, while fbcon does, when possible.

 So the solution to keep the %game running at the same speed on every machine is to
 draw less frames per second on slow machines (instead of drawing 1 frame every %game
 cycle, we'll draw one frame for 2 %games cycles, for example). This is where
 gametime is usefull: The gametime::update() method calculates the delay between
 the last call and the current call. It can then calculate if we've been late, and
 catch the time back by telling to the other %objects that we must perform 2 %games
 cycles instead of 1 to be sync (this is the result of the gametime::get_frames_to_do()
 method). For example, if the last loop took 1/35 of a second to be completed,
 gametime::get_frames_to_do() will return 2, so the loop will perform 2 %game updates
 before drawing the %screen. On the contrary, if the machine is too fast (if it can
 draw 2 frames for each %game cycle, for example), it will usleep() to stay in sync.

 In a more general manner, every class that get's updated and draw something on
 the %screen MUST have an update() method, that updates it's state once, and a
 draw() method to draw it on the %screen.
 
 */

/*! 
 \page page2 Programming rules

 \section indent Indentation
 No matter who wrote a file, it must be readable by anybody. The beginning of 
 readability is a correct indentation. Here are a few rules to follow:

 \li Block indentation is set to 4 characters.
 \li A space should be placed before/after every operator and separator.
 \li Air your code. Skip lines between blocks, and don't hesitate to 
     comment when it is useful.
 
 Here is a sample code: 

 \verbatim
 int main (int argc, char * argv [])
 {
     int i;

     for (i = 0; i < 10; i++)
     {
         int ret = myfunc (0, 1, 2);
         if (ret <= 10) return ret;
     }

     return 0;
 }
 \endverbatim

 \section doxydoc Documentation System
 The document you are currently reading is generated by Doxygen from the source
 code. Doxygen is a very powerful documentation system that allows programmers
 to quickly document their code with class references, diagrams (inheritance, class
 hierarchy, ...) and many other useful things with a very small effort, letting
 them concentrate on the code rather than the documentation. The programmer documentation
 can easily be generated provided the source code (or rather the header files) are
 correctly commented. For more informations, have a look at Doxygen's own documentation
 which is available at http://www.doxygen.org.

 Your classes must all be clearly documented, to allow other developers to use them,
 and yourself to remember how they work. You'll have to document at file level, class
 level and member level.
 
 \subsection filelev Documenting at file level
 EVERY file of Adonthell HAS TO start with the following:
 \verbatim
 /*
   $Id:

   Copyright (C) <year>   <your name>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */


/**
 * @file   <filename>
 * @author <yourname> <<your_email@adress>>
 * 
 * @brief  <Briefly describe what's in this file>
 * 
 * 
 */
 \endverbatim

 The first block is for Copyright issues. You clearly show that this file is 
 distributed under the terms of the GPL ; that it comes with no warranty and
 that you are the copyright holder of the file. The Copyright line has to show
 all the years this file has been worked on. Several persons may hold
 the copyright of a file. In this case, put one Copyright line per person.
 The $Id: line is for the CVS system. It will put useful information on this
 line when the file will be committed, indicating who last modified this file,
 when, and which version number it is.

 The second block is for referencing this file into Doxygen's documentation 
 system. It's quite explicit and MANDATORY if you want to document the classes
 in this file.

 \subsection classlev Documenting at class level
 Each class must have a little documentation block explaining what it does.
 Moreover, you can't document a class member if the class itself isn't 
 documented. Just put a Doxygen block before your class and explain what it is
 for:

 \verbatim
 /**
  * This very powerful class blah blah....
  * blah blah blah blah blah
  * blah blah blah blah.
  *
  * @bug Uh, I still have to write the members.
  *
  */
 class my_very_cool_class
 {
     .....
 }
 \endverbatim

 Don't hesitate to use Doxygen's special tags, like \\note or \\bug. 

 \subsection memblev Documenting at member level
 Once your class is briefly described, you can start the "true" documenting, that
 is, clearly and precisely describe your public interface. Once again, everything
 is better explained in Doxygen's own documentation, but here is an example for
 a member function:
 
 \verbatim
 class my_class
 {
 public:
     /**
      * Returns the square root of the parameter.
      * @param a_number Number to calculate the square root of.
      * @return Square root of a_number.
      */
     float sqrt (float a_number); 
 }

 \endverbatim

 Once you have done this, and rebuild the documentation, your class will appear in
 the Class Hierarchy diagram, with it's own documentation page and automatically
 generated Inheritance Diagram. Have a look at the \link image Image Class Page 
 \endlink for a sample of the result you can expect, and \link image.h the image.h
 file \endlink (see the source code) to see how it has been done.

 \section names Method naming
 \subsection namgen General naming conventions
 There are several different more or less popular ways to name your functions and
 classes. Depending on what you like, you can call the same function 
 perform_something () or PerformSomething (), etc... To keep the interface as clear
 and homogeneous as possible, here are a few rules to follow when naming your classes
 and functions:

 \li Use lower cases, and '_' as a separator if your function name has several words
 (ex: perform_something ()).

 \li Member access functions should be as short as possible. For read-access, use the
 most direct name possible (i.e. length () for the length of an object), for write
 access, use set_member style names (set_length (u_int16 l)). Of course, the member
 itself should then have a different name than length. Placing an underscore right
 after (length_) for naming your member is widely used through the code - but you
 are free to do something else if you don't like it.
 
 \li Methods returning something more complicated than a simple %data type (i.e. 
 functions that returns a pointer to a complex %data structure, etc...) should 
 use a get_name style instead. For example, a method returning an %image of an
 %animation should be named get_image ().

 \li If your class is static, the "manual" constructor/destructor should then be named
 respectively init () and cleanup ().

 Let's see a concrete example of this naming convention through a class interface:
 \verbatim
   class animation
   {
   public:
       // Constructor
       animation (); 
       
       // Destructor
       ~animation ();

       u_int16 length ()
       {
           return length_;
       }

       u_int16 height ()
       {
           return height_;
       }

       image * get_image (u_int16 pos)
       {
           return frame[pos];
       }

       .....

   private:
       u_int16 length_;
       u_int16 height_;
       vector <image> frame;
   }
 \endverbatim

 \subsection namcons Constructor selection for Python
 As Python can only have one constructor per class, you have to choose
 which one will be available.
 The default constructor often makes sense ; but if your class requires a
 constructor with arguments, then you can transgress this rule of course.
 In any case, be aware that you need to be able to reach the state of ANY
 of your C++ constructors from Python by using the available constructor
 plus one or more of your class methods.
 To select which constructor is available, embed the others with a ifndef SWIG
 directive.
 \verbatim
 class image
 {
 public:
     // Default constructor (the one available from Python) 
     image ();
    
 #ifndef SWIG
     // Creates an image of size (l, h)
     image (u_int16 l, u_int16 h); 
 #endif

     // This method makes it possible to reach the state of the second constructor
     // from Python by doing: im = image (); im.resize (l, h); 
     // Moreover, it's also useful in other cases.
     void resize (u_int16 l, u_int16 h);
   ...
 };
 \endverbatim


 \subsection nampy Making overloaded methods and operators available for Python
 SWIG doesn't like overloaded functions and operators, and will print a warning
 if it meets one. But the functions or operators you have overloaded have to be
 available from the Python interface. To make them available, you should us a
 ifdef SWIG directive to declare another inlined function that matches the overloaded
 function or operator. An example is better than a long explanation:
 \verbatim
 class myclass
 {
 public:

     ......
    /**
     * Drawing method 1
     */ 
     void draw (int x, int y);
#ifndef SWIG
    /**
     * Drawing method 2
     * @attention Not available from Python. Use draw_part () instead.
     * @sa draw_part ()
     */ 
     void draw (int x, int y, int l, int h);
#endif
    /**
     * Synonym of draw (x, y, l, h) to guarantee it's availability from Python.
     */ 
     void draw_part (int x, int y, int l, int h)
     {
         draw (x, y, l, h);
     }

    /**
     * Copy operator (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () instead.
     * @sa copy ()
     */
     myclass& operator = (const myclass & src);

     /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
     void copy (const myclass& src) 
     {     
         *this = src; 
     }    
     ...

 }
 \endverbatim
 
 Don't forget to comment your methods accordingly to their access.

 Functions synonym to Operators should have explicit names. As an operator
 could have several meanings (+ could be said "add" or "concat", for 
 example) you are free to choose whatever name fits best with your usage.

 \section args Argument passing conventions

 \subsection objcts Object passing
 Most often you will work with %objects created by yourself or someone else.
 Passing such %objects to methods by value has to be absolutely avoided, for
 performances and bug issues. Passing a big object by value to a function
 requires memory to be allocated for the function's object, and of course the
 copy-constructor and destructor to be called. Needless to say, that without
 a copy-constructor most complicated %objects won't be correctly passed, and
 this is a source of useless bug tracking.

 Instead of passing your %objects by value, you'll pass them by reference.
 That way, no memory is allocated, and actions are performed directly on your
 object. To make it obvious which methods modify the object you're passing
 and which don't, the following conventions has been set up:
 \li When a function requires an object and doesn't modify it, pass it by
     const reference.
     \code
     void doesntmodify (const myclass & myobject);
     \endcode

 \li When a function requires an object and modifies it, pass it by address.
     \code
     void modify (myclass * myobject);
     \endcode

 \note Of course, this doesn't apply to your operator overloading functions
       which are obviously explicit.

 And, to make sure nobody will ever pass one of your %objects by value,
 declare the copy-constructor as private:
 \code
 class myclass
 {

 ...

 private:
     myclass (myclass & src);
 };
 \endcode
 This will cause a compilation error if someone ever tries to pass an object 
 of your class by value.

 */

/*! \page page3 The Event System
 
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

/*! \page page4 The Map Engine

The map engine consists of several parts that work together to display the
%game world on %screen.

\section map_data Map Data

The landmap holds the %data of the map: the actual \link mapobject graphics
\endlink used on that map, and the information how to place them. A single map
can contain multiple, independent \link mapsquare_area terrains \endlink
that share the same graphics though. All terrains (or submaps) of a map are
kept in memory, so transition between them is fast. Therefore it is desirable
to keep related areas, like different levels of a dungeon or the floors of a
building, on the same map.

\section map_rendering The Renderer

The %data on its own is only of little use as long as there is no way of displaying
it. This is the \link mapview mapview's \endlink task. It renders a certain area
of the map and everything on it onto a surface. That way, it can be as easily
displayed on %screen as it can be saved to disk.

The %mapview itself is rather dumb and without further instructions it would
render the same part of the map for the rest of eternity. A \link py_script python
script \endlink can be used to achieve a dynamic behaviour of the mapview, like
following the player around the map. This script is called once every cycle of the
%game and has access to practically all API functions and in-%game data, to permit
as much flexibility as possible.
*/

/*! \page page5 The Dialogue Engine

The dialogue engine is handling conversations between the player and NPCs.
Dialogues themselves are %python scripts that are executed step by step by
the lowlevel dialog class.

Each step results in a number of strings, the speech of the NPC and an optional
list of the player's possible responses. These are displayed in the \link
dialog_engine Dialogue Window \endlink . Once the player has chosen an answer
the next step of the script is executed.

The script itself returns three lists. Possible NPC speeches, the player's possible
responses for each of those speaches and finally a list with the successor of each
choice. The dialogue engine randomly picks one of the NPC speeches and the according
answers. It also makes sure that each piece of dialogue is only used once during the
conversation. More complex operations, such as depending a piece of dialogue on a
certain condition are already handled in the %python script.

For more information you should refer to the Dialogue Editor documentation.
*/
