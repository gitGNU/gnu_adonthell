%module mapengine

%{

#include "types.h"

%}

%{
#include "mapengine.h"
#include "map.h"
%}

class mapengine
{
 public:
  static void map_engine(landmap * amap);
  static void update_and_show(landmap * amap);
  static void fade_out(landmap * amap, u_int16 depx, u_int16 depy); 
  static void fade_in(landmap * amap, u_int16 depx, u_int16 depy); 
};
