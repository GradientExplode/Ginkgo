#ifndef _gkg_processing_resampling_QuinticResampler_h_
#define _gkg_processing_resampling_QuinticResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class QuinticResampler : public SplineResampler< T >,
                         public Singleton< QuinticResampler< T > >
{

  public:

    ~QuinticResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< QuinticResampler< T > >;

    QuinticResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
