#ifndef _gkg_mri_reconstruction_core_FullKSpaceReconstructor3d_h_
#define _gkg_mri_reconstruction_core_FullKSpaceReconstructor3d_h_


#include <gkg-mri-reconstruction-core/Reconstructor.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class FullKSpaceReconstructor3d :
                              public Reconstructor< T >,
                              public Singleton< FullKSpaceReconstructor3d< T > >
{

  public:

    ~FullKSpaceReconstructor3d();

    void setXApodization( const Apodization< T >* xApodization );
    void setYApodization( const Apodization< T >* yApodization );
    void setZApodization( const Apodization< T >* zApodization );

    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose = false ) const;

  private:

    friend class Singleton< FullKSpaceReconstructor3d< T > >;

    FullKSpaceReconstructor3d();

    const Apodization< T >* _xApodization;
    const Apodization< T >* _yApodization;
    const Apodization< T >* _zApodization;

};


}


#endif
