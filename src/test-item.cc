#include "item.h"

void disp_item( const item it );
void cmp( const item i1, const item i2 );

int main()
{
  unsigned int at[NUM_ATT];
  bool ch[NUM_CHR], ae[NUM_EFF];
  effect pe, ef[NUM_EFF];

  at[0] = 1;
  at[1] = 1;
  at[2] = 5;
  at[3] = 1;
  for( int i = 0; i < 5; i++ )
    ch[i] = false;
  for( int i = 5; i < 9; i++ )
    ch[i] = true;
  for( int i = 9; i < 11; i++ )
    ch[i] = false;
  for( int i = 0; i < 4; i++ )
    ae[i] = false;
  ae[4] = true;
  ef[4].setName( "heal" );
  pe.setName( "poison" );

  item nuthin;
  item hp( NULL, NULL, 27, "Heal Potion", "heal-potion.gfx", 0,
	   at, ch, ae, ef );
  item po( 2, "Poison", "poison.gfx", 0 );

  po.setUseEffAct( true );
  po.setUseEff( pe );

  disp_item( nuthin );
  disp_item( hp );
  disp_item( po );

  cmp( nuthin, hp );
  cmp( hp, po );
  cmp( po, nuthin );

  return 0;
}

void disp_item( const item it )
{
  cout << it.getName()
       << "\n\tID:\t" << it.getId() << "\tType:\t" << it.getType()
       << "\n\tSize:\t" << it.getSize() << "\tWeight:\t" << it.getWeight()
       << "\tValue:\t" << it.getValue() << "\tUses:\t" << it.getUses()
       << "\n\tMoveable?\t" << it.getMoveable()
       << "\n\tContainer?\t" << it.getContainer()
       << "\n\tLight?\t\t" << it.getLight()
       << "\n\tLocked?\t\t" << it.getLocked()
       << "\n\tOpenable?\t" << it.getOpenable()
       << "\n\tUseable?\t" << it.getUseable()
       << "\n\tObtainable?\t" << it.getObtainable()
       << "\n\tDroppable?\t" << it.getDroppable()
       << "\n\tCombineable?\t" << it.getCombineable()
       << "\n\tEquipable?\t" << it.getEquipable()
       << "\n\tUnequipable?\t" << it.getUnequipable()
       << "\n\tObtain effect active?\t" << it.getObtainEffAct()
       << "\n\tDrop effect active?\t" << it.getDropEffAct()
       << "\n\tEquip effect active?\t" << it.getEquipEffAct()
       << "\n\tUnequip effect active?\t" << it.getUnequipEffAct()
       << "\n\tUse effect active?\t" << it.getUseEffAct();
  if( it.getObtainEffAct() )
    cout << "\n\tObtain effect:\t" << it.getObtainEff().getName();
  if( it.getDropEffAct() )
    cout << "\n\tDrop effect:\t" << it.getDropEff().getName();
  if( it.getEquipEffAct() )
    cout << "\n\tEquip effect:\t" << it.getEquipEff().getName();
  if( it.getUnequipEffAct() )
    cout << "\n\tUnequip effect:\t" << it.getUnequipEff().getName();
  if( it.getUseEffAct() )
    cout << "\n\tUse effect:\t" << it.getUseEff().getName();
  cout << endl;
}

void cmp( const item i1, const item i2 )
{
  cout << i1.getName() << " == " << i2.getName() << ": ";
  if( i1 == i2 )
    cout << "TRUE\n";
  else
    cout << "FALSE\n";
  cout << i2.getName() << " == " << i1.getName() << ": ";
  if( i2 == i1 )
    cout << "TRUE\n";
  else
    cout << "FALSE\n";
  cout << i1.getName() << " != " << i2.getName() << ": ";
  if( i1 != i2 )
    cout << "TRUE\n";
  else
    cout << "FALSE\n";
  cout << i2.getName() << " != " << i1.getName() << ": ";
  if( i2 != i1 )
    cout << "TRUE\n";
  else
    cout << "FALSE\n";
}
