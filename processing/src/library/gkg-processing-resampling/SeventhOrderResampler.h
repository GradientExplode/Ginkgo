#ifndef _gkg_processing_resampling_SeventhOrderResampler_h_
#define _gkg_processing_resampling_SeventhOrderResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class SeventhOrderResampler : public SplineResampler< T >,
                              public Singleton< SeventhOrderResampler< T > >
{

  public:

    ~SeventhOrderResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< SeventhOrderResampler< T > >;

    SeventhOrderResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
