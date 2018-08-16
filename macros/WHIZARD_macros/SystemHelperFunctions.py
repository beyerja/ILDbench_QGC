#!/usr/bin/python

""" Helper functions for I/O and directory structure.
"""

import os

#-------------------------------------------------------------------------------

def readFileContent( filepath ):
    """ Read content of file into string and return
    """
    with open(filepath, "r") as file:
        return file.read()

#-------------------------------------------------------------------------------

def createSubdirectory( dir_path ):
    """ Creates last directory in the given chain, all previous directories must
        exist.
    """
    newpath = r"{}".format( dir_path )
    if not os.path.exists(newpath):
        os.makedirs(newpath)
        
#-------------------------------------------------------------------------------