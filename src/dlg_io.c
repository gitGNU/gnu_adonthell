/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   
   See the COPYING file for more details.
*/  

#include <stdio.h>
#include <stdlib.h>

#include "../map/types.h"

/* Dec -> Hex */ 
void 
d2h (s_int32 d, u_int8 s, FILE * str) 
{
    u_int8 i;

    for (i = 0; i < s; i++)
        fputc ((unsigned char)(d >> (i * 8)), str);
}

/* Hex -> Dec */ 
s_int32 h2d (u_int8 s, FILE * str) 
{
    u_int8 i;
    s_int32 d = 0;
    unsigned char ch = 0;

    for (i = 0; i < s; i++)
    {
        ch = getc (str);
        d += ch << (8 * i);
    }

    return d;
}

/* rs == Read String */ 
char *
rs (u_int32 s, FILE * istr) 
{
    u_int32 size = h2d (s, istr);
    char *str = (char *)malloc (sizeof (char) * (size + 1));
       
    fgets (str, size, istr);
    str[size] = '\0';

    return str;
}

/* ri = Read Int */ 
u_int8 ri (s_int32 * buffer, u_int32 length, u_int8 s, FILE * istr) 
{
    u_int32 i;
        
    for (i = 0; i < length; i++)
    {
        if (feof (istr))
            return 0;
            
        buffer[i] = h2d (4, istr);
    }

    return 1;
}
