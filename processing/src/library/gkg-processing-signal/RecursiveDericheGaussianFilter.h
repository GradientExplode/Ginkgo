#ifndef _gkg_processing_signal_RecursiveDericheGaussianFilter_h_
#define _gkg_processing_signal_RecursiveDericheGaussianFilter_h_


#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T > class DericheGaussianCoefficients;


template < class T >
class RecursiveDericheGaussianFilter : 
                         public Singleton< RecursiveDericheGaussianFilter< T > >
{

  public:

    ~RecursiveDericheGaussianFilter();

    void recurse( const DericheGaussianCoefficients< T >& coefficients,
                  const std::vector< T >& in,
                  std::vector< T >& out );
                  

  protected:

    friend class Singleton< RecursiveDericheGaussianFilter< T > >;

    RecursiveDericheGaussianFilter();

    std::vector< T > _temporary;

};



}


#endif
