#ifndef _mapsquare_h
#define _mapsquare_h

class mapcharacter;
class mapsquare
{
  u_int16 patternnbr;        /* First pattern */
  u_int16 secpatternnbr;     /* Second pattern */
  u_int16 event;             /* Event number (0 if none) */
  u_int16 eventleave;        /* Event when leaving (0 if none) */
  u_int16 item;              /* Item on it? (0 if none) */
  u_int8 flags;                  /* Flags (see below) */
  u_int8 solid;                  /* Solidness options */
  u_int8 alpha;                  /* Translucency levels, 128=neutral */
  u_int8 OCCUPED;                /* Somebody on it? (And who?)*/ 

 public:
  mapsquare();
  void get(FILE * file);
  void put_character(mapcharacter * character);
  void leave_character();
  u_int16 is_occuped();
  u_int16 get_down_pattern_nbr();
  u_int16 get_up_pattern_nbr();
  u_int16 get_eventcomenbr();
  u_int16 get_eventleavenbr();
  bool mapsquare::is_unreachable();
  bool mapsquare::is_others_unreachable();
  bool is_mask();
  bool is_up();
  bool is_trans(u_int8 * alpha_res);
  bool is_solid_up();
  bool is_solid_down();
  bool is_solid_left();
  bool is_solid_right();
  // alpha_res can be NULL if you don't care about the alpha
};

#define PARAM_TRANS 1        /* Translucency*/
#define PARAM_MASK 2         /* Pixmap Mask */
#define PARAM_UP 4           /* Display the second pattern above characters */
#define PARAM_UNREACHABLE 64 /* Can main character go on? */
#define PARAM_OTHERS_UNREACHABLE 128 /* Can others characters go on? */

#define SOLID_LEFT 1
#define SOLID_RIGHT 2
#define SOLID_UP 4
#define SOLID_DOWN 8


#endif
