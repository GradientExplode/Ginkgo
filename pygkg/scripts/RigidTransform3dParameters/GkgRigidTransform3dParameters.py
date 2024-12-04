import os, sys

scriptDirectory = os.path.dirname( os.path.abspath( __file__) )
gkgSharePath = os.path.dirname( scriptDirectory )
pyGkgPath = os.path.join( gkgSharePath, 'python', 'pygkg' )
sys.path.insert( 0, pyGkgPath )

from Rotation3dFunctions import *


################################################################################
# main
################################################################################

# in case of help
if ( ( len( sys.argv ) == 2 ) and ( sys.argv[ 1 ] == '-h' ) ):

  print "Usage: GkgRigidTransform3dParameters -t <*.trm>"

# in case of batch mode
elif ( ( len( sys.argv ) == 3 ) and ( sys.argv[ 1 ] == '-t' ) ):

  parameters = getRigidParametersFromAnatomistTransform( sys.argv[ 2 ] )
    
  print 'Rx = ', parameters[ 0 ], "deg", \
        'Ry = ', parameters[ 1 ], "deg", \
        'Rz = ', parameters[ 2 ], "deg", \
        'Tx = ', parameters[ 3 ], "mm", \
        'Ty = ', parameters[ 4 ], "mm", \
        'Tz = ', parameters[ 5 ], "mm"

# in case of graphical user interface mode
else:

  print "Usage: GkgRigidTransform3dParameters -t <*.trm>"
