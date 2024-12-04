#ifndef _gkg_mri_reconstruction_parallel_PreLearnReconstructor_h_
#define _gkg_mri_reconstruction_parallel_PreLearnReconstructor_h_


#include <gkg-mri-reconstruction-parallel/ParallelReconstructor.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class QuickResampler;
template < class T > class Spreader;
class CNormWeighting;


template < class T >
class PreLearnReconstructor : public ParallelReconstructor< T >,
                              public Singleton< PreLearnReconstructor< T > >
{

  public:

    ~PreLearnReconstructor();

    void setReference( const Volume< std::complex< T > >& reference );
    void setFoldedReference( 
                       const Volume< std::complex< T > >& foldedReference );    

    void setNeighborhood( Neighborhood3d::Type neighborhood3dType );

    void setQuickResampler(
                    const QuickResampler< std::complex< T > >& quickResampler );
    void setSpreader( const Spreader< std::complex< T > >& spreader );

    void setCNormWeighting( const CNormWeighting& cNormWeighting );


    void processUnfoldingMatrices();

    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose ) const;

  private:

    friend class Singleton< PreLearnReconstructor< T > >;

    PreLearnReconstructor();

    Volume< std::complex< T > > _reference;
    Volume< std::complex< T > > _foldedReference;

    Volume< gkg::CMatrix > _unfoldingMatrices;
    bool _unfoldingMatricesProcessed;

    std::list< Vector3d< int32_t > > _neighborOffsets;

    const QuickResampler< std::complex< T > >* _quickResampler;
    const Spreader< std::complex< T > >* _spreader;
    const CNormWeighting* _cNormWeighting;
    
};


}


#endif
