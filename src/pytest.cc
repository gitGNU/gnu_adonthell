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
    void initbasec (void);
    void initgfxc (void);
    void initinputc (void);
}
extern "C"
int main(int argc, char * argv[])
{    
    python::init();
    initbasec();
    initgfxc();
    initinputc();
    Py_Main (argc, argv);
    python::cleanup();
}
