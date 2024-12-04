#ifndef _gkg_mri_reconstruction_parallel_NoiseCovarianceFunctor_h_
#define _gkg_mri_reconstruction_parallel_NoiseCovarianceFunctor_h_


#include <complex>


namespace gkg
{


template < class T > class Volume;
template < class M > class TestFunction;
class Matrix;
class CMatrix;


template < class T, class M >
class NoiseCovarianceFunctor
{

  public:

    NoiseCovarianceFunctor();
    virtual ~NoiseCovarianceFunctor();

    virtual void getCovariance( const Volume< T >& coilImages,
                                const Volume< M >& noiseMask,
                                const TestFunction< M >& testFunction,
                                Matrix& noiseCovariance,
                                bool verbose = false ) const;
    virtual void getCovariance( const Volume< std::complex< T > >& coilImages,
                                const Volume< M >& noiseMask,
                                const TestFunction< M >& testFunction,
                                CMatrix& noiseCovariance,
                                bool verbose = false ) const;

};


}


#endif
