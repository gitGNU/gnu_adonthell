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
    #    2) Append all followers whose condition is true to the speech
    #       to be used for the next selection. Also append the speaker
    #       associated with each line of text.
    #
    #    All data for these actions is retrieved from the derived class.
    def run (self, index):
        self.speech = []
        self.speaker = []
        result = 0
                
        # -- get the code to execute and the list of following
        #    dialogue options
        code, followers = self.dlg[index][1:]
        
        # -- if there is code, try to execute it
        if code != -1: 
            self.execute (self.code[code], 1)
    
        # -- for each following option:
        for (speech, operation, cond) in followers:
            
            # -- if we're in an elif or else part of a condition
            #    and a former condition was true already, continue
            if operation == 1 and result == 1: continue
            
            # -- try to ececute the condition, if there is one
            if cond != -1:
                result = self.execute (self.cond[cond], 0)
            else:
                result = 1
            
            # -- only append the option if the condition was met
            if result == 1:
                speaker = self.dlg[speech][0]
                if speaker == "Default": speaker = self.the_npc.get_name ()
                self.speaker.append (speaker)
                self.speech.append (speech)

        # -- finally, return           
        return
    
    
    # -- Execute some arbitrary Python code
    def execute (self, statement, multiline):
        try:
            # -- this is for multiline code without retval
            if multiline == 1:
                code = compile (statement, "", "exec")
                return eval (code, self.namespace, locals ())
            # -- this is for single line statements with retval
            else:
                return eval (statement, self.namespace, locals ())
        except:
            # -- in case of an error, print what happened and continue
            err_type, value = sys.exc_info ()[:2]
            print "Error in statement\n  %s  %s:\n    '%s'" \
                % (statement, str (err_type), str (value))

        return 0


    # -- Decide whether the dialogue engine should return control to the
    #    player (1) or not (0) 
    def stop (self, index):
        followers = self.dlg[index][2]

        if len (followers) > 0 and self.dlg[followers[0][0]][0] == None: return 0
        else: return 1

        
    # -- Allow usage of undeclared variables
    def __getattr__ (self, name):
        return 0
    
