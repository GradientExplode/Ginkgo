#ifndef _gkg_mri_reconstruction_correction_EchoVolumarPhaseCorrection_h_
#define _gkg_mri_reconstruction_correction_EchoVolumarPhaseCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


template < class T >
class EchoVolumarPhaseCorrection : 
                             public Singleton< EchoVolumarPhaseCorrection< T > >
{

  public:

    ~EchoVolumarPhaseCorrection();

    void estimate( int32_t slice,
                   const std::vector< T >& ahnConstantPhaseCorrection,
                   const std::vector< T >& ahnLinearPhaseCorrection,
                   int32_t rawLineCount,
                   int32_t rawSliceCount,
                   std::vector< T >& eviConstantPhaseCorrection,
                   std::vector< T >& eviLinearPhaseCorrection,
                   const std::vector< int32_t >& flip,
                   const T& bestky,
                   const T& bestkz,
                   int32_t fitPointCount,
                   int32_t constantFitOrder,
                   int32_t linearFitOrder ) const;
    void estimate(
          const std::vector< std::vector< T > >& ahnConstantPhaseCorrection,
          const std::vector< std::vector< T > >& ahnLinearPhaseCorrection,
          int32_t rawLineCount,
          int32_t rawSliceCount,
          std::vector< std::vector< T > >& eviConstantPhaseCorrection,
          std::vector< std::vector< T > >& eviLinearPhaseCorrection,
          bool verbose,
          const std::vector< int32_t >& flip,
          const T& bestky,
          const T& bestkz,
          int32_t fitPointCount,
          int32_t constantFitOrder,
          int32_t linearFitOrder ) const;

    void apply(
            Volume< std::complex< T > >& data,
            const std::vector< std::vector< T > >& eviConstantPhaseCorrection,
            const std::vector< std::vector< T > >& eviLinearPhaseCorrection,
            int32_t rawLineCount,
            int32_t rawSliceCount,
            bool verbose ) const;

  private:

    friend class Singleton< EchoVolumarPhaseCorrection< T > >;

    EchoVolumarPhaseCorrection();

    void collectPointIndices( std::vector< int32_t >& positivePointIndices,
                              std::vector< int32_t >& negativePointIndices,
                              int32_t slice,
                              int32_t rawLineCount,
                              int32_t rawSliceCount,
                              const std::vector< int32_t >& flip,
                              const T& bestky,
                              const T& bestkz ) const;
    std::vector< T > fit( const std::vector< int32_t >& pointIndices,
                          const std::vector< T >& phaseCorrection,
                          const T& bestky,
                          const T& bestkz,
                          int32_t fitOrder ) const;
    void getEviPhaseCorrection( const std::vector< T >& positiveA,
                                const std::vector< T >& negativeA,
                                int32_t slice,
                                int32_t rawLineCount,
                                int32_t rawSliceCount,
                                std::vector< T >& eviPhaseCorrection,
                                const std::vector< int32_t >& flip,
                                const T& bestky,
                                const T& bestkz,
                                int32_t fitOrder ) const;
    void unwrap( std::vector< T >& wrappedPhase,
                 const T& bestky, const T& bestkz ) const;



};


}


#endif
