%module landmap

%{

#include "landmap.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

class landmap 
{
public:
  landmap();
  void clear();
  ~landmap();

  landmap& operator =(const landmap& lm);

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
  void update();
}
