#!/usr/bin/python

""" Returns the working directory for the WHIZARD calculations.
"""    

import sys

#-------------------------------------------------------------------------------

def getOutputDirectory():
    mordor = '/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq'
    return mordor

#-------------------------------------------------------------------------------

def main(arguments):
    output_dir = getOutputDirectory()
    return output_dir
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

#-------------------------------------------------------------------------------