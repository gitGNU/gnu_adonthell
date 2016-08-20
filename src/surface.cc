/*
   Copyright (C) 1999/2000/2001/2004   Alexandre Courbot
   Copyright (C) 2016  Kai Sterker
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
 * @file surface.cc
 * @author Alexandre Courbot
 * @author Kai Sterker
 *
 * Defines the surface class.
 */ 

#include "surface.h"
#include "screen.h"

#include <iostream>

using namespace std; 


SDL_Rect surface::srcrect; 
SDL_Rect surface::dstrect; 


surface::surface (const u_int8 & scale) : drawable ()
{
	scale_ = scale;
	offset_x_ = 0;
	is_masked_ = false;
	mask_changed_ = false;
	alpha_ = SDL_ALPHA_OPAQUE;
	alpha_channel_ = false;
    Surface = NULL;
    Info = new pixel_info();
}

surface::~surface ()
{
    delete Info;
    if (Surface) SDL_DestroyTexture (Surface);
}

void surface::set_mask (bool m)
{
    // since SDL textures do not support masking, we need to
    // convert the masked image into the appropriate RGBA
    // representation.
    if (!Surface)
    {
    	// if there is no surface yet, we will have to apply the
    	// surface conversion later
    	mask_changed_ = true;
    	return;
    }

    if (m && !is_masked_)
    {
    	is_masked_ = true;
        mask_changed_ = false;
        
        SDL_Surface *s1 = to_sw_surface();
        SDL_Surface *s2 = SDL_CreateRGBSurfaceFrom(NULL, s1->w, s1->h,
                        32, 0, R_MASK, G_MASK, B_MASK, A_MASK);

        Info->Format = s2->format->format;

        SDL_Texture *tmp = SDL_CreateTexture (screen::get_renderer(), Info->Format, SDL_TEXTUREACCESS_STREAMING, s1->w, s1->h);
        if (!tmp) std::cout << "*** surface_sdl::set_mask: " << SDL_GetError() << std::endl;

        SDL_LockTexture(tmp, NULL, &s2->pixels, &s2->pitch);

        SDL_SetSurfaceAlphaMod (s1, SDL_ALPHA_OPAQUE);
        SDL_SetSurfaceBlendMode (s1, SDL_BLENDMODE_NONE);
        SDL_BlitSurface (s1, NULL, s2, NULL);

        SDL_UnlockTexture(tmp);

        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
        SDL_DestroyTexture(Surface);

        Surface = tmp;
        alpha_channel_ = true;
    }

    is_masked_ = m;
}


void surface::set_alpha (u_int8 t, const bool & alpha_channel)
{
    if ((t == 255) && (alpha_ != 255) && Surface)
    {
        SDL_SetTextureAlphaMod(Surface, t);
        SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_NONE);
    }

    else if (!alpha_channel && alpha_channel_ && Surface)
    {
        SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_NONE);
    }

    alpha_ = t;
    alpha_channel_ = alpha_channel || is_masked_;
}

SDL_Surface *surface::to_sw_surface(SDL_Rect *rect) const
{
    int bpp;
    SDL_Surface *s;
    u_int32 rmask, gmask, bmask, amask;

    lock(rect);

    SDL_PixelFormatEnumToMasks(Info->Format, &bpp, &rmask, &gmask, &bmask, &amask);
    if (!rect)
    {
    	s = SDL_CreateRGBSurfaceFrom(Info->Pixels, length() * scale(), height() * scale(),
                bpp, Info->Pitch, rmask, gmask, bmask, amask);
    }
    else
    {
    	s = SDL_CreateRGBSurfaceFrom(Info->Pixels, rect->w, rect->h,
                bpp, Info->Pitch, rmask, gmask, bmask, amask);
    }

    if (is_masked_)
    {
        u_int32 trans_col = alpha_channel_? SDL_MapRGBA(s->format, 0xFF, 0x00, 0xFF, 0xFF) : SDL_MapRGB(s->format, 0xFF, 0x00, 0xFF);
        SDL_SetColorKey(s, 1, trans_col);
    }

    if (alpha_channel_ || alpha_ != 255)
    {
        if (!alpha_channel_ || is_masked_) SDL_SetSurfaceAlphaMod (s, alpha_);
        SDL_SetSurfaceBlendMode (s, SDL_BLENDMODE_BLEND);
    }

    return s;
}

void surface::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                    u_int16 sh, const drawing_area * da_opt,
                    surface * target) const
{ 
    setup_rects (x, y, sx, sy, sl, sh, da_opt); 

    if (!dstrect.w || !dstrect.h)
        return;

    if (!target || target == &screen::display)
    {
        // blit to screen surface (--> hardware accelerated)
        if (alpha_channel_ || alpha_ != 255)
        {
            if (!alpha_channel_ || is_masked_) SDL_SetTextureAlphaMod(Surface, alpha_);
            SDL_SetTextureBlendMode(Surface, SDL_BLENDMODE_BLEND);
        }

        if (scale() > 1)
        {
        	srcrect.x *= scale();
        	srcrect.y *= scale();
        	srcrect.w *= scale();
        	srcrect.h *= scale();
        }

        if (screen::scale() > 1)
        {
            dstrect.x = dstrect.x * screen::scale() + screen::offset_x();
            dstrect.y = dstrect.y * screen::scale() + screen::offset_y();
            dstrect.w *= screen::scale();
            dstrect.h *= screen::scale();
        }

        SDL_RenderCopy (screen::get_renderer(), Surface, &srcrect, &dstrect);
    }
    else
    {
    	// make sure the drawing target uses the same scale
    	if (target->scale() != scale())
    	{
    		target->set_scale(scale());
    	}

        if (scale() > 1)
		{
			srcrect.x *= scale();
			srcrect.y *= scale();
			srcrect.w *= scale();
			srcrect.h *= scale();

			dstrect.x = dstrect.x * scale() + target->offset_x_;
			dstrect.y *= scale();
			dstrect.w *= scale();
			dstrect.h *= scale();

			if (dstrect.x < 0) dstrect.x = 0;
			if (dstrect.y < 0) dstrect.y = 0;
		}

        // make sure we only update the part of the target texture that is actually changed
    	if (dstrect.x + dstrect.w > target->length() * scale()) dstrect.w = target->length() * scale() - dstrect.x;
    	if (dstrect.y + dstrect.h > target->height() * scale()) dstrect.h = target->height() * scale() - dstrect.y;
    	if (dstrect.w <= 0 || dstrect.h <= 0) return;

    	srcrect.w = dstrect.w;
    	srcrect.h = dstrect.h;

        // blit from one surface to another (--> needs to be in software)
        SDL_Surface *source_surf = to_sw_surface ();
        SDL_Surface *target_surf = target->to_sw_surface (&dstrect);

        SDL_BlitSurface (source_surf, &srcrect, target_surf, NULL);

        target->unlock();

        SDL_FreeSurface(source_surf);
        SDL_FreeSurface(target_surf);
    }
}

void surface::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int32 col, 
                        drawing_area * da_opt)
{
    if (da_opt) 
    {
        SDL_Rect da = da_opt->setup_rects ();
        dstrect.x = da.x;
        dstrect.y = da.y;
        dstrect.w = da.w;
        dstrect.h = da.h;
    }
    else
    { 
        dstrect.x = x;
        dstrect.y = y;
        dstrect.w = l;
        dstrect.h = h;
    }

    if (this == &screen::display)
    {
        u_int8 r, g, b, a;
        unmap_color(col, r, g, b, a);

        if (screen::scale() > 1)
        {
            dstrect.x = dstrect.x * screen::scale() + screen::offset_x();
            dstrect.y = dstrect.y * screen::scale() + screen::offset_y();
            dstrect.w *= screen::scale();
            dstrect.h *= screen::scale();
        }

        SDL_SetRenderDrawBlendMode(screen::get_renderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(screen::get_renderer(), r, g, b, a);
        SDL_RenderFillRect(screen::get_renderer(), &dstrect);
    }
    else
    {
    	if (is_masked_ && col == screen::trans_col())
    	{
    		col = map_color(255, 0, 255, SDL_ALPHA_TRANSPARENT);
    	}

        if (scale() > 1)
        {
            dstrect.x = dstrect.x * scale() + offset_x_;
            dstrect.y *= scale();
            dstrect.w *= scale();
            dstrect.h *= scale();

            if (dstrect.x < 0) dstrect.x = 0;
        }

        if (dstrect.x + dstrect.w > length() * scale()) dstrect.w = length() * scale() - dstrect.x;
        if (dstrect.y + dstrect.h > height() * scale()) dstrect.h = height() * scale() - dstrect.y;
        if (dstrect.w <= 0 || dstrect.h <= 0) return;

        lock(&dstrect);

        u_int32 *src = new u_int32[dstrect.w * dstrect.h];
        std::fill_n(src, dstrect.w * dstrect.h, col);

        SDL_ConvertPixels (dstrect.w, dstrect.h,
			SDL_PIXELFORMAT_ARGB8888, (const void*) src, dstrect.w*4,
            Info->Format, Info->Pixels, Info->Pitch);

        delete[] src;
        unlock();
    }
}

// convert RGBA color to surface format
u_int32 surface::map_color (const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a) const
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// convert surface color format into RGBA
void surface::unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const
{
    a = (col & 0xFF000000) >> 24;
    r = (col & 0x00FF0000) >> 16;
    g = (col & 0x0000FF00) >> 8;
    b = (col & 0x000000FF);
}

void surface::lock (SDL_Rect *rect) const
{
    if (!length () || !height ()) return;

    if (this != &screen::display)
    {
        SDL_LockTexture (Surface, rect, &Info->Pixels, &Info->Pitch);
        Info->BytesPerPixel = SDL_BYTESPERPIXEL(Info->Format);
    }
}

void surface::unlock () const
{
    if (!length () || !height ()) return;   

    if (Info->Pixels)
    {
        SDL_UnlockTexture (Surface);
        Info->Pixels = NULL;
    }
}

void surface::put_pix (u_int16 x, u_int16 y, u_int32 col) 
{
    u_int8 r, g, b, a;

    if (this == &screen::display)
    {
        unmap_color(col, r, g, b, a);

        if (screen::scale() > 1)
        {
            fillrect(x, y, 1, 1, col);
            return;
        }

        x += screen::offset_x();
        y += screen::offset_y();

        SDL_SetRenderDrawBlendMode(screen::get_renderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(screen::get_renderer(), r, g, b, a);
        SDL_RenderDrawPoint(screen::get_renderer(), x, y);
        return;
    }

    if (!Info->Pixels) return;

    u_int8 *offset = ((Uint8 *) Info->Pixels) + y * Info->Pitch + x * Info->BytesPerPixel;

    if (!alpha_channel_)
    {
        unmap_color(col, r, g, b, a);

        switch (Info->Format)
        {
            case SDL_PIXELFORMAT_RGB24:
                *(offset) = r;
                *(++offset) = g;
                *(++offset) = b;
                break;
            case SDL_PIXELFORMAT_BGR24:
                *(offset) = b;
                *(++offset) = g;
                *(++offset) = r;
                break;
        }
        return;
    }

#ifdef __BIG_ENDIAN__
    unmap_color(col, b, a, g, r);
#else
    unmap_color(col, r, g, b, a);
#endif
            
    switch (Info->Format)
    {
        case SDL_PIXELFORMAT_BGR888:
            *(++offset) = r;
            *(++offset) = g;
            *(++offset) = b;
            break;
        case SDL_PIXELFORMAT_RGBA8888:
            *(offset) = a;
            *(++offset) = r;
            *(++offset) = g;
            *(++offset) = b;
            break;
        case SDL_PIXELFORMAT_ARGB8888:
            *(offset) = r;
            *(++offset) = g;
            *(++offset) = b;
            *(++offset) = a;
            break;
        case SDL_PIXELFORMAT_RGB888:
            *(++offset) = b;
            *(++offset) = g;
            *(++offset) = r;
            break;
        case SDL_PIXELFORMAT_ABGR8888:
            *(offset) = b;
            *(++offset) = g;
            *(++offset) = r;
            *(++offset) = a;
            break;
        case SDL_PIXELFORMAT_BGRA8888:
            *(offset) = a;
            *(++offset) = b;
            *(++offset) = g;
            *(++offset) = r;
            break;
        default:
            std::cout << "*** surface::put_pix: Unsupported format " << SDL_GetPixelFormatName(Info->Format) << std::endl;
            exit(1);
    }}

u_int32 surface::get_pix (u_int16 x, u_int16 y) const
{
    if (!Info->Pixels) return 0;

    u_int8 r, g, b, a = SDL_ALPHA_OPAQUE;
    u_int8 *offset = ((Uint8 *) Info->Pixels) + y * Info->Pitch + x * Info->BytesPerPixel;

    if (!alpha_channel_)
    {
        switch (Info->Format)
        {
            case SDL_PIXELFORMAT_RGB24:
                r = *(offset);
                g = *(++offset);
                b = *(++offset);
                break;
            case SDL_PIXELFORMAT_BGR24:
                b = *(offset);
                g = *(++offset);
                r = *(++offset);
                break;
        }

        return map_color (r, g, b, a);
    }
    
    switch (Info->Format)
    {
		case SDL_PIXELFORMAT_BGR888:
			r = *(++offset);
			g = *(++offset);
			b = *(++offset);
			break;
		case SDL_PIXELFORMAT_BGRA8888:
			a = *(offset);
			r = *(++offset);
			g = *(++offset);
			b = *(++offset);
			break;
		case SDL_PIXELFORMAT_ABGR8888:
			r = *(offset);
			g = *(++offset);
			b = *(++offset);
			a = *(++offset);
			break;
		case SDL_PIXELFORMAT_RGB888:
			b = *(++offset);
			g = *(++offset);
			r = *(++offset);
			break;
		case SDL_PIXELFORMAT_ARGB8888:
			b = *(offset);
			g = *(++offset);
			r = *(++offset);
			a = *(++offset);
			break;
		case SDL_PIXELFORMAT_RGBA8888:
			a = *(offset);
			b = *(++offset);
			g = *(++offset);
			r = *(++offset);
			break;
		default:
            std::cout << "surface::get_pix: Unsupported format " << SDL_GetPixelFormatName(Info->Format) << std::endl;
            exit(1);
    }

#ifdef __BIG_ENDIAN__
    return map_color (g, r, a, b);
#else
    return map_color (r, g, b, a);
#endif
}

void surface::set_scale(const u_int8 & new_scale)
{
    // cannot scale the screen surface
    if (this == &screen::display) return;

    // already at requested scale
	if (scale() == new_scale)
		return;

	if (Surface == NULL)
	{
		scale_ = new_scale;
		return;
	}

	int bpp, pitch;
	u_int8 *target_data;
	u_int32 rmask, gmask, bmask, amask;

    lock();

    SDL_PixelFormatEnumToMasks(Info->Format, &bpp, &rmask, &gmask, &bmask, &amask);
	SDL_Texture *target = SDL_CreateTexture (screen::get_renderer(), Info->Format, SDL_TEXTUREACCESS_STREAMING, length()*new_scale, height()*new_scale);
    if (!target) std::cout << "*** surface_sdl::set_scale: " << SDL_GetError() << std::endl;

	
	SDL_LockTexture(target, NULL, (void**)&target_data, &pitch);
	SDL_Surface *target_surf = SDL_CreateRGBSurfaceFrom(target_data, length()*new_scale, height()*new_scale, bpp, pitch, rmask, gmask, bmask, amask);

    s_int32 target_line_length = target_surf->format->BytesPerPixel * length()*new_scale;

    u_int32 real_height = height()*scale();
    u_int32 real_length = length()*scale();

    for (s_int32 src_y = 0; src_y < real_height; src_y += scale())
    {
        s_int32 target_x = 0;
        s_int32 target_x_end = 0;

        // we scale one line horizontally
        for (s_int32 src_x = 0; src_x < real_length; src_x += scale())
        {
        	u_int8 *src = ((Uint8 *) Info->Pixels) + src_y * Info->Pitch + src_x * Info->BytesPerPixel;
            for (target_x_end += new_scale; target_x < target_x_end; ++target_x)
            {
            	u_int8 *dest = target_data + target_x * Info->BytesPerPixel;
            	memcpy(dest, src, Info->BytesPerPixel);
            }
        }

        // the next lines will be the same, so we just copy them
        for (u_int32 i = 1; i < new_scale; i++)
        {
            u_int8 *target_next_line = target_data + target_surf->pitch;
            memcpy (target_next_line, target_data, target_line_length);
            target_data = target_next_line;
        }

        // goto next line
        target_data += target_surf->pitch;
    }

    // scaling complete, now swap the underlying texture
    SDL_DestroyTexture(Surface);
    Surface = target;
    Info->Pixels = target_surf->pixels;
    scale_ = new_scale;

    unlock();
    SDL_FreeSurface(target_surf);
}

surface& surface::operator = (const surface& src)
{
    (drawable&) (*this) = (drawable&) src;
    alpha_channel_ = src.has_alpha_channel();
    is_masked_ = src.is_masked();
    alpha_ = src.alpha();
    scale_ = src.scale();

    if (Surface) SDL_DestroyTexture(Surface);
    if (!src.Surface)
    {
        Surface = NULL;
    }
    else
    {
        int l, h, pitch;
        void *src_pixels, *dst_pixels;

        SDL_QueryTexture(src.Surface, &Info->Format, NULL, &l, &h);
        Surface = SDL_CreateTexture (screen::get_renderer(), Info->Format, SDL_TEXTUREACCESS_STREAMING, l, h);
	    if (!Surface) std::cout << "*** surface_sdl::operator=: " << SDL_GetError() << std::endl;

        SDL_LockTexture(src.Surface, NULL, &src_pixels, &pitch);
        SDL_LockTexture(Surface, NULL, &dst_pixels, &pitch);

        while (h-- > 0)
        {
            memcpy (dst_pixels, src_pixels, pitch);
            src_pixels = (u_int8*) src_pixels + pitch;
            dst_pixels = (u_int8*) dst_pixels + pitch;
        }

        SDL_UnlockTexture(Surface);
    }

    return *this; 
}
 


// Protected methods



void surface::resize (u_int16 l, u_int16 h)
{
    if (l == length () && h == height ()) return;

    if (Surface) SDL_DestroyTexture(Surface);

	set_length (l);
	set_height (h);

	if (l == 0 || h == 0)
	{
		Surface = NULL;
		return;
	}

    // is screen surface initialized?
    if (screen::get_renderer())
    {
#ifdef __BIG_ENDIAN__
        Info->Format = has_alpha_channel() ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_RGB24;
#else
        Info->Format = has_alpha_channel() ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_BGR24;
#endif
        Surface = SDL_CreateTexture (screen::get_renderer(), Info->Format, SDL_TEXTUREACCESS_STREAMING, l * scale(), h * scale());
        if (!Surface) std::cout << "*** surface::resize: " << SDL_GetError() << std::endl;
    }
    else
    {
        Surface = NULL;
        std::cout << "*** surface:resize: screen surface not initialized!" << std::endl;
        exit(1);
    }
}

void surface::clear () 
{
    if (Surface)
    {
        SDL_DestroyTexture(Surface);
        Surface = NULL;
        set_length (0);
        set_height (0); 
        set_alpha (255);
        is_masked_ = false;
        alpha_channel_ = false;
    }
}

void surface::set_data(void * data, u_int16 l, u_int16 h, u_int8 bytes_per_pixel, u_int32 red_mask,
                           u_int32 green_mask, u_int32 blue_mask, u_int32 alpha_mask)
{
    if (Surface) SDL_DestroyTexture(Surface);

    scale_ = 1;

    set_length(l);
    set_height(h);

	if (l == 0 || h == 0)
	{
		Surface = NULL;
		return;
	}

    alpha_channel_ = alpha_mask != 0;

    Info->Format = SDL_MasksToPixelFormatEnum (bytes_per_pixel * 8, red_mask, green_mask, blue_mask, alpha_mask);
    Surface = SDL_CreateTexture (screen::get_renderer(), Info->Format, SDL_TEXTUREACCESS_STREAMING, l, h);
    if (!Surface) std::cerr << "*** surface::set_data: " << SDL_GetError() << std::endl;

    lock(NULL);

    int pitch = bytes_per_pixel * l;
    void *src = data;

    while (h-- > 0)
    {
        memcpy (Info->Pixels, src, pitch);
        src = (u_int8*) src + pitch;
        Info->Pixels = (u_int8*) Info->Pixels + Info->Pitch;
    }

    unlock();

    if (mask_changed_)
    {
    	set_mask(true);
    }
}

void * surface::get_data (u_int8 bytes_per_pixel,
                              u_int32 red_mask, u_int32 green_mask,
                              u_int32 blue_mask, u_int32 alpha_mask) const
{
	if (scale() != 1)
	{
		// always save images without scaling
		surface tmp;
		tmp = *this;
		tmp.set_scale(1);
		return tmp.get_data(bytes_per_pixel, red_mask, green_mask, blue_mask, alpha_mask);
	}

    lock(NULL);

    u_int32 dst_format = SDL_MasksToPixelFormatEnum(bytes_per_pixel*8, red_mask, green_mask, blue_mask, alpha_mask);
    void *dst_pixels = calloc (length() * height(), bytes_per_pixel);
    int dst_pitch = length() * bytes_per_pixel;

    if (dst_format != Info->Format)
    {
        SDL_ConvertPixels(length(), height(), Info->Format, Info->Pixels, Info->Pitch, dst_format, dst_pixels, dst_pitch);
    }
    else
    {
        u_int8 *dest = (u_int8*) dst_pixels;
        int h = height();
        while (h-- > 0)
        {
            memcpy (dest, (u_int8*) Info->Pixels, Info->Pitch);
            Info->Pixels = (u_int8*) Info->Pixels + Info->Pitch;
            dest = dest + dst_pitch;
        }
    }

    return dst_pixels;
}


// Private methods



void surface::setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                           u_int16 sl, u_int16 sh, const drawing_area * draw_to) const
{
    if (draw_to)
    { 
        drawing_area im_zone (x, y, sl, sh);
        im_zone.assign_drawing_area (draw_to);

        SDL_Rect da_int = im_zone.setup_rects ();
        dstrect.x = da_int.x;
        dstrect.y = da_int.y;
        dstrect.w = da_int.w;
        dstrect.h = da_int.h;

        srcrect = dstrect;
        srcrect.x = x < dstrect.x ? sx + dstrect.x - x : sx;
        srcrect.y = y < dstrect.y ? sy + dstrect.y - y : sy;
    }
    else
    {
        srcrect.x = sx;
        srcrect.y = sy;
        srcrect.w = sl;
        srcrect.h = sh;

        dstrect = srcrect;
        dstrect.x = x;
        dstrect.y = y;
    } 
}
