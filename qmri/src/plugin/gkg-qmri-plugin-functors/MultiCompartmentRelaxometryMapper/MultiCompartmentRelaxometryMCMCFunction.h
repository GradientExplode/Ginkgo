#ifndef _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentMCMCFunction_h_
#define _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentMCMCFunction_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>

namespace gkg
{


class MultiCompartmentRelaxometryMCMCFunction :
                                            public MonteCarloMarkovChainFunction
{

  public:
  
    MultiCompartmentRelaxometryMCMCFunction( 
                         const bool& t2Relaxometry, 
                         const bool& t1Relaxometry, 
                         const bool& competitiveT2, 
                         const bool& competitiveT1, 
                         const std::vector< Vector >& echoTimes, 
                         const Vector& flipAngles, 
                         const std::vector< Vector >& repetitionTimes, 
                         const std::vector< double >& t2NoiseStandardDeviations,
                         const std::vector< double >& t1NoiseStandardDeviations,
                         const int32_t& t2WeightedVolumeCount, 
                         const int32_t& t1WeightedVolumeCount, 
                         const std::vector< int32_t >& t2WeightedCounts, 
                         const std::vector< int32_t >& t1WeightedCounts, 
                         const int32_t& globalT2WeightedCount, 
                         const int32_t& globalT1WeightedCount, 
                         const int32_t& parameterCount, 
                         const int32_t& arteryCount,
                         const bool& hasCSF );
    virtual ~MultiCompartmentRelaxometryMCMCFunction(); 
    
    double getLikelihoodRatio( const Vector& realMeasurements, 
                               const Vector& currentMeasurements, 
                               const Vector& newMeasurements ) const; 
    
    bool getT2Relaxometry() const; 
    bool getT1Relaxometry() const; 
    
    int32_t getT2WeightedVolumeCount() const; 
    int32_t getT1WeightedVolumeCount() const; 
    
    const std::vector< int32_t >& getT2WeightedCounts() const;
    const std::vector< int32_t >& getT1WeightedCounts() const;
    
    int32_t getT2WeightedCount( int32_t t2WeightedVolumeIndex ) const;
    int32_t getT1WeightedCount( int32_t t1WeightedVolumeIndex ) const;
    
    int32_t getGlobalT2WeightedCount() const;
    int32_t getGlobalT1WeightedCount() const;
    
    const std::vector< double >& getT2NoiseStandardDeviations() const;
    const std::vector< double >& getT1NoiseStandardDeviations() const;
    
    double getT2NoiseStandardDeviation( int32_t t2WeightedVolumeIndex ) const;
    double getT1NoiseStandardDeviation( int32_t t1WeightedVolumeIndex ) const;
    
    const std::vector< double >& getT2NoiseVariances() const;
    const std::vector< double >& getT1NoiseVariances() const;
    
    double getT2NoiseVariance( int32_t t2WeightedVolumeIndex ) const;
    double getT1NoiseVariance( int32_t t1WeightedVolumeIndex ) const;
    
    int32_t getParameterCount() const;
    int32_t getArteryCount() const;
    bool getHasCSF() const;
    
    // parameters(  0 ) -> parenchymaProtonDensity 
    // parameters(  1 ) -> parenchymaT1
    // parameters(  2 ) -> parenchymaT2 
    // parameters(  3 ) -> csfFraction
    // parameters(  4 ) -> csfProtonDensity
    // parameters(  5 ) -> csfT1
    // parameters(  6 ) -> csfT2
    // parameters(  7 ) -> arteriesProtonDensity 
    // parameters(  8 ) -> artery1Fraction 
    // parameters(  9 ) -> artery1T1
    // parameters( 10 ) -> artery1T2 
    // ...
    // parameters( 5 + 3N ) -> arteryNFraction 
    // parameters( 5 + 3N + 1 ) -> arteryNT1 
    // parameters( 5 + 3N + 2 ) -> arteryNT2
    // parameters( 7 + 3N + 1 ) -> second T2 scaling 
    // ... 
    // parameters( 7 + 3N + k ) -> (k+1)th T2 scaling
    // parameters( 7 + 3N + k + 1 ) -> first T1 scaling
    // ...
    // parameters( 3 + 3N + k + l ) -> l th T1 scaling
    
    void setB1Scaling( float b1SCaling );
    void setCoilScaling( float coilScaling );
    void getValuesAt( const Vector& parameters, Vector& values ) const; 
    
  protected: 
  
    const bool _t2Relaxometry; 
    const bool _t1Relaxometry; 
    
    const bool _competitiveT2; 
    const bool _competitiveT1; 

    const std::vector< Vector > _echoTimes;
    const Vector _flipAngles; 
    const std::vector< Vector > _repetitionTimes;
    
    std::vector< double > _t2NoiseStandardDeviations;
    std::vector< double > _t1NoiseStandardDeviations;
    
    const int32_t _t2WeightedVolumeCount;
    const int32_t _t1WeightedVolumeCount;

    const std::vector< int32_t > _t2WeightedCounts;
    const std::vector< int32_t > _t1WeightedCounts;
    
    const int32_t _globalT2WeightedCount;
    const int32_t _globalT1WeightedCount;
    
    const int32_t _parameterCount; 
    const int32_t _arteryCount;
    const bool _hasCSF;

    std::vector< double > _t2NoiseVariances;
    std::vector< double > _t1NoiseVariances;
    
    float _b1Scaling; 
    float _coilScaling;
    

};

}


#endif
