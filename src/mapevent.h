class map;
class mapcharacter;
class mapevent
{
  //  static u_int8 (*mapevent::event[256])
  //    (mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int16 type;
  u_int16 param1;
  u_int16 param2;
  u_int16 param3;
  u_int16 param4;
  u_int16 param5;
  u_int16 param6;
  u_int16 param7;
  u_int16 param8;
  u_int16 param9;
  char param10[30];            /* Change to dynamic allocation? */
  u_int8 otherevent;           /* u_int16! */

  u_int8 event_0(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_1(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_2(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_3(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_4(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_5(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_6(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_7(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_8(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_9(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_10(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_11(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  u_int8 event_255(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);
  
 public:
  mapevent();
  void get(FILE * file);
  void run(mapcharacter*aguy, map*amap, u_int16 x, u_int16 y);
};
