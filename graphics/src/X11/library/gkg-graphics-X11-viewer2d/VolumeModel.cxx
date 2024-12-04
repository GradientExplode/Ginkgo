#include <gkg-graphics-X11-viewer2d/VolumeModel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/TransformFitter.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class T >
gkg::VolumeModel< T >::VolumeModel( const gkg::Volume< T >& volume,
                                    bool verbose )
                      : _volume( volume ),
                        _verbose( verbose )
{

  try
  {

    _sizeX = _volume.getSizeX();
    _sizeY = _volume.getSizeY();
    _sizeZ = _volume.getSizeZ();
    _sizeT = _volume.getSizeT();
    
    _resolutionX = 1.0;
    _resolutionY = 1.0;
    _resolutionZ = 1.0;
    _resolutionT = 1.0;

    if ( _volume.getHeader().hasAttribute( "resolutionX" ) )
    {

      _volume.getHeader().getAttribute( "resolutionX", _resolutionX );

    }
    if ( _volume.getHeader().hasAttribute( "resolutionY" ) )
    {

      _volume.getHeader().getAttribute( "resolutionY", _resolutionY );

    }
    if ( _volume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      _volume.getHeader().getAttribute( "resolutionZ", _resolutionZ );

    }
    if ( _volume.getHeader().hasAttribute( "resolutionT" ) )
    {

      _volume.getHeader().getAttribute( "resolutionT", _resolutionT );

    }

    if ( _verbose )
    {

      std::cout << "volume size(s) : "
                << _sizeX << " x "
                << _sizeY << " x "
                << _sizeZ << " x "
                << _sizeT
                << std::endl;

      std::cout << "volume resolution(s) : "
                << _resolutionX << "mm x "
                << _resolutionY << "mm x "
                << _resolutionZ << "mm x "
                << _resolutionT << "mm"
                << std::endl;

    }

    if ( _verbose )
    {

      std::cout << "processing minimum, maximum : " << std::flush;

    }

    processMinimum();
    processMaximum();

    if ( _verbose )
    {

      std::cout << _minimum << "->" << _maximum << std::endl;

    }

    _lowerThreshold = _minimum;
    _upperThreshold = _maximum;

    _raster = new gkg::Raster( ( gkg::PixelCoordinate )_sizeX,
                               ( gkg::PixelCoordinate )_sizeY );
    fillRaster( _sizeZ / 2, 0 );

    _image = new gkg::Image( _raster );
    _transformFitter = new gkg::TransformFitter( _image );
    _patch = new gkg::Patch( _transformFitter );

  }
  GKG_CATCH( "template < class T > "
             "gkg::VolumeModel< T >::VolumeModel( "
             "const gkg::Volume< T >& volume, "
             "bool verbose )" );

}


template < class T >
gkg::VolumeModel< T >::~VolumeModel()
{
}


template < class T >
int32_t gkg::VolumeModel< T >::getSizeX() const
{

  return _sizeX;

}


template < class T >
int32_t gkg::VolumeModel< T >::getSizeY() const
{

  return _sizeY;

}


template < class T >
int32_t gkg::VolumeModel< T >::getSizeZ() const
{

  return _sizeZ;

}


template < class T >
int32_t gkg::VolumeModel< T >::getSizeT() const
{

  return _sizeT;

}


template < class T >
double gkg::VolumeModel< T >::getResolutionX() const
{

  return _resolutionX;

}


template < class T >
double gkg::VolumeModel< T >::getResolutionY() const
{

  return _resolutionY;

}


template < class T >
double gkg::VolumeModel< T >::getResolutionZ() const
{

  return _resolutionZ;

}


template < class T >
double gkg::VolumeModel< T >::getResolutionT() const
{

  return _resolutionT;

}


template < class T >
gkg::FloatCoordinate gkg::VolumeModel< T >::getSpanX() const
{

  return _sizeX * _resolutionX;


}


template < class T >
gkg::FloatCoordinate gkg::VolumeModel< T >::getSpanY() const
{

  return _sizeY * _resolutionY;


}


template < class T >
double gkg::VolumeModel< T >::getMinimum() const
{

  return _minimum;

}


template < class T >
double gkg::VolumeModel< T >::getMaximum() const
{

  return _maximum;

}


template < class T >
double gkg::VolumeModel< T >::getLowerThreshold() const
{

  return _lowerThreshold;

}


template < class T >
double gkg::VolumeModel< T >::getUpperThreshold() const
{

  return _upperThreshold;

}


template < class T >
void gkg::VolumeModel< T >::setLowerThreshold( double value )
{

  _lowerThreshold = value;

}


template < class T >
void gkg::VolumeModel< T >::setUpperThreshold( double value )
{

  _upperThreshold = value;

}


template < class T >
const T& gkg::VolumeModel< T >::getValueAt( int32_t x,
                                            int32_t y,
                                            int32_t z,
                                            int32_t t ) const
{

  try
  {

    return _volume( x, y, z, t );

  }
  GKG_CATCH( "const T& gkg::VolumeModel< T >::getValueAt( "
             "int32_t x, "
             "int32_t y, "
             "int32_t z, "
             "int32_t t ) const" );

}


template < class T >
gkg::Raster* gkg::VolumeModel< T >::getRaster() const
{

  return _raster;

} 


template < class T >
gkg::Image* gkg::VolumeModel< T >::getImage() const
{

  return _image;

} 


template < class T >
gkg::Patch* gkg::VolumeModel< T >::getPatch() const
{

  return _patch;

} 


template < class T >
void gkg::VolumeModel< T >::fillRaster( int32_t slice, int32_t rank )
{

  try
  {

    gkg::PixelCoordinate pixelWidth = ( gkg::PixelCoordinate )_sizeX;
    gkg::PixelCoordinate pixelHeight = ( gkg::PixelCoordinate )_sizeY;

    gkg::PixelCoordinate x, y;
    gkg::FloatColorIntensity r = 0.0;
    gkg::FloatColorIntensity g = 0.0;
    gkg::FloatColorIntensity b = 0.0;
    gkg::FloatColorIntensity a = 1.0;
    for ( y = 0; y < pixelHeight; y++ )
    {

      for ( x = 0; x < pixelWidth; x++ )
      {

        threshold( _volume( x, y, slice, rank ), r, g, b, a );
        _raster->poke( x, pixelHeight - y - 1, r, g, b, a );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::fillRaster( "
             "int32_t slice, int32_t rank )" );

}


template < class T >
void gkg::VolumeModel< T >::threshold( const T& inputValue,
                                       gkg::FloatColorIntensity& r,
                                       gkg::FloatColorIntensity& g,
                                       gkg::FloatColorIntensity& b,
                                       gkg::FloatColorIntensity& a ) const
{

  try
  {

    if ( inputValue < _lowerThreshold )
    {

      r = ( gkg::FloatColorIntensity )0.0;
      g = ( gkg::FloatColorIntensity )0.0;
      b = ( gkg::FloatColorIntensity )0.0;
      a = ( gkg::FloatColorIntensity )1.0;

    }
    else if ( inputValue > _upperThreshold )
    {

      r = ( gkg::FloatColorIntensity )1.0;
      g = ( gkg::FloatColorIntensity )1.0;
      b = ( gkg::FloatColorIntensity )1.0;
      a = ( gkg::FloatColorIntensity )1.0;

    }
    else
    {

      if ( _upperThreshold == _lowerThreshold )
      {

        r = ( gkg::FloatColorIntensity )_lowerThreshold;
        g = ( gkg::FloatColorIntensity )_lowerThreshold;
        b = ( gkg::FloatColorIntensity )_lowerThreshold;
        a = ( gkg::FloatColorIntensity )1.0;

      }
      else
      {

        gkg::FloatColorIntensity value = ( gkg::FloatColorIntensity )(
                                ( double )( inputValue - _lowerThreshold ) /
                                ( double )( _upperThreshold - _lowerThreshold ) );
        r = ( gkg::FloatColorIntensity )value;
        g = ( gkg::FloatColorIntensity )value;
        b = ( gkg::FloatColorIntensity )value;
        a = ( gkg::FloatColorIntensity )1.0;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::threshold( const T& inputValue, "
             "gkg::FloatColorIntensity& r, "
             "gkg::FloatColorIntensity& g, "
             "gkg::FloatColorIntensity& b, "
             "gkg::FloatColorIntensity& a ) const" );

}


template < class T >
void gkg::VolumeModel< T >::processMinimum()
{

  try
  {

    gkg::MinimumFilter< gkg::Volume< T >, double > minimumFilter;
    minimumFilter.filter( _volume, _minimum );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::processMinimum()" );

}


template < class T >
void gkg::VolumeModel< T >::processMaximum()
{

  try
  {

    gkg::MaximumFilter< gkg::Volume< T >, double > maximumFilter;
    maximumFilter.filter( _volume, _maximum );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::processMaximum()" );

}


template < class T >
void gkg::VolumeModel< T >::update( const gkg::Volume< T >& volume,
                                    int32_t slice,
				    int32_t rank )
{

  try
  {

    int32_t sizeX = _volume.getSizeX();
    int32_t sizeY = _volume.getSizeY();
    int32_t sizeZ = _volume.getSizeZ();
    int32_t sizeT = _volume.getSizeT();
    
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    double resolutionT = 1.0;

    if ( volume.getHeader().hasAttribute( "resolutionX" ) )
    {

      volume.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( volume.getHeader().hasAttribute( "resolutionY" ) )
    {

      volume.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( volume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      volume.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    if ( volume.getHeader().hasAttribute( "resolutionT" ) )
    {

      volume.getHeader().getAttribute( "resolutionT", resolutionT );

    }

    if ( _verbose )
    {

      std::cout << "new volume size(s) : "
                << sizeX << " x "
                << sizeY << " x "
                << sizeZ << " x "
                << sizeT
                << std::endl;

      std::cout << "new volume resolution(s) : "
                << resolutionX << "mm x "
                << resolutionY << "mm x "
                << resolutionZ << "mm x "
                << resolutionT << "mm"
                << std::endl;

    }

    if ( ( sizeX != _sizeX ) || ( sizeY != _sizeY ) || 
         ( sizeZ != _sizeZ ) || ( sizeT != _sizeT ) ||
	 ( resolutionX != _resolutionX ) || 
	 ( resolutionY != _resolutionY ) ||
	 ( resolutionZ != _resolutionZ ) || 
	 ( resolutionT != _resolutionT ) )
    {
    
      throw std::runtime_error( "volume not compatible" );
    
    }

    _volume = volume;

    if ( _verbose )
    {

      std::cout << "processing minimum, maximum : " << std::flush;

    }

    processMinimum();
    processMaximum();

    if ( _verbose )
    {

      std::cout << _minimum << "->" << _maximum << std::endl;

    }

    fillRaster( slice, rank );

  }
  GKG_CATCH( "template < class T > "
             "gkg::VolumeModel< T >::update( "
             "const gkg::Volume< T >& volume, "
             "int32_t slice, "
	     "int32_t rank )" );

}


// specializations


namespace gkg
{


template <>
void VolumeModel< RGBComponent >::threshold( const RGBComponent& inputValue,
                                             FloatColorIntensity& r,
                                             FloatColorIntensity& g,
                                             FloatColorIntensity& b,
                                             FloatColorIntensity& a ) const
{

  try
  {

    r = ( gkg::FloatColorIntensity )inputValue.r * _upperThreshold / 255.0;
    g = ( gkg::FloatColorIntensity )inputValue.g * _upperThreshold  / 255.0;
    b = ( gkg::FloatColorIntensity )inputValue.b * _upperThreshold  / 255.0;

    if ( r > 1.0 )
    {

      r = 1.0;

    }
    if ( g > 1.0 )
    {

      g = 1.0;

    }
    if ( b > 1.0 )
    {

      b = 1.0;

    }

    a = ( gkg::FloatColorIntensity )1.0;

  }
  GKG_CATCH( "template <> "
             "void VolumeModel< RGBComponent >::threshold( "
             "const RGBComponent& inputValue, "
             "FloatColorIntensity& r, "
             "FloatColorIntensity& g, "
             "FloatColorIntensity& b, "
             "FloatColorIntensity& a ) const" );

}


template <>
void VolumeModel< RGBComponent >::processMinimum()
{

  try
  {

    _minimum = 0.0;

  }
  GKG_CATCH( "template <> "
             "void VolumeModel< RGBComponent >::processMinimum()" );

}


template <>
void VolumeModel< RGBComponent >::processMaximum()
{

  try
  {

    _maximum = 1.0;

  }
  GKG_CATCH( "template <> "
             "void VolumeModel< RGBComponent >::processMaximum()" );

}


}


//
// explicit instanciations
//

template class gkg::VolumeModel< uint8_t >;
template class gkg::VolumeModel< int8_t >;
template class gkg::VolumeModel< uint16_t >;
template class gkg::VolumeModel< int16_t >;
template class gkg::VolumeModel< uint32_t >;
template class gkg::VolumeModel< int32_t >;
template class gkg::VolumeModel< uint64_t >;
template class gkg::VolumeModel< int64_t >;
template class gkg::VolumeModel< float >;
template class gkg::VolumeModel< double >;
template class gkg::VolumeModel< gkg::RGBComponent >;
