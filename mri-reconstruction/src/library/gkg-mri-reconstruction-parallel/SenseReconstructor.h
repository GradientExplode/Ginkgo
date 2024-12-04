#ifndef _gkg_mri_reconstruction_parallel_SenseReconstructor_h_
#define _gkg_mri_reconstruction_parallel_SenseReconstructor_h_


#include <gkg-mri-reconstruction-parallel/ParallelReconstructor.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class QuickResampler;
template < class T > class Spreader;
class IllMatrixInverser;
class CNormWeighting;


template < class T >
class SenseReconstructor : public ParallelReconstructor< T >,
                           public Singleton< SenseReconstructor< T > >
{

  public:

    ~SenseReconstructor();

    void setB1SensitivityMap(
                          const Volume< std::complex< T > >& b1SensitivityMap );
    void setQuickResampler(
                    const QuickResampler< std::complex< T > >& quickResampler );
    void setSpreader( const Spreader< std::complex< T > >& spreader );
    void setIllMatrixInverser( const IllMatrixInverser& illMatrixInverser );
    void setCNormWeighting( const CNormWeighting& cNormWeighting );


    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose ) const;

  private:

    friend class Singleton< SenseReconstructor< T > >;

    SenseReconstructor();

    Volume< std::complex< T > > _b1SensitivityMap;
    const QuickResampler< std::complex< T > >* _quickResampler;
    const Spreader< std::complex< T > >* _spreader;
    const IllMatrixInverser* _illMatrixInverser;
    const CNormWeighting* _cNormWeighting;

};


}


#endif
