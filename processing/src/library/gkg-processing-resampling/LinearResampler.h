#ifndef _gkg_processing_resampling_LinearResampler_h_
#define _gkg_processing_resampling_LinearResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class LinearResampler : public SplineResampler< T >,
                        public Singleton< LinearResampler< T > >
{

  public:

    ~LinearResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< LinearResampler< T > >;

    LinearResampler();

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
    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
