#ifndef _gkg_simulation_kokkos_DiffusionProcessKernels_h_
#define _gkg_simulation_kokkos_DiffusionProcessKernels_h_


#include <gkg-simulation-kokkos/Defines.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


////////////////////////////////////////////////////////////////////////////////
// struct VirtualTissueKokkosContainer
////////////////////////////////////////////////////////////////////////////////

struct VirtualTissueKokkosContainer
{

  public:

    VirtualTissueKokkosContainer(
      int32_t theAtomCount,
      UInt8VectorView theAtomTypes,
      BoolVectorView theAtomCompressedRepresentations,
      FloatVectorView theAtomParameters,
      UInt64VectorView theAtomParameterOffsets,      
      Int32VectorView theAtomPopulationIds,
      Int32VectorView theAtomCellIds,
      Int32VectorView theAtomCountPerGridVoxel,
      Int32VectorView theAtomOffsetPerGridVoxel,
      Int32VectorView theAtomIndexLut,
      const BoundingBox< float >& theFieldOfView,
      const Vector3d< double >& theGridResolutionInUm,
      int32_t theAtomLutSizeX,
      int32_t theAtomLutSizeY,
      int32_t theAtomLutSizeZ,
      int32_t theAtomLutSizeXY,
      int32_t theAtomLutSizeXYZ,
      int32_t theMaximumAtomCountPerGridVoxel );

    virtual ~VirtualTissueKokkosContainer();

    void getAtomLutIndices( float coordinateX,
                            float coordinateY,
                            float coordinateZ,
                            int* atomIndices,
                            int& atomCount ) const;

    bool belongToAtom( float coordinateX,
                       float coordinateY,
                       float coordinateZ,
                       int32_t atomIndex ) const;

    bool inBox( const float& coordinateX,
                const float& coordinateY,
                const float& coordinateZ ) const;

    float getDistanceToMembrane( float coordinateX,
                                 float coordinateY,
                                 float coordinateZ,
                                 int32_t atomIndex ) const;

    void getElasticBounceDirection(
                                 float particleCoordinateX,
                                 float particleCoordinateY,
                                 float particleCoordinateZ,
                                 float displacementDirectionX,
                                 float displacementDirectionY,
                                 float displacementDirectionZ,
                                 bool insideAtom,
                                 int32_t atomIndex,
                                 float& reflectedDisplacementDirectionX,
                                 float& reflectedDisplacementDirectionY,
                                 float& reflectedDisplacementDirectionZ ) const;

    bool isRayCollidingWithAtom( float particleCoordinateX,
                                 float particleCoordinateY,
                                 float particleCoordinateZ,
                                 float stepX,
                                 float stepY,
                                 float stepZ,
                                 bool insideAtom,
                                 int32_t atomIndex,
                                 float& collisionCoordinateX,
                                 float& collisionCoordinateY,
                                 float& collisionCoordinateZ,
                                 float& collisionDistance ) const;

    // parameter(s)
    int32_t atomCount;
    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    
    Int32VectorView atomPopulationIds;
    Int32VectorView atomCellIds;

    Int32VectorView atomCountPerGridVoxel;
    Int32VectorView atomOffsetPerGridVoxel;
    Int32VectorView atomIndexLut;

    BoundingBox< float > fieldOfView;

    float fieldOfViewLowerX;
    float fieldOfViewLowerY;
    float fieldOfViewLowerZ;
    float fieldOfViewSpanX;
    float fieldOfViewSpanY;
    float fieldOfViewSpanZ;
    float gridResolutionInUmX;
    float gridResolutionInUmY;
    float gridResolutionInUmZ;
    int32_t atomLutSizeX;
    int32_t atomLutSizeY;
    int32_t atomLutSizeZ;
    int32_t atomLutSizeXY;
    int32_t atomLutSizeXYZ;
    int32_t maximumAtomCountPerGridVoxel;

};


////////////////////////////////////////////////////////////////////////////////
// struct DiffusionProcessParticleInitializer
////////////////////////////////////////////////////////////////////////////////

struct DiffusionProcessParticleInitializer
{

  public:

    enum Type
    {

      Everywhere,
      OnlyIntracellular,
      OnlyExtracellular,
      AllParticlesAtFieldOfViewCenter,
      AllParticlesOnlyIntracellularAtFieldOfViewCenteredXYPlane,
      AllParticlesOnlyIntracellularAtFieldOfViewCenteredYZPlane,
      AllParticlesOnlyIntracellularAtFieldOfViewCenteredXZPlane

    };

    DiffusionProcessParticleInitializer(
      Type theType,
      RCPointer< VirtualTissueKokkosContainer > theVirtualTissueKokkosContainer,
      Float3VectorView theParticleCoordinates,
      Int32VectorView theParticleIds,
      Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace > 
                                                                  theRandPool );

    virtual ~DiffusionProcessParticleInitializer();

    void operator()( int32_t particleIndex ) const;

    // parameter(s)
    Type type;
    RCPointer< VirtualTissueKokkosContainer > virtualTissueKokkosContainer;
    Float3VectorView particleCoordinates;
    Int32VectorView particleIds;
    Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace > randPool;

};


////////////////////////////////////////////////////////////////////////////////
// struct DiffusionProcessMonteCarloAndMRISimulator
////////////////////////////////////////////////////////////////////////////////

struct DiffusionProcessMonteCarloAndMRISimulator
{

  public:

    enum ParticleToMembraneInteractionType
    {

      EqualStepLengthRandomLeap,
      ElasticBouncing

    };


    DiffusionProcessMonteCarloAndMRISimulator(
      // for Monte-Carlo simulation
      RCPointer< VirtualTissueKokkosContainer > theVirtualTissueKokkosContainer,
      FloatVectorView theCellPermeabilities,
      FloatVectorView theCellDiffusivitiesInUm2PerUs,
      FloatVectorView theCellStepLengths,
      FloatVectorView theCellTwiceStepLengthOverThreeDiffusivities,
      UInt64VectorView theAllPopulationsCellOffsets,
      float theParticleBulkDiffusivityInUm2PerUs,
      float theBulkStepLength,
      float theTwiceBulkStepLengthOverThreeBulkDiffusivity,
      ParticleToMembraneInteractionType theParticleToMembraneInteractionType,
      int32_t theParticleCount,
      Float3VectorView theParticleCoordinates,
      Int32VectorView theParticleIds,
      Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace >
                                                                    theRandPool,
      // for diffusion MRI signal attenuation simulation
      int32_t theMriSequenceCount,
      int32_t totalDiffusionWeightedContrastCount,
      FloatVectorView theAllSequencesPhaseShifts,
      Int32VectorView thePerContrastMriSequenceIndices,
      Int32VectorView thePerSequenceTimeStepCounts,
      Int32VectorView thePerSequencePhaseShiftStartingIndices,
      Int32VectorView thePerSequenceDiffusionWeightedContrastCounts,
      Int32VectorView theFormerAccumulatedDiffusionWeightedContrastCounts,
      const std::vector< FloatVectorView >& theAllSequencesPhaseAccumulators );

    virtual ~DiffusionProcessMonteCarloAndMRISimulator();

    void resetTimeStep();
    void step();

    void operator()( int32_t particleIndex ) const;


    uint64_t getPhaseShiftIndex( int32_t diffusionWeightedContrastIndex,
                                 int32_t currentTimeStep ) const;

    bool computeSubStepsWithElasticBouncing(
                              float& particleCoordinateX,
                              float& particleCoordinateY,
                              float& particleCoordinateZ,
                              int32_t& particleId,
                              float& displacementDirectionX,
                              float& displacementDirectionY,
                              float& displacementDirectionZ,
                              float& displacementNorm,
                              int32_t* sphereAtomIndices,
                              bool& hasMembraneCollision,
                              float crossingProbability,
                              float twiceStepLengthOverThreeDiffusivity ) const;
    bool computeSubStepsWithEqualStepLengthRandomLeap(
                              float& particleCoordinateX,
                              float& particleCoordinateY,
                              float& particleCoordinateZ,
                              int32_t& particleId,
                              float& displacementDirectionX,
                              float& displacementDirectionY,
                              float& displacementDirectionZ,
                              float& displacementNorm,
                              int32_t* sphereAtomIndices,
                              float crossingProbability,
                              float twiceStepLengthOverThreeDiffusivity ) const;
 

    // parameter(s)
    RCPointer< VirtualTissueKokkosContainer > virtualTissueKokkosContainer;
    FloatVectorView cellPermeabilities;
    FloatVectorView cellDiffusivitiesInUm2PerUs;
    FloatVectorView cellStepLengths;
    FloatVectorView cellTwiceStepLengthOverThreeDiffusivities;
    UInt64VectorView allPopulationsCellOffsets;
    float particleBulkDiffusivityInUm2PerUs;
    float bulkStepLength;
    float twiceBulkStepLengthOverThreeBulkDiffusivity;
    ParticleToMembraneInteractionType particleToMembraneInteractionType;
    int32_t particleCount;
    Float3VectorView particleCoordinates;
    Int32VectorView particleIds;
    Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace > randPool;

    int32_t mriSequenceCount;
    int32_t totalDiffusionWeightedContrastCount;
    FloatVectorView allSequencesPhaseShifts;

    Int32VectorView perContrastMriSequenceIndices;
    Int32VectorView perSequenceTimeStepCounts;
    Int32VectorView perSequencePhaseShiftStartingIndices;
    Int32VectorView perSequenceDiffusionWeightedContrastCounts;

    Int32VectorView formerAccumulatedDiffusionWeightedContrastCounts;

    std::vector< FloatVectorView > allSequencesPhaseAccumulators;

    int32_t currentTimeStep;


};


}


#endif

