/*
   $Id$

   Copyright (C) 1999 - 2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "mapobject.h"
#include "fileops.h"

#ifdef _DEBUG_
u_int16 mapobject::a_d_diff;
#endif

void mapobject::init()
{
  length_=height_=0;
#ifdef _EDIT_
  in_editor=false;
  strcpy(file_name,"");
#endif
#ifdef USE_PYTHON
  schedule=NULL;
#endif
  schedule_activated=false;
}

void mapobject::clear()
{
  u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    delete anim[i];
  anim.clear();
  init();
}

mapobject::mapobject() : maptpl(0,0,1,1,9,9)
{
  init();
#ifdef _DEBUG_
  cout << "mapobject() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

mapobject::~mapobject()
{
  clear();
#ifdef _DEBUG_
  cout << "~mapobject() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

void mapobject::play()
{
  u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    anim[i]->play();
}

void mapobject::stop()
{
  u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    anim[i]->stop();
}

void mapobject::rewind()
{
  u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    anim[i]->rewind();
}

mapobject &mapobject::operator =(const mapobject &mo)
{
  u_int16 i,j;
  posx=mo.posx;
  posy=mo.posy;
  length_=mo.length_;
  height_=mo.height_;
  basex=mo.basex;
  basey=mo.basey;
#ifdef _EDIT_
  currentpart=mo.currentpart;
#endif
  for(i=0;i<maptpl::length();i++)
    delete[] placetpl[i];
  delete[] placetpl;
  maptpl::set_length(mo.maptpl::length_);
  maptpl::set_height(mo.maptpl::height_);
  placetpl=new (mapsquaretpl*)[maptpl::length()];
  for(i=0;i<maptpl::length();i++)
   { 
     placetpl[i]=new mapsquaretpl[maptpl::height()];
     for(j=0;j<maptpl::height();j++)
       placetpl[i][j].walkable=mo.placetpl[i][j].walkable;
   }
  for(i=0;i<nbr_of_animations();i++)
    delete anim[i];
  anim.clear();
  for(i=0;i<mo.anim.size();i++)
    anim.push_back(NULL);
  for(i=0;i<nbr_of_animations();i++)
    {
      anim[i]=new animation;
      *(anim[i])=*(mo.anim[i]);
    }
  return *this;
}

void mapobject::update()
{
  static u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    anim[i]->update();
#ifdef USE_PYTHON
  if(schedule && schedule_activated) 
    PyEval_EvalCode(schedule,data::globals,NULL);
#endif
}

void mapobject::draw_free(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  static u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    anim[i]->draw(x,y,da_opt);
}

void mapobject::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  draw_free(x-basex*MAPSQUARE_SIZE,y-basey*MAPSQUARE_SIZE,da_opt);
}

void mapobject::draw_border_free(u_int16 x, u_int16 y, 
				 drawing_area * da_opt=NULL)
{
  screen::drawbox(x,y,length(),1,0xFFFFFF,da_opt);
  screen::drawbox(x,y+height(),length(),1,0xFFFFFF,da_opt);
  screen::drawbox(x+length(),y,1,height()+1,0xFFFFFF,da_opt);
  screen::drawbox(x,y,1,height()+1,0xFFFFFF,da_opt);
}

void mapobject::draw_border(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  draw_border_free(x-basex*MAPSQUARE_SIZE,y-basey*MAPSQUARE_SIZE,da_opt);
}

void mapobject::calculate_dimensions()
{
  u_int16 i;
  length_=0;
  height_=0;
  for(i=0;i<nbr_of_animations();i++)
    {
      u_int16 tl,th;
      if((tl=anim[i]->length()+anim[i]->xoffset())>length())
	length_=tl;
      
      if((th=anim[i]->height()+anim[i]->yoffset())>height())
	height_=th;
    }
}

s_int8 mapobject::get(gzFile file)
{
  u_int16 i;
  u_int16 nbr_of_parts;
  anim.clear();
  gzread(file,&nbr_of_parts,sizeof(nbr_of_parts));
#ifdef _EDIT_
  currentpart=0;
#endif
  for(i=0;i<nbr_of_parts;i++)
    {
      anim.push_back(new animation);
      anim.back()->get_off(file);
      anim.back()->play();
    }
  maptpl::get(file);
  resize_view(9,9);
  calculate_dimensions();
  return 0;
}

s_int8 mapobject::load(const char * fname)
{
  gzFile file;
  s_int8 retvalue = -1;
  char fdef[strlen(fname)+strlen(MAPOBJECTS_DIR)+1];
  strcpy(fdef,MAPOBJECTS_DIR);
  strcat(fdef,fname);
  file=gzopen(fdef,"rb"); 
  if(!file) return -1;
  if(fileops::get_version (file, 1, 1, fdef))
    retvalue=get(file);
  gzclose(file);
  return retvalue;
}

s_int8 mapobject::insert_animation(animation * an, u_int16 pos)
{
  vector<animation*>::iterator i;
  if(pos>nbr_of_animations()) return -2;
  i=anim.begin();
  while(pos--) i++;
  anim.insert(i,an);
  an->play();
#ifdef _DEBUG_
  cout << "Added animation: " << nbr_of_animations() << " total in mapobject.\n";
#endif
  return 0;
}

s_int8 mapobject::delete_animation(u_int16 pos)
{
  vector<animation*>::iterator i;

  if(pos>nbr_of_animations()-1) return -2;
  i=anim.begin();
  while(pos--) i++;
  anim.erase(i);
#ifdef _DEBUG_
  cout << "Removed part: " << nbr_of_animations() << " total in mapobject.\n";
#endif
#ifdef _EDIT_
  if(currentpart>=nbr_of_animations()) currentpart=nbr_of_animations()-1;
  if(in_editor) 
    {
      must_upt_label_part=true;
    }
  if(!nbr_of_animations()) 
    {
      currentpart=0;
    }
#endif
  return 0;
}

#ifdef USE_PYTHON
void mapobject::set_schedule(char * file)
{
  char script[255];
  strcpy (script, "scripts/schedules/");
  strcat (script, file);
  strcat (script, ".py");
  
  FILE *f = fopen (script, "r");
  
  // See whether the script exists at all
  if (f)
    {
      // Compile the script into a PyCodeObject for quicker execution
      _node *n = PyParser_SimpleParseFile (f, script, Py_file_input);
      if (n)
        {
	  // If no errors occured update schedule code ...
	  if (schedule) delete schedule;
	  schedule = PyNode_Compile (n, file);
	  PyNode_Free (n);
	}
      else
        {
	  cout << "\n*** Cannot set schedule: Error in" << flush;
	  show_traceback ();
        }
      fclose (f);
    }
  else cout << "\n*** Cannot open schedule: file \"" << script
	    << "\" not found!" << flush;
}
#endif

#ifdef _EDIT_
s_int8 mapobject::put(gzFile file)
{
  u_int16 i;
  u_int16 nbr_of_parts=anim.size();
  // version information 
  fileops::put_version (file, 1);

  gzwrite(file,&nbr_of_parts,sizeof(nbr_of_parts));
  for(i=0;i<nbr_of_parts;i++)
    anim[i]->put_off(file);
  maptpl::put(file);
  return 0;
}

s_int8 mapobject::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  char fdef[strlen(fname)+strlen(MAPOBJECTS_DIR)+1];
  strcpy(fdef,MAPOBJECTS_DIR);
  strcat(fdef,fname);
  file=gzopen(fdef,"wb6"); 
  if(!file) return -1;
  retvalue=put(file);
  gzclose(file);
  return retvalue;
}
#endif


#ifdef _EDIT_

void mapobject::zoom(u_int16 sx, u_int16 sy, mapobject * src)
{
  u_int16 i;
  clear();
  for(i=0;i<src->nbr_of_animations();i++)
    {
      animation * an=new animation;
      an->zoom((src->anim[i]->length()*sx)/src->length(),
		   (src->anim[i]->height()*sy)/src->height(),src->anim[i]);
      an->set_offset((src->anim[i]->xoffset()*sx)/src->length(),(src->anim[i]->yoffset()*sy)/src->height());
      an->play();
      anim.push_back(an);
    }
}

void mapobject::zoom_to_fit(u_int16 sm, mapobject * src)
{
  u_int16 nl,nh,m;
  m=src->length()>=src->height()?src->length():src->height();
  if(m<sm)
    {
      nl=src->length();
      nh=src->height();
    }
  else
    {
      if(m==src->length())
	{
	  nl=sm;
	  nh=(src->height()*sm)/src->length();
	}
      else
	{
	  nh=sm;
	  nl=(src->length()*sm)/src->height();
	}
    }
  zoom(nl,nh,src);
}

void mapobject::init_parts()
{
  u_int16 i;
  for(i=0;i<nbr_of_animations();i++)
    {
      anim[i]->rewind();
      anim[i]->play();
    }
}

u_int16 mapobject::increase_part(u_int16 c)
{
  c++;
  if(c==nbr_of_animations()) c=0;
  return c;
}

u_int16 mapobject::decrease_part(u_int16 c)
{
  if(c==0) c=nbr_of_animations()-1;
  else c--;
  return c;
}

void mapobject::set_currentpart(u_int16 p)
{
  currentpart=p;
  must_upt_label_part=true;
}

void mapobject::set_part_xoffset(u_int16 p, s_int16 xoff)
{
  if(xoff<0) return;
  anim[p]->set_offset(xoff,anim[p]->yoffset());
  must_upt_label_part=true;
}

void mapobject::set_part_yoffset(u_int16 p, s_int16 yoff)
{
  if(yoff<0) return;
  anim[p]->set_offset(anim[p]->xoffset(),yoff);
  must_upt_label_part=true;
}

void mapobject::update_label_part()
{
  if(nbr_of_animations())
    {
      sprintf(label_txt,"Current part: %d/%d\nOffset: %d, %d\n"
	      "Obj. size: %d, %d\n%s",
	      currentpart,nbr_of_animations(),anim[currentpart]->xoffset(), 
	      anim[currentpart]->yoffset(),length(),height(),
	      anim[currentpart]->is_empty()?"Part is empty\n":"");
      if(show_grid)
	{
	  static char tmps[300];
	  sprintf(tmps,"\nCursor:\nX: %d\nY: %d\n"
		  "\nObject size:\nLength: %d\nHeight: %d",
		  posx,posy,length(),height());
	  strcat(label_txt,tmps);
	}
    }
  else
    sprintf(label_txt,"No parts yet!");
  label_part->set_text(label_txt);
  must_upt_label_part=false;
}

void mapobject::quick_save()
{
  char s[500];
  if(!strcmp(file_name,""))
    {
      sprintf(s,"You should save (F5) before calling this...");
    }
  else if(save(file_name))
    {
      sprintf(s,"Error saving %s!",file_name);
    }
  else
    {
      sprintf(s,"%s saved successfully!",file_name);
    }
  set_info_win(s);      
}

void mapobject::quick_load()
{
  char s[500];
  if(!strcmp(file_name,""))
    {
      sprintf(s,"You should load (F6) before calling this...");
    }
  else if(load(file_name))
    {
      sprintf(s,"Error saving %s!",file_name);
    }
  else
    {
      sprintf(s,"%s loaded successfully!",file_name);
    }
  set_info_win(s);      
}

void mapobject::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save mapobject as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapobject::update_editor),
			     makeFunctor(*this,
					 &mapobject::draw_editor));
  if(!s) return;
  char st[500];
  if(save(s))
    {
      sprintf(st,"Error saving %s!",s);
    }
  else sprintf(st,"%s saved successfully!",s);
  strcpy(file_name,s);
  set_info_win(st);
  delete qw;
}

void mapobject::load()
{
  mapobject * t=new mapobject;
  win_file_select * wf=new win_file_select(60,20,200,200,th,font,".mobj",
					   MAPOBJECTS_DIR);
  char * s=wf->wait_for_select(makeFunctor(*this,&mapobject::update_editor),
			       makeFunctor(*this,&mapobject::draw_editor));
  if(!s)
    {
      delete t;
      delete wf;
      return;
    }
  char st[500];
  if(t->load(s)) 
    {
      sprintf(st,"Error loading %s!",s);
    }
  else 
    {
      sprintf(st,"%s loaded successfully!",s);
      currentpart=0;
      *(mapobject*)this=*t;
    }
  strcpy(file_name,s);
  set_info_win(st);
  delete t;
  delete wf;
  must_upt_label_part=true;
}

void mapobject::new_animation()
{
  animation * a=new animation;
  u_int16 p;
  do
    {
      char tmp[255];
      char * s2;
      sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_animations(),
	      nbr_of_animations());
      win_query * qw2=new win_query(70,40,th,font,tmp);
      s2=qw2->wait_for_text(makeFunctor(*this,
				       &mapobject::update_editor),
			   makeFunctor(*this,
				       &mapobject::draw_editor));
      if(!s2)
	{ 
	  delete qw2;
	  delete a;
	  return;
	}
      if(!s2[0]) p=nbr_of_animations();
      else p=atoi(s2);
      delete qw2;
    }
  while(p>nbr_of_animations());
  insert_animation(a,p);
  anim[p]->editor();
  init_parts();
  must_upt_label_part=true;
}

void mapobject::add_animation()
{
  animation * a=new animation;
  u_int16 p;
  win_file_select * wf=new win_file_select(60,20,200,200,th,font,".anim");
  char * s=wf->wait_for_select(makeFunctor(*this,&mapobject::update_editor),
			       makeFunctor(*this,&mapobject::draw_editor));
  if(!s) return;
  char st[500];
  if(a->load(s))
    {
      sprintf(st,"Error loading %s!",s);
      delete wf;
      delete a;
      return;
    }
  sprintf(st,"%s loaded successfully!",s);
  do
    {
      char tmp[255];
      char * s2;
      sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_animations(),
	      nbr_of_animations());
      win_query * qw2=new win_query(70,40,th,font,tmp);
      s2=qw2->wait_for_text(makeFunctor(*this,
				       &mapobject::update_editor),
			   makeFunctor(*this,
				       &mapobject::draw_editor));
      if(!s2)
	{ 
	  delete qw2;
	  delete wf;
	  delete a;
	  return;
	}
      if(!s2[0]) p=nbr_of_animations();
      else p=atoi(s2);
      delete qw2;
    }
  while(p>nbr_of_animations());
  insert_animation(a,p);
  delete wf;
  init_parts();
  must_upt_label_part=true;
}

void mapobject::set_part_offset(u_int16 partnbr, u_int16 x, u_int16 y)
{
  anim[partnbr]->set_offset(x,y);
}

void mapobject::resize_grid()
{
  u_int16 mx=0,my=0;
  u_int16 rx,ry;
  u_int16 i;
  if(!nbr_of_animations()) return;
  for(i=0;i<nbr_of_animations();i++)
    {
      if(mx<anim[i]->length()+anim[i]->xoffset()) mx=anim[i]->length()+anim[i]->xoffset();
      if(my<anim[i]->height()+anim[i]->yoffset()) my=anim[i]->height()+anim[i]->yoffset();
    }
  rx=mx/20+(mx%20!=0);
  ry=my/20+(my%20!=0);
  if(rx==maptpl::length() && ry==maptpl::height()) return;
  resize(rx,ry);
  resize_view(9,9);
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_CTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

enum dir{left, right, up, down};

inline void move_part(u_int16 p)
{
 
}

void mapobject::update_editor_keys()
{
  // Switch grid mode on/off
  if(input::has_been_pushed(SDLK_g))
    {show_grid=show_grid==true?false:true; must_upt_label_part=true;}

  // Save
  if(input::has_been_pushed(SDLK_F5))
    { 
      if(SDL_GetModState()&KMOD_SHIFT)
	quick_save();
      else save(); 
    }
  // Load
  if(input::has_been_pushed(SDLK_F6))
    { 
      if(SDL_GetModState()&KMOD_SHIFT)
	quick_load();
      else load(); 
    }
  
  // Add an animation to the mapobject
  if(input::has_been_pushed(SDLK_F1))
    { add_animation(); }
  // Delete selected animation
  if(input::has_been_pushed(SDLK_F2))
    { delete_animation(currentpart); }
  // Creates a new animation and let you edit it
  if(input::has_been_pushed(SDLK_F3))
    { new_animation(); }

  if(!nbr_of_animations()) return;

  // Navigate through animations
  // + Shift: change the position of the selected animation
  if(testkey(SDLK_KP_PLUS))
    if(SDL_GetModState()&KMOD_SHIFT)
      {
	if(currentpart<nbr_of_animations()-1)
	  {
	    animation * tmp;
	    tmp=anim[currentpart];
	    anim[currentpart]=anim[currentpart+1];
	    anim[currentpart+1]=tmp;
	    currentpart++;
	    must_upt_label_part=true;
	  }
      }
    else set_currentpart(increase_part(currentpart));
  
  // Navigate through animations
  // + Shift: change the position of the selected animation
  if(testkey(SDLK_KP_MINUS))
    if(SDL_GetModState()&KMOD_SHIFT)
      {
	if(currentpart>0)
	  {
	    animation * tmp;
	    tmp=anim[currentpart];
	    anim[currentpart]=anim[currentpart-1];
	    anim[currentpart-1]=tmp;
	    currentpart--;
	    must_upt_label_part=true;
	  }
      }
    else set_currentpart(decrease_part(currentpart));
  
  // Edit selected animation
  if(input::has_been_pushed(SDLK_RETURN))
    {
      anim[currentpart]->editor();
      init_parts();
    }

  // If grid on:
  // changes the selected square
  // + Shift: sets the current animation's offset
  // + Alt: Resize the grid
  if(testkey(SDLK_LEFT) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[currentpart]->xoffset())
	    set_part_xoffset(currentpart,anim[currentpart]->xoffset()-1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ 
	  if((maptpl::length()-1)*MAPSQUARE_SIZE>=
	     anim[currentpart]->length()+anim[currentpart]->xoffset())
	    maptpl::resize(maptpl::length()-1,maptpl::height()); 
	}
      else
	{ move_cursor_left(); must_upt_label_part=true;}
    }

  // If grid on:
  // changes the selected square
  // + Shift: sets the current animation's offset
  // + Alt: Resize the grid
  if(testkey(SDLK_RIGHT) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[currentpart]->xoffset()+anim[currentpart]->length()<
	     maptpl::length()*MAPSQUARE_SIZE)
	    set_part_xoffset(currentpart,anim[currentpart]->xoffset()+1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ maptpl::resize(maptpl::length()+1,maptpl::height()); }
      else
	{ move_cursor_right(); must_upt_label_part=true;}
    }

  // If grid on:
  // changes the selected square
  // + Shift: sets the current animation's offset
  // + Alt: Resize the grid
  if(testkey(SDLK_UP) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[currentpart]->yoffset())
	    set_part_yoffset(currentpart,anim[currentpart]->yoffset()-1);
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ 
	  if((maptpl::height()-1)*MAPSQUARE_SIZE>=
	     anim[currentpart]->height()+anim[currentpart]->yoffset())
	    maptpl::resize(maptpl::length(),maptpl::height()-1); 
	}
      else
	{ move_cursor_up(); must_upt_label_part=true;}
    }

  // If grid on:
  // changes the selected square
  // + Shift: sets the current animation's offset
  // + Alt: Resize the grid
  if(testkey(SDLK_DOWN) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[currentpart]->yoffset()+anim[currentpart]->height()<
	     maptpl::height()*MAPSQUARE_SIZE)
	    set_part_yoffset(currentpart,anim[currentpart]->yoffset()+1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ maptpl::resize(maptpl::length(),maptpl::height()+1); }
      else
	{ move_cursor_down(); must_upt_label_part=true;}
    }

  // If grid on:
  // Sets whether a square is walkable from left or not.
  if(input::has_been_pushed(SDLK_x))
    if(show_grid)
      placetpl[posx][posy].set_walkable_west
	(placetpl[posx][posy].is_walkable_west()?false:true);
  // If grid on:
  // Sets whether a square is walkable from right or not.
  if(input::has_been_pushed(SDLK_v))
    if(show_grid)
      placetpl[posx][posy].set_walkable_east
	(placetpl[posx][posy].is_walkable_east()?false:true);
  // If grid on:
  // Sets whether a square is walkable from south or not.
  if(input::has_been_pushed(SDLK_c))
    if(show_grid)
      placetpl[posx][posy].set_walkable_south
	(placetpl[posx][posy].is_walkable_south()?false:true);
  // If grid on:
  // Sets whether a square is walkable from north or not.
  if(input::has_been_pushed(SDLK_d))
    if(show_grid)
      placetpl[posx][posy].set_walkable_north
	(placetpl[posx][posy].is_walkable_north()?false:true);

  // If grid on:
  // Sets the current square as the base square.
  if(input::has_been_pushed(SDLK_b))
    if(show_grid)
      set_base_tile(posx,posy);
}

void mapobject::update_editor()
{
  update();
  maptpl::update();
  if(info_win_count)
    {
      if(info_win_count==1)
	{
	  info_win->set_border_visible(true);
	  info_win->set_background_visible(true);
	  info_win->set_visible_all(true);
	}
      info_win->update();
      info_win_count++;
      if(info_win_count==200) 
	{
	  info_win->set_border_visible(false);
	  info_win->set_background_visible(false);
	  info_win->set_visible_all(false);
	  info_win_count=0;
	}
    }
}

void mapobject::draw_editor()
{
  bg->draw(0,0);
  if(nbr_of_animations())
    {
      draw_free(0-(MAPSQUARE_SIZE*d_posx),0-(MAPSQUARE_SIZE*d_posy),da);
      if(!anim[currentpart]->is_empty())
	anim[currentpart]->draw_border(0-(MAPSQUARE_SIZE*d_posx),
				      0-(MAPSQUARE_SIZE*d_posy),da);
    }
  calculate_dimensions();
  if(must_upt_label_part) update_label_part();
  if(show_grid) maptpl::draw();
  container->draw();

  info_win->draw();
}

void mapobject::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do();i++) update_editor();
  draw_editor();
}

void mapobject::set_info_win(char * text)
{
  info_win_label->set_text(text);
  info_win_label->set_auto_size(true);
  info_win->set_align_all(WIN_ALIGN_CENTER);
  info_win_count=1;
}

void mapobject::editor()
{
  image temp;
  string t;
  t=WIN_DIRECTORY;
  t+=WIN_BACKGROUND_DIRECTORY;
  t+=WIN_THEME_ORIGINAL;
  t+=WIN_BACKGROUND_FILE;
  temp.load_pnm(t.data());
  bg=new image(320,240);
  bg->putbox_tile_img(&temp);
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);
  container=new win_container(200,12,110,216,th);
  label_status=new win_label(5,5,100,30,th,font);
  label_part=new win_label(5,70,100,120,th,font);
  container->add(label_status);
  container->add(label_part);
  container->set_border_visible(true);
  container->set_visible_all(true);
  currentpart=0;

  info_win=new win_container(20,10,280,20,th);
  info_win_label=new win_label(0,5,0,0,th,font);
  info_win->add(info_win_label);
  info_win_count=0;
  info_win->set_border_size(WIN_SIZE_MINI);
  info_win->set_border_visible(false);
  info_win->set_background_visible(false);
  info_win->set_visible_all(false);

  must_upt_label_part=true;
  show_grid=false;
  in_editor=true;
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      static u_int16 i;
      input::update();
      //      resize_grid();
      for(i=0;i<screen::frames_to_do();i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }
  in_editor=false;
  delete container;
  delete info_win;
  delete bg;
  delete th;
  delete font;
}
#endif
