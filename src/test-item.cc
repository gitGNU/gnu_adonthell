// Test program for the item superclass, as defined in item.h and
// documented in item.txt. Note that the behaviour of the class is checked for
// conformance to the documentation.

#include "item.h"

void disp_item( const item it );
void cmp( const item i1, const item i2 );

int main( int argc, char* argv[] )
{
  // First, see if the user wants debug on for all items
  bool debug = false;
  for( int i = 1; i < argc; i++ )
    if( strcmp( argv[i], "debug" ) == 0 || strcmp( argv[i], "DEBUG" ) == 0 )
    {
      cout << "DEBUG ON!" << endl;
      debug = true;
    }

  // CONSTRUCTOR TESTS
  // ===============================================================

  cout << "\nCONSTRUCTORS:\n";

  // Test the default constructor

  cout << "\tDefault... ";
  item CT_def;

  if( debug )
    CT_def.debug();

  if( CT_def.next != NULL )
    cout << "FAILED, because .next != NULL\n";
  else if( CT_def.prev != NULL )
    cout << "FAILED, because .prev != NULL\n";
  else if( CT_def.getId() != 0 )
    cout << "FAILED, because .getID != 0\n";
  else if( CT_def.getName() != "" )
    cout << "FAILED, because .getName() != ''\n";
  else if( CT_def.getGfx() != "" )
    cout << "FAILED, because .getGfx() != ''\n";
  else if( CT_def.getType() != 0 )
    cout << "FAILED, because .getType() != 0\n";
  else if( CT_def.getSize() != 0 )
    cout << "FAILED, because .getSize() != 0\n";
  else if( CT_def.getWeight() != 0 )
    cout << "FAILED, because .getWeight() != 0\n";
  else if( CT_def.getValue() != 0 )
    cout << "FAILED, because .getValue() != 0\n";
  else if( CT_def.getUses() != 0 )
    cout << "FAILED, because .getUses() != 0\n";
  else if( CT_def.getMoveable() )
    cout << "FAILED, because .getMoveable() != false\n";
  else if( CT_def.getContainer() )
    cout << "FAILED, because .getContainer() != false\n";
  else if( CT_def.getLight() )
    cout << "FAILED, because .getLight() != false\n";
  else if( CT_def.getLocked() )
    cout << "FAILED, because .getLocked() != false\n";
  else if( CT_def.getOpenable() )
    cout << "FAILED, because .getOpenable() != false\n";
  else if( CT_def.getUseable() )
    cout << "FAILED, because .getUseable() != false\n";
  else if( !CT_def.getObtainable() )
    cout << "FAILED, because .getObtainable() != true\n";
  else if( !CT_def.getDroppable() )
    cout << "FAILED, because .getDroppable() != true\n";
  else if( !CT_def.getCombineable() )
    cout << "FAILED, because .getCombineable() != true\n";
  else if( CT_def.getEquipable() )
    cout << "FAILED, because .getEquipable() != false\n";
  else if( CT_def.getUnequipable() )
    cout << "FAILED, because .getUnequipable() != false\n";
  else if( CT_def.getAttached() )
    cout << "FAILED, because .getAttached() != false\n";
  else if( CT_def.getObtainEffAct() )
    cout << "FAILED, because .getObtainEffAct() != false\n";
  else if( CT_def.getDropEffAct() )
    cout << "FAILED, because .getDropEffAct() != false\n";
  else if( CT_def.getEquipEffAct() )
    cout << "FAILED, because .getEquipEffAct() != false\n";
  else if( CT_def.getUnequipEffAct() )
    cout << "FAILED, because .getUnequipEffAct() != false\n";
  else if( CT_def.getUseEffAct() )
    cout << "FAILED, because .getUseEffAct() != false\n";
  else if( CT_def.getAttItem() != NULL )
    cout << "FAILED, because .getAttItem() != NULL\n";
  else if( CT_def.number != 1 )
    cout << "FAILED, because .number != 1\n";
  else
    cout << "OK!\n";

  // Test the full constructor

  cout << "\tFull... ";
  unsigned int at[NUM_ATT];
  bool ch[NUM_CHR], ae[NUM_EFF];
  effect ef[NUM_EFF];
  for( unsigned int i = 0; i < NUM_ATT; i++ )
    at[i] = 0;
  for( unsigned int i = 0; i < NUM_CHR; i++ )
    ch[i] = false;
  for( unsigned int i = 0; i < NUM_EFF; i++ )
    ae[i] = false;
  item CT_ful( 27, "Something", "something.gfx", 0, at, ch, ae, ef, 27 );

  if( debug )
    CT_ful.debug();

  if( CT_ful.next != NULL )
    cout << "FAILED, because .next != NULL\n";
  else if( CT_ful.prev != NULL )
    cout << "FAILED, because .prev != NULL\n";
  else if( CT_ful.getId() != 27 )
    cout << "FAILED, because .getID != 27\n";
  else if( CT_ful.getName() != "Something" )
    cout << "FAILED, because .getName() != 'Something'\n";
  else if( CT_ful.getGfx() != "something.gfx" )
    cout << "FAILED, because .getGfx() != 'something.gfx'\n";
  else if( CT_ful.getType() != 0 )
    cout << "FAILED, because .getType() != 0\n";
  else if( CT_ful.getSize() != 0 )
    cout << "FAILED, because .getSize() != 0\n";
  else if( CT_ful.getWeight() != 0 )
    cout << "FAILED, because .getWeight() != 0\n";
  else if( CT_ful.getValue() != 0 )
    cout << "FAILED, because .getValue() != 0\n";
  else if( CT_ful.getUses() != 0 )
    cout << "FAILED, because .getUses() != 0\n";
  else if( CT_ful.getMoveable() )
    cout << "FAILED, because .getMoveable() != false\n";
  else if( CT_ful.getContainer() )
    cout << "FAILED, because .getContainer() != false\n";
  else if( CT_ful.getLight() )
    cout << "FAILED, because .getLight() != false\n";
  else if( CT_ful.getLocked() )
    cout << "FAILED, because .getLocked() != false\n";
  else if( CT_ful.getOpenable() )
    cout << "FAILED, because .getOpenable() != false\n";
  else if( CT_ful.getUseable() )
    cout << "FAILED, because .getUseable() != false\n";
  else if( CT_ful.getObtainable() )
    cout << "FAILED, because .getObtainable() != false\n";
  else if( CT_ful.getDroppable() )
    cout << "FAILED, because .getDroppable() != false\n";
  else if( CT_ful.getCombineable() )
    cout << "FAILED, because .getCombineable() != false\n";
  else if( CT_ful.getEquipable() )
    cout << "FAILED, because .getEquipable() != false\n";
  else if( CT_ful.getUnequipable() )
    cout << "FAILED, because .getUnequipable() != false\n";
  else if( CT_ful.getAttached() )
    cout << "FAILED, because .getAttached() != false\n";
  else if( CT_ful.getObtainEffAct() )
    cout << "FAILED, because .getObtainEffAct() != false\n";
  else if( CT_ful.getDropEffAct() )
    cout << "FAILED, because .getDropEffAct() != false\n";
  else if( CT_ful.getEquipEffAct() )
    cout << "FAILED, because .getEquipEffAct() != false\n";
  else if( CT_ful.getUnequipEffAct() )
    cout << "FAILED, because .getUnequipEffAct() != false\n";
  else if( CT_ful.getUseEffAct() )
    cout << "FAILED, because .getUseEffAct() != false\n";
  else if( CT_ful.getAttItem() != NULL )
    cout << "FAILED, because .getAttItem() != NULL\n";
  else if( CT_ful.number != 27 )
    cout << "FAILED, because .number != 27\n";
  else
    cout << "OK!\n";

  // Test the sparse constructor

  cout << "\tSparse... ";
  item CT_spa( 32, "Something Else", "something-else.gfx", 0, 99 );

  if( debug )
    CT_spa.debug();

  if( CT_spa.next != NULL )
    cout << "FAILED, because .next != NULL\n";
  else if( CT_spa.prev != NULL )
    cout << "FAILED, because .prev != NULL\n";
  else if( CT_spa.getId() != 32 )
    cout << "FAILED, because .getID != 32\n";
  else if( CT_spa.getName() != "Something Else" )
    cout << "FAILED, because .getName() != 'Something Else'\n";
  else if( CT_spa.getGfx() != "something-else.gfx" )
    cout << "FAILED, because .getGfx() != 'something-else.gfx'\n";
  else if( CT_spa.getType() != 0 )
    cout << "FAILED, because .getType() != 0\n";
  else if( CT_spa.getSize() != 0 )
    cout << "FAILED, because .getSize() != 0\n";
  else if( CT_spa.getWeight() != 0 )
    cout << "FAILED, because .getWeight() != 0\n";
  else if( CT_spa.getValue() != 0 )
    cout << "FAILED, because .getValue() != 0\n";
  else if( CT_spa.getUses() != 0 )
    cout << "FAILED, because .getUses() != 0\n";
  else if( CT_spa.getMoveable() )
    cout << "FAILED, because .getMoveable() != false\n";
  else if( CT_spa.getContainer() )
    cout << "FAILED, because .getContainer() != false\n";
  else if( CT_spa.getLight() )
    cout << "FAILED, because .getLight() != false\n";
  else if( CT_spa.getLocked() )
    cout << "FAILED, because .getLocked() != false\n";
  else if( CT_spa.getOpenable() )
    cout << "FAILED, because .getOpenable() != false\n";
  else if( CT_spa.getUseable() )
    cout << "FAILED, because .getUseable() != false\n";
  else if( !CT_spa.getObtainable() )
    cout << "FAILED, because .getObtainable() != true\n";
  else if( !CT_spa.getDroppable() )
    cout << "FAILED, because .getDroppable() != true\n";
  else if( !CT_spa.getCombineable() )
    cout << "FAILED, because .getCombineable() != true\n";
  else if( CT_spa.getEquipable() )
    cout << "FAILED, because .getEquipable() != false\n";
  else if( CT_spa.getUnequipable() )
    cout << "FAILED, because .getUnequipable() != false\n";
  else if( CT_spa.getAttached() )
    cout << "FAILED, because .getAttached() != false\n";
  else if( CT_spa.getObtainEffAct() )
    cout << "FAILED, because .getObtainEffAct() != false\n";
  else if( CT_spa.getDropEffAct() )
    cout << "FAILED, because .getDropEffAct() != false\n";
  else if( CT_spa.getEquipEffAct() )
    cout << "FAILED, because .getEquipEffAct() != false\n";
  else if( CT_spa.getUnequipEffAct() )
    cout << "FAILED, because .getUnequipEffAct() != false\n";
  else if( CT_spa.getUseEffAct() )
    cout << "FAILED, because .getUseEffAct() != false\n";
  else if( CT_spa.getAttItem() != NULL )
    cout << "FAILED, because .getAttItem() != NULL\n";
  else if( CT_spa.number != 99 )
    cout << "FAILED, because .number != 99\n";
  else
    cout << "OK!\n";

  // Test the from file constructor (an easy test, since it has not yet
  // been implemented! :)

  cout << "\tFile... ";
  item CT_fil( "foo" ); // no seg fault, no problem
  if( debug )
    CT_fil.debug();
  cout << "OK!\n";      // Ha!

  // ===============================================================

  // OVERLOADED OPERATOR TESTS
  // ===============================================================

  cout << "\nOVERLOADED OPERATORS:\n";

  item OT_1, OT_3;
  item OT_2( 27, "Foo", "foo.gfx", 1, 99 );

  if( debug )
  {
    OT_1.debug();
    OT_2.debug();
    OT_3.debug();
  }

  OT_2.attach( OT_3 );

  cout << "\t== (Step 1)... ";
  if( OT_1 == OT_2 )
    cout << "FAILED, because OT_1 should not equal OT_2!\n";
  else
    cout << "OK!\n";

  cout << "\t!= (Step 1)... ";
  if( OT_1 != OT_2 )
    cout << "OK!\n";
  else
    cout << "FAILED, because OT_1 should not equal OT_2!\n";

  OT_1 = OT_2;
  
  cout << "\t== (Step 2)... ";
  if( OT_1 == OT_2 )
    cout << "OK!\n";
  else
    cout << "FAILED, because OT_1 should not equal OT_2!\n";

  cout << "\t!= (Step 2)... ";
  if( OT_1 != OT_2 )
    cout << "FAILED, because OT_1 should not equal OT_2!\n";
  else
    cout << "OK!\n";

  cout << "If Step 2 was OK, = is also OK!\n";

  // ===============================================================

  // FUNCTION TESTS
  // ===============================================================

  cout << "\nFUNCTIONS:\n";

  item FT_1, FT_3;
  item FT_2( 27, "Foo", "foo.gfx", 1, 99 );
  effect FT_E( "Foobar" );

  if( debug )
  {
    FT_1.debug();
    FT_2.debug();
    FT_3.debug();
  }

  FT_2.attach( FT_3 );
  FT_2.setObtainEffAct( true );
  FT_2.setDropEffAct( true );
  FT_2.setEquipEffAct( true );
  FT_2.setUnequipEffAct( true );
  FT_2.setUseEffAct( true );
  FT_2.setObtainEff( FT_E );
  FT_2.setDropEff( FT_E );
  FT_2.setEquipEff( FT_E );
  FT_2.setUnequipEff( FT_E );
  FT_2.setUseEff( FT_E );

  cout << "\tget/setId()... ";
  FT_1.setId( FT_2.getId() );
  if( FT_1.getId() == FT_2.getId() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setName()... ";
  FT_1.setName( FT_2.getName() );
  if( FT_1.getName() == FT_2.getName() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setGfx()... ";
  FT_1.setGfx( FT_2.getGfx() );
  if( FT_1.getGfx() == FT_2.getGfx() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setType()... ";
  FT_1.setType( FT_2.getType() );
  if( FT_1.getType() == FT_2.getType() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setSize()... ";
  FT_1.setSize( FT_2.getSize() );
  if( FT_1.getSize() == FT_2.getSize() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setWeight()... ";
  FT_1.setWeight( FT_2.getWeight() );
  if( FT_1.getWeight() == FT_2.getWeight() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setValue()... ";
  FT_1.setValue( FT_2.getValue() );
  if( FT_1.getValue() == FT_2.getValue() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUses()... ";
  FT_1.setUses( FT_2.getUses() );
  if( FT_1.getUses() == FT_2.getUses() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setMoveable()... ";
  FT_1.setMoveable( FT_2.getMoveable() );
  if( FT_1.getMoveable() == FT_2.getMoveable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setContainer()... ";
  FT_1.setContainer( FT_2.getContainer() );
  if( FT_1.getContainer() == FT_2.getContainer() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setLight()... ";
  FT_1.setLight( FT_2.getLight() );
  if( FT_1.getLight() == FT_2.getLight() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setLocked()... ";
  FT_1.setLocked( FT_2.getLocked() );
  if( FT_1.getLocked() == FT_2.getLocked() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setOpenable()... ";
  FT_1.setOpenable( FT_2.getOpenable() );
  if( FT_1.getOpenable() == FT_2.getOpenable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUseable()... ";
  FT_1.setUseable( FT_2.getUseable() );
  if( FT_1.getUseable() == FT_2.getUseable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setObtainable()... ";
  FT_1.setObtainable( FT_2.getObtainable() );
  if( FT_1.getObtainable() == FT_2.getObtainable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setDroppable()... ";
  FT_1.setDroppable( FT_2.getDroppable() );
  if( FT_1.getDroppable() == FT_2.getDroppable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setCombineable()... ";
  FT_1.setCombineable( FT_2.getCombineable() );
  if( FT_1.getCombineable() == FT_2.getCombineable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setEquipable()... ";
  FT_1.setEquipable( FT_2.getEquipable() );
  if( FT_1.getEquipable() == FT_2.getEquipable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUnequipable()... ";
  FT_1.setUnequipable( FT_2.getUnequipable() );
  if( FT_1.getUnequipable() == FT_2.getUnequipable() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tgetAttached()... ";
  if( FT_2.getAttached() && !FT_1.getAttached() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tgetAttItem/attach()... ";
  FT_1.attach( *( FT_2.getAttItem() ) );
  if( FT_1.getAttItem() == FT_2.getAttItem() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setObtainEffAct()... ";
  FT_1.setObtainEffAct( FT_2.getObtainEffAct() );
  if( FT_1.getObtainEffAct() == FT_2.getObtainEffAct() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setDropEffAct()... ";
  FT_1.setDropEffAct( FT_2.getDropEffAct() );
  if( FT_1.getDropEffAct() == FT_2.getDropEffAct() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setEquipEffAct()... ";
  FT_1.setEquipEffAct( FT_2.getEquipEffAct() );
  if( FT_1.getEquipEffAct() == FT_2.getEquipEffAct() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUnequipEffAct()... ";
  FT_1.setUnequipEffAct( FT_2.getUnequipEffAct() );
  if( FT_1.getUnequipEffAct() == FT_2.getUnequipEffAct() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUseEffAct()... ";
  FT_1.setUseEffAct( FT_2.getUseEffAct() );
  if( FT_1.getUseEffAct() == FT_2.getUseEffAct() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setObtainEff()... ";
  FT_1.setObtainEff( FT_2.getObtainEff() );
  if( FT_1.getObtainEff() == FT_2.getObtainEff() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setDropEff()... ";
  FT_1.setDropEff( FT_2.getDropEff() );
  if( FT_1.getDropEff() == FT_2.getDropEff() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setEquipEff()... ";
  FT_1.setEquipEff( FT_2.getEquipEff() );
  if( FT_1.getEquipEff() == FT_2.getEquipEff() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUnequipEff()... ";
  FT_1.setUnequipEff( FT_2.getUnequipEff() );
  if( FT_1.getUnequipEff() == FT_2.getUnequipEff() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tget/setUseEff()... ";
  FT_1.setUseEff( FT_2.getUseEff() );
  if( FT_1.getUseEff() == FT_2.getUseEff() )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

  cout << "\tdetach()... ";
  if( FT_1.detach() && !FT_1.getAttached() &&
      ( FT_1.getAttItem() != FT_2.getAttItem() ) )
    cout << "OK!\n";
  else
    cout << "FAILED!\n";

}
