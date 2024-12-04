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

  print "Usage: GkgAffineTransform3dParameters -t <*.trm>"

# in case of batch mode
elif ( ( len( sys.argv ) == 3 ) and ( sys.argv[ 1 ] == '-t' ) ):

  parameters = getAffineParametersFromAnatomistTransform( sys.argv[ 2 ] )
    
  print 'Sx = ', parameters[ 0 ],  \
        'Sy = ', parameters[ 1 ],  \
        'Sz = ', parameters[ 2 ],  \
        'Shxy = ', parameters[ 3 ],  \
        'Shxz = ', parameters[ 4 ],  \
        'Shyz = ', parameters[ 5 ], \
        'Rx = ', parameters[ 6 ], "deg", \
        'Ry = ', parameters[ 7 ], "deg", \
        'Rz = ', parameters[ 8 ], "deg", \
        'Tx = ', parameters[ 9 ], "mm", \
        'Ty = ', parameters[ 10 ], "mm", \
        'Tz = ', parameters[ 11 ], "mm"

# in case of graphical user interface mode
else:

  print "Usage: GkgAffineTransform3dParameters -t <*.trm>"
