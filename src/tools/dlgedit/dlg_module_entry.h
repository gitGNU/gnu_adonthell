/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_module_entry.h
 *
 * @author Kai Sterker
 * @brief The textual contents of a DlgModule.
 */

#ifndef DLG_MODULE_ENTRY_H
#define DLG_MODULE_ENTRY_H

#include <string>

/**
 * The %DlgModuleeEntry keeps the custom Python code of a certain
 * dialogue and a few other settings.
 */
class DlgModuleEntry
{
public:
    DlgModuleEntry ();
    
    /**
     * Attribute access.
     */
    //@{
    /**
     * Get the import statements assigned to this module.
     * @return the module's additional imports.
     */
    std::string imports ()          { return imports_; }
    /**
     * Get the constructor code assigned to this module.
     * @return the module's constructor code.
     */
    std::string ctor ()              { return ctor_; }
    /**
     * Get the destructor code assigned to this module.
     * @return the module's destructor code.
     */
    std::string dtor ()             { return dtor_; }
    /**
     * Get the user defined methods of this module.
     * @return the module's user defined methods.
     */
    std::string methods ()          { return methods_; }
    
    void setImports (std::string i) { imports_ = i; }
    void setCtor (std::string c)    { ctor_ = c; }
    void setDtor (std::string d)    { dtor_ = d; }
    void setMethods (std::string m) { methods_ = m; }
    //@}
        
private:
    std::string imports_;           // additional import statements
    std::string ctor_;              // constructor code
    std::string dtor_;              // destructor code
    std::string methods_;           // user defined methods
};

#endif // DLG_MODULE_ENTRY_H
