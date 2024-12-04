#ifndef _gkg_simulation_kokkos_AtomOverlapSolver_i_h_
#define _gkg_simulation_kokkos_AtomOverlapSolver_i_h_


#include <gkg-simulation-kokkos/AtomOverlapSolver.h>
#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-simulation-virtual-tissue/AtomForceApplierFactory.h>
#include <gkg-simulation-virtual-tissue/AtomSmootherFactory.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <ext/numeric>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct RepulsionForceInitializer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::RepulsionForceInitializer::RepulsionForceInitializer(
                                                    Float3VectorView theForces )
                               : forces( theForces )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::RepulsionForceInitializer::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    forces( atomIndex, 0 ) = 0.0f;
    forces( atomIndex, 1 ) = 0.0f;
    forces( atomIndex, 2 ) = 0.0f;

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::RepulsionForceInitializer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct RepulsionForceComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::RepulsionForceComputer::RepulsionForceComputer(
                                int32_t theAtomCount,
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
                                Float3VectorView theForces )
                            : atomCount( theAtomCount ),
                              atomTypes( theAtomTypes ),
                              atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                              atomParameters( theAtomParameters ),
                              atomParameterOffsets( theAtomParameterOffsets ),
                              populationIds( thePopulationIds ),
                              cellIds( theCellIds ),
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
                              lutAtomCountPerVoxel( theLutAtomCountPerVoxel ),
                              lutAtomOffsetPerVoxel( theLutAtomOffsetPerVoxel ),
                              atomIndexLut( theAtomIndexLut ),
                              neighborhoodHalfSize( theNeighborhoodHalfSize ),
                              forces( theForces )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::RepulsionForceComputer::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    ////////////////////////////////////////////////////////////////////////////
    // collecting the current population and cell id(s)
    ////////////////////////////////////////////////////////////////////////////

    int32_t populationId = populationIds( atomIndex );
    int32_t cellId = cellIds( atomIndex );


    ////////////////////////////////////////////////////////////////////////////
    // computing the LUT voxel to which the atom belongs
    ////////////////////////////////////////////////////////////////////////////

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


    ////////////////////////////////////////////////////////////////////////////
    // looping over neighbors of the LUT voxel to which the sphere atom belongs
    ////////////////////////////////////////////////////////////////////////////

    int32_t neighborLutVoxel = 0;
    int32_t atomCountInNeighborVoxel = 0;
    int32_t otherAtomIndex = 0;
    float repulsionForceAlongX = 0.0f;
    float repulsionForceAlongY = 0.0f;
    float repulsionForceAlongZ = 0.0f;

    int32_t nx = 0;
    int32_t ny = 0;
    int32_t nz = 0;
    int32_t s = 0;
    for ( nz = std::max( iz - neighborhoodHalfSize, 0 );
          nz < std::min( iz + neighborhoodHalfSize + 1, lutSizeZ );
          nz++ )
    {

      for ( ny = std::max( iy - neighborhoodHalfSize, 0 );
            ny < std::min( iy + neighborhoodHalfSize + 1, lutSizeY );
            ny++ )
      {

        for ( nx = std::max( ix - neighborhoodHalfSize, 0 );
              nx < std::min( ix + neighborhoodHalfSize + 1, lutSizeX );
              nx++ )
        {


          neighborLutVoxel = nx + 
                             ny * lutSizeX +
                             nz * lutSizeXY;
          atomCountInNeighborVoxel = lutAtomCountPerVoxel( neighborLutVoxel );


          for ( s = 0; s < atomCountInNeighborVoxel; s++ )
          {

            otherAtomIndex = atomIndexLut( 
               ( int64_t)lutAtomOffsetPerVoxel( neighborLutVoxel ) + 
               ( int64_t)s );

            if ( ( populationIds( otherAtomIndex ) != populationId ) ||
                 ( cellIds( otherAtomIndex ) != cellId ) )
            {


              gkg::AtomColliderFactory::AtomCollider
                atomCollider = gkg::AtomColliderFactory::getInstance().
                                 getAtomCollider( atomTypes( atomIndex ),
                                                  atomTypes( otherAtomIndex ) );
              if ( ( *atomCollider )(
                                &atomParameters( 
                                            atomParameterOffsets( atomIndex ) ),
                                atomCompressedRepresentations( atomIndex ),
                                &atomParameters( 
                                       atomParameterOffsets( otherAtomIndex ) ),
                                atomCompressedRepresentations( otherAtomIndex ),
                                repulsionForceAlongX,
                                repulsionForceAlongY,
                                repulsionForceAlongZ ) )
              {

                forces( atomIndex, 0 ) += repulsionForceAlongX;
                forces( atomIndex, 1 ) += repulsionForceAlongY;
                forces( atomIndex, 2 ) += repulsionForceAlongZ;

              }

            }

          }                                            

        }

      }

    }

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::RepulsionForceComputer::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct TotalRepulsionForceNormComputer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::TotalRepulsionForceNormComputer::TotalRepulsionForceNormComputer(
                                                    Float3VectorView theForces )
                                     : forces( theForces )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::TotalRepulsionForceNormComputer::operator()(
                                          int32_t atomIndex, float& sum  ) const
{

#ifndef __NVCC__

  try
  {

#endif

    sum += std::sqrt( forces( atomIndex, 0 ) *
                      forces( atomIndex, 0 ) +
                      forces( atomIndex, 1 ) *
                      forces( atomIndex, 1 ) +
                      forces( atomIndex, 2 ) *
                      forces( atomIndex, 2 ) );

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::TotalRepulsionForceNormComputer::operator()( "
             "int32_t atomIndex, float& sum ) const" );

#endif

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct RepulsionForceApplier
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::RepulsionForceApplier::RepulsionForceApplier(
                                UInt8VectorView theAtomTypes,
                                BoolVectorView theAtomCompressedRepresentations,
                                FloatVectorView theAtomParameters,
                                UInt64VectorView theAtomParameterOffsets,
                                Float3VectorView theForces,
                                float theMaximumForceNorm )
                           : atomTypes( theAtomTypes ),
                             atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                             atomParameters( theAtomParameters ),
                             atomParameterOffsets( theAtomParameterOffsets ),
                             forces( theForces ),
                             maximumForceNorm( theMaximumForceNorm )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::RepulsionForceApplier::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    ////////////////////////////////////////////////////////////////////////////
    // computing current force norm
    ////////////////////////////////////////////////////////////////////////////

    float forceNorm = std::sqrt( forces( atomIndex, 0 ) *
                                 forces( atomIndex, 0 ) +
                                 forces( atomIndex, 1 ) *
                                 forces( atomIndex, 1 ) +
                                 forces( atomIndex, 2 ) *
                                 forces( atomIndex, 2 ) );


    ////////////////////////////////////////////////////////////////////////////
    // reducing force norm to a maximum
    ////////////////////////////////////////////////////////////////////////////

    float maximumForceNormOverForceNorm = maximumForceNorm / forceNorm;
    if ( maximumForceNormOverForceNorm < 1.0 )
    {

      forces( atomIndex, 0 ) *= maximumForceNormOverForceNorm;
      forces( atomIndex, 1 ) *= maximumForceNormOverForceNorm;
      forces( atomIndex, 2 ) *= maximumForceNormOverForceNorm;

    }


    ////////////////////////////////////////////////////////////////////////////
    // applying force to current sphere atom
    ////////////////////////////////////////////////////////////////////////////

    gkg::AtomForceApplierFactory::AtomForceApplier
      atomForceApplier = 
        gkg::AtomForceApplierFactory::getInstance().getAtomForceApplier(
                                                       atomTypes( atomIndex ) );
    ( *atomForceApplier )(
                         forces( atomIndex, 0 ),
                         forces( atomIndex, 1 ),
                         forces( atomIndex, 2 ),
                         &atomParameters( atomParameterOffsets( atomIndex ) ),
                         atomCompressedRepresentations( atomIndex ) );

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::RepulsionForceApplier::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct AtomSmoother
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::AtomSmoother::AtomSmoother(
                                int32_t theAtomCount,
                                UInt8VectorView theAtomTypes,
                                BoolVectorView theAtomCompressedRepresentations,
                                FloatVectorView theAtomParameters,
                                UInt64VectorView theAtomParameterOffsets,
                                UInt32VectorView theAtomBranchLabels )
                  : atomCount( theAtomCount ),
                    atomTypes( theAtomTypes ),
                    atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                    atomParameters( theAtomParameters ),
                    atomParameterOffsets( theAtomParameterOffsets ),
                    atomBranchLabels( theAtomBranchLabels )
{
}


KOKKOS_INLINE_FUNCTION
void gkg::AtomSmoother::operator()( int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    // we smooth only atoms belonging to branches
    uint32_t atomBranchLabel = atomBranchLabels( atomIndex );
    if ( atomBranchLabel )
    {

      int32_t previousAtomIndex = atomIndex - 1;
      int32_t nextAtomIndex = atomIndex + 1;

      // checking that previous and next atom indices are in the range
      // [0;atomCount-1] and that they also belong to the same branch
      if ( ( previousAtomIndex >= 0 ) &&
           ( nextAtomIndex < ( atomCount - 1 ) ) &&
           ( atomBranchLabels( previousAtomIndex ) == atomBranchLabel ) &&
           ( atomBranchLabels( nextAtomIndex ) == atomBranchLabel ) )

      {

        gkg::AtomSmootherFactory::AtomSmoother
          atomSmoother = 
                       gkg::AtomSmootherFactory::getInstance().getAtomSmoother(
                                                 atomTypes( previousAtomIndex ),
                                                 atomTypes( atomIndex ),
                                                 atomTypes( nextAtomIndex ) );

        ( *atomSmoother )( &atomParameters( 
                                    atomParameterOffsets( previousAtomIndex ) ),
                           atomCompressedRepresentations( previousAtomIndex ),
                           &atomParameters( 
                                    atomParameterOffsets( atomIndex ) ),
                           atomCompressedRepresentations( atomIndex ),
                           &atomParameters( 
                                    atomParameterOffsets( nextAtomIndex ) ),
                           atomCompressedRepresentations( nextAtomIndex ) );

      }

    }

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::AtomSmoother::operator()( "
             "int32_t atomIndex ) const" );

#endif

}


#endif

