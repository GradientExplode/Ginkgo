#ifndef _gkg_simulation_kokkos_AtomLut_i_h_
#define _gkg_simulation_kokkos_AtomLut_i_h_


#include <gkg-simulation-kokkos/AtomLut.h>
#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <ext/numeric>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct AtomCountPerGridVoxelInitializer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::AtomCountPerGridVoxelInitializer::AtomCountPerGridVoxelInitializer(
                                      Int32VectorView theAtomCountPerGridVoxel )
                                      : atomCountPerGridVoxel(
                                                      theAtomCountPerGridVoxel )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::AtomCountPerGridVoxelInitializer::operator()( int32_t lutVoxel ) const
{

#ifndef __NVCC__

  try
  {

#endif

    atomCountPerGridVoxel( lutVoxel ) = 0;
    
#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::AtomCountPerGridVoxelInitializer::operator()( "
             "int32_t lutVoxel ) const" );

#endif

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct AtomCountPerGridVoxelComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::AtomCountPerGridVoxelComputer::AtomCountPerGridVoxelComputer(
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
                                Int32VectorView theAtomCountPerGridVoxel )
                              : atomTypes( theAtomTypes ),
                                atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                                atomParameters( theAtomParameters ),
                                atomParameterOffsets( theAtomParameterOffsets ),
                                lowerX( theLowerX ),
                                spanX( theSpanX ),
                                lowerY( theLowerY ),
                                spanY( theSpanY ),
                                lowerZ( theLowerZ ),
                                spanZ( theSpanZ ),
                                lutSizeX( theLutSizeX ),
                                lutSizeY( theLutSizeY ),                                                      
                                lutSizeZ( theLutSizeZ ),
                                lutSizeXY( theLutSizeXY ),
                                atomCountPerGridVoxel(
                                                      theAtomCountPerGridVoxel )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::AtomCountPerGridVoxelComputer::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t ix = 0;
    int32_t iy = 0;
    int32_t iz = 0;

    ( *gkg::AtomMethodFactory::getInstance().getGetAtomLutVoxelMethod(
               atomTypes( atomIndex ) ) )(
                 &atomParameters( atomParameterOffsets( atomIndex ) ),
                 atomCompressedRepresentations( atomIndex ),
                 lowerX,
                 lowerY,
                 lowerZ,
                 spanX,
                 spanY,
                 spanZ,
                 lutSizeX,
                 lutSizeY,
                 lutSizeZ,
                 ix,
                 iy,
                 iz );

    ix = std::min( std::max( ix, 0 ), lutSizeX - 1 );
    iy = std::min( std::max( iy, 0 ), lutSizeY - 1 );
    iz = std::min( std::max( iz, 0 ), lutSizeZ - 1 );

    int32_t lutVoxel = ix + 
                       iy * lutSizeX +
                       iz * lutSizeXY;

    Kokkos::atomic_increment( &atomCountPerGridVoxel( lutVoxel ) );
    
#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::AtomCountPerGridVoxelComputer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct SpreadedAtomCountPerGridVoxelComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::SpreadedAtomCountPerGridVoxelComputer::
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
                                Int32VectorView theAtomCountPerGridVoxel )
                              : atomTypes( theAtomTypes ),
                                atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                                atomParameters( theAtomParameters ),
                                atomParameterOffsets( theAtomParameterOffsets ),
                                lowerX( theLowerX ),
                                gridResolutionInUmX( theGridResolutionInUmX ),
                                lowerY( theLowerY ),
                                gridResolutionInUmY( theGridResolutionInUmY ),
                                lowerZ( theLowerZ ),
                                gridResolutionInUmZ( theGridResolutionInUmZ ),
                                lutSizeX( theLutSizeX ),
                                lutSizeY( theLutSizeY ),                                                      
                                lutSizeZ( theLutSizeZ ),
                                lutSizeXY( theLutSizeXY ),
                                atomCountPerGridVoxel(
                                                      theAtomCountPerGridVoxel )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::SpreadedAtomCountPerGridVoxelComputer::operator()(
                                                       int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t infX = 0;
    int32_t supX = 0;
    int32_t infY = 0;
    int32_t supY = 0;
    int32_t infZ = 0;
    int32_t supZ = 0;
    ( *gkg::AtomMethodFactory::getInstance().getGetAtomLutVoxelSpanMethod(
               atomTypes( atomIndex ) ) )(
                 &atomParameters( atomParameterOffsets( atomIndex ) ),
                 atomCompressedRepresentations( atomIndex ),
                 lowerX,
                 lowerY,
                 lowerZ,
                 gridResolutionInUmX,
                 gridResolutionInUmY,
                 gridResolutionInUmZ,
                 lutSizeX,
                 lutSizeY,
                 lutSizeZ,
                 infX,
                 supX,
                 infY,
                 supY,
                 infZ,
                 supZ );

    int32_t indexX = 0;
    int32_t indexY = 0;
    int32_t indexZ = 0;
    int32_t lutVoxel = 0; 
    for ( indexZ = infZ; indexZ <= supZ; indexZ++ )
    {

      for ( indexY = infY; indexY <= supY; indexY++ )
      {

        for ( indexX = infX; indexX <= supX; indexX++ )
        {

          lutVoxel = indexX +
                     indexY * lutSizeX +
                     indexZ * lutSizeXY;
          Kokkos::atomic_increment( &atomCountPerGridVoxel( lutVoxel ) );

        }

      }

    }

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::SpreadedAtomCountPerGridVoxelComputer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct MaximumAtomCountPerGridVoxelComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::MaximumAtomCountPerGridVoxelComputer::MaximumAtomCountPerGridVoxelComputer(
                                       Int32VectorView theLutAtomCountPerVoxel )
                                          : lutAtomCountPerVoxel(
                                                       theLutAtomCountPerVoxel )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::MaximumAtomCountPerGridVoxelComputer::operator()(
                                   int32_t lutVoxel,
                                   int32_t& maximumAtomCountPerGridVoxel ) const
{

#ifndef __NVCC__

  try
  {

#endif

    if ( lutAtomCountPerVoxel( lutVoxel ) > maximumAtomCountPerGridVoxel )
    {
      
      maximumAtomCountPerGridVoxel = lutAtomCountPerVoxel( lutVoxel );

    }
    
#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::MaximumAtomCountPerGridVoxelComputer::operator()( "
             "int32_t lutVoxel, "
             "int32_t& maximumAtomCountPerGridVoxel ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct TotalAtomCountComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::TotalAtomCountComputer::TotalAtomCountComputer(
                                       Int32VectorView theLutAtomCountPerVoxel )
                            : lutAtomCountPerVoxel( theLutAtomCountPerVoxel )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::TotalAtomCountComputer::operator()( int32_t lutVoxel,
                                              int32_t& totalAtomCount ) const
{

#ifndef __NVCC__

  try
  {

#endif

    totalAtomCount += lutAtomCountPerVoxel( lutVoxel );
    
#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::TotalAtomCountComputer::operator()( "
             "int32_t lutVoxel, "
             "int32_t& totalAtomCount ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct AtomIndexLutComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::AtomIndexLutComputer::AtomIndexLutComputer(
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
                                int32_t theLutSizeXYZ,
                                Int32VectorView theAtomCountPerGridVoxel,
                                Int32VectorView theAtomOffsetPerGridVoxel,
                                Int32VectorView theAtomIndexLut )
                          : atomTypes( theAtomTypes ),
                            atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                            atomParameters( theAtomParameters ),
                            atomParameterOffsets( theAtomParameterOffsets ),
                            lowerX( theLowerX ),
                            spanX( theSpanX ),
                            lowerY( theLowerY ),
                            spanY( theSpanY ),
                            lowerZ( theLowerZ ),
                            spanZ( theSpanZ ),
                            lutSizeX( theLutSizeX ),
                            lutSizeY( theLutSizeY ),
                            lutSizeZ( theLutSizeZ ),
                            lutSizeXY( theLutSizeXY ),
                            lutSizeXYZ( theLutSizeXYZ ),
                            atomCountPerGridVoxel( theAtomCountPerGridVoxel ),
                            atomOffsetPerGridVoxel( theAtomOffsetPerGridVoxel ),
                            atomIndexLut( theAtomIndexLut )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::AtomIndexLutComputer::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t ix = 0;
    int32_t iy = 0;
    int32_t iz = 0;
    ( *gkg::AtomMethodFactory::getInstance().getGetAtomLutVoxelMethod(
               atomTypes( atomIndex ) ) )(
                 &atomParameters( atomParameterOffsets( atomIndex ) ),
                 atomCompressedRepresentations( atomIndex ),
                 lowerX,
                 lowerY,
                 lowerZ,
                 spanX,
                 spanY,
                 spanZ,
                 lutSizeX,
                 lutSizeY,
                 lutSizeZ,
                 ix,
                 iy,
                 iz );

    ix = std::min( std::max( ix, 0 ), lutSizeX - 1 );
    iy = std::min( std::max( iy, 0 ), lutSizeY - 1 );
    iz = std::min( std::max( iz, 0 ), lutSizeZ - 1 );

    int32_t lutVoxel = ix + 
                       iy * lutSizeX +
                       iz * lutSizeXY;

    Kokkos::atomic_assign(
                  &atomIndexLut( ( int64_t )atomOffsetPerGridVoxel( lutVoxel ) + 
                                 ( int64_t )atomCountPerGridVoxel( lutVoxel ) ),
                  atomIndex );

    Kokkos::atomic_increment( &atomCountPerGridVoxel( lutVoxel ) );

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::AtomIndexLutComputer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct SpreadedAtomIndexLutComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::SpreadedAtomIndexLutComputer::SpreadedAtomIndexLutComputer(
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
                                Int32VectorView theAtomIndexLut )
                                  : atomTypes( theAtomTypes ),
                                    atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                                    atomParameters( theAtomParameters ),
                                    atomParameterOffsets(
                                                      theAtomParameterOffsets ),
                                    lowerX( theLowerX ),
                                    gridResolutionInUmX(
                                                       theGridResolutionInUmX ),
                                    lowerY( theLowerY ),
                                    gridResolutionInUmY(
                                                       theGridResolutionInUmY ),
                                    lowerZ( theLowerZ ),
                                    gridResolutionInUmZ(
                                                       theGridResolutionInUmZ ),
                                    lutSizeX( theLutSizeX ),
                                    lutSizeY( theLutSizeY ),
                                    lutSizeZ( theLutSizeZ ),
                                    lutSizeXY( theLutSizeXY ),
                                    lutSizeXYZ( theLutSizeXYZ ),
                                    atomCountPerGridVoxel(
                                                     theAtomCountPerGridVoxel ),
                                    atomOffsetPerGridVoxel(
                                                    theAtomOffsetPerGridVoxel ),
                                    atomIndexLut( theAtomIndexLut )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::SpreadedAtomIndexLutComputer::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t infX = 0;
    int32_t supX = 0;
    int32_t infY = 0;
    int32_t supY = 0;
    int32_t infZ = 0;
    int32_t supZ = 0;
    ( *gkg::AtomMethodFactory::getInstance().getGetAtomLutVoxelSpanMethod(
               atomTypes( atomIndex ) ) )(
                 &atomParameters( atomParameterOffsets( atomIndex ) ),
                 atomCompressedRepresentations( atomIndex ),
                 lowerX,
                 lowerY,
                 lowerZ,
                 gridResolutionInUmX,
                 gridResolutionInUmY,
                 gridResolutionInUmZ,
                 lutSizeX,
                 lutSizeY,
                 lutSizeZ,
                 infX,
                 supX,
                 infY,
                 supY,
                 infZ,
                 supZ );

    int32_t indexX = 0;
    int32_t indexY = 0;
    int32_t indexZ = 0;
    int32_t lutVoxel = 0; 
    for ( indexZ = infZ; indexZ <= supZ; indexZ++ )
    {

      for ( indexY = infY; indexY <= supY; indexY++ )
      {

        for ( indexX = infX; indexX <= supX; indexX++ )
        {

          lutVoxel = indexX +
                     indexY * lutSizeX +
                     indexZ * lutSizeXY;
          Kokkos::atomic_assign(
                  &atomIndexLut( ( int64_t )atomOffsetPerGridVoxel( lutVoxel ) + 
                                 ( int64_t )atomCountPerGridVoxel( lutVoxel ) ),
                  atomIndex );
          Kokkos::atomic_increment( &atomCountPerGridVoxel( lutVoxel ) );

        }

      }

    }

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::SpreadedAtomIndexLutComputer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


#endif



