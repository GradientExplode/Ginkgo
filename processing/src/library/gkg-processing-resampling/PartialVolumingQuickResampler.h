#ifndef _gkg_processing_resampling_PartialVolumingQuickResampler_h_
#define _gkg_processing_resampling_PartialVolumingQuickResampler_h_


#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;


template < class T >
class PartialVolumingQuickResampler :
                         public QuickResampler< T >,
                         public Singleton< PartialVolumingQuickResampler< T > >
{

  public:

    ~PartialVolumingQuickResampler();

    std::string getName() const;

    void resample( const Volume< T >& inVolume,
                   const T& outBackground,
                   const Vector3d< float >& outLocation,
                   T& outValue,
                   const Vector3d< double >* resolution = 0,
                   int32_t rank = 0 ) const;

    void resample( const Volume< T >& inVolume,
                   const T& outBackground,
                   const Vector3d< double >& outLocation,
                   T& outValue,
                   const Vector3d< double >* resolution = 0,
                   int32_t rank = 0 ) const;

  private:

    friend class Singleton< PartialVolumingQuickResampler< T > >;

    PartialVolumingQuickResampler();

};


}


#endif
