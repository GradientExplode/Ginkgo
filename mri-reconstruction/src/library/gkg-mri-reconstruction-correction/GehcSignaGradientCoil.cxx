#include <gkg-mri-reconstruction-correction/GehcSignaGradientCoil.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>

//
// useful macro
//
#define READ_VALUE( which, value )                     \
    is >> label;                                       \
    if ( label != which )                              \
    {                                                  \
                                                       \
      throw std::runtime_error( "wrong file format" ); \
                                                       \
    }                                                  \
    else                                               \
    {                                                  \
                                                       \
      is >> value;                                     \
                                                       \
    }                                                  \
                                                       \
    if ( is.eof() )                                    \
    {                                                  \
                                                       \
      is.close();                                      \
      throw std::runtime_error( "wrong file format" ); \
                                                       \
    }


//
// useful inline function
//
template < class T >
inline gkg::Vector3d< T > swapXY( const gkg::Vector3d< T >& v )
{

  return gkg::Vector3d< T >( v.y, v.x, v.z );

}


//
// class GehcSignaGradientCoil< T >
//
template < class T >
gkg::GehcSignaGradientCoil< T >::GehcSignaGradientCoil(
                                                   const std::string& fileName )
                                : gkg::GradientCoil< T >()
{

  try
  {

    std::ifstream is( fileName.c_str() );

    std::string label;
    is >> label;
    while ( !is.eof() && ( label != "GRADWARPTYPE" ) )
    {

      is >> label;

    }
    int32_t type;
    is >> type;

    switch ( type )
    {

      case 1 :

        _type = gkg::GehcSignaGradientCoil< T >::BRM_CRM;
        break;

      default:

        is.close();
        throw std::runtime_error( "not a managed GEHC gradient coil" );

    }


    READ_VALUE( "SCALEX1", _scaleX[ 0 ] );
    READ_VALUE( "SCALEX2", _scaleX[ 1 ] );
    READ_VALUE( "SCALEX3", _scaleX[ 2 ] );
    READ_VALUE( "SCALEX4", _scaleX[ 3 ] );
    READ_VALUE( "SCALEX5", _scaleX[ 4 ] );
    READ_VALUE( "SCALEX6", _scaleX[ 5 ] );
    READ_VALUE( "SCALEX7", _scaleX[ 6 ] );
    READ_VALUE( "SCALEX8", _scaleX[ 7 ] );
    READ_VALUE( "SCALEX9", _scaleX[ 8 ] );
    READ_VALUE( "SCALEX10", _scaleX[ 9 ] );

    READ_VALUE( "SCALEY1", _scaleY[ 0 ] );
    READ_VALUE( "SCALEY2", _scaleY[ 1 ] );
    READ_VALUE( "SCALEY3", _scaleY[ 2 ] );
    READ_VALUE( "SCALEY4", _scaleY[ 3 ] );
    READ_VALUE( "SCALEY5", _scaleY[ 4 ] );
    READ_VALUE( "SCALEY6", _scaleY[ 5 ] );
    READ_VALUE( "SCALEY7", _scaleY[ 6 ] );
    READ_VALUE( "SCALEY8", _scaleY[ 7 ] );
    READ_VALUE( "SCALEY9", _scaleY[ 8 ] );
    READ_VALUE( "SCALEY10", _scaleY[ 9 ] );

    READ_VALUE( "SCALEZ1", _scaleZ[ 0 ] );
    READ_VALUE( "SCALEZ2", _scaleZ[ 1 ] );
    READ_VALUE( "SCALEZ3", _scaleZ[ 2 ] );
    READ_VALUE( "SCALEZ4", _scaleZ[ 3 ] );
    READ_VALUE( "SCALEZ5", _scaleZ[ 4 ] );
    READ_VALUE( "SCALEZ6", _scaleZ[ 5 ] );
    READ_VALUE( "SCALEZ7", _scaleZ[ 6 ] );
    READ_VALUE( "SCALEZ8", _scaleZ[ 7 ] );
    READ_VALUE( "SCALEZ9", _scaleZ[ 8 ] );
    READ_VALUE( "SCALEZ10", _scaleZ[ 9 ] );

    READ_VALUE( "DELTA", _delta );

    is.close();

  }
  GKG_CATCH( "template < class T > "
             "gkg::GehcSignaGradientCoil< T >::GehcSignaGradientCoil( "
             "const std::string& fileName )" );

}


template < class T >
gkg::GehcSignaGradientCoil< T >::~GehcSignaGradientCoil()
{
}


template < class T >
typename gkg::GehcSignaGradientCoil< T >::Type 
gkg::GehcSignaGradientCoil< T >::getType() const
{

  return _type;

}


template < class T >
const T& gkg::GehcSignaGradientCoil< T >::getScaleX( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= 10 ) )
    {

      throw std::runtime_error( "wrong index" );

    }
    return _scaleX[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::GehcSignaGradientCoil< T >::getScaleX( "
             "int index ) const" );

}


template < class T >
const T& gkg::GehcSignaGradientCoil< T >::getScaleY( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= 10 ) )
    {

      throw std::runtime_error( "wrong index" );

    }
    return _scaleY[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::GehcSignaGradientCoil< T >::getScaleY( "
             "int index ) const" );

}


template < class T >
const T& gkg::GehcSignaGradientCoil< T >::getScaleZ( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= 10 ) )
    {

      throw std::runtime_error( "wrong index" );

    }
    return _scaleZ[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::GehcSignaGradientCoil< T >::getScaleZ( "
             "int index ) const" );

}


template < class T >
const T& gkg::GehcSignaGradientCoil< T >::getDelta() const
{

  return _delta;

}


template < class T >
void gkg::GehcSignaGradientCoil< T >::getXYZWedges(
                                      const gkg::Vector3d< T >& phaseVector,
                                      const gkg::Vector3d< T >& frequencyVector,
                                      const gkg::Vector3d< T >& point,
                                      std::vector< float >& wedgeX,
                                      std::vector< float >& wedgeY,
                                      std::vector< float >& wedgeZ ) const
{

  try
  {

    switch ( _type )
    {

      case gkg::GehcSignaGradientCoil< T >::BRM_CRM:

        getBrmCrmWedges( phaseVector, frequencyVector, point,
                         wedgeX, wedgeY, wedgeZ );
        break;

      default:

        throw std::runtime_error( "not managed" );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGradientCoil< T >::getXYZWedges( "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeX, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeY, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeZ ) const" );

}


template < class T >
void gkg::GehcSignaGradientCoil< T >::getBrmCrmWedges(
                                      const gkg::Vector3d< T >& phaseVector,
                                      const gkg::Vector3d< T >& frequencyVector,
                                      const gkg::Vector3d< T >& point,
                                      std::vector< float >& wedgeX,
                                      std::vector< float >& wedgeY,
                                      std::vector< float >& wedgeZ ) const
{


  try
  {

    // 
    // wedge initialization
    //
    wedgeX = std::vector< float >( GRADIENT_COIL_WEDGE_COUNT, 0.0f );
    wedgeY = std::vector< float >( GRADIENT_COIL_WEDGE_COUNT, 0.0f );
    wedgeZ = std::vector< float >( GRADIENT_COIL_WEDGE_COUNT, 0.0f );

    //
    // processing X wedges
    //
    this->getHarmonicCosinus11( wedgeX,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleX[ 0 ] );
    this->getHarmonicCosinus31( wedgeX,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleX[ 2 ] );
    this->getHarmonicCosinus51( wedgeX,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleX[ 4 ] );


    //
    // processing Y wedges
    //
    this->getHarmonicCosinus11( wedgeY,
                                swapXY( phaseVector ),
                                swapXY( frequencyVector ),
                                swapXY( point ),
                                _scaleY[ 0 ] );
    this->getHarmonicCosinus31( wedgeY,
                                swapXY( phaseVector ),
                                swapXY( frequencyVector ),
                                swapXY( point ),
                                _scaleY[ 2 ] );
    this->getHarmonicCosinus51( wedgeY,
                                swapXY( phaseVector ),
                                swapXY( frequencyVector ),
                                swapXY( point ),
                                _scaleY[ 4 ] );


    //
    // processing Z wedges
    //
    this->getHarmonicCosinus10( wedgeZ,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleZ[ 0 ] );
    this->getHarmonicCosinus30( wedgeZ,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleZ[ 2 ] );
    this->getHarmonicCosinus50( wedgeZ,
                                phaseVector,
                                frequencyVector,
                                point,
                                _scaleZ[ 4 ] );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGradientCoil< T >::getCrmBrmWedges( "
             "const gkg::Vector3d< T >& phaseVector, "
             "const gkg::Vector3d< T >& frequencyVector, "
             "const gkg::Vector3d< T >& point, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeX, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeY, "
             "typename gkg::GradientCoil< T >::Wedge& wedgeZ ) const" );

}


//
// explicit instanciation(s)
//

template class gkg::GehcSignaGradientCoil< float >;
template class gkg::GehcSignaGradientCoil< double >;
