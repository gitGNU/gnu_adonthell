#include "types.h"
#include <vector>
#include <String.h>
#include <stdio.h>

class map;
class mapcharacter;

class instruction
{
   u_int16 type;
   u_int16 param_val[9];
   char *string_val;
   u_int16 otherevent_val;

  public:
  instruction();
  instruction(const int &, String*);
  virtual void exec(mapcharacter*, map*, u_int16, u_int16)=0;
  u_int16 param(int);
  u_int16 otherevent();
  char* string();
  void get(FILE*);

  friend istream& operator>> (istream&, instruction&);
  friend ostream& operator<< (ostream&, instruction&);
};

