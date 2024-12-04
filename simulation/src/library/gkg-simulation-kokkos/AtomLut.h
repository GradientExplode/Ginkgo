#ifndef _gkg_simulation_kokkos_AtomLut_h_
#define _gkg_simulation_kokkos_AtomLut_h_


#include <gkg-simulation-kokkos/Defines.h>


namespace gkg
{


struct AtomCountPerGridVoxelInitializer
{

  public:
  
    AtomCountPerGridVoxelInitializer( 
                                     Int32VectorView theAtomCountPerGridVoxel );

    void operator()( int32_t lutVoxel ) const;
    
    Int32VectorView atomCountPerGridVoxel;

};



struct AtomCountPerGridVoxelComputer
{

  public:

    AtomCountPerGridVoxelComputer( 
                        UInt8VectorView theAtomTypes,
                        BoolVectorView theAtomCompressedRepresentations,
                        FloatVectorView theAtomParameters,
                        UInt64VectorView theAtomParameterOffsets,
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
                        Int32VectorView theAtomCountPerGridVoxel );

    void operator()( int32_t atomIndex ) const;

    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
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
    Int32VectorView atomCountPerGridVoxel;

};


struct SpreadedAtomCountPerGridVoxelComputer
{

  public:

    SpreadedAtomCountPerGridVoxelComputer( 
                        UInt8VectorView theAtomTypes,
                        BoolVectorView theAtomCompressedRepresentations,
                        FloatVectorView theAtomParameters,
                        UInt64VectorView theAtomParameterOffsets,
                        float theLowerX,
                        float theGridResolutionInUmX,
                        float theLowerY,
                        float theGridResolutionInUmY,
                        float theLowerZ,
                        float theGridResolutionInUmZ,
                        int32_t theLutSizeX,
                        int32_t theLutSizeY,
                        int32_t theLutSizeZ,
                        int32_t theLutSizeXY,
                        Int32VectorView theAtomCountPerGridVoxel );

    void operator()( int32_t atomIndex ) const;

    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    float lowerX;
    float gridResolutionInUmX;
    float lowerY;
    float gridResolutionInUmY;
    float lowerZ;
    float gridResolutionInUmZ;
    int32_t lutSizeX;
    int32_t lutSizeY;
    int32_t lutSizeZ;
    int32_t lutSizeXY;
    Int32VectorView atomCountPerGridVoxel;

};


struct MaximumAtomCountPerGridVoxelComputer
{

  public:

    typedef int32_t value_type;

    MaximumAtomCountPerGridVoxelComputer( 
                                      Int32VectorView theLutAtomCountPerVoxel );

    void operator()( int32_t lutVoxel,
                     int32_t& maximumAtomCountPerGridVoxel ) const;

    Int32VectorView lutAtomCountPerVoxel;

};


struct TotalAtomCountComputer
{

  public:

    typedef int32_t value_type;

    TotalAtomCountComputer( Int32VectorView theLutAtomCountPerVoxel );

    void operator()( int32_t lutVoxel,
                     int32_t& totalAtomCount ) const;

    Int32VectorView lutAtomCountPerVoxel;

};


struct AtomIndexLutComputer
{

  public:

    AtomIndexLutComputer( UInt8VectorView theAtomTypes,
                          BoolVectorView theAtomCompressedRepresentations,
                          FloatVectorView theAtomParameters,
                          UInt64VectorView theAtomParameterOffsets,
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
                          Int32VectorView theAtomCountPerGridVoxel,
                          Int32VectorView theAtomOffsetPerGridVoxel,
                          Int32VectorView theAtomIndexLut );

    void operator()( int32_t atomIndex ) const;

    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
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
    Int32VectorView atomCountPerGridVoxel;
    Int32VectorView atomOffsetPerGridVoxel;
    Int32VectorView atomIndexLut;

};


struct SpreadedAtomIndexLutComputer
{

  public:

    SpreadedAtomIndexLutComputer(
                                UInt8VectorView theAtomTypes,
                                BoolVectorView theAtomCompressedRepresentations,
                                FloatVectorView theAtomParameters,
                                UInt64VectorView theAtomParameterOffsets,
                                float theLowerX,
                                float theGridResolutionInUmX,
                                float theLowerY,
                                float theGridResolutionInUmY,
                                float theLowerZ,
                                float theGridResolutionInUmZ,
                                int32_t theLutSizeX,
                                int32_t theLutSizeY,
                                int32_t theLutSizeZ,
                                int32_t theLutSizeXY,
                                int32_t theLutSizeXYZ,
                                Int32VectorView theAtomCountPerGridVoxel,
                                Int32VectorView theAtomOffsetPerGridVoxel,
                                Int32VectorView theAtomIndexLut );

    void operator()( int32_t atomIndex ) const;

    UInt8VectorView atomTypes;
    BoolVectorView atomCompressedRepresentations;
    FloatVectorView atomParameters;
    UInt64VectorView atomParameterOffsets;
    float lowerX;
    float gridResolutionInUmX;
    float lowerY;
    float gridResolutionInUmY;
    float lowerZ;
    float gridResolutionInUmZ;
    int32_t lutSizeX;
    int32_t lutSizeY;
    int32_t lutSizeZ;
    int32_t lutSizeXY;
    int32_t lutSizeXYZ;
    Int32VectorView atomCountPerGridVoxel;
    Int32VectorView atomOffsetPerGridVoxel;
    Int32VectorView atomIndexLut;

};


}


#endif

