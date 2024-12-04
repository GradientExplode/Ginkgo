#ifndef _gkg_mri_reconstruction_core_Reconstructor_h_
#define _gkg_mri_reconstruction_core_Reconstructor_h_


#include <gkg-core-cppext/StdInt.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class Reconstructor
{

  public:

    virtual ~Reconstructor();

    virtual void reconstruct( const Volume< std::complex< T > >& raw,
                              Volume< std::complex< T > >& reconstructed,
                              bool verbose ) const = 0;

  protected:

    Reconstructor();

};


}


#endif
