#ifndef _gkg_mri_reconstruction_core_FullKSpaceReconstructor2d_h_
#define _gkg_mri_reconstruction_core_FullKSpaceReconstructor2d_h_


#include <gkg-mri-reconstruction-core/Reconstructor.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class FullKSpaceReconstructor2d :
                              public Reconstructor< T >,
                              public Singleton< FullKSpaceReconstructor2d< T > >
{

  public:

    ~FullKSpaceReconstructor2d();

    void setRawLineCount( int32_t rawLineCount );
    int32_t getRawLineCount() const;

    void setModulationTransferFunction( const std::vector< std::vector<
                            std::complex< T > > >& modulationTransferFunction );
    const std::vector< std::vector< std::complex< T > > >&
       getModulationTransferFunction() const;

    void setXApodization( const Apodization< T >* xApodization );
    void setYApodization( const Apodization< T >* yApodization );

    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose = false ) const;

  private:

    friend class Singleton< FullKSpaceReconstructor2d< T > >;

    FullKSpaceReconstructor2d();

    int32_t _rawLineCount;
    std::vector< std::vector< std::complex< T > > > _modulationTransferFunction;
    const Apodization< T >* _xApodization;
    const Apodization< T >* _yApodization;

};


}


#endif
