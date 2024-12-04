#ifndef _gkg_dmri_fiber_distance_NormalizedPairwiseCentroidDistanceFunctor_h_
#define _gkg_dmri_fiber_distance_NormalizedPairwiseCentroidDistanceFunctor_h_


#include <gkg-dmri-fiber-distance/FiberDistanceFunctor.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{


class NormalizedPairwiseCentroidDistanceFunctor :
                  public FiberDistanceFunctor,
                  public Creator1Arg< NormalizedPairwiseCentroidDistanceFunctor,
                                      FiberDistanceFunctor,
                                      const std::vector< double >& >
{

  public:

    virtual ~NormalizedPairwiseCentroidDistanceFunctor();

    float getDistance( const LightCurve3d< float >& centroid1,
                       const LightCurve3d< float >& centroid2,
                       const float* centroidLength1 = 0,
                       const float* centroidLength2 = 0,
                       float* nonCorrectedDistancePointer = 0,
                       float* distanceCorrectionPointer = 0 ) const;

    double getDistanceSquare( const Matrix& samples,
                              int32_t sampleIndex1,
                              int32_t sampleIndex2 ) const;

    static std::string getStaticName();
    static std::string getScalarParameterHelp();

  protected:

    friend struct Creator1Arg< NormalizedPairwiseCentroidDistanceFunctor,
                               FiberDistanceFunctor,
                               const std::vector< double >& >;

    NormalizedPairwiseCentroidDistanceFunctor(
                                const std::vector< double >& scalarParameters );

    double fastComputeDistanceSquare( const double* s1,
                                      const double* s2,
                                      int32_t count ) const;
    float fastComputeDistanceSquare( const float* s1,
                                     const float* s2,
                                     int32_t count ) const;

    float getFiberLength( const float* s1, int32_t count ) const;
    double getFiberLength( const double* s1, int32_t count ) const;

    float _normalizationFactor;
    float _lowerLengthThreshold;
    float _upperLengthThreshold;
    float _defaultNormalizedDistance;

};


}


#endif
