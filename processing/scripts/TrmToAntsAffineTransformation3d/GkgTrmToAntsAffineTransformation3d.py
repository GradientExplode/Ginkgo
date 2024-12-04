import os
import sys
import glob
import optparse
import math
from array import array


scriptDirectory = os.path.dirname( os.path.abspath( __file__) )
gkgSharePath = os.path.dirname( scriptDirectory )
sys.path.insert( 0, os.path.join( gkgSharePath, 'python' ) )

import gkg


################################################################################
################################################################################
# various functions
################################################################################
################################################################################

################################################################################
# removing .minf
################################################################################

def removeMinf( fileNameIma ):

  fileNameMinf = fileNameIma + '.minf'
  if ( os.path.exists( fileNameMinf ) ):
  
    os.remove( fileNameMinf )


################################################################################
# removing an entire directory
################################################################################

def rm( *args ):

  sources = []
  for pattern in args:
  
    sources += glob.glob( pattern )
    
  sys.stdout.flush()
  for path in sources:
  
    if not os.path.islink( path ):

      # this test avoids an error on dead symlinks
      os.chmod( path, 0o777 )

    if os.path.isdir( path ) and not os.path.islink( path ):

      rm( os.path.join( path, '*' ) )
      os.rmdir( path )

    else:
      os.remove( path )



################################################################################
################################################################################
# main function
#
#  GkgTrmToAntsAffineTransformation3d
#    -transformation <*.trm>
#    -reference <*.ima>
#    -floating <*.ima>
#    -t  <rigid/affine/affine_and_diffeomorphic>
#    -o <float_to_reference_transformation.trm,.ima>
#    -oi <reference_to_float_transformation.trm,.ima>
#    -intermediate <True/False>
#    -verbose <True/False>
#
################################################################################
################################################################################


parser = optparse.OptionParser()
parser.add_option( '-t', '--trmTransform3d',
                   dest = 'fileNameTrmTransform3d',
                   help = '*.trm 3D transform to be converted to '
                          'Ants *.mat format',
                   metavar = 'FILE' )
                   
parser.add_option( '-r', '--reference',
                   dest = 'fileNameReferenceVolume',
                   help = 'reference volume filename to compute '
                          'reference center',
                   metavar = 'FILE' )
                   
parser.add_option( '-a', '--antsMatAffineTransform3d',
                   dest = 'fileNameMatTransform3d',
                   help = 'Output *.mat Ants 3D transform filename',
                   metavar = 'FILE' )
                   
parser.add_option( '-I', '--intermediate',
                   dest = 'intermediate',
                   action = 'store_true',
                   default = False,
                   help = 'Store intermediate result(s) (default: %default)' )

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


if ( options.fileNameTrmTransform3d is None ):

  raise Exception( 'a *.trm 3D transform is required' )

else:

  if ( not os.path.exists( options.fileNameTrmTransform3d ) ):

    raise Exception( 'cannot read *.trm 3D transform' )



if ( options.fileNameReferenceVolume is None ):

  raise Exception( 'a reference volume is required' )

else:

  if ( not os.path.exists( options.fileNameReferenceVolume ) ):

    raise Exception( 'cannot read reference volume' )



################################################################################
# creating an intermediate directory
################################################################################

intermediateDirectory = os.path.join(
                          os.path.dirname( 
                            os.path.abspath( options.fileNameMatTransform3d ) ),
                          'intermediate' )

if ( options.verbose ):

  print( '*.trm 3D transform :', options.fileNameTrmTransform3d )
  print( 'reference volume :', options.fileNameReferenceVolume )
  print( '*.mat 3D transform :', options.fileNameMatTransform3d )
  print( 'keep intermediate results : ', options.intermediate )
  print( 'intermediate directory : ', intermediateDirectory )


 
if ( not os.path.exists( intermediateDirectory ) ):

  os.mkdir( intermediateDirectory )
  if ( not os.path.exists( intermediateDirectory ) ):

    raise Exception( 'cannot create directory ' + intermediateDirectory )


################################################################################
# inverting the *.trm 3D transform since the Ants *.mat file corresponds to the
# inverse transformation
################################################################################

fileNameTrmInverseTransform3d = os.path.join( intermediateDirectory,
                                              'inverse.trm' )
gkg.executeCommand( { 'algorithm' : 'Transform3dInverter',
                        'parameters' : \
                          { 'fileNameIn' : options.fileNameTrmTransform3d,
                            'fileNameOut' : fileNameTrmInverseTransform3d,
                            'verbose' : options.verbose
                          },
                        'verbose' : options.verbose
                      } )


################################################################################
# reading the *.trm 3D transform since the Ants *.mat file corresponds reference and floating to intermediate directory
################################################################################

f = open( fileNameTrmInverseTransform3d, 'r' )
lines = f.readlines()
f.close()

line1 = lines[ 0 ].split()
line2 = lines[ 1 ].split()
line3 = lines[ 2 ].split()
line4 = lines[ 3 ].split()

Tprime = [ float( line1[ 0 ] ),
           float( line1[ 1 ] ),
           float( line1[ 2 ] ) ]
 
Rprime = [ [ float( line2[ 0 ] ), float( line2[ 1 ] ), float( line2[ 2 ] ) ],
           [ float( line3[ 0 ] ), float( line3[ 1 ] ), float( line3[ 2 ] ) ],
           [ float( line4[ 0 ] ), float( line4[ 1 ] ), float( line4[ 2 ] ) ] ]


################################################################################
# computing the reference center
################################################################################

# extracting dimensions and resolution from the reference volume
f = open( options.fileNameReferenceVolume[ : -3 ] + 'dim', 'r' )
lines = f.readlines()
line1 = lines[ 0 ].split()
line3 = lines[ 2 ].split()

sizeX = int( line1[ 0 ] )
sizeY = int( line1[ 1 ] )
sizeZ = int( line1[ 2 ] )

resolutionX = float( line3[ 1 ] )
resolutionY = float( line3[ 3 ] )
resolutionZ = float( line3[ 5 ] )

C = [ sizeX * resolutionX / 2.0,
      sizeY * resolutionY / 2.0,
      sizeZ * resolutionZ / 2.0 ]


################################################################################
# computing the Ants rotation and translation
################################################################################

R = Rprime
R[ 2 ][ 0 ] = -R[ 2 ][ 0 ]
R[ 2 ][ 1 ] = -R[ 2 ][ 1 ]
R[ 0 ][ 2 ] = -R[ 0 ][ 2 ]
R[ 1 ][ 2 ] = -R[ 1 ][ 2 ]

T = [ 0.0, 0.0, 0.0 ]
T[ 0 ] = -C[ 0 ] + ( R[ 0 ][ 0 ] * C[ 0 ] +
                     R[ 0 ][ 1 ] * C[ 1 ] +
                     R[ 0 ][ 2 ] * C[ 2 ] ) + Tprime[ 0 ]
T[ 1 ] = -C[ 1 ] + ( R[ 1 ][ 0 ] * C[ 0 ] +
                     R[ 1 ][ 1 ] * C[ 1 ] +
                     R[ 1 ][ 2 ] * C[ 2 ] ) + Tprime[ 1 ]
T[ 2 ] = -C[ 2 ] + ( R[ 2 ][ 0 ] * C[ 0 ] +
                     R[ 2 ][ 1 ] * C[ 1 ] +
                     R[ 2 ][ 2 ] * C[ 2 ] ) - Tprime[ 2 ]

if ( options.verbose ):

  print( '====================== R ======================' )
  print( R )
  print( '====================== T ======================' )
  print( T )
  print( '====================== Fixed ======================' )
  print( C )


################################################################################
# saving Ants transform to .mat file
################################################################################

file = open( options.fileNameMatTransform3d, 'wb' )

integer_array = array( 'i', [ 0, 12, 1, 0, 27 ] )
integer_array.tofile( file )

file.write( 'AffineTransform_double_3_3\0' )

double_array = array( 'd', [ R[ 0 ][ 0 ], R[ 0 ][ 1 ], R[ 0 ][ 2 ],
                             R[ 1 ][ 0 ], R[ 1 ][ 1 ], R[ 1 ][ 2 ],
                             R[ 2 ][ 0 ], R[ 2 ][ 1 ], R[ 2 ][ 2 ],
                             T[ 0 ], T[ 1 ], T[ 2 ] ] )
double_array.tofile( file )

integer_array = array( 'i', [ 0, 3, 1, 0, 6 ] )
integer_array.tofile( file )

file.write( 'fixed\0' )

double_array = array( 'd', [ C[ 0 ], C[ 1 ], C[ 2 ] ] )
double_array.tofile( file )

file.close()


################################################################################
# last removing intermediate directory if not required
################################################################################

if options.intermediate == False:

  rm( intermediateDirectory )

