#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"
#include "pnm.h"

void pnm_gotonextline(FILE * file)
{
  char buff;
  do
    {
      fread(&buff,1,1,file);
    }while(buff!='\n');
}

int pnm_checkforcomment(FILE * file)
{
  char buff;
  fread(&buff,1,1,file);
  if(buff=='#') {pnm_gotonextline(file); return(1);}
  else {fseek(file,-1,SEEK_CUR);return(0);}
}

void * read_pnm(FILE * file, u_int16 * lenght, u_int16 * height)
{
  void * image;
  char sign[10];
  u_int16 l,h;
  u_int32 i=0;
  fread(sign,2,1,file);
  if ((sign[0]!='P')||(sign[1]!='6')) {printf("Invalid format.\n");return(NULL);}
  pnm_gotonextline(file);
  /* Getting height and lenght */
  while(pnm_checkforcomment(file));
  do
    {
      fread(&sign[i],1,1,file);
      i++;
    }while(sign[i-1]!=' ');
  sign[i-1]=0;
  l=atoi(sign);
  i=0;
  do
    {
      fread(&sign[i],1,1,file);
      i++;
    }while(sign[i-1]!='\n');
  sign[i-1]=0;
  h=atoi(sign);
  /* Going to next line */
  pnm_gotonextline(file);
  /* Reading the image */
  image=calloc((l)*(h),3);
  fread(image,3,(l)*(h),file);
  if(lenght) *lenght=l;
  if(height) *height=h;
  return(image);
}

void put_pnm(FILE * file, void * image, u_int16 lenght, u_int16 height)
{
  fprintf(file,"P6\n%d %d\n255\n",lenght,height);
  fwrite(image,lenght*height*3,1,file);
}
 
int write_pnm(char * filename, void * image, u_int16 lenght, u_int16 height)
{
  FILE * file;
  file=fopen(filename,"w"); 
  if(!file) return(1);
  else
    {
      put_pnm(file,image,lenght,height);
      fclose(file);
      return(0);
    }
}
