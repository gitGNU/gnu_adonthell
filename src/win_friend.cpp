#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_friend.h"
 

void draw_text(u_int16 sx,u_int16 sy,u_int16 ex,u_int16 ey,win_font *font,text_w & txt,drawing_area * da)
{
  u_int16 i=0;
  u_int16 j;
  u_int8 k;
  bool b;
  txt.len_fl=0;
  txt.pos_tmp=txt.pos;
  txt.end_win=false;
  while( (sy+i*(font->height)<ey) &&  (txt.pos_tmp < txt.lenght ) ) { 
    j=0;
    b=true;
    while((b) && ((sx+j)<ex) && (txt.pos_tmp<txt.lenght)) {
      k=txt.text[txt.pos_tmp];
      if(k==' ') 
	j+=font->lenght;
      else { 
	if(k=='\n') b=false;
	else { 
	  font->table[k].putbox_mask(sx+j,sy+i*(font->height),da);
	  j+= font->table[k].length;
	}
      }
      if(i==0) txt.len_fl++;
      txt.pos_tmp++;
    }
    i++;
  }
  txt.end_win=(txt.pos_tmp<txt.lenght);
}







