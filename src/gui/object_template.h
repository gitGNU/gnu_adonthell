#ifndef OBJECT_TEMPLATE_H_
#define OBJECT_TEMPLATE_H_

#include<string>
#include "fileops.h"

namespace gui
{ 
  class object_template
    {
    public:
      
      /**
       * read a stream to load image
       */
      virtual void load (igzstream & is) = 0;  
      
      
      /**
       * write image into the stream
       */
      virtual void save (ogzstream & os) = 0; 
      
      /**
       * get name of the border
       * @return name of the border
       */
      std::string get_name () const;
      
      /**
       * set name of the border
       * @param name :  name of the border
       */
      void set_name (const std::string & name); 
		
		
      /**
       * Build is automaticly call when you load a border
       */
      virtual void build() = 0;


      /**
       * write in standart ouput border information 
       */
      virtual  void display_info ()= 0;
      
      
      virtual ~object_template () {};

    protected:
      
      /* name of the template */
      std::string my_name;

    };		
}
#endif
