/* this is just a simple test file.. */

#include "inventory.h"

int main()
{
  inventory *inv = new inventory();
  inv->debug();
  inv->setMaxItemO( 4 );

  item *i = new item( 0, "Short Sword", "", 1, 1 );
  item *j = new item( 1, "Harbringer", "", 0, 1 );
  item *k = new item( 2, "Holocaust Cloak", "", 2, 1 );
  item *l = new item( 3, "Sting", "", 1, 1 );

  /*
  i->debug();
  j->debug();
  k->debug();
  l->debug();
  */

  i->setWeight( 120 );
  j->setWeight( 0 );
  k->setWeight( 50 );
  k->setNumber( 5 );
  l->setWeight( 100 );

  inv->insert( i );
  inv->insert( j );
  inv->insert( k );
  inv->insert( k ); // Yep, I put it in twice, so there should be 10
  inv->insert( l );

  inv->showDisp();
  inv->showHash();
  

  inv->getItem( 0 ); 
  inv->getItem( 27 ); // try to get something with an illegal index
  inv->getItem( "Holocaust Cloak" );
  inv->getItem( "foo" ); // try to get something that ain't in the hash

  cout << endl;
  inv->swap( 0, 2 );
  inv->remove( "foo", 27 );
  inv->remove( "Holocaust Cloak", 3 );
  inv->remove( 1, 0 );

  inv->showDisp();

  inv->remove( 1, 0 ); // try to remove a placeholder (should fail)
  inv->getItem( 1 ); // try to get a placeholder (should fail)

  inv->remove( 3, 0 ); // make another placeholder
  inv->swap( 2, 3 ); // swap an item and a placeholder
  j->setNumber( 27 );
  inv->insert( j );
  inv->insert( k );
  inv->showDisp();

  // Get rid of everything so that the display vector will be cleared
  for( u_int32 i = 0; i < inv->getDispItems(); i++ )
    inv->remove( i, 0 );

  delete i;
  delete j;
  delete k;
  delete l;

  delete inv;

  return 0;
}
