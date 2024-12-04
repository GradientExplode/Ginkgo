#ifndef _gkg_dmri_microstructure_axcaliber_AxCaliberOdfCartesianField_h_
#define _gkg_dmri_microstructure_axcaliber_AxCaliberOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class AxCaliberOdfCartesianField : public OdfCartesianField
{

  public:

    enum Parameters
    {

      InitialHinderedDiffusivity= 0,
      InitialIntracellularFraction,
      InitialIntracellularPerpendicularDiffusivity,
      InitialIntracellularParallelDiffusivity,
      InitialDiameterMean,
      InitialDiameterStdDev,

      LowerHinderedDiffusivity,
      LowerIntracellularFraction,
      LowerIntracellularPerpendicularDiffusivity,
      LowerIntracellularParallelDiffusivity,
      LowerDiameterMean,
      LowerDiameterStdDev,

      UpperHinderedDiffusivity,
      UpperIntracellularFraction,
      UpperIntracellularPerpendicularDiffusivity,
      UpperIntracellularParallelDiffusivity,
      UpperDiameterMean,
      UpperDiameterStdDev,

      DeltaHinderedDiffusivity,
      DeltaIntracellularFraction,
      DeltaIntracellularPerpendicularDiffusivity,
      DeltaIntracellularParallelDiffusivity,
      DeltaDiameterMean,
      DeltaDiameterStdDev,

      FixedHinderedDiffusivity,
      FixedIntracellularFraction,
      FixedIntracellularPerpendicularDiffusivity,
      FixedIntracellularParallelDiffusivity,
      FixedDiameterMean,
      FixedDiameterStdDev,

      MaximumCallaghanModelRootCount,
      MaximumCallaghanModelFunctionCount,

      NoiseStandardDeviation,

      ApplyQuickSearchOptimizer,
      QuickSearchStepCount,

      ApplyNlpOptimizer,
      NlpMaximumIterationCount,
      NlpMaximumSizeOfTheSimplex,

      ApplyMcmcOptimizer,
      McmcBurninCount,              
      McmcSampleCount,              
      McmcIntervalCount,            
      McmcMaximumIterationCount

    };
    
    AxCaliberOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    AxCaliberOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    AxCaliberOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );


    virtual ~AxCaliberOdfCartesianField();


    const Vector& getInitialParameters() const;
    const Vector& getLowerParameters() const;
    const Vector& getUpperParameters() const;
    const Vector& getDeltaParameters() const;
    const std::vector< bool >& getFixed() const;

    int32_t getMaximumCallaghanModelRootCount() const;
    int32_t getMaximumCallaghanModelFunctionCount() const;

    double getNoiseStandardDeviation() const;

    int32_t getQuickSearchStepCount() const;
    int32_t getNLPMaximumIterationCount() const;
    double getNLPMaximumSizeOfTheSimplex() const;
    int32_t getMCMCBurninCount() const;
    int32_t getMCMCSampleCount() const;
    int32_t getMCMCIntervalCount() const;
    int32_t getMCMCMaximumIterationCount() const;

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void initialize( 
                   const std::vector< double >& specificScalarParameters,
                   const std::vector< std::string >& specificStringParameters );
    void prepareMCMCFunction( bool verbose );

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    Vector _initialParameters;
    Vector _lowerParameters;
    Vector _upperParameters;
    Vector _deltaParameters;
    std::vector< bool > _fixed;

    int32_t _maximumCallaghanModelRootCount;
    int32_t _maximumCallaghanModelFunctionCount;

    double _noiseStandardDeviation;

    bool _applyQuickSearchOptimizer;
    int32_t _quicksearchStepCount;

    bool _applyNlpOptimizer;
    int32_t _nlpMaximumIterationCount;
    double _nlpMaximumSizeOfTheSimplex;

    bool _applyMcmcOptimizer;
    int32_t _mcmcBurninCount;
    int32_t _mcmcSampleCount;
    int32_t _mcmcIntervalCount;
    int32_t _mcmcMaximumIterationCount;

    RCPointer< AxCaliberMCMCFunction > _axCaliberMCMCFunction;

};


}


#endif
