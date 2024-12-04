#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define DOT_PRODUCT_ZERO   0.05


template < class T >
static T getError( T m[ 3 ][ 3 ], T ax, T ay, T az )
{

  T cx = std::cos( ax );
  T sx = std::sin( ax );
  T sy = std::sin( ay );
  T cz = std::cos( az );
  T sz = std::sin( az );

  return std::fabs( m[ 0 ][ 1 ] - ( sx * sy * cz - cx * sz ) ) +
         std::fabs( m[ 0 ][ 2 ] - ( cx * sy * cz + sx * sz ) ) +
         std::fabs( m[ 1 ][ 1 ] - ( sx * sy * sz + cx * cz ) ) +
         std::fabs( m[ 1 ][ 2 ] - ( cx * sy * sz - sx * cz ) );

}


namespace gkg
{


template < class T >
Vector3d< T > getXYZAnglesFromTrieder( const Vector3d< T >& u,
                                       const Vector3d< T >& v,
                                       const Vector3d< T >& w,
                                       bool degree )
{

  try
  {

    Vector3d< T > angles;
    Vector3d< T > U = u;
    Vector3d< T > V = v;
    Vector3d< T > W = w;

    U.normalize();
    V.normalize();
    W.normalize();

    // sanity checks
    if ( ( U.dot( V ) > DOT_PRODUCT_ZERO ) ||
         ( U.dot( W ) > DOT_PRODUCT_ZERO ) ||
         ( V.dot( W ) > DOT_PRODUCT_ZERO ) )
    {

      throw std::runtime_error( "not an orthogonal (u,v,w) trieder" );

    }
    if ( std::fabs( U.cross( V ).dot( W ) - 1.0 ) > DOT_PRODUCT_ZERO )
    {

      throw std::runtime_error( "not a direct (u,v,w) trieder" );

    }

    T m[ 3 ][ 3 ];

    m[ 0 ][ 0 ] = U.x;
    m[ 0 ][ 1 ] = V.x;
    m[ 0 ][ 2 ] = W.x;
    m[ 1 ][ 0 ] = U.y;
    m[ 1 ][ 1 ] = V.y;
    m[ 1 ][ 2 ] = W.y;
    m[ 2 ][ 0 ] = U.z;
    m[ 2 ][ 1 ] = V.z;
    m[ 2 ][ 2 ] = W.z;

    // case where cos( ay ) = 0
    if ( ( m[ 2 ][ 0 ] == 1 ) || ( m[ 2 ][ 0 ] == -1 ) )
    {

      T by1 = + M_PI / 2;
      // in that case the matrix can be written:
      //       0             sx*cz-cx*sz            cx*cz+sx*sz
      //       0             sx*sz+cx*cz            cx*sz-sx*cz
      //       -1                 0                      0
      // which leads to:
      //       0              sin(ax-az)            cos(ax-az)
      //       0              cos(ax-az)           -sin(ax-az)
      //      -1                  0                      0
      // we can choose az = 0.0
      T bz1 = 0.0;
      T bx1 = std::atan2( m[ 0 ][ 1 ], m[ 0 ][ 2 ] );

      T by2 = - M_PI / 2;
      // in that case the matrix can be written:
      //       0             -sx*cz-cx*sz            -cx*cz+sx*sz
      //       0             -sx*sz+cx*cz            -cx*sz-sx*cz
      //       -1                 0                      0
      // which leads to:
      //       0              -sin(ax+az)            -cos(ax+az)
      //       0              cos(ax+az)             -sin(ax+az)
      //       1                  0                      0
      // we can choose az = 0.0
      T bz2 = 0.0;
      T bx2 = std::atan2( -m[ 0 ][ 1 ], -m[ 0 ][ 2 ] );

      if ( getError( m, bx1, by1, bz1 ) < getError( m, bx2, by2, bz2 ) )
      {

        angles.x = bx1;
        angles.y = by1;
        angles.z = bz1;

      }
      else
      {

        angles.x = bx2;
        angles.y = by2;
        angles.z = bz2;

      }

    }
    else
    {

      T by1 = std::asin( -m[ 2 ][ 0 ] );
      T sign1 = ( std::cos( by1 ) < 0.0 ? -1.0 : +1.0 );
      T bx1 = std::atan2( m[ 2 ][ 1 ] / sign1, m[ 2 ][ 2 ] / sign1 );
      T bz1 = std::atan2( m[ 1 ][ 0 ] / sign1, m[ 0 ][ 0 ] / sign1 );

      T by2 = M_PI - by1;
      if ( by2 > M_PI )
      {

        by2 -= 2 * M_PI;

      }
      T sign2 = ( std::cos( by2 ) < 0.0 ? -1.0 : +1.0 );
      T bx2 = std::atan2( m[ 2 ][ 1 ] / sign2, m[ 2 ][ 2 ] / sign2 );
      T bz2 = std::atan2( m[ 1 ][ 0 ] / sign2, m[ 0 ][ 0 ] / sign2 );


      if ( getError( m, bx1, by1, bz1 ) < getError( m, bx2, by2, bz2 ) )
      {

        angles.x = bx1;
        angles.y = by1;
        angles.z = bz1;

      }
      else
      {

        angles.x = bx2;
        angles.y = by2;
        angles.z = bz2;

      }

    }

    if ( degree )
    {

      angles *= 180.0 / M_PI;

    }

    return angles;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > gkg::getXYZAnglesFromTrieder( "
             "const gkg::Vector3d< T >& u, "
             "const gkg::Vector3d< T >& v, "
             "const gkg::Vector3d< T >& w, "
             "bool degree )" );

}


template < class T >
void getXYZScalingsAndTrieder( const Matrix& rotationAndScaling,
                               Vector3d< T >& scalings,
                               Vector3d< T >& u,
                               Vector3d< T >& v,
                               Vector3d< T >& w )
{

  try
  {

    // collecting trieder
    u.x = rotationAndScaling( 0, 0 );
    u.y = rotationAndScaling( 1, 0 );
    u.z = rotationAndScaling( 2, 0 );

    v.x = rotationAndScaling( 0, 1 );
    v.y = rotationAndScaling( 1, 1 );
    v.z = rotationAndScaling( 2, 1 );

    w.x = rotationAndScaling( 0, 2 );
    w.y = rotationAndScaling( 1, 2 );
    w.z = rotationAndScaling( 2, 2 );

    // collecting scaling factors
    scalings.x = u.getNorm();
    scalings.y = v.getNorm();
    scalings.z = w.getNorm();

    // collecting trieder
    u.normalize();
    v.normalize();
    w.normalize();

  }
  GKG_CATCH( "template < class T > "
             "void getXYZScalingsAndTrieder( const Matrix& rotationAndScaling, "
             "Vector3d< T >& scalings, "
             "Vector3d< T >& u, "
             "Vector3d< T >& v, "
             "Vector3d< T >& w )" );


}


template < class T >
void getXYZScalingsAndShearingsAndTrieder(
                               const Matrix& rotationAndScalingAndShearings,
                               Vector3d< T >& scalings,
                               Vector3d< T >& shearings,
                               Vector3d< T >& u,
                               Vector3d< T >& v,
                               Vector3d< T >& w )
{

  try
  {

    // collecting trieder
    u.x = rotationAndScalingAndShearings( 0, 0 );
    u.y = rotationAndScalingAndShearings( 1, 0 );
    u.z = rotationAndScalingAndShearings( 2, 0 );

    v.x = rotationAndScalingAndShearings( 0, 1 );
    v.y = rotationAndScalingAndShearings( 1, 1 );
    v.z = rotationAndScalingAndShearings( 2, 1 );

    w.x = rotationAndScalingAndShearings( 0, 2 );
    w.y = rotationAndScalingAndShearings( 1, 2 );
    w.z = rotationAndScalingAndShearings( 2, 2 );

    Vector3d< T > unitU;
    Vector3d< T > unitV;
    Vector3d< T > unitW;

    scalings.z = w.getNorm();
    unitW = w / w.getNorm();

    unitU = v.cross( unitW );
    scalings.y = unitU.getNorm();
    unitU.normalize();
    
    scalings.x = u.dot( unitU );
    unitV = unitW.cross( unitU );

    shearings.x = u.dot( unitV ) / scalings.x;
    shearings.z = v.dot( unitW ) / scalings.y;
    shearings.y = u.dot( unitW ) / scalings.x - shearings.x * shearings.z;

    u = unitU;
    v = unitV;
    w = unitW;

  }
  GKG_CATCH( "template < class T > "
             "void getXYZScalingsAndShearingsAndTrieder( "
             "const Matrix& rotationAndScalingAndShearings, "
             "Vector3d< T >& scalings, "
             "Vector3d< T >& shearings, "
             "Vector3d< T >& u, "
             "Vector3d< T >& v, "
             "Vector3d< T >& w )" );


}


template < class T >
void getRandomOrthonormalTriederFromFirstAxis(
                                           const gkg::Vector3d< T >& firstAxis,
                                           gkg::Vector3d< T >& secondAxis,
					   gkg::Vector3d< T >& thirdAxis )
{
						
  try
  {

    gkg::Vector3d< T > copyOfFirstAxis( firstAxis );
    copyOfFirstAxis.normalize();

    T absX = std::abs( copyOfFirstAxis.x );
    T absY = std::abs( copyOfFirstAxis.y );
    T absZ = std::abs( copyOfFirstAxis.z );


    //////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    //////////////////////////////////////////////////////////////////////////
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                 getImplementationFactory();

    // initializing random generator 
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    if ( ( gkg::equal( copyOfFirstAxis.x, ( T )1.0 , ( T )1e-10 ) ) &&
         ( gkg::equal( copyOfFirstAxis.y, ( T )0.0 , ( T )1e-10 ) ) &&
         ( gkg::equal( copyOfFirstAxis.z, ( T )0.0 , ( T )1e-10 ) ) )
    {

      secondAxis.x = ( T )0.0;
      secondAxis.y = ( T )1.0;
      secondAxis.z = ( T )0.0;

      thirdAxis.x = ( T )0.0;
      thirdAxis.y = ( T )0.0;
      thirdAxis.z = ( T )1.0;

    }
    else if ( ( gkg::equal( copyOfFirstAxis.x, ( T )0.0 , ( T )1e-10 ) ) &&
              ( gkg::equal( copyOfFirstAxis.y, ( T )1.0 , ( T )1e-10 ) ) &&
              ( gkg::equal( copyOfFirstAxis.z, ( T )0.0 , ( T )1e-10 ) ) )
    {

      secondAxis.x = ( T )0.0;
      secondAxis.y = ( T )0.0;
      secondAxis.z = ( T )1.0;

      thirdAxis.x = ( T )1.0;
      thirdAxis.y = ( T )0.0;
      thirdAxis.z = ( T )0.0;

    }
    else if ( ( gkg::equal( copyOfFirstAxis.x, ( T )0.0 , ( T )1e-10 ) ) &&
              ( gkg::equal( copyOfFirstAxis.y, ( T )0.0 , ( T )1e-10 ) ) &&
              ( gkg::equal( copyOfFirstAxis.z, ( T )1.0 , ( T )1e-10 ) ) )
    {

      secondAxis.x = ( T )1.0;
      secondAxis.y = ( T )0.0;
      secondAxis.z = ( T )0.0;

      thirdAxis.x = ( T )0.0;
      thirdAxis.y = ( T )1.0;
      thirdAxis.z = ( T )0.0;

    }
    else if ( !gkg::equal( copyOfFirstAxis.y, ( T )0.0 , ( T )1e-10 ) &&
              ( ( absY >= absX ) && ( absY >= absZ ) ) )
    {

      T x2 = copyOfFirstAxis.x * copyOfFirstAxis.x;
      T y2 = copyOfFirstAxis.y * copyOfFirstAxis.y;
      T z2 = copyOfFirstAxis.z * copyOfFirstAxis.z;
      T xz = 2.0 * copyOfFirstAxis.x * copyOfFirstAxis.z;
      double limit = std::sqrt( ( x2 + y2 ) / ( x2 + y2 + z2 ) );

      secondAxis.z = ( T )factory->getUniformRandomNumber( randomGenerator, 
                                                           -limit,
                                                           +limit );

      double delta = ( xz * secondAxis.z ) * ( xz * secondAxis.z ) -
                     ( 4 * ( x2 + y2 ) * 
                       ( ( y2 + z2 ) * secondAxis.z * secondAxis.z - y2 ) );

      secondAxis.x = ( T )( ( -xz * secondAxis.z + std::sqrt( delta ) ) / 
                            ( 2 * ( x2 + y2 ) ) );
      secondAxis.y = ( T )( -( copyOfFirstAxis.x / copyOfFirstAxis.y ) * 
                             secondAxis.x -
                             ( copyOfFirstAxis.z / copyOfFirstAxis.y ) *
                             secondAxis.z );

      thirdAxis = copyOfFirstAxis.cross( secondAxis );

    }
    else if ( !gkg::equal( copyOfFirstAxis.z, ( T )0.0 , ( T )1e-10 ) &&
              ( ( absZ >= absX ) && ( absZ >= absY ) ) )
    {

      T x2 = copyOfFirstAxis.x * copyOfFirstAxis.x;
      T y2 = copyOfFirstAxis.y * copyOfFirstAxis.y;
      T z2 = copyOfFirstAxis.z * copyOfFirstAxis.z;
      T xy = 2.0 * copyOfFirstAxis.x * copyOfFirstAxis.y;
      double limit = std::sqrt( ( y2 + z2 ) / ( x2 + y2 + z2 ) );

      secondAxis.x = ( T )factory->getUniformRandomNumber( randomGenerator, 
                                                           -limit,
                                                           +limit );
      double delta = ( xy * secondAxis.x ) * ( xy * secondAxis.x ) -
                     ( 4 * ( y2 + z2 ) * 
                       ( ( x2 + z2 ) * secondAxis.x * secondAxis.x - z2 ) );

      secondAxis.y = ( T )( ( -xy * secondAxis.x + std::sqrt( delta ) ) / 
                            ( 2 * ( y2 + z2 ) ) );
      secondAxis.z = ( T )( -( copyOfFirstAxis.x / copyOfFirstAxis.z ) *
                             secondAxis.x -
                             ( copyOfFirstAxis.y / copyOfFirstAxis.z ) *
                             secondAxis.y );

      thirdAxis = copyOfFirstAxis.cross( secondAxis );
      
    }
    else if ( !gkg::equal( copyOfFirstAxis.x, ( T )0.0 , ( T )1e-10 ) &&
              ( ( absX >= absY ) && ( absX >= absZ ) )  )
    {

      T x2 = copyOfFirstAxis.x * copyOfFirstAxis.x;
      T y2 = copyOfFirstAxis.y * copyOfFirstAxis.y;
      T z2 = copyOfFirstAxis.z * copyOfFirstAxis.z;
      T yz = 2.0 * copyOfFirstAxis.y * copyOfFirstAxis.z;
      double limit = std::sqrt( ( x2 + y2 ) / ( x2 + y2 + z2 ) );

      secondAxis.z = ( T )factory->getUniformRandomNumber( randomGenerator, 
                                                           -limit,
                                                           +limit );

      double
        delta = ( yz * secondAxis.z ) * ( yz * secondAxis.z ) -
                ( 4 * ( x2 + y2 ) * 
                  ( ( x2 + z2 ) * secondAxis.z * secondAxis.z - x2 ) );

      secondAxis.y = ( T )( ( -yz * secondAxis.z + std::sqrt( delta ) ) / 
                            ( 2 * ( x2 + y2 ) ) );
      secondAxis.x = ( T )( -( copyOfFirstAxis.y / copyOfFirstAxis.x ) *
                             secondAxis.y -
                             ( copyOfFirstAxis.z / copyOfFirstAxis.x ) *
                             secondAxis.z );

      thirdAxis = copyOfFirstAxis.cross( secondAxis );
      
    }
           


  }
  GKG_CATCH( "template < class T > "
             "void getRandomOrthonormalTriederFromFirstAxis( "
             "const gkg::Vector3d< T >& firstAxis, "
             "gkg::Vector3d< T >& secondAxis, "
	     "gkg::Vector3d< T >& thirdAxis )" );					     
						
						
}



template
Vector3d< float > getXYZAnglesFromTrieder< float >( const Vector3d< float >& u,
                                                    const Vector3d< float >& v,
                                                    const Vector3d< float >& w,
                                                    bool degree );

template
Vector3d< double > getXYZAnglesFromTrieder< double >(
                                                   const Vector3d< double >& u,
                                                   const Vector3d< double >& v,
                                                   const Vector3d< double >& w,
                                                   bool degree );

template
void getXYZScalingsAndTrieder< float >( const Matrix& rotationAndScaling,
                                        Vector3d< float >& scalings,
                                        Vector3d< float >& u,
                                        Vector3d< float >& v,
                                        Vector3d< float >& w );

template
void getXYZScalingsAndTrieder< double >( const Matrix& rotationAndScaling,
                                         Vector3d< double >& scalings,
                                         Vector3d< double >& u,
                                         Vector3d< double >& v,
                                         Vector3d< double >& w );

template
void getXYZScalingsAndShearingsAndTrieder< float >(
                                   const Matrix& rotationAndScalingAndShearings,
                                   Vector3d< float >& scalings,
                                   Vector3d< float >& shearings,
                                   Vector3d< float >& u,
                                   Vector3d< float >& v,
                                   Vector3d< float >& w );

template
void getXYZScalingsAndShearingsAndTrieder< double >(
                                   const Matrix& rotationAndScalingAndShearings,
                                   Vector3d< double >& scalings,
                                   Vector3d< double >& shearings,
                                   Vector3d< double >& u,
                                   Vector3d< double >& v,
                                   Vector3d< double >& w );

template 
void getRandomOrthonormalTriederFromFirstAxis< float >(
                                      const gkg::Vector3d< float >& firstAxis,
                                      gkg::Vector3d< float >& secondAxis,
				      gkg::Vector3d< float >& thirdAxis );


template 
void getRandomOrthonormalTriederFromFirstAxis< double >(
                                      const gkg::Vector3d< double >& firstAxis,
                                      gkg::Vector3d< double >& secondAxis,
				      gkg::Vector3d< double >& thirdAxis );

}


#undef DOT_PRODUCT_ZERO
