#ifndef _gkg_processing_resampling_NearestNeighborResampler_h_
#define _gkg_processing_resampling_NearestNeighborResampler_h_


#include <gkg-processing-resampling/Resampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class NearestNeighborResampler : public Resampler< T >,
                               public Singleton< NearestNeighborResampler< T > >
{

  public:

    ~NearestNeighborResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< NearestNeighborResampler< T > >;

    NearestNeighborResampler();

    void doResample( const Volume< T >& inVolume,
                     const Transform3d< float >& transform3d,
                     const T& outBackground,
                     const Vector3d< float >& outLocation,
                     T& outValue );
    void doResample( const Volume< T >& inVolume,
                     const Transform3d< double >& transform3d,
                     const T& outBackground,
                     const Vector3d< double >& outLocation,
                     T& outValue );

};


}


#endif
