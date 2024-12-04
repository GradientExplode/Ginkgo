#ifndef _gkg_mri_reconstruction_core_HalfKSpaceReconstructor3d_h_
#define _gkg_mri_reconstruction_core_HalfKSpaceReconstructor3d_h_


#include <gkg-mri-reconstruction-core/Reconstructor.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T > class Apodization;


template < class T >
class HalfKSpaceReconstructor3d :
                              public Reconstructor< T >,
                              public Singleton< HalfKSpaceReconstructor3d< T > >
{

  public:

    ~HalfKSpaceReconstructor3d();

    void setOverScanCountY( int32_t overScanCountY );
    int32_t getOverScanCountY() const;

    void setOverScanCountZ( int32_t overScanCountZ );
    int32_t getOverScanCountZ() const;

    void setRawLineCount( int32_t rawLineCount );
    int32_t getRawLineCount() const;

    void setRawSliceCount( int32_t rawSliceCount );
    int32_t getRawSliceCount() const;

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

    void setZScrollingRatio( const T& zScrollingRatio );
    const T& getZScrollingRatio() const;

    void setXApodization( const Apodization< T >* xApodization );
    void setYApodization( const Apodization< T >* yApodization );
    void setZApodization( const Apodization< T >* zApodization );

    void reconstruct( const Volume< std::complex< T > >& raw,
                      Volume< std::complex< T > >& reconstructed,
                      bool verbose = false ) const;

  private:

    friend class Singleton< HalfKSpaceReconstructor3d< T > >;

    HalfKSpaceReconstructor3d();

    int32_t _overScanCountY;
    int32_t _overScanCountZ;
    int32_t _rawLineCount;
    int32_t _rawSliceCount;
    double _halfFourierThreshold;
    std::vector< std::vector< std::complex< T > > > _modulationTransferFunction;
    std::vector< std::vector< T > > _constantPhaseCorrection;
    std::vector< std::vector< T > > _linearPhaseCorrection;
    T _yScrollingRatio;
    T _zScrollingRatio;
    const Apodization< T >* _xApodization;
    const Apodization< T >* _yApodization;
    const Apodization< T >* _zApodization;


};


}


#endif
