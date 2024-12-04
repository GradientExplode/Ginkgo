import os
import sys
import optparse

scriptDirectory = os.path.dirname( os.path.abspath( __file__) )
gkgSharePath = os.path.dirname( scriptDirectory )
sys.path.insert( 0, os.path.join( gkgSharePath, 'python' ) )

from core.command.CommandFactory import *



################################################################################
################################################################################
# main function
################################################################################
################################################################################


parser = optparse.OptionParser()
parser.add_option( '-i', '--input',
                   dest = 'fileNameInputVolume',
                   help = 'input volume filename',
                   metavar = 'FILE' )
parser.add_option( '-o', '--output',
                   dest = 'fileNameOutputVolume',
                   help = 'output volume filename',
                   metavar = 'FILE' )
parser.add_option( '-m', '--mask',
                   dest = 'fileNameMaskVolume',
                   help = 'mask volume filename',
                   metavar = 'FILE' )
parser.add_option( '-v', '--verbose',
                   dest = 'verbose',
                   action = 'store_true',
                   default = False,
                   help = 'Show as much information as possible '
                          '(default: %default)' )


( options, args ) = parser.parse_args()


################################################################################
# checking that input files exist
################################################################################

if ( options.fileNameInputVolume is None ):

  raise Exception( 'a input volume is required' )

else:

  if ( not os.path.exists( options.fileNameInputVolume ) ):

    raise Exception( 'cannot read input volume' )


################################################################################
# launching the ANTs bias field correction command
################################################################################

CommandFactory().executeCommand( 
  { "algorithm" : "AntsBiasFieldCorrection",
    "parameters" : \
       { "fileNameInputVolume" : options.fileNameInputVolume,
         "fileNameOutputVolume" : options.fileNameOutputVolume,
         "fileNameMaskVolume" : options.fileNameMaskVolume,
         "verbose" : options.verbose
       },
    "verbose" : options.verbose
  } )
