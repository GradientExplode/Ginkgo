#ifndef _gkg_mri_reconstruction_core_HalfKSpaceReconstructor2d_h_
#define _gkg_mri_reconstruction_core_HalfKSpaceReconstructor2d_h_


#include <gkg-mri-reconstruction-core/Reconstructor.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class HalfKSpaceReconstructor2d :
                              public Reconstructor< T >,
                              public Singleton< HalfKSpaceReconstructor2d< T > >
{

  public:

    ~HalfKSpaceReconstructor2d();

    void setOverScanCount( int32_t overScanCount );
    int32_t getOverScanCount() const;

    void setRawLineCount( int32_t rawLineCount );
    int32_t getRawLineCount() const;

    void setHalfFourierThreshold( double halfFourierThreshold );
    double getHalfFourierThreshold() const;                         

    void setModulationTransferFunction( const std::vector< std::vector<
                            std::complex< T > > >& modulationTransferFunction );
    const std::vector< std::vector< std::complex< T > > >&
       getModulationTransferFunction() const;

    void setPhaseCorrection(
                 const std::vector< std::vector< T > >& constantPhaseCorrection,
                 const std::vector< std::vector< T > >& linearPhaseCorrection );
    const std::vector< std::vector< T > >&
      getConstantPhaseCorrection() const;
    const std::vector< std::vector< T > >&
      getLinearPhaseCorrection() const;

    void setYScrollingRatio( const T& yScrollingRatio );
    const T& getYScrollingRatio() const;

    void setXApodization( const Apodization< T >* xApodization );
    void setYApodization( const Apodization< T >* yApodization );

    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose = false ) const;

  private:

    friend class Singleton< HalfKSpaceReconstructor2d< T > >;

    HalfKSpaceReconstructor2d();

    int32_t _overScanCount;
    int32_t _rawLineCount;
    double _halfFourierThreshold;
    std::vector< std::vector< std::complex< T > > > _modulationTransferFunction;
    std::vector< std::vector< T > > _constantPhaseCorrection;
    std::vector< std::vector< T > > _linearPhaseCorrection;
    T _yScrollingRatio;
    const Apodization< T >* _xApodization;
    const Apodization< T >* _yApodization;


};


}


#endif
