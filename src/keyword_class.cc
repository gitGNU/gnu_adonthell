
#include <functional>
#include <map.h>
#include "keyword_class.h"
using namespace std;

keyword_class::keyword_class(const int& nb,const keyword_struct ks[])
{
  int i=0;

  for (i=0;i<nb;++i)
    insert(pair<const String, keyword_struct>(ks[i].name,ks[i]));
}

int keyword_class::nb_args(const String & name) const
{
  const_iterator i;

  i=find(name);
  return find(name)->second.argc;
}

int keyword_class::operator() (const String& name) const
{
  return (*find(name)).second.nb;
}

keyword_struct::keyword_struct(String n, int a, int b)
{
  name=n;
  nb=a;
  argc=b;
}
