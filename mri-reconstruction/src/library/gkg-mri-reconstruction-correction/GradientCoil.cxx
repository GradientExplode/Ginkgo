#include <gkg-mri-reconstruction-correction/GradientCoil.h>
#include <gkg-core-exception/Exception.h>


//
// useful macro(s)
//
#define power1( a ) ( a )
#define power2( a ) ( ( a ) * ( a ) )
#define power3( a ) ( ( a ) * ( a ) * ( a ) )
#define power4( a ) ( ( a ) * ( a ) * ( a ) * ( a ) )
#define power5( a ) ( ( a ) * ( a ) * ( a ) * ( a ) * ( a ) )


// coefficient index macro(s):
//  (0,0) (0,1) (0,2) (0,3) (0,4) (0,5)
//  (1,0) (1,1) (1,2) (1,3) (1,4)
//  (2,0) (2,1) (2,2) (2,3)
//  (3,0) (3,1) (3,2)
//  (4,0) (4,1)
//  (5,0)

#define I0_0   0
#define I0_1   1
#define I0_2   2
#define I0_3   3
#define I0_4   4
#define I0_5   5
#define I1_0   6
#define I1_1   7
#define I1_2   8
#define I1_3   9
#define I1_4  10
#define I2_0  11
#define I2_1  12
#define I2_2  13
#define I2_3  14
#define I3_0  15
#define I3_1  16
#define I3_2  17
#define I4_0  18
#define I4_1  19
#define I5_0  20


//
// class GradientCoil< T >
//
template < class T >
gkg::GradientCoil< T >::GradientCoil()
{
}


template < class T >
gkg::GradientCoil< T >::~GradientCoil()
{
}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus11( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * power1( point.x );
    wedge[ I0_1 ] += coefficient * power1( phaseVector.x );

    wedge[ I1_0 ] += coefficient * power1( frequencyVector.x );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus11(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}



template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus21( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.x ) * power1( point.z ) );
    wedge[ I0_1 ] += coefficient * ( power1( phaseVector.z ) * 
                                     power1( point.x ) +
                                     power1( phaseVector.x ) *
                                     power1( point.z ) );
    wedge[ I0_2 ] += coefficient * ( power1( phaseVector.x ) * 
                                     power1( phaseVector.z ) );

    wedge[ I1_0 ] += coefficient * ( power1( frequencyVector.z ) * 
                                     power1( point.x ) +
                                     power1( frequencyVector.x ) *
                                     power1( point.z ) );
    wedge[ I1_1 ] += coefficient * ( power1( frequencyVector.z ) *
                                     power1( phaseVector.x ) +
                                     power1( frequencyVector.x ) *
                                     power1( phaseVector.z ) );

    wedge[ I2_0 ] += coefficient * ( power1( frequencyVector.x ) *
                                     power1( frequencyVector.z ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus21(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}



template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus31( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.x ) * ( -power2( point.x ) -
                     power2( point.y ) + 4 * power2( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( -3 * power1( phaseVector.x ) *
                     power2( point.x ) - 2 * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) -
                     power1( phaseVector.x ) * power2( point.y ) + 
                     8 * power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) + 4 * power1( phaseVector.x ) *
                     power2( point.z ) );
    wedge[ I0_2 ] += coefficient * ( -3 * power2( phaseVector.x ) *
                     power1( point.x ) - power2( phaseVector.y ) *
                     power1( point.x ) + 4 * power2( phaseVector.z ) *
                     power1( point.x ) - 2 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) +
                     8 * power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) );
    wedge[ I0_3 ] += coefficient * ( power1( phaseVector.x ) *
                     ( -power2( phaseVector.x ) - power2( phaseVector.y ) +
                     4 * power2( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( -3 * power1( frequencyVector.x ) *
                     power2( point.x ) - 2 * power1( frequencyVector.y ) *
                     power1( point.x ) * power1( point.y ) -
                     power1( frequencyVector.x ) * power2( point.y ) +
                     8 * power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) + 4 * power1( frequencyVector.x ) *
                     power2( point.z ) );
    wedge[ I1_1 ] += coefficient * ( 2 * ( -3 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.x ) -
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.x ) + 4 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.y ) - power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) +
                     4 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.z ) + 4 * power1( frequencyVector.x ) *
                     power1( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( -3 * power1( frequencyVector.x ) *
                     power2( phaseVector.x ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) -
                     power1( frequencyVector.x ) * power2( phaseVector.y ) +
                     8 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) + 4 * power1( frequencyVector.x ) *
                     power2( phaseVector.z ) );

    wedge[ I2_0 ] += coefficient * ( -3 * power2( frequencyVector.x ) *
                     power1( point.x ) - power2( frequencyVector.y ) *
                     power1( point.x ) + 4 * power2( frequencyVector.z ) *
                     power1( point.x ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) +
                     8 * power1( frequencyVector.x ) * 
                     power1( frequencyVector.z ) * power1( point.z ) );
    wedge[ I2_1 ] += coefficient * ( -3 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) - power2( frequencyVector.y ) *
                     power1( phaseVector.x ) + 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) +
                     8 * power1( frequencyVector.x ) * 
                     power1( frequencyVector.z ) * power1( phaseVector.z ) );

    wedge[ I3_0 ] += coefficient * ( power1( frequencyVector.x ) * 
                     ( -power2( frequencyVector.x ) -
                     power2( frequencyVector.y ) + 4 * 
                     power2( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus31(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}



template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus41( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.x ) * power1( point.z ) *
                     ( -3 * power2( point.x ) - 3 * power2( point.y ) + 
                     4 * power2( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( -3 * power1( phaseVector.z ) * 
                     power3( point.x ) - 3 * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.y ) - 9 *
                     power1( phaseVector.x ) * power2( point.x ) *
                     power1( point.z ) - 6 * power1( phaseVector.y ) * 
                     power1( point.x ) * power1( point.y ) * power1( point.z ) -
                     3 * power1( phaseVector.x ) * power2( point.y ) *
                     power1( point.z ) + 12 * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.z ) + 
                     4 * power1( phaseVector.x ) * power3( point.z ) );
    wedge[ I0_2 ] += coefficient * ( 3 * ( -3 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power2( point.x ) -
                     2 * power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) - 
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) - 3 * power2( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) - 
                     power2( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) +  4 * power2( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) - 
                     2 * power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) * power1( point.z ) +
                     4 * power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I0_3 ] += coefficient * ( -9 * power2( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     3 * power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) + 4 * power3( phaseVector.z ) *
                     power1( point.x ) - 6 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 3 * power3( phaseVector.x ) *
                     power1( point.z ) - 3 * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) + 
                     12 * power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) );
    wedge[ I0_4 ] += coefficient * ( power1( phaseVector.x ) *
                     power1( phaseVector.z ) * ( -3 * power2( phaseVector.x ) -
                     3 * power2( phaseVector.y ) +
                     4 * power2( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( -3 * power1( frequencyVector.z ) *
                     power3( point.x ) - 3 * power1( frequencyVector.z ) *
                     power1( point.x ) * power2( point.y ) -
                     9 * power1( frequencyVector.x ) * power2( point.x ) *
                     power1( point.z ) - 6 * power1( frequencyVector.y ) *
                     power1( point.x ) * power1( point.y ) * power1( point.z ) -
                     3 * power1( frequencyVector.x ) * power2( point.y ) *
                     power1( point.z ) + 12 * power1( frequencyVector.z ) *
                     power1( point.x ) * power2( point.z ) + 
                     4 * power1( frequencyVector.x ) * power3( point.z ) );
    wedge[ I1_1 ] += coefficient * ( 3 * ( -3 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.x ) -
                     3 * power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power2( point.x ) - 2 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.y ) - power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.y ) -
                     power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) - 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power1( point.z ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) + 8 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.y ) *
                     power1( point.z ) - 2 * power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) + 4 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.z ) + 
                     4 * power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( 3 * ( -3 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( point.x ) -
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power1( point.x ) - 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) + 4 * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) * power1( point.x ) -
                     2 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) - 
                     2*power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.y ) -
                     2 * power1( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) -
                     3 * power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power1( point.z ) - 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.z ) - power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) +
                     8 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.z ) +
                     4 * power1( frequencyVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I1_3 ] += coefficient * ( -3 * power1( frequencyVector.z ) *
                     power3( phaseVector.x ) - 3 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.y ) - 
                     9 * power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power1( phaseVector.z ) - 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) - 3 * power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) +
                     12 * power1( frequencyVector.z ) * power1( phaseVector.x )*
                     power2( phaseVector.z ) + 4 * power1( frequencyVector.x ) *
                     power3( phaseVector.z ) );

    wedge[ I2_0 ] += coefficient * ( 3 * ( -3 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.x ) -
                     2 * power1( frequencyVector.y ) * 
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.y ) - power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.y ) - 
                     3 * power2( frequencyVector.x ) * power1( point.x ) *
                     power1( point.z ) - power2( frequencyVector.y ) *
                     power1( point.x ) * power1( point.z ) +
                     4 * power2( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) *
                     power1( point.z ) + 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.z ) ) );
    wedge[ I2_1 ] += coefficient * ( 3 * ( -6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) - 2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) - 3 * power2( frequencyVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) - 
                     power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) + 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.y ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 3 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.z ) - 
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.z ) + 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.z ) -
                     2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.z ) + 8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I2_2 ] += coefficient * ( 3 * ( -3 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) -
                     2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) - power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) - 
                     3 * power2( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) - power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) +
                     4 * power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) - 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) + 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) ) );

    wedge[ I3_0 ] += coefficient * ( -9 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) -
                     3 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.x ) +
                     4 * power3( frequencyVector.z ) * power1( point.x ) - 
                     6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.y ) - 3 * power3( frequencyVector.x ) *
                     power1( point.z ) - 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( point.z ) + 
                     12 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( point.z ) );
    wedge[ I3_1 ] += coefficient * ( -9 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) -
                     3 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) +
                     4 * power3( frequencyVector.z ) * power1( phaseVector.x ) -
                     6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) - 3 * power3( frequencyVector.x ) *
                     power1( phaseVector.z ) - 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.z ) + 
                     12 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) );

    wedge[ I4_0 ] += coefficient * ( power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * ( -3 *
                     power2( frequencyVector.x ) -
                     3 * power2( frequencyVector.y ) +
                     4 * power2( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus41(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus51( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.x ) * ( power4( point.x ) +
                     2 * power2( point.x ) * power2( point.y ) +
                     power4( point.y ) - 12 * power2( point.x ) * 
                     power2( point.z ) - 12 * power2( point.y ) * 
                     power2( point.z ) + 8 * power4( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( 5 * power1( phaseVector.x ) *
                     power4( point.x ) + 4 * power1( phaseVector.y ) *
                     power3( point.x ) * power1( point.y ) +
                     6 * power1( phaseVector.x ) * power2( point.x ) *
                     power2( point.y ) + 4 * power1( phaseVector.y ) *
                     power1( point.x ) * power3( point.y ) +
                     power1( phaseVector.x ) * power4( point.y ) -
                     24 * power1( phaseVector.z ) * power3( point.x ) *
                     power1( point.z ) - 24 * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.y ) * power1( point.z ) -
                     36 * power1( phaseVector.x ) * power2( point.x ) *
                     power2( point.z ) - 24 * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) * power2( point.z ) -
                     12 * power1( phaseVector.x ) * power2( point.y ) *
                     power2( point.z ) + 32 * power1( phaseVector.z ) *
                     power1( point.x ) * power3( point.z ) +
                     8 * power1( phaseVector.x ) * power4( point.z ) );
    wedge[ I0_2 ] += coefficient * ( 2 * ( 5 * power2( phaseVector.x ) *
                     power3( point.x ) + power2( phaseVector.y ) *
                     power3( point.x ) - 6 * power2( phaseVector.z ) *
                     power3( point.x ) + 6 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power2( point.x ) *
                     power1( point.y ) + 3 * power2( phaseVector.x ) *
                     power1( point.x ) * power2( point.y ) +
                     3 * power2( phaseVector.y ) * power1( point.x ) *
                     power2( point.y ) - 6 * power2( phaseVector.z ) *
                     power1( point.x ) * power2( point.y ) + 
                     2 * power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power3( point.y ) - 36 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power2( point.x ) *
                     power1( point.z ) - 24 * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.y ) * power1( point.z ) -
                     12 * power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) * power1( point.z ) -
                     18 * power2( phaseVector.x ) * power1( point.x ) *
                     power2( point.z ) - 6 * power2( phaseVector.y ) *
                     power1( point.x ) * power2( point.z ) + 
                     24 * power2( phaseVector.z ) * power1( point.x ) *
                     power2( point.z ) - 12 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power2( point.z ) + 16 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power3( point.z ) ) );
    wedge[ I0_3 ] += coefficient * ( 2 * ( 5 * power3( phaseVector.x ) *
                     power2( point.x ) + 3 * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power2( point.x ) -
                     18 * power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power2( point.x ) + 6 * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) + 2 * power3( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) - 
                     12 * power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) +
                     power3( phaseVector.x ) * power2( point.y ) +
                     3 * power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power2( point.y ) - 6 * power1( phaseVector.x ) *
                     power2( phaseVector.z ) * power2( point.y ) -
                     36 * power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) - 
                     12 * power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) +
                     16 *power3( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) - 24 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 
                     6 * power3( phaseVector.x ) * power2( point.z ) -
                     6 * power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power2( point.z ) + 24 * power1( phaseVector.x ) *
                     power2( phaseVector.z ) * power2( point.z ) ) );
    wedge[ I0_4 ] += coefficient * ( 5 * power4( phaseVector.x ) * 
                     power1( point.x ) + 6 * power2( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) +
                     power4( phaseVector.y ) * power1( point.x ) -
                     36 * power2( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) - 12 * power2( phaseVector.y ) *
                     power2( phaseVector.z ) * power1( point.x ) +
                     8 * power4( phaseVector.z ) * power1( point.x ) +
                     4 * power3( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 4 * power1( phaseVector.x ) *
                     power3( phaseVector.y ) * power1( point.y ) -
                     24 * power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power2( phaseVector.z ) * power1( point.y ) -
                     24 * power3( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 24 * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 32 * power1( phaseVector.x ) * 
                     power3( phaseVector.z ) * power1( point.z ) );
    wedge[ I0_5 ] += coefficient * ( power1( phaseVector.x ) *
                     ( power4( phaseVector.x ) + 2 * power2( phaseVector.x ) *
                     power2( phaseVector.y ) + power4( phaseVector.y ) -
                     12 * power2( phaseVector.x ) * power2( phaseVector.z ) -
                     12 * power2( phaseVector.y ) * power2( phaseVector.z ) +
                     8 * power4( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( 5 * power1( frequencyVector.x ) *
                     power4( point.x ) + 4 * power1( frequencyVector.y ) *
                     power3( point.x ) * power1( point.y ) +
                     6 * power1( frequencyVector.x ) * power2( point.x ) *
                     power2( point.y ) + 4 * power1( frequencyVector.y ) *
                     power1( point.x ) * power3( point.y ) +
                     power1( frequencyVector.x ) * power4( point.y ) -
                     24 * power1( frequencyVector.z ) * power3( point.x ) *
                     power1( point.z ) - 24 * power1( frequencyVector.z ) *
                     power1( point.x ) * power2( point.y ) *
                     power1( point.z ) - 36 * power1( frequencyVector.x ) *
                     power2( point.x ) * power2( point.z ) -
                     24 * power1( frequencyVector.y ) * power1( point.x ) *
                     power1( point.y ) * power2( point.z ) -
                     12 * power1( frequencyVector.x ) * power2( point.y ) *
                     power2( point.z ) + 32 * power1( frequencyVector.z ) *
                     power1( point.x ) * power3( point.z ) +
                     8 * power1( frequencyVector.x ) * power4( point.z ) );
    wedge[ I1_1 ] += coefficient * ( 4 * ( 5 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power3( point.x ) +
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power3( point.x ) - 6 * power1( frequencyVector.z ) *
                     power1( phaseVector.z )*power3( point.x ) +
                     3 * power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power2( point.x ) * power1( point.y ) +
                     3 * power1( frequencyVector.x ) * power1( phaseVector.y ) *
                     power2( point.x ) * power1( point.y ) + 
                     3 * power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( point.x ) * power2( point.y ) +
                     3 * power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.x ) * power2( point.y ) -
                     6 * power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.y ) +
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power3( point.y ) + power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power3( point.y ) - 
                     18 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.x ) *
                     power1( point.z ) - 18 * power1( frequencyVector.x ) *
                     power1( phaseVector.z ) * power2( point.x ) *
                     power1( point.z ) - 12 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) * power1( point.z ) -
                     12 * power1( frequencyVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.y ) * power1( point.z ) -
                     6 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( point.y ) * power1( point.z ) -
                     6 * power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) * power1( point.z ) -
                     18 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power2( point.z ) - 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power2( point.z ) + 24 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power2( point.z ) - 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.y ) *
                     power2( point.z ) - 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power2( point.z ) + 8 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power3( point.z ) +
                     8 * power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power3( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( 6 * ( 5 * power1( frequencyVector.x ) *
                     power2( phaseVector.x ) * power2( point.x ) +
                     2 * power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power2( point.x ) +
                     power1( frequencyVector.x ) * power2( phaseVector.y ) *
                     power2( point.x ) - 12 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.x ) - 6 * power1( frequencyVector.x ) *
                     power2( phaseVector.z ) * power2( point.x ) +
                     2 * power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( point.x ) * power1( point.y ) +
                     4 * power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) + 2 * power1( frequencyVector.y ) *
                     power2( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) - 8 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) -
                     4 * power1( frequencyVector.y ) * power2( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) +
                     power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power2( point.y ) + 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power2( point.y ) + power1( frequencyVector.x ) * 
                     power2( phaseVector.y ) * power2( point.y ) -
                     4 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power2( point.y ) -
                     2 * power1( frequencyVector.x ) * power2( phaseVector.z ) *
                     power2( point.y ) - 12 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( point.x ) *
                     power1( point.z ) - 4 * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) - 24 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) -
                     8 * power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) + 16 * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) - 8 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) * power1( point.z ) -
                     8 * power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.y ) *
                     power1( point.z ) - 8 * power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) -
                     6 * power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power2( point.z ) - 4 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power2( point.z ) - 2 * power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power2( point.z ) +
                     16 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.z ) + 8 * power1( frequencyVector.x ) *
                     power2( phaseVector.z ) * power2( point.z ) ) );

    wedge[ I1_3 ] += coefficient * ( 4 * ( 5 * power1( frequencyVector.x ) *
                     power3( phaseVector.x ) * power1( point.x ) +
                     3 * power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) +
                     3 * power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) +
                     power1( frequencyVector.y ) * power3( phaseVector.y ) *
                     power1( point.x ) - 18 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) - 6 * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) - 18 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) - 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.x ) + 8 * power1( frequencyVector.z ) *
                     power3( phaseVector.z ) * power1( point.x ) +
                     power1( frequencyVector.y ) * power3( phaseVector.x ) *
                     power1( point.y ) + 3 * power1( frequencyVector.x ) *
                     power2( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 3 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power1( point.y ) + power1( frequencyVector.x ) *
                     power3( phaseVector.y ) * power1( point.y ) -
                     12 * power1( frequencyVector.z ) * power1( phaseVector.x )*
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.y ) - 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) - 6 * power1( frequencyVector.z ) *
                     power3( phaseVector.x ) * power1( point.z ) - 
                     6 * power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) -
                     18 * power1( frequencyVector.x ) * power2( phaseVector.x )*
                     power1( phaseVector.z ) * power1( point.z ) -
                     12 * power1( frequencyVector.y ) * power1( phaseVector.x )*
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) - 6 * power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 24 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) + 8 * power1( frequencyVector.x ) *
                     power3( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I1_4 ] += coefficient * ( 5 * power1( frequencyVector.x ) *
                     power4( phaseVector.x ) + 4 * power1( frequencyVector.y ) *
                     power3( phaseVector.x ) * power1( phaseVector.y ) +
                     6 * power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power2( phaseVector.y ) + 4 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power3( phaseVector.y ) +
                     power1( frequencyVector.x ) * power4( phaseVector.y ) -
                     24 * power1( frequencyVector.z ) * power3( phaseVector.x )*
                     power1( phaseVector.z ) - 24 * power1( frequencyVector.z )*
                     power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) - 36 * power1( frequencyVector.x )*
                     power2( phaseVector.x ) * power2( phaseVector.z ) -
                     24 * power1( frequencyVector.y ) * power1( phaseVector.x )*
                     power1( phaseVector.y ) * power2( phaseVector.z ) -
                     12 * power1( frequencyVector.x ) * power2( phaseVector.y )*
                     power2( phaseVector.z ) + 32 * power1( frequencyVector.z )*
                     power1( phaseVector.x ) * power3( phaseVector.z ) +
                     8 * power1( frequencyVector.x ) *
                     power4( phaseVector.z ) );

    wedge[ I2_0 ] += coefficient * ( 2 * ( 5 * power2( frequencyVector.x ) *
                     power3( point.x ) + power2( frequencyVector.y ) *
                     power3( point.x ) - 6 * power2( frequencyVector.z ) *
                     power3( point.x ) + 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( point.x ) *
                     power1( point.y ) + 3 * power2( frequencyVector.x ) *
                     power1( point.x ) * power2( point.y ) +
                     3 * power2( frequencyVector.y ) * power1( point.x ) *
                     power2( point.y ) - 6 * power2( frequencyVector.z ) *
                     power1( point.x ) * power2( point.y ) +
                     2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power3( point.y ) -
                     36 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.x ) *
                     power1( point.z ) - 24 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.y ) * power1( point.z ) -
                     12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.y ) *
                     power1( point.z ) - 18 * power2( frequencyVector.x ) *
                     power1( point.x ) * power2( point.z ) -
                     6 * power2( frequencyVector.y ) * power1( point.x ) *
                     power2( point.z ) + 24 * power2( frequencyVector.z ) *
                     power1( point.x ) * power2( point.z ) -
                     12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) *
                     power2( point.z ) + 16 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power3( point.z ) ) );
    wedge[ I2_1 ] += coefficient * ( 6 * ( 5 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( point.x ) +
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power2( point.x ) - 6 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.x ) +
                     2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power2( point.x ) - 12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power2( point.x ) + 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.y ) +
                     2 * power2( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) +
                     2 * power2( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) -
                     4 * power2( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) - 
                     8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) +
                     power2( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( point.y ) + power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power2( point.y ) - 
                     2 * power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( point.y ) + 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power2( point.y ) - 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power2( point.y ) - 24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) -
                     8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.z ) -
                     12 * power2( frequencyVector.x ) * power1( phaseVector.z )*
                     power1( point.x ) * power1( point.z ) -
                     4 * power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) +
                     16 * power2( frequencyVector.z ) * power1( phaseVector.z )*
                     power1( point.x ) * power1( point.z ) -
                     8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.y ) * power1( point.z ) -
                     8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) * power1( point.z ) -
                     8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) -
                     6 * power2( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( point.z ) - 2 * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power2( point.z ) +
                     8 * power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( point.z ) - 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power2( point.z ) + 16 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I2_2 ] += coefficient * ( 6 * ( 5 * power2( frequencyVector.x ) *
                     power2( phaseVector.x ) * power1( point.x ) +
                     power2( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( point.x ) - 6 * power2( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( point.x ) +
                     4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) +
                     power2( frequencyVector.x ) * power2( phaseVector.y ) *
                     power1( point.x ) + power2( frequencyVector.y ) *
                     power2( phaseVector.y ) * power1( point.x ) -
                     2 * power2( frequencyVector.z ) * power2( phaseVector.y ) *
                     power1( point.x ) - 24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     6 * power2( frequencyVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) - 2 * power2( frequencyVector.y ) *
                     power2( phaseVector.z ) * power1( point.x ) +
                     8 * power2( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.x ) + 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( point.y ) + 2 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 2 * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) - 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( phaseVector.y ) *
                     power1( point.y ) - 8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.y ) -
                     8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) -
                     4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) - 12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( point.z ) - 8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.z ) -
                     4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power1( point.z ) - 12 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 4 * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) + 16 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.z ) +
                     16 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I2_3 ] += coefficient * ( 2 * ( 5 * power2( frequencyVector.x ) *
                     power3( phaseVector.x ) + power2( frequencyVector.y ) *
                     power3( phaseVector.x ) - 6 * power2( frequencyVector.z ) *
                     power3( phaseVector.x ) + 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( phaseVector.y ) + 3 * power2( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( phaseVector.y ) +
                     3 * power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) - 6 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.y ) +
                     2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power3( phaseVector.y ) -
                     36 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( phaseVector.z ) - 24 * power1( frequencyVector.y )*
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) -
                     12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) - 18 * power2( frequencyVector.x )*
                     power1( phaseVector.x ) * power2( phaseVector.z ) -
                     6 * power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power2( phaseVector.z ) + 24 * power2( frequencyVector.z )*
                     power1( phaseVector.x ) * power2( phaseVector.z ) -
                     12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power2( phaseVector.z ) + 16 * power1( frequencyVector.x )*
                     power1( frequencyVector.z ) * power3( phaseVector.z ) ) );

    wedge[ I3_0 ] += coefficient * ( 2 * ( 5 * power3( frequencyVector.x ) *
                     power2( point.x ) + 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( point.x ) -
                     18 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( point.x ) +
                     6 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.x ) *
                     power1( point.y ) + 2 * power3( frequencyVector.y ) *
                     power1( point.x ) * power1( point.y ) -
                     12 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( point.x ) *
                     power1( point.y ) + power3( frequencyVector.x ) *
                     power2( point.y ) + 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( point.y ) -
                     6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( point.y ) - 
                     36 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) - 12 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) + 16 * power3( frequencyVector.z ) *
                     power1( point.x ) * power1( point.z ) -
                     24 * power1( frequencyVector.x ) * 
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.y ) * power1( point.z ) - 
                     6 * power3( frequencyVector.x ) * power2( point.z ) -
                     6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( point.z ) +
                     24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( point.z ) ) );
    wedge[ I3_1 ] += coefficient * ( 4 * ( 5 * power3( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.x ) +
                     3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.x ) - 18 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) + 3 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.x ) + power3( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.x ) -
                     6 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) - 18 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.x ) - 6 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.x ) + 8 * power3( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) +
                     3 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.y ) + power3( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.y ) -
                     6 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.y ) + power3( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) +
                     3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.y ) - 6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) - 12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.y ) -
                     18 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.z ) - 6 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.z ) + 8 * power3( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.z ) -
                     12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.z ) - 6 * power3( frequencyVector.x ) *
                     power1( phaseVector.z ) * power1( point.z ) -
                     6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I3_2 ] += coefficient * ( 2 * ( 5 * power3( frequencyVector.x ) *
                     power2( phaseVector.x ) + 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( phaseVector.x ) -
                     18 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( phaseVector.x ) +
                     6 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) + 2 * power3( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) -
                     12 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) + power3( frequencyVector.x ) *
                     power2( phaseVector.y ) + 3 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( phaseVector.y ) -
                     6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( phaseVector.y ) -
                     36 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) - 12 * power2( frequencyVector.y )*
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) + 16 * power3( frequencyVector.z )*
                     power1( phaseVector.x ) * power1( phaseVector.z ) -
                     24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) -
                     6 * power3( frequencyVector.x ) * power2( phaseVector.z ) -
                     6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power2( phaseVector.z ) +
                     24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power2( phaseVector.z ) ) );

    wedge[ I4_0 ] += coefficient * ( 5 * power4( frequencyVector.x ) *
                     power1( point.x ) + 6 * power2( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( point.x ) +
                     power4( frequencyVector.y ) * power1( point.x ) -
                     36 * power2( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( point.x ) -
                     12 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( point.x ) +
                     8 * power4( frequencyVector.z ) * power1( point.x ) +
                     4 * power3( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) +
                     4 * power1( frequencyVector.x ) *
                     power3( frequencyVector.y ) * power1( point.y ) -
                     24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( frequencyVector.z ) *
                     power1( point.y ) - 24 * power3( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.z ) -
                     24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.z ) + 32 * power1( frequencyVector.x ) *
                     power3( frequencyVector.z ) * power1( point.z ) );
    wedge[ I4_1 ] += coefficient * ( 5 * power4( frequencyVector.x ) *
                     power1( phaseVector.x ) + 6 * power2( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.x ) +
                     power4( frequencyVector.y ) * power1( phaseVector.x ) -
                     36 * power2( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) -
                     12 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) +
                     8 * power4( frequencyVector.z ) * power1( phaseVector.x ) +
                     4 * power3( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) +
                     4 * power1( frequencyVector.x ) *
                     power3( frequencyVector.y ) * power1( phaseVector.y ) -
                     24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power2( frequencyVector.z ) *
                     power1( phaseVector.y ) - 24 * power3( frequencyVector.x )*
                     power1( frequencyVector.z ) * power1( phaseVector.z ) -
                     24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) + 32 * power1( frequencyVector.x )*
                     power3( frequencyVector.z ) * power1( phaseVector.z ) );

    wedge[ I5_0 ] += coefficient * ( power1( frequencyVector.x ) *
                     ( power4( frequencyVector.x ) + 2 *
                     power2( frequencyVector.x ) * power2( frequencyVector.y ) +
                     power4( frequencyVector.y ) - 12 *
                     power2( frequencyVector.x ) * power2( frequencyVector.z ) -
                     12 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) +
                     8 * power4( frequencyVector.z )) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus51(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus10( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * power1( point.z );
    wedge[ I0_1 ] += coefficient * power1( phaseVector.z );
    wedge[ I1_0 ] += coefficient * power1( frequencyVector.z );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus10(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus20( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( 1.0 * ( -1.0 * power2( point.x ) -
                     1.0 * power2( point.y ) + 2.0 * power2( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( 2.0 * ( -1.0 * power1( phaseVector.x ) *
                     power1( point.x ) - 1.0 * power1( phaseVector.y ) *
                     power1( point.y ) + 2.0 * power1( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I0_2 ] += coefficient * ( 1.0 * ( -1.0 * power2( phaseVector.x ) -
                     1.0 * power2( phaseVector.y ) +
                     2.0 * power2( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( 2.0 * ( -1.0 * power1( frequencyVector.x )*
                     power1( point.x ) - 1.0 * power1( frequencyVector.y ) *
                     power1( point.y ) + 2.0 * power1( frequencyVector.z ) *
                     power1( point.z ) ) );
    wedge[ I1_1 ] += coefficient * ( 2.0 * ( -1.0 * power1( frequencyVector.x )*
                     power1( phaseVector.x ) - 1.0 *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) +
                     2.0 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) ) );

    wedge[ I2_0 ] += coefficient * ( 1.0 * ( -1.0 * power2( frequencyVector.x )-
                     1.0 * power2( frequencyVector.y ) +
                     2.0 * power2( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus20(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus30( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.z ) * ( -3 *
                     power2( point.x ) - 3 * power2( point.y ) +
                     2 * power2( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( 3 * ( -( power1( phaseVector.z ) *
                     power2( point.x ) ) - power1( phaseVector.z ) *
                     power2( point.y ) - 2 * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) -
                     2 * power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) + 2 * power1( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I0_2 ] += coefficient * ( 3 * ( -2 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) -
                     2 * power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - power2( phaseVector.x ) *
                     power1( point.z ) - power2( phaseVector.y ) *
                     power1( point.z ) + 2 * power2( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I0_3 ] += coefficient * ( power1( phaseVector.z ) * ( -3 *
                     power2( phaseVector.x ) - 3 * power2( phaseVector.y ) +
                     2 * power2( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( 3 * ( -( power1( frequencyVector.z ) *
                     power2( point.x ) ) - power1( frequencyVector.z ) *
                     power2( point.y ) - 2 * power1( frequencyVector.x ) *
                     power1( point.x ) * power1( point.z ) -
                     2 * power1( frequencyVector.y ) * power1( point.y ) *
                     power1( point.z ) + 2 * power1( frequencyVector.z ) *
                     power2( point.z ) ) );
    wedge[ I1_1 ] += coefficient * ( 6 * ( -( power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) ) -
                     power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) - power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.y ) -
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.z ) -
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.z ) + 2 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( 3 * ( -( power1( frequencyVector.z ) *
                     power2( phaseVector.x ) ) - power1( frequencyVector.z ) *
                     power2( phaseVector.y ) - 2 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) -
                     2 * power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) + 2 * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) ) );

    wedge[ I2_0 ] += coefficient * ( 3 * ( -2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) -
                     2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.y ) -
                     power2( frequencyVector.x ) * power1( point.z ) -
                     power2( frequencyVector.y ) * power1( point.z ) +
                     2 * power2( frequencyVector.z ) * power1( point.z ) ) );
    wedge[ I2_1 ] += coefficient * ( 3 * ( -2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) -
                     2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) -
                     power2( frequencyVector.x ) * power1( phaseVector.z ) -
                     power2( frequencyVector.y ) * power1( phaseVector.z ) +
                     2 * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) ) );

    wedge[ I3_0 ] += coefficient * ( power1( frequencyVector.z ) * ( -3 *
                     power2( frequencyVector.x ) -
                     3 * power2( frequencyVector.y ) + 2 *
                     power2( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus30(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus40( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( 1.5 * ( 1.0 * power4( point.x ) +
                     2.0 * power2( point.x ) * power2( point.y ) +
                     1.0 * power4( point.y ) - 8.0 * power2( point.x ) *
                     power2( point.z ) - 8.0 * power2( point.y ) *
                     power2( point.z ) + 2.666666666666666 *
                     power4( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( 6.0 * ( 1.0 * power1( phaseVector.x ) *
                     power3( point.x ) + 1.0 * power1( phaseVector.y ) *
                     power2( point.x ) * power1( point.y ) +
                     1.0 * power1( phaseVector.x ) * power1( point.x ) *
                     power2( point.y ) + 1.0 * power1( phaseVector.y ) *
                     power3( point.y ) - 4.0 * power1( phaseVector.z ) *
                     power2( point.x ) * power1( point.z ) -
                     4.0 * power1( phaseVector.z ) * power2( point.y ) *
                     power1( point.z ) - 4.0 * power1( phaseVector.x ) *
                     power1( point.x ) * power2( point.z ) -
                     4.0 * power1( phaseVector.y ) * power1( point.y ) *
                     power2( point.z ) + 2.666666666666666 *
                     power1( phaseVector.z ) * power3( point.z ) ) );
    wedge[ I0_2 ] += coefficient * ( 9.0 * ( 1.0 * power2( phaseVector.x ) *
                     power2( point.x ) + 0.3333333333333333 *
                     power2( phaseVector.y ) * power2( point.x ) -
                     1.333333333333333 * power2( phaseVector.z ) *
                     power2( point.x ) + 1.333333333333333 *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) + 
                     0.3333333333333333 * power2( phaseVector.x ) *
                     power2( point.y ) + 1.0 * power2( phaseVector.y ) *
                     power2( point.y ) - 1.333333333333333 *
                     power2( phaseVector.z ) * power2( point.y ) - 
                     5.333333333333333 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) - 5.333333333333333 *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 
                     1.333333333333333 * power2( phaseVector.x ) *
                     power2( point.z ) - 1.333333333333333 *
                     power2( phaseVector.y ) * power2( point.z ) + 
                     2.666666666666666 * power2( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I0_3 ] += coefficient * ( 6.0 * ( 1.0 * power3( phaseVector.x ) *
                     power1( point.x ) + 1.0 * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) -
                     4.0 * power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) + 1.0 * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) +
                     1.0 * power3( phaseVector.y ) * power1( point.y ) - 
                     4.0 * power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) - 4.0 * power2( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.z ) -
                     4.0 * power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 2.666666666666666 *
                     power3( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I0_4 ] += coefficient * ( 1.5 * ( 1.0 * power4( phaseVector.x ) +
                     2.0 * power2( phaseVector.x ) * power2( phaseVector.y ) +
                     1.0 * power4( phaseVector.y ) - 8.0 *
                     power2( phaseVector.x ) * power2( phaseVector.z ) -
                     8.0 * power2( phaseVector.y ) * power2( phaseVector.z ) +
                     2.666666666666666 * power4( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( 6.0 * ( 1.0 * power1( frequencyVector.x ) *
                     power3( point.x ) + 1.0 * power1( frequencyVector.y ) *
                     power2( point.x ) * power1( point.y ) +
                     1.0 * power1( frequencyVector.x ) * power1( point.x ) *
                     power2( point.y ) + 1.0 * power1( frequencyVector.y ) *
                     power3( point.y ) - 4.0 * power1( frequencyVector.z ) *
                     power2( point.x ) * power1( point.z ) - 4.0 *
                     power1( frequencyVector.z ) * power2( point.y ) *
                     power1( point.z ) - 4.0 * power1( frequencyVector.x ) *
                     power1( point.x ) * power2( point.z ) - 4.0 *
                     power1( frequencyVector.y ) * power1( point.y ) *
                     power2( point.z ) + 2.666666666666666 *
                     power1( frequencyVector.z ) * power3( point.z ) ) );
    wedge[ I1_1 ] += coefficient * ( 18.0 * ( 1.0 * power1( frequencyVector.x )*
                     power1( phaseVector.x ) * power2( point.x ) + 
                     0.3333333333333333 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( point.x ) - 
                     1.333333333333333 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.x ) + 
                     0.6666666666666666 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power1( point.y ) + 0.6666666666666666 *
                     power1( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) + 
                     0.3333333333333333 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( point.y ) + 
                     1.0 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( point.y ) - 
                     1.333333333333333 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.y ) - 
                     2.666666666666666 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power1( point.z ) - 2.666666666666666 *
                     power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) - 
                     2.666666666666666 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) - 2.666666666666666 *
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 
                     1.333333333333333 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( point.z ) - 
                     1.333333333333333 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( point.z ) + 
                     2.666666666666666 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( 18.0 * ( 1.0 * power1( frequencyVector.x )*
                     power2( phaseVector.x ) * power1( point.x ) + 
                     0.6666666666666666 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) + 0.3333333333333333 *
                     power1( frequencyVector.x ) * power2( phaseVector.y ) *
                     power1( point.x ) - 2.666666666666666 *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) - 
                     1.333333333333333 * power1( frequencyVector.x ) *
                     power2( phaseVector.z ) * power1( point.x ) + 
                     0.3333333333333333 * power1( frequencyVector.y ) *
                     power2( phaseVector.x ) * power1( point.y ) + 
                     0.6666666666666666 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 1.0 * power1( frequencyVector.y ) *
                     power2( phaseVector.y ) * power1( point.y ) - 
                     2.666666666666666 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 1.333333333333333 *
                     power1( frequencyVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) - 1.333333333333333 *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( point.z ) - 1.333333333333333 *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power1( point.z ) - 2.666666666666666 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.z ) - 
                     2.666666666666666 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 2.666666666666666 *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I1_3 ] += coefficient * ( 6.0 * ( 1.0 * power1( frequencyVector.x ) *
                     power3( phaseVector.x ) + 1.0 *
                     power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( phaseVector.y ) + 1.0 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) + 1.0 *
                     power1( frequencyVector.y ) * power3( phaseVector.y ) -
                     4.0 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( phaseVector.z ) -
                     4.0 * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) - 
                     4.0 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) -
                     4.0 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( phaseVector.z ) + 
                     2.666666666666666 * power1( frequencyVector.z ) *
                     power3( phaseVector.z ) ) );

    wedge[ I2_0 ] += coefficient * ( 9.0 * ( 1.0 * power2( frequencyVector.x ) *
                     power2( point.x ) +  0.3333333333333333 *
                     power2( frequencyVector.y ) * power2( point.x ) - 
                     1.333333333333333 * power2( frequencyVector.z ) *
                     power2( point.x ) + 1.333333333333333 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( point.x ) * power1( point.y ) + 
                     0.3333333333333333 * power2( frequencyVector.x ) *
                     power2( point.y ) + 1.0 * power2( frequencyVector.y ) *
                     power2( point.y ) - 1.333333333333333 *
                     power2( frequencyVector.z ) * power2( point.y ) - 
                     5.333333333333333 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) - 5.333333333333333 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.y ) * power1( point.z ) - 
                     1.333333333333333 * power2( frequencyVector.x ) *
                     power2( point.z ) - 1.333333333333333 *
                     power2( frequencyVector.y ) * power2( point.z ) + 
                     2.666666666666666 * power2( frequencyVector.z ) *
                     power2( point.z ) ) );
    wedge[ I2_1 ] += coefficient * ( 18.0 * ( 1.0 * power2( frequencyVector.x )*
                     power1( phaseVector.x ) * power1( point.x ) + 
                     0.3333333333333333 * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.x ) - 
                     1.333333333333333 * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) + 
                     0.6666666666666666 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.x ) - 2.666666666666666 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) + 
                     0.6666666666666666 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.y ) + 0.3333333333333333 *
                     power2( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( point.y ) + 1.0 * power2( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.y ) - 
                     1.333333333333333 * power2( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.y ) - 
                     2.666666666666666 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.y ) - 2.666666666666666 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.z ) - 
                     2.666666666666666 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.z ) -  1.333333333333333 * 
                     power2( frequencyVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 1.333333333333333 *
                     power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) + 2.666666666666666 *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I2_2 ] += coefficient * ( 9.0 * ( 1.0 * power2( frequencyVector.x ) *
                     power2( phaseVector.x ) + 0.3333333333333333 *
                     power2( frequencyVector.y ) * power2( phaseVector.x ) - 
                     1.333333333333333 * power2( frequencyVector.z ) *
                     power2( phaseVector.x ) + 1.333333333333333 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) + 
                     0.3333333333333333 * power2( frequencyVector.x ) *
                     power2( phaseVector.y ) + 1.0 *
                     power2( frequencyVector.y ) * power2( phaseVector.y ) - 
                     1.333333333333333 * power2( frequencyVector.z ) *
                     power2( phaseVector.y ) - 5.333333333333333 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) - 
                     5.333333333333333 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) - 1.333333333333333 *
                     power2( frequencyVector.x ) * power2( phaseVector.z ) - 
                     1.333333333333333 * power2( frequencyVector.y ) *
                     power2( phaseVector.z ) + 2.666666666666666 *
                     power2( frequencyVector.z ) * power2( phaseVector.z ) ) );

    wedge[ I3_0 ] += coefficient * ( 6.0 * ( 1.0 * power3( frequencyVector.x ) *
                     power1( point.x ) + 1.0 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( point.x ) -
                     4.0 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( point.x ) +
                     1.0 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) +
                     1.0 * power3( frequencyVector.y ) * power1( point.y ) -
                     4.0 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( point.y ) - 
                     4.0 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.z ) -
                     4.0 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.z ) + 
                     2.666666666666666 * power3( frequencyVector.z ) *
                     power1( point.z ) ) );
    wedge[ I3_1 ] += coefficient * ( 6.0 * ( 1.0 * power3( frequencyVector.x ) *
                     power1( phaseVector.x ) + 1.0 *
                     power1( frequencyVector.x ) * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) - 4.0 *
                     power1( frequencyVector.x ) * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) + 1.0 *
                     power2( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) + 1.0 *
                     power3( frequencyVector.y ) * power1( phaseVector.y ) -
                     4.0 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.y ) - 
                     4.0 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) -
                     4.0 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) + 
                     2.666666666666666 * power3( frequencyVector.z ) *
                     power1( phaseVector.z ) ) );

    wedge[ I4_0 ] += coefficient * ( 1.5 * ( 1.0 * power4( frequencyVector.x ) +
                     2.0 * power2( frequencyVector.x ) *
                     power2( frequencyVector.y ) + 1.0 *
                     power4( frequencyVector.y ) - 8.0 *
                     power2( frequencyVector.x ) * power2( frequencyVector.z ) -
                     8.0 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) + 2.666666666666666 *
                     power4( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus40(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}


template < class T >
void gkg::GradientCoil< T >::getHarmonicCosinus50( 
                                  std::vector< float >& wedge,
                                  const gkg::Vector3d< T >& phaseVector,
                                  const gkg::Vector3d< T >& frequencyVector,
                                  const gkg::Vector3d< T >& point,
                                  const T& coefficient ) const
{

  try
  {

    if ( wedge.size() != GRADIENT_COIL_WEDGE_COUNT )
    {

      throw std::runtime_error( "bad wedge size" );

    }

    wedge[ I0_0 ] += coefficient * ( power1( point.z ) * ( 15 *
                     power4( point.x ) + 30 * power2( point.x ) *
                     power2( point.y ) + 15 * power4( point.y ) -
                     40 * power2( point.x ) * power2( point.z ) -
                     40 * power2( point.y ) * power2( point.z ) +
                     8 * power4( point.z ) ) );
    wedge[ I0_1 ] += coefficient * ( 5 * ( 3 * power1( phaseVector.z ) *
                     power4( point.x ) + 6 * power1( phaseVector.z ) *
                     power2( point.x ) * power2( point.y ) + 3 * 
                     power1( phaseVector.z ) * power4( point.y ) +
                     12 * power1( phaseVector.x ) * power3( point.x ) *
                     power1( point.z ) + 12 * power1( phaseVector.y ) *
                     power2( point.x ) * power1( point.y ) * power1( point.z ) +
                     12 * power1( phaseVector.x ) * power1( point.x ) *
                     power2( point.y ) * power1( point.z ) + 12 *
                     power1( phaseVector.y ) * power3( point.y ) *
                     power1( point.z ) - 24 * power1( phaseVector.z ) *
                     power2( point.x ) * power2( point.z ) -
                     24 * power1( phaseVector.z ) * power2( point.y ) *
                     power2( point.z ) - 16 * power1( phaseVector.x ) *
                     power1( point.x ) * power3( point.z ) -
                     16 * power1( phaseVector.y ) * power1( point.y ) *
                     power3( point.z ) + 8 * power1( phaseVector.z ) *
                     power4( point.z ) ) );
    wedge[ I0_2 ] += coefficient * ( 10 * ( 6 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power3( point.x ) +
                     6 * power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power2( point.x ) * power1( point.y ) + 6 *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.y ) +
                     6 * power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power3( point.y ) + 9 * power2( phaseVector.x ) *
                     power2( point.x ) * power1( point.z ) + 3 *
                     power2( phaseVector.y ) * power2( point.x ) *
                     power1( point.z ) - 12 * power2( phaseVector.z ) *
                     power2( point.x ) * power1( point.z ) + 12 *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) * power1( point.z ) +
                     3 * power2( phaseVector.x ) * power2( point.y ) *
                     power1( point.z ) + 9 * power2( phaseVector.y ) *
                     power2( point.y ) * power1( point.z ) - 12 *
                     power2( phaseVector.z ) * power2( point.y ) *
                     power1( point.z ) - 24 * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power2( point.z ) - 24 * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) *
                     power2( point.z ) - 4 * power2( phaseVector.x ) *
                     power3( point.z ) - 4 * power2( phaseVector.y ) *
                     power3( point.z ) + 8 * power2( phaseVector.z ) *
                     power3( point.z ) ) );
    wedge[ I0_3 ] += coefficient * ( 10 * ( 9 * power2( phaseVector.x ) *
                     power1( phaseVector.z ) * power2( point.x ) + 3 *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power2( point.x ) - 4 * power3( phaseVector.z ) *
                     power2( point.x ) + 12 * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) + 3 *
                     power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) + 9 * power2( phaseVector.y ) *
                     power1( phaseVector.z ) * power2( point.y ) - 4 *
                     power3( phaseVector.z ) * power2( point.y ) + 6 *
                     power3( phaseVector.x ) * power1( point.x ) *
                     power1( point.z ) + 6 * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) - 24 * power1( phaseVector.x ) *
                     power2( phaseVector.z ) * power1( point.x ) *
                     power1( point.z ) + 6 * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) + 6 * power3( phaseVector.y ) *
                     power1( point.y ) * power1( point.z ) - 24 *
                     power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 12 *
                     power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.z ) - 12 * power2( phaseVector.y ) *
                     power1( phaseVector.z ) * power2( point.z ) + 8 *
                     power3( phaseVector.z ) * power2( point.z ) ) );
    wedge[ I0_4 ] += coefficient * ( 5 * ( 12 * power3( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) + 12 *
                     power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) - 16 *
                     power1( phaseVector.x ) * power3( phaseVector.z ) *
                     power1( point.x ) + 12 * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) + 12 * power3( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) - 16 *
                     power1( phaseVector.y ) * power3( phaseVector.z ) *
                     power1( point.y ) + 3 * power4( phaseVector.x ) *
                     power1( point.z ) + 6 * power2( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) + 3 *
                     power4( phaseVector.y ) * power1( point.z ) - 24 *
                     power2( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) - 24 * power2( phaseVector.y ) *
                     power2( phaseVector.z ) * power1( point.z ) + 8 *
                     power4( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I0_5 ] += coefficient * ( power1( phaseVector.z ) * ( 15 *
                     power4( phaseVector.x ) + 30 * power2( phaseVector.x ) *
                     power2( phaseVector.y ) + 15 * power4( phaseVector.y ) -
                     40 * power2( phaseVector.x ) * power2( phaseVector.z ) -
                     40 * power2( phaseVector.y ) * power2( phaseVector.z ) +
                     8 * power4( phaseVector.z ) ) );

    wedge[ I1_0 ] += coefficient * ( 5 * ( 3 * power1( frequencyVector.z ) *
                     power4( point.x ) + 6 * power1( frequencyVector.z ) *
                     power2( point.x ) * power2( point.y ) + 3 *
                     power1( frequencyVector.z ) * power4( point.y ) +
                     12 * power1( frequencyVector.x ) * power3( point.x ) *
                     power1( point.z ) + 12 * power1( frequencyVector.y ) *
                     power2( point.x ) * power1( point.y ) * power1( point.z ) +
                     12 * power1( frequencyVector.x ) * power1( point.x ) *
                     power2( point.y ) * power1( point.z ) + 12 *
                     power1( frequencyVector.y ) * power3( point.y ) *
                     power1( point.z ) - 24 * power1( frequencyVector.z ) *
                     power2( point.x ) * power2( point.z ) -
                     24 * power1( frequencyVector.z ) * power2( point.y ) *
                     power2( point.z ) - 16 * power1( frequencyVector.x ) *
                     power1( point.x ) * power3( point.z ) -
                     16 * power1( frequencyVector.y ) * power1( point.y ) *
                     power3( point.z ) + 8 * power1( frequencyVector.z ) *
                     power4( point.z ) ) );
    wedge[ I1_1 ] += coefficient * ( 20 * ( 3 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power3( point.x ) + 3 *
                     power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power3( point.x ) + 3 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power2( point.x ) *
                     power1( point.y ) + 3 * power1( frequencyVector.y ) *
                     power1( phaseVector.z ) * power2( point.x ) *
                     power1( point.y ) + 3 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power2( point.y ) + 3 * power1( frequencyVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power2( point.y ) + 3 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power3( point.y ) + 3 *
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power3( point.y ) + 9 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( point.x ) *
                     power1( point.z ) + 3 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( point.x ) *
                     power1( point.z ) - 12 * power1( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.x ) *
                     power1( point.z ) + 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power1( point.y ) * power1( point.z ) + 6 *
                     power1( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) * power1( point.z ) +
                     3 * power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( point.y ) * power1( point.z ) + 9 *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power2( point.y ) * power1( point.z ) - 12 *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power2( point.y ) * power1( point.z ) - 12 *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) * power2( point.z ) - 12 *
                     power1( frequencyVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power2( point.z ) - 12 *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) * power2( point.z ) - 12 *
                     power1( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power2( point.z ) - 4 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power3( point.z ) - 4 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power3( point.z ) + 8 *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power3( point.z ) ) );
    wedge[ I1_2 ] += coefficient * ( 30 * ( 3 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power2( point.x ) +
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power2( point.x ) + 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.x ) + 2 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power2( point.x ) - 4 * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) * power2( point.x ) + 4 *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.y ) + 4 * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.y ) + 4 *
                     power1( frequencyVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.y ) + power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power2( point.y ) + 3 *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) *
                     power2( point.y ) + 2 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power2( point.y ) + 6 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power2( point.y ) - 4 * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) * power2( point.y ) + 6 *
                     power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) + 4 *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) + 2 * power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) - 16 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) - 8 *
                     power1( frequencyVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) + 2 *
                     power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( point.y ) * power1( point.z ) + 4 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) + 6 * power1( frequencyVector.y ) *
                     power2( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) - 16 * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 8 *
                     power1( frequencyVector.y ) * power2( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 4 *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power2( point.z ) - 4 * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power2( point.z ) - 8 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power2( point.z ) - 8 *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power2( point.z ) + 8 *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I1_3 ] += coefficient * ( 20 * ( 3 * power1( frequencyVector.z ) *
                     power3( phaseVector.x ) * power1( point.x ) + 3 *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.x ) + 9 *
                     power1( frequencyVector.x ) * power2( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) + 6 *
                     power1( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) + 3 * power1( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) - 12 * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.x ) - 4 * power1( frequencyVector.x ) *
                     power3( phaseVector.z ) * power1( point.x ) + 3 *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) + 3 *
                     power1( frequencyVector.z ) * power3( phaseVector.y ) *
                     power1( point.y ) + 3 * power1( frequencyVector.y ) *
                     power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.y ) + 6 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) + 9 *
                     power1( frequencyVector.y ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) - 12 *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power2( phaseVector.z ) * power1( point.y ) - 4 *
                     power1( frequencyVector.y ) * power3( phaseVector.z ) *
                     power1( point.y ) + 3 * power1( frequencyVector.x ) *
                     power3( phaseVector.x ) * power1( point.z ) + 3 *
                     power1( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.z ) + 3 *
                     power1( frequencyVector.x ) * power1( phaseVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) + 3 *
                     power1( frequencyVector.y ) * power3( phaseVector.y ) *
                     power1( point.z ) - 12 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 12 * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.z ) - 12 * power1( frequencyVector.x ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) - 12 * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power2( phaseVector.z ) *
                     power1( point.z ) + 8 * power1( frequencyVector.z ) *
                     power3( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I1_4 ] += coefficient * ( 5 * ( 3 * power1( frequencyVector.z ) *
                     power4( phaseVector.x ) + 6 * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power2( phaseVector.y ) + 3 *
                     power1( frequencyVector.z ) * power4( phaseVector.y ) +
                     12 * power1( frequencyVector.x ) * power3( phaseVector.x )*
                     power1( phaseVector.z ) + 12 * power1( frequencyVector.y )*
                     power2( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) + 12 * power1( frequencyVector.x )*
                     power1( phaseVector.x ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) + 12 * power1( frequencyVector.y )*
                     power3( phaseVector.y ) * power1( phaseVector.z ) - 24 *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power2( phaseVector.z ) - 24 * power1( frequencyVector.z )*
                     power2( phaseVector.y ) * power2( phaseVector.z ) - 
                     16 * power1( frequencyVector.x ) * power1( phaseVector.x )*
                     power3( phaseVector.z ) - 16 * power1( frequencyVector.y )*
                     power1( phaseVector.y ) * power3( phaseVector.z ) + 8 *
                     power1( frequencyVector.z ) * power4( phaseVector.z ) ) );

    wedge[ I2_0 ] += coefficient * ( 10 * ( 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power3( point.x ) + 6 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power2( point.x ) * power1( point.y ) + 6 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( point.x ) * power2( point.y ) + 6 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power3( point.y ) + 9 * power2( frequencyVector.x ) *
                     power2( point.x ) * power1( point.z ) + 3 *
                     power2( frequencyVector.y ) * power2( point.x ) *
                     power1( point.z ) - 12 * power2( frequencyVector.z ) *
                     power2( point.x ) * power1( point.z ) + 12 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( point.x ) * power1( point.y ) * power1( point.z ) +
                     3 * power2( frequencyVector.x ) * power2( point.y ) *
                     power1( point.z ) + 9 * power2( frequencyVector.y ) *
                     power2( point.y ) * power1( point.z ) - 12 *
                     power2( frequencyVector.z ) * power2( point.y ) *
                     power1( point.z ) - 24 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) *
                     power2( point.z ) - 24 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.y ) *
                     power2( point.z ) - 4 * power2( frequencyVector.x ) *
                     power3( point.z ) - 4 * power2( frequencyVector.y ) *
                     power3( point.z ) + 8 * power2( frequencyVector.z ) *
                     power3( point.z ) ) );
    wedge[ I2_1 ] += coefficient * ( 30 * ( 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( point.x ) + 2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power2( point.x ) + 3 * power2( frequencyVector.x ) *
                     power1( phaseVector.z )*power2( point.x ) +
                     power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power2( point.x ) - 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.x ) + 4 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) *
                     power1( point.y ) + 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) * power1( point.y ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) *
                     power1( point.y ) + 2 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power2( point.y ) + 6 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power2( point.y ) + power2( frequencyVector.x ) *
                     power1( phaseVector.z ) * power2( point.y ) + 3 *
                     power2( frequencyVector.y ) * power1( phaseVector.z ) *
                     power2( point.y ) - 4 * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) * power2( point.y ) + 6 *
                     power2( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) + 2 *
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) - 8 *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) * power1( point.z ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.x ) *
                     power1( point.z ) - 16 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.x ) * power1( point.z ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.y ) *
                     power1( point.z ) + 2 * power2( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) + 6 * power2( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) - 8 * power2( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.y ) *
                     power1( point.z ) - 16 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.y ) * power1( point.z ) - 8 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( point.z ) - 8 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power2( point.z ) - 4 *
                     power2( frequencyVector.x ) * power1( phaseVector.z ) *
                     power2( point.z ) - 4 * power2( frequencyVector.y ) *
                     power1( phaseVector.z ) * power2( point.z ) + 8 *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) *
                     power2( point.z ) ) );
    wedge[ I2_2 ] += coefficient * ( 30 * ( 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( point.x ) + 4 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.x ) + 2 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( point.x ) + 6 *
                     power2( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) + 2 *
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) - 8 *
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.x ) - 8 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.x ) + 2 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) *
                     power1( point.y ) + 4 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.y ) * power1( point.y ) + 6 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( point.y ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.y ) + 2 * power2( frequencyVector.x ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) + 6 * power2( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 8 * power2( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) -  8 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.y ) + 3 * power2( frequencyVector.x ) *
                     power2( phaseVector.x ) * power1( point.z ) +
                     power2( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( point.z ) - 4 * power2( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( point.z ) + 4 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( point.z ) + power2( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( point.z ) + 3 *
                     power2( frequencyVector.y ) * power2( phaseVector.y ) *
                     power1( point.z ) - 4 * power2( frequencyVector.z ) *
                     power2( phaseVector.y ) * power1( point.z ) - 16 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.z ) *
                     power1( point.z ) - 16 * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) * power1( point.z ) - 4 *
                     power2( frequencyVector.x ) * power2( phaseVector.z ) *
                     power1( point.z ) - 4 * power2( frequencyVector.y ) *
                     power2( phaseVector.z ) * power1( point.z ) + 8 *
                     power2( frequencyVector.z ) * power2( phaseVector.z ) *
                     power1( point.z ) ) );
    wedge[ I2_3 ] += coefficient * ( 10 * ( 6 * power1( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power3( phaseVector.x ) + 6 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) * power1( phaseVector.y ) + 6 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.y ) + 6 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power3( phaseVector.y ) + 9 * power2( frequencyVector.x ) *
                     power2( phaseVector.x ) * power1( phaseVector.z ) + 3 *
                     power2( frequencyVector.y ) * power2( phaseVector.x ) *
                     power1( phaseVector.z ) - 12 * power2( frequencyVector.z )*
                     power2( phaseVector.x ) * power1( phaseVector.z ) + 12 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) + 3 * power2( frequencyVector.x ) *
                     power2( phaseVector.y ) * power1( phaseVector.z ) + 9 *
                     power2( frequencyVector.y ) * power2( phaseVector.y ) *
                     power1( phaseVector.z ) - 12 * power2( frequencyVector.z )*
                     power2( phaseVector.y ) * power1( phaseVector.z ) - 24 *
                     power1( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power2( phaseVector.z ) - 24 *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) * power2( phaseVector.z ) - 4 *
                     power2( frequencyVector.x ) * power3( phaseVector.z ) -
                     4 * power2( frequencyVector.y ) * power3( phaseVector.z ) +
                     8 * power2( frequencyVector.z ) *
                     power3( phaseVector.z ) ) );

    wedge[ I3_0 ] += coefficient * ( 10 * ( 9 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( point.x ) + 3 *
                     power2( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power2( point.x ) - 4 * power3( frequencyVector.z ) *
                     power2( point.x ) + 12 * power1( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.x ) * power1( point.y ) + 3 *
                     power2( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power2( point.y ) + 9 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power2( point.y ) - 4 *
                     power3( frequencyVector.z ) * power2( point.y ) + 6 *
                     power3( frequencyVector.x ) * power1( point.x ) *
                     power1( point.z ) + 6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( point.x ) *
                     power1( point.z ) - 24 * power1( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( point.x ) *
                     power1( point.z ) + 6 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( point.y ) *
                     power1( point.z ) + 6 * power3( frequencyVector.y ) *
                     power1( point.y ) * power1( point.z ) - 24 *
                     power1( frequencyVector.y ) * power2( frequencyVector.z ) *
                     power1( point.y ) * power1( point.z ) - 12 *
                     power2( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power2( point.z ) - 12 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power2( point.z ) + 8 *
                     power3( frequencyVector.z ) * power2( point.z ) ) );
    wedge[ I3_1 ] += coefficient * ( 20 * ( 9 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) + 3 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.x ) - 4 * power3( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.x ) + 6 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.x ) + 3 * power3( frequencyVector.x ) *
                     power1( phaseVector.z ) * power1( point.x ) + 3 *
                     power1( frequencyVector.x ) * power2( frequencyVector.y ) *
                     power1( phaseVector.z ) * power1( point.x ) - 12 *
                     power1( frequencyVector.x ) * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.x ) + 6 *
                     power1( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( point.y ) + 3 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) + 9 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.y ) - 4 * power3( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( point.y ) + 3 *
                     power2( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.z ) * power1( point.y ) + 3 *
                     power3( frequencyVector.y ) * power1( phaseVector.z ) *
                     power1( point.y ) - 12 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.y ) + 3 * power3( frequencyVector.x ) *
                     power1( phaseVector.x ) * power1( point.z ) + 3 *
                     power1( frequencyVector.x ) * power2( frequencyVector.y ) *
                     power1( phaseVector.x ) * power1( point.z ) - 12 *
                     power1( frequencyVector.x ) * power2( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( point.z ) + 3 *
                     power2( frequencyVector.x ) * power1( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( point.z ) + 3 *
                     power3( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( point.z ) - 12 * power1( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( phaseVector.y ) *
                     power1( point.z ) - 12 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.z ) - 12 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.z ) *
                     power1( point.z ) + 8 * power3( frequencyVector.z ) *
                     power1( phaseVector.z ) * power1( point.z ) ) );
    wedge[ I3_2 ] += coefficient * ( 10 * ( 9 * power2( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power2( phaseVector.x ) + 3 *
                     power2( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power2( phaseVector.x ) - 4 * power3( frequencyVector.z ) *
                     power2( phaseVector.x ) + 12 * power1( frequencyVector.x )*
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.x ) * power1( phaseVector.y ) + 3 *
                     power2( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power2( phaseVector.y ) + 9 * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power2( phaseVector.y ) - 4 *
                     power3( frequencyVector.z ) * power2( phaseVector.y ) + 6 *
                     power3( frequencyVector.x ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) + 6 * power1( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) - 24 * power1( frequencyVector.x )*
                     power2( frequencyVector.z ) * power1( phaseVector.x ) *
                     power1( phaseVector.z ) + 6 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( phaseVector.y ) *
                     power1( phaseVector.z ) + 6 * power3( frequencyVector.y ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) - 24 *
                     power1( frequencyVector.y ) * power2( frequencyVector.z ) *
                     power1( phaseVector.y ) * power1( phaseVector.z ) - 12 *
                     power2( frequencyVector.x ) * power1( frequencyVector.z ) *
                     power2( phaseVector.z ) - 12 * power2( frequencyVector.y )*
                     power1( frequencyVector.z ) * power2( phaseVector.z ) + 8 *
                     power3( frequencyVector.z ) * power2( phaseVector.z ) ) );

    wedge[ I4_0 ] += coefficient * ( 5 * ( 12 * power3( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( point.x ) + 12 *
                     power1( frequencyVector.x ) * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.x ) - 16 *
                     power1( frequencyVector.x ) * power3( frequencyVector.z ) *
                     power1( point.x ) + 12 * power2( frequencyVector.x ) *
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( point.y ) + 12 * power3( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( point.y ) - 16 *
                     power1( frequencyVector.y ) * power3( frequencyVector.z ) *
                     power1( point.y ) + 3 * power4( frequencyVector.x ) *
                     power1( point.z ) + 6 * power2( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( point.z ) + 3 *
                     power4( frequencyVector.y ) * power1( point.z ) - 24 *
                     power2( frequencyVector.x ) * power2( frequencyVector.z ) *
                     power1( point.z ) - 24 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) * power1( point.z ) + 8 *
                     power4( frequencyVector.z ) * power1( point.z ) ) );
    wedge[ I4_1 ] += coefficient * ( 5 * ( 12 * power3( frequencyVector.x ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) + 12*
                     power1( frequencyVector.x ) * power2( frequencyVector.y ) *
                     power1( frequencyVector.z ) * power1( phaseVector.x ) - 16*
                     power1( frequencyVector.x ) * power3( frequencyVector.z ) *
                     power1( phaseVector.x ) + 12 * power2( frequencyVector.x )*
                     power1( frequencyVector.y ) * power1( frequencyVector.z ) *
                     power1( phaseVector.y ) + 12 * power3( frequencyVector.y )*
                     power1( frequencyVector.z ) * power1( phaseVector.y ) - 16*
                     power1( frequencyVector.y ) * power3( frequencyVector.z ) *
                     power1( phaseVector.y ) + 3 * power4( frequencyVector.x ) *
                     power1( phaseVector.z ) + 6 * power2( frequencyVector.x ) *
                     power2( frequencyVector.y ) * power1( phaseVector.z ) + 3 *
                     power4( frequencyVector.y ) * power1( phaseVector.z ) - 
                     24 * power2( frequencyVector.x ) *
                     power2( frequencyVector.z ) * power1( phaseVector.z ) - 24*
                     power2( frequencyVector.y ) * power2( frequencyVector.z ) *
                     power1( phaseVector.z ) + 8 * power4( frequencyVector.z ) *
                     power1( phaseVector.z ) ) );

    wedge[ I5_0 ] += coefficient * ( power1( frequencyVector.z ) * ( 15 *
                     power4( frequencyVector.x ) + 30 *
                     power2( frequencyVector.x ) * power2( frequencyVector.y ) +
                     15 * power4( frequencyVector.y ) - 40 *
                     power2( frequencyVector.x ) * power2( frequencyVector.z ) -
                     40 * power2( frequencyVector.y ) *
                     power2( frequencyVector.z ) + 8 *
                     power4( frequencyVector.z ) ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientCoil< T >::getHarmonicCosinus50(  "
             "std::vector< float >& wedge, "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "const T& coefficient ) const" );

}



//
// explicit instanciation(s)
//

template class gkg::GradientCoil< float >;
template class gkg::GradientCoil< double >;


#undef power1
#undef power2
#undef power3
#undef power4
#undef power5

#undef I0_0
#undef I0_1
#undef I0_2
#undef I0_3
#undef I0_4
#undef I0_5
#undef I1_0
#undef I1_1
#undef I1_2
#undef I1_3
#undef I1_4
#undef I2_0
#undef I2_1
#undef I2_2
#undef I2_3
#undef I3_0
#undef I3_1
#undef I3_2
#undef I4_0
#undef I4_1
#undef I5_0
