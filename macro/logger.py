# encoding: utf-8
"""
loki.core.logger
~~~~~~~~~~~~~~~~

Module providing a global logger

"""
__author__    = "Will Davey"
__email__     = "will.davey@cern.ch"
__created__   = "2016-02-21"
__copyright__ = "Copyright 2016 Will Davey"
__license__   = "GPL http://www.gnu.org/licenses/gpl.html"

## modules
import logging
import sys, os
#import time

#: is global logger initialized (global variable)
gInitialized = False

DEFAULT  = u'\x1b[39;49m'
BLUE     = u'\x1b[34m'
BLUEBOLD = u'\x1b[1;34m'
RED      = u'\x1b[31m'
REDBOLD  = u'\x1b[1;31m'
REDBKG   = u'\x1b[1;41;37m'
YELLOW   = u'\x1b[33m'
UNSET    = u'\x1b[0m'

# - - - - - - - - - - function defs - - - - - - - - - - - - #
#____________________________________________________________
def initialize(level=None):
    """initialize global logger
    
    :param level: logging output level
    :type level: logging.LEVEL (eg. DEBUG, INFO, WARNING...)
    """
    logging.basicConfig(
        filemode='w',
        level=level if level!=None else logging.INFO,
        #format='[%(asctime)s %(name)-16s %(levelname)-7s]  %(message)s',
        format='[%(asctime)s %(levelname)-7s]  %(message)s',
        #datefmt='%Y-%m-%d %H:%M:%S',
        datefmt='%H:%M:%S',
        )
    logging.getLogger("global")
    if supports_color():
        logging.StreamHandler.emit = add_coloring_to_emit_ansi(logging.StreamHandler.emit)

#____________________________________________________________
def log():
    """Return global logger"""
    global gInitialized
    if not gInitialized: 
        initialize()
        gInitialized = True
    return logging.getLogger("global")    


#____________________________________________________________
def setLevel(level):
    """Set global logging level"""
    log().setLevel(level)


#______________________________________________________________________________=buf=
def supports_color():
    """
    Returns True if the running system's terminal supports color, and False
    otherwise.
    """
    plat = sys.platform
    supported_platform = plat != 'Pocket PC' and (plat != 'win32' or
                                                  'ANSICON' in os.environ)
    # isatty is not always implemented
    is_a_tty = hasattr(sys.stdout, 'isatty') and sys.stdout.isatty()
    if not supported_platform or not is_a_tty:
        return False
    return True

#______________________________________________________________________________=buf=
def add_coloring_to_emit_ansi(fn):
    # add methods we need to the class
    def new(*args):
        levelno = args[1].levelno
        if(  levelno>=logging.CRITICAL): color = REDBKG
        elif(levelno>=logging.ERROR)   : color = REDBOLD 
        elif(levelno>=logging.WARNING) : color = RED 
        elif(levelno>=logging.INFO)    : color = DEFAULT 
        elif(levelno>=logging.DEBUG)   : color = YELLOW 
        else:                            color = YELLOW 
        args[1].msg = color + args[1].msg +  UNSET
        return fn(*args)
    return new




## EOF
