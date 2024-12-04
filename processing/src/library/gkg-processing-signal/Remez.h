#ifndef _gkg_processing_signal_Remez_h_
#define _gkg_processing_signal_Remez_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <complex>


#define GKG_MINIMUM_REMEZ_GRID_SIZE   16


namespace gkg
{


class Remez
{

  public:

    enum FilterType
    {

      BandPass,
      Differentiator,
      Hilbert

    };

    Remez( FilterType filterType,
           int32_t gridDensity = GKG_MINIMUM_REMEZ_GRID_SIZE );

    bool getOptimalFilter( std::vector< double >& response,
                           int32_t sampleCount,
                           const std::vector< double >& frequencies,
                           const std::vector< double >& magnitudes,
                           const std::vector< double >& weights ) const;
    bool getOptimalFilter( std::vector< std::complex< double > >& response,
                           int32_t sampleCount,
                           const std::vector< double >& frequencies,
                           const std::vector< double >& magnitudes,
                           const std::vector< double >& weights ) const;

  protected:

    void getParameters( const std::vector< int32_t >& extremaIndexes,
                        const std::vector< double >& grid,
                        const std::vector< double >& D,
                        const std::vector< double >& W,
                        std::vector< double >& ad,
                        std::vector< double >& x,
                        std::vector< double >& y ) const;
    void getError( const std::vector< double >& grid,
                   const std::vector< double >& D,
                   const std::vector< double >& W,
                   const std::vector< double >& ad,
                   const std::vector< double >& x,
                   const std::vector< double >& y,
                   std::vector< double >& error ) const;
    bool getExtrema( std::vector< int32_t >& extremaIndexes,
                     const std::vector< double >& error ) const;
    bool hasConverged( const std::vector< int32_t >& extremaIndexes,
                       const std::vector< double >& error ) const;
    double getA( double frequency,
                 const std::vector< double >& ad,
                 const std::vector< double >& x,
                 const std::vector< double >& y ) const;
    void getFrequencySamples( std::vector< double >& response,
                              const std::vector< double >& taps,
                              int32_t tapsCount ) const;

    FilterType _filterType;
    int32_t _gridDensity;

};


}


#endif
