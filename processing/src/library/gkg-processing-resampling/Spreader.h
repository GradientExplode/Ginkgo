#ifndef _gkg_processing_resampling_Spreader_h_
#define _gkg_processing_resampling_Spreader_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <complex>
#include <string>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;


template < class T >
class Spreader
{

  public:

    virtual ~Spreader();

    virtual std::string getName() const = 0;

    virtual void spread( const T& value,
                         const Vector3d< float >& outLocation,
                         Volume< T >& outVolume,
                         const Vector3d< double >* resolution = 0,
                         int32_t rank = 0 ) const = 0;

    virtual void spread( const T& value,
                         const Vector3d< double >& outLocation,
                         Volume< T >& outVolume,
                         const Vector3d< double >* resolution = 0,
                         int32_t rank = 0 ) const = 0;

  protected:

    Spreader();

};


}


#endif
