#ifndef _gkg_processing_resampling_QuadraticResampler_h_
#define _gkg_processing_resampling_QuadraticResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class QuadraticResampler : public SplineResampler< T >,
                           public Singleton< QuadraticResampler< T > >
{

  public:

    ~QuadraticResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< QuadraticResampler< T > >;

    QuadraticResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
