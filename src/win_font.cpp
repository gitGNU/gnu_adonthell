#include <stdio.h>
#include <string.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"

win_font::win_font()
{
  table=NULL;
}

win_font::win_font(char * fic)
{
  table=NULL;
  load(fic);
}

win_font::~win_font()
{
  erase();
}
void win_font::erase()
{
  delete [] table;
  table=NULL;
}

void win_font::load(char * rep)
{
  if(table!=NULL) erase();
  FILE *f;
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_FONT_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_FONT_FILE_IDX);
 
 //open file with information about font
  if((f=fopen(path,"rb"))==NULL)
    {printf("%s not found\n",WIN_FONT_FILE_IDX);exit(1);}  

  // load font
  image *font=new image();
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_FONT_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_FONT_FILE_PIC);
  font->load(path);
  //delete [] path;
  //create a table for each letter
  table=new image[NB_TABLE_CHAR];
  char i;int j;
  u_int16 pos,tl;
  j=0;
  while(j<NB_TABLE_CHAR && !feof(f) )
    {
      fread(&i,sizeof(i),1,f);
      fread(&pos,sizeof(pos),1,f);
      fread(&tl,sizeof(tl),1,f);
      table[i].resize(tl,font->height);
      table[i].putbox_part_img(font,0,0,tl,font->height,pos,0);
      j++;
    }
  height=font->height;
  lenght=table['A'].length;
  fclose(f);
}









