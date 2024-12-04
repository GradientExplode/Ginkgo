#ifndef _gkg_mri_reconstruction_correction_EchoPlanarPhaseCorrection_h_
#define _gkg_mri_reconstruction_correction_EchoPlanarPhaseCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class T >
class EchoPlanarPhaseCorrection : 
                             public Singleton< EchoPlanarPhaseCorrection< T > >
{

  public:

    ~EchoPlanarPhaseCorrection();

    void estimate( const std::vector< T >& ahnConstantPhaseCorrection,
                   const std::vector< T >& ahnLinearPhaseCorrection,
                   int32_t rawLineCount,
                   std::vector< T >& epiConstantPhaseCorrection,
                   std::vector< T >& epiLinearPhaseCorrection,
                   const std::vector< int32_t >& flip,
                   const T& bestky,
                   int32_t fitPointCount,
                   int32_t constantFitOrder,
                   int32_t linearFitOrder ) const;
    void estimate(
          const std::vector< std::vector< T > >& ahnConstantPhaseCorrection,
          const std::vector< std::vector< T > >& ahnLinearPhaseCorrection,
          int32_t rawLineCount,
          std::vector< std::vector< T > >& epiConstantPhaseCorrection,
          std::vector< std::vector< T > >& epiLinearPhaseCorrection,
          bool verbose,
          const std::vector< int32_t >& flip,
          const T& bestky,
          int32_t fitPointCount,
          int32_t constantFitOrder,
          int32_t linearFitOrder ) const;

    void apply(
            Volume< std::complex< T > >& data,
            const std::vector< std::vector< T > >& epiConstantPhaseCorrection,
            const std::vector< std::vector< T > >& epiLinearPhaseCorrection,
            int32_t rawLineCount,
            bool verbose ) const;

  private:

    friend class Singleton< EchoPlanarPhaseCorrection< T > >;

    EchoPlanarPhaseCorrection();

    void collectPointIndices( std::vector< int32_t >& positivePointIndices,
                              std::vector< int32_t >& negativePointIndices,
                              int32_t rawLineCount,
                              const std::vector< int32_t >& flip,
                              const T& bestky ) const;
    std::vector< T > fit( const std::vector< int32_t >& pointIndices,
                          const std::vector< T >& phaseCorrection,
                          const T& bestky,
                          int32_t fitOrder ) const;
    void getEpiPhaseCorrection( const std::vector< T >& positiveA,
                                const std::vector< T >& negativeA,
                                int32_t rawLineCount,
                                std::vector< T >& epiPhaseCorrection,
                                const std::vector< int32_t >& flip,
                                const T& bestky,
                                int32_t fitOrder ) const;
    void unwrap( std::vector< T >& wrappedPhase,
                 const T& bestky ) const;



};


}


#endif
