s_int8 image::simpleget(SDL_RWops * file)
{
  void * tmp;

  tmp=read_pnm(file,&length,&height);
  // Pitch value= Surface length * bytes_per_pixel
  data=SDL_CreateRGBSurfaceFrom(tmp,length,height,24,
				length*3,0x0000FF,0x00FF00,0xFF0000,0);
  if (!data) return(-1);
  return(0);
}

s_int8 image::simpleput(SDL_RWops * file)
{
  put_pnm(file,data->pixels,length,height);
  return(0);
}

s_int8 image::simpleload(char * fname)
{
  s_int8 res;
  SDL_RWops * file=SDL_RWFromFile(fname,"r");
  if(!file) return(-1);
  res=simpleget(file);
  SDL_RWclose(file);
  return(res);
}
