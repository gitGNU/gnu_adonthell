# This file was created automatically by SWIG.
import questc
class storage:
    def __init__(self,*args):
        self.this = apply(questc.new_storage,args)
        self.thisown = 1

    def set(*args):
        val = apply(questc.storage_set,args)
        return val
    def get(*args):
        val = apply(questc.storage_get,args)
        return val
    def __del__(self,questc=questc):
        if self.thisown == 1 :
            questc.delete_storage(self)
    def __repr__(self):
        return "<C storage instance at %s>" % (self.this,)
class storagePtr(storage):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = storage



class quest(storage):
    def __init__(self,*args):
        self.this = apply(questc.new_quest,args)
        self.thisown = 1

    def __del__(self,questc=questc):
        if self.thisown == 1 :
            questc.delete_quest(self)
    def __repr__(self):
        return "<C quest instance at %s>" % (self.this,)
class questPtr(quest):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = quest





#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

STAND_NORTH = questc.STAND_NORTH
STAND_SOUTH = questc.STAND_SOUTH
STAND_WEST = questc.STAND_WEST
STAND_EAST = questc.STAND_EAST
WALK_NORTH = questc.WALK_NORTH
WALK_SOUTH = questc.WALK_SOUTH
WALK_WEST = questc.WALK_WEST
WALK_EAST = questc.WALK_EAST
NBR_MOVES = questc.NBR_MOVES
NO_MOVE = questc.NO_MOVE
