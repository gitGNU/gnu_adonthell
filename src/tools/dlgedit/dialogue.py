#
#  $Id$
#
#  (C) Copyright 2002 Kai Sterker <kaisterker@linuxgames.com>
#  Part of the Adonthell Project http://adonthell.linuxgames.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License.
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY.
#
#  See the COPYING file for more details
#

import sys

# -- Base class for all dialogue scripts
class base:

    # -- Execute one iteration of the dialogue
    #    This consists of the following steps:
    #
    #    1) Run the code of the selected line of text
    #    2) Append all followes whose condition is true to the speech
    #       to be used for the next selection. Also append the speaker
    #       associated with each line of text.
    #    3) return whether the next choice is to be made by the Dialogue
    #       Engine (1) or the Player (0)
    #
    #    All data for these actions is retrieved from the derived class.
    def run (self, index):
        self.speech = []
        self.speaker = []
        
        # -- get the code to execute, the hint fot the dialogue engine
        #    and the list of following dialogue options
        code, stop, follow = self.dlg[index][1:]
        
        # -- if there is code, try to execute it
        if code != None: 
            self.execute (self.code[code])
    
        # -- for each following option:
        for speech, operation, cond in follow:
            
            # -- if we're in an elif or else part of a condition
            #    and a former condition was true already, continue
            if operation == 1 and result == 1: continue
            
            # -- try to ececute the condition, if there is one
            if cond != None:
                result = self.execute (self.cond[cond])
            else
                result = 1
            
            # -- only append the option if the condition was met
            if result == 1:
                self.speech.append (speech)
                self.speaker.append (self.dlg[speech][0])

        # -- finally, tell the dialogue engine what to do            
        return stop
    
    
    # -- Execute some arbitrary Python code
    def execute (self, statement):
        try:
            # -- try to execute the code
            retval = eval (statement)
            return retval
        except:
            # -- in case of an error, print what happened and continue
            err_type, value = sys.exc_info ()[:2]
            print "Error:\n  " + str (err_type) + ":\n  \"" + str (value) + "\""

        return 0
