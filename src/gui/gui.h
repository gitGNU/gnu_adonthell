/*
   
   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#ifndef GUI_H_
#define GUI_H_

#include "base.h"
#include "background.h"
#include "border.h"
#include "container.h"
#include "event.h"
#include "font.h"
#include "keys.h"
#include "label_input.h"
#include "manager.h"
#include "object.h"
#include "scrollbar.h"
#include "scroll.h"
#include "select_list.h"
#include "theme.h"
#include "types.h"
#include "label.h"
#include "ttf.h"

namespace gui {

//define a label object
typedef object<label> label_obj;

//define a write object
typedef object <label_input> label_input_obj;

}

#endif
