#include <unistd.h>
#include "python_class.h"
#include "item_base.h"

// SWIG init prototypes. Should we use dynamic linking??? 
extern "C"
{
    void inititemtestc (void);
}

// a quick item test
void test ()
{    
    item_base book ("The Yeti", "The yeti is a rough and noble beast", "book", USE); 
    book.set_val ("type", BOOK);
    book.use ();
    
    character_base dwarf;
    dwarf.set_name ("Dungar");
    dwarf.set_val ("strength", 19);
    dwarf.set_val ("dexterity", 12);
     
    item_base axe ("Axe", "Dungar's Axe", "weapon", EQUIP | USE);
    axe.set_val ("strength", 5);
    axe.set_val ("dexterity", -5);
    axe.set_val ("type", WEAPON);
    axe.equip (&dwarf);
    axe.use ();
}

int main (int argc, char *argv[])
{
    python::init ();
    
    // Shared modules path 
    python::insert_path (DATA_DIR"/modules"); 

    char *path = getcwd (NULL, 0);
    python::insert_path (path);
    free (path);

    /* Initialise SWIG module. This should go if we ever switch to dynamic 
       link */
    inititemtestc ();
        
    PyObject *py_module = python::import_module ("itemtest"); 
    if (!py_module) return false;     
    
    data::globals = PyModule_GetDict (py_module);
    
    // testing
    test ();
    
    python::cleanup ();
    return 0;
}
