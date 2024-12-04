#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCFunction_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCFunction_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class SingleCompartmentRelaxometryMCMCFunction :
                                            public MonteCarloMarkovChainFunction
{

  public:
  
    virtual ~SingleCompartmentRelaxometryMCMCFunction(); 
    
    double getLikelihoodRatio( const Vector& realMeasurements, 
                               const Vector& currentMeasurements, 
                               const Vector& newMeasurements ) const; 
    
    const std::vector< int32_t >& getMeasurementCounts() const;
    int32_t getMeasurementCount( int32_t inputVolumeIndex ) const;
    
    const std::vector< double >& getNoiseStandardDeviations() const;
    double getNoiseStandardDeviation( int32_t inputVolumeIndex ) const;
    
    const std::vector< double >& getNoiseVariances() const;
    double getNoiseVariance( int32_t inputVolumeIndex ) const;
    
    int32_t getParameterCount() const;

    void setSite( const Vector3d< int32_t >& site );

    virtual void getValuesAt( const Vector& parameters,
                              Vector& values ) const = 0; 

  protected: 

    SingleCompartmentRelaxometryMCMCFunction( 
                         const std::vector< int32_t >& measurementCounts,
                         const std::vector< double >& noiseStandardDeviations );

  
    std::vector< int32_t > _measurementCounts;

    std::vector< double > _noiseStandardDeviations;
    int32_t _parameterCount; 

    std::vector< double > _noiseVariances;


    Vector3d< int32_t > _site;

};

}


#endif
