#ifndef _gkg_processing_resampling_CubicResampler_h_
#define _gkg_processing_resampling_CubicResampler_h_


#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class CubicResampler : public SplineResampler< T >,
                       public Singleton< CubicResampler< T > >
{

  public:

    ~CubicResampler();

    std::string getName() const;
    int32_t getOrder() const;

  private:

    friend class Singleton< CubicResampler< T > >;

    CubicResampler();

    double getBSplineWeight( int32_t i, double x ) const;

};


}


#endif
