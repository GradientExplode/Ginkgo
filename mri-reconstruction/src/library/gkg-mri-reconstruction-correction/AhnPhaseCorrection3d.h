#ifndef _gkg_mri_reconstruction_correction_AhnPhaseCorrection3d_h_
#define _gkg_mri_reconstruction_correction_AhnPhaseCorrection3d_h_


#include <gkg-mri-reconstruction-correction/AhnPhaseCorrection.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T >
class AhnPhaseCorrection3d :  public Singleton< AhnPhaseCorrection3d< T > >
{

  public:

    enum Type
    {

      ALL_SLICES,
      CENTRAL_SLICE

    };

    ~AhnPhaseCorrection3d();

    void estimate( const Volume< std::complex< T > >& data,
                   int32_t rawLineCount,
                   int32_t rawSliceCount,
                   std::vector< std::vector< T > >& constantPhaseCorrection,
                   std::vector< std::vector< T > >& linearPhaseCorrection,
                   Type type,
                   bool verbose = false,
                   const T& sideExclusionPercentage = 100.0,
                   const T& magnitudeThresholdPercentage = 15.0,
                   int32_t centerExcludedPointCount = 3,
                   bool normalizeConstant = false,
                   bool normalizeLinear = false ) const;

    void apply( Volume< std::complex< T > >& data,
                const std::vector< std::vector< T > >& constantPhaseCorrection,
                const std::vector< std::vector< T > >& linearPhaseCorrection,
                int32_t rawLineCount,
                int32_t rawSliceCount,
                bool verbose = false ) const;

  private:

    friend class Singleton< AhnPhaseCorrection3d< T > >;

    AhnPhaseCorrection3d();

};


}


#endif
