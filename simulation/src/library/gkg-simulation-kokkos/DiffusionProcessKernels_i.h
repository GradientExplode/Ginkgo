#ifndef _gkg_simulation_kokkos_DiffusionProcessKernels_i_h_
#define _gkg_simulation_kokkos_DiffusionProcessKernels_i_h_


#include <gkg-simulation-kokkos/DiffusionProcessKernels.h>
#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-core-exception/Exception.h>
#include <limits>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct VirtualTissueKokkosContainer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


inline
gkg::VirtualTissueKokkosContainer::VirtualTissueKokkosContainer(
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
                                const gkg::BoundingBox< float >& theFieldOfView,
                                const Vector3d< double >& theGridResolutionInUm,
                                int32_t theAtomLutSizeX,
                                int32_t theAtomLutSizeY,
                                int32_t theAtomLutSizeZ,
                                int32_t theAtomLutSizeXY,
                                int32_t theAtomLutSizeXYZ,
                                int32_t theMaximumAtomCountPerGridVoxel )
                                  : atomCount( theAtomCount ),
                                    atomTypes( theAtomTypes ),
                                    atomCompressedRepresentations(
                                             theAtomCompressedRepresentations ),
                                    atomParameters( theAtomParameters ),
                                    atomParameterOffsets(
                                                      theAtomParameterOffsets ),
                                    atomPopulationIds( theAtomPopulationIds ),
                                    atomCellIds( theAtomCellIds ),
                                    atomCountPerGridVoxel(
                                                     theAtomCountPerGridVoxel ),
                                    atomOffsetPerGridVoxel(
                                                    theAtomOffsetPerGridVoxel ),
                                    atomIndexLut( theAtomIndexLut ),
                                    fieldOfView( theFieldOfView ),
                                    gridResolutionInUmX(
                                      ( float )theGridResolutionInUm.x ),
                                    gridResolutionInUmY(
                                      ( float )theGridResolutionInUm.y ),
                                    gridResolutionInUmZ(
                                      ( float )theGridResolutionInUm.z ),
                                    atomLutSizeX( theAtomLutSizeX ),
                                    atomLutSizeY( theAtomLutSizeY ),
                                    atomLutSizeZ( theAtomLutSizeZ ),
                                    atomLutSizeXY( theAtomLutSizeXY ),
                                    atomLutSizeXYZ( theAtomLutSizeXYZ ),
                                    maximumAtomCountPerGridVoxel(
                                               theMaximumAtomCountPerGridVoxel )
{

#ifndef __NVCC__

  try
  {

#endif

    fieldOfViewLowerX = fieldOfView.getLowerX();
    fieldOfViewLowerY = fieldOfView.getLowerY();
    fieldOfViewLowerZ = fieldOfView.getLowerZ();

    fieldOfViewSpanX = fieldOfView.getSpanX();
    fieldOfViewSpanY = fieldOfView.getSpanY();
    fieldOfViewSpanZ = fieldOfView.getSpanZ();

#ifndef __NVCC__

  }
  GKG_CATCH( "inline "
             "gkg::VirtualTissueKokkosContainer:: "
             "VirtualTissueKokkosContainer( "
             "int32_t theAtomCount, "
             "UInt8VectorView theAtomTypes, "
             "BoolVectorView theAtomCompressedRepresentations, "
             "FloatVectorView theAtomParameters, "
             "UInt64VectorView theAtomParameterOffsets, "
             "Int32VectorView theAtomPopulationIds, "
             "Int32VectorView theAtomCellIds, "
             "Int32VectorView theAtomCountPerGridVoxel, "
             "Int32VectorView theAtomOffsetPerGridVoxel, "
             "Int32VectorView theAtomIndexLut, "
             "const gkg::BoundingBox< float >& theFieldOfView, "
             "const Vector3d< double >& theGridResolutionInUm, "
             "int32_t theAtomLutSizeX, "
             "int32_t theAtomLutSizeY, "
             "int32_t theAtomLutSizeZ, "
             "int32_t theAtomLutSizeXY, "
             "int32_t theAtomLutSizeXYZ, "
             "int32_t theMaximumAtomCountPerGridVoxel )" );

#endif

}


inline
gkg::VirtualTissueKokkosContainer::~VirtualTissueKokkosContainer()
{
}


KOKKOS_INLINE_FUNCTION
void gkg::VirtualTissueKokkosContainer::getAtomLutIndices(
                                                          float coordinateX,
                                                          float coordinateY,
                                                          float coordinateZ,
                                                          int* atomIndices,
                                                          int& atomCount ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t atomLutX = std::min( 
                         std::max( 
                           ( int32_t )( coordinateX /
                                        gridResolutionInUmX ),
                           ( int32_t )0 ),
                         atomLutSizeX - 1 );
    int32_t atomLutY = std::min(
                         std::max( 
                           ( int32_t )( coordinateY /
                                        gridResolutionInUmY ),
                           ( int32_t )0 ),
                         atomLutSizeY - 1 );
    int32_t atomLutZ = std::min( 
                         std::max( 
                           ( int32_t )( coordinateZ /
                                        gridResolutionInUmZ ),
                           ( int32_t )0 ),
                         atomLutSizeZ - 1 );

    int32_t lutVoxel = atomLutX + 
                       atomLutY * atomLutSizeX + 
                       atomLutZ * atomLutSizeXY;

    int32_t lutOffset = atomOffsetPerGridVoxel( lutVoxel );

    atomCount = atomCountPerGridVoxel( lutVoxel );

    int32_t index = 0;
    for ( index = 0; index < atomCount; index++ )
    {

      atomIndices[ index ] = atomIndexLut( lutOffset + index );

    }


#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::VirtualTissueKokkosContainer::"
             "getAtomLUTIndices( "
             "float coordinateX, "
             "float coordinateY, "
             "float coordinateZ, "
             "int* atomIndices, "
             "int& atomCount ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
bool gkg::VirtualTissueKokkosContainer::belongToAtom( float coordinateX,
                                                      float coordinateY,
                                                      float coordinateZ,
                                                      int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    return ( *gkg::AtomMethodFactory::getInstance().getBelongToMethod(
               atomTypes( atomIndex ) ) )(
                 &atomParameters( atomParameterOffsets( atomIndex ) ),
                 atomCompressedRepresentations( atomIndex ),
                 coordinateX,
                 coordinateY,
                 coordinateZ );                 

#ifndef __NVCC__

  }
  GKG_CATCH( "bool gkg::VirtualTissueKokkosContainer::"
             "belongToAtom( "
             "float coordinateX, "
             "float coordinateY, "
             "float coordinateZ, "
             "int32_t atomIndex ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
bool gkg::VirtualTissueKokkosContainer::inBox( const float& coordinateX,
                                               const float& coordinateY,
                                               const float& coordinateZ ) const
{

#ifndef __NVCC__

  try
  {

#endif

    if ( ( coordinateX < 0.0 ) ||
         ( coordinateY < 0.0 ) ||
         ( coordinateZ < 0.0 ) ||
         ( coordinateX > fieldOfViewSpanX ) ||
         ( coordinateY > fieldOfViewSpanY ) ||
         ( coordinateZ > fieldOfViewSpanZ ) )
    {

      return false;

    }
    return true;

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::VirtualTissueKokkosContainer::inBox( "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
float gkg::VirtualTissueKokkosContainer::getDistanceToMembrane(
                                                       float coordinateX,
                                                       float coordinateY,
                                                       float coordinateZ,
                                                       int32_t atomIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    return ( *gkg::AtomMethodFactory::getInstance().
               getDistanceToMembraneMethod(
                 atomTypes( atomIndex ) ) )(
                   &atomParameters( atomParameterOffsets( atomIndex ) ),
                   atomCompressedRepresentations( atomIndex ),
                   coordinateX,
                   coordinateY,
                   coordinateZ );

#ifndef __NVCC__

  }
  GKG_CATCH( "float gkg::VirtualTissueKokkosContainer::getDistanceToMembrane( "
             "float coordinateX, "
             "float coordinateY, "
             "float coordinateZ, "
             "int32_t atomIndex ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
void gkg::VirtualTissueKokkosContainer::getElasticBounceDirection(
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
                                  float& reflectedDisplacementDirectionZ ) const
{

#ifndef __NVCC__

  try
  {

#endif

    float normalX = 0.0f;
    float normalY = 0.0f;
    float normalZ = 0.0f;
    ( *gkg::AtomMethodFactory::getInstance().
               getOutwardNormalToMembraneMethod(
                 atomTypes( atomIndex ) ) )(
                   &atomParameters( atomParameterOffsets( atomIndex ) ),
                   atomCompressedRepresentations( atomIndex ),
                   particleCoordinateX,
                   particleCoordinateY,
                   particleCoordinateZ,
                   normalX,
                   normalY,
                   normalZ );

    if ( insideAtom )
    {

      normalX = -normalX;
      normalY = -normalY;
      normalZ = -normalZ;

    }

    float twiceDisplacementDirectionDotNormal =
                                  2.0f * ( displacementDirectionX * normalX +
                                           displacementDirectionY * normalY +
                                           displacementDirectionZ * normalZ );

    reflectedDisplacementDirectionX =
         displacementDirectionX - twiceDisplacementDirectionDotNormal * normalX;
    reflectedDisplacementDirectionY =
         displacementDirectionY - twiceDisplacementDirectionDotNormal * normalY;
    reflectedDisplacementDirectionZ =
         displacementDirectionZ - twiceDisplacementDirectionDotNormal * normalZ;

    float reflectedDisplacementNorm = std::sqrt(
                                            reflectedDisplacementDirectionX *
                                            reflectedDisplacementDirectionX +
                                            reflectedDisplacementDirectionY *
                                            reflectedDisplacementDirectionY +
                                            reflectedDisplacementDirectionZ *
                                            reflectedDisplacementDirectionZ );

    reflectedDisplacementDirectionX /= reflectedDisplacementNorm;
    reflectedDisplacementDirectionY /= reflectedDisplacementNorm;
    reflectedDisplacementDirectionZ /= reflectedDisplacementNorm;

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::VirtualTissueKokkosContainer::"
             "getElasticBounceDirection( "
             "float particleCoordinateX, "
             "float particleCoordinateY, "
             "float particleCoordinateZ, "
             "float displacementDirectionX, "
             "float displacementDirectionY, "
             "float displacementDirectionZ, "
             "bool insideAtom, "
             "int32_t atomIndex, "
             "float& reflectedDisplacementDirectionX, "
             "float& reflectedDisplacementDirectionY, "
             "float& reflectedDisplacementDirectionZ ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
bool gkg::VirtualTissueKokkosContainer::isRayCollidingWithAtom(
                                                float particleCoordinateX,
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
                                                float& collisionDistance ) const
{

#ifndef __NVCC__

  try
  {

#endif

    return ( *gkg::AtomMethodFactory::getInstance().
               getIsRayCollidingWithAtomMethod(
                 atomTypes( atomIndex ) ) )(
                   &atomParameters( atomParameterOffsets( atomIndex ) ),
                   atomCompressedRepresentations( atomIndex ),
                   particleCoordinateX,
                   particleCoordinateY,
                   particleCoordinateZ,
                   stepX,
                   stepY,
                   stepZ,
                   insideAtom,
                   collisionCoordinateX,
                   collisionCoordinateY,
                   collisionCoordinateZ,
                   collisionDistance );

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::VirtualTissueKokkosContainer::"
             "isRayCollidingWithAtom( "
             "float particleCoordinateX, "
             "float particleCoordinateY, "
             "float particleCoordinateZ, "
             "float stepX, "
             "float stepY, "
             "float stepZ, "
             "bool insideAtom, "
             "int32_t atomIndex, "
             "float& collisionCoordinateX, "
             "float& collisionCoordinateY, "
             "float& collisionCoordinateZ, "
             "float& collisionDistance ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct DiffusionProcessParticleInitializer
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::DiffusionProcessParticleInitializer::DiffusionProcessParticleInitializer(
                         gkg::DiffusionProcessParticleInitializer::Type theType,
                         gkg::RCPointer< gkg::VirtualTissueKokkosContainer >
                                                theVirtualTissueKokkosContainer,
                         Float3VectorView theParticleCoordinates,
                         Int32VectorView theParticleIds,
                         Kokkos::Random_XorShift64_Pool<
                                   Kokkos::DefaultExecutionSpace > theRandPool )
                                         : type( theType ),
                                           virtualTissueKokkosContainer(
                                              theVirtualTissueKokkosContainer ),
                                           particleCoordinates(
                                                       theParticleCoordinates ),
                                           particleIds( theParticleIds ),
                                           randPool( theRandPool )
{
}


inline
gkg::DiffusionProcessParticleInitializer::~DiffusionProcessParticleInitializer()
{
}


//
// particleId = -1 -> extra
//            = >0 -> intra
//

KOKKOS_INLINE_FUNCTION
void gkg::DiffusionProcessParticleInitializer::operator()( 
                                                   int32_t particleIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    // get a random number state from the pool for the active thread
    Kokkos::Random_XorShift64_Pool<
                                 Kokkos::DefaultExecutionSpace >::generator_type
      randomGenerator = randPool.get_state();

    float& coordinateX = particleCoordinates( particleIndex, 0 );
    float& coordinateY = particleCoordinates( particleIndex, 1 );
    float& coordinateZ = particleCoordinates( particleIndex, 2 );
    int32_t& particleId = particleIds( particleIndex );

    if ( type == gkg::DiffusionProcessParticleInitializer::Everywhere )
    {

      // randomly picking new coordinates
      coordinateX = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanX );
      coordinateY = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanY );
      coordinateZ = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanZ );

      // allocating array of atom indices
      int32_t atomCount = 0;
      int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

      // collecting the atoms located close to the particle
      virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                       coordinateY,
                                                       coordinateZ,
                                                       atomIndices,
                                                       atomCount );

      // checking whether the particle lies within one of the atom(s)
      particleId = -1;
      int32_t atomIndex = 0;
      for ( atomIndex = 0;
            atomIndex < atomCount;
            atomIndex++ )
      {

        if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
        {

          particleId = atomIndices[ atomIndex ];
          break;

        }

      }

      delete [] atomIndices;

    }
    else if ( type ==
                   gkg::DiffusionProcessParticleInitializer::OnlyIntracellular )
    {

      // looping while the chosen particle is not intracellular
      do
      {

        // randomly picking new coordinates
        coordinateX = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanX );
        coordinateY = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanY );
        coordinateZ = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanZ );

        // allocating array of atom indices
        int32_t atomCount = 0;
        int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

        // collecting the atoms located close to the particle
        virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                         coordinateY,
                                                         coordinateZ,
                                                         atomIndices,
                                                         atomCount );

        // checking whether the particle lies within one of the atom(s)
        particleId = -1;
        int32_t atomIndex = 0;
        for ( atomIndex = 0;
              atomIndex < atomCount;
              atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            particleId = atomIndices[ atomIndex ];
            break;

          }

        }

        delete [] atomIndices;

      }
      while ( particleId == -1 );

    }
    else if ( type ==
                   gkg::DiffusionProcessParticleInitializer::OnlyExtracellular )
    {

      // looping while the chosen particle is not extracellular
      do
      {

        // randomly picking new coordinates
        coordinateX = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanX );
        coordinateY = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanY );
        coordinateZ = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanZ );

        // allocating array of atom indices
        int32_t atomCount = 0;
        int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

        // collecting the atoms located close to the particle
        virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                         coordinateY,
                                                         coordinateZ,
                                                         atomIndices,
                                                         atomCount );

        // checking whether the particle lies within one of the atom(s)
        particleId = -1;
        int32_t atomIndex = 0;
        for ( atomIndex = 0;
              atomIndex < atomCount;
              atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            particleId = atomIndices[ atomIndex ];
            break;

          }

        }

        delete [] atomIndices;

      }
      while ( particleId != -1 );

    }
    else if ( type ==
     gkg::DiffusionProcessParticleInitializer::AllParticlesAtFieldOfViewCenter )
    {

      // setting coordinates to the field of view center
      coordinateX = virtualTissueKokkosContainer->fieldOfViewSpanX / 2.0f;
      coordinateY = virtualTissueKokkosContainer->fieldOfViewSpanY / 2.0f;
      coordinateZ = virtualTissueKokkosContainer->fieldOfViewSpanZ / 2.0f;

    }
    else if ( type ==
     gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredXYPlane )
    {

      do
      {

        // setting coordinates to the XY Plane at the field the view center
        coordinateX = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanX );
        coordinateY = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanY );
        coordinateZ = virtualTissueKokkosContainer->fieldOfViewSpanZ / 2.0f;

        // allocating array of atom indices
        int32_t atomCount = 0;
        int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

        // collecting the atoms located close to the particle
        virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                         coordinateY,
                                                         coordinateZ,
                                                         atomIndices,
                                                         atomCount );

        // checking whether the particle lies within one of the atom(s)
        particleId = -1;
        int32_t atomIndex = 0;
        for ( atomIndex = 0;
              atomIndex < atomCount;
              atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            particleId = atomIndices[ atomIndex ];
            break;

          }

        }

        delete [] atomIndices;

      }
      while ( particleId == -1 );

    }
    else if ( type ==
     gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredYZPlane )
    {

      do
      {

        // setting coordinates to the YZ Plane at the field the view center
        coordinateX = virtualTissueKokkosContainer->fieldOfViewSpanX / 2.0f;
        coordinateY = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanY );
        coordinateZ = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanZ );

        // allocating array of atom indices
        int32_t atomCount = 0;
        int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

        // collecting the atoms located close to the particle
        virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                         coordinateY,
                                                         coordinateZ,
                                                         atomIndices,
                                                         atomCount );

        // checking whether the particle lies within one of the atom(s)
        particleId = -1;
        int32_t atomIndex = 0;
        for ( atomIndex = 0;
              atomIndex < atomCount;
              atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            particleId = atomIndices[ atomIndex ];
            break;

          }

        }

        delete [] atomIndices;

      }
      while ( particleId == -1 );

    }
    else if ( type ==
     gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredXZPlane )
    {

      do
      {
        // setting coordinates to the XZ Plane at the field the view center
        coordinateX = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanX );
        coordinateY = virtualTissueKokkosContainer->fieldOfViewSpanY / 2.0f;
        coordinateZ = randomGenerator.frand(
                               virtualTissueKokkosContainer->fieldOfViewSpanZ );

        // allocating array of atom indices
        int32_t atomCount = 0;
        int32_t* atomIndices = new int32_t[ 
                   virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

        // collecting the atoms located close to the particle
        virtualTissueKokkosContainer->getAtomLutIndices( coordinateX,
                                                         coordinateY,
                                                         coordinateZ,
                                                         atomIndices,
                                                         atomCount );

        // checking whether the particle lies within one of the atom(s)
        particleId = -1;
        int32_t atomIndex = 0;
        for ( atomIndex = 0;
              atomIndex < atomCount;
              atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    coordinateX,
                                                    coordinateY,
                                                    coordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            particleId = atomIndices[ atomIndex ];
            break;

          }

        }

        delete [] atomIndices;

      }
      while ( particleId == -1 );

    }

    // give the state back, which will allow another thread to aquire it
    randPool.free_state( randomGenerator );

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::DiffusionProcessParticleInitializer::operator()( "
             "int32_t particleIndex ) const" );

#endif

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// struct DiffusionProcessMonteCarloAndMRISimulator
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline
gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                      DiffusionProcessMonteCarloAndMRISimulator(
      gkg::RCPointer< gkg::VirtualTissueKokkosContainer >
                                                theVirtualTissueKokkosContainer,
      FloatVectorView theCellPermeabilities,
      FloatVectorView theCellDiffusivitiesInUm2PerUs,
      FloatVectorView theCellStepLengths,
      FloatVectorView theCellTwiceStepLengthOverThreeDiffusivities,
      UInt64VectorView theAllPopulationsCellOffsets,
      float theParticleBulkDiffusivityInUm2PerUs,
      float theBulkStepLength,
      float theTwiceBulkStepLengthOverThreeBulkDiffusivity,
      gkg::DiffusionProcessMonteCarloAndMRISimulator::
         ParticleToMembraneInteractionType theParticleToMembraneInteractionType,
      int32_t theParticleCount,
      Float3VectorView theParticleCoordinates,
      Int32VectorView theParticleIds,
      Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace >
                                                                    theRandPool,
      // for diffusion MRI signal attenuation simulation
      int32_t theMriSequenceCount,
      int32_t theTotalDiffusionWeightedContrastCount,
      FloatVectorView theAllSequencesPhaseShifts,

      Int32VectorView thePerContrastMriSequenceIndices,
      Int32VectorView thePerSequenceTimeStepCounts,
      Int32VectorView thePerSequencePhaseShiftStartingIndices,
      Int32VectorView thePerSequenceDiffusionWeightedContrastCounts,
      Int32VectorView theFormerAccumulatedDiffusionWeightedContrastCounts,

      const std::vector< FloatVectorView >&
                                              theAllSequencesPhaseAccumulators )
            : virtualTissueKokkosContainer( theVirtualTissueKokkosContainer ),
              cellPermeabilities( theCellPermeabilities ),
              cellDiffusivitiesInUm2PerUs( theCellDiffusivitiesInUm2PerUs ),
              cellStepLengths( theCellStepLengths ),
              cellTwiceStepLengthOverThreeDiffusivities(
                                 theCellTwiceStepLengthOverThreeDiffusivities ),
              allPopulationsCellOffsets( theAllPopulationsCellOffsets ), 
              particleBulkDiffusivityInUm2PerUs(
                                         theParticleBulkDiffusivityInUm2PerUs ),
              bulkStepLength( theBulkStepLength ),
              twiceBulkStepLengthOverThreeBulkDiffusivity(
                               theTwiceBulkStepLengthOverThreeBulkDiffusivity ),
              particleToMembraneInteractionType(
                                         theParticleToMembraneInteractionType ),
              particleCount( theParticleCount ),
              particleCoordinates( theParticleCoordinates ),
              particleIds( theParticleIds ),
              randPool( theRandPool ),
              mriSequenceCount( theMriSequenceCount ),
              totalDiffusionWeightedContrastCount(
                                       theTotalDiffusionWeightedContrastCount ),
              allSequencesPhaseShifts( theAllSequencesPhaseShifts ),

              perContrastMriSequenceIndices( thePerContrastMriSequenceIndices ),
              perSequenceTimeStepCounts( thePerSequenceTimeStepCounts ),
              perSequencePhaseShiftStartingIndices(
                                      thePerSequencePhaseShiftStartingIndices ),
              perSequenceDiffusionWeightedContrastCounts(
                                thePerSequenceDiffusionWeightedContrastCounts ),
              formerAccumulatedDiffusionWeightedContrastCounts(
                          theFormerAccumulatedDiffusionWeightedContrastCounts ),
              allSequencesPhaseAccumulators( theAllSequencesPhaseAccumulators ),
              currentTimeStep( 0 )
{
}


inline
gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                    ~DiffusionProcessMonteCarloAndMRISimulator()
{
}


KOKKOS_INLINE_FUNCTION
void gkg::DiffusionProcessMonteCarloAndMRISimulator::resetTimeStep()
{

#ifndef __NVCC__

  try
  {

#endif

    currentTimeStep = 0;

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::DiffusionProcessMonteCarloAndMRISimulator::"
             "resetTimeStep()" );

#endif

}


KOKKOS_INLINE_FUNCTION
void gkg::DiffusionProcessMonteCarloAndMRISimulator::step()
{

#ifndef __NVCC__

  try
  {

#endif

    ++ currentTimeStep;

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::DiffusionProcessMonteCarloAndMRISimulator::step()" );

#endif

}


//
// particleId = -1 -> extra
//            >= 0 -> intra
//

KOKKOS_INLINE_FUNCTION
void gkg::DiffusionProcessMonteCarloAndMRISimulator::operator()( 
                                                   int32_t particleIndex ) const
{

#ifndef __NVCC__

  try
  {

#endif

    ////////////////////////////////////////////////////////////////////////////
    // first, trying to update the particle position
    ////////////////////////////////////////////////////////////////////////////


    float formerParticleCoordinateX = particleCoordinates( particleIndex, 0 );
    float formerParticleCoordinateY = particleCoordinates( particleIndex, 1 );
    float formerParticleCoordinateZ = particleCoordinates( particleIndex, 2 );

    float particleCoordinateX = formerParticleCoordinateX;
    float particleCoordinateY = formerParticleCoordinateY;
    float particleCoordinateZ = formerParticleCoordinateZ;
    int32_t& particleId = particleIds( particleIndex );

    // the step length and diffusivity depends whether we are extracellular or
    // within a cell
    float stepLength = bulkStepLength;
    float twiceStepLengthOverThreeDiffusivity = 
                                    twiceBulkStepLengthOverThreeBulkDiffusivity;

    // if the particle lies within a cell, then tuning the step length,
    // diffusivity accordingly
    if ( particleId >= 0 )
    {

      int32_t populationId = virtualTissueKokkosContainer->atomPopulationIds(
                                                                   particleId );
      int32_t cellId = virtualTissueKokkosContainer->atomCellIds( particleId );

      uint64_t allPopulationCellId = allPopulationsCellOffsets( populationId ) +
                                    cellId;

      stepLength = cellStepLengths( allPopulationCellId );
      twiceStepLengthOverThreeDiffusivity = 
               cellTwiceStepLengthOverThreeDiffusivities( allPopulationCellId );

    }


    bool isStepValid = true;

    // preallocating table of atom indices to speed-up method
    // size is twice the maximum atom count per LUT voxel because
    // the table will contain atoms from original and moved coordinates 
    int32_t* atomIndices = new int32_t[ 
               2 * virtualTissueKokkosContainer->maximumAtomCountPerGridVoxel ];

    float displacementDirectionX = 0.0f;
    float displacementDirectionY = 0.0f;
    float displacementDirectionZ = 0.0f;
    float displacementNorm = stepLength;
    float norm = 0.0f;
    int32_t iteration = 0;
    bool hasMembraneCollision = false;
    float crossingProbability = 0.0f;
    do
    {

      // get a random number state from the pool for the active thread
      Kokkos::Random_XorShift64_Pool<
                                 Kokkos::DefaultExecutionSpace >::generator_type
        randomGenerator = randPool.get_state();

      if ( isStepValid || !hasMembraneCollision )
      {

        displacementDirectionX = randomGenerator.normal( 0.0, stepLength );
        displacementDirectionY = randomGenerator.normal( 0.0, stepLength );
        displacementDirectionZ = randomGenerator.normal( 0.0, stepLength );

        // give the state back, which will allow another thread to aquire it
        randPool.free_state( randomGenerator );

        // dans le code de Kevin, il n'y a pas le sqrt .... a verifier
        norm = std::sqrt( displacementDirectionX *
                          displacementDirectionX +
                          displacementDirectionY *
                          displacementDirectionY +
                          displacementDirectionZ *
                          displacementDirectionZ );
        if ( norm > 0.0f )
        {

          displacementDirectionX /= norm;
          displacementDirectionY /= norm;
          displacementDirectionZ /= norm;

        }

      }

      crossingProbability = randomGenerator.frand();

      if ( particleToMembraneInteractionType ==
             gkg::DiffusionProcessMonteCarloAndMRISimulator::ElasticBouncing )
      {

        isStepValid = computeSubStepsWithElasticBouncing(
                                          particleCoordinateX,
                                          particleCoordinateY,
                                          particleCoordinateZ,
                                          particleId,
                                          displacementDirectionX,
                                          displacementDirectionY,
                                          displacementDirectionZ,
                                          displacementNorm,
                                          atomIndices,
                                          hasMembraneCollision,
                                          crossingProbability,
                                          twiceStepLengthOverThreeDiffusivity );

      }
      else
      {

        isStepValid = computeSubStepsWithEqualStepLengthRandomLeap(
                                          particleCoordinateX,
                                          particleCoordinateY,
                                          particleCoordinateZ,
                                          particleId,
                                          displacementDirectionX,
                                          displacementDirectionY,
                                          displacementDirectionZ,
                                          displacementNorm,
                                          atomIndices,
                                          crossingProbability,
                                          twiceStepLengthOverThreeDiffusivity );

      }
      ++ iteration;

    }
    while ( !isStepValid && ( iteration < 10 ) );


    // deleting preallocated table of atom indices
    delete [] atomIndices;

    // in case we get out of the box, we reset the particle coordinates to the
    // former ones
    if ( !virtualTissueKokkosContainer->inBox( particleCoordinateX,
                                               particleCoordinateY,
                                               particleCoordinateZ ) )
    {

      particleCoordinateX = formerParticleCoordinateX;
      particleCoordinateY = formerParticleCoordinateY;
      particleCoordinateZ = formerParticleCoordinateZ;

    }

    // setting the new coordinates if we stay within the box
    particleCoordinates( particleIndex, 0 ) = particleCoordinateX;
    particleCoordinates( particleIndex, 1 ) = particleCoordinateY;
    particleCoordinates( particleIndex, 2 ) = particleCoordinateZ;


    ////////////////////////////////////////////////////////////////////////////
    // second, updating the phase accumulator(s) for all sequence(s) accordingly
    ////////////////////////////////////////////////////////////////////////////

    float* allSequencesPhaseAccumulatorPtr = 
             &( allSequencesPhaseAccumulators[
                                             ( uint64_t )particleIndex ]( 0 ) );
    const float* allSequencesPhaseShiftPtr = 0;
    const int32_t* perContrastMriSequenceIndicesPtr =
      &perContrastMriSequenceIndices( 0 );

    int32_t diffusionWeightedContrastIndex = 0;
    uint64_t phaseShiftIndex = 0;
    for ( diffusionWeightedContrastIndex = 0;
          diffusionWeightedContrastIndex < totalDiffusionWeightedContrastCount;
          diffusionWeightedContrastIndex++ )
    {

      if ( currentTimeStep < perSequenceTimeStepCounts( 
                                           *perContrastMriSequenceIndicesPtr ) )
      {

        phaseShiftIndex = getPhaseShiftIndex( diffusionWeightedContrastIndex,
                                              currentTimeStep );
        
        allSequencesPhaseShiftPtr = &allSequencesPhaseShifts( phaseShiftIndex );

        *allSequencesPhaseAccumulatorPtr +=
          *allSequencesPhaseShiftPtr * particleCoordinateX +
          *( allSequencesPhaseShiftPtr + 1U ) * particleCoordinateY +
          *( allSequencesPhaseShiftPtr + 2U ) * particleCoordinateZ;


      }
      ++ allSequencesPhaseAccumulatorPtr;
      ++ perContrastMriSequenceIndicesPtr;

    }

#ifndef __NVCC__

  }
  GKG_CATCH( "void gkg::DiffusionProcessMonteCarloAndMRISimulator::operator()( "
             "int32_t particleIndex ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
uint64_t gkg::DiffusionProcessMonteCarloAndMRISimulator::getPhaseShiftIndex(
                                         int32_t diffusionWeightedContrastIndex,
                                         int32_t currentTimeStep ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t currentMriSequenceIndex = perContrastMriSequenceIndices(
                                               diffusionWeightedContrastIndex );
    int32_t forCurrentSequencePhaseShiftStartingIndex = 
                perSequencePhaseShiftStartingIndices( currentMriSequenceIndex );
    int32_t forCurrentSequenceDiffusionWeightedContrastCount =
                            perSequenceDiffusionWeightedContrastCounts(
                                                      currentMriSequenceIndex );
    int32_t forCurrentSequenceContrastIndex =
              diffusionWeightedContrastIndex - 
              formerAccumulatedDiffusionWeightedContrastCounts(
                                                      currentMriSequenceIndex );

    uint64_t forCurrentSequencePhaseShiftOffset =
             ( ( uint64_t )currentTimeStep * 
               ( uint64_t )forCurrentSequenceDiffusionWeightedContrastCount +
               ( uint64_t )forCurrentSequenceContrastIndex ) * 3U;

    return forCurrentSequencePhaseShiftStartingIndex +
           forCurrentSequencePhaseShiftOffset;

#ifndef __NVCC__

  }
  GKG_CATCH( "uint64_t gkg::DiffusionProcessMonteCarloAndMRISimulator::"
             "getPhaseShiftIndex( "
             "int32_t diffusionWeightedContrastIndex "
             "int32_t currentTimeStep ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
bool gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                             computeSubStepsWithElasticBouncing(
                              float& particleCoordinateX,
                              float& particleCoordinateY,
                              float& particleCoordinateZ,
                              int32_t& particleId,
                              float& displacementDirectionX,
                              float& displacementDirectionY,
                              float& displacementDirectionZ,
                              float& displacementNorm,
                              int32_t* atomIndices,
                              bool& hasMembraneCollision,
                              float crossingProbability,
                              float twiceStepLengthOverThreeDiffusivity ) const
{

#ifndef __NVCC__

  try
  {

#endif

    hasMembraneCollision = false;

    int32_t originalParticleId = particleId;
    int32_t updatedParticleId = particleId;

    float stepX = displacementDirectionX * displacementNorm;
    float stepY = displacementDirectionY * displacementNorm;
    float stepZ = displacementDirectionZ * displacementNorm;

    float updateParticleCoordinateX = particleCoordinateX + stepX;
    float updateParticleCoordinateY = particleCoordinateY + stepY;
    float updateParticleCoordinateZ = particleCoordinateZ + stepZ;


    // if we go out of the virtual tissue box, it is not a valid step
    if ( !virtualTissueKokkosContainer->inBox( updateParticleCoordinateX,
                                               updateParticleCoordinateY,
                                               updateParticleCoordinateZ ) )
    {

      // in this case, we did not have any collision
      hasMembraneCollision = false;
      return false;

    }

    // allocating array of atom indices
    int32_t originalParticleAtomCount = 0;
    virtualTissueKokkosContainer->getAtomLutIndices(
                                                    particleCoordinateX,
                                                    particleCoordinateY,
                                                    particleCoordinateZ,
                                                    atomIndices,
                                                    originalParticleAtomCount );

    int32_t updatedParticleAtomCount = 0;
    virtualTissueKokkosContainer->getAtomLutIndices(
                                        updateParticleCoordinateX,
                                        updateParticleCoordinateY,
                                        updateParticleCoordinateZ,
                                        atomIndices + originalParticleAtomCount,
                                        updatedParticleAtomCount );

    int32_t atomCount = originalParticleAtomCount +
                        updatedParticleAtomCount;


    ////////////////////////////////////////////////////////////////////////////
    // in case we were in the extracellular space
    ////////////////////////////////////////////////////////////////////////////
    if ( originalParticleId == -1 )
    {

      float collisionCoordinateX = 0.0f;
      float collisionCoordinateY = 0.0f;
      float collisionCoordinateZ = 0.0f;
      float collisionDistance = 0.0f;

      float membraneCoordinateX = 0.0f;
      float membraneCoordinateY = 0.0f;
      float membraneCoordinateZ = 0.0f;

      float reflectedDisplacementDirectionX = 0.0f;
      float reflectedDisplacementDirectionY = 0.0f;
      float reflectedDisplacementDirectionZ = 0.0f;

      float distanceToMembrane = std::numeric_limits<float>::infinity();

      int32_t atomIndex = 0;
      for ( atomIndex = 0; atomIndex < atomCount; atomIndex++ )
      {

        if( virtualTissueKokkosContainer->isRayCollidingWithAtom(
                                                particleCoordinateX,
                                                particleCoordinateY,
                                                particleCoordinateZ,
                                                stepX,
                                                stepY,
                                                stepZ,
                                                false,
                                                atomIndices[ atomIndex ],
                                                collisionCoordinateX,
                                                collisionCoordinateY,
                                                collisionCoordinateZ,
                                                collisionDistance ) )
        {

          if ( collisionDistance < distanceToMembrane )
          {

            membraneCoordinateX = collisionCoordinateX;
            membraneCoordinateY = collisionCoordinateY;
            membraneCoordinateZ = collisionCoordinateZ;
            distanceToMembrane = collisionDistance;
            updatedParticleId = atomIndices[ atomIndex ];
            hasMembraneCollision = true;

          }

        }

      }


      // in case there are no collision
      if ( !hasMembraneCollision )
      {

        particleCoordinateX = updateParticleCoordinateX;
        particleCoordinateY = updateParticleCoordinateY;
        particleCoordinateZ = updateParticleCoordinateZ;

        // redundant but for the sake of clarity
        particleId = -1;

        return true;

      }
      // in case the particle is colliding with a membrane
      else
      {

        // if we are at this point, it means that a collision occurred
        particleCoordinateX = membraneCoordinateX;
        particleCoordinateY = membraneCoordinateY;
        particleCoordinateZ = membraneCoordinateZ;

        displacementNorm -= distanceToMembrane;
        float oneThousandthDisplacementNorm = 0.001f * displacementNorm;

        float cellPermeability = 0.0f;


        if ( updatedParticleId > 0 )
        {

          int32_t populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                            updatedParticleId );
          int32_t cellId = virtualTissueKokkosContainer->atomCellIds(
                                                            updatedParticleId );

          uint64_t allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                         cellId;
          cellPermeability = cellPermeabilities( allPopulationCellId );

        }

        bool hasPermeability = ( cellPermeability > 0.0f ? true : false );

        if ( hasPermeability )
        {

          float transmissionProbability = twiceStepLengthOverThreeDiffusivity *
                                          cellPermeability;

          bool membraneCrossingAccepted = 
           ( ( transmissionProbability > crossingProbability ) ? true : false );

          if ( membraneCrossingAccepted )
          {

            particleCoordinateX += oneThousandthDisplacementNorm *
                                   displacementDirectionX;
            particleCoordinateY += oneThousandthDisplacementNorm *
                                   displacementDirectionY;
            particleCoordinateZ += oneThousandthDisplacementNorm *
                                   displacementDirectionZ;

            particleId = updatedParticleId;

            return false;

          }
          else
          {

            // computing the elastic reflection
            virtualTissueKokkosContainer->getElasticBounceDirection(
                                            particleCoordinateX,
                                            particleCoordinateY,
                                            particleCoordinateZ,
                                            displacementDirectionX,
                                            displacementDirectionY,
                                            displacementDirectionZ,
                                            false,
                                            updatedParticleId,
                                            reflectedDisplacementDirectionX,
                                            reflectedDisplacementDirectionY,
                                            reflectedDisplacementDirectionZ );
            particleCoordinateX -= oneThousandthDisplacementNorm *
                                   displacementDirectionX;
            particleCoordinateY -= oneThousandthDisplacementNorm * 
                                   displacementDirectionY;
            particleCoordinateZ -= oneThousandthDisplacementNorm * 
                                   displacementDirectionZ;

            displacementDirectionX = reflectedDisplacementDirectionX;
            displacementDirectionY = reflectedDisplacementDirectionY;
            displacementDirectionZ = reflectedDisplacementDirectionZ;

            // redundant but for the sake of clarity
            particleId = -1;

            return false;

          }


        }
        // if we don't have permeability, just applying elastic bounding
        else
        {

          // computing the elastic reflection
          virtualTissueKokkosContainer->getElasticBounceDirection(
                                          particleCoordinateX,
                                          particleCoordinateY,
                                          particleCoordinateZ,
                                          displacementDirectionX,
                                          displacementDirectionY,
                                          displacementDirectionZ,
                                          false,
                                          updatedParticleId,
                                          reflectedDisplacementDirectionX,
                                          reflectedDisplacementDirectionY,
                                          reflectedDisplacementDirectionZ );

          particleCoordinateX -= oneThousandthDisplacementNorm * 
                                 displacementDirectionX;
          particleCoordinateY -= oneThousandthDisplacementNorm * 
                                 displacementDirectionY;
          particleCoordinateZ -= oneThousandthDisplacementNorm * 
                                 displacementDirectionZ;

          displacementDirectionX = reflectedDisplacementDirectionX;
          displacementDirectionY = reflectedDisplacementDirectionY;
          displacementDirectionZ = reflectedDisplacementDirectionZ;

          // redundant but for the sake of clarity
          particleId = -1;

          return false;

        }

      }

    }
    ////////////////////////////////////////////////////////////////////////////
    // in case we were in the intracellular space
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      // in most cases, the particle remains inside the atom, so it is better
      // first to check this case to speed-up the computation and not impose
      // the use of a further loop over atom indices

      if ( virtualTissueKokkosContainer->belongToAtom(
                                                    updateParticleCoordinateX,
                                                    updateParticleCoordinateY,
                                                    updateParticleCoordinateZ,
                                                    originalParticleId ) )
      {

        hasMembraneCollision = false;
        particleCoordinateX = updateParticleCoordinateX;
        particleCoordinateY = updateParticleCoordinateY;
        particleCoordinateZ = updateParticleCoordinateZ;
        particleId = originalParticleId;

        return true;

      }
      // in case of putative collisions, we have to do something more complex
      else
      {

        float collisionCoordinateX = 0.0f;
        float collisionCoordinateY = 0.0f;
        float collisionCoordinateZ = 0.0f;
        float collisionDistance = 0.0f;

        float membraneCoordinateX = 0.0f;
        float membraneCoordinateY = 0.0f;
        float membraneCoordinateZ = 0.0f;
        float distanceToMembrane = 0.0f;

        float reflectedDisplacementDirectionX = 0.0f;
        float reflectedDisplacementDirectionY = 0.0f;
        float reflectedDisplacementDirectionZ = 0.0f;

        int32_t atomIndex = 0;
        for ( atomIndex = 0; atomIndex < atomCount; atomIndex++ )
        {

          // first case: we stay in the same cell with the atom of index
          // 'atomIndex'
          if ( ( virtualTissueKokkosContainer->atomPopulationIds( 
                                                atomIndices[ atomIndex ] ) ==
                 virtualTissueKokkosContainer->atomPopulationIds( 
                                                             particleId ) ) &&
               ( virtualTissueKokkosContainer->atomCellIds( 
                                                atomIndices[ atomIndex ] ) ==
                 virtualTissueKokkosContainer->atomCellIds( particleId ) ) )
          {

            // then we check that we effectively belong to the atom of index 
            // 'atomIndex'
            if ( virtualTissueKokkosContainer->belongToAtom(
                                                  updateParticleCoordinateX,
                                                  updateParticleCoordinateY,
                                                  updateParticleCoordinateZ,
                                                  atomIndices[ atomIndex ] ) )
            {

              hasMembraneCollision = false;

              particleCoordinateX = updateParticleCoordinateX;
              particleCoordinateY = updateParticleCoordinateY;
              particleCoordinateZ = updateParticleCoordinateZ;
              particleId = atomIndices[ atomIndex ];

              return true;

            }
            // if the particle does not belong to the atom of index 'atomIndex'
            // then we check if there exists a putative collision
            else if ( virtualTissueKokkosContainer->isRayCollidingWithAtom(
                                                  particleCoordinateX,
                                                  particleCoordinateY,
                                                  particleCoordinateZ,
                                                  stepX,
                                                  stepY,
                                                  stepZ,
                                                  true,
                                                  atomIndices[ atomIndex ],
                                                  collisionCoordinateX,
                                                  collisionCoordinateY,
                                                  collisionCoordinateZ,
                                                  collisionDistance ) )
            {

              if ( collisionDistance > distanceToMembrane )
              {

                membraneCoordinateX = collisionCoordinateX;
                membraneCoordinateY = collisionCoordinateY;
                membraneCoordinateZ = collisionCoordinateZ;
                distanceToMembrane = collisionDistance;
                updatedParticleId = atomIndices[ atomIndex ];

              }

            }

          }

        }

        // if we are at this point, it means that a collision occurred
        hasMembraneCollision = true;
        particleCoordinateX = membraneCoordinateX;
        particleCoordinateY = membraneCoordinateY;
        particleCoordinateZ = membraneCoordinateZ;

        displacementNorm -= distanceToMembrane;
        float oneThousandthDisplacementNorm = 0.001f * displacementNorm;

        particleId = updatedParticleId;

        float cellPermeability = 0.0f;
        if ( particleId > 0 )
        {

          int32_t populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                            particleId );
          int32_t cellId = virtualTissueKokkosContainer->atomCellIds(
                                                            particleId );

          uint64_t allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                         cellId;
          cellPermeability = cellPermeabilities( allPopulationCellId );

        }

        bool hasPermeability = ( cellPermeability > 0.0f ? true : false );

        if ( hasPermeability )
        {

          float transmissionProbability = twiceStepLengthOverThreeDiffusivity *
                                          cellPermeability;

          bool membraneCrossingAccepted =
           ( ( transmissionProbability > crossingProbability ) ? true : false );

          if ( membraneCrossingAccepted )
          {

            particleCoordinateX += oneThousandthDisplacementNorm *
                                   displacementDirectionX;
            particleCoordinateY += oneThousandthDisplacementNorm *
                                   displacementDirectionY;
            particleCoordinateZ += oneThousandthDisplacementNorm *
                                   displacementDirectionZ;

            particleId = -1;

            return false;

          }
          else
          {

            // computing the elastic reflection
            virtualTissueKokkosContainer->getElasticBounceDirection(
                                            particleCoordinateX,
                                            particleCoordinateY,
                                            particleCoordinateZ,
                                            displacementDirectionX,
                                            displacementDirectionY,
                                            displacementDirectionZ,
                                            true,
                                            particleId,
                                            reflectedDisplacementDirectionX,
                                            reflectedDisplacementDirectionY,
                                            reflectedDisplacementDirectionZ );

          
            particleCoordinateX -= oneThousandthDisplacementNorm * 
                                   displacementDirectionX;
            particleCoordinateY -= oneThousandthDisplacementNorm * 
                                   displacementDirectionY;
            particleCoordinateZ -= oneThousandthDisplacementNorm * 
                                   displacementDirectionZ;


            displacementDirectionX = reflectedDisplacementDirectionX;
            displacementDirectionY = reflectedDisplacementDirectionY;
            displacementDirectionZ = reflectedDisplacementDirectionZ;

            return false;

          }

        }
        // if we don't have permeability, just applying elastic bounding
        else
        {

          // computing the elastic reflection
          virtualTissueKokkosContainer->getElasticBounceDirection(
                                            particleCoordinateX,
                                            particleCoordinateY,
                                            particleCoordinateZ,
                                            displacementDirectionX,
                                            displacementDirectionY,
                                            displacementDirectionZ,
                                            true,
                                            particleId,
                                            reflectedDisplacementDirectionX,
                                            reflectedDisplacementDirectionY,
                                            reflectedDisplacementDirectionZ );

        
          particleCoordinateX -= oneThousandthDisplacementNorm * 
                                 displacementDirectionX;
          particleCoordinateY -= oneThousandthDisplacementNorm * 
                                 displacementDirectionY;
          particleCoordinateZ -= oneThousandthDisplacementNorm * 
                                 displacementDirectionZ;


          displacementDirectionX = reflectedDisplacementDirectionX;
          displacementDirectionY = reflectedDisplacementDirectionY;
          displacementDirectionZ = reflectedDisplacementDirectionZ;

          return false;

        }

      }

    }
        
    return false;

#ifndef __NVCC__

  }
  GKG_CATCH( "bool gkg::DiffusionProcessMonteCarloAndMRISimulator::"
             "computeSubStepsWithElasticBouncing( "
             "float& particleCoordinateX, "
             "float& particleCoordinateY, "
             "float& particleCoordinateZ, "
             "int32_t& particleId, "
             "float& displacementDirectionX, "
             "float& displacementDirectionY, "
             "float& displacementDirectionZ, "
             "float& displacementNorm, "
             "int32_t* atomIndices, "
             "bool& hasMembraneCollision, "
             "float crossingProbability, "
             "float twiceStepLengthOverThreeDiffusivity ) const" );

#endif

}


KOKKOS_INLINE_FUNCTION
bool gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                   computeSubStepsWithEqualStepLengthRandomLeap(
                              float& particleCoordinateX,
                              float& particleCoordinateY,
                              float& particleCoordinateZ,
                              int32_t& particleId,
                              float& displacementDirectionX,
                              float& displacementDirectionY,
                              float& displacementDirectionZ,
                              float& displacementNorm,
                              int32_t* atomIndices,
                              float crossingProbability,
                              float twiceStepLengthOverThreeDiffusivity ) const
{

#ifndef __NVCC__

  try
  {

#endif

    int32_t originalParticleId = particleId;
    //int32_t updatedParticleId = particleId;

    float stepX = displacementDirectionX * displacementNorm;
    float stepY = displacementDirectionY * displacementNorm;
    float stepZ = displacementDirectionZ * displacementNorm;

    float updateParticleCoordinateX = particleCoordinateX + stepX;
    float updateParticleCoordinateY = particleCoordinateY + stepY;
    float updateParticleCoordinateZ = particleCoordinateZ + stepZ;

    // if we go out of the virtual tissue box, it is not a valid step
    if ( !virtualTissueKokkosContainer->inBox( updateParticleCoordinateX,
                                               updateParticleCoordinateY,
                                               updateParticleCoordinateZ ) )
    {

      return false;

    }

    // allocating array of atom indices
    int32_t originalParticleAtomCount = 0;
    virtualTissueKokkosContainer->getAtomLutIndices(
                                                    particleCoordinateX,
                                                    particleCoordinateY,
                                                    particleCoordinateZ,
                                                    atomIndices,
                                                    originalParticleAtomCount );

    int32_t updatedParticleAtomCount = 0;
    virtualTissueKokkosContainer->getAtomLutIndices(
                                        updateParticleCoordinateX,
                                        updateParticleCoordinateY,
                                        updateParticleCoordinateZ,
                                        atomIndices + originalParticleAtomCount,
                                        updatedParticleAtomCount );

    int32_t atomCount = originalParticleAtomCount +
                        updatedParticleAtomCount;


    ////////////////////////////////////////////////////////////////////////////
    // in case we were in the extracellular space
    //
    //  case 1:
    //                       +--+-+--+
    //       x--------->x    |  | |  |
    //                       |  | |  |
    //                       +--+-+--+
    //
    //  case 2:
    //                       +--+-+--+
    //       x---------------|>x| |  |
    //                       |  | |  |
    //                       +--+-+--+
    //
    ////////////////////////////////////////////////////////////////////////////
    if ( originalParticleId == -1 )
    {

      int32_t atomIndex = 0;
      for ( atomIndex = 0; atomIndex < atomCount; atomIndex++ )
      {

        // in case we are about to enter the intracellular space
        if ( virtualTissueKokkosContainer->belongToAtom(
                                                    updateParticleCoordinateX,
                                                    updateParticleCoordinateY,
                                                    updateParticleCoordinateZ,
                                                    atomIndices[ atomIndex ] ) )
        {

          float cellPermeability = 0.0f;
          int32_t populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                     atomIndices[ atomIndex ] );
          int32_t cellId = virtualTissueKokkosContainer->atomCellIds(
                                                     atomIndices[ atomIndex ] );

          uint64_t allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                         cellId;
          cellPermeability = cellPermeabilities( allPopulationCellId );

          // if cell is permeable
          if ( cellPermeability > 0.0f )
          {

            float transmissionProbability = 
                                           twiceStepLengthOverThreeDiffusivity *
                                           cellPermeability;
            if ( transmissionProbability > crossingProbability )
            {

              particleCoordinateX = updateParticleCoordinateX;
              particleCoordinateY = updateParticleCoordinateY;
              particleCoordinateZ = updateParticleCoordinateZ;
              particleId = atomIndices[ atomIndex ];

              return true;

            }
            else
            {

              return false;

            }

          }
          // if cell is impermeable
          else
          {

            return false;

          }

        }

      }
      // if we are there then we have stayed in the extracellular space
      particleCoordinateX = updateParticleCoordinateX;
      particleCoordinateY = updateParticleCoordinateY;
      particleCoordinateZ = updateParticleCoordinateZ;
      particleId = -1;

      return true;

    }
    ////////////////////////////////////////////////////////////////////////////
    // in case we were in the intracellular space
    //
    //  case 1:
    //           cell 1              cell 2
    //        +----+-+----+      +----+-+----+
    //        |x->x| |    |      |    | |    |
    //        |    | |    |      |    | |    |
    //        |    | |    |      |    | |    |
    //        +----+-+----+      +----+-+----+
    //
    //  case 2:
    //           cell 1              cell 2
    //        +----+-+----+      +----+-+----+
    //        |x---+-+->x |      |    | |    |
    //        |    | |    |      |    | |    |
    //        |    | |    |      |    | |    |
    //        +----+-+----+      +----+-+----+
    //
    //  case 3:
    //           cell 1              cell 2
    //        +----+-+----+      +----+-+----+
    //        |x---+-+----+------+->x | |    |
    //        |    | |    |      |    | |    |
    //        |    | |    |      |    | |    |
    //        +----+-+----+      +----+-+----+
    //
    //  case 4:
    //           cell 1              cell 2
    //        +----+-+----+      +----+-+----+
    //        |x---+-+----+-->x  |    | |    |
    //        |    | |    |      |    | |    |
    //        |    | |    |      |    | |    |
    //        +----+-+----+      +----+-+----+
    //
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      // in most cases, the particle remains inside the atom, so it is better
      // first to check this case to speed-up the computation and not impose
      // the use of a further loop over atom indices (case 1)

      if ( virtualTissueKokkosContainer->belongToAtom(
                                                    updateParticleCoordinateX,
                                                    updateParticleCoordinateY,
                                                    updateParticleCoordinateZ,
                                                    originalParticleId ) )
      {

        particleCoordinateX = updateParticleCoordinateX;
        particleCoordinateY = updateParticleCoordinateY;
        particleCoordinateZ = updateParticleCoordinateZ;
        particleId = originalParticleId;

        return true;

      }
      // in case of putative collisions, we have to do something more complex
      else
      {

        int32_t atomIndex = 0;
        for ( atomIndex = 0; atomIndex < atomCount; atomIndex++ )
        {

          if ( virtualTissueKokkosContainer->belongToAtom(
                                                    updateParticleCoordinateX,
                                                    updateParticleCoordinateY,
                                                    updateParticleCoordinateZ,
                                                    atomIndices[ atomIndex ] ) )
          {

            // if the particle is located in another atom of the same cell
            // (case 2)
            if ( ( virtualTissueKokkosContainer->atomPopulationIds( 
                                                   atomIndices[ atomIndex ] ) ==
                   virtualTissueKokkosContainer->atomPopulationIds( 
                                                               particleId ) ) &&
                 ( virtualTissueKokkosContainer->atomCellIds( 
                                                   atomIndices[ atomIndex ] ) ==
                   virtualTissueKokkosContainer->atomCellIds( particleId ) ) )
            {

              particleCoordinateX = updateParticleCoordinateX;
              particleCoordinateY = updateParticleCoordinateY;
              particleCoordinateZ = updateParticleCoordinateZ;
              particleId = atomIndices[ atomIndex ];

              return true;

            }
            // if the particle is located in another atom from another cell
            // (case 3)
            else
            {

              float cellPermeabilityOfOriginatingCell = 0.0f;
              int32_t populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                                   particleId );
              int32_t cellId = virtualTissueKokkosContainer->atomCellIds(
                                                                   particleId );

              uint64_t allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                             cellId;
              cellPermeabilityOfOriginatingCell =
                                      cellPermeabilities( allPopulationCellId );



              if ( cellPermeabilityOfOriginatingCell > 0.0f )
              {

                float transmissionProbabilityFromOriginatingCell =
                                           twiceStepLengthOverThreeDiffusivity *
                                           cellPermeabilityOfOriginatingCell;
                if ( transmissionProbabilityFromOriginatingCell >
                     crossingProbability )
                {


                  float cellPermeabilityOfTargetCell = 0.0f;
                  populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                     atomIndices[ atomIndex ] );
                  cellId = virtualTissueKokkosContainer->atomCellIds(
                                                     atomIndices[ atomIndex ] );

                  allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                        cellId;
                  cellPermeabilityOfTargetCell =
                                      cellPermeabilities( allPopulationCellId );


                  float transmissionProbabilityToTargetCell =
                                           twiceStepLengthOverThreeDiffusivity *
                                           cellPermeabilityOfTargetCell;

                  if ( transmissionProbabilityToTargetCell >
                       crossingProbability )
                  {
                   
                    particleCoordinateX = updateParticleCoordinateX;
                    particleCoordinateY = updateParticleCoordinateY;
                    particleCoordinateZ = updateParticleCoordinateZ;
                    particleId = atomIndices[ atomIndex ];

                    return true;

                  }
                  // we failed to cross the second cell membrane
                  else
                  {

                    return false;

                  }

                }
                // we failed to cross the first cell membrane
                else
                {

                  return false;

                }

              }
              // the originating cell is impermeable
              else
              {

                return false;

              }

            }

          }

        }

        // if we are there then we have moved to the extracellular space
        // (case 4)
        float cellPermeabilityOfOriginatingCell = 0.0f;

        int32_t populationId =
                         virtualTissueKokkosContainer->atomPopulationIds(
                                                                   particleId );
        int32_t cellId = virtualTissueKokkosContainer->atomCellIds(
                                                                   particleId );

        uint64_t allPopulationCellId = allPopulationsCellOffsets(
                                                                populationId ) +
                                       cellId;
        cellPermeabilityOfOriginatingCell =
                                      cellPermeabilities( allPopulationCellId );

        if ( cellPermeabilityOfOriginatingCell > 0.0f )
        {

          float transmissionProbabilityFromOriginatingCell =
                                           twiceStepLengthOverThreeDiffusivity *
                                           cellPermeabilityOfOriginatingCell;
          if ( transmissionProbabilityFromOriginatingCell >
               crossingProbability )
          {

                   
            particleCoordinateX = updateParticleCoordinateX;
            particleCoordinateY = updateParticleCoordinateY;
            particleCoordinateZ = updateParticleCoordinateZ;
            particleId = -1;

            return true;

          }
          // we failed to cross the cell membrane
          else
          {

            return false;

          }

        }

      }

    }
        
    return false;

#ifndef __NVCC__

  }
  GKG_CATCH( "bool gkg::DiffusionProcessMonteCarloAndMRISimulator::"
             "computeSubStepsWithEqualStepLengthRandomLeap( "
             "float& particleCoordinateX, "
             "float& particleCoordinateY, "
             "float& particleCoordinateZ, "
             "int32_t& particleId, "
             "float& displacementDirectionX, "
             "float& displacementDirectionY, "
             "float& displacementDirectionZ, "
             "float& displacementNorm, "
             "int32_t* atomIndices, "
             "float crossingProbability, "
             "float twiceStepLengthOverThreeDiffusivity ) const" );

#endif

}


#endif
