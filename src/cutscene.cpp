/*
 * Cutscene classes: Part of the Adonthell project.
 *
 * This file is covered under the GNU GPL. You know the drill.
 * 
 * (c)2000 Chris Harris
 *
 */
 
#include "cutscene.h"

cutscene::cutscene()
{
  cyclecounter=0;
  keyframes=(keyframe*)calloc(sizeof(keyframe),1);              //First
  keyframes->nextframe=(keyframe*)calloc(sizeof(keyframe),1);   //...and Last
  keyframes->nextframe->prevframe=keyframes;
  keyframes->prevframe=NULL;
  keyframes->nextframe->nextframe=NULL;

  keyframes->imagekeys=(imagekey*)calloc(sizeof(imagekey),1);
  keyframes->nextframe->imagekeys=(imagekey*)calloc(sizeof(imagekey),1);

  keyframes->imagekeys->nextimg=NULL;
  keyframes->nextframe->imagekeys->nextimg=NULL;

  num_keyframes=2;
  current_keyframe_ptr=keyframes;
  num_anims=1;

  initialize_timer(); // Just in case someone forgets :P
}

cutscene::~cutscene()
{

}

void cutscene::initialize_timer(void)
{
  timer = SDL_GetTicks();
}

u_int8 cutscene::insert_keyframe(u_int16 key)
{

  struct keyframe *ptr;
  ptr=keyframes;
  u_int16 x;
  if(key==0)
    { //That would mean putting one at the beginning
      ptr->prevframe=(keyframe*)calloc(sizeof(keyframe),1);
      ptr->prevframe->nextframe=ptr;
      ptr->prevframe->prevframe=NULL;
      ptr->prevframe->imagekeys=(imagekey*)calloc(sizeof(imagekey),1);
      keyframes=ptr->prevframe;
      current_keyframe_ptr=keyframes;
    }else{
      for(x=0;x<key;x++) ptr=ptr->nextframe; //Find the frame to insert before
      ptr->prevframe->nextframe=(keyframe*)calloc(sizeof(keyframe),1);
      ptr->prevframe->nextframe->imagekeys=(imagekey*)calloc(sizeof(imagekey),1);
      ptr->prevframe->nextframe->prevframe=ptr->prevframe->nextframe;
      ptr->prevframe->nextframe->nextframe=ptr;
      ptr->prevframe=ptr->prevframe->nextframe; //swappy swappy !
    }
  
  num_keyframes++;
  
  return(0); // OK, maybe some error checking later ?? 
}

u_int8 cutscene::delete_keyframe(u_int16 key)
{

  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 x;
  for(x=0;x<key;x++) ptr=ptr->nextframe; //Find the frame to be deleted
  ptr2=ptr->imagekeys;
  for(x=0;x<num_anims;x++) 
    {            
      ptr2->anim=NULL;
      ptr2=ptr2->nextimg;
    }

  ptr->prevframe->nextframe=ptr->nextframe;
  ptr->nextframe->prevframe=ptr->prevframe;
  free(ptr);
  num_keyframes--;

  return(0); // OK, maybe some error checking later ??              
}

u_int8 cutscene::insert_imagekey(u_int8 ikey)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 i,j;
  
  for(i=0;i<num_keyframes;i++) 
    {
      ptr2=ptr->imagekeys;
      if(ikey==0)
	{
	  ptr2->previmg=(imagekey*)calloc(sizeof(imagekey),1);
	  ptr2->previmg->nextimg=ptr2;
	  ptr2->previmg->previmg=NULL;
	  ptr->imagekeys=ptr2->previmg;
	  ptr=ptr->nextframe;
	}else{
	  for(j=0;j<ikey;j++) ptr2=ptr2->nextimg;
	  ptr2->previmg->nextimg=(imagekey*)calloc(sizeof(imagekey),1);
	  ptr2->previmg=ptr2->previmg->nextimg;
	  ptr2->previmg->nextimg=ptr2;
	  ptr=ptr->nextframe;
	}
    }

  num_anims++;
    
  return(0);
}

u_int8 cutscene::delete_imagekey(u_int8 ikey)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  u_int16 x;
  ptr=keyframes;
  for(x=0;x<num_keyframes;x++) 
    {
      ptr2=ptr->imagekeys;
      for(x=0;x<ikey;x++) ptr2=ptr2->nextimg;
      ptr2->previmg->nextimg=ptr2->nextimg;
      ptr2->nextimg->previmg=ptr2->previmg;
      free(ptr2);
      ptr=ptr->nextframe; 
    }

  num_anims--;

  return(0);
  
}

u_int8 cutscene::swap_imagekey(u_int8 ikeya, u_int8 ikeyb)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  struct imagekey *ptr3;
  u_int16 x;
  ptr=keyframes;
  for(x=0;x<num_keyframes;x++) 
    {
      ptr2=ptr->imagekeys;
      ptr3=ptr->imagekeys;
      for(x=0;x<ikeya;x++) ptr2=ptr2->nextimg;
      for(x=0;x<ikeyb;x++) ptr3=ptr3->nextimg;
      ptr2->previmg->nextimg=ptr3;
      ptr2->nextimg->previmg=ptr3;
      ptr3->previmg->nextimg=ptr2;
      ptr3->nextimg->previmg=ptr2;
    }

  return(0); // Sure, it worked. Why check.
	
}

u_int8 cutscene::render_scene()
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  struct imagekey *ptr3;
  u_int16 i,j;
  s_int16 x,y;

  if(cyclecounter>=current_keyframe_ptr->cycles)
    {
      current_keyframe_ptr=current_keyframe_ptr->nextframe;
      cyclecounter=1;
    }

  ptr=current_keyframe_ptr->nextframe;

  if(current_keyframe_ptr->nextframe==NULL) return(1);

  //  ptr2=current_keyframe_ptr->imagekeys;
  //  ptr3=ptr->imagekeys;
  for(i=0;i<num_anims;i++)
    {
      ptr2=current_keyframe_ptr->imagekeys;
      ptr3=ptr->imagekeys;
      for(j=0;j<i;j++) 
	{
	  ptr2=ptr2->nextimg;
	  ptr3=ptr3->nextimg;
	}
      if(ptr2->visible==1) 
	{ 
	  if((cyclecounter%ptr2->animspeed)==0) ptr2->anim->next_frame();
	  x=(ptr2->x+(((ptr3->x-ptr2->x)*cyclecounter)/current_keyframe_ptr->cycles));
	  y=(ptr2->y+(((ptr3->y-ptr2->y)*cyclecounter)/current_keyframe_ptr->cycles));
	  //  fprintf(stdout, "cyclecount: %d, x: %d, y: %d\n",cyclecounter,x,y);
	  ptr2->anim->draw(x,y);
	}
    }
  cyclecounter += SDL_GetTicks() - timer;
  timer = SDL_GetTicks();

  return(0);
}
 
u_int8 cutscene::load(char * fname)
{
  int x,y;
  u_int16 tmp_num_keyframes;
  u_int8  tmp_num_anims;
  struct keyframe *ptr1=keyframes;
  struct imagekey *ptr2;

  FILE * file=fopen(fname,"r");
  if(!file) return(1);

  // number of keyframes
  fread(&tmp_num_keyframes,sizeof(u_int16),1,file);

  // number of anims
  fread(&tmp_num_anims,sizeof(u_int8),1,file);
   
  for(x=0;x<tmp_num_keyframes-2;x++) { this->insert_keyframe(0); }
  for(x=0;x<tmp_num_anims-1;x++) { this->insert_imagekey(0); }

  // keyframes
  ptr1=keyframes;

  for(x=0;x<num_keyframes;x++)
    {
      fread(&ptr1->cycles,sizeof(s_int32),1,file);
      ptr1=ptr1->nextframe;
    }

  ptr1=keyframes;

  // imagekeys
  for(x=0;x<num_keyframes;x++)
    {
      ptr2=ptr1->imagekeys;
      for(y=0;y<num_anims;y++)
	{
	  fread(&ptr2->x,sizeof(s_int16),1,file);
	  fread(&ptr2->y,sizeof(s_int16),1,file);
	  fread(&ptr2->visible,sizeof(bool),1,file);
	  fread(&ptr2->animspeed,sizeof(u_int16),1,file);
	  ptr2=ptr2->nextimg;
	}
      ptr1=ptr1->nextframe;
    }

  fclose(file);
  return(0);
}

u_int8 cutscene::save(FILE *file)
{
  int x,y;
  struct keyframe *ptr1=keyframes;
  struct imagekey *ptr2;

  // number of keyframes
  fwrite(&num_keyframes,sizeof(u_int16),1,file);

  // number of anims
  fwrite(&num_anims,sizeof(u_int8),1,file);
  
  // keyframes
  for(x=0;x<num_keyframes;x++)
    {
      fwrite(&ptr1->cycles,sizeof(s_int32),1,file);
      ptr1=ptr1->nextframe;
    }

  fprintf(stderr, "Test one\n");

  ptr1=keyframes;

  // imagekeys
  for(x=0;x<num_keyframes;x++)
    {
      ptr2=ptr1->imagekeys;
      for(y=0;y<num_anims;y++)
	{
	  fwrite(&ptr2->x,sizeof(s_int16),1,file);
	  fwrite(&ptr2->y,sizeof(s_int16),1,file);
	  fwrite(&ptr2->visible,sizeof(bool),1,file);
	  fwrite(&ptr2->animspeed,sizeof(u_int16),1,file);
	  ptr2=ptr2->nextimg;
	}
      ptr1=ptr1->nextframe;
    }

  return(0);
}

void cutscene::set_cycles(u_int16 key, u_int16 cycles)
{
  struct keyframe *ptr;
  ptr=keyframes;
  u_int16 i;
  for(i=0;i<key;i++) ptr=ptr->nextframe; //Find 
  ptr->cycles=cycles;
}
 
void cutscene::set_imagekey_anim(u_int8 img, animation * a)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 i,j;
  for(i=0;i<num_keyframes;i++) 
    {
      ptr2=ptr->imagekeys;
      for(j=0;j<img;j++) ptr2=ptr2->nextimg;
      ptr2->anim=a;
      ptr=ptr->nextframe; //Find
    }
}
 
void cutscene::set_imagekey_animspeed(u_int16 key, u_int8 img, u_int16 as)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 i,j;
  for(i=0;i<key;i++) ptr=ptr->nextframe; //Find    
  ptr2=ptr->imagekeys;
  for(j=0;j<img;j++) ptr2=ptr2->nextimg;
  ptr2->animspeed=as;
}
 
void cutscene::set_imagekey_visible(u_int16 key, u_int8 img, bool visible)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 i,j;
  for(i=0;i<key;i++) ptr=ptr->nextframe; //Find    
  ptr2=ptr->imagekeys;
  for(j=0;j<img;j++) ptr2=ptr2->nextimg;
  ptr2->visible=visible;
}

void cutscene::set_coordinates(u_int16 key, u_int8 img, s_int16 x, s_int16 y)
{
  struct keyframe *ptr;
  struct imagekey *ptr2;
  ptr=keyframes;
  u_int16 i,j;
  for(i=0;i<key;i++) ptr=ptr->nextframe;
  ptr2=ptr->imagekeys;
  for(j=0;j<img;j++) ptr2=ptr2->nextimg;
  ptr2->x=x;
  ptr2->y=y;
    
}

u_int16 cutscene::get_current_keyframe()
{
  return(current_keyframe);
}

void cutscene::set_current_keyframe(u_int16 key)
{
  current_keyframe=key;
}
