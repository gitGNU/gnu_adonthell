#ifndef WIN_SELECT_H_
#define WIN_SELECT_H_


class win_select{
  
 private:
  list<void *> l_list;
  void * last_select;
  
 public:
  win_select();
  ~win_select();
  void add(void *);
  void remove(void *);
  void * get_select();
  void set_select(void *);
};




#endif
