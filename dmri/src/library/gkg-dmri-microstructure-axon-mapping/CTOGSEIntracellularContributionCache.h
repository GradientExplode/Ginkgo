#ifndef _gkg_dmri_microstructure_axon_mapping_CTOGSEIntracellularContributionCache_h_
#define _gkg_dmri_microstructure_axon_mapping_CTOGSEIntracellularContributionCache_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-communication-thread/Mutex.h>
#include <vector>
#include <map>


namespace gkg
{


class NumericalAnalysisImplementationFactory;


class CTOGSEIntracellularContributionCache :
                        public Singleton< CTOGSEIntracellularContributionCache >
{

  public:

    ~CTOGSEIntracellularContributionCache();

    void update( const Dictionary& gradientCharacteristics,
                 int32_t maximumGPDCylinderSumIndex,
                 double defaultKappa,
                 double lowerKappa,
                 double upperKappa,
                 int32_t kappaBinCount,
                 bool fixedKappa,
                 int32_t dotProductBinCount,
                 double defaultParallelDiffusivity,
                 double lowerParallelDiffusivity,
                 double upperParallelDiffusivity,
                 int32_t parallelDiffusivityBinCount,
                 bool fixedParallelDiffusivity,
                 double defaultAxonDiameter,
                 double lowerAxonDiameter,
                 double upperAxonDiameter,
                 int32_t axonDiameterBinCount,
                 bool fixedAxonDiameter );

    const std::vector< double >& getGradientMagnitudes() const;
    const std::vector< double >& getLittleDeltas() const;
    const std::vector< double >& getBigDeltas() const;
    const std::vector< double >& getRiseTimes() const;
    const std::vector< double >& getBValues() const;
    const std::vector< double >& getFrequencies() const;
    const std::vector< double >& getPhases() const;

    const std::vector< int32_t >& getSequenceConfigurationIndices() const;
    int32_t getSequenceConfigurationCount() const;
    const std::vector< std::vector< double > >& 
                                              getSequenceConfigurations() const;

    double getIntracellularContribution( int32_t rank,
                                         double kappa,
                                         double dotProduct,
                                         double parallelDiffusivity,
                                         double axonDiameter ) const;

    double getIntracellularContribution( int32_t bIndex,
                                        int32_t rank,
                                        double kappa,
                                        double dotProduct,
                                        double parallelDiffusivity,
                                        double axonDiameter ) const;

  protected:

    double computeIntracellularContribution( double bValue,
                                             double diffusionGradientMagnitude,
                                             double littleDelta,
                                             double bigDelta,
                                             double riseTime,
                                             double frequency,
                                             double phase,
                                             double kappa,
                                             double dotProduct,
                                             double parallelDiffusivity,
                                             double axonDiameter ) const;
    double computePerpendicularAttenuationInCylinder(
                                                    double littleDelta,
                                                    double bigDelta,
                                                    double riseTime,
                                                    double frequency,  
                                                    double phase,      
                                                    double parallelDiffusivity,
                                                    double axonDiameter ) const;
    double computeC2n( int32_t n, double x ) const;

    friend class Singleton< CTOGSEIntracellularContributionCache >;

    CTOGSEIntracellularContributionCache();

    NumericalAnalysisImplementationFactory* _factory;

    const Dictionary* _gradientCharacteristics;
    int32_t _maximumGPDCylinderSumIndex;
    double _defaultKappa;
    double _lowerKappa;
    double _upperKappa;
    int32_t _kappaBinCount;
    bool _fixedKappa;
    int32_t _dotProductBinCount;
    double _defaultParallelDiffusivity;
    double _lowerParallelDiffusivity;
    double _upperParallelDiffusivity;
    int32_t _parallelDiffusivityBinCount;
    bool _fixedParallelDiffusivity;
    double _defaultAxonDiameter;
    double _lowerAxonDiameter;
    double _upperAxonDiameter;
    int32_t _axonDiameterBinCount;
    bool _fixedAxonDiameter;

    std::vector< double > _gradientMagnitudes;
    std::vector< double > _littleDeltas;
    std::vector< double > _bigDeltas;
    std::vector< double > _riseTimes;
    std::vector< double > _bValues;
    std::vector< double > _frequencies;
    std::vector< double > _phases; 

    std::vector< int32_t > _sequenceConfigurationIndices;
    std::vector< std::vector< double > > _sequenceConfigurations;

    std::vector< 
      std::vector<
        std::vector<
          std::vector<
            std::vector< double > > > > > _intracellularContributionLut;

    Mutex _mutex;

};



}



#endif
