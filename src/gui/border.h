/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_BORDER_H_
#define GUI_BORDER_H_

#include <string.h>
#include "types.h"
#include "../gfx/image.h"
#include "../gfx/drawing_area.h"


namespace gui {
  
  class theme;
  class base;
  
#define NB_BORDER_IMAGE 6
  
  /**
   * This class provide border for each objet
   */
  class border
    {
    public:
      
      /**
       * Default constructor
       */
      border();
      
      /**
       * Constructor
       * @param base objet
       */
      border(base * wb);
  
      /**
       * Constructor
       * build a border with the param instance
       */
      border(border &);
      
      /**
       * Constructor
       * Construct a border with a dir location and a size for the border
       */
      border(char *rep ,char *size=BORDER_NORMAL_SIZE);
  
      /**
       * Destructor
       */
      ~border();
  
      /**
       * load a border file 
       * @param directory (theme)
       * @param size name
       */
      void load(char *,char *);
  
      /**
       * update the border
       */
      void update();
  
      /**
       * call when destroy
       */
      void destroy();
      
      /**
       * call to draw the data
       */
      void draw(gfx::drawing_area * da);
  
      /**
       * Define if border are visible
       */
      void set_visible_border(bool b){visible_border_=b;}
      
      /**
       * Define is border used brightness
       */
      void set_brightness_border(bool b);
  
      /**
       * Define border used for this object.
       */
      void set_border(border & );
      
      /**
       * define theme used by this border
       */
      void set_border(theme & wth, u_int8 size = border::NORMAL);
      
      /**
       * Define if this border is transluancy
       */
      void set_trans_border(bool b);

      /**
       * Get the border length
       */
      u_int16 length_border();
      
      /**
       * get the border height
       */
      u_int16 height_border();
      
#ifdef SWIG
#define border_MINI 0
#define border_NORMAL 1
#else
      const static u_int8 MINI = 0;
      const static u_int8 NORMAL = 1;
#endif
    private:
      
      /**
       * Init the border
       */
      void init();
      
      /**
       * Refresh, update the size of the border
       */
      void refresh();
      
      border & operator=(border &);
      
      /* horizontal border template */
      gfx::image * h_border_template_;
      
      /* vertical border template */
      gfx::image * v_border_template_;
      
      /* border */
      gfx::image * border_[NB_BORDER_IMAGE];
      
      gfx::image * border_brightness_[NB_BORDER_IMAGE];
      
      /* border used to draw */
      gfx::image ** border_draw_;
      
      bool visible_border_;
      
      bool brightness_;
      
      u_int8 trans_;
      
      /* pointer to base */
      base * wb_;
    };
}
#endif


