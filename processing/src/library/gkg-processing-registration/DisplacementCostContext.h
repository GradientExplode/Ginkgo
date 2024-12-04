#ifndef _gkg_processing_registration_DisplacementCostContext_h_
#define _gkg_processing_registration_DisplacementCostContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


class DisplacementCostGauge;


template < class T >
class DisplacementCostContext : public LoopContext< int32_t >
{

  public:


    enum Type
    {

      SumOfAbsoluteDifference,
      CorrelationCoefficient,
      MutualInformation,
      NormalizedMutualInformation,
      CorrelationRatio

    };

    DisplacementCostContext(
      DisplacementCostGauge& gauge,
      Type type,
      const std::vector< double >& similarityMeasureParameters,
      const Volume< uint8_t >& sourceVolume,
      const Volume< uint8_t >& transformedVolume,
      const std::vector< Vector3d< int32_t > >& controlPoints,
      const int32_t& currentControlPointSpacing,
      const int32_t& currentHalfWidthSearchSpace,
      const int32_t& currentHalfWidthSearchSpaceBis,
      const int32_t& currentSearchSpaceVoxelCount,
      const BoundingBox< int32_t >& sourceBoundingBox,
      const bool& useRandomMode,
      const int32_t& maximumSampleCount,
      const std::vector< gkg::Vector3d< float > >& displacements,
      const gkg::Vector3d< int32_t >& sourceVolumeSize,
      const float& totalRegularizationWeight,
      std::vector< float >& displacementCosts,

      std::vector< Matrix >& jointProbabilityMatrices );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    float computeSimilarityMeasure(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
      const gkg::RCPointer< std::vector< gkg::Matrix > >&
                                                       jointProbabilityMatrices,
      const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
      const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                            dericheGaussianCoefficients ) const;

    float computeSumOfAbsoluteDifference(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                    jointIntensityLists ) const;

    float computeCorrelationCoefficient(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
      const gkg::RCPointer< std::vector< gkg::Matrix > >&
                                                       jointProbabilityMatrices,
      const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
      const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                            dericheGaussianCoefficients ) const;

    float computeMutualInformation(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
      const gkg::RCPointer< std::vector< gkg::Matrix > >&
                                                       jointProbabilityMatrices,
      const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
      const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                            dericheGaussianCoefficients ) const;

    float computeNormalizedMutualInformation(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
      const gkg::RCPointer< std::vector< gkg::Matrix > >&
                                                       jointProbabilityMatrices,
      const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
      const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                            dericheGaussianCoefficients ) const;

    float computeCorrelationRatio(
      int32_t d,
      const std::vector< std::list< std::pair< uint8_t, uint8_t > > >&
                                                            jointIntensityLists,
      const gkg::RCPointer< std::vector< gkg::Matrix > >&
                                                       jointProbabilityMatrices,
      const gkg::RCPointer< gkg::Matrix >& smoothedJointProbabilities,
      const gkg::RCPointer< gkg::DericheGaussianCoefficients< double > >&
                                            dericheGaussianCoefficients ) const;

    const Type& _type;
    const std::vector< double >& _similarityMeasureParameters;
    const Volume< uint8_t >& _sourceVolume;
    const Volume< uint8_t >& _transformedVolume;
    const std::vector< Vector3d< int32_t > >& _controlPoints;
    const int32_t& _currentControlPointSpacing;
    const int32_t& _currentHalfWidthSearchSpace;
    const int32_t& _currentHalfWidthSearchSpaceBis;
    const int32_t& _currentSearchSpaceVoxelCount;
    const BoundingBox< int32_t >& _sourceBoundingBox;
    const bool& _useRandomMode;
    const int32_t& _maximumSampleCount;
    const std::vector< gkg::Vector3d< float > >& _displacements;
    const gkg::Vector3d< int32_t >& _sourceVolumeSize;
    const float& _totalRegularizationWeight;
    std::vector< float >& _displacementCosts;
    
};


}


#endif
