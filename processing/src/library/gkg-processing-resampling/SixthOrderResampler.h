#ifndef _gkg_processing_resampling_SixthOrderResampler_h_
#define _gkg_processing_resampling_SixthOrderResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class SixthOrderResampler : public SplineResampler< T >,
                            public Singleton< SixthOrderResampler< T > >
{

  public:

    ~SixthOrderResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< SixthOrderResampler< T > >;

    SixthOrderResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
