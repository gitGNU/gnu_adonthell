#include "python/base.h"

extern "C" {
extern int Py_Main(int, char **);
}

extern "C"
{
    /** 
     * SWIG init prototype.
     * 
     */
    void init_base (void);
    void init_gfx (void);
    void init_input (void);
}
extern "C"
int main(int argc, char * argv[])
{    
    python::init();
    init_base();
    init_gfx();
    init_input();

    python::insert_path(".");
    data::globals = PyModule_GetDict (python::import_module("input"));

    Py_Main (argc, argv);
    python::cleanup();
}
