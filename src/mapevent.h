#ifndef _mapevent_h
#define _mapevent_h

#include <vector>

class instruction;
class map;
class mapcharacter;

class mapevent : vector<instruction*>
{
  //  static u_int8 (*mapevent::event[256])
  //    (mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);

   vector<instruction*> program;
   u_int16 otherevent_val;

 public:
  mapevent();
  void get(FILE * file);
  void run(mapcharacter*aguy, map*amap, u_int16 x, u_int16 y);
  void set_otherevent(const u_int16&);
  u_int16 otherevent();
};

#endif
