# This file was created automatically by SWIG.
import characterc
class storage:
    def __init__(self,*args):
        self.this = apply(characterc.new_storage,args)
        self.thisown = 1

    def set(*args):
        val = apply(characterc.storage_set,args)
        return val
    def get(*args):
        val = apply(characterc.storage_get,args)
        return val
    def __del__(self,characterc=characterc):
        if self.thisown == 1 :
            characterc.delete_storage(self)
    def __repr__(self):
        return "<C storage instance at %s>" % (self.this,)
class storagePtr(storage):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = storage



class character_base(storage):
    def __init__(self,*args):
        self.this = apply(characterc.new_character_base,args)
        self.thisown = 1

    def __del__(self,characterc=characterc):
        if self.thisown == 1 :
            characterc.delete_character_base(self)
    def get_name(*args):
        val = apply(characterc.character_base_get_name,args)
        return val
    def set_name(*args):
        val = apply(characterc.character_base_set_name,args)
        return val
    def get_color(*args):
        val = apply(characterc.character_base_get_color,args)
        return val
    def __repr__(self):
        return "<C character_base instance at %s>" % (self.this,)
class character_basePtr(character_base):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = character_base



class character(character_base):
    def __init__(self,*args):
        self.this = apply(characterc.new_character,args)
        self.thisown = 1

    def __del__(self,characterc=characterc):
        if self.thisown == 1 :
            characterc.delete_character(self)
    def __repr__(self):
        return "<C character instance at %s>" % (self.this,)
class characterPtr(character):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = character





#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

STAND_NORTH = characterc.STAND_NORTH
STAND_SOUTH = characterc.STAND_SOUTH
STAND_WEST = characterc.STAND_WEST
STAND_EAST = characterc.STAND_EAST
WALK_NORTH = characterc.WALK_NORTH
WALK_SOUTH = characterc.WALK_SOUTH
WALK_WEST = characterc.WALK_WEST
WALK_EAST = characterc.WALK_EAST
NBR_MOVES = characterc.NBR_MOVES
NO_MOVE = characterc.NO_MOVE
DWARF = characterc.DWARF
ELF = characterc.ELF
HALFELF = characterc.HALFELF
HUMAN = characterc.HUMAN
FEMALE = characterc.FEMALE
MALE = characterc.MALE
