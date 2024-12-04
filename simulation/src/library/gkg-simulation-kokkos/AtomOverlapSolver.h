#ifndef _gkg_simulation_kokkos_AtomOverlapSolver_h_
#define _gkg_simulation_kokkos_AtomOverlapSolver_h_


#include <gkg-simulation-kokkos/Defines.h>


namespace gkg
{


struct RepulsionForceInitializer
{

  public:

    RepulsionForceInitializer( Float3VectorView theForces );

    void operator()( int32_t atomIndex ) const;

    Float3VectorView forces;

};


struct RepulsionForceComputer
{

  public:

    RepulsionForceComputer( int32_t theAtomCount,
                            UInt8VectorView theAtomTypes,
                            BoolVectorView theAtomCompressedRepresentations,
                            FloatVectorView theAtomParameters,
                            UInt64VectorView theAtomParameterOffsets,
                            Int32VectorView thePopulationIds,
                            Int32VectorView theCellIds,
                            float theLowerX,
                            float theSpanX,
                            float theLowerY,
                            float theSpanY,
                            float theLowerZ,
                            float theSpanZ,
                            int32_t theLutSizeX,
                            int32_t theLutSizeY,
                            int32_t theLutSizeZ,
                            int32_t theLutSizeXY,
                            int32_t theLutSizeXYZ,
                            Int32VectorView theLutAtomCountPerVoxel,
                            Int32VectorView theLutAtomOffsetPerVoxel,
                            Int32VectorView theAtomIndexLut,
                            int32_t theNeighborhoodHalfSize,
                            Float3VectorView theForces );

    void operator()( int32_t atomIndex ) const;

    int32_t atomCount;
    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    Int32VectorView populationIds;
    Int32VectorView cellIds;
    float lowerX;
    float spanX;
    float lowerY;
    float spanY;
    float lowerZ;
    float spanZ;
    int32_t lutSizeX;
    int32_t lutSizeY;
    int32_t lutSizeZ;
    int32_t lutSizeXY;
    int32_t lutSizeXYZ;
    Int32VectorView lutAtomCountPerVoxel;
    Int32VectorView lutAtomOffsetPerVoxel;
    Int32VectorView atomIndexLut;
    int32_t neighborhoodHalfSize;
    Float3VectorView forces;


};


struct TotalRepulsionForceNormComputer
{

  public:

    typedef float value_type;

    TotalRepulsionForceNormComputer( Float3VectorView theForces );

    void operator()( int32_t atomIndex, float& sum ) const;

    Float3VectorView forces;

};


struct RepulsionForceApplier
{

  public:

    RepulsionForceApplier( UInt8VectorView theAtomTypes,
                           BoolVectorView theAtomCompressedRepresentations,
                           FloatVectorView theAtomParameters,
                           UInt64VectorView theAtomParameterOffsets,
                           Float3VectorView theForces,
                           float theMaximumForceNorm );

    void operator()( int32_t atomIndex ) const;

    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    Float3VectorView forces;
    float maximumForceNorm;


};


struct AtomSmoother
{

  public:

    AtomSmoother( int32_t theAtomCount,
                  UInt8VectorView theAtomTypes,
                  BoolVectorView theAtomCompressedRepresentations,
                  FloatVectorView theAtomParameters,
                  UInt64VectorView theAtomParameterOffsets,
                  UInt32VectorView theAtomBranchLabels );

    void operator()( int32_t atomIndex ) const;

    int32_t atomCount;
    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    UInt32VectorView atomBranchLabels;

};


}


#endif

