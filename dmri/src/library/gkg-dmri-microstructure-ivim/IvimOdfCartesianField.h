#ifndef _gkg_dmri_microstructure_ivim_IvimOdfCartesianField_h_
#define _gkg_dmri_microstructure_ivim_IvimOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <vector>


namespace gkg
{


class IvimOdfCartesianField : public OdfCartesianField
{

  public:

    IvimOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    IvimOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    IvimOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );


    virtual ~IvimOdfCartesianField();


    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void initialize( 
                   const std::vector< double >& specificScalarParameters,
                   const std::vector< std::string >& specificStringParameters,
                   bool verbose );
    void extractMultipleShellInformation( bool verbose );

    void extractGaussianPartDw( RCPointer< Volume< float > >& gaussianPartDw,
                                bool verbose ) const;

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    float _ivimToParenchymaTransitionBValue;
    float _parenchymaGaussianToNonGaussianTransitionBValue;

    int32_t _shellCount;
    std::vector< float > _shellBValues;
    std::vector< int32_t > _shellIndexOffsets;
    std::vector< int32_t > _shellOrientationCounts;
    std::vector< OrientationSet > _shellOrientationSets;

    std::vector< float > _rawBValues;
    std::vector< Vector3d< float > > _rawOrientations;
    float _deltaBValue;
    RCPointer< HomogeneousTransform3d< float > > _homogeneousTransform3d;
    Dictionary _gradientCharacteristics;
    std::vector< float > _rawDiffusionTimes;
    std::vector< Vector3d< float > > _rawMotionRotation3ds;

    int32_t _ivimToParenchymaTransitionShellIndex;
    int32_t _parenchymaGaussianToNonGaussianTransitionShellIndex;

    RCPointer< TensorCartesianField > _parenchymaTensorCartesianField;

    Vector _initialParameters;
    double _noiseVariance;
    std::vector< RCPointer< OptimizerConstraint > > _optimizerConstraints;
    std::vector< double > _nlpOptimizerParameters;

};


}


#endif
