#include "python_class.h"

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

int main(int argc, char * argv[])
{    
    // add the working directory to python's path (otherwise, scripts
    // without absolute path cannot be executed)
//     string path = dirname (argv[0]);
//     if (path[0] != '/') 
//     {
//         string tmp = getcwd (NULL, 0);
//         path = tmp + path;
//     }

    python::init();
    initbasec();
    initgfxc();
    initinputc();
    Py_Main (argc, argv);
    python::cleanup();
}
