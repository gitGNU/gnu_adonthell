/*
   $Id$

   Copyright (C) 2000-2001 Michael Smith <msmith@labyrinth.net.au>
   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>

   Generously borrowed from oggedit v0.2, an OggVorbis Tag Editor

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __VCEDIT_H
#define __VCEDIT_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>

typedef size_t (*vcedit_read_func)(void *, size_t, size_t, void *);
typedef size_t (*vcedit_write_func)(const void *, size_t, size_t, void *);

typedef struct {
	ogg_sync_state		*oy;
	ogg_stream_state	*os;

	vorbis_comment		*vc;

	vcedit_read_func read;
	vcedit_write_func write;

	void		*in;
	long		serial;
	unsigned char	*mainbuf;
	unsigned char	*bookbuf;
	int		mainlen;
	int		booklen;
	char 	    *lasterror;
} vcedit_state;

extern vcedit_state *	vcedit_new_state(void);
extern void		vcedit_clear(vcedit_state *state);
extern vorbis_comment *	vcedit_comments(vcedit_state *state);
extern int		vcedit_open(vcedit_state *state, FILE *in);
extern int		vcedit_open_callbacks(vcedit_state *state, void *in,
		vcedit_read_func read_func, vcedit_write_func write_func);
extern int		vcedit_write(vcedit_state *state, void *out);
extern char *   vcedit_error(vcedit_state *state);

#ifdef __cplusplus
}
#endif 

#endif /* __VCEDIT_H */

