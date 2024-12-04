#ifndef _gkg_processing_histogram_HistogramAnalyzer_h_
#define _gkg_processing_histogram_HistogramAnalyzer_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


class HistogramAnalyzer : public Singleton< HistogramAnalyzer >
{

  public:

    virtual ~HistogramAnalyzer();

    // methods without mask for volume(s)
    template < class T >
    void getBoundaries( const Volume< T >& volume,
                        T& lowerBoundary,
                        T& upperBoundary ) const;

    template < class T >
    void getHistogram( const Volume< T >& volume,
                       std::vector< double >& histogram,
                       T& lowerBoundary,
                       T& upperBoundary,
                       double kernelWidth = -1.0,
                       bool normalize = true ) const;

    template < class T >
    void getCumulatedHistogram( const Volume< T >& volume,
                                std::vector< double >& cumulatedHistogram,
                                T& lowerBoundary,
                                T& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;

    template < class T >
    void getPercentileUpperBoundaries( const Volume< T >& volume,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileLowerBoundaries( const Volume< T >& volume,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileBoundaries( const Volume< T >& volume,
                                  int32_t levelCount,
                                  double percent,
                                  T& lowerBoundary,
                                  T& upperBoundary ) const;

    template < class T >
    void getPercentileBoundariesAndMinMax( const Volume< T >& volume,
                                           int32_t levelCount,
                                           double percent,
                                           T& lowerBoundary,
                                           T& upperBoundary,
                                           T& minimum,
                                           T& maximum ) const;

    // methods with mask for volume(s)
    template < class T >
    void getBoundaries( const Volume< T >& volume,
                        const Volume< int16_t >& mask,
                        T& lowerBoundary,
                        T& upperBoundary ) const;

    template < class T >
    void getHistogram( const Volume< T >& volume,
                       const Volume< int16_t >& mask,
                       std::vector< double >& histogram,
                       T& lowerBoundary,
                       T& upperBoundary,
                       double kernelWidth = -1.0,
                       bool normalize = true ) const;

    template < class T >
    void getCumulatedHistogram( const Volume< T >& volume,
                                const Volume< int16_t >& mask,
                                std::vector< double >& cumulatedHistogram,
                                T& lowerBoundary,
                                T& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;

    template < class T >
    void getPercentileUpperBoundaries( const Volume< T >& volume,
                                       const Volume< int16_t >& mask,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileLowerBoundaries( const Volume< T >& volume,
                                       const Volume< int16_t >& mask,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileBoundaries( const Volume< T >& volume,
                                  const Volume< int16_t >& mask,
                                  int32_t levelCount,
                                  double percent,
                                  T& lowerBoundary,
                                  T& upperBoundary ) const;

    // methods for vector(s)
    template < class T >
    void getBoundaries( const std::vector< T >& v,
                        T& lowerBoundary,
                        T& upperBoundary ) const;

    template < class T >
    void getHistogram( const std::vector< T >& v,
                       std::vector< double >& histogram,
                       T& lowerBoundary,
                       T& upperBoundary,
                       double kernelWidth = -1.0,
                       bool normalize = true ) const;

    template < class T >
    void getCumulatedHistogram( const std::vector< T >& v,
                                std::vector< double >& cumulatedHistogram,
                                T& lowerBoundary,
                                T& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;

    template < class T >
    void getPercentileUpperBoundaries( const std::vector< T >& v,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileLowerBoundaries( const std::vector< T >& v,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileBoundaries( const std::vector< T >& v,
                                  int32_t levelCount,
                                  double percent,
                                  T& lowerBoundary,
                                  T& upperBoundary ) const;

    // methods for list(s)
    template < class T >
    void getBoundaries( const std::list< T >& v,
                        T& lowerBoundary,
                        T& upperBoundary ) const;

    template < class T >
    void getHistogram( const std::list< T >& v,
                       std::vector< double >& histogram,
                       T& lowerBoundary,
                       T& upperBoundary,
                       double kernelWidth = -1.0,
                       bool normalize = true ) const;

    template < class T >
    void getCumulatedHistogram( const std::list< T >& v,
                                std::vector< double >& cumulatedHistogram,
                                T& lowerBoundary,
                                T& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;

    template < class T >
    void getPercentileUpperBoundaries( const std::list< T >& v,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileLowerBoundaries( const std::list< T >& v,
                                       int32_t levelCount,
                                       double percent,
                                       T& lowerBoundary,
                                       T& upperBoundary ) const;

    template < class T >
    void getPercentileBoundaries( const std::list< T >& v,
                                  int32_t levelCount,
                                  double percent,
                                  T& lowerBoundary,
                                  T& upperBoundary ) const;

    // methods for Vector
    void getBoundaries( const Vector& v,
                        double& lowerBoundary,
                        double& upperBoundary ) const;

    void getHistogram( const Vector& v,
                       std::vector< double >& histogram,
                       double& lowerBoundary,
                       double& upperBoundary,
                       double kernelWidth = -1.0,
                       bool normalize = true ) const;

    void getCumulatedHistogram( const Vector& v,
                                std::vector< double >& cumulatedHistogram,
                                double& lowerBoundary,
                                double& upperBoundary,
                                double kernelWidth = -1.0,
                                bool normalize = true ) const;

    void getPercentileUpperBoundaries( const Vector& v,
                                       int32_t levelCount,
                                       double percent,
                                       double& lowerBoundary,
                                       double& upperBoundary ) const;

    void getPercentileLowerBoundaries( const Vector& v,
                                       int32_t levelCount,
                                       double percent,
                                       double& lowerBoundary,
                                       double& upperBoundary ) const;

    void getPercentileBoundaries( const Vector& v,
                                  int32_t levelCount,
                                  double percent,
                                  double& lowerBoundary,
                                  double& upperBoundary ) const;

  protected:

    friend class Singleton< HistogramAnalyzer >;

    HistogramAnalyzer();

};


}


#endif
