#ifndef _gkg_mri_reconstruction_parallel_B1SensitivityFunctor_h_
#define _gkg_mri_reconstruction_parallel_B1SensitivityFunctor_h_


#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class B1SensitivityFunctor
{

  public:

    B1SensitivityFunctor( const T& epsilon = 0.000001 );
    virtual ~B1SensitivityFunctor();

    virtual void process( const Volume< std::complex< T > >& coilVolume,
                          const Volume< std::complex< T > >& b1Reference,
                          Volume< std::complex< T > >& b1Sensitivity,
                          bool verbose = false ) const;

  protected:

    T _epsilon;

};


}


#endif
