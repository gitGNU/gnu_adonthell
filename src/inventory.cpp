/* this is just a simple test file.. */

#include <iostream.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "item.h"
#include "inventory.h"

void
main (void)
{
    inventory *inv = new inventory ();
    int a[] = { 0, 1, 2, 3, 4, 5 };
    item *i = new item (1, "a Short Sword", a, 120, "", NULL);
    item *k = new item (2, "a Holocaust Cloak", a, 50, "", NULL);
    item *l = new item (3, "A sword named String", a, 100, "", NULL);
    item *j = new item (4, "a Harbringer", a, 120, "", NULL);
     
    item x;
    cout << "Sizeof empty item " << sizeof (x) << "\n" << flush;

    inv->insert (i);
    inv->insert (k);
    inv->insert (j);
    inv->insert (l);
    inv->display ();
}
