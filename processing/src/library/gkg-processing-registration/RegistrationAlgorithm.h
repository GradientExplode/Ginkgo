#ifndef _gkg_processing_registration_RegistrationAlgorithm_h_
#define _gkg_processing_registration_RegistrationAlgorithm_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <list>
#include <vector>
#include <string>


namespace gkg
{


template < class T > class MinimumSpanningTree;


template < class T1, class T2 >
class RegistrationAlgorithm
{

  public:

    enum TunedParameters
    {

      Direct,
      Inverse

    };


    RegistrationAlgorithm(
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       int32_t resamplingOrder,
                       const std::list< int32_t >& subSamplingSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       bool verbose );
    virtual ~RegistrationAlgorithm();

    void match( RCPointer< Volume< T1 > > reference,
                RCPointer< Volume< T2 > > floating,
                const Vector& initialParameters,
                const T1& referenceLowerThreshold,
                const T2& floatingLowerThreshold,
                RCPointer< NonLinearTransform3d< float > > transform3d,
                const std::string& intermediateFileName = "",
                TunedParameters tunedParameters = Direct );

  protected:

    void computeDisplacementCosts(
                       const Volume< uint8_t >& sourceVolume,
                       const Volume< uint8_t >& transformedVolume,
                       int32_t currentHalfWidthSearchSpace,
                       int32_t currentControlPointSpacing,
                       int32_t currentSearchSpaceQuantisation,
                       float currentRegularizationWeight,
                       std::vector< float >& displacementCosts ) const;
    void attributeOptimalDisplacements(
                 const Volume< float >& currentFlowFieldAlongX,
                 const Volume< float >& currentFlowFieldAlongY,
                 const Volume< float >& currentFlowFieldAlongZ,
                 std::vector< float >& displacementCosts,
                 int32_t currentHalfWidthSearchSpace,
                 int32_t currentSearchSpaceQuantisation,
                 const MinimumSpanningTree< uint8_t >& minimumSpanningTree,
                 Volume< float >& coarserFlowFieldAlongX,
                 Volume< float >& coarserFlowFieldAlongY,
                 Volume< float >& coarserFlowFieldAlongZ ) const;
    void scaleAndSquareTransformation(
                            Volume< float >& flowFieldAlongX,
                            Volume< float >& flowFieldAlongY,
                            Volume< float >& flowFieldAlongZ,
                            int32_t currentControlPointSpacing,
                            int32_t exponentiationStepCount ) const;
    void symmetrizeDirectAndInverseTransformations(
                            Volume< float >& directFlowFieldAlongX,
                            Volume< float >& directFlowFieldAlongY,
                            Volume< float >& directFlowFieldAlongZ,
                            Volume< float >& inverseFlowFieldAlongX,
                            Volume< float >& inverseFlowFieldAlongY,
                            Volume< float >& inverseFlowFieldAlongZ,
                            int32_t currentControlPointSpacing ) const;
    void computeJacobianDeterminant( const Volume< float >& flowFieldAlongX,
                                     const Volume< float >& flowFieldAlongY,
                                     const Volume< float >& flowFieldAlongZ,
                                     int32_t currentControlPointSpacing,
                                     float& jacobianMean,
                                     float& jacobianStandardDeviation,
                                     float& jacobianMinimum,
                                     float& jacobianMaximum,
                                     float& jacobianNegativeRatio ) const;
    float computeHarmonicEnergy( const Volume< float >& flowFieldAlongX,
                                 const Volume< float >& flowFieldAlongY,
                                 const Volume< float >& flowFieldAlongZ ) const;

    void getDisplacements(  
                        int32_t currentSearchSpaceSize,
                        int32_t currentSearchSpaceVoxelCount,
                        int32_t currentHalfWidthSearchSpace,
                        int32_t currentSearchSpaceQuantisation,
                        std::vector< Vector3d< float > >& displacements ) const;

    void fastDistanceTransform(
                            int32_t currentSearchSpaceSize,
                            const Vector3d< float >& deltaDisplacement,
                            std::vector< float >& costs,
                            std::vector< int32_t >& displacementIndices ) const;
    void fastDistanceTransformSquare(
                            int32_t index,
                            int32_t length,
                            float displacement,
                            int32_t offsetToNextElement,
                            std::vector< int32_t >& v,
                            std::vector< float >& z,
                            std::vector< float >& f,
                            std::vector< int32_t >& ind1,
                            std::vector< float >& costs,
                            std::vector< int32_t >& displacementIndices ) const;

    void getInterpolation3d( const Volume< float >& flowField,
                             const Volume< float >& flowFieldAlongX,
                             const Volume< float >& flowFieldAlongY,
                             const Volume< float >& flowFieldAlongZ,
                             Volume< float >& flowFieldPrime ) const;
    void combineDeformations3d(
                               const Volume< float >& flowFieldAlongX1,
                               const Volume< float >& flowFieldAlongY1,
                               const Volume< float >& flowFieldAlongZ1,
                               const Volume< float >& flowFieldAlongX2,
                               const Volume< float >& flowFieldAlongY2,
                               const Volume< float >& flowFieldAlongZ2,
                               Volume< float >& combinedFlowFieldAlongX,
                               Volume< float >& combinedFlowFieldAlongY,
                               Volume< float >& combinedFlowFieldAlongZ ) const;

    void fastInverseTransform( const Volume< float >& directFlowFieldAlongX,
                               const Volume< float >& directFlowFieldAlongY,
                               const Volume< float >& directFlowFieldAlongZ,
                               Volume< float >& inverseFlowFieldAlongX,
                               Volume< float >& inverseFlowFieldAlongY,
                               Volume< float >& inverseFlowFieldAlongZ ) const;

    void computeGradientAlongX(
                          const gkg::Volume< float >& flowField,
                          gkg::Volume< float >& flowFieldGradientAlongX ) const;
    void computeGradientAlongY(
                          const gkg::Volume< float >& flowField,
                          gkg::Volume< float >& flowFieldGradientAlongY ) const;
    void computeGradientAlongZ(
                          const gkg::Volume< float >& flowField,
                          gkg::Volume< float >& flowFieldGradientAlongZ ) const;

    float computeMutualInformation( const Matrix& jointProbMatrix ) const;


    std::string _similarityMeasureName;
    std::string _optimizerName;
    int32_t _resamplingOrder;
    std::list< int32_t > _subSamplingSizes;
    std::vector< double > _similarityMeasureParameters;
    std::vector< double > _optimizerParameters;

    // in case of Nelder-Mead simplex optimizer:
    //   1      -> maximum iteration count
    //   2      -> maximum test size
    //   3 .. n -> variations of parameters
    // in case of Fletcher-Reeves conjugate gradient optimizer:
    //   1 -> maximum iteration count
    //   2 -> step size
    //   3 -> maximum test gradient
    //   4 -> maximum tolerance

    bool _verbose;

};


}


#endif
