# This file was created automatically by SWIG.
import adonthellc
class gz_file:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,gz_file):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = gz_file.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = gz_file.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_gz_file,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_gz_file(self)
    def open(*args): return apply(adonthellc.gz_file_open,args)
    def close(*args): return apply(adonthellc.gz_file_close,args)
    def is_open(*args): return apply(adonthellc.gz_file_is_open,args)
    def eof(*args): return apply(adonthellc.gz_file_eof,args)
    def __repr__(self):
        return "<C gz_file instance at %s>" % (self.this,)

class gz_filePtr(gz_file):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = gz_file
adonthellc.gz_file_swigregister(gz_filePtr)
class igzstream(gz_file):
    __setmethods__ = {}
    for _s in [gz_file]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,igzstream):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = igzstream.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [gz_file]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = igzstream.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_igzstream,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_igzstream(self)
    def open(*args): return apply(adonthellc.igzstream_open,args)
    def get_block(*args): return apply(adonthellc.igzstream_get_block,args)
    def __repr__(self):
        return "<C igzstream instance at %s>" % (self.this,)

class igzstreamPtr(igzstream):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = igzstream
adonthellc.igzstream_swigregister(igzstreamPtr)
class ogzstream(gz_file):
    __setmethods__ = {}
    for _s in [gz_file]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,ogzstream):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = ogzstream.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [gz_file]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = ogzstream.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_ogzstream,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_ogzstream(self)
    def open(*args): return apply(adonthellc.ogzstream_open,args)
    def put_block(*args): return apply(adonthellc.ogzstream_put_block,args)
    def __repr__(self):
        return "<C ogzstream instance at %s>" % (self.this,)

class ogzstreamPtr(ogzstream):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ogzstream
adonthellc.ogzstream_swigregister(ogzstreamPtr)
class fileops:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,fileops):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = fileops.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = fileops.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_fileops,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_fileops(self)
    def __repr__(self):
        return "<C fileops instance at %s>" % (self.this,)

class fileopsPtr(fileops):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = fileops
adonthellc.fileops_swigregister(fileopsPtr)
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
        self.this = apply(adonthellc.new_storage,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_storage(self)
    def set_val(*args): return apply(adonthellc.storage_set_val,args)
    def get_val(*args): return apply(adonthellc.storage_get_val,args)
    def next(*args): return apply(adonthellc.storage_next,args)
    def __repr__(self):
        return "<C storage instance at %s>" % (self.this,)

class storagePtr(storage):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = storage
adonthellc.storage_swigregister(storagePtr)
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
        self.this = apply(adonthellc.new_objects,args)
        self.thisown = 1
    def set_val(*args): return apply(adonthellc.objects_set_val,args)
    def get_val(*args): return apply(adonthellc.objects_get_val,args)
    def erase(*args): return apply(adonthellc.objects_erase,args)
    def next(*args): return apply(adonthellc.objects_next,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_objects(self)
    def __repr__(self):
        return "<C objects instance at %s>" % (self.this,)

class objectsPtr(objects):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = objects
adonthellc.objects_swigregister(objectsPtr)
class event:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_event(self)
    def script_file(*args): return apply(adonthellc.event_script_file,args)
    def save(*args): return apply(adonthellc.event_save,args)
    def load(*args): return apply(adonthellc.event_load,args)
    def set_script(*args): return apply(adonthellc.event_set_script,args)
    def get_script_state(*args): return apply(adonthellc.event_get_script_state,args)
    def put_script_state(*args): return apply(adonthellc.event_put_script_state,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C event instance at %s>" % (self.this,)

class eventPtr(event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = event
adonthellc.event_swigregister(eventPtr)
class event_list:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,event_list):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = event_list.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = event_list.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_event_list(self)
    def clear(*args): return apply(adonthellc.event_list_clear,args)
    def add_event(*args): return apply(adonthellc.event_list_add_event,args)
    def save(*args): return apply(adonthellc.event_list_save,args)
    def load(*args): return apply(adonthellc.event_list_load,args)
    def __init__(self,*args):
        self.this = apply(adonthellc.new_event_list,args)
        self.thisown = 1
    def __repr__(self):
        return "<C event_list instance at %s>" % (self.this,)

class event_listPtr(event_list):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = event_list
adonthellc.event_list_swigregister(event_listPtr)
class event_handler:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,event_handler):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = event_handler.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = event_handler.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_event_handler,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_event_handler(self)
    def __repr__(self):
        return "<C event_handler instance at %s>" % (self.this,)

class event_handlerPtr(event_handler):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = event_handler
adonthellc.event_handler_swigregister(event_handlerPtr)
class gametime:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,gametime):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = gametime.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = gametime.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_gametime,args)
        self.thisown = 1
    def tick(*args): return apply(adonthellc.gametime_tick,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_gametime(self)
    def __repr__(self):
        return "<C gametime instance at %s>" % (self.this,)

class gametimePtr(gametime):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = gametime
adonthellc.gametime_swigregister(gametimePtr)
class time_event(event):
    __setmethods__ = {}
    for _s in [event]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,time_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = time_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [event]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = time_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_time_event,args)
        self.thisown = 1
    def save(*args): return apply(adonthellc.time_event_save,args)
    __setmethods__["minute"] = adonthellc.time_event_minute_set
    __getmethods__["minute"] = adonthellc.time_event_minute_get
    __setmethods__["m_step"] = adonthellc.time_event_m_step_set
    __getmethods__["m_step"] = adonthellc.time_event_m_step_get
    __setmethods__["hour"] = adonthellc.time_event_hour_set
    __getmethods__["hour"] = adonthellc.time_event_hour_get
    __setmethods__["h_step"] = adonthellc.time_event_h_step_set
    __getmethods__["h_step"] = adonthellc.time_event_h_step_get
    __setmethods__["day"] = adonthellc.time_event_day_set
    __getmethods__["day"] = adonthellc.time_event_day_get
    __setmethods__["d_step"] = adonthellc.time_event_d_step_set
    __getmethods__["d_step"] = adonthellc.time_event_d_step_get
    __setmethods__["time"] = adonthellc.time_event_time_set
    __getmethods__["time"] = adonthellc.time_event_time_get
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_time_event(self)
    def __repr__(self):
        return "<C time_event instance at %s>" % (self.this,)

class time_eventPtr(time_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = time_event
adonthellc.time_event_swigregister(time_eventPtr)
class input:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,input):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = input.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = input.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_input,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_input(self)
    def __repr__(self):
        return "<C input instance at %s>" % (self.this,)

class inputPtr(input):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = input
adonthellc.input_swigregister(inputPtr)
class audio:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,audio):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = audio.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = audio.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_audio,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_audio(self)
    def __repr__(self):
        return "<C audio instance at %s>" % (self.this,)

class audioPtr(audio):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = audio
adonthellc.audio_swigregister(audioPtr)
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
        self.this = apply(adonthellc.new_character_base,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_character_base(self)
    def get_name(*args): return apply(adonthellc.character_base_get_name,args)
    def get_id(*args): return apply(adonthellc.character_base_get_id,args)
    def set_name(*args): return apply(adonthellc.character_base_set_name,args)
    def get_color(*args): return apply(adonthellc.character_base_get_color,args)
    def set_color(*args): return apply(adonthellc.character_base_set_color,args)
    def get_portrait(*args): return apply(adonthellc.character_base_get_portrait,args)
    def set_portrait(*args): return apply(adonthellc.character_base_set_portrait,args)
    def get_dialogue(*args): return apply(adonthellc.character_base_get_dialogue,args)
    def set_dialogue(*args): return apply(adonthellc.character_base_set_dialogue,args)
    def get_state(*args): return apply(adonthellc.character_base_get_state,args)
    def put_state(*args): return apply(adonthellc.character_base_put_state,args)
    def __repr__(self):
        return "<C character_base instance at %s>" % (self.this,)

class character_basePtr(character_base):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = character_base
adonthellc.character_base_swigregister(character_basePtr)
class drawing_area:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,drawing_area):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = drawing_area.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = drawing_area.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_drawing_area,args)
        self.thisown = 1
    def x(*args): return apply(adonthellc.drawing_area_x,args)
    def y(*args): return apply(adonthellc.drawing_area_y,args)
    def length(*args): return apply(adonthellc.drawing_area_length,args)
    def height(*args): return apply(adonthellc.drawing_area_height,args)
    def move(*args): return apply(adonthellc.drawing_area_move,args)
    def resize(*args): return apply(adonthellc.drawing_area_resize,args)
    def assign_drawing_area(*args): return apply(adonthellc.drawing_area_assign_drawing_area,args)
    def assigned_drawing_area(*args): return apply(adonthellc.drawing_area_assigned_drawing_area,args)
    def detach_drawing_area(*args): return apply(adonthellc.drawing_area_detach_drawing_area,args)
    def setup_rects(*args): return apply(adonthellc.drawing_area_setup_rects,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_drawing_area(self)
    def __repr__(self):
        return "<C drawing_area instance at %s>" % (self.this,)

class drawing_areaPtr(drawing_area):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = drawing_area
adonthellc.drawing_area_swigregister(drawing_areaPtr)
class quest(storage):
    __setmethods__ = {}
    for _s in [storage]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,quest):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = quest.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [storage]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = quest.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_quest,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_quest(self)
    def __repr__(self):
        return "<C quest instance at %s>" % (self.this,)

class questPtr(quest):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = quest
adonthellc.quest_swigregister(questPtr)
class drawable:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,drawable):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = drawable.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = drawable.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_drawable(self)
    def length(*args): return apply(adonthellc.drawable_length,args)
    def height(*args): return apply(adonthellc.drawable_height,args)
    def update(*args): return apply(adonthellc.drawable_update,args)
    def input_update(*args): return apply(adonthellc.drawable_input_update,args)
    def draw(*args): return apply(adonthellc.drawable_draw,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C drawable instance at %s>" % (self.this,)

class drawablePtr(drawable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = drawable
adonthellc.drawable_swigregister(drawablePtr)
class surface(drawable):
    __setmethods__ = {}
    for _s in [drawable]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,surface):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = surface.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [drawable]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = surface.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_surface,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_surface(self)
    def is_masked(*args): return apply(adonthellc.surface_is_masked,args)
    def set_mask(*args): return apply(adonthellc.surface_set_mask,args)
    def alpha(*args): return apply(adonthellc.surface_alpha,args)
    def set_alpha(*args): return apply(adonthellc.surface_set_alpha,args)
    def draw(*args): return apply(adonthellc.surface_draw,args)
    def draw_part(*args): return apply(adonthellc.surface_draw_part,args)
    def fillrect(*args): return apply(adonthellc.surface_fillrect,args)
    def fillrect_rgb(*args): return apply(adonthellc.surface_fillrect_rgb,args)
    def lock(*args): return apply(adonthellc.surface_lock,args)
    def unlock(*args): return apply(adonthellc.surface_unlock,args)
    def put_pix(*args): return apply(adonthellc.surface_put_pix,args)
    def put_pix_rgb(*args): return apply(adonthellc.surface_put_pix_rgb,args)
    def get_pix(*args): return apply(adonthellc.surface_get_pix,args)
    def get_pix_rgb(*args): return apply(adonthellc.surface_get_pix_rgb,args)
    def copy(*args): return apply(adonthellc.surface_copy,args)
    __setmethods__["vis"] = adonthellc.surface_vis_set
    __getmethods__["vis"] = adonthellc.surface_vis_get
    def __repr__(self):
        return "<C surface instance at %s>" % (self.this,)

class surfacePtr(surface):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = surface
adonthellc.surface_swigregister(surfacePtr)
class screen:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,screen):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = screen.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = screen.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_screen,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_screen(self)
    def __repr__(self):
        return "<C screen instance at %s>" % (self.this,)

class screenPtr(screen):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = screen
adonthellc.screen_swigregister(screenPtr)
class image(surface):
    __setmethods__ = {}
    for _s in [surface]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,image):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = image.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [surface]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = image.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_image,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_image(self)
    def resize(*args): return apply(adonthellc.image_resize,args)
    def clear(*args): return apply(adonthellc.image_clear,args)
    def get(*args): return apply(adonthellc.image_get,args)
    def load(*args): return apply(adonthellc.image_load,args)
    def get_raw(*args): return apply(adonthellc.image_get_raw,args)
    def load_raw(*args): return apply(adonthellc.image_load_raw,args)
    def get_pnm(*args): return apply(adonthellc.image_get_pnm,args)
    def load_pnm(*args): return apply(adonthellc.image_load_pnm,args)
    def put(*args): return apply(adonthellc.image_put,args)
    def save(*args): return apply(adonthellc.image_save,args)
    def put_raw(*args): return apply(adonthellc.image_put_raw,args)
    def save_raw(*args): return apply(adonthellc.image_save_raw,args)
    def put_pnm(*args): return apply(adonthellc.image_put_pnm,args)
    def save_pnm(*args): return apply(adonthellc.image_save_pnm,args)
    def zoom(*args): return apply(adonthellc.image_zoom,args)
    def zoom_to(*args): return apply(adonthellc.image_zoom_to,args)
    def tile(*args): return apply(adonthellc.image_tile,args)
    def tile_to(*args): return apply(adonthellc.image_tile_to,args)
    def brightness(*args): return apply(adonthellc.image_brightness,args)
    def copy(*args): return apply(adonthellc.image_copy,args)
    def __repr__(self):
        return "<C image instance at %s>" % (self.this,)

class imagePtr(image):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = image
adonthellc.image_swigregister(imagePtr)
class animationframe:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,animationframe):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = animationframe.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = animationframe.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_animationframe,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_animationframe(self)
    def clear(*args): return apply(adonthellc.animationframe_clear,args)
    def is_masked(*args): return apply(adonthellc.animationframe_is_masked,args)
    def set_mask(*args): return apply(adonthellc.animationframe_set_mask,args)
    def alpha(*args): return apply(adonthellc.animationframe_alpha,args)
    def set_alpha(*args): return apply(adonthellc.animationframe_set_alpha,args)
    def image_nbr(*args): return apply(adonthellc.animationframe_image_nbr,args)
    def set_image_nbr(*args): return apply(adonthellc.animationframe_set_image_nbr,args)
    def delay(*args): return apply(adonthellc.animationframe_delay,args)
    def set_delay(*args): return apply(adonthellc.animationframe_set_delay,args)
    def nextframe(*args): return apply(adonthellc.animationframe_nextframe,args)
    def set_nextframe(*args): return apply(adonthellc.animationframe_set_nextframe,args)
    def offx(*args): return apply(adonthellc.animationframe_offx,args)
    def offy(*args): return apply(adonthellc.animationframe_offy,args)
    def set_offset(*args): return apply(adonthellc.animationframe_set_offset,args)
    def get(*args): return apply(adonthellc.animationframe_get,args)
    def put(*args): return apply(adonthellc.animationframe_put,args)
    def __repr__(self):
        return "<C animationframe instance at %s>" % (self.this,)

class animationframePtr(animationframe):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = animationframe
adonthellc.animationframe_swigregister(animationframePtr)
class animation(drawable):
    __setmethods__ = {}
    for _s in [drawable]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,animation):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = animation.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [drawable]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = animation.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_animation,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_animation(self)
    def clear(*args): return apply(adonthellc.animation_clear,args)
    def play(*args): return apply(adonthellc.animation_play,args)
    def stop(*args): return apply(adonthellc.animation_stop,args)
    def playstate(*args): return apply(adonthellc.animation_playstate,args)
    def rewind(*args): return apply(adonthellc.animation_rewind,args)
    def next_frame(*args): return apply(adonthellc.animation_next_frame,args)
    def update(*args): return apply(adonthellc.animation_update,args)
    def draw(*args): return apply(adonthellc.animation_draw,args)
    def get(*args): return apply(adonthellc.animation_get,args)
    def load(*args): return apply(adonthellc.animation_load,args)
    def put(*args): return apply(adonthellc.animation_put,args)
    def save(*args): return apply(adonthellc.animation_save,args)
    def nbr_of_frames(*args): return apply(adonthellc.animation_nbr_of_frames,args)
    def nbr_of_images(*args): return apply(adonthellc.animation_nbr_of_images,args)
    def currentframe(*args): return apply(adonthellc.animation_currentframe,args)
    def set_currentframe(*args): return apply(adonthellc.animation_set_currentframe,args)
    def xoffset(*args): return apply(adonthellc.animation_xoffset,args)
    def yoffset(*args): return apply(adonthellc.animation_yoffset,args)
    def set_offset(*args): return apply(adonthellc.animation_set_offset,args)
    def get_frame(*args): return apply(adonthellc.animation_get_frame,args)
    def get_image(*args): return apply(adonthellc.animation_get_image,args)
    def insert_image(*args): return apply(adonthellc.animation_insert_image,args)
    def insert_frame(*args): return apply(adonthellc.animation_insert_frame,args)
    def delete_image(*args): return apply(adonthellc.animation_delete_image,args)
    def delete_frame(*args): return apply(adonthellc.animation_delete_frame,args)
    def zoom(*args): return apply(adonthellc.animation_zoom,args)
    def copy(*args): return apply(adonthellc.animation_copy,args)
    def __repr__(self):
        return "<C animation instance at %s>" % (self.this,)

class animationPtr(animation):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = animation
adonthellc.animation_swigregister(animationPtr)
class mapsquare_walkable:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare_walkable):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare_walkable.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare_walkable.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapsquare_walkable,args)
        self.thisown = 1
    def get(*args): return apply(adonthellc.mapsquare_walkable_get,args)
    def put(*args): return apply(adonthellc.mapsquare_walkable_put,args)
    def is_walkable_west(*args): return apply(adonthellc.mapsquare_walkable_is_walkable_west,args)
    def is_walkable_east(*args): return apply(adonthellc.mapsquare_walkable_is_walkable_east,args)
    def is_walkable_north(*args): return apply(adonthellc.mapsquare_walkable_is_walkable_north,args)
    def is_walkable_south(*args): return apply(adonthellc.mapsquare_walkable_is_walkable_south,args)
    def set_walkable_west(*args): return apply(adonthellc.mapsquare_walkable_set_walkable_west,args)
    def set_walkable_east(*args): return apply(adonthellc.mapsquare_walkable_set_walkable_east,args)
    def set_walkable_north(*args): return apply(adonthellc.mapsquare_walkable_set_walkable_north,args)
    def set_walkable_south(*args): return apply(adonthellc.mapsquare_walkable_set_walkable_south,args)
    def get_walkable(*args): return apply(adonthellc.mapsquare_walkable_get_walkable,args)
    def set_walkable(*args): return apply(adonthellc.mapsquare_walkable_set_walkable,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare_walkable(self)
    def __repr__(self):
        return "<C mapsquare_walkable instance at %s>" % (self.this,)

class mapsquare_walkablePtr(mapsquare_walkable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare_walkable
adonthellc.mapsquare_walkable_swigregister(mapsquare_walkablePtr)
class mapsquare_walkable_area(drawable):
    __setmethods__ = {}
    for _s in [drawable]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare_walkable_area):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare_walkable_area.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [drawable]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare_walkable_area.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare_walkable_area(self)
    def clear(*args): return apply(adonthellc.mapsquare_walkable_area_clear,args)
    def draw(*args): return apply(adonthellc.mapsquare_walkable_area_draw,args)
    def area_length(*args): return apply(adonthellc.mapsquare_walkable_area_area_length,args)
    def area_height(*args): return apply(adonthellc.mapsquare_walkable_area_area_height,args)
    def get_square(*args): return apply(adonthellc.mapsquare_walkable_area_get_square,args)
    def resize_area(*args): return apply(adonthellc.mapsquare_walkable_area_resize_area,args)
    def base_x(*args): return apply(adonthellc.mapsquare_walkable_area_base_x,args)
    def base_y(*args): return apply(adonthellc.mapsquare_walkable_area_base_y,args)
    def set_base(*args): return apply(adonthellc.mapsquare_walkable_area_set_base,args)
    def get(*args): return apply(adonthellc.mapsquare_walkable_area_get,args)
    def put(*args): return apply(adonthellc.mapsquare_walkable_area_put,args)
    def copy(*args): return apply(adonthellc.mapsquare_walkable_area_copy,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C mapsquare_walkable_area instance at %s>" % (self.this,)

class mapsquare_walkable_areaPtr(mapsquare_walkable_area):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare_walkable_area
adonthellc.mapsquare_walkable_area_swigregister(mapsquare_walkable_areaPtr)
class mapsquare_tile:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare_tile):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare_tile.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare_tile.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapsquare_tile,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare_tile(self)
    def __repr__(self):
        return "<C mapsquare_tile instance at %s>" % (self.this,)

class mapsquare_tilePtr(mapsquare_tile):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare_tile
adonthellc.mapsquare_tile_swigregister(mapsquare_tilePtr)
class mapsquare_char:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare_char):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare_char.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare_char.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapsquare_char,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare_char(self)
    def __repr__(self):
        return "<C mapsquare_char instance at %s>" % (self.this,)

class mapsquare_charPtr(mapsquare_char):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare_char
adonthellc.mapsquare_char_swigregister(mapsquare_charPtr)
class mapsquare(mapsquare_walkable):
    __setmethods__ = {}
    for _s in [mapsquare_walkable]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [mapsquare_walkable]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapsquare,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare(self)
    def x(*args): return apply(adonthellc.mapsquare_x,args)
    def y(*args): return apply(adonthellc.mapsquare_y,args)
    def is_free(*args): return apply(adonthellc.mapsquare_is_free,args)
    def whoshere(*args): return apply(adonthellc.mapsquare_whoshere,args)
    __setmethods__["g"] = adonthellc.mapsquare_g_set
    __getmethods__["g"] = adonthellc.mapsquare_g_get
    __setmethods__["h"] = adonthellc.mapsquare_h_set
    __getmethods__["h"] = adonthellc.mapsquare_h_get
    __setmethods__["f"] = adonthellc.mapsquare_f_set
    __getmethods__["f"] = adonthellc.mapsquare_f_get
    __setmethods__["parent"] = adonthellc.mapsquare_parent_set
    __getmethods__["parent"] = adonthellc.mapsquare_parent_get
    __setmethods__["can_use_for_pathfinding"] = adonthellc.mapsquare_can_use_for_pathfinding_set
    __getmethods__["can_use_for_pathfinding"] = adonthellc.mapsquare_can_use_for_pathfinding_get
    def __repr__(self):
        return "<C mapsquare instance at %s>" % (self.this,)

class mapsquarePtr(mapsquare):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare
adonthellc.mapsquare_swigregister(mapsquarePtr)
class mapsquare_area:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapsquare_area):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapsquare_area.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapsquare_area.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapsquare_area,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapsquare_area(self)
    def clear(*args): return apply(adonthellc.mapsquare_area_clear,args)
    def area_length(*args): return apply(adonthellc.mapsquare_area_area_length,args)
    def area_height(*args): return apply(adonthellc.mapsquare_area_area_height,args)
    def get_square(*args): return apply(adonthellc.mapsquare_area_get_square,args)
    def resize_area(*args): return apply(adonthellc.mapsquare_area_resize_area,args)
    def __repr__(self):
        return "<C mapsquare_area instance at %s>" % (self.this,)

class mapsquare_areaPtr(mapsquare_area):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapsquare_area
adonthellc.mapsquare_area_swigregister(mapsquare_areaPtr)
class mapobject(mapsquare_walkable_area):
    __setmethods__ = {}
    for _s in [mapsquare_walkable_area]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapobject):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapobject.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [mapsquare_walkable_area]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapobject.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapobject,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapobject(self)
    def clear(*args): return apply(adonthellc.mapobject_clear,args)
    def update(*args): return apply(adonthellc.mapobject_update,args)
    def draw(*args): return apply(adonthellc.mapobject_draw,args)
    def draw_from_base(*args): return apply(adonthellc.mapobject_draw_from_base,args)
    def get(*args): return apply(adonthellc.mapobject_get,args)
    def load(*args): return apply(adonthellc.mapobject_load,args)
    def put(*args): return apply(adonthellc.mapobject_put,args)
    def save(*args): return apply(adonthellc.mapobject_save,args)
    def nbr_of_animations(*args): return apply(adonthellc.mapobject_nbr_of_animations,args)
    def get_animation(*args): return apply(adonthellc.mapobject_get_animation,args)
    def insert_animation(*args): return apply(adonthellc.mapobject_insert_animation,args)
    def delete_animation(*args): return apply(adonthellc.mapobject_delete_animation,args)
    def copy(*args): return apply(adonthellc.mapobject_copy,args)
    def __repr__(self):
        return "<C mapobject instance at %s>" % (self.this,)

class mapobjectPtr(mapobject):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapobject
adonthellc.mapobject_swigregister(mapobjectPtr)
class mapcharacter(mapsquare_walkable_area,character_base):
    __setmethods__ = {}
    for _s in [mapsquare_walkable_area,character_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapcharacter):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapcharacter.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [mapsquare_walkable_area,character_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapcharacter.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapcharacter,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapcharacter(self)
    def clear(*args): return apply(adonthellc.mapcharacter_clear,args)
    def filename(*args): return apply(adonthellc.mapcharacter_filename,args)
    def update(*args): return apply(adonthellc.mapcharacter_update,args)
    def draw(*args): return apply(adonthellc.mapcharacter_draw,args)
    def draw_bubble(*args): return apply(adonthellc.mapcharacter_draw_bubble,args)
    def get(*args): return apply(adonthellc.mapcharacter_get,args)
    def load(*args): return apply(adonthellc.mapcharacter_load,args)
    def put(*args): return apply(adonthellc.mapcharacter_put,args)
    def save(*args): return apply(adonthellc.mapcharacter_save,args)
    def get_state(*args): return apply(adonthellc.mapcharacter_get_state,args)
    def put_state(*args): return apply(adonthellc.mapcharacter_put_state,args)
    def set_map(*args): return apply(adonthellc.mapcharacter_set_map,args)
    def remove_from_map(*args): return apply(adonthellc.mapcharacter_remove_from_map,args)
    def mymap(*args): return apply(adonthellc.mapcharacter_mymap,args)
    def stand_north(*args): return apply(adonthellc.mapcharacter_stand_north,args)
    def stand_south(*args): return apply(adonthellc.mapcharacter_stand_south,args)
    def stand_east(*args): return apply(adonthellc.mapcharacter_stand_east,args)
    def stand_west(*args): return apply(adonthellc.mapcharacter_stand_west,args)
    def stand(*args): return apply(adonthellc.mapcharacter_stand,args)
    def go_north(*args): return apply(adonthellc.mapcharacter_go_north,args)
    def go_south(*args): return apply(adonthellc.mapcharacter_go_south,args)
    def go_east(*args): return apply(adonthellc.mapcharacter_go_east,args)
    def go_west(*args): return apply(adonthellc.mapcharacter_go_west,args)
    def can_go_north(*args): return apply(adonthellc.mapcharacter_can_go_north,args)
    def can_go_south(*args): return apply(adonthellc.mapcharacter_can_go_south,args)
    def can_go_east(*args): return apply(adonthellc.mapcharacter_can_go_east,args)
    def can_go_west(*args): return apply(adonthellc.mapcharacter_can_go_west,args)
    def look_invert(*args): return apply(adonthellc.mapcharacter_look_invert,args)
    def whosnext(*args): return apply(adonthellc.mapcharacter_whosnext,args)
    def speak(*args): return apply(adonthellc.mapcharacter_speak,args)
    def is_speaking(*args): return apply(adonthellc.mapcharacter_is_speaking,args)
    def get_bubble(*args): return apply(adonthellc.mapcharacter_get_bubble,args)
    def set_offset(*args): return apply(adonthellc.mapcharacter_set_offset,args)
    def remove_from_pos(*args): return apply(adonthellc.mapcharacter_remove_from_pos,args)
    def jump_to(*args): return apply(adonthellc.mapcharacter_jump_to,args)
    def submap(*args): return apply(adonthellc.mapcharacter_submap,args)
    def posx(*args): return apply(adonthellc.mapcharacter_posx,args)
    def posy(*args): return apply(adonthellc.mapcharacter_posy,args)
    def offx(*args): return apply(adonthellc.mapcharacter_offx,args)
    def offy(*args): return apply(adonthellc.mapcharacter_offy,args)
    def currentmove(*args): return apply(adonthellc.mapcharacter_currentmove,args)
    def set_goal(*args): return apply(adonthellc.mapcharacter_set_goal,args)
    def follow_path(*args): return apply(adonthellc.mapcharacter_follow_path,args)
    def goal_reached(*args): return apply(adonthellc.mapcharacter_goal_reached,args)
    def set_schedule(*args): return apply(adonthellc.mapcharacter_set_schedule,args)
    def schedule_file(*args): return apply(adonthellc.mapcharacter_schedule_file,args)
    def is_schedule_activated(*args): return apply(adonthellc.mapcharacter_is_schedule_activated,args)
    def set_schedule_active(*args): return apply(adonthellc.mapcharacter_set_schedule_active,args)
    def set_action(*args): return apply(adonthellc.mapcharacter_set_action,args)
    def action_file(*args): return apply(adonthellc.mapcharacter_action_file,args)
    def is_action_activated(*args): return apply(adonthellc.mapcharacter_is_action_activated,args)
    def set_action_active(*args): return apply(adonthellc.mapcharacter_set_action_active,args)
    def launch_action(*args): return apply(adonthellc.mapcharacter_launch_action,args)
    def get_animation(*args): return apply(adonthellc.mapcharacter_get_animation,args)
    def copy(*args): return apply(adonthellc.mapcharacter_copy,args)
    def __repr__(self):
        return "<C mapcharacter instance at %s>" % (self.this,)

class mapcharacterPtr(mapcharacter):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapcharacter
adonthellc.mapcharacter_swigregister(mapcharacterPtr)
class path:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,path):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = path.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = path.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def clear(*args): return apply(adonthellc.path_clear,args)
    def calculate(*args): return apply(adonthellc.path_calculate,args)
    def nbr_moves(*args): return apply(adonthellc.path_nbr_moves,args)
    def get_move(*args): return apply(adonthellc.path_get_move,args)
    def get_state(*args): return apply(adonthellc.path_get_state,args)
    def put_state(*args): return apply(adonthellc.path_put_state,args)
    def __init__(self,*args):
        self.this = apply(adonthellc.new_path,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_path(self)
    def __repr__(self):
        return "<C path instance at %s>" % (self.this,)

class pathPtr(path):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = path
adonthellc.path_swigregister(pathPtr)
class character(mapcharacter):
    __setmethods__ = {}
    for _s in [mapcharacter]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,character):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = character.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [mapcharacter]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = character.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_character,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_character(self)
    def __repr__(self):
        return "<C character instance at %s>" % (self.this,)

class characterPtr(character):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = character
adonthellc.character_swigregister(characterPtr)
class base_map_event(event):
    __setmethods__ = {}
    for _s in [event]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,base_map_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = base_map_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [event]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = base_map_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_base_map_event,args)
        self.thisown = 1
    def save(*args): return apply(adonthellc.base_map_event_save,args)
    def load(*args): return apply(adonthellc.base_map_event_load,args)
    __setmethods__["submap"] = adonthellc.base_map_event_submap_set
    __getmethods__["submap"] = adonthellc.base_map_event_submap_get
    __setmethods__["x"] = adonthellc.base_map_event_x_set
    __getmethods__["x"] = adonthellc.base_map_event_x_get
    __setmethods__["y"] = adonthellc.base_map_event_y_set
    __getmethods__["y"] = adonthellc.base_map_event_y_get
    __setmethods__["dir"] = adonthellc.base_map_event_dir_set
    __getmethods__["dir"] = adonthellc.base_map_event_dir_get
    __setmethods__["map"] = adonthellc.base_map_event_map_set
    __getmethods__["map"] = adonthellc.base_map_event_map_get
    __setmethods__["c"] = adonthellc.base_map_event_c_set
    __getmethods__["c"] = adonthellc.base_map_event_c_get
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_base_map_event(self)
    def __repr__(self):
        return "<C base_map_event instance at %s>" % (self.this,)

class base_map_eventPtr(base_map_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = base_map_event
adonthellc.base_map_event_swigregister(base_map_eventPtr)
class enter_event(base_map_event):
    __setmethods__ = {}
    for _s in [base_map_event]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,enter_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = enter_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [base_map_event]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = enter_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_enter_event,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_enter_event(self)
    def __repr__(self):
        return "<C enter_event instance at %s>" % (self.this,)

class enter_eventPtr(enter_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = enter_event
adonthellc.enter_event_swigregister(enter_eventPtr)
class leave_event(base_map_event):
    __setmethods__ = {}
    for _s in [base_map_event]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,leave_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = leave_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [base_map_event]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = leave_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_leave_event,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_leave_event(self)
    def __repr__(self):
        return "<C leave_event instance at %s>" % (self.this,)

class leave_eventPtr(leave_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = leave_event
adonthellc.leave_event_swigregister(leave_eventPtr)
class action_event(base_map_event):
    __setmethods__ = {}
    for _s in [base_map_event]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,action_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = action_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [base_map_event]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = action_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_action_event,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_action_event(self)
    def __repr__(self):
        return "<C action_event instance at %s>" % (self.this,)

class action_eventPtr(action_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = action_event
adonthellc.action_event_swigregister(action_eventPtr)
class landmap(event_list):
    __setmethods__ = {}
    for _s in [event_list]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,landmap):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = landmap.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [event_list]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = landmap.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_landmap,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_landmap(self)
    def clear(*args): return apply(adonthellc.landmap_clear,args)
    def nbr_of_mapobjects(*args): return apply(adonthellc.landmap_nbr_of_mapobjects,args)
    def nbr_of_submaps(*args): return apply(adonthellc.landmap_nbr_of_submaps,args)
    def nbr_of_mapcharacters(*args): return apply(adonthellc.landmap_nbr_of_mapcharacters,args)
    def filename(*args): return apply(adonthellc.landmap_filename,args)
    def get_mapcharacter(*args): return apply(adonthellc.landmap_get_mapcharacter,args)
    def get_mapobject(*args): return apply(adonthellc.landmap_get_mapobject,args)
    def get_submap(*args): return apply(adonthellc.landmap_get_submap,args)
    def update(*args): return apply(adonthellc.landmap_update,args)
    def get(*args): return apply(adonthellc.landmap_get,args)
    def load(*args): return apply(adonthellc.landmap_load,args)
    def put(*args): return apply(adonthellc.landmap_put,args)
    def save(*args): return apply(adonthellc.landmap_save,args)
    def get_state(*args): return apply(adonthellc.landmap_get_state,args)
    def put_state(*args): return apply(adonthellc.landmap_put_state,args)
    def put_mapobject(*args): return apply(adonthellc.landmap_put_mapobject,args)
    def remove_mapobject(*args): return apply(adonthellc.landmap_remove_mapobject,args)
    def insert_submap(*args): return apply(adonthellc.landmap_insert_submap,args)
    def delete_submap(*args): return apply(adonthellc.landmap_delete_submap,args)
    def insert_mapobject(*args): return apply(adonthellc.landmap_insert_mapobject,args)
    def delete_mapobject(*args): return apply(adonthellc.landmap_delete_mapobject,args)
    def __repr__(self):
        return "<C landmap instance at %s>" % (self.this,)

class landmapPtr(landmap):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = landmap
adonthellc.landmap_swigregister(landmapPtr)
class mapview(drawable):
    __setmethods__ = {}
    for _s in [drawable]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,mapview):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = mapview.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [drawable]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = mapview.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_mapview,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_mapview(self)
    def attach_map(*args): return apply(adonthellc.mapview_attach_map,args)
    def detach_map(*args): return apply(adonthellc.mapview_detach_map,args)
    def set_pos(*args): return apply(adonthellc.mapview_set_pos,args)
    def center_on(*args): return apply(adonthellc.mapview_center_on,args)
    def currentsubmap(*args): return apply(adonthellc.mapview_currentsubmap,args)
    def posx(*args): return apply(adonthellc.mapview_posx,args)
    def posy(*args): return apply(adonthellc.mapview_posy,args)
    def offx(*args): return apply(adonthellc.mapview_offx,args)
    def offy(*args): return apply(adonthellc.mapview_offy,args)
    def can_scroll_right(*args): return apply(adonthellc.mapview_can_scroll_right,args)
    def can_scroll_left(*args): return apply(adonthellc.mapview_can_scroll_left,args)
    def can_scroll_up(*args): return apply(adonthellc.mapview_can_scroll_up,args)
    def can_scroll_down(*args): return apply(adonthellc.mapview_can_scroll_down,args)
    def scroll_right(*args): return apply(adonthellc.mapview_scroll_right,args)
    def scroll_left(*args): return apply(adonthellc.mapview_scroll_left,args)
    def scroll_down(*args): return apply(adonthellc.mapview_scroll_down,args)
    def scroll_up(*args): return apply(adonthellc.mapview_scroll_up,args)
    def get_state(*args): return apply(adonthellc.mapview_get_state,args)
    def put_state(*args): return apply(adonthellc.mapview_put_state,args)
    def resize(*args): return apply(adonthellc.mapview_resize,args)
    def set_schedule(*args): return apply(adonthellc.mapview_set_schedule,args)
    def schedule_file(*args): return apply(adonthellc.mapview_schedule_file,args)
    def update(*args): return apply(adonthellc.mapview_update,args)
    def draw(*args): return apply(adonthellc.mapview_draw,args)
    def __repr__(self):
        return "<C mapview instance at %s>" % (self.this,)

class mapviewPtr(mapview):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = mapview
adonthellc.mapview_swigregister(mapviewPtr)
class adonthell:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,adonthell):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = adonthell.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = adonthell.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_adonthell,args)
        self.thisown = 1
    def main(*args): return apply(adonthellc.adonthell_main,args)
    def main_loop(*args): return apply(adonthellc.adonthell_main_loop,args)
    def main_quit(*args): return apply(adonthellc.adonthell_main_quit,args)
    def fade_out(*args): return apply(adonthellc.adonthell_fade_out,args)
    def fade_in(*args): return apply(adonthellc.adonthell_fade_in,args)
    def get_state(*args): return apply(adonthellc.adonthell_get_state,args)
    def put_state(*args): return apply(adonthellc.adonthell_put_state,args)
    def control_active(*args): return apply(adonthellc.adonthell_control_active,args)
    def set_control_active(*args): return apply(adonthellc.adonthell_set_control_active,args)
    def get_landmap(*args): return apply(adonthellc.adonthell_get_landmap,args)
    def update_map(*args): return apply(adonthellc.adonthell_update_map,args)
    def set_update_map(*args): return apply(adonthellc.adonthell_set_update_map,args)
    def get_mapview(*args): return apply(adonthellc.adonthell_get_mapview,args)
    def draw(*args): return apply(adonthellc.adonthell_draw,args)
    def set_mapview_schedule(*args): return apply(adonthellc.adonthell_set_mapview_schedule,args)
    def mapview_start(*args): return apply(adonthellc.adonthell_mapview_start,args)
    def mapview_stop(*args): return apply(adonthellc.adonthell_mapview_stop,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_adonthell(self)
    def __repr__(self):
        return "<C adonthell instance at %s>" % (self.this,)

class adonthellPtr(adonthell):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = adonthell
adonthellc.adonthell_swigregister(adonthellPtr)
class win_manager:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_manager):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_manager.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_manager.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_manager,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_manager(self)
    def add(*args): return apply(adonthellc.win_manager_add,args)
    def remove(*args): return apply(adonthellc.win_manager_remove,args)
    def update(*args): return apply(adonthellc.win_manager_update,args)
    def input_update(*args): return apply(adonthellc.win_manager_input_update,args)
    def draw(*args): return apply(adonthellc.win_manager_draw,args)
    def set_focus(*args): return apply(adonthellc.win_manager_set_focus,args)
    def destroy(*args): return apply(adonthellc.win_manager_destroy,args)
    def __repr__(self):
        return "<C win_manager instance at %s>" % (self.this,)

class win_managerPtr(win_manager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_manager
adonthellc.win_manager_swigregister(win_managerPtr)
class win_font:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_font):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_font.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_font.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_font,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_font(self)
    def load(*args): return apply(adonthellc.win_font_load,args)
    def in_table(*args): return apply(adonthellc.win_font_in_table,args)
    def height(*args): return apply(adonthellc.win_font_height,args)
    def length(*args): return apply(adonthellc.win_font_length,args)
    __setmethods__["cursor"] = adonthellc.win_font_cursor_set
    __getmethods__["cursor"] = adonthellc.win_font_cursor_get
    def __repr__(self):
        return "<C win_font instance at %s>" % (self.this,)

class win_fontPtr(win_font):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_font
adonthellc.win_font_swigregister(win_fontPtr)
class win_event:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_event):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_event.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_event.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_event,args)
        self.thisown = 1
    def set_return_code(*args): return apply(adonthellc.win_event_set_return_code,args)
    def update(*args): return apply(adonthellc.win_event_update,args)
    def py_signal_connect(*args): return apply(adonthellc.win_event_py_signal_connect,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_event(self)
    def __repr__(self):
        return "<C win_event instance at %s>" % (self.this,)

class win_eventPtr(win_event):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_event
adonthellc.win_event_swigregister(win_eventPtr)
class win_border:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_border):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_border.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_border.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_border,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_border(self)
    def load(*args): return apply(adonthellc.win_border_load,args)
    def update(*args): return apply(adonthellc.win_border_update,args)
    def destroy(*args): return apply(adonthellc.win_border_destroy,args)
    def draw(*args): return apply(adonthellc.win_border_draw,args)
    def set_visible_border(*args): return apply(adonthellc.win_border_set_visible_border,args)
    def set_brightness_border(*args): return apply(adonthellc.win_border_set_brightness_border,args)
    def set_border(*args): return apply(adonthellc.win_border_set_border,args)
    def set_trans_border(*args): return apply(adonthellc.win_border_set_trans_border,args)
    def length_border(*args): return apply(adonthellc.win_border_length_border,args)
    def height_border(*args): return apply(adonthellc.win_border_height_border,args)
    def __repr__(self):
        return "<C win_border instance at %s>" % (self.this,)

class win_borderPtr(win_border):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_border
adonthellc.win_border_swigregister(win_borderPtr)
class win_background:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_background):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_background.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_background.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_background,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_background(self)
    def load(*args): return apply(adonthellc.win_background_load,args)
    def update(*args): return apply(adonthellc.win_background_update,args)
    def destroy(*args): return apply(adonthellc.win_background_destroy,args)
    def draw(*args): return apply(adonthellc.win_background_draw,args)
    def set_visible_background(*args): return apply(adonthellc.win_background_set_visible_background,args)
    def set_background(*args): return apply(adonthellc.win_background_set_background,args)
    def set_brightness_background(*args): return apply(adonthellc.win_background_set_brightness_background,args)
    def set_trans_background(*args): return apply(adonthellc.win_background_set_trans_background,args)
    def __repr__(self):
        return "<C win_background instance at %s>" % (self.this,)

class win_backgroundPtr(win_background):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_background
adonthellc.win_background_swigregister(win_backgroundPtr)
class win_base(win_event,win_border,drawing_area,win_background):
    __setmethods__ = {}
    for _s in [win_event,win_border,drawing_area,win_background]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_base):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_base.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_event,win_border,drawing_area,win_background]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_base.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_base,args)
        self.thisown = 1
    def x(*args): return apply(adonthellc.win_base_x,args)
    def y(*args): return apply(adonthellc.win_base_y,args)
    def pad_x(*args): return apply(adonthellc.win_base_pad_x,args)
    def pad_y(*args): return apply(adonthellc.win_base_pad_y,args)
    def real_x(*args): return apply(adonthellc.win_base_real_x,args)
    def real_y(*args): return apply(adonthellc.win_base_real_y,args)
    def move(*args): return apply(adonthellc.win_base_move,args)
    def resize(*args): return apply(adonthellc.win_base_resize,args)
    def is_visible(*args): return apply(adonthellc.win_base_is_visible,args)
    def set_visible(*args): return apply(adonthellc.win_base_set_visible,args)
    def is_activate(*args): return apply(adonthellc.win_base_is_activate,args)
    def set_activate(*args): return apply(adonthellc.win_base_set_activate,args)
    def is_focus(*args): return apply(adonthellc.win_base_is_focus,args)
    def set_focus(*args): return apply(adonthellc.win_base_set_focus,args)
    def is_trans(*args): return apply(adonthellc.win_base_is_trans,args)
    def set_trans(*args): return apply(adonthellc.win_base_set_trans,args)
    def is_brightness(*args): return apply(adonthellc.win_base_is_brightness,args)
    def set_brightness(*args): return apply(adonthellc.win_base_set_brightness,args)
    def set_align(*args): return apply(adonthellc.win_base_set_align,args)
    def align(*args): return apply(adonthellc.win_base_align,args)
    def is_can_be_selected(*args): return apply(adonthellc.win_base_is_can_be_selected,args)
    def set_can_be_selected(*args): return apply(adonthellc.win_base_set_can_be_selected,args)
    def update(*args): return apply(adonthellc.win_base_update,args)
    def draw(*args): return apply(adonthellc.win_base_draw,args)
    def input_update(*args): return apply(adonthellc.win_base_input_update,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_base(self)
    def __repr__(self):
        return "<C win_base instance at %s>" % (self.this,)

class win_basePtr(win_base):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_base
adonthellc.win_base_swigregister(win_basePtr)
class win_container(win_base):
    __setmethods__ = {}
    for _s in [win_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_container):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_container.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_container.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_container,args)
        self.thisown = 1
    def move(*args): return apply(adonthellc.win_container_move,args)
    def resize(*args): return apply(adonthellc.win_container_resize,args)
    def add(*args): return apply(adonthellc.win_container_add,args)
    def remove(*args): return apply(adonthellc.win_container_remove,args)
    def remove_all(*args): return apply(adonthellc.win_container_remove_all,args)
    def destroy(*args): return apply(adonthellc.win_container_destroy,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_container(self)
    def update(*args): return apply(adonthellc.win_container_update,args)
    def input_update(*args): return apply(adonthellc.win_container_input_update,args)
    def draw(*args): return apply(adonthellc.win_container_draw,args)
    def set_visible_all(*args): return apply(adonthellc.win_container_set_visible_all,args)
    def set_brightness(*args): return apply(adonthellc.win_container_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_container_set_trans,args)
    def set_space_with_border(*args): return apply(adonthellc.win_container_set_space_with_border,args)
    def set_space_with_object(*args): return apply(adonthellc.win_container_set_space_with_object,args)
    def space_with_border(*args): return apply(adonthellc.win_container_space_with_border,args)
    def space_with_object(*args): return apply(adonthellc.win_container_space_with_object,args)
    def set_layout(*args): return apply(adonthellc.win_container_set_layout,args)
    def set_focus_object(*args): return apply(adonthellc.win_container_set_focus_object,args)
    def focus_object(*args): return apply(adonthellc.win_container_focus_object,args)
    def __repr__(self):
        return "<C win_container instance at %s>" % (self.this,)

class win_containerPtr(win_container):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_container
adonthellc.win_container_swigregister(win_containerPtr)
class win_image(image,win_base):
    __setmethods__ = {}
    for _s in [image,win_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_image):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_image.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [image,win_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_image.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_image,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_image(self)
    def draw(*args): return apply(adonthellc.win_image_draw,args)
    def update(*args): return apply(adonthellc.win_image_update,args)
    def input_update(*args): return apply(adonthellc.win_image_input_update,args)
    def set_brightness(*args): return apply(adonthellc.win_image_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_image_set_trans,args)
    def pack(*args): return apply(adonthellc.win_image_pack,args)
    def set_auto_refresh(*args): return apply(adonthellc.win_image_set_auto_refresh,args)
    def __repr__(self):
        return "<C win_image instance at %s>" % (self.this,)

class win_imagePtr(win_image):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_image
adonthellc.win_image_swigregister(win_imagePtr)
class label(image):
    __setmethods__ = {}
    for _s in [image]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,label):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = label.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [image]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = label.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_label,args)
        self.thisown = 1
    def text_string(*args): return apply(adonthellc.label_text_string,args)
    def text_char(*args): return apply(adonthellc.label_text_char,args)
    def set_text(*args): return apply(adonthellc.label_set_text,args)
    def add_text(*args): return apply(adonthellc.label_add_text,args)
    def set_font(*args): return apply(adonthellc.label_set_font,args)
    def set_form(*args): return apply(adonthellc.label_set_form,args)
    def set_cursor_visible(*args): return apply(adonthellc.label_set_cursor_visible,args)
    def set_cursor_moveable(*args): return apply(adonthellc.label_set_cursor_moveable,args)
    def resize(*args): return apply(adonthellc.label_resize,args)
    def update(*args): return apply(adonthellc.label_update,args)
    def input_update(*args): return apply(adonthellc.label_input_update,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_label(self)
    def __repr__(self):
        return "<C label instance at %s>" % (self.this,)

class labelPtr(label):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = label
adonthellc.label_swigregister(labelPtr)
class win_label(label,win_base):
    __setmethods__ = {}
    for _s in [label,win_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_label):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_label.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [label,win_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_label.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_label,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_label(self)
    def draw(*args): return apply(adonthellc.win_label_draw,args)
    def update(*args): return apply(adonthellc.win_label_update,args)
    def input_update(*args): return apply(adonthellc.win_label_input_update,args)
    def set_brightness(*args): return apply(adonthellc.win_label_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_label_set_trans,args)
    def pack(*args): return apply(adonthellc.win_label_pack,args)
    def set_auto_refresh(*args): return apply(adonthellc.win_label_set_auto_refresh,args)
    def __repr__(self):
        return "<C win_label instance at %s>" % (self.this,)

class win_labelPtr(win_label):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_label
adonthellc.win_label_swigregister(win_labelPtr)
class label_input(label):
    __setmethods__ = {}
    for _s in [label]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,label_input):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = label_input.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [label]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = label_input.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def input_update(*args): return apply(adonthellc.label_input_input_update,args)
    def __init__(self,*args):
        self.this = apply(adonthellc.new_label_input,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_label_input(self)
    def __repr__(self):
        return "<C label_input instance at %s>" % (self.this,)

class label_inputPtr(label_input):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = label_input
adonthellc.label_input_swigregister(label_inputPtr)
class win_write(label_input,win_base):
    __setmethods__ = {}
    for _s in [label_input,win_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_write):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_write.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [label_input,win_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_write.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_write,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_write(self)
    def draw(*args): return apply(adonthellc.win_write_draw,args)
    def update(*args): return apply(adonthellc.win_write_update,args)
    def input_update(*args): return apply(adonthellc.win_write_input_update,args)
    def set_brightness(*args): return apply(adonthellc.win_write_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_write_set_trans,args)
    def pack(*args): return apply(adonthellc.win_write_pack,args)
    def set_auto_refresh(*args): return apply(adonthellc.win_write_set_auto_refresh,args)
    def __repr__(self):
        return "<C win_write instance at %s>" % (self.this,)

class win_writePtr(win_write):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_write
adonthellc.win_write_swigregister(win_writePtr)
class win_mapview(mapview,win_base):
    __setmethods__ = {}
    for _s in [mapview,win_base]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_mapview):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_mapview.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [mapview,win_base]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_mapview.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_mapview,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_mapview(self)
    def draw(*args): return apply(adonthellc.win_mapview_draw,args)
    def update(*args): return apply(adonthellc.win_mapview_update,args)
    def input_update(*args): return apply(adonthellc.win_mapview_input_update,args)
    def set_brightness(*args): return apply(adonthellc.win_mapview_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_mapview_set_trans,args)
    def pack(*args): return apply(adonthellc.win_mapview_pack,args)
    def set_auto_refresh(*args): return apply(adonthellc.win_mapview_set_auto_refresh,args)
    def __repr__(self):
        return "<C win_mapview instance at %s>" % (self.this,)

class win_mapviewPtr(win_mapview):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_mapview
adonthellc.win_mapview_swigregister(win_mapviewPtr)
class win_scrollbar:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_scrollbar):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_scrollbar.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_scrollbar.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_scrollbar,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_scrollbar(self)
    def update_bar(*args): return apply(adonthellc.win_scrollbar_update_bar,args)
    def load(*args): return apply(adonthellc.win_scrollbar_load,args)
    def update_back(*args): return apply(adonthellc.win_scrollbar_update_back,args)
    def destroy(*args): return apply(adonthellc.win_scrollbar_destroy,args)
    def set_scrollbar(*args): return apply(adonthellc.win_scrollbar_set_scrollbar,args)
    def set_visible_scrollbar(*args): return apply(adonthellc.win_scrollbar_set_visible_scrollbar,args)
    def set_trans_scrollbar(*args): return apply(adonthellc.win_scrollbar_set_trans_scrollbar,args)
    def set_brightness_scrollbar(*args): return apply(adonthellc.win_scrollbar_set_brightness_scrollbar,args)
    def draw(*args): return apply(adonthellc.win_scrollbar_draw,args)
    def height_bar(*args): return apply(adonthellc.win_scrollbar_height_bar,args)
    def __repr__(self):
        return "<C win_scrollbar instance at %s>" % (self.this,)

class win_scrollbarPtr(win_scrollbar):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_scrollbar
adonthellc.win_scrollbar_swigregister(win_scrollbarPtr)
class win_scroll(win_container,win_scrollbar):
    __setmethods__ = {}
    for _s in [win_container,win_scrollbar]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_scroll):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_scroll.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_container,win_scrollbar]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_scroll.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_scroll,args)
        self.thisown = 1
    def amplitude(*args): return apply(adonthellc.win_scroll_amplitude,args)
    def add(*args): return apply(adonthellc.win_scroll_add,args)
    def remove(*args): return apply(adonthellc.win_scroll_remove,args)
    def remove_all(*args): return apply(adonthellc.win_scroll_remove_all,args)
    def resize(*args): return apply(adonthellc.win_scroll_resize,args)
    def destroy(*args): return apply(adonthellc.win_scroll_destroy,args)
    def draw(*args): return apply(adonthellc.win_scroll_draw,args)
    def update(*args): return apply(adonthellc.win_scroll_update,args)
    def input_update(*args): return apply(adonthellc.win_scroll_input_update,args)
    def set_space_between_border(*args): return apply(adonthellc.win_scroll_set_space_between_border,args)
    def set_space_between_object(*args): return apply(adonthellc.win_scroll_set_space_between_object,args)
    def cursor_y(*args): return apply(adonthellc.win_scroll_cursor_y,args)
    def set_auto_scrollbar(*args): return apply(adonthellc.win_scroll_set_auto_scrollbar,args)
    def set_auto_refresh(*args): return apply(adonthellc.win_scroll_set_auto_refresh,args)
    def set_brightness(*args): return apply(adonthellc.win_scroll_set_brightness,args)
    def set_trans(*args): return apply(adonthellc.win_scroll_set_trans,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_scroll(self)
    def __repr__(self):
        return "<C win_scroll instance at %s>" % (self.this,)

class win_scrollPtr(win_scroll):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_scroll
adonthellc.win_scroll_swigregister(win_scrollPtr)
class win_select(win_scroll):
    __setmethods__ = {}
    for _s in [win_scroll]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_select):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_select.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_scroll]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_select.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_select,args)
        self.thisown = 1
    def input_update(*args): return apply(adonthellc.win_select_input_update,args)
    def add(*args): return apply(adonthellc.win_select_add,args)
    def remove(*args): return apply(adonthellc.win_select_remove,args)
    def remove_all(*args): return apply(adonthellc.win_select_remove_all,args)
    def set_mode(*args): return apply(adonthellc.win_select_set_mode,args)
    def set_border_select(*args): return apply(adonthellc.win_select_set_border_select,args)
    def set_circle(*args): return apply(adonthellc.win_select_set_circle,args)
    def is_circle(*args): return apply(adonthellc.win_select_is_circle,args)
    def set_default(*args): return apply(adonthellc.win_select_set_default,args)
    def set_default_object(*args): return apply(adonthellc.win_select_set_default_object,args)
    def set_default_position(*args): return apply(adonthellc.win_select_set_default_position,args)
    def get_selected_object(*args): return apply(adonthellc.win_select_get_selected_object,args)
    def get_selected_position(*args): return apply(adonthellc.win_select_get_selected_position,args)
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_select(self)
    def __repr__(self):
        return "<C win_select instance at %s>" % (self.this,)

class win_selectPtr(win_select):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_select
adonthellc.win_select_swigregister(win_selectPtr)
class win_theme:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,win_theme):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = win_theme.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = win_theme.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_win_theme,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_win_theme(self)
    def destroy(*args): return apply(adonthellc.win_theme_destroy,args)
    def __repr__(self):
        return "<C win_theme instance at %s>" % (self.this,)

class win_themePtr(win_theme):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = win_theme
adonthellc.win_theme_swigregister(win_themePtr)
class text_bubble(win_label):
    __setmethods__ = {}
    for _s in [win_label]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,text_bubble):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = text_bubble.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_label]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = text_bubble.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_text_bubble,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_text_bubble(self)
    def update(*args): return apply(adonthellc.text_bubble_update,args)
    def remaining(*args): return apply(adonthellc.text_bubble_remaining,args)
    def __repr__(self):
        return "<C text_bubble instance at %s>" % (self.this,)

class text_bubblePtr(text_bubble):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = text_bubble
adonthellc.text_bubble_swigregister(text_bubblePtr)
class dialog_screen(win_container):
    __setmethods__ = {}
    for _s in [win_container]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,dialog_screen):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = dialog_screen.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_container]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = dialog_screen.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_dialog_screen,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_dialog_screen(self)
    def init(*args): return apply(adonthellc.dialog_screen_init,args)
    def set_portrait(*args): return apply(adonthellc.dialog_screen_set_portrait,args)
    def set_name(*args): return apply(adonthellc.dialog_screen_set_name,args)
    def set_npc(*args): return apply(adonthellc.dialog_screen_set_npc,args)
    def update(*args): return apply(adonthellc.dialog_screen_update,args)
    def run(*args): return apply(adonthellc.dialog_screen_run,args)
    def __repr__(self):
        return "<C dialog_screen instance at %s>" % (self.this,)

class dialog_screenPtr(dialog_screen):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = dialog_screen
adonthellc.dialog_screen_swigregister(dialog_screenPtr)
class data_screen(win_container):
    __setmethods__ = {}
    for _s in [win_container]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,data_screen):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = data_screen.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [win_container]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = data_screen.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_data_screen,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_data_screen(self)
    def update(*args): return apply(adonthellc.data_screen_update,args)
    def get_result(*args): return apply(adonthellc.data_screen_get_result,args)
    def __repr__(self):
        return "<C data_screen instance at %s>" % (self.this,)

class data_screenPtr(data_screen):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = data_screen
adonthellc.data_screen_swigregister(data_screenPtr)
class gamedata:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,gamedata):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = gamedata.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = gamedata.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_gamedata,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_gamedata(self)
    def put(*args): return apply(adonthellc.gamedata_put,args)
    def get(*args): return apply(adonthellc.gamedata_get,args)
    def directory(*args): return apply(adonthellc.gamedata_directory,args)
    def description(*args): return apply(adonthellc.gamedata_description,args)
    def location(*args): return apply(adonthellc.gamedata_location,args)
    def time(*args): return apply(adonthellc.gamedata_time,args)
    def set_description(*args): return apply(adonthellc.gamedata_set_description,args)
    def set_directory(*args): return apply(adonthellc.gamedata_set_directory,args)
    def __repr__(self):
        return "<C gamedata instance at %s>" % (self.this,)

class gamedataPtr(gamedata):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = gamedata
adonthellc.gamedata_swigregister(gamedataPtr)
class game:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,game):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = game.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = game.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_game,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_game(self)
    def __repr__(self):
        return "<C game instance at %s>" % (self.this,)

class gamePtr(game):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = game
adonthellc.game_swigregister(gamePtr)
class nls:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,nls):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = nls.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = nls.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(adonthellc.new_nls,args)
        self.thisown = 1
    def __del__(self,adonthellc=adonthellc):
        if getattr(self,'thisown',0):
            adonthellc.delete_nls(self)
    def __repr__(self):
        return "<C nls instance at %s>" % (self.this,)

class nlsPtr(nls):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = nls
adonthellc.nls_swigregister(nlsPtr)
Python = adonthellc.Python
C = adonthellc.C
READ = adonthellc.READ
WRITE = adonthellc.WRITE
fileops_put_version = adonthellc.fileops_put_version

fileops_get_version = adonthellc.fileops_get_version

EVENTS_DIR = adonthellc.EVENTS_DIR
ENTER_EVENT = adonthellc.ENTER_EVENT
LEAVE_EVENT = adonthellc.LEAVE_EVENT
TIME_EVENT = adonthellc.TIME_EVENT
ACTION_EVENT = adonthellc.ACTION_EVENT
MAX_EVENT = adonthellc.MAX_EVENT
event_list_register_event = adonthellc.event_list_register_event

event_handler_register_event = adonthellc.event_handler_register_event

event_handler_remove_event = adonthellc.event_handler_remove_event

event_handler_raise_event = adonthellc.event_handler_raise_event

CYCLE_LENGTH = adonthellc.CYCLE_LENGTH
FRAME_RATE = adonthellc.FRAME_RATE
FTS_LIMIT = adonthellc.FTS_LIMIT
gametime_start_action = adonthellc.gametime_start_action

gametime_stop_action = adonthellc.gametime_stop_action

gametime_frames_to_skip = adonthellc.gametime_frames_to_skip

gametime_update = adonthellc.gametime_update

SDLK_UNKNOWN = adonthellc.SDLK_UNKNOWN
SDLK_FIRST = adonthellc.SDLK_FIRST
SDLK_BACKSPACE = adonthellc.SDLK_BACKSPACE
SDLK_TAB = adonthellc.SDLK_TAB
SDLK_CLEAR = adonthellc.SDLK_CLEAR
SDLK_RETURN = adonthellc.SDLK_RETURN
SDLK_PAUSE = adonthellc.SDLK_PAUSE
SDLK_ESCAPE = adonthellc.SDLK_ESCAPE
SDLK_SPACE = adonthellc.SDLK_SPACE
SDLK_EXCLAIM = adonthellc.SDLK_EXCLAIM
SDLK_QUOTEDBL = adonthellc.SDLK_QUOTEDBL
SDLK_HASH = adonthellc.SDLK_HASH
SDLK_DOLLAR = adonthellc.SDLK_DOLLAR
SDLK_AMPERSAND = adonthellc.SDLK_AMPERSAND
SDLK_QUOTE = adonthellc.SDLK_QUOTE
SDLK_LEFTPAREN = adonthellc.SDLK_LEFTPAREN
SDLK_RIGHTPAREN = adonthellc.SDLK_RIGHTPAREN
SDLK_ASTERISK = adonthellc.SDLK_ASTERISK
SDLK_PLUS = adonthellc.SDLK_PLUS
SDLK_COMMA = adonthellc.SDLK_COMMA
SDLK_MINUS = adonthellc.SDLK_MINUS
SDLK_PERIOD = adonthellc.SDLK_PERIOD
SDLK_SLASH = adonthellc.SDLK_SLASH
SDLK_0 = adonthellc.SDLK_0
SDLK_1 = adonthellc.SDLK_1
SDLK_2 = adonthellc.SDLK_2
SDLK_3 = adonthellc.SDLK_3
SDLK_4 = adonthellc.SDLK_4
SDLK_5 = adonthellc.SDLK_5
SDLK_6 = adonthellc.SDLK_6
SDLK_7 = adonthellc.SDLK_7
SDLK_8 = adonthellc.SDLK_8
SDLK_9 = adonthellc.SDLK_9
SDLK_COLON = adonthellc.SDLK_COLON
SDLK_SEMICOLON = adonthellc.SDLK_SEMICOLON
SDLK_LESS = adonthellc.SDLK_LESS
SDLK_EQUALS = adonthellc.SDLK_EQUALS
SDLK_GREATER = adonthellc.SDLK_GREATER
SDLK_QUESTION = adonthellc.SDLK_QUESTION
SDLK_AT = adonthellc.SDLK_AT
SDLK_LEFTBRACKET = adonthellc.SDLK_LEFTBRACKET
SDLK_BACKSLASH = adonthellc.SDLK_BACKSLASH
SDLK_RIGHTBRACKET = adonthellc.SDLK_RIGHTBRACKET
SDLK_CARET = adonthellc.SDLK_CARET
SDLK_UNDERSCORE = adonthellc.SDLK_UNDERSCORE
SDLK_BACKQUOTE = adonthellc.SDLK_BACKQUOTE
SDLK_a = adonthellc.SDLK_a
SDLK_b = adonthellc.SDLK_b
SDLK_c = adonthellc.SDLK_c
SDLK_d = adonthellc.SDLK_d
SDLK_e = adonthellc.SDLK_e
SDLK_f = adonthellc.SDLK_f
SDLK_g = adonthellc.SDLK_g
SDLK_h = adonthellc.SDLK_h
SDLK_i = adonthellc.SDLK_i
SDLK_j = adonthellc.SDLK_j
SDLK_k = adonthellc.SDLK_k
SDLK_l = adonthellc.SDLK_l
SDLK_m = adonthellc.SDLK_m
SDLK_n = adonthellc.SDLK_n
SDLK_o = adonthellc.SDLK_o
SDLK_p = adonthellc.SDLK_p
SDLK_q = adonthellc.SDLK_q
SDLK_r = adonthellc.SDLK_r
SDLK_s = adonthellc.SDLK_s
SDLK_t = adonthellc.SDLK_t
SDLK_u = adonthellc.SDLK_u
SDLK_v = adonthellc.SDLK_v
SDLK_w = adonthellc.SDLK_w
SDLK_x = adonthellc.SDLK_x
SDLK_y = adonthellc.SDLK_y
SDLK_z = adonthellc.SDLK_z
SDLK_DELETE = adonthellc.SDLK_DELETE
SDLK_WORLD_0 = adonthellc.SDLK_WORLD_0
SDLK_WORLD_1 = adonthellc.SDLK_WORLD_1
SDLK_WORLD_2 = adonthellc.SDLK_WORLD_2
SDLK_WORLD_3 = adonthellc.SDLK_WORLD_3
SDLK_WORLD_4 = adonthellc.SDLK_WORLD_4
SDLK_WORLD_5 = adonthellc.SDLK_WORLD_5
SDLK_WORLD_6 = adonthellc.SDLK_WORLD_6
SDLK_WORLD_7 = adonthellc.SDLK_WORLD_7
SDLK_WORLD_8 = adonthellc.SDLK_WORLD_8
SDLK_WORLD_9 = adonthellc.SDLK_WORLD_9
SDLK_WORLD_10 = adonthellc.SDLK_WORLD_10
SDLK_WORLD_11 = adonthellc.SDLK_WORLD_11
SDLK_WORLD_12 = adonthellc.SDLK_WORLD_12
SDLK_WORLD_13 = adonthellc.SDLK_WORLD_13
SDLK_WORLD_14 = adonthellc.SDLK_WORLD_14
SDLK_WORLD_15 = adonthellc.SDLK_WORLD_15
SDLK_WORLD_16 = adonthellc.SDLK_WORLD_16
SDLK_WORLD_17 = adonthellc.SDLK_WORLD_17
SDLK_WORLD_18 = adonthellc.SDLK_WORLD_18
SDLK_WORLD_19 = adonthellc.SDLK_WORLD_19
SDLK_WORLD_20 = adonthellc.SDLK_WORLD_20
SDLK_WORLD_21 = adonthellc.SDLK_WORLD_21
SDLK_WORLD_22 = adonthellc.SDLK_WORLD_22
SDLK_WORLD_23 = adonthellc.SDLK_WORLD_23
SDLK_WORLD_24 = adonthellc.SDLK_WORLD_24
SDLK_WORLD_25 = adonthellc.SDLK_WORLD_25
SDLK_WORLD_26 = adonthellc.SDLK_WORLD_26
SDLK_WORLD_27 = adonthellc.SDLK_WORLD_27
SDLK_WORLD_28 = adonthellc.SDLK_WORLD_28
SDLK_WORLD_29 = adonthellc.SDLK_WORLD_29
SDLK_WORLD_30 = adonthellc.SDLK_WORLD_30
SDLK_WORLD_31 = adonthellc.SDLK_WORLD_31
SDLK_WORLD_32 = adonthellc.SDLK_WORLD_32
SDLK_WORLD_33 = adonthellc.SDLK_WORLD_33
SDLK_WORLD_34 = adonthellc.SDLK_WORLD_34
SDLK_WORLD_35 = adonthellc.SDLK_WORLD_35
SDLK_WORLD_36 = adonthellc.SDLK_WORLD_36
SDLK_WORLD_37 = adonthellc.SDLK_WORLD_37
SDLK_WORLD_38 = adonthellc.SDLK_WORLD_38
SDLK_WORLD_39 = adonthellc.SDLK_WORLD_39
SDLK_WORLD_40 = adonthellc.SDLK_WORLD_40
SDLK_WORLD_41 = adonthellc.SDLK_WORLD_41
SDLK_WORLD_42 = adonthellc.SDLK_WORLD_42
SDLK_WORLD_43 = adonthellc.SDLK_WORLD_43
SDLK_WORLD_44 = adonthellc.SDLK_WORLD_44
SDLK_WORLD_45 = adonthellc.SDLK_WORLD_45
SDLK_WORLD_46 = adonthellc.SDLK_WORLD_46
SDLK_WORLD_47 = adonthellc.SDLK_WORLD_47
SDLK_WORLD_48 = adonthellc.SDLK_WORLD_48
SDLK_WORLD_49 = adonthellc.SDLK_WORLD_49
SDLK_WORLD_50 = adonthellc.SDLK_WORLD_50
SDLK_WORLD_51 = adonthellc.SDLK_WORLD_51
SDLK_WORLD_52 = adonthellc.SDLK_WORLD_52
SDLK_WORLD_53 = adonthellc.SDLK_WORLD_53
SDLK_WORLD_54 = adonthellc.SDLK_WORLD_54
SDLK_WORLD_55 = adonthellc.SDLK_WORLD_55
SDLK_WORLD_56 = adonthellc.SDLK_WORLD_56
SDLK_WORLD_57 = adonthellc.SDLK_WORLD_57
SDLK_WORLD_58 = adonthellc.SDLK_WORLD_58
SDLK_WORLD_59 = adonthellc.SDLK_WORLD_59
SDLK_WORLD_60 = adonthellc.SDLK_WORLD_60
SDLK_WORLD_61 = adonthellc.SDLK_WORLD_61
SDLK_WORLD_62 = adonthellc.SDLK_WORLD_62
SDLK_WORLD_63 = adonthellc.SDLK_WORLD_63
SDLK_WORLD_64 = adonthellc.SDLK_WORLD_64
SDLK_WORLD_65 = adonthellc.SDLK_WORLD_65
SDLK_WORLD_66 = adonthellc.SDLK_WORLD_66
SDLK_WORLD_67 = adonthellc.SDLK_WORLD_67
SDLK_WORLD_68 = adonthellc.SDLK_WORLD_68
SDLK_WORLD_69 = adonthellc.SDLK_WORLD_69
SDLK_WORLD_70 = adonthellc.SDLK_WORLD_70
SDLK_WORLD_71 = adonthellc.SDLK_WORLD_71
SDLK_WORLD_72 = adonthellc.SDLK_WORLD_72
SDLK_WORLD_73 = adonthellc.SDLK_WORLD_73
SDLK_WORLD_74 = adonthellc.SDLK_WORLD_74
SDLK_WORLD_75 = adonthellc.SDLK_WORLD_75
SDLK_WORLD_76 = adonthellc.SDLK_WORLD_76
SDLK_WORLD_77 = adonthellc.SDLK_WORLD_77
SDLK_WORLD_78 = adonthellc.SDLK_WORLD_78
SDLK_WORLD_79 = adonthellc.SDLK_WORLD_79
SDLK_WORLD_80 = adonthellc.SDLK_WORLD_80
SDLK_WORLD_81 = adonthellc.SDLK_WORLD_81
SDLK_WORLD_82 = adonthellc.SDLK_WORLD_82
SDLK_WORLD_83 = adonthellc.SDLK_WORLD_83
SDLK_WORLD_84 = adonthellc.SDLK_WORLD_84
SDLK_WORLD_85 = adonthellc.SDLK_WORLD_85
SDLK_WORLD_86 = adonthellc.SDLK_WORLD_86
SDLK_WORLD_87 = adonthellc.SDLK_WORLD_87
SDLK_WORLD_88 = adonthellc.SDLK_WORLD_88
SDLK_WORLD_89 = adonthellc.SDLK_WORLD_89
SDLK_WORLD_90 = adonthellc.SDLK_WORLD_90
SDLK_WORLD_91 = adonthellc.SDLK_WORLD_91
SDLK_WORLD_92 = adonthellc.SDLK_WORLD_92
SDLK_WORLD_93 = adonthellc.SDLK_WORLD_93
SDLK_WORLD_94 = adonthellc.SDLK_WORLD_94
SDLK_WORLD_95 = adonthellc.SDLK_WORLD_95
SDLK_KP0 = adonthellc.SDLK_KP0
SDLK_KP1 = adonthellc.SDLK_KP1
SDLK_KP2 = adonthellc.SDLK_KP2
SDLK_KP3 = adonthellc.SDLK_KP3
SDLK_KP4 = adonthellc.SDLK_KP4
SDLK_KP5 = adonthellc.SDLK_KP5
SDLK_KP6 = adonthellc.SDLK_KP6
SDLK_KP7 = adonthellc.SDLK_KP7
SDLK_KP8 = adonthellc.SDLK_KP8
SDLK_KP9 = adonthellc.SDLK_KP9
SDLK_KP_PERIOD = adonthellc.SDLK_KP_PERIOD
SDLK_KP_DIVIDE = adonthellc.SDLK_KP_DIVIDE
SDLK_KP_MULTIPLY = adonthellc.SDLK_KP_MULTIPLY
SDLK_KP_MINUS = adonthellc.SDLK_KP_MINUS
SDLK_KP_PLUS = adonthellc.SDLK_KP_PLUS
SDLK_KP_ENTER = adonthellc.SDLK_KP_ENTER
SDLK_KP_EQUALS = adonthellc.SDLK_KP_EQUALS
SDLK_UP = adonthellc.SDLK_UP
SDLK_DOWN = adonthellc.SDLK_DOWN
SDLK_RIGHT = adonthellc.SDLK_RIGHT
SDLK_LEFT = adonthellc.SDLK_LEFT
SDLK_INSERT = adonthellc.SDLK_INSERT
SDLK_HOME = adonthellc.SDLK_HOME
SDLK_END = adonthellc.SDLK_END
SDLK_PAGEUP = adonthellc.SDLK_PAGEUP
SDLK_PAGEDOWN = adonthellc.SDLK_PAGEDOWN
SDLK_F1 = adonthellc.SDLK_F1
SDLK_F2 = adonthellc.SDLK_F2
SDLK_F3 = adonthellc.SDLK_F3
SDLK_F4 = adonthellc.SDLK_F4
SDLK_F5 = adonthellc.SDLK_F5
SDLK_F6 = adonthellc.SDLK_F6
SDLK_F7 = adonthellc.SDLK_F7
SDLK_F8 = adonthellc.SDLK_F8
SDLK_F9 = adonthellc.SDLK_F9
SDLK_F10 = adonthellc.SDLK_F10
SDLK_F11 = adonthellc.SDLK_F11
SDLK_F12 = adonthellc.SDLK_F12
SDLK_F13 = adonthellc.SDLK_F13
SDLK_F14 = adonthellc.SDLK_F14
SDLK_F15 = adonthellc.SDLK_F15
SDLK_NUMLOCK = adonthellc.SDLK_NUMLOCK
SDLK_CAPSLOCK = adonthellc.SDLK_CAPSLOCK
SDLK_SCROLLOCK = adonthellc.SDLK_SCROLLOCK
SDLK_RSHIFT = adonthellc.SDLK_RSHIFT
SDLK_LSHIFT = adonthellc.SDLK_LSHIFT
SDLK_RCTRL = adonthellc.SDLK_RCTRL
SDLK_LCTRL = adonthellc.SDLK_LCTRL
SDLK_RALT = adonthellc.SDLK_RALT
SDLK_LALT = adonthellc.SDLK_LALT
SDLK_RMETA = adonthellc.SDLK_RMETA
SDLK_LMETA = adonthellc.SDLK_LMETA
SDLK_LSUPER = adonthellc.SDLK_LSUPER
SDLK_RSUPER = adonthellc.SDLK_RSUPER
SDLK_MODE = adonthellc.SDLK_MODE
SDLK_COMPOSE = adonthellc.SDLK_COMPOSE
SDLK_HELP = adonthellc.SDLK_HELP
SDLK_PRINT = adonthellc.SDLK_PRINT
SDLK_SYSREQ = adonthellc.SDLK_SYSREQ
SDLK_BREAK = adonthellc.SDLK_BREAK
SDLK_MENU = adonthellc.SDLK_MENU
SDLK_POWER = adonthellc.SDLK_POWER
SDLK_EURO = adonthellc.SDLK_EURO
SDLK_LAST = adonthellc.SDLK_LAST
KMOD_NONE = adonthellc.KMOD_NONE
KMOD_LSHIFT = adonthellc.KMOD_LSHIFT
KMOD_RSHIFT = adonthellc.KMOD_RSHIFT
KMOD_LCTRL = adonthellc.KMOD_LCTRL
KMOD_RCTRL = adonthellc.KMOD_RCTRL
KMOD_LALT = adonthellc.KMOD_LALT
KMOD_RALT = adonthellc.KMOD_RALT
KMOD_LMETA = adonthellc.KMOD_LMETA
KMOD_RMETA = adonthellc.KMOD_RMETA
KMOD_NUM = adonthellc.KMOD_NUM
KMOD_CAPS = adonthellc.KMOD_CAPS
KMOD_MODE = adonthellc.KMOD_MODE
KMOD_RESERVED = adonthellc.KMOD_RESERVED
input_init = adonthellc.input_init

input_shutdown = adonthellc.input_shutdown

input_update = adonthellc.input_update

input_is_pushed = adonthellc.input_is_pushed

input_has_been_pushed = adonthellc.input_has_been_pushed

input_get_next_key = adonthellc.input_get_next_key

input_get_next_unicode = adonthellc.input_get_next_unicode

input_set_key_repeat = adonthellc.input_set_key_repeat

input_clear_keys_queue = adonthellc.input_clear_keys_queue

NUM_WAVES = adonthellc.NUM_WAVES
NUM_MUSIC = adonthellc.NUM_MUSIC
NUM_CHANNELS = adonthellc.NUM_CHANNELS
audio_init = adonthellc.audio_init

audio_cleanup = adonthellc.audio_cleanup

audio_put_state = adonthellc.audio_put_state

audio_get_state = adonthellc.audio_get_state

audio_load_background = adonthellc.audio_load_background

audio_unload_background = adonthellc.audio_unload_background

audio_set_background_volume = adonthellc.audio_set_background_volume

audio_pause_music = adonthellc.audio_pause_music

audio_unpause_music = adonthellc.audio_unpause_music

audio_load_wave = adonthellc.audio_load_wave

audio_unload_wave = adonthellc.audio_unload_wave

audio_play_wave = adonthellc.audio_play_wave

audio_play_background = adonthellc.audio_play_background

audio_fade_in_background = adonthellc.audio_fade_in_background

audio_fade_out_background = adonthellc.audio_fade_out_background

audio_change_background = adonthellc.audio_change_background

audio_is_initialized = adonthellc.audio_is_initialized

audio_is_schedule_activated = adonthellc.audio_is_schedule_activated

audio_is_background_finished = adonthellc.audio_is_background_finished

audio_set_schedule_active = adonthellc.audio_set_schedule_active

audio_set_schedule = adonthellc.audio_set_schedule

audio_run_schedule = adonthellc.audio_run_schedule

DIALOG_DIR = adonthellc.DIALOG_DIR
DWARF = adonthellc.DWARF
ELF = adonthellc.ELF
HALFELF = adonthellc.HALFELF
HUMAN = adonthellc.HUMAN
FEMALE = adonthellc.FEMALE
MALE = adonthellc.MALE
NPC = adonthellc.NPC
PLAYER = adonthellc.PLAYER
PARTY = adonthellc.PARTY
cvar = adonthellc.cvar
screen_set_video_mode = adonthellc.screen_set_video_mode

screen_length = adonthellc.screen_length

screen_height = adonthellc.screen_height

screen_bytes_per_pixel = adonthellc.screen_bytes_per_pixel

screen_trans_col = adonthellc.screen_trans_col

screen_clear = adonthellc.screen_clear

screen_show = adonthellc.screen_show

screen_is_fullscreen = adonthellc.screen_is_fullscreen

screen_set_fullscreen = adonthellc.screen_set_fullscreen

screen_info = adonthellc.screen_info

screen_transition = adonthellc.screen_transition

PLAY = adonthellc.PLAY
STOP = adonthellc.STOP
ALL_WALKABLE = adonthellc.ALL_WALKABLE
WALKABLE_SOUTH = adonthellc.WALKABLE_SOUTH
WALKABLE_NORTH = adonthellc.WALKABLE_NORTH
WALKABLE_EAST = adonthellc.WALKABLE_EAST
WALKABLE_WEST = adonthellc.WALKABLE_WEST
NONE_WALKABLE = adonthellc.NONE_WALKABLE
MAPOBJECTS_DIR = adonthellc.MAPOBJECTS_DIR
MAPCHAR_DIR = adonthellc.MAPCHAR_DIR
STAND_NORTH = adonthellc.STAND_NORTH
STAND_SOUTH = adonthellc.STAND_SOUTH
STAND_WEST = adonthellc.STAND_WEST
STAND_EAST = adonthellc.STAND_EAST
WALK_NORTH = adonthellc.WALK_NORTH
WALK_SOUTH = adonthellc.WALK_SOUTH
WALK_WEST = adonthellc.WALK_WEST
WALK_EAST = adonthellc.WALK_EAST
NBR_MOVES = adonthellc.NBR_MOVES
NO_MOVE = adonthellc.NO_MOVE
MAPS_DIR = adonthellc.MAPS_DIR
WIN_NB_TABLE_CHAR = adonthellc.WIN_NB_TABLE_CHAR
WIN_TEXT_MAX_LENGTH = adonthellc.WIN_TEXT_MAX_LENGTH
WIN_FONT_HEIGHT = adonthellc.WIN_FONT_HEIGHT
WIN_FONT_LENGHT = adonthellc.WIN_FONT_LENGHT
WIN_SPACE_LENGHT = adonthellc.WIN_SPACE_LENGHT
WIN_DIRECTORY = adonthellc.WIN_DIRECTORY
WIN_FONT_DIRECTORY = adonthellc.WIN_FONT_DIRECTORY
WIN_BORDER_DIRECTORY = adonthellc.WIN_BORDER_DIRECTORY
WIN_BACKGROUND_DIRECTORY = adonthellc.WIN_BACKGROUND_DIRECTORY
WIN_SCROLLBAR_DIRECTORY = adonthellc.WIN_SCROLLBAR_DIRECTORY
WIN_CURSOR_DIRECTORY = adonthellc.WIN_CURSOR_DIRECTORY
WIN_FONT_FILE_IDX = adonthellc.WIN_FONT_FILE_IDX
WIN_FONT_FILE_PIC = adonthellc.WIN_FONT_FILE_PIC
WIN_FONT_FILE = adonthellc.WIN_FONT_FILE
WIN_V_BORDER_TEMPLATE_FILE = adonthellc.WIN_V_BORDER_TEMPLATE_FILE
WIN_H_BORDER_TEMPLATE_FILE = adonthellc.WIN_H_BORDER_TEMPLATE_FILE
WIN_CORNER_TOP_LEFT_FILE = adonthellc.WIN_CORNER_TOP_LEFT_FILE
WIN_CORNER_TOP_RIGHT_FILE = adonthellc.WIN_CORNER_TOP_RIGHT_FILE
WIN_CORNER_BOTTOM_LEFT_FILE = adonthellc.WIN_CORNER_BOTTOM_LEFT_FILE
WIN_CORNER_BOTTOM_RIGHT_FILE = adonthellc.WIN_CORNER_BOTTOM_RIGHT_FILE
WIN_BACKGROUND_FILE = adonthellc.WIN_BACKGROUND_FILE
WIN_SCROLLBAR_BAR_TOP = adonthellc.WIN_SCROLLBAR_BAR_TOP
WIN_SCROLLBAR_BAR_MID = adonthellc.WIN_SCROLLBAR_BAR_MID
WIN_SCROLLBAR_BAR_BOT = adonthellc.WIN_SCROLLBAR_BAR_BOT
WIN_SCROLLBAR_BAR_FLEX = adonthellc.WIN_SCROLLBAR_BAR_FLEX
WIN_SCROLLBAR_BACK_TOP = adonthellc.WIN_SCROLLBAR_BACK_TOP
WIN_SCROLLBAR_BACK_MID = adonthellc.WIN_SCROLLBAR_BACK_MID
WIN_SCROLLBAR_BACK_BOT = adonthellc.WIN_SCROLLBAR_BACK_BOT
WIN_CURSOR_FILE = adonthellc.WIN_CURSOR_FILE
WIN_BORDER_NORMAL_SIZE = adonthellc.WIN_BORDER_NORMAL_SIZE
WIN_BORDER_MINI_SIZE = adonthellc.WIN_BORDER_MINI_SIZE
WIN_BRIGHTNESS_LEVEL = adonthellc.WIN_BRIGHTNESS_LEVEL
win_manager_get_active = adonthellc.win_manager_get_active

win_manager_init = adonthellc.win_manager_init

win_manager_cleanup = adonthellc.win_manager_cleanup

win_manager_add_theme = adonthellc.win_manager_add_theme

win_manager_remove_theme = adonthellc.win_manager_remove_theme

win_manager_get_theme = adonthellc.win_manager_get_theme

win_manager_add_font = adonthellc.win_manager_add_font

win_manager_remove_font = adonthellc.win_manager_remove_font

win_manager_get_font = adonthellc.win_manager_get_font

win_event_ACTIVATE = adonthellc.win_event_ACTIVATE
win_event_UNACTIVATE = adonthellc.win_event_UNACTIVATE
win_event_UPDATE = adonthellc.win_event_UPDATE
win_event_DRAW = adonthellc.win_event_DRAW
win_event_DRAW_ON_VISIBLE = adonthellc.win_event_DRAW_ON_VISIBLE
win_event_ACTIVATE_KEY = adonthellc.win_event_ACTIVATE_KEY
win_event_SELECT = adonthellc.win_event_SELECT
win_event_UNSELECT = adonthellc.win_event_UNSELECT
win_event_KEYBOARD = adonthellc.win_event_KEYBOARD
win_event_SCROLL_UP = adonthellc.win_event_SCROLL_UP
win_event_SCROLL_DOWN = adonthellc.win_event_SCROLL_DOWN
win_event_NEXT = adonthellc.win_event_NEXT
win_event_PREVIOUS = adonthellc.win_event_PREVIOUS
win_event_CLOSE = adonthellc.win_event_CLOSE
win_event_DESTROY = adonthellc.win_event_DESTROY
win_border_MINI = adonthellc.win_border_MINI
win_border_NORMAL = adonthellc.win_border_NORMAL
win_base_ALIGN_NONE = adonthellc.win_base_ALIGN_NONE
win_base_ALIGN_LEFT = adonthellc.win_base_ALIGN_LEFT
win_base_ALIGN_CENTER = adonthellc.win_base_ALIGN_CENTER
win_base_ALIGN_RIGHT = adonthellc.win_base_ALIGN_RIGHT
win_container_SPACE_WITH_BORDER = adonthellc.win_container_SPACE_WITH_BORDER
win_container_SPACE_WITH_OBJECT = adonthellc.win_container_SPACE_WITH_OBJECT
win_container_LIST_LAYOUT = adonthellc.win_container_LIST_LAYOUT
win_container_NO_LAYOUT = adonthellc.win_container_NO_LAYOUT
label_NOTHING = adonthellc.label_NOTHING
label_AUTO_HEIGHT = adonthellc.label_AUTO_HEIGHT
label_AUTO_SIZE = adonthellc.label_AUTO_SIZE
win_scroll_PAD_DEFAULT = adonthellc.win_scroll_PAD_DEFAULT
win_select_MODE_BORDER = adonthellc.win_select_MODE_BORDER
win_select_MODE_BRIGHTNESS = adonthellc.win_select_MODE_BRIGHTNESS
MAX_COLOR = adonthellc.MAX_COLOR
LOAD_SCREEN = adonthellc.LOAD_SCREEN
SAVE_SCREEN = adonthellc.SAVE_SCREEN
gamedata_init = adonthellc.gamedata_init

gamedata_cleanup = adonthellc.gamedata_cleanup

gamedata_load_characters = adonthellc.gamedata_load_characters

gamedata_load_quests = adonthellc.gamedata_load_quests

gamedata_load_mapengine = adonthellc.gamedata_load_mapengine

gamedata_load_audio = adonthellc.gamedata_load_audio

gamedata_load = adonthellc.gamedata_load

gamedata_save = adonthellc.gamedata_save

gamedata_unload = adonthellc.gamedata_unload

gamedata_next_save = adonthellc.gamedata_next_save

gamedata_user_data_dir = adonthellc.gamedata_user_data_dir

gamedata_game_data_dir = adonthellc.gamedata_game_data_dir

gamedata_get_saved_game = adonthellc.gamedata_get_saved_game

gamedata_quests = adonthellc.gamedata_quests

gamedata_player = adonthellc.gamedata_player

gamedata_get_character = adonthellc.gamedata_get_character

gamedata_get_quest = adonthellc.gamedata_get_quest

gamedata_characters = adonthellc.gamedata_characters

gamedata_engine = adonthellc.gamedata_engine

game_init = adonthellc.game_init

game_user_data_dir = adonthellc.game_user_data_dir

game_global_data_dir = adonthellc.game_global_data_dir

game_game_data_dir = adonthellc.game_game_data_dir

game_find_file = adonthellc.game_find_file

game_find_directory = adonthellc.game_find_directory

nls_init = adonthellc.nls_init

nls_set_language = adonthellc.nls_set_language

nls_translate = adonthellc.nls_translate


