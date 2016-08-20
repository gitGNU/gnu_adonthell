/*
   (C) Copyright 2000/2001/2004 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "label.h"

u_int16 label::cursor_blink_cycle = 75; 

#define LINE_LENGTH(line) (line.pos_x + line.offset_x/screen::scale())

/**
   Constructor
*/
label::label () : image ()
{
    // no font at the beginning
    my_font_ = NULL;
    new_text_ = "";
    
    // init the cursor and the text vector
    init_vec_cursor (); 
    
    // set my default form
    set_form (NOTHING); 
    
    set_cursor_visible (false); 

    set_cursor_moveable (false); 
    
    cursor_cur_blink_ = 0;

    set_mask (true);
}


/**
   Destructor
*/
label::~label ()
{
}


/**
   Set the font
*/
void label::set_font (win_font & font)
{
    my_font_ = &font;
}


/**
   Set the text 
*/
void label::set_text (const string & text)
{
    // init the vector and the cursor
    init_vec_cursor ();
    
    my_old_cursor_ = my_cursor_;
    
    // set the text
    my_text_ = text;
    my_cursor_.idx = my_text_.length ();  
    
    // build the vector
    build (true);
}



/**
   Add text
*/
void label::add_text (const string & text)
{
    new_text_ += text;
    
    // collect more text if we have unfinished utf8
    int size = new_text_.length ();
    if (size == 2 && (u_int8) new_text_[0] >= 0xE0) return;
    if (size == 1 && (u_int8) new_text_[0] >= 0x80) return;

    my_old_cursor_ = my_cursor_; 
    
    if (my_old_cursor_.idx == my_text_.length ()) 
    {
        my_text_ += new_text_;
        my_cursor_.idx = my_text_.length (); 
    }
    else my_text_.insert (my_cursor_.idx, new_text_);
    new_text_ = "";
    
    build (false);
}


/**
   REsize the label
*/
void label::resize (u_int16 l, u_int16 h)
{
    image::resize (l, h); 
    set_text (my_text_); 
}


/**
   Set the form
*/
void label::set_form (const u_int8 form)
{
    my_form_ = form;

    if (my_text_.empty())
    	return;

    build (true); 
}


/**
   Init vector and cursor
*/
void label::init_vec_cursor ()
{
    // init the cursor
	memset(&my_cursor_, 0, sizeof my_cursor_);
    
    // init the vector
    my_vect_.clear ();
    
    // create a line in the vector
    Sline_text tmp;
    memset(&tmp, 0, sizeof tmp);
    
    // add the new line at the beginning of the vector
    my_vect_.push_back (tmp);
    
    // the beginning of the display line,  0 is the first line
    start_line_ = 0;
}



/**
   Update the vector 
   start :  it's the index where the function must start to update
*/

void label::build (const bool erase_all)
{
    if (my_font_ == NULL) return;
    set_mask (false);
    switch (my_form_)
    {
        case NOTHING :
            build_form_nothing (); 
            update_cursor ();
            draw_string (!erase_all); 
            break;  
            
        case AUTO_HEIGHT : 
            build_form_auto_height ();
            update_cursor ();
            draw_string (!erase_all); 
            break;
            
        case AUTO_SIZE :
            build_form_auto_size ();
            update_cursor ();
            draw_string (false);
            break;
    }
    set_mask (true); 
}



/**
   Set if cursor is visible
*/
void label::set_cursor_visible (const bool b)
{
    visible_cursor_ = b; 
}


/**
   Set the cursor moveable with arrow
*/
void label::set_cursor_moveable (const bool b)
{
   moveable_cursor_ = b;   
}


/**
   Build the label when the form set top nothing
*/ 
void label::build_form_nothing ()
{
    // temporary variable
    u_int16 j, word_length, word_length_pix, start_idx;
    s_int16 word_offset_x;
    
    // temporary line
    Sline_text line_tmp;
    
    // we start at the beginning index of cursor line
    line_tmp.idx_beg = my_vect_[my_old_cursor_.line].idx_beg;  
    line_tmp.pos_x = 0;
    line_tmp.offset_x = 0;

    // we start always at the begin index of the line
    start_idx = line_tmp.idx_beg;   
    
    // erase the vector 
    vector <Sline_text>::iterator ii = my_vect_.begin ();
    u_int16 i = 0; 
    while (i != my_old_cursor_.line) { i++; ii++; } 
    my_vect_.erase (ii, my_vect_.end ());

    u_int16 prev_c = 0;
    
    while (start_idx < my_text_.length () )
    { 
        // if cur letter is an \n
        if (my_text_[start_idx] == '\n')
        {
            // the last index of this line
            line_tmp.idx_end = start_idx;
            
            // add to the vector line 
            my_vect_.push_back (line_tmp); 
            
            // init a Sline_text
            line_tmp.pos_x = 0; 
            line_tmp.offset_x = 0;
            line_tmp.idx_beg = ++start_idx; 

            prev_c = 0;
        }
        else if (my_text_[start_idx] == ' ')
        {
            if ((*my_font_) [' '].length () + LINE_LENGTH(line_tmp) > length ())
            {
                line_tmp.idx_end = start_idx;
                
                // add to the vector line 
                my_vect_.push_back (line_tmp); 
                
                // init a Sline_text
                line_tmp.pos_x = 0;
                line_tmp.offset_x = 0;
                line_tmp.idx_beg = ++start_idx; 
                
                prev_c = 0;
            }
            else
            {
                line_tmp.pos_x += (*my_font_) [' '].length ();
                if (prev_c != 0) line_tmp.offset_x += my_font_->kerning(prev_c, ' ');
                start_idx++;

                prev_c = ' ';
            }
        }
        else
        { 
            // find a word
            
            switch (find_word (start_idx, word_length, word_length_pix, word_offset_x, prev_c, LINE_LENGTH(line_tmp)))
            {
                case 0 : // enough place
                    line_tmp.pos_x += word_length_pix;
                    line_tmp.offset_x += word_offset_x;

                    break;
                    
                case 1 : // enough place,  but return at the next line 
                    // here we erase end of the last line

                    if (length () && height ())
                    {
                        offset_x_ = line_tmp.offset_x;
                        fillrect (line_tmp.pos_x,
                                  (my_vect_.size () - start_line_) * my_font_->height (),
                                  length () - LINE_LENGTH(line_tmp),
                                  my_font_->height (), screen::trans_col () );  
                    }
                    line_tmp.idx_end = (start_idx - word_length) - 1;   
                    my_vect_.push_back (line_tmp); 
                    
                    line_tmp.pos_x = word_length_pix; 
                    line_tmp.offset_x = word_offset_x;
                    line_tmp.idx_beg = start_idx - word_length;
                    
                    break;
                    
                case 2 : // not enough place
                    
                    j = start_idx - word_length;
                    while (j < start_idx)
                    {
                        u_int16 c = ucd (j);
                        if (line_tmp.pos_x + (*my_font_) [c].length ()  > length ())
                        {
                            line_tmp.idx_end = j - 1;
                            my_vect_.push_back (line_tmp);
                            
                            line_tmp.pos_x = 0;
                            line_tmp.offset_x = 0;
                            line_tmp.idx_beg = j; 
                        }
                        line_tmp.pos_x += (*my_font_) [c].length (); 
                        if (prev_c != 0) line_tmp.offset_x += my_font_->kerning(prev_c, c);
                        j++; 

                        prev_c = c;
                    }
                    break;  
            } 
        } 
    }
    
    // it is the last line
    line_tmp.idx_end = start_idx - 1;  
    my_vect_.push_back (line_tmp);    
}


void label::build_form_auto_height ()
{
    // it's the same
    build_form_nothing (); 
    
    // now verify if it's always the same size
    
    u_int16 new_size = my_vect_.size () * my_font_->height ();

    if (new_size  != height ())
    {
        image tmp (length (), new_size);
        tmp.fillrect (0, 0, length (), new_size, screen::trans_col ()); 
        draw (0, 0, 0, 0, length (), my_old_cursor_.pos_y + my_font_->height (), NULL, &tmp); 
        image::resize (length (), new_size); 
        tmp.draw (0, 0, NULL, this); 
    }
}


void label::build_form_auto_size ()
{
    // find the max height and the max length

    // clear the vector_
    my_vect_.clear ();
    
    // temporary line
    Sline_text line_tmp;
    
    line_tmp.pos_x = 0;
    line_tmp.offset_x = 0;
    line_tmp.idx_beg = 0;
    
    u_int16 i = 0, max_length = 0; 
    u_int16 prev_c = 0;
    
    while ( i < my_text_.size ())
    {
        if (my_text_[i] == '\n')
        {
            if (line_tmp.pos_x + LINE_LENGTH(line_tmp) > max_length)
			{
            	max_length = LINE_LENGTH(line_tmp);
			}
            line_tmp.idx_end = i; 
            my_vect_.push_back (line_tmp);
            
            line_tmp.idx_beg = i+1;
            line_tmp.pos_x = 0; 
            line_tmp.offset_x = 0;

            prev_c = 0;
        }
        else
        {
        	u_int16 c = ucd (i);
            line_tmp.pos_x += (*my_font_) [c].length ();
            if (prev_c != 0) line_tmp.offset_x += my_font_->kerning(prev_c, c);
            prev_c = c;
        }
        i++; 
    }
    
    if (LINE_LENGTH(line_tmp) > max_length)
	{
    	max_length = LINE_LENGTH(line_tmp);
	}

    // the last line
    line_tmp.idx_end = i-1;
    my_vect_.push_back (line_tmp);
    
    // now resize the label
    image::resize (max_length, my_vect_.size () * my_font_->height ());  
}

void label::fit_text_width()
{
	u_int16 new_size = 0;
	for (vector<Sline_text>::iterator i = my_vect_.begin(); i != my_vect_.end(); i++)
	{
	    if (LINE_LENGTH((*i)) > new_size)
		{
	    	new_size = LINE_LENGTH((*i));
		}
	}

    if (new_size < length ())
    {
    	set_length(new_size);
    }
}

void label::clean_surface (const bool erase_all)
{
    if (length () && height ())
    {     
        if ( my_cursor_.idx != my_text_.length ())
        {
        	offset_x_ = my_old_cursor_.offset_x;
            fillrect ( my_old_cursor_.pos_x, my_old_cursor_.pos_y, length () - my_old_cursor_.pos_x,
                       my_font_->height (), screen::trans_col (), NULL);
            offset_x_ = 0;
            fillrect (0, my_old_cursor_.pos_y + my_font_->height (), length (),
                      height () -my_old_cursor_.pos_y + my_font_->height (), screen::trans_col ()); 
        }
        else if (erase_all)
        {
            offset_x_ = 0;
            fillrect (0, 0, length (), height (), screen::trans_col ()); 
        }
    }
}





// find a word
// index :  the word begin at the index
// wlength : size of word
// wlengthpix : size of word in pixel
// length :

// return 0 if enough size for this word,  1 if enough but must return on the next line, 2 if the word is bigger than the length 
u_int8 label::find_word (u_int16 & index, u_int16 & wlength, u_int16 & wlengthpix, s_int16 & woffset, u_int16 & last_letter, const u_int16 rlength)
{
	u_int16 c = 0;

    wlength = index;
    wlengthpix = 0;
    woffset = 0;
    while (index < my_text_.length ()  && my_text_[index] != ' ' && my_text_[index] != '\n')
    {
    	c = ucd (index);
        wlengthpix += (*my_font_) [c].length ();
        if (last_letter != 0) woffset += my_font_->kerning(last_letter, c);
    	last_letter = c;
        index++;
    }

    // count of characters (which is != count of letters due to utf-8 encoding)
    wlength = index - wlength;
	last_letter = c;
    
    // if size of word is bigger than the length of label 
    if (wlengthpix + woffset/screen::scale() < length () - rlength)  return 0;
    else if (wlengthpix + woffset/screen::scale() < length ())  return 1;
    return 2;
}



void label::update_cursor ()
{
    // find the cursor position
    bool b = false; 
    
    // init the blink cursor
    cursor_cur_blink_ = cursor_blink_cycle; 
    
    // find the iterator line where is the cursor
    while (!b && my_cursor_.line < my_vect_.size () )
    { 
        if (my_cursor_.idx >= my_vect_[my_cursor_.line].idx_beg &&
            my_cursor_.idx <= my_vect_[my_cursor_.line].idx_end ) b = true;
        else if (my_cursor_.idx >  my_vect_[my_cursor_.line].idx_end) 
        {
            if (my_cursor_.line ==  my_vect_.size () - 1) b = true; 
            else my_cursor_.line++;
        }
        else if (my_cursor_.idx <  my_vect_[my_cursor_.line].idx_beg)
        {
            my_cursor_.line--; 
        }
    }
 
    // now find the x position of the cursor
    my_cursor_.pos_x = 0;
    my_cursor_.offset_x = 0;
    
    u_int16 prev_c = 0;
    u_int16 j = my_vect_[my_cursor_.line].idx_beg;
    while (j < my_cursor_.idx) {
    	u_int16 c = ucd (j);
        my_cursor_.pos_x+= (*my_font_) [c].length ();
        if (prev_c != 0) my_cursor_.offset_x += my_font_->kerning(prev_c, c);
    	prev_c = c;
        j++;
    }

    // find y position
    my_cursor_.pos_y = (my_cursor_.line - start_line_) * my_font_->height (); 
}



// if bool is false redraw all,  if bool is true redraw just at beginning of the cursor 
void label::draw_string (const bool at_cursor)
{ 
    u_int16 tmp_start_line;
    u_int16 tx = 0, ty = 0;
    u_int16 idx_cur_line, j; 
    u_int16 c, prev_c = 0;
    s_int16 ox = 0;
    
    // if not at cursor, we erase all
    clean_surface (!at_cursor); 
    
    if (at_cursor)
    { 
        tmp_start_line = my_old_cursor_.line;
        tx = my_old_cursor_.pos_x;
        ox = my_cursor_.offset_x; // my_cursor_ has been updated with the correct offset!
        idx_cur_line = my_old_cursor_.idx;
        ty = (tmp_start_line - start_line_) * my_font_->height ();
    }
    else
    { 
        tmp_start_line = start_line_; 
        idx_cur_line = my_vect_[tmp_start_line].idx_beg; 
    } 
    
    // draw the first line
    for (j = idx_cur_line;
         j < my_vect_[tmp_start_line].idx_end + 1 ;
         j++)
    {
        c = ucd (j);
        if (c != '\n' && my_font_->in_table (c))
        {
            if (prev_c != 0) ox += my_font_->kerning(prev_c, c);
        	offset_x_ = ox;
            (*my_font_) [c].draw (tx, ty, NULL, this);
            tx += (*my_font_) [c].length ();
            prev_c = c;
        }
        else prev_c = 0;
    }
    ty += my_font_->height ();
    tmp_start_line++; 
    
    
    // draw another line
    while (tmp_start_line < my_vect_.size ())
    {
        tx = 0; 
        ox = 0;
        for (j = my_vect_[tmp_start_line].idx_beg;
             j <  my_vect_[tmp_start_line].idx_end + 1 ;
             j++)
        {
            c = ucd (j);
            if (my_font_->in_table (c))
            {
                if (prev_c != 0) ox += my_font_->kerning(prev_c, c);
            	offset_x_ = ox;
                (*my_font_) [c].draw (tx, ty, NULL, this);
                tx += (*my_font_) [c].length (); 
                prev_c = c;
            }
            else prev_c = 0;
        }
        ty += my_font_->height ();
        tmp_start_line++;
    } 
}


bool label::update ()
{ 
    if (visible_cursor_)
    {
        if (! (height () && length ())) return true;  
        if (cursor_cur_blink_ == cursor_blink_cycle)
        {
            set_mask(false);
            cursor_draw ();
            set_mask(true);
            cursor_cur_blink_ = 0; 
        }else if (cursor_cur_blink_ == (cursor_blink_cycle >> 1))
            cursor_undraw ();   
        cursor_cur_blink_++; 
    }  
    return true; 
}



void label::cursor_draw ()
{
     // draw the cursor
    u_int16 idx = my_cursor_.idx;
	offset_x_ = my_cursor_.offset_x;
    if (last_letter (idx)  || my_text_[idx] == '\n')  
        my_font_->cursor->draw (my_cursor_.pos_x, my_cursor_.pos_y,NULL, this);
    else
        my_font_->cursor->draw (my_cursor_.pos_x, my_cursor_.pos_y,0, 0, 
                                (*my_font_) [ucd (idx)].length (),
                                my_font_->height (), NULL, this); 
}

void label::cursor_undraw ()
{ 
    // draw letter instead  
    u_int16 idx = my_cursor_.idx;
	offset_x_ = my_cursor_.offset_x;
    if (last_letter (idx) || my_text_[idx] == '\n') 
    {
        fillrect(my_cursor_.pos_x, my_cursor_.pos_y,
                 my_font_->cursor->length () ,
                 my_font_->cursor->height(),
                 screen::trans_col());
    }
    else (*my_font_) [ucd (idx)].draw (my_cursor_.pos_x, my_cursor_.pos_y, NULL, this);
}

bool label::last_letter (u_int16 idx) 
{
    if ((u_int8) my_text_[idx] == 0xEF) return my_text_.length () - idx == 2;
    if ((u_int8) my_text_[idx] == 0xC3) return my_text_.length () - idx == 1;
    return my_cursor_.idx == my_text_.length ();
}

bool label::input_update ()
{
    if(input::has_been_pushed(KEY_CURSOR_NEXT)) 
    {
        if (! (height () && length ())) return false;  
        // cursor_undraw (); 
        // cursor_next (); 
    }
    else if (input::has_been_pushed(KEY_CURSOR_PREVIOUS))
    {
        if (! (height () && length ())) return false;  
        // cursor_undraw ();  
        // cursor_previous (); 
    }
    
    return true; 
}


void label::cursor_next ()
{
    if (!moveable_cursor_) return; 
    if (my_cursor_.idx < my_text_.length ()) 
    {
    	// TODO: kerning
        u_int8 count;
        if (my_cursor_.idx < my_text_.length () - 2 && (u_int8) my_text_[my_cursor_.idx+1] == 0xEF) count = 3;
        else if (my_cursor_.idx < my_text_.length () - 1 && (u_int8) my_text_[my_cursor_.idx+1] == 0xC3) count = 2;
        else count = 1;
        
        my_cursor_.idx += count; 
        update_cursor ();
    }
}


void label::cursor_previous ()
{
    if (!moveable_cursor_) return; 
    if (my_cursor_.idx > 0) 
    {
    	// TODO: kerning
        u_int8 count;
        if (my_cursor_.idx > 2 && (u_int8) my_text_[my_cursor_.idx-3] == 0xEF) count = 3;
        else if (my_cursor_.idx > 1 && (u_int8) my_text_[my_cursor_.idx-2] == 0xC3) count = 2;
        else count = 1;

        my_cursor_.idx -= count;
        update_cursor ();
    }
}


const string & label::text_string () const
{
    return my_text_;  
}

const char * label::text_char () const
{
    return my_text_.c_str (); 
}

// utf-8 --> utf-16
u_int16 label::ucd (const std::string & text, u_int16 & i) const
{
    const u_int8 c = text.at(i);
    if (c < 0x80) return c;

    if (c < 0xe0) 
    {
        const u_int8 c1 = text.at(++i);
        return ((u_int16) (c & 0x1f) << 6)
            |   (u_int16) (c1 ^ 0x80);
    }
    
    const u_int8 c1 = text.at(++i);
    const u_int8 c2 = text.at(++i);
    return ((u_int16) (c & 0x0f) << 12)
        |  ((u_int16) (c1 ^ 0x80) << 6)
        |   (u_int16) (c2 ^ 0x80);
}
