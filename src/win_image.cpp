#include <list>
#include "types.h"
#include "image.h"
#include "win_base.h"

#include "win_container.h"
#include "win_image.h"


win_image::win_image(u_int16 tx,u_int16 ty,image * tpic,win_container * twc):win_base(tx,ty,0,0,twc,twc->get_drawing_area())
{
  picture=tpic;
  if(picture)
    {
      height=picture->height;
      length=picture->length;
    }
}

void win_image::set_image(image * tpic)
{
  picture=tpic;
  if(picture)
    {
      height=picture->height;
      length=picture->length;
    }
}

void win_image::draw()
{
  if(visible && wc && picture)
    {
      picture->putbox(wc->get_x()+x,wc->get_y()+y);
      draw_border();
    }
}

void win_image::update()
{

}















