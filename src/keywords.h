#include "keyword_class.h"

#ifndef _keywords
#define _keywords


#define kw(x,n,m) keyword_struct(#x , n, m)
// x: name of keyword, n: number of keyword, m: nb of arguments required (including itself)

#define KEYWORD_NB 3

const keyword_struct KEYWORD_LIST[] =
{
  kw(ITEM,1,2),
  kw(END,2,1),
  kw(EVENT,3,0)
}; 

const keyword_class KEYWORD(KEYWORD_NB,KEYWORD_LIST);

#endif
