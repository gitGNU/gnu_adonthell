# This file was created automatically by SWIG.
import itemtestc
class storage:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,storage):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = storage.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = storage.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_storage,args)
        self.thisown = 1
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_storage(self)
    def set_val(*args): return apply(itemtestc.storage_set_val,args)
    def get_val(*args): return apply(itemtestc.storage_get_val,args)
    def next(*args): return apply(itemtestc.storage_next,args)
    def put_state(*args): return apply(itemtestc.storage_put_state,args)
    def get_state(*args): return apply(itemtestc.storage_get_state,args)
    def clear(*args): return apply(itemtestc.storage_clear,args)
    def __repr__(self):
        return "<C storage instance at %s>" % (self.this,)

class storagePtr(storage):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = storage
itemtestc.storage_swigregister(storagePtr)
class objects:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,objects):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = objects.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = objects.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_objects,args)
        self.thisown = 1
    def set_val(*args): return apply(itemtestc.objects_set_val,args)
    def get_val(*args): return apply(itemtestc.objects_get_val,args)
    def erase(*args): return apply(itemtestc.objects_erase,args)
    def next(*args): return apply(itemtestc.objects_next,args)
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_objects(self)
    def __repr__(self):
        return "<C objects instance at %s>" % (self.this,)

class objectsPtr(objects):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = objects
itemtestc.objects_swigregister(objectsPtr)
class inventory_base:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,inventory_base):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = inventory_base.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = inventory_base.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_inventory_base,args)
        self.thisown = 1
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_inventory_base(self)
    def init(*args): return apply(itemtestc.inventory_base_init,args)
    def get_name(*args): return apply(itemtestc.inventory_base_get_name,args)
    def get_description(*args): return apply(itemtestc.inventory_base_get_description,args)
    def set_name(*args): return apply(itemtestc.inventory_base_set_name,args)
    def set_description(*args): return apply(itemtestc.inventory_base_set_description,args)
    def add_item(*args): return apply(itemtestc.inventory_base_add_item,args)
    def remove_item(*args): return apply(itemtestc.inventory_base_remove_item,args)
    def get_item(*args): return apply(itemtestc.inventory_base_get_item,args)
    def put(*args): return apply(itemtestc.inventory_base_put,args)
    def save(*args): return apply(itemtestc.inventory_base_save,args)
    def get(*args): return apply(itemtestc.inventory_base_get,args)
    def load(*args): return apply(itemtestc.inventory_base_load,args)
    def put_state(*args): return apply(itemtestc.inventory_base_put_state,args)
    def get_state(*args): return apply(itemtestc.inventory_base_get_state,args)
    def clear(*args): return apply(itemtestc.inventory_base_clear,args)
    def transfer_item_to_inventory(*args): return apply(itemtestc.inventory_base_transfer_item_to_inventory,args)
    def get_amount(*args): return apply(itemtestc.inventory_base_get_amount,args)
    def set_amount(*args): return apply(itemtestc.inventory_base_set_amount,args)
    def adjust_amount(*args): return apply(itemtestc.inventory_base_adjust_amount,args)
    def __repr__(self):
        return "<C inventory_base instance at %s>" % (self.this,)

class inventory_basePtr(inventory_base):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = inventory_base
itemtestc.inventory_base_swigregister(inventory_basePtr)
class character_base(storage):
    __setmethods__ = {}
    for _s in [storage]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,character_base):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = character_base.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [storage]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = character_base.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_character_base,args)
        self.thisown = 1
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_character_base(self)
    def get_name(*args): return apply(itemtestc.character_base_get_name,args)
    def get_id(*args): return apply(itemtestc.character_base_get_id,args)
    def set_name(*args): return apply(itemtestc.character_base_set_name,args)
    def get_color(*args): return apply(itemtestc.character_base_get_color,args)
    def set_color(*args): return apply(itemtestc.character_base_set_color,args)
    def get_portrait(*args): return apply(itemtestc.character_base_get_portrait,args)
    def set_portrait(*args): return apply(itemtestc.character_base_set_portrait,args)
    def get_dialogue(*args): return apply(itemtestc.character_base_get_dialogue,args)
    def set_dialogue(*args): return apply(itemtestc.character_base_set_dialogue,args)
    def get_state(*args): return apply(itemtestc.character_base_get_state,args)
    def put_state(*args): return apply(itemtestc.character_base_put_state,args)
    def get_inventory(*args): return apply(itemtestc.character_base_get_inventory,args)
    def set_inventory(*args): return apply(itemtestc.character_base_set_inventory,args)
    def get_equipment(*args): return apply(itemtestc.character_base_get_equipment,args)
    def set_equipment(*args): return apply(itemtestc.character_base_set_equipment,args)
    def __repr__(self):
        return "<C character_base instance at %s>" % (self.this,)

class character_basePtr(character_base):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = character_base
itemtestc.character_base_swigregister(character_basePtr)
class item_base(storage):
    __setmethods__ = {}
    for _s in [storage]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,item_base):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = item_base.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [storage]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = item_base.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_item_base,args)
        self.thisown = 1
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_item_base(self)
    def init(*args): return apply(itemtestc.item_base_init,args)
    def get_name(*args): return apply(itemtestc.item_base_get_name,args)
    def get_description(*args): return apply(itemtestc.item_base_get_description,args)
    def add_type(*args): return apply(itemtestc.item_base_add_type,args)
    def del_type(*args): return apply(itemtestc.item_base_del_type,args)
    def get_types(*args): return apply(itemtestc.item_base_get_types,args)
    def set_default_type(*args): return apply(itemtestc.item_base_set_default_type,args)
    def equip(*args): return apply(itemtestc.item_base_equip,args)
    def combine(*args): return apply(itemtestc.item_base_combine,args)
    def use(*args): return apply(itemtestc.item_base_use,args)
    def drop(*args): return apply(itemtestc.item_base_drop,args)
    def pickup(*args): return apply(itemtestc.item_base_pickup,args)
    def hand_over(*args): return apply(itemtestc.item_base_hand_over,args)
    def enchant(*args): return apply(itemtestc.item_base_enchant,args)
    def sell(*args): return apply(itemtestc.item_base_sell,args)
    def eat(*args): return apply(itemtestc.item_base_eat,args)
    def has_ability(*args): return apply(itemtestc.item_base_has_ability,args)
    def set_ability(*args): return apply(itemtestc.item_base_set_ability,args)
    def put(*args): return apply(itemtestc.item_base_put,args)
    def save(*args): return apply(itemtestc.item_base_save,args)
    def get(*args): return apply(itemtestc.item_base_get,args)
    def load(*args): return apply(itemtestc.item_base_load,args)
    def clear(*args): return apply(itemtestc.item_base_clear,args)
    def __repr__(self):
        return "<C item_base instance at %s>" % (self.this,)

class item_basePtr(item_base):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = item_base
itemtestc.item_base_swigregister(item_basePtr)
class equipment:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,equipment):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = equipment.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = equipment.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(itemtestc.new_equipment,args)
        self.thisown = 1
    def set_character(*args): return apply(itemtestc.equipment_set_character,args)
    def get_character(*args): return apply(itemtestc.equipment_get_character,args)
    def equip(*args): return apply(itemtestc.equipment_equip,args)
    def unequip(*args): return apply(itemtestc.equipment_unequip,args)
    def __del__(self,itemtestc=itemtestc):
        if getattr(self,'thisown',0):
            itemtestc.delete_equipment(self)
    def __repr__(self):
        return "<C equipment instance at %s>" % (self.this,)

class equipmentPtr(equipment):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = equipment
itemtestc.equipment_swigregister(equipmentPtr)
DIALOG_DIR = itemtestc.DIALOG_DIR
DWARF = itemtestc.DWARF
ELF = itemtestc.ELF
HALFELF = itemtestc.HALFELF
HUMAN = itemtestc.HUMAN
FEMALE = itemtestc.FEMALE
MALE = itemtestc.MALE
NPC = itemtestc.NPC
PLAYER = itemtestc.PLAYER
PARTY = itemtestc.PARTY
ACTION_DIR = itemtestc.ACTION_DIR
EQUIP = itemtestc.EQUIP
COMBINE = itemtestc.COMBINE
USE = itemtestc.USE
ENCHANT = itemtestc.ENCHANT
DROP = itemtestc.DROP
PICKUP = itemtestc.PICKUP
CHARGE = itemtestc.CHARGE
EAT = itemtestc.EAT
BOOK = itemtestc.BOOK
WEAPON = itemtestc.WEAPON
POTION = itemtestc.POTION
FOOD = itemtestc.FOOD
KEY = itemtestc.KEY
ARMOR = itemtestc.ARMOR
LEFT_HAND = itemtestc.LEFT_HAND
RIGHT_HAND = itemtestc.RIGHT_HAND
BOTH_HANDS = itemtestc.BOTH_HANDS
HEAD = itemtestc.HEAD
LEGS = itemtestc.LEGS
BODY = itemtestc.BODY
RING = itemtestc.RING

