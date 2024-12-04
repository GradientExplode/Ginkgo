#ifndef _gkg_mri_reconstruction_parallel_B1ReferenceFunctor_h_
#define _gkg_mri_reconstruction_parallel_B1ReferenceFunctor_h_


#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class B1ReferenceFunctor
{

  public:

    B1ReferenceFunctor();
    virtual ~B1ReferenceFunctor();

    virtual void process( const Volume< std::complex< T > >& coilVolume,
                          Volume< std::complex< T > >& b1Reference,
                          bool verbose = false ) const;

};


}


#endif
