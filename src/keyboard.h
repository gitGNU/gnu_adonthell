#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

class keyboard
{
 public:
  static bool charkeys[65536];
  static u_int16 last_key;
  static void update_keyboard();
  static u_int16 getkeypressed();
  static bool is_pushed(u_int16 key);
};

#ifdef SDL
int keyboard_init();
#endif

#endif