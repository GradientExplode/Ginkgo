#ifndef _gkg_processing_resampling_QuickResampler_h_
#define _gkg_processing_resampling_QuickResampler_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <complex>
#include <string>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;


template < class T >
class QuickResampler
{

  public:

    virtual ~QuickResampler();

    virtual std::string getName() const = 0;

    virtual void resample( const Volume< T >& inVolume,
                           const T& outBackground,
                           const Vector3d< float >& outLocation,
                           T& outValue,
                           const Vector3d< double >* resolution = 0,
                           int32_t rank = 0 ) const = 0;

    virtual void resample( const Volume< T >& inVolume,
                           const T& outBackground,
                           const Vector3d< double >& outLocation,
                           T& outValue,
                           const Vector3d< double >* resolution = 0,
                           int32_t rank = 0 ) const = 0;

  protected:

    QuickResampler();

};


}


#endif
