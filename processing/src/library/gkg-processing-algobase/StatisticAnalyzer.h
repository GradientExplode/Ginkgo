#ifndef _gkg_processing_algobase_StatisticAnalyzer_h_
#define _gkg_processing_algobase_StatisticAnalyzer_h_


namespace gkg
{


template < class T > class Volume;

template < class C > class StatisticAnalyzer;


template < class T >
class StatisticAnalyzer< Volume< T > >
{

  public:

    double getAverage( const Volume< T >& in ) const;
    double getStdDev( const Volume< T >& in ) const;
    T getMinimum( const Volume< T >& in ) const;
    T getMaximum( const Volume< T >& in ) const;
    void getMinMax( const Volume< T >& in, T& minimum, T& maximum ) const;
    T getCorrectedMaximum( const Volume< T >& in,
                           int32_t thrown = 20, int32_t kept = 100 ) const;
    double getDoubleMinimum( const Volume< T >& in ) const;
    double getDoubleMaximum( const Volume< T >& in ) const;
    double getDoubleCorrectedMaximum( const Volume< T >& in,
                                 int32_t thrown = 20, int32_t kept = 100 ) const;

    double getNoiseAverage( const Volume< T >& in,
                            double noisePosition = 0.9 ) const;
    double getNoiseStdDev( const Volume< T >& in,
                           double noisePosition = 0.9 ) const;

    double getDoubleAutomaticThreshold( const Volume< T >& in,
                                double noisePosition = 0.9,
                                double noiseRatio = 0.01,
                                int32_t thrown = 20, int32_t kept = 100 ) const;
    T getAutomaticThreshold( const Volume< T >& in,
                             double noisePosition = 0.9,
                             double noiseRatio = 0.01,
                             int32_t thrown = 20, int32_t kept = 100 ) const;

};


}


#endif
