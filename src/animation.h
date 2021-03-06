/*
   Copyright (C) 1999/2000/2001   Alexandre Courbot.
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


/**
 * @file   animation.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the animationframe and animation classes.
 *  
 */



#ifndef _ANIMATION_H
#define _ANIMATION_H


#include "image.h"
#include <vector>


/**
 * Handles images properties in an animation.
 * %Objects of this class have no reason to exist if not affected to an
 * animation. The fact is, that often in an animation, you want the
 * same image to appear at different times, different positions or with
 * different mask and alpha values. An animationframe is a class that
 * contains the index of the image to display, the alpha and mask parameters
 * to give it, the time (in game cycles) it should be displayed before going
 * to the next frame, and the index and the frame to display right after
 * this one. As images and animationframes are arranged into an indexed array
 * in an animation, the index values only make sense from the animation point
 * of view.
 * 
 */ 
class animationframe
{
 public:

    /** 
     * Default constructor. 
     * 
     */
    animationframe ();

    /** 
     * Destructor.
     * 
     */
    ~animationframe ();

    /** 
     * Resets an animationframe to it's initial (i.e post-constructor) state.
     * 
     */
    void clear ();

 
    /**
     * @name Mask and Alpha Settings.
     * 
     */ 
    //@{ 

    /** 
     * Returns whether this frame is masked or not.
     * 
     * 
     * @return true if the surface is masked, false otherwise.
     */
    bool is_masked () const
    {
        return is_masked_;
    }
    
    /** 
     * Sets the mask parameter of this frame.
     * 
     * @param mask true if the surface should be masked, false otherwise.
     */
    void set_mask (bool mask)
    {
        is_masked_ = mask;
    }

    /** 
     * Returns the alpha value the this frame.
     * 
     * 
     * @return the alpha value of the frame.
     */
    u_int8 alpha () const
    {
        return alpha_;
    }     

    /** 
     * Sets the alpha value for this frame.
     * 
     * @param a new alpha value.
     */
    void set_alpha (u_int8 a)
    {
        alpha_ = a;
    }

    //@}


    /**
     * @name Image, delay and next frame settings.
     * 
     */ 
    //@{ 
    
    /** 
     * Returns the image number this frame points to.
     * 
     * 
     * @return the index of the image this frame points to.
     */
    u_int16 image_nbr () const
    {
        return imagenbr;
    }
    
    /** 
     * Sets the image this frame should point to.
     * 
     * @param imnbr the index of the image this frame should point to.
     */
    void set_image_nbr (u_int16 imnbr)
    {
        imagenbr = imnbr;
    }
    
    /** 
     * Returns the duration of this frame.
     * 
     * 
     * @return the delay (in game cycles) of this frame (0 means infinite).
     */
    u_int16 delay () const
    {
        return delay_;
    }

    /** 
     * Sets the duration of this frame.
     * 
     * @param d new delay (in game cycles, 0 means infinite).
     */
    void set_delay (u_int16 d)
    {
        delay_ = d;
    }
    
    /** 
     * Returns the index of the frame that will be displayed once
     * the delay of this one expired.
     * 
     * 
     * @return the index of the frame next to this one.
     */
    u_int16 nextframe () const
    {
        return nextframe_;
    }
    
    /** 
     * Sets the index of the frame that will be displayed right after
     * this one.
     * 
     * @param nf index of the frame that will be next to this one.
     */
    void set_nextframe (u_int16 nf)
    {
        nextframe_ = nf;
    }
    
    //@}


    /**
     * @name Individual frames relative position.
     * 
     */
    //@{
    
    /** 
     * Returns the X offset (i.e position relative to the animation's position)
     * of this frame.
     * 
     * 
     * @return the X offset of this frame.
     */
    u_int16 offx () const
    {
        return gapx;
    }
     
    /** 
     * Returns the Y offset (i.e position relative to the animation's position)
     * of this frame.
     * 
     * 
     * @return the Y offset of this frame.
     */
    u_int16 offy () const
    {
        return gapy;
    }
    
    /** 
     * Sets the offset for this frame.
     * 
     * @param ox new X offset.
     * @param ox new Y offset.
     */
    void set_offset (u_int16 ox,  u_int16 oy)
    {
        gapx = ox;
        gapy = oy;
    }

    //@}

    
    /**
     * @name Saving/Loading Methods.
     * 
     */
    //@{
    

    /** 
     * Loads an animationframe from an opened file.
     * 
     * @param file the opened file from which to read.
     * 
     * @return 0 in case of success, error number in case of error.
     */
    s_int8 get (igzstream& file);

    /** 
     * Saves an animationframe into an opened file.
     * 
     * @param file the opened file where to save.
     * 
     * @return 0 in case of success, error number in case of error.
     */
    s_int8 put (ogzstream& file) const;

    
    //@}
 
    
private:
    u_int16 imagenbr;
    bool is_masked_;
    u_int8 alpha_;
    s_int16 gapx;
    s_int16 gapy;
    u_int16 delay_;
    u_int16 nextframe_;     
};



/**
 * Whether the animation is currently playing or not.
 */ 
typedef enum { PLAY = true, STOP = false } play_state; 



/**
 * Class that handles animated elements, their update and their playback.
 * Most often, you will want your drawn %objects to be animated. Then you'll
 * probably want to use this class. An animation contains:
 *   - A set of images arranged in an indexed array.
 *   - A set of animation_frames.
 *   - A global position offset.
 *
 * During playback, the animation look at the first animation_frame. Each
 * animation_frame refers to an image of the animation, and give it special
 * mask and alpha parameters, as well as a position offset. It also have
 * a delay parameter, telling how many %game cycles this frame should stay.
 * Once the delay expired, the animation jumps to the next frame, which
 * is pointed by the current frame. That way, you can easily performs loops or
 * others effects. Each image, as well as each animation_frame, can be accessed
 * individually, thought you'd better try to avoid as much as possible to mess
 * with that.
 * 
 */ 
class animation : public drawable
{
public:

    /**
     * Default constructor.
     * 
     */
    animation ();

    /**
     * Destructor.
     * 
     */ 
    ~animation ();


    /** 
     * Clears an animation, that is put it back into the original
     * (constructor) state.
     * 
     */
    void clear ();


    /**
     * @name Playback control methods.
     * 
     */
    //@{
    

    /** 
     * Starts the playback of the animation.
     * 
     */
    void play ()
    {
        play_flag = PLAY;
#ifdef _EDIT_
        if (in_editor)
            must_upt_label_status = true;
#endif
    }

    /** 
     * Stops the playback of the animation.
     * 
     */
    void stop ()
    {
        play_flag = STOP;
#ifdef _EDIT_
        if (in_editor)
            must_upt_label_status = true;
#endif
    }

    /** 
     * Returns whether the animation is currently being played.
     * 
     * 
     * @return PLAY is the animation is currently playing, STOP otherwise.
     */
    play_state playstate () const
    {
        return play_flag; 
    }
    
    /** 
     * Rewinds the animation to it's beginning.
     * 
     */
    void rewind ()
    {
        currentframe_ = 0;
        speedcounter = 0;
    }


    /** 
     * Directly jumps to the next frame.
     * 
     */
    void next_frame ();
    
    //@}
    
    

    /**
     * @name State updating Methods.
     * 
     */ 
    //@{
    
    /**
     * Updates the animation state.
     * 
     */ 
    bool update ();

    //@}


    /**
     * @name Drawing Methods.
     * 
     */ 
    //@{ 

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const;

    //@}


    /**
     * @name Saving/Loading Methods.
     * @note There is no way to save animations with this class.
     * 
     */
    //@{
    
    /**
     * Loads an animation from an opened file.
     * @param file the opened file from which to load.
     * @return 0 in case of success, error code otherwise.
     *
     * @todo length and height are loaded while they are calculated later.
     *       Remove this when format will change.
     * 
     */ 
    s_int8 get (igzstream& file);

    /** 
     * Loads an animation from it's filename.
     * 
     * @param fname the name of the file to load.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 load (string fname);
    
    
    /** Saves an animation into an opened file, in %game format, with
     *  alpha and mask values. 
     *  @warning as the animation which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving animations with full
     *           truecolor quality, use animation_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save ()
     */
    s_int8 put (ogzstream& file) const;

    /** Saves an animation into an file, in %game format, with
     *  alpha and mask values.
     *  @warning as the animation which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving animations with full
     *           truecolor quality, use animation_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put ()
     */
    s_int8 save (string fname) const;

    //@}


    /**
     * @name Global animation properties Methods.
     * 
     */
    //@{
    
    /** 
     * Returns the number of frames in this animation.
     * 
     * 
     * @return the number of frames in this animation.
     */
    u_int16 nbr_of_frames () const
    {
        return frame.size ();
    }
 
    /** 
     * Returns the number of images in this animation.
     * 
     * 
     * @return the number of images in this animation.
     */
    u_int16 nbr_of_images () const
    {
        return t_frame.size ();
    }
     
    /** 
     * Returns the index of the currently displayed frame.
     * 
     * 
     * @return the index of the frame currently displayed.
     */
    u_int16 currentframe () const
    {
        return currentframe_;
    }
    
    /** 
     * Sets the current frame.
     * 
     * @param framenbr the index of the frame to display now.
     */
    void set_currentframe (u_int16 framenbr)
    {
        currentframe_ = framenbr;
        speedcounter = 0; 
    }
    
    /** 
     * Returns the global X offset of the animation.
     * 
     * 
     * @return the global X offset of the animation.
     */
    s_int16 xoffset () const
    {
        return xoffset_;
    }
     
    /** 
     * Returns the global Y offset of the animation.
     * 
     * 
     * @return the global Y offset of the animation.
     */
    s_int16 yoffset () const
    {
        return yoffset_;
    }

    /** 
     * Set the global offsets of this animation.
     * 
     * @param x new X global offset.
     * @param y new Y global offset.
     */
    void set_offset (s_int16 x, s_int16 y)
    {
        xoffset_ = x;
        yoffset_ = y;
    }

    
    //@}
    
     

    /**
     * @name Image and Animationframe control Methods.
     * 
     */
    //@{

    /** 
     * Returns a pointer to a desired frame.
     * 
     * @param nbr the index of the frame to get.
     * 
     * @return pointer to the nbr frame.
     */
    animationframe *get_frame (u_int16 nbr)
    {
        return &(frame[nbr]);
    }
    
    /** 
     * Returns a pointer to a desired image.
     * 
     * @param nbr the index of the image to get.
     * 
     * @return pointer to the nbr image.
     */
    image *get_image (u_int16 nbr)
    {
        return t_frame[nbr];
    }
    
    /** 
     * Inserts an image at a given position of the image array.
     * All the frames will be updated so the operation doesn't affect
     * the animation in any way.
     *
     * The animation will be responsible for freeing the inserted image.
     * 
     * @param im pointer to the image to add.
     * @param pos index where to add the image.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 insert_image (const image * im, u_int16 pos);


    /** 
     * Inserts a frame at a given position of the animationframe array.
     * All the frames will be updated so the operation doesn't affect
     * the animation in any way.
     * 
     * @param af the animationframe to add.
     * @param pos index where to add the frame.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 insert_frame (const animationframe af, u_int16 pos);

    /** 
     * Removes an image at a given position.
     * The image itself will also be deleted ().
     * All the frames will be updated so the operation doesn't affect
     * the animation in any way.
     *
     * @param pos The index of the image to remove.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 delete_image (u_int16 pos);
 
    /** 
     * Removes a frame at a given position.
     * All the frames will be updated so the operation doesn't affect
     * the animation in any way.
     * 
     * @param pos The index of the animationframe to remove.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 delete_frame (u_int16 pos); 
    
    //@}
    
    /**
     * @name Special FX methods.
     * 
     */
    //@{

    /** 
     * Zooms an animation.
     * 
     * @param sx Desired X size.
     * @param sy Desired Y size.
     * @param src Source animation to zoom.
     */
    void zoom (u_int16 sx, u_int16 sy, const animation * src);

    //@}

#ifndef SWIG
    /**
     * Animation copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */ 
    animation& operator = (const animation& src);
#endif

    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (const animation& src) 
    {
        *this = src; 
    }

    
private:
    /**
     * Forbid value passing.
     */
    animation(const animation& src);

    /** 
     * Calculate the real dimensions of the animation, depending
     * of it's frames and images.
     * 
     */
    void calculate_dimensions (); 

    mutable vector <image *> t_frame;
    mutable vector <animationframe> frame;
    u_int16 currentframe_;
    u_int16 speedcounter;
    play_state play_flag;
    s_int16 xoffset_, yoffset_;
     
#ifndef SWIG
    friend class win_anim;
#endif
};

#endif
