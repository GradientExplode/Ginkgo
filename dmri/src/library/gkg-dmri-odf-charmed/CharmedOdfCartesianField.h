#ifndef _gkg_dmri_odf_charmed_CharmedOdfCartesianField_h_
#define _gkg_dmri_odf_charmed_CharmedOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-odf-charmed/CharmedMCMCFunction.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class CharmedOdfCartesianField : public OdfCartesianField
{

  public:

    enum Type
    {

      OneCompartment,
      TwoCompartments

    };

    enum CharmedOneCompartmentParameters
    {
      
      OneCompartment_InitialRadius, 
      OneCompartment_InitialIsotropicFraction, 
      OneCompartment_InitialIsotropicDiffusivity,                   
      OneCompartment_InitialParallelDiffusivity,
      OneCompartment_InitialPerpendicularDiffusivity, 
      OneCompartment_InitialRestrictedFraction,

      OneCompartment_LowerRadius, 
      OneCompartment_LowerIsotropicFraction, 
      OneCompartment_LowerIsotropicDiffusivity,                 
      OneCompartment_LowerParallelDiffusivity,
      OneCompartment_LowerPerpendicularDiffusivity,  
      OneCompartment_LowerRestrictedFraction,

      OneCompartment_UpperRadius, 
      OneCompartment_UpperIsotropicFraction, 
      OneCompartment_UpperIsotropicDiffusivity,                 
      OneCompartment_UpperParallelDiffusivity,
      OneCompartment_UpperPerpendicularDiffusivity, 
      OneCompartment_UpperRestrictedFraction,
 
      OneCompartment_DeltaRadius, 
      OneCompartment_DeltaIsotropicFraction, 
      OneCompartment_DeltaIsotropicDiffusivity,                 
      OneCompartment_DeltaParallelDiffusivity,
      OneCompartment_DeltaPerpendicularDiffusivity,  
      OneCompartment_DeltaRestrictedFraction, 

      OneCompartment_FixedRadius, 
      OneCompartment_FixedIsotropicFraction, 
      OneCompartment_FixedIsotropicDiffusivity,                   
      OneCompartment_FixedParallelDiffusivity,
      OneCompartment_FixedPerpendicularDiffusivity,  
      OneCompartment_FixedRestrictedFraction,
      OneCompartment_FixedPrincipalOrientation,
    

      OneCompartment_NoiseStandardDeviation,

      OneCompartment_ApplyQuickSearchOptimizer,
      OneCompartment_QuickSearchStepCount,

      OneCompartment_ApplyNlpOptimizer,
      OneCompartment_NlpMaximumIterationCount,
      OneCompartment_NlpMaximumSizeOfTheSimplex,

      OneCompartment_ApplyMcmcOptimizer,
      OneCompartment_McmcBurninCount,              
      OneCompartment_McmcSampleCount,              
      OneCompartment_McmcIntervalCount,            
      OneCompartment_McmcMaximumIterationCount

    };

    enum CharmedTwoeCompartmentParameters
    {
      
      TwoCompartment_InitialRadius, 
      TwoCompartment_InitialIsotropicFraction, 
      TwoCompartment_InitialIsotropicDiffusivity,                   
      TwoCompartment_InitialParallelDiffusivity,
      TwoCompartment_InitialPerpendicularDiffusivity, 
      TwoCompartment_InitialRestrictedFraction1,
      TwoCompartment_InitialRestrictedFraction2,

      TwoCompartment_LowerRadius, 
      TwoCompartment_LowerIsotropicFraction, 
      TwoCompartment_LowerIsotropicDiffusivity,                 
      TwoCompartment_LowerParallelDiffusivity,
      TwoCompartment_LowerPerpendicularDiffusivity,  
      TwoCompartment_LowerRestrictedFraction1,
      TwoCompartment_LowerRestrictedFraction2,

      TwoCompartment_UpperRadius, 
      TwoCompartment_UpperIsotropicFraction, 
      TwoCompartment_UpperIsotropicDiffusivity,                 
      TwoCompartment_UpperParallelDiffusivity,
      TwoCompartment_UpperPerpendicularDiffusivity, 
      TwoCompartment_UpperRestrictedFraction1,
      TwoCompartment_UpperRestrictedFraction2,
 
      TwoCompartment_DeltaRadius, 
      TwoCompartment_DeltaIsotropicFraction, 
      TwoCompartment_DeltaIsotropicDiffusivity,                 
      TwoCompartment_DeltaParallelDiffusivity,
      TwoCompartment_DeltaPerpendicularDiffusivity,  
      TwoCompartment_DeltaRestrictedFraction1,
      TwoCompartment_DeltaRestrictedFraction2,

      TwoCompartment_FixedRadius, 
      TwoCompartment_FixedIsotropicFraction, 
      TwoCompartment_FixedIsotropicDiffusivity,                   
      TwoCompartment_FixedParallelDiffusivity,
      TwoCompartment_FixedPerpendicularDiffusivity,  
      TwoCompartment_FixedRestrictedFraction1,
      TwoCompartment_FixedPrincipalOrientation,
      TwoCompartment_FixedRestrictedFraction2,
      TwoCompartment_FixedSecondaryOrientation,

      TwoCompartment_NoiseStandardDeviation,

      TwoCompartment_ApplyQuickSearchOptimizer,
      TwoCompartment_QuickSearchStepCount,

      TwoCompartment_ApplyNlpOptimizer,
      TwoCompartment_NlpMaximumIterationCount,
      TwoCompartment_NlpMaximumSizeOfTheSimplex,

      TwoCompartment_ApplyMcmcOptimizer,
      TwoCompartment_McmcBurninCount,              
      TwoCompartment_McmcSampleCount,              
      TwoCompartment_McmcIntervalCount,            
      TwoCompartment_McmcMaximumIterationCount

    };

    CharmedOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    CharmedOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    CharmedOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    virtual ~CharmedOdfCartesianField();

    Type getType() const;
    const Vector& getInitialParameters() const;
    const Vector& getLowerParameters() const;
    const Vector& getUpperParameters() const;
    const Vector& getDeltaParameters() const;
    const std::vector< bool >& getFixed() const;
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

    Type _type;  
    
    Vector _initialParameters;
    Vector _lowerParameters;
    Vector _upperParameters;
    Vector _deltaParameters;
    std::vector< bool > _fixed;
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

    RCPointer< TensorCartesianField > _tensorCartesianField;
    RCPointer< CharmedMCMCFunction > _charmedMCMCFunction;

};


}


#endif
