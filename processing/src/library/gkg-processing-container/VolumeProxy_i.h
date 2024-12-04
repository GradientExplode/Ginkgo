#ifndef _gkg_processing_container_VolumeProxy_i_h_
#define _gkg_processing_container_VolumeProxy_i_h_


#include <gkg-processing-container/VolumeProxy.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


template < class T >
inline
gkg::VolumeProxy< T >::VolumeProxy( int32_t sizeX, int32_t sizeY, int32_t sizeZ, 
                                    int32_t sizeT )
                      : gkg::HeaderedObject(),
                        _sizeX( sizeX ),
                        _sizeY( sizeY ),
                        _sizeZ( sizeZ ),
                        _sizeT( sizeT ),
                        _sizeXY( ( int64_t )_sizeX *
                                 ( int64_t ) _sizeY ),
                        _sizeXYZ( _sizeXY * ( int64_t )_sizeZ ),
                        _sizeXYZT( _sizeXYZ * ( int64_t )_sizeT )
{

  try
  {

    this->prepareHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::VolumeProxy< T >::VolumeProxy( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::VolumeProxy< T >::VolumeProxy( const gkg::Vector3d< int32_t >& size, 
                                    int32_t sizeT )
                      : gkg::HeaderedObject(),
                        _sizeX( size.x ),
                        _sizeY( size.y ),
                        _sizeZ( size.z ),
                        _sizeT( sizeT ),
                        _sizeXY( ( int64_t )_sizeX * ( int64_t )_sizeY ),
                        _sizeXYZ( _sizeXY * ( int64_t )_sizeZ ),
                        _sizeXYZT( _sizeXYZ * ( int64_t )_sizeT )
{

  try
  {

    this->prepareHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::VolumeProxy< T >::VolumeProxy( "
             "const gkg::Vector3d< int32_t >& size, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::VolumeProxy< T >::VolumeProxy( const gkg::Vector2d< int32_t >& size, 
                                    int32_t sizeZ,
                                    int32_t sizeT )
                      : gkg::HeaderedObject(),
                        _sizeX( size.x ),
                        _sizeY( size.y ),
                        _sizeZ( sizeZ ),
                        _sizeT( sizeT ),
                        _sizeXY( ( int64_t )_sizeX * ( int64_t )_sizeY ),
                        _sizeXYZ( _sizeXY * ( int64_t )_sizeZ ),
                        _sizeXYZT( _sizeXYZ * ( int64_t )_sizeT )
{

  try
  {

    this->prepareHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::VolumeProxy< T >::VolumeProxy( "
             "const gkg::Vector2d< int32_t >& size, "
             "int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::VolumeProxy< T >::VolumeProxy( const gkg::VolumeProxy< T >& other )
                      : gkg::HeaderedObject( other ),
                        _sizeX( other._sizeX ),
                        _sizeY( other._sizeY ),
                        _sizeZ( other._sizeZ ),
                        _sizeT( other._sizeT ),
                        _sizeXY( other._sizeXY ),
                        _sizeXYZ( other._sizeXYZ ),
                        _sizeXYZT( other._sizeXYZT )
{
}


template < class T >
inline
gkg::VolumeProxy< T >::~VolumeProxy()
{
}


template < class T >
inline
int32_t gkg::VolumeProxy< T >::getSizeX() const
{

  return _sizeX;

}


template < class T >
inline
int32_t gkg::VolumeProxy< T >::getSizeY() const
{

  return _sizeY;

}


template < class T >
inline
int32_t gkg::VolumeProxy< T >::getSizeZ() const
{

  return _sizeZ;

}


template < class T >
inline
int32_t gkg::VolumeProxy< T >::getSizeT() const
{

  return _sizeT;

}


template < class T >
inline
int64_t gkg::VolumeProxy< T >::getSizeXY() const
{

  return _sizeXY;

}


template < class T >
inline
int64_t gkg::VolumeProxy< T >::getSizeXYZ() const
{

  return _sizeXYZ;

}


template < class T >
inline
int64_t gkg::VolumeProxy< T >::getSizeXYZT() const
{

  return _sizeXYZT;

}


template < class T >
template < class U >
inline
bool gkg::VolumeProxy< T >::hasSameSize( const gkg::VolumeProxy< U >& other,
                                         bool ignoreSizeT )
{

  try
  {

    bool hasSameSize = false;

    if ( ignoreSizeT )
    {

      hasSameSize = ( ( _sizeX == other.getSizeX() ) &&
                      ( _sizeY == other.getSizeY() ) &&
                      ( _sizeZ == other.getSizeZ() ) );

    }
    else
    {

      hasSameSize = ( ( _sizeX == other.getSizeX() ) &&
                      ( _sizeY == other.getSizeY() ) &&
                      ( _sizeZ == other.getSizeZ() ) &&
                      ( _sizeT == other.getSizeT() ) );

    }

    return hasSameSize;

  }
  GKG_CATCH( "template < class T > "
             "template < class U > "
             "inline "
             "bool gkg::VolumeProxy< T >::hasSameSize( "
             "const gkg::VolumeProxy< U >& other, "
             "bool ignoreSizeT )" );

}


template < class T >
template < class U >
inline
bool gkg::VolumeProxy< T >::hasSameResolution( 
                                            const gkg::VolumeProxy< U >& other,
                                            bool ignoreResolutionT,
                                            double epsilon )
{

  try
  {

    bool hasSameResolution = false;
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    double oResolutionX = 1.0;
    double oResolutionY = 1.0;
    double oResolutionZ = 1.0;
    if ( this->getHeader().hasAttribute( "resolutionX" ) &&
         other.getHeader().hasAttribute( "resolutionX" ) )
    {

      this->getHeader().getAttribute( "resolutionX", resolutionX );
      other.getHeader().getAttribute( "resolutionX", oResolutionX );

    }
    if ( this->getHeader().hasAttribute( "resolutionY" ) &&
         other.getHeader().hasAttribute( "resolutionY" ) )
    {

      this->getHeader().getAttribute( "resolutionY", resolutionY );
      other.getHeader().getAttribute( "resolutionY", oResolutionY );

    }
    if ( this->getHeader().hasAttribute( "resolutionZ" ) &&
         other.getHeader().hasAttribute( "resolutionZ" ) )
    {

      this->getHeader().getAttribute( "resolutionZ", resolutionZ );
      other.getHeader().getAttribute( "resolutionZ", oResolutionZ );

    }

    hasSameResolution = 
                     ( ( std::fabs( resolutionX - oResolutionX ) < epsilon ) && 
                       ( std::fabs( resolutionY - oResolutionY ) < epsilon ) &&
                       ( std::fabs( resolutionZ - oResolutionZ ) < epsilon ) );

    if ( !ignoreResolutionT )
    {

      double resolutionT = 1.0;
      double oResolutionT = 1.0;
      if ( this->getHeader().hasAttribute( "resolutionT" ) &&
           other.getHeader().hasAttribute( "resolutionT" ) )
      {

        this->getHeader().getAttribute( "resolutionT", resolutionT );
        other.getHeader().getAttribute( "resolutionT", oResolutionT );

      }

      hasSameResolution = hasSameResolution && 
                          ( std::fabs( resolutionT - oResolutionT ) < epsilon );

    }

    return hasSameResolution;

  }
  GKG_CATCH( "template < class T > "
             "template < class U > "
             "inline "
             "bool gkg::VolumeProxy< T >::hasSameResolution(  "
             "const gkg::VolumeProxy< U >& other )" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getSize( gkg::Vector3d< int32_t >& size ) const
{

  try
  {

    size.x = this->getSizeX();
    size.y = this->getSizeY();
    size.z = this->getSizeZ();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getSize( "
             "gkg::Vector3d< int32_t >& size ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getSize( gkg::Vector3d< int32_t >& size,
                                     int32_t& sizeT ) const
{

  try
  {

    size.x = this->getSizeX();
    size.y = this->getSizeY();
    size.z = this->getSizeZ();
    sizeT = this->getSizeT();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getSize( "
             "gkg::Vector3d< int32_t >& size, "
             "int32_t& sizeT ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getResolution(
                                    gkg::Vector3d< double >& resolution ) const
{

  try
  {

    resolution.x = 1.0;
    resolution.y = 1.0;
    resolution.z = 1.0;

    if ( this->getHeader().hasAttribute( "resolutionX" ) )
    {

      this->getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( this->getHeader().hasAttribute( "resolutionY" ) )
    {

      this->getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( this->getHeader().hasAttribute( "resolutionZ" ) )
    {

      this->getHeader().getAttribute( "resolutionZ", resolution.z );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getResolution( "
             "gkg::Vector3d< double >& resolution ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getResolution(
                                    gkg::Vector3d< double >& resolution,
                                    double& resolutionT ) const
{

  try
  {

    this->getResolution( resolution );
    resolutionT = 1.0;
    if ( this->getHeader().hasAttribute( "resolutionT" ) )
    {

      this->getHeader().getAttribute( "resolutionT", resolutionT );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getResolution( "
             "gkg::Vector3d< double >& resolution, "
             "double& resolutionT ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::setResolution(
                                     const gkg::Vector3d< double >& resolution )
{

  try
  {

    if ( ( resolution.x <= 0.0 ) || ( resolution.y <= 0.0 ) || 
         ( resolution.z <= 0.0 ) )
    {
  
      throw std::runtime_error( "Resolution shall be greater than 0" );
  
    }
  
    this->getHeader().addAttribute( "resolutionX", resolution.x );
    this->getHeader().addAttribute( "resolutionY", resolution.y );
    this->getHeader().addAttribute( "resolutionZ", resolution.z );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::setResolution( "
             "gkg::Vector3d< double >& resolution )" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::setResolution(
                                      const gkg::Vector3d< double >& resolution,
                                      double resolutionT )
{

  try
  {

    if ( resolutionT <= 0.0 )
    {
  
      throw std::runtime_error( "Resolution shall be greater than 0" );
  
    }
  
    this->setResolution( resolution );
    this->getHeader().addAttribute( "resolutionT", resolutionT );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::setResolution( "
             "gkg::Vector3d< double >& resolution, "
             "double resolutionT )" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getFieldOfView( gkg::Vector3d< float >& fov ) const
{

  try
  {

    gkg::Vector3d< double > resolution;
    this->getResolution( resolution );

    fov.x = ( float )( _sizeX * resolution.x );
    fov.y = ( float )( _sizeY * resolution.y );
    fov.z = ( float )( _sizeZ * resolution.z );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getFieldOfView( "
             "Vector3d< float >& fov ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getFieldOfView( gkg::Vector3d< double >& fov ) const
{

  try
  {

    gkg::Vector3d< double > resolution;
    this->getResolution( resolution );

    fov.x = ( double )( _sizeX * resolution.x );
    fov.y = ( double )( _sizeY * resolution.y );
    fov.z = ( double )( _sizeZ * resolution.z );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getFieldOfView( "
             "Vector3d< double >& fov ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getFieldOfViewCenter(
                                       gkg::Vector3d< float >& fovCenter ) const
{

  try
  {

    gkg::Vector3d< float > fov;
    this->getFieldOfView( fov );

    fovCenter = fov * 0.5f;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getFieldOfViewCenter( "
             "gkg::Vector3d< float >& fovCenter ) const" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::getFieldOfViewCenter(
                                      gkg::Vector3d< double >& fovCenter ) const
{

  try
  {

    gkg::Vector3d< double > fov;
    this->getFieldOfView( fov );

    fovCenter = fov * 0.5;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::getFieldOfViewCenter( "
             "gkg::Vector3d< double >& fovCenter ) const" );

}


template < class T >
inline
gkg::VolumeProxy< T >& 
gkg::VolumeProxy< T >::operator=( const gkg::VolumeProxy< T >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _sizeX = other._sizeX;
    _sizeY = other._sizeY;
    _sizeZ = other._sizeZ;
    _sizeT = other._sizeT;
    _sizeXY = other._sizeXY;
    _sizeXYZ = other._sizeXYZ;
    _sizeXYZT = other._sizeXYZT;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::VolumeProxy< T >&  "
             "gkg::VolumeProxy< T >::operator=( "
             "const gkg::VolumeProxy< T >& other )" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::update( const gkg::Observable* /*observable*/,
                                    void* /*arg*/ )
{

  try
  {

    _header.getAttribute( "sizeX", _sizeX );
    _header.getAttribute( "sizeY", _sizeY );
    _header.getAttribute( "sizeZ", _sizeZ );
    _header.getAttribute( "sizeT", _sizeT );
    _sizeXY = ( int64_t )_sizeX * ( int64_t )_sizeY;
    _sizeXYZ = _sizeXY * ( int64_t )_sizeZ;
    _sizeXYZT = _sizeXYZ * ( int64_t )_sizeT;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::update( "
             "const gkg::Observable* /*observable*/, "
             "void* /*arg*/ )" );

}


template < class T >
inline
void gkg::VolumeProxy< T >::prepareHeader()
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "sizeX" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "sizeY" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "sizeZ" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "sizeT" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Volume" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "sizeX", _sizeX );
    _header.addAttribute( "sizeY", _sizeY );
    _header.addAttribute( "sizeZ", _sizeZ );
    _header.addAttribute( "sizeT", _sizeT );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeProxy< T >::prepareHeader()" );

}


#endif


