#ifndef _gkg_dmri_microstructure_noddi_NoddiOdfCartesianField_h_
#define _gkg_dmri_microstructure_noddi_NoddiOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class NoddiOdfCartesianField : public OdfCartesianField
{

  public:

    enum SphereDistributionFunction
    {

      Watson,
      Bingham

    };

    enum WatsonParameters
    {

      W_InitialIsotropicFraction = 0, 
      W_InitialIntracellularFraction, 
      W_InitialKappa,                 
      W_InitialParallelDiffusivity,   
      W_InitialIsotropicDiffusivity,
      W_InitialStationaryFraction,  

      W_LowerIsotropicFraction,       
      W_LowerIntracellularFraction,   
      W_LowerKappa,                   
      W_LowerParallelDiffusivity,     
      W_LowerIsotropicDiffusivity,
      W_LowerStationaryFraction,    

      W_UpperIsotropicFraction,       
      W_UpperIntracellularFraction,   
      W_UpperKappa,                   
      W_UpperParallelDiffusivity,     
      W_UpperIsotropicDiffusivity,
      W_UpperStationaryFraction,    
 
      W_DeltaIsotropicFraction,       
      W_DeltaIntracellularFraction,   
      W_DeltaKappa,                   
      W_DeltaParallelDiffusivity,     
      W_DeltaIsotropicDiffusivity,
      W_DeltaStationaryFraction,    

      W_FixedIsotropicFraction,       
      W_FixedIntracellularFraction,   
      W_FixedKappa,                   
      W_FixedPrincipalOrientation,    
      W_FixedParallelDiffusivity,     
      W_FixedIsotropicDiffusivity, 
      W_FixedStationaryFraction,   

      W_NoiseStandardDeviation,

      W_ApplyQuickSearchOptimizer,
      W_QuickSearchStepCount,

      W_ApplyNlpOptimizer,
      W_NlpMaximumIterationCount,
      W_NlpMaximumSizeOfTheSimplex,

      W_ApplyMcmcOptimizer,
      W_McmcBurninCount,              
      W_McmcSampleCount,              
      W_McmcIntervalCount,            
      W_McmcMaximumIterationCount


    };

    enum BinghamParameters
    {

      B_InitialIsotropicFraction = 0, 
      B_InitialIntracellularFraction, 
      B_InitialKappa1,                
      B_InitialKappa2,                
      B_InitialParallelDiffusivity,   
      B_InitialIsotropicDiffusivity,
      B_InitialStationaryFraction,  

      B_LowerIsotropicFraction,       
      B_LowerIntracellularFraction,   
      B_LowerKappa1,                  
      B_LowerKappa2,                  
      B_LowerParallelDiffusivity,     
      B_LowerIsotropicDiffusivity, 
      B_LowerStationaryFraction,   

      B_UpperIsotropicFraction,       
      B_UpperIntracellularFraction,   
      B_UpperKappa1,                  
      B_UpperKappa2,                  
      B_UpperParallelDiffusivity,     
      B_UpperIsotropicDiffusivity,
      B_UpperStationaryFraction,    
 
      B_DeltaIsotropicFraction,       
      B_DeltaIntracellularFraction,   
      B_DeltaKappa1,                  
      B_DeltaKappa2,                  
      B_DeltaParallelDiffusivity,     
      B_DeltaIsotropicDiffusivity,
      B_DeltaStationaryFraction,   

      B_FixedIsotropicFraction,       
      B_FixedIntracellularFraction,   
      B_FixedKappa1,                  
      B_FixedKappa2,                  
      B_FixedPrincipalOrientation,    
      B_FixedSecondaryOrientation,    
      B_FixedParallelDiffusivity,     
      B_FixedIsotropicDiffusivity,
      B_FixedStationaryFraction,    

      B_NoiseStandardDeviation,

      B_ApplyQuickSearchOptimizer,
      B_QuickSearchStepCount,

      B_ApplyNlpOptimizer,
      B_NlpMaximumIterationCount,
      B_NlpMaximumSizeOfTheSimplex,

      B_ApplyMcmcOptimizer,
      B_McmcBurninCount,              
      B_McmcSampleCount,              
      B_McmcIntervalCount,            
      B_McmcMaximumIterationCount

    };
    
    NoddiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    NoddiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    NoddiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );


    virtual ~NoddiOdfCartesianField();


    SphereDistributionFunction getSphereDistributionFunction() const;
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

    SphereDistributionFunction _sphereDistributionFunction;
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
    RCPointer< NoddiMCMCFunction > _noddiMCMCFunction;

};


}


#endif
