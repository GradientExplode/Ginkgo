#ifndef _gkg_mri_reconstruction_correction_AhnPhaseCorrection_h_
#define _gkg_mri_reconstruction_correction_AhnPhaseCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class T >
class AhnPhaseCorrection :  public Singleton< AhnPhaseCorrection< T > >
{

  public:

    enum Type
    {

      ALL_SLICES,
      CENTRAL_SLICE

    };

    ~AhnPhaseCorrection();

    void estimate( const Volume< std::complex< T > >& data,
                   int32_t rawLineCount,
                   std::vector< T >& constantPhaseCorrection,
                   std::vector< T >& linearPhaseCorrection,
                   const T& sideExclusionPercentage = 100.0,
                   const T& magnitudeThresholdPercentage = 15.0,
                   int32_t centerExcludedPointCount = 3,
                   bool normalizeConstant = false,
                   bool normalizeLinear = false ) const;

    void estimate( const Volume< std::complex< T > >& data,
                   int32_t rawLineCount,
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
                bool verbose = false ) const;

  private:

    friend class Singleton< AhnPhaseCorrection< T > >;

    AhnPhaseCorrection();

};


}


#endif
