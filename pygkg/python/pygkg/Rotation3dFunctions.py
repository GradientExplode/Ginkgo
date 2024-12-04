import os
import math


################################################################################
# Function to handle transformations, rotations, scaling, shearing, translation
################################################################################

def getNorm( vector3d ):
  
  if len( vector3d ) != 3:
    
    message = 'getNorm: bad input vector length'
    raise RuntimeError, message
  
  x = vector3d[ 0 ]
  y = vector3d[ 1 ]
  z = vector3d[ 2 ]
  
  return math.sqrt( x * x + y * y + z * z )
  

def normalize( vector3d ):
  
  if len( vector3d ) != 3:
    
    message = 'normalize: bad input vector length'
    raise RuntimeError, message
  
  norm = getNorm( vector3d )
  
  vector3d[ 0 ] /= norm
  vector3d[ 1 ] /= norm
  vector3d[ 2 ] /= norm
  
  
def dot( vector3d1, vector3d2 ):
  
  if len( vector3d1 ) != 3 or len( vector3d2 ) != 3:
    
    message = 'dot: bad input vector length'
    raise RuntimeError, message
  
  return vector3d1[ 0 ] * vector3d2[ 0 ] + \
         vector3d1[ 1 ] * vector3d2[ 1 ] + \
         vector3d1[ 2 ] * vector3d2[ 2 ]
        
        
def cross( vector3d1, vector3d2 ):
  
  if len( vector3d1 ) != 3 or len( vector3d2 ) != 3:
    
    message = 'dot: bad input vector length'
    raise RuntimeError, message
  
  res = [ vector3d1[ 1 ] * vector3d2[ 2 ] - vector3d1[ 2 ] * vector3d2[ 1 ],
          vector3d1[ 2 ] * vector3d2[ 0 ] - vector3d1[ 0 ] * vector3d2[ 2 ],
          vector3d1[ 0 ] * vector3d2[ 1 ] - vector3d1[ 1 ] * vector3d2[ 0 ] ]
  return res 


def getError( matrix3x3, ax, ay, az ):

  if len( matrix3x3 ) != 3 or len( matrix3x3[ 0 ] ) != 3 :
    
    message = 'getError: bad input matrix size'
    raise RuntimeError, message
  
  cx = math.cos( ax );
  sx = math.sin( ax );
  sy = math.sin( ay );
  cz = math.cos( az );
  sz = math.sin( az );
  
  m = matrix3x3
  
  return math.fabs( m[ 0 ][ 1 ] - ( sx * sy * cz - cx * sz ) ) + \
         math.fabs( m[ 0 ][ 2 ] - ( cx * sy * cz + sx * sz ) ) + \
         math.fabs( m[ 1 ][ 1 ] - ( sx * sy * sz + cx * cz ) ) + \
         math.fabs( m[ 1 ][ 2 ] - ( cx * sy * sz - sx * cz ) )


def getXYZAnglesFromTrieder( vector3dU, vector3dV, vector3dW, degree = False ):

  if len( vector3dU ) != 3:
    
    message = 'getXYZAnglesFromTrieder: bad input vector length'
    raise RuntimeError, message
  
  angles = [ 0, 0, 0 ]
  
  U = vector3dU
  V = vector3dV
  W = vector3dW

  normalize( U );
  normalize( V );
  normalize( W );
  
  # sanity checks
  DOT_PRODUCT_ZERO = 0.05
   
  if ( ( dot( U, V ) > DOT_PRODUCT_ZERO ) or
       ( dot( U, W ) > DOT_PRODUCT_ZERO ) or
       ( dot( V, W ) > DOT_PRODUCT_ZERO ) ):
  
    message = 'not an orthogonal (u,v,w) trieder'
    raise RuntimeError, message
    
  if ( math.fabs( dot( cross( U, V ), W ) - 1.0 ) > DOT_PRODUCT_ZERO ):
  
    message = 'not a direct (u,v,w) trieder'
    raise RuntimeError, message
  

  m = [ [ U[ 0 ], V[ 0 ], W[ 0 ] ],
        [ U[ 1 ], V[ 1 ], W[ 1 ] ],
        [ U[ 2 ], V[ 2 ], W[ 2 ] ] ]


  if ( ( m[ 2 ][ 0 ] == 1 ) or ( m[ 2 ][ 0 ] == -1 ) ):

    by1 = math.pi / 2
    bz1 = 0.0
    bx1 = math.atan2( m[ 0 ][ 1 ], m[ 0 ][ 2 ] )
    by2 = -math.pi / 2

    # in that case the matrix can be written:
    #       0             -sx*cz-cx*sz            -cx*cz+sx*sz
    #       0             -sx*sz+cx*cz            -cx*sz-sx*cz
    #      -1                 0                      0
    # which leads to:
    #       0              -sin(ax+az)            -cos(ax+az)
    #       0               cos(ax+az)            -sin(ax+az)
    #       1                  0                      0
    # we can choose az = 0.0

    bz2 = 0.0
    bx2 = math.atan2( -m[ 0 ][ 1 ], -m[ 0 ][ 2 ] )

    if ( getError( m, bx1, by1, bz1 ) < getError( m, bx2, by2, bz2 ) ):

      angles[ 0 ] = bx1
      angles[ 1 ] = by1
      angles[ 2 ] = bz1

    else:

      angles[ 0 ] = bx2
      angles[ 1 ] = by2
      angles[ 2 ] = bz2

  else:

    
    by1 = math.asin( -m[ 2 ][ 0 ] )
    
    sign1 = 1.0
    if ( math.cos( by1 ) < 0.0 ):
      
      sign1 = -1.0
       
    else:
      
      sign1 = 1.0
      
    bx1 = math.atan2( m[ 2 ][ 1 ] / sign1, m[ 2 ][ 2 ] / sign1 )
    bz1 = math.atan2( m[ 1 ][ 0 ] / sign1, m[ 0 ][ 0 ] / sign1 )

    by2 = math.pi - by1

    if ( by2 > math.pi ):

      by2 -= 2 * math.pi

    sign2 = 1.0
    if ( math.cos( by2 ) < 0.0 ):
      
      sign2 = -1.0 
    else:
      
      sign2 = 1.0
    
    bx2 = math.atan2( m[ 2 ][ 1 ] / sign2, m[ 2 ][ 2 ] / sign2 )
    bz2 = math.atan2( m[ 1 ][ 0 ] / sign2, m[ 0 ][ 0 ] / sign2 )
    
    if ( getError( m, bx1, by1, bz1 ) < getError( m, bx2, by2, bz2 ) ):

      angles[ 0 ] = bx1
      angles[ 1 ] = by1
      angles[ 2 ] = bz1

    else:

      angles[ 0 ] = bx2
      angles[ 1 ] = by2
      angles[ 2 ] = bz2

  
  #####################################
  # Add in python version, additional sanity check
  for i in range( 0, 3 ):
    
    if angles[ i ] < -math.pi/2 :
    
      angles[ i ] += math.pi
    
    elif angles[ i ] > math.pi/2 :
    
      angles[ i ] -= math.pi
    
    else:
      
      nothingToDo = True
  #####################################

  if ( degree ):
  
    angles[ 0 ] *= ( 180.0 / math.pi )
    angles[ 1 ] *= ( 180.0 / math.pi )
    angles[ 2 ] *= ( 180.0 / math.pi )

  return angles


def getXYZScalingsAndTrieder( matrixRotationAndScaling,
                              vector3dScalings,
                              Vector3dU,
                              Vector3dV,
                              Vector3dW ):

  Vector3dU[ 0 ] = matrixRotationAndScaling[ 0, 0 ]
  Vector3dU[ 1 ] = matrixRotationAndScaling[ 1, 0 ]
  Vector3dU[ 2 ] = matrixRotationAndScaling[ 2, 0 ]

  Vector3dV[ 0 ] = matrixRotationAndScaling[ 0, 1 ]
  Vector3dV[ 1 ] = matrixRotationAndScaling[ 1, 1 ]
  Vector3dV[ 2 ] = matrixRotationAndScaling[ 2, 1 ]

  Vector3dW[ 0 ] = matrixRotationAndScaling[ 0, 2 ]
  Vector3dW[ 1 ] = matrixRotationAndScaling[ 1, 2 ]
  Vector3dW[ 2 ] = matrixRotationAndScaling[ 2, 2 ]

  # collecting scaling factors
  scalings[ 0 ] = getNorm( Vector3dU )
  scalings[ 1 ] = getNorm( Vector3dV )
  scalings[ 2 ] = getNorm( Vector3dW )

  # collecting trieder
  normalize( Vector3dU )
  normalize( Vector3dV )
  normalize( Vector3dW )

  

def getXYZScalingsAndShearingsAndTrieder( rotationAndScalingAndShearings ):

  vector3dScalings = [ 1.0, 1.0, 1.0 ]
  vector3dShearings = [ 0.0, 0.0, 0.0 ]
  vector3dU = [ 0.0, 0.0, 0.0 ]
  vector3dV = [ 0.0, 0.0, 0.0 ]
  vector3dW = [ 0.0, 0.0, 0.0 ]
  
  vector3dU[ 0 ] = rotationAndScalingAndShearings[ 0 ][ 0 ]
  vector3dU[ 1 ] = rotationAndScalingAndShearings[ 1 ][ 0 ]
  vector3dU[ 2 ] = rotationAndScalingAndShearings[ 2 ][ 0 ]

  vector3dV[ 0 ] = rotationAndScalingAndShearings[ 0 ][ 1 ]
  vector3dV[ 1 ] = rotationAndScalingAndShearings[ 1 ][ 1 ]
  vector3dV[ 2 ] = rotationAndScalingAndShearings[ 2 ][ 1 ]

  vector3dW[ 0 ] = rotationAndScalingAndShearings[ 0 ][ 2 ]
  vector3dW[ 1 ] = rotationAndScalingAndShearings[ 1 ][ 2 ]
  vector3dW[ 2 ] = rotationAndScalingAndShearings[ 2 ][ 2 ]

  vector3dScalings[ 2 ] = getNorm( vector3dW )
  unitW = [ 0.0, 0.0, 0.0 ]
  unitW[ 0 ] = vector3dW[ 0 ] / getNorm( vector3dW )
  unitW[ 1 ] = vector3dW[ 1 ] / getNorm( vector3dW )
  unitW[ 2 ] = vector3dW[ 2 ] / getNorm( vector3dW )
  
  unitU = cross( vector3dV, unitW )
  vector3dScalings[ 1 ] = getNorm( unitU )
  normalize( unitU )
  
  vector3dScalings[ 0 ] = dot( vector3dU, unitU )
  unitV = cross( unitW, unitU )
  
  vector3dShearings[ 0 ] = dot( vector3dU, unitV ) / vector3dScalings[ 0 ]
  vector3dShearings[ 2 ] = dot( vector3dV, unitW ) / vector3dScalings[ 1 ]
  vector3dShearings[ 1 ] = dot( vector3dU, unitW ) / vector3dScalings[ 0 ] - \
                           vector3dShearings[ 0 ] * vector3dShearings[ 2 ]

  vector3dU = unitU
  vector3dV = unitV
  vector3dW = unitW
  
  return [ vector3dScalings, vector3dShearings, 
           vector3dU, vector3dV, vector3dW ]


def getAffineParameters( homogeneousTransform ):
  
  
  if len( homogeneousTransform ) != 4 or \
     len( homogeneousTransform[ 0 ] ) != 4 :
    
    message = 'getAffineParameters: bad input transform size, should be 4x4'
    raise RuntimeError, message
  
  resVect = getXYZScalingsAndShearingsAndTrieder( homogeneousTransform );
  
  scalings = resVect[ 0 ]
  shearings = resVect[ 1 ]
  u = resVect[ 2 ]
  v = resVect[ 3 ]
  w = resVect[ 4 ]

  angles = getXYZAnglesFromTrieder( u, v, w, True )

  translations = [ homogeneousTransform[ 0 ][ 3 ],
                   homogeneousTransform[ 1 ][ 3 ],
                   homogeneousTransform[ 2 ][ 3 ] ]
  
  return [     scalings[ 0 ],     scalings[ 1 ],     scalings[ 2 ], 
              shearings[ 0 ],    shearings[ 1 ],    shearings[ 2 ],
                 angles[ 0 ],       angles[ 1 ],       angles[ 2 ],
           translations[ 0 ], translations[ 1 ], translations[ 2 ] ]
                                                     
                                                     
def getRigidParameters( homogeneousTransform ):
  
  
  if len( homogeneousTransform ) != 4 or \
     len( homogeneousTransform[ 0 ] ) != 4 :
    
    message = 'getRigidParameters: bad input transform size, should be 4x4'
    raise RuntimeError, message
  
  affineParameters = getAffineParameters( homogeneousTransform )
  
  
  rigidParameters = [ affineParameters [ 6 ],    # degX
                      affineParameters [ 7 ],    # degY
                      affineParameters [ 8 ],    # degZ
                      affineParameters [ 9 ],    # tX
                      affineParameters [ 10 ],   # tY
                      affineParameters [ 11 ] ]  # tZ

  return rigidParameters
  

def getAffineParametersFromAnatomistTransform( transform ):
  
  if type( transform ) == str:
    
    # Read transformation in file 
    f = open( transform, 'r' )
    dataRead = [ line.split() for line in f ]
    f.close()
    
    d = dataRead
     
    # Create 4x3 matrix
    matrix4x3 = [[ float( d[0][0] ), float( d[0][1] ), float( d[0][2] ) ],
                 [ float( d[1][0] ), float( d[1][1] ), float( d[1][2] ) ],
                 [ float( d[2][0] ), float( d[2][1] ), float( d[2][2] ) ],
                 [ float( d[3][0] ), float( d[3][1] ), float( d[3][2] ) ]]
    
  elif type( transform ) == list:
  
    matrix4x3 = transform
    
    if len( matrix4x3 ) != 4 or \
       len( matrix4x3[ 0 ] ) != 3 :
      
      message = 'getAffineParametersFromAnatomistTransform: ' + \
                'bad input matrix size, should be 4x3'
      raise RuntimeError, message
    
  else:
  
    message = 'getRigidParametersFromAnatomistTransform bad input type'
    raise RuntimeError, message
    
  m = matrix4x3
  homogeneousTransform = \
                       [ [ m[ 1 ][ 0 ], m[ 1 ][ 1 ], m[ 1 ][ 2 ], m[ 0 ][ 0 ] ],
                         [ m[ 2 ][ 0 ], m[ 2 ][ 1 ], m[ 2 ][ 2 ], m[ 0 ][ 1 ] ],
                         [ m[ 3 ][ 0 ], m[ 3 ][ 1 ], m[ 3 ][ 2 ], m[ 0 ][ 2 ] ],
                         [           0,           0,           0,         1 ] ]
                         
  return getAffineParameters( homogeneousTransform )


def getRigidParametersFromAnatomistTransform( transform ):
  
  # transform is either a trm file or a matrix4x3
  
  if type( transform ) == str:
    
    # Read transformation in file 
    f = open( transform, 'r' )
    dataRead = [ line.split() for line in f ]
    f.close()
    
    d = dataRead
     
    # Create 4x3 matrix
    matrix4x3 = [[ float( d[0][0] ), float( d[0][1] ), float( d[0][2] ) ],
                 [ float( d[1][0] ), float( d[1][1] ), float( d[1][2] ) ],
                 [ float( d[2][0] ), float( d[2][1] ), float( d[2][2] ) ],
                 [ float( d[3][0] ), float( d[3][1] ), float( d[3][2] ) ]]
    
  elif type( transform ) == list:
  
    matrix4x3 = transform
  
    if len( matrix4x3 ) != 4 or len( matrix4x3[ 0 ] ) != 3 :
      
      message = 'getRigidParametersFromAnatomistTransform: ' + \
                'bad input matrix size, should be 4x3'
      raise RuntimeError, message
    
  else:
  
    message = 'getRigidParametersFromAnatomistTransform bad input type'
    raise RuntimeError, message
  
  m = matrix4x3
  homogeneousTransform = \
                       [ [ m[ 1 ][ 0 ], m[ 1 ][ 1 ], m[ 1 ][ 2 ], m[ 0 ][ 0 ] ],
                         [ m[ 2 ][ 0 ], m[ 2 ][ 1 ], m[ 2 ][ 2 ], m[ 0 ][ 1 ] ],
                         [ m[ 3 ][ 0 ], m[ 3 ][ 1 ], m[ 3 ][ 2 ], m[ 0 ][ 2 ] ],
                         [           0,           0,           0,         1 ] ]
                         
  return getRigidParameters( homogeneousTransform )
  

def multiplyTwo4x4Matrix( a, b ):
  
  c = [ [ 0, 0, 0, 0 ], [ 0, 0, 0, 0 ], [ 0, 0, 0, 0 ], [ 0, 0, 0, 0 ] ]
  
  c[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + \
            a[0][2] * b[2][0] + a[0][3] * b[3][0];
  c[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + \
            a[0][2] * b[2][1] + a[0][3] * b[3][1];
  c[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + \
            a[0][2] * b[2][2] + a[0][3] * b[3][2];
  c[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + \
            a[0][2] * b[2][3] + a[0][3] * b[3][3];
  c[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + \
            a[1][2] * b[2][0] + a[1][3] * b[3][0];
  c[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + \
            a[1][2] * b[2][1] + a[1][3] * b[3][1];
  c[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + \
            a[1][2] * b[2][2] + a[1][3] * b[3][2];
  c[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + \
            a[1][2] * b[2][3] + a[1][3] * b[3][3];
  c[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + \
            a[2][2] * b[2][0] + a[2][3] * b[3][0];
  c[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + \
            a[2][2] * b[2][1] + a[2][3] * b[3][1];
  c[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + \
            a[2][2] * b[2][2] + a[2][3] * b[3][2];
  c[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + \
            a[2][2] * b[2][3] + a[2][3] * b[3][3];
  c[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + \
            a[3][2] * b[2][0] + a[3][3] * b[3][0];
  c[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + \
            a[3][2] * b[2][1] + a[3][3] * b[3][1];
  c[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + \
            a[3][2] * b[2][2] + a[3][3] * b[3][2];
  c[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + \
            a[3][2] * b[2][3] + a[3][3] * b[3][3];
  
  return c


def createHomogeneousTransform( affineParameters, degree = True ):

  if len( affineParameters ) != 12:
    
    message = 'createHomogeneousTransform: bad affine parameters size, ' + \
              'expecting 12!'
    raise RuntimeError, message
  
  sx = affineParameters[ 0 ]
  sy = affineParameters[ 1 ]
  sz = affineParameters[ 2 ]
  
  sxy = affineParameters[ 3 ]
  sxz = affineParameters[ 4 ]
  syz = affineParameters[ 5 ]
  
  if degree:
    
    rx = affineParameters[ 6 ] * math.pi / 180.0
    ry = affineParameters[ 7 ] * math.pi / 180.0
    rz = affineParameters[ 8 ] * math.pi / 180.0
  
  else:
  
    rx = affineParameters[ 6 ]
    ry = affineParameters[ 7 ]
    rz = affineParameters[ 8 ]
  
  tx = affineParameters[ 9 ]
  ty = affineParameters[ 10 ]
  tz = affineParameters[ 11 ]
  
  
  scalingsMatrix = [ [ sx, 0,  0, 0 ], 
                     [ 0, sy,  0, 0 ], 
                     [ 0,  0, sz, 0 ],
                     [ 0,  0,  0, 1 ] ]
  
  shearingsMatrix = [ [ 1,     0,  0,  0 ], 
                      [ sxy,   1,  0,  0 ], 
                      [ sxz, syz,  1,  0 ],
                      [ 0,     0,  0,  1 ] ]
  
  rotationMatrixX = [ [ 1,              0,               0, 0 ], 
                      [ 0, math.cos( rx ), -math.sin( rx ), 0 ], 
                      [ 0, math.sin( rx ),  math.cos( rx ), 0 ],
                      [ 0,              0,               0, 1 ] ]
  
  rotationMatrixY = [ [  math.cos( ry ), 0, math.sin( ry ), 0 ],
                      [               0, 1,              0, 0 ],
                      [ -math.sin( ry ), 0, math.cos( ry ), 0 ],
                      [ 0,               0,              0, 1 ] ]
  
  rotationMatrixZ = [ [ math.cos( rz ), -math.sin( rz ), 0, 0 ], 
                      [ math.sin( rz ),  math.cos( rz ), 0, 0 ],
                      [              0, 0,               1, 0 ],
                      [              0, 0,               0, 1 ] ]
  
  translationMatrix = [ [ 1, 0, 0, tx ],
                        [ 0, 1, 0, ty ],
                        [ 0, 0, 1, tz ],
                        [ 0, 0, 0,  1 ] ]
  
  op = multiplyTwo4x4Matrix
  
  rotationsMatrix = op( rotationMatrixZ, 
                        op( rotationMatrixY, rotationMatrixX ) )
  
  # Composition
  matrix = op( translationMatrix, op( rotationsMatrix, 
                                   op( shearingsMatrix, scalingsMatrix ) ) )
  
  return matrix 


def createHomogeneousTransformFromRigidParameters( rigidParameters ):

  if len( rigidParameters ) != 6:
    
    message = 'createHomogeneousTransformFromRigidParameters: bad ' + \
              'rigid parameters size, expecting 6!'
    raise RuntimeError, message
  
  sx = sy = sz = 1
  sxy = sxz = syz = 0
  
  affineParameters = [ sx, sy, sz, sxy, sxz, syz, \
                       rigidParameters[ 0 ], rigidParameters[ 1 ],
                       rigidParameters[ 2 ], rigidParameters[ 3 ],
                       rigidParameters[ 4 ], rigidParameters[ 5 ] ]
  
  return createHomogeneousTransform( affineParameters )


