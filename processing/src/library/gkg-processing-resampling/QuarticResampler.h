#ifndef _gkg_processing_resampling_QuarticResampler_h_
#define _gkg_processing_resampling_QuarticResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class QuarticResampler : public SplineResampler< T >,
                         public Singleton< QuarticResampler< T > >
{

  public:

    ~QuarticResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< QuarticResampler< T > >;

    QuarticResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
