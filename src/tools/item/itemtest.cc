#include <unistd.h>
#include "python_class.h"
#include "inventory_base.h"
#include "character_base.h"

// SWIG init prototypes. Should we use dynamic linking??? 
extern "C"
{
    void inititemtestc (void);
}

item_base * new_item ()
{
    string n,d,t,s;
    u_int32 ab;
    string vstr;
    s_int32 v;
    cout << "New item name: ";
    getline(cin,n);
    cout << "Description: ";
    getline(cin,d);
    cout << "Type (book, weapon): ";
    getline (cin , t);
    if ( t == "book" )
        ab = USE|DROP|PICKUP;
    else if ( t == "weapon" )
        ab = EQUIP|USE|PICKUP|DROP|ENCHANT|COMBINE;
    else {
        t = "book";
        ab = USE|DROP|PICKUP;
    }
    item_base * new_item = new item_base ( n, d, t, ab );
    cout << "Enter (string, value) pairs (empty string to abort): \n";
    while ( 1 ) {
        cout << "String: ";
        getline ( cin, s );
        if ( s == "" )
            break;
        cout << "Value: ";
        getline ( cin, vstr );
        v = atoi ( vstr.c_str () );
        new_item->set_val ( s, v );
    }
    return new_item;
}

inventory_base * new_inventory ()
{
    string n,d;
    cout << "New inventory name: ";
    getline ( cin, n );
    cout << "Description: ";
    getline ( cin, d );
    inventory_base * new_inv = new inventory_base ();
    new_inv->set_name ( n );
    new_inv->set_description ( d );
    return new_inv;
}

// a quick item test
void test ()
{    

    character_base *dwarf = new character_base();
    dwarf->set_name ("Dungar");
    dwarf->set_val ("strength", 19);
    dwarf->set_val ("dexterity", 12);


    string an;
    bool another = true;
    item_base * item;
    while ( another ) {
        cout << "new item\n";
        item = new_item ();
        cout << "adding item to inventory...\n";
        dwarf->get_inventory()->add_item ( item );
        cout << "another item? (y/n) ";
        getline ( cin ,an);
        if ( (an != "y") && (an != "Y") )
            another = false;
    }
    

    cout << "Using (last) item...\n";
    
    item->use();
    

 //   cout << item1->get_name() << endl;
 //   cout << item1->get_description() << endl;

    cout << "saving (last) item...\n";
    item->save ( "test.item" );

    cout << "clearing item...\n";
    item->clear ();
    cout << "Name: " << item->get_name() << endl;
    cout << "Description: " << item->get_description() << endl;
    
    storage::iterator iter = item->begin ();
    while ( iter != item->end () ) {
        cout << iter->first << " = " << iter->second << endl;
        iter++;
    }

    
    cout << "loading item...\n";
    item->load ( "test.item" );

    cout << "loaded item:\n";
    cout << "Name: " << item->get_name() << endl;
    cout << "Description: " << item->get_description() << endl;
    
    /*storage::iterator*/ iter = item->begin ();
    while ( iter != item->end () ) {
        cout << iter->first << " = " << iter->second << endl;
        iter++;
    }
    
    cout << "iterate over items\n";

    inventory_base *inv=dwarf->get_inventory();
    for ( vector<item_base *>::iterator iter = inv->begin() ; iter != inv->end(); iter++ )
        cout << (*iter)->get_name() << endl;
    
    
    cout << "Saving inventory: \n";
    inv->save ( "test.inventory" );
    
    cout << "clearing inventory...\n";
    inv->clear ();
    cout << "iterate over items\n";

    /*inventory_base * */inv=dwarf->get_inventory();
    for ( vector<item_base *>::iterator iter = inv->begin() ; iter != inv->end(); iter++ )
        cout << (*iter)->get_name() << endl;

    cout << "Loading inventory...\n";
    inv->load("test.inventory");
    cout << "iterate over items\n";

    /*inventory_base * */inv=dwarf->get_inventory();
    for ( vector<item_base *>::iterator iter = inv->begin() ; iter != inv->end(); iter++ )
        cout << (*iter)->get_name() << endl;


    //dwarf.set_inventory(inv);
    
    
    //item_base *axe= new item_base ("Axe", "Dungar's Axe", "weapon", EQUIP|USE|PICKUP|DROP|ENCHANT|COMBINE, &dwarf);
    //axe->set_val ("strength", 5);
    //axe->set_val ("dexterity", -5);
    //axe->set_val ("type", WEAPON);
    
    //inv->add_item(axe);
    //inv->add_item(book);
//    book.equip (&dwarf);
    //axe->equip (&dwarf);
    //axe->use ();
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
