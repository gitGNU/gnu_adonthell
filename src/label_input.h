#ifndef _WRITE_H__
#define _WRITE_H__

#include "label.h"


class label_input : public label
{
 public:
    
  bool input_update();
  
 protected:
  void insert(const u_int16 pos, const char * letter);

  void insert_char(const u_int16 pos, const char letter);
  
  
};


#endif



