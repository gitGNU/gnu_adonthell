/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <string.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_border.h"
#include "win_font.h"
#include "win_base.h"
#include "win_select.h"
#include "win_container.h"
#include "win_label.h"


win_label::win_label (s_int16 x, s_int16 y, u_int16 l, u_int16 h, win_font * fo, win_container * tmpwc):win_base (x, y, l, h, tmpwc, tmpwc->get_drawing_area ())
{
    font = fo;
    auto_height = false;
    auto_size = false;
    cursor = NULL;
    x_pad_l = 0;
    size_texte = 0;
    cur_text_pos = 0;
    tmp_image = new image ();
    tmp_draw = new image ();
    set_text("");
}

win_label::~win_label ()
{
    if (cursor)
        delete cursor;
    delete tmp_image;
    delete tmp_draw;

    dettach_select ();
}

void
win_label::set_font (win_font * tmp)
{
    font = tmp;
    if (font)
        init_draw ();
}

void
win_label::set_text (const char *tmp)
{
    if (font)
    {
        strcpy (texte, tmp);
        size_texte = strlen (texte);
        init_draw ();
    }
    else
    {
        cout << "You must set font\n";
        exit (1);
    }
}

bool
win_label::add_text (const char *tmp)
{
    if (strlen (tmp) + strlen (texte) < WIN_TEXT_MAX_LENGTH)
    {
      strcat (texte, tmp);
      set_text (texte);
        return true;
    }
    else
        return false;
}

char *
win_label::get_text ()
{
    if (size_texte == 0)
        return NULL;
    else
        return texte;
}

void
win_label::clear_text ()
{
    set_text ("");
}

void
win_label::set_auto_size (bool tmp)
{
    auto_size = tmp;
    if (auto_size)
        auto_height = false;
    init_draw ();
    if (wc)
        wc->find_obj_max_y ();  //for scrollbar
}

void
win_label::set_auto_height (bool tmp)
{
    auto_height = tmp;
    if (auto_height)
        auto_size = false;
    init_draw ();
    if (wc)
        wc->find_obj_max_y ();  //for scrollbar
}

void
win_label::set_cursor (win_cursor * tmp)
{
    if (tmp)
    {
        if (cursor)
            delete cursor;

        cursor = new image ();
        *cursor = *(tmp->cursor);
    }
}

void
win_label::init_draw ()
{
    u_int16 i = 0;

    if (auto_size)
    {
        u_int16 max_length = 0;
        u_int16 max_height = 0;
        u_int16 tmp_length = 0 + x_pad_l;

        while (i < size_texte && texte[i] != '\0')
        {
            if (texte[i] == '\n')
            {
                max_height += font->height;
                if (max_length < tmp_length)
                    max_length = tmp_length;
                tmp_length = 0 + x_pad_l;
            }
            else
            {
                if (texte[i] == ' ')
                    tmp_length += font->length;
                else if (font->in_table (texte[i]))
                    tmp_length += font->table[texte[i]].length;
            }
            i++;
        }
        if (max_length < tmp_length)
            max_length = tmp_length;
        win_base::resize (max_length, max_height + font->height);
    }
    else
    {
        if (auto_height)
        {
            u_int16 use_space = 0 + x_pad_l; //space used for the line
            u_int16 word_space = 0; //size of the word
            u_int16 begin_word = 0; //position of the word in the string
            u_int16 tmp_pos, tmp_pos2, tmp_y = font->height;

            i = 0;
            while (i < size_texte && texte[i] != '\0')
            {
                begin_word = i;
                tmp_pos = begin_word;
                word_space = 0; //size of the word start at 0
                while (texte[tmp_pos] != '\0' && texte[tmp_pos] != '\n' && texte[tmp_pos] != ' ')
                {
                    if (font->in_table (texte[tmp_pos]))
                        word_space += font->table[texte[tmp_pos]].length; //calcul word size
                    tmp_pos++;
                }
                //now tmp_pos is the index of end of the word in the string
                if (word_space > length - use_space) //if size of word > of the free space on line
                {
                    if (word_space > length) //if size of word > at length of the label 
                    {
                        tmp_pos2 = begin_word;
                        while (tmp_pos2 < tmp_pos)
                        {
                            if (font->in_table (texte[tmp_pos2]))
                                if (use_space + font->table[texte[tmp_pos2]].length < length)
                                    use_space += font->table[texte[tmp_pos2]].length;
                                else
                                {
                                    tmp_y += font->height; //modify
                                    use_space = x_pad_l + font->table[texte[tmp_pos2]].length;
                                }
                            tmp_pos2++;
                        }
                    }
                    else
                    {
                        //if word_space < length && word_space > length-use_space
                        tmp_pos2 = begin_word;
                        use_space = x_pad_l + 0; //add the pad
                        tmp_y += font->height;
                        while (tmp_pos2 < tmp_pos)
                        {
                            if (font->in_table (texte[tmp_pos2]))
                                use_space += font->table[texte[tmp_pos2]].length;
                            tmp_pos2++;
                        }
                    }
                }
                else
                {
                    tmp_pos2 = begin_word;
                    while (tmp_pos2 < tmp_pos)
                    {
                        if (font->in_table (texte[tmp_pos2]))
                            use_space += font->table[texte[tmp_pos2]].length;
                        tmp_pos2++;
                    }
                }
                i = tmp_pos;
                if (texte[i] == ' ')
                    use_space += font->length;
                else if (texte[i] == '\n')
                {
                    use_space = 0 + x_pad_l; //add pad
                    tmp_y += font->height;
                }
                i++;
            }                   //fin while
            //set height to tmpy
            win_base::resize (length, tmp_y);
        }
    }
  tmp_draw->resize(length,height);  
  draw_text(cur_text_pos);
}

void win_label::draw_text(u_int16 i)
{
  u_int16 tmp_y=0;
  u_int16 use_space=0+x_pad_l;
  u_int16 word_space=0;
  u_int16 begin_word=0;
  u_int16 tmp_pos,tmp_pos2;
 
  for(u_int16 k=0;k<height;k++)
    for(u_int16 j=0;j<length;j++)
      tmp_draw->put_pix(j,k,screen::trans_pix);
  
  while(i<size_texte && texte[i]!='\0' && tmp_y<height)
    {
        begin_word = i;
        tmp_pos = begin_word;
        word_space = 0;
        while (texte[tmp_pos] != '\0' && texte[tmp_pos] != '\n' && texte[tmp_pos] != ' ')
        {

            if (font->in_table (texte[tmp_pos]))
                word_space += font->table[texte[tmp_pos]].length; //calcul word size
            tmp_pos++;
        }

        if (word_space > length - use_space)
        {
            if (word_space > length)
            {
                tmp_pos2 = begin_word;
                while (tmp_pos2 < tmp_pos)
                {
                    if (font->in_table (texte[tmp_pos2]))
                        if (use_space + font->table[texte[tmp_pos2]].length < length)
                        {
                            tmp_draw->putbox_img (&(font->table[texte[tmp_pos2]]), use_space, tmp_y);
                            use_space += font->table[texte[tmp_pos2++]].length;
                        }
                        else
                        {
                            use_space = 0 + x_pad_l;
                            tmp_y += font->height;
                            tmp_draw->putbox_img (&(font->table[texte[tmp_pos2]]), use_space, tmp_y);
                            use_space += font->table[texte[tmp_pos2++]].length;
                        }
                    else
                        tmp_pos2++;
                }
            }
            else
            {
                //if word_space < length && word_space > length-use_space
                tmp_pos2 = begin_word;
                use_space = 0 + x_pad_l;
                tmp_y += font->height;
                while (tmp_pos2 < tmp_pos && tmp_y < height)
                {

                    if (font->in_table (texte[tmp_pos2]))
                    {
                        tmp_draw->putbox_img (&(font->table[texte[tmp_pos2]]), use_space, tmp_y);
                        use_space += font->table[texte[tmp_pos2++]].length;
                    }
                    else
                        tmp_pos2++;
                }
            }
        }
        else
        {
            tmp_pos2 = begin_word;
            while (tmp_pos2 < tmp_pos && tmp_y < height)
            {
                if (font->in_table (texte[tmp_pos2]))
                {
                    tmp_draw->putbox_img (&(font->table[texte[tmp_pos2]]), use_space, tmp_y);
                    use_space += font->table[texte[tmp_pos2]].length;
                }
                tmp_pos2++;
            }
        }
        i = tmp_pos;
        if (texte[i] == ' ')
            use_space += font->length;
        else if (texte[i] == '\n')
        {
            use_space = 0 + x_pad_l;
            tmp_y += font->height;
        }
        i++;
    }
    last_text_pos = i;
}


void
win_label::resize (u_int16 l, u_int16 h)
{
    win_base::resize (l, h);
    if (font)
        init_draw ();
    else
    {
        cout << "You must set font\n";
        exit (1);
    }
}


void
win_label::draw ()
{
    if (visible && font)
    {
        draw_background ();
        if (wselect && selected)
        {
            if (select_mode == WIN_SELECT_MODE_BRIGHTNESS)
            {
                tmp_image->brightness (tmp_draw, 180);
                tmp_image->putbox_mask (real_x, real_y, da);
            }
            else
            {
                if (select_mode == WIN_SELECT_MODE_CURSOR)
                {
                    draw_cursor ();
                    tmp_draw->putbox_mask (real_x, real_y, da);
                }
                else
                {
                    if (select_mode == WIN_SELECT_MODE_BORDER)
                        tmp_draw->putbox_mask (real_x, real_y, da);
                }
            }
        }
        else
        {

            tmp_draw->putbox_mask (real_x, real_y, da);

        }
        draw_border ();
    }
}

void
win_label::draw_cursor ()
{
    if (cursor)
        cursor->putbox_mask (real_x, real_y, da);
}

void
win_label::update ()
{

}


void
win_label::attach_select (win_select * tmp)
{
    wselect = tmp;
}

void
win_label::dettach_select ()
{
    if (wselect)
    {
        wselect->remove (this);
        wselect = NULL;
        x_pad_l = 0;
    }
}


void
win_label::set_select_mode (u_int8 tmp)
{
    win_base::set_select_mode (tmp);
    if (select_mode == WIN_SELECT_MODE_CURSOR)
        if (wselect->cursor)
            x_pad_l = wselect->cursor->cursor->length;
    init_draw ();
}
