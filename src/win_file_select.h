#ifndef _WIN_FILE_SELECT_H_
#define _WIN_FILE_SELECT_H_

class win_select;
class win_font;
class win_label;
class win_write;

#include <string> 

class win_file_select:public win_container
{
protected:
    win_label * curdir_;
    win_select *combo_file_;
    win_font *font_;

    char base_dir_[500];
    char cur_dir_[500];
    char cur_file_[100];
    char ext[50];
    void list_directory (char *ext);

public:
	
    win_file_select (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                     win_theme * theme, win_font * wf, char *fext, char *bdir = NULL);
    ~win_file_select ();
    void resize (u_int16, u_int16);
    void set_curdirectory (char *);

    string wait_for_select (const Functor0 & updatefunc,
                            const Functor0 & drawfunc);
};
#endif
