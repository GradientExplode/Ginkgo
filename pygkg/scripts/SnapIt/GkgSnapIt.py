import sys, os, math
import PyQt4
from PyQt4 import QtCore, QtGui
import anatomist.api as anatomist


def getRotationX( angle ):

  return [ [ 1.0,                0.0,                 0.0 ],
           [ 0.0, +math.cos( angle ), -math.sin( angle )  ],
           [ 0.0, +math.sin( angle ), +math.cos( angle )  ] ]
           

def getRotationY( angle ):

  return [ [ +math.cos( angle ), 0.0, +math.sin( angle ) ],
           [                0.0, 1.0,                0.0 ],
           [ -math.sin( angle ), 0.0, +math.cos( angle ) ] ]


def getRotationZ( angle ):

  return [ [ +math.cos( angle ), -math.sin( angle ), 0.0 ],
           [ +math.sin( angle ), +math.cos( angle ), 0.0 ],
           [                0.0,                0.0, 1.0 ] ]


def composeRotations( r1, r2 ):

  r = [ [ 0.0, 0.0, 0.0 ],
        [ 0.0, 0.0, 0.0 ],
        [ 0.0, 0.0, 0.0 ] ]
        
  for i in range( 0, 3 ):

    for j in range( 0, 3 ):

      r[ i ][ j ] = 0.0    
      for k in range( 0, 3 ):
      
        r[ i ][ j ] += r1[ i ][ k ] * r2[ k ][ j ]

  return r
   
   
def toQuaternion( r ):

  h = [ r[ 0 ][ 0 ], r[ 0 ][ 1 ], r[ 0 ][ 2 ], 0.0,
        r[ 1 ][ 0 ], r[ 1 ][ 1 ], r[ 1 ][ 2 ], 0.0,
        r[ 2 ][ 0 ], r[ 2 ][ 1 ], r[ 2 ][ 2 ], 0.0,
        0.0, 0.0, 0.0, 1.0 ]
        
  q = [ 0.0,  0.0, 0.0, 0.0 ]
  s = float( 0.0 )
  tr = h[ 0 ] + h[ 5 ] + h[ 10 ]
  if ( tr > 0.0 ):
  
    s = math.sqrt( tr + 1.0 )
    q[ 3 ] = s * 0.5
    s = 0.5 / s
    q[ 0 ] = ( h[ 6 ] - h[ 9 ] ) * s
    q[ 1 ] = ( h[ 8 ] - h[ 2 ] ) * s
    q[ 2 ] = ( h[ 1 ] - h[ 4 ] ) * s

  else:
  
    i = 0
    j = 0
    k = 0
    if ( h[ 5 ] > h[ 0 ] ):

      i = 1
      
    if ( h[ 10 ] > h[ i + i * 4 ] ):
    
      i = 2
      
    j = ( i + 1 ) % 3
    k = ( j + 1 ) % 3
    s = math.sqrt( h[ i + i * 4 ] - ( h[ j + j * 4 ] + h[ k + k * 4 ] ) + 1.0 )
    q[ i ] = s * 0.5
    s = 0.5 / s
    q[ 3 ] = ( h[ j * 4 + k ] - h[ k * 4 + j ] ) * s
    q[ j ] = ( h[ i * 4 + j ] + h[ j * 4 + i ] ) * s
    q[ k ] = ( h[ i * 4 + k ] + h[ k * 4 + i ] ) * s

  return q
  

def getCountString( count ):

  countString = str( count )
  zeroesToBeAdded = 10 - len( countString )
  zeroes = '0000000000'
  countString = zeroes[ 0: zeroesToBeAdded ] + countString
  print countString
  return countString
  
  
runqt = True
if QtGui.qApp.startingUp():

  qapp = QtGui.QApplication( sys.argv )

  theAnatomist = anatomist.Anatomist( )

  anatomistWindow3d = theAnatomist.createWindow( wintype='3D' )
  
  
  zoom = 0.7
  
  windowSize = [ 1024, 1024 ]
  
  backGroundColor = [ 1, 1, 1 ]
 
  theAnatomist.execute( 'WindowConfig',
                        windows = [ anatomistWindow3d ],
                        cursor_visibility = 0 )
  theAnatomist.execute( 'WindowConfig',
                        windows = [ anatomistWindow3d ],
                        show_toolbars = 0 )
  theAnatomist.execute( 'WindowConfig', 
                         windows = [ anatomistWindow3d ],
                         view_size = windowSize )
  theAnatomist.execute( 'WindowConfig',
                        windows = [ anatomistWindow3d ],
                        light = { 'background' : backGroundColor } )

  objects = list()

  #################### hierarchy
  hierarchy = theAnatomist.loadObject( \
  '/home/cpoupon/gkg-5.0/data/fiber-clustering/atlas-long-bundles/bundles.hie' )

  #################### bundles
  bundles = theAnatomist.loadObject( \
  '/home/cpoupon/Data/DemoGinkgo/Results/15-BundleLabelling/001/labelledBundles.bundles' )
  bundles.setMaterial( \
                     theAnatomist.Material( diffuse = [ 1.0, 1.0, 1.0, 0.4 ] ) )
  bundlesReferential = theAnatomist.createReferential()
  theAnatomist.assignReferential( bundlesReferential,
                                  bundles )

  objects.append( bundles )

  #################### head mesh
  headMesh = theAnatomist.loadObject( \
  '/home/cpoupon/Data/DemoGinkgo/Data/00-Morphologist/t1-1mm-1/001/t1mri/default_acquisition/default_analysis/segmentation/mesh/001_head.mesh' )
  headMesh.setMaterial( \
                     theAnatomist.Material( diffuse = [ 1.0, 1.0, 1.0, 0.1 ] ) )
  headMeshReferential = theAnatomist.createReferential()
  theAnatomist.assignReferential( headMeshReferential,
                                  headMesh )
  objects.append( headMesh )


  theAnatomist.loadTransformation(
  '/home/cpoupon/Data/DemoGinkgo/Results/06-ToAnatomyMatching/001/dw_to_t1.trm',
   bundlesReferential,
   headMeshReferential )

  theAnatomist.assignReferential( headMeshReferential,
                                  anatomistWindow3d )

  
  theAnatomist.addObjects( objects, [ anatomistWindow3d ] );


  quaternions = list()

  initialRotation = getRotationX( -math.pi / 2 )
  initialQuaternion = toQuaternion( initialRotation )

  quaternions.append( initialQuaternion )



  for p in range( 0, 201 ):

  
    angle = 2 * math.pi * float( p ) / 200.0 
    print angle
    rotationY = getRotationY( angle )
    rotation = composeRotations( rotationY, initialRotation )
   
    quaternions.append( toQuaternion( rotation ) )
    
  for p in range( 0, 201 ):

  
    angle = 2 * math.pi * float( p ) / 200.0 
    print angle
    rotationX = getRotationX( -angle )
    rotation = composeRotations( rotationX, initialRotation )
   
    quaternions.append( toQuaternion( rotation ) )
    
                    
  print quaternions

  count = 1
  for q in quaternions:
  
    anatomistWindow3d.camera( view_quaternion = q,
                              zoom = zoom )

    theAnatomist.execute( 'WindowConfig',
                          windows = [ anatomistWindow3d ],
                          snapshot = os.path.join( '/tmp/snapshot' + \
                                                   getCountString( count ) + \
                                                   '.jpg' ) )
                                                
    count = count + 1
    
  anatomistWindow3d.close()

