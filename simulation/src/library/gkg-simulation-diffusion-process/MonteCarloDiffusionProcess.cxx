#include <gkg-simulation-diffusion-process/MonteCarloDiffusionProcess.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-simulation-kokkos/Defines.h>
#include <gkg-simulation-kokkos/AtomLut_i.h>
#include <gkg-simulation-kokkos/DiffusionProcessKernels_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-communication-sysinfo/Timer.h>
#include <gkg-core-exception/Exception.h>
#include <list>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <complex>
#include <fstream>



gkg::MonteCarloDiffusionProcess::MonteCarloDiffusionProcess(
                             gkg::RCPointer< gkg::VirtualTissue > virtualTissue,
                             const gkg::Dictionary& dictionary )
                                : _virtualTissue( virtualTissue )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // particle initialization, one of everywhere, only-intracellular,
    // only-extracellular
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary::const_iterator 
      i = dictionary.find( "particle_initialization" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error(
                               "a 'particle_initialization' item is required" );

    }
    setParticleInitializationType( i->second->getString() );


    ////////////////////////////////////////////////////////////////////////////
    // particle to membrane interaction type, one of 
    // equal-step-length-random-leap, elastic-bouncing
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "particle_to_membrane_interaction_type" );
    if ( i == dictionary.end() )
    {

      _particleToMembraneInteractionType =
      gkg::DiffusionProcessMonteCarloAndMRISimulator::EqualStepLengthRandomLeap;


    }
    setParticleMembraneInteractionType( i->second->getString() );


    ////////////////////////////////////////////////////////////////////////////
    // collecting session count
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "session_count" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'session_count' item is required" );

    }
    _sessionCount = ( int32_t )( i->second->getScalar() + 0.5 );


    ////////////////////////////////////////////////////////////////////////////
    // collecting particle diffusivity in um2/us (typically around 2e-3 um2/us)
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "particle_bulk_diffusivity_in_um2_per_us" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error(
               "a 'particle_bulk_diffusivity_in_um2_per_us' item is required" );

    }
    _particleBulkDiffusivityInUm2PerUs = ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting particle count
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "particle_count_per_session" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error(
                            "a 'particle_count_per_session' item is required" );

    }
    _particleCountPerSession = ( int32_t )( i->second->getScalar() + 0.5 );


    ////////////////////////////////////////////////////////////////////////////
    // collecting grid resolution in um
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "grid_resolution_in_um" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'grid_resolution_in_um' item is required" );

    }
    gkg::GenericObjectList 
      golGridResolutionInUm = i->second->getValue< gkg::GenericObjectList >();

    if ( golGridResolutionInUm.size() != 3U )
    {

      throw std::runtime_error(
                            "'grid_resolution_in_um' field must have 3 items" );


    }
    _gridResolutionInUm.x = golGridResolutionInUm[ 0 ]->getScalar();
    _gridResolutionInUm.y = golGridResolutionInUm[ 1 ]->getScalar();
    _gridResolutionInUm.z = golGridResolutionInUm[ 2 ]->getScalar(); 


    ////////////////////////////////////////////////////////////////////////////
    // collecting time step in us
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "time_step_in_us" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'time_step_in_us' item is required" );

    }
    _timeStepInUs = ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting total duration in us
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "total_duration_in_us" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'total_duration_in_us' item is required" );

    }
    _totalDurationInUs = ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting particle positions backup percentage
    ////////////////////////////////////////////////////////////////////////////

    i = dictionary.find( "particle_backup_percentage" );
    if ( i == dictionary.end() )
    {

      _particleBackupPercentage = 100.0f;

    }
    else
    {

      _particleBackupPercentage = ( float )i->second->getScalar();
      if ( ( _particleBackupPercentage <= 0.0f ) ||
           ( _particleBackupPercentage > 100.0f ) )
      {

        throw std::runtime_error( 
                     "particle backup percentage must lie within ]0.0;100.0]" );

      }

      int32_t randomSelectionCount = 
                                ( int32_t )( ( float )_particleCountPerSession *
                                             _particleBackupPercentage /
                                             100.0f );
      _randomParticleSelection.resize( randomSelectionCount );
      gkg::getRandomIndexVector< int32_t >( _randomParticleSelection,
                                            _particleCountPerSession - 1 );

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing step length in um
    ////////////////////////////////////////////////////////////////////////////

    _bulkStepLength = ( float )std::sqrt(
                                      6.0 * _particleBulkDiffusivityInUm2PerUs *
                                      _timeStepInUs );


    ////////////////////////////////////////////////////////////////////////////
    // computing step count (rounded to the next integer)
    ////////////////////////////////////////////////////////////////////////////

    _stepCount = ( int32_t )( _totalDurationInUs / _timeStepInUs );
    if ( ( _totalDurationInUs / _timeStepInUs - ( float )_stepCount ) > 0.0f )
    {

      ++ _stepCount;

    }

  }
  GKG_CATCH( "gkg::MonteCarloDiffusionProcess::MonteCarloDiffusionProcess( "
             "gkg::RCPointer< gkg::VirtualTissue > virtualTissue, "
             "const gkg::Dictionary& dictionary )" );

}


gkg::MonteCarloDiffusionProcess::~MonteCarloDiffusionProcess()
{
}


float gkg::MonteCarloDiffusionProcess::getTimeStepInUs() const
{

  try
  {

    return _timeStepInUs;

  }
  GKG_CATCH( "float gkg::MonteCarloDiffusionProcess::getTimeStepInUs() const" );


}



void gkg::MonteCarloDiffusionProcess::simulate(
                  gkg::RCPointer< gkg::MRISequencePool > mriSequencePool,
                  const std::string& fileNameDiffusionProcessOutputParticles,
                  const std::string& fileNameDiffusionProcessOutputTrajectories,
                  std::vector< std::vector< float > >&
                                       perSequenceDiffusionWeightedAttenuations,
                  bool verbose,
                  std::ofstream* osLog ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // instaciating a timer to measure the computational efficacy of the algo
    ////////////////////////////////////////////////////////////////////////////

    gkg::Timer timer;
    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // computing atom LUT size
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "computing lookup-table of atom(s)",
                          verbose, osLog, true );

    float fieldOfViewLowerX = _virtualTissue->getFieldOfView().getLowerX();
    float fieldOfViewLowerY = _virtualTissue->getFieldOfView().getLowerY();
    float fieldOfViewLowerZ = _virtualTissue->getFieldOfView().getLowerZ();

    float fieldOfViewSpanX = _virtualTissue->getFieldOfView().getSpanX();
    float fieldOfViewSpanY = _virtualTissue->getFieldOfView().getSpanY();
    float fieldOfViewSpanZ = _virtualTissue->getFieldOfView().getSpanZ();

    int32_t atomLutSizeX = ( int32_t )( fieldOfViewSpanX /
                                        _gridResolutionInUm.x );
    if ( ( fieldOfViewSpanX / _gridResolutionInUm.x -
           atomLutSizeX ) > 0.0f )
    {

      ++ atomLutSizeX;

    }

    int32_t atomLutSizeY = ( int32_t )( fieldOfViewSpanY /
                                        _gridResolutionInUm.y );
    if ( ( fieldOfViewSpanY / _gridResolutionInUm.y -
           atomLutSizeY ) > 0.0f )
    {

      ++ atomLutSizeY;

    }

    int32_t atomLutSizeZ = ( int32_t )( fieldOfViewSpanZ /
                                        _gridResolutionInUm.z );
    if ( ( fieldOfViewSpanZ / _gridResolutionInUm.z -
           atomLutSizeZ ) > 0.0f )
    {

      ++ atomLutSizeZ;

    }

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "atom LUT size : ( "
               << atomLutSizeX << ", "
               << atomLutSizeY << ", "
               << atomLutSizeZ << " )"
               << std::endl;

      }
      else
      {

        std::cout << "atom LUT size : ( "
                  << atomLutSizeX << ", "
                  << atomLutSizeY << ", "
                  << atomLutSizeZ << " )"
                  << std::endl;

      }

    }

    int32_t atomLutSizeXY = atomLutSizeX * atomLutSizeY;
    int32_t atomLutSizeXYZ = atomLutSizeXY * atomLutSizeZ;

    timer.storeTime();

    ////////////////////////////////////////////////////////////////////////////
    // preparing atom view(s) and computing the maximum atom count
    // per voxel of the LUT
    ////////////////////////////////////////////////////////////////////////////

    int32_t atomCount = _virtualTissue->getAtomCount();
    uint64_t atomParameterCount = _virtualTissue->getAtomParameterCount();

    UInt8VectorView atomTypes( "atomTypes", atomCount );
    BoolVectorView atomCompressedRepresentations(
                                   "atomCompressedRepresentations", atomCount );
    FloatVectorView atomParameters( "atomParameters", atomParameterCount );
    UInt64VectorView atomParameterOffsets( "atomParameterOffsets", atomCount );
    Int32VectorView atomPopulationIds( "atomPopulationIds", atomCount );
    Int32VectorView atomCellIds( "atomCellIds", atomCount );

    int32_t atomIndex = 0;
    int32_t populationId = 0;
    int32_t cellId = 0;
    float atomSpanRadius = 0.0f;
    float maximumAtomSpanRadius = 0.0f;
    uint64_t atomParameterIndex = 0U;
    gkg::VirtualTissue::const_iterator
      p = _virtualTissue->begin(),
      pe = _virtualTissue->end();
    while ( p != pe )
    {

      populationId = ( *p )->getId();
      gkg::Population::const_iterator
        c = ( *p )->begin(),
        ce = ( *p )->end();
      while ( c != ce )
      {

        cellId = ( *c )->getId();
        gkg::Cell::const_iterator
          a = ( *c )->begin(),
          ae = ( *c )->end();
        while ( a != ae )
        {

          atomTypes( atomIndex ) = ( *a )->getType();
          atomCompressedRepresentations( atomIndex ) =
                                          ( *a )->hasCompressedRepresentation();
          ( *a )->getTranslatedParameters( 
                                          &atomParameters( atomParameterIndex ),
                                          -fieldOfViewLowerX,
                                          -fieldOfViewLowerY,
                                          -fieldOfViewLowerZ );
          atomParameterOffsets( atomIndex ) = atomParameterIndex;
          atomSpanRadius = ( *a )->getSpanRadius();
          atomPopulationIds( atomIndex ) = populationId;
          atomCellIds( atomIndex ) = cellId;

          if ( atomSpanRadius > maximumAtomSpanRadius )
          {

            maximumAtomSpanRadius = atomSpanRadius;

          }

          atomParameterIndex += ( uint64_t )( *a )->getParameterCount();

          ++ atomIndex;
          ++ a;

        }

        ++ c;

      }

      ++ p;

    }

    // allocating a view to store the number of atoms for each voxel
    Int32VectorView atomCountPerGridVoxel( "atomCountPerGridVoxel",
                                          ( int64_t )atomLutSizeXYZ );

    // allocating a Kokkos kernel that initializes the number of atoms 
    // for each voxel to zero
    auto atomCountPerGridVoxelInitializer = 
           gkg::AtomCountPerGridVoxelInitializer( atomCountPerGridVoxel );

    // initializing the view storing the number of atoms for each 
    // voxel of the LUT table
    Kokkos::parallel_for( range_policy( 0, atomLutSizeXYZ ),
                          atomCountPerGridVoxelInitializer );
    Kokkos::fence();

    // allocating a Kokkos kernel that computes the number of atoms 
    // for all voxels
    auto spreadedAtomCountPerGridVoxelComputer =
           gkg::SpreadedAtomCountPerGridVoxelComputer(
                       atomTypes,
                       atomCompressedRepresentations,
                       atomParameters,
                       atomParameterOffsets,
                       0.0f,   // already removed fieldOfViewLowerX
                       ( float )_gridResolutionInUm.x,
                       0.0f,   // already removed fieldOfViewLowerY
                       ( float )_gridResolutionInUm.y,
                       0.0f,   // already removed fieldOfViewLowerZ
                       ( float )_gridResolutionInUm.z,
                       atomLutSizeX,
                       atomLutSizeY,
                       atomLutSizeZ,
                       atomLutSizeXY,
                       atomCountPerGridVoxel );

    // computing the number of atoms for all voxels
    Kokkos::parallel_for( range_policy( 0, atomCount ),
                          spreadedAtomCountPerGridVoxelComputer );
    Kokkos::fence();

    // allocating a Kokkos kernel that computes the maximum number of  
    // atoms for all voxels
    auto maximumAtomCountPerGridVoxelComputer =
           gkg::MaximumAtomCountPerGridVoxelComputer( atomCountPerGridVoxel );

    // computing the maximum number of atoms for all voxels
    int32_t maximumAtomCountPerGridVoxel = 0;
    Kokkos::parallel_reduce( atomLutSizeXYZ, 
                             maximumAtomCountPerGridVoxelComputer,
                             maximumAtomCountPerGridVoxel );
    Kokkos::fence();


    // allocating a view to store the offset for each voxel
    Int32VectorView atomOffsetPerGridVoxel( "atomOffsetPerGridVoxel",
                                           ( int64_t )atomLutSizeXYZ );


    // computing the atoms offest for all voxels
    atomOffsetPerGridVoxel( 0 ) = 0;
    int32_t atomOffsetPerGridVoxelIndex = 1;
    for ( atomOffsetPerGridVoxelIndex = 1;
          atomOffsetPerGridVoxelIndex  < atomLutSizeXYZ;
          atomOffsetPerGridVoxelIndex++ )
    {

      atomOffsetPerGridVoxel( atomOffsetPerGridVoxelIndex ) = 
        atomOffsetPerGridVoxel( atomOffsetPerGridVoxelIndex - 1 ) + 
        atomCountPerGridVoxel( atomOffsetPerGridVoxelIndex - 1 );

    }
    int64_t atomOffsetPerGridVoxelSize =
                       ( int64_t )atomOffsetPerGridVoxel( atomLutSizeXYZ - 1 ) +
                       ( int64_t )atomCountPerGridVoxel( atomLutSizeXYZ - 1 );

    // re-initializing the view storing the number of atoms for each voxel        
    Kokkos::parallel_for( range_policy( 0, atomLutSizeXYZ ),
                          atomCountPerGridVoxelInitializer );
    Kokkos::fence();


    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "maximum atom count per voxel :  "
               << maximumAtomCountPerGridVoxel
               << std::endl;
        *osLog << "atom lut memory consumption :  "
               << atomOffsetPerGridVoxelSize << "="
               << ( int64_t )atomOffsetPerGridVoxelSize *
                  ( int64_t )4 / 
                  ( 1024.0 * 1024.0 )
               << " MBytes"
               << std::endl;

      }
      else
      {

        std::cout << "maximum atom count per voxel :  "
                  << maximumAtomCountPerGridVoxel
                  << std::endl;
        std::cout << "atom lut memory consumption :  "
                  << atomOffsetPerGridVoxelSize << "="
                  << ( int64_t )atomOffsetPerGridVoxelSize *
                     ( int64_t )4 /
                     ( 1024.0 * 1024.0 )
                  << " MBytes"
                  << std::endl;

      }

    }


    // allocating the look-up table
    Int32VectorView atomIndexLut( "atomIndexLut",
                                  atomOffsetPerGridVoxelSize );

    // allocating an atom index LUT computer
    auto spreadedAtomIndexLutComputer =
            gkg::SpreadedAtomIndexLutComputer(
                    atomTypes,
                    atomCompressedRepresentations,
                    atomParameters,
                    atomParameterOffsets,
                    0.0f,   // already removed fieldOfViewLowerX
                    ( float )_gridResolutionInUm.x,
                    0.0f,   // already removed fieldOfViewLowerY
                    ( float )_gridResolutionInUm.y,
                    0.0f,   // already removed fieldOfViewLowerZ
                    ( float )_gridResolutionInUm.z,
                    atomLutSizeX,
                    atomLutSizeY,
                    atomLutSizeZ,
                    atomLutSizeXY,
                    atomLutSizeXYZ,
                    atomCountPerGridVoxel,
                    atomOffsetPerGridVoxel,
                    atomIndexLut );

    // filling the look-up table with atom indices
    Kokkos::parallel_for( range_policy( 0, atomCount ),
                          spreadedAtomIndexLutComputer );
    Kokkos::fence();

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // allocating Kokkos structure(s) for particles
    ////////////////////////////////////////////////////////////////////////////

    // for particle(s)
    Float3VectorView particleCoordinates( "particleCoordinates",
                                           _particleCountPerSession );
    Int32VectorView particleIds( "particleIds", _particleCountPerSession );


    ////////////////////////////////////////////////////////////////////////////
    // creating a virtual tissue kokkos container kernel
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage(
                         "creating a virtual tissue kokkos container kernel : ",
                         verbose,
                         osLog );

    gkg::RCPointer< gkg::VirtualTissueKokkosContainer >
      virtualTissueKokkosContainer(
        new gkg::VirtualTissueKokkosContainer(
                                        atomCount,
                                        atomTypes,
                                        atomCompressedRepresentations,
                                        atomParameters,
                                        atomParameterOffsets,
                                        atomPopulationIds,
                                        atomCellIds,
                                        atomCountPerGridVoxel,
                                        atomOffsetPerGridVoxel,
                                        atomIndexLut,
                                        _virtualTissue->getFieldOfView(),
                                        _gridResolutionInUm,
                                        atomLutSizeX,
                                        atomLutSizeY,
                                        atomLutSizeZ,
                                        atomLutSizeXY,
                                        atomLutSizeXYZ,
                                        maximumAtomCountPerGridVoxel ) );

    displaySimpleMessage( "done", verbose, osLog, true );

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // allocating a random pool
    ////////////////////////////////////////////////////////////////////////////
    uint64_t seed = uint64_t( rand() );

    Kokkos::Random_XorShift64_Pool< Kokkos::DefaultExecutionSpace >
      randPool( seed );


    ////////////////////////////////////////////////////////////////////////////
    // creating a particle kokkos initialization kernel
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "creating a particle kokkos initialization kernel : ",
                          verbose,
                          osLog );

    gkg::RCPointer< gkg::DiffusionProcessParticleInitializer >
      diffusionProcessParticleInitializer(
        new gkg::DiffusionProcessParticleInitializer(
                                        _particleInitializationType,
                                        virtualTissueKokkosContainer,
                                        particleCoordinates,
                                        particleIds,
                                        randPool ) );

    displaySimpleMessage( "done", verbose, osLog, true );


    ////////////////////////////////////////////////////////////////////////////
    // collecting time step count for all MRI sequences
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "collecting time step count for all MRI sequences : ",
                          verbose,
                          osLog );

    int32_t mriSequenceCount = mriSequencePool->getCount();

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << mriSequenceCount 
               << " MRI pulse sequences"
               << std::endl;

      }
      else
      {

        std::cout << mriSequenceCount 
                  << " MRI pulse sequences"
                  << std::endl;

      }

    }

    Int32VectorView perSequenceTimeStepCounts( "perSequenceTimeStepCounts",
                                               mriSequenceCount );
    int32_t mriSequenceIndex = 0;
    int32_t perSequenceTimeStepCount = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      const gkg::RCPointer< gkg::MRISequence >&
        mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );
      perSequenceTimeStepCount = mriSequence->getTimeStepCount();
      perSequenceTimeStepCounts( mriSequenceIndex ) = perSequenceTimeStepCount;

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "  - "
                 << mriSequencePool->getMRISequenceName( mriSequenceIndex )
                 << " -> "
                 << perSequenceTimeStepCount
                 << " time steps"
                 << std::endl;

        }
        else
        {

          std::cout << "  - "
                    << mriSequencePool->getMRISequenceName( mriSequenceIndex )
                    << " -> "
                    << perSequenceTimeStepCount
                    << " time steps"
                    << std::endl;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // checking that the echo time(s) of all MRI sequence(s) are below
      // the simulation time
      //////////////////////////////////////////////////////////////////////////
      // to be checked, maybe >= is sufficient
      if ( perSequenceTimeStepCount > _stepCount )
      {

        throw std::runtime_error(
          std::string( "the number of diffusion process step count is " ) +
          " smaller than the required step count for sequence '" +
          mriSequencePool->getMRISequenceName( mriSequenceIndex ) );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting the gradient magnitude count for each MRI sequence
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage(
             "collecting the gradient magnitude count for all MRI sequences : ",
             verbose,
             osLog,
             true );

    Int32VectorView 
      perSequenceGradientMagnitudeCounts( "perSequenceGradientMagnitudeCounts",
                                          mriSequenceCount );
    int32_t perSequenceGradientMagnitudeCount = 0;
    int32_t allSequencesGradientMagnitudeCount = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      const gkg::RCPointer< gkg::MRISequence >&
        mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );

      perSequenceGradientMagnitudeCount =
                                  mriSequence->getTotalGradientMagnitudeCount();
      perSequenceGradientMagnitudeCounts( mriSequenceIndex ) =
                                              perSequenceGradientMagnitudeCount;
      allSequencesGradientMagnitudeCount += perSequenceGradientMagnitudeCount;
      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "  - "
                 << mriSequencePool->getMRISequenceName( mriSequenceIndex )
                 << " -> "
                 << perSequenceGradientMagnitudeCount
                 << " gradient magnitudes"
                 << std::endl;

        }
        else
        {

          std::cout << "  - "
                    << mriSequencePool->getMRISequenceName( mriSequenceIndex )
                    << " -> "
                    << perSequenceGradientMagnitudeCount
                    << " gradient magnitudes"
                    << std::endl;

        }

      }

    }
    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  ------------------------------------------- "
               << std::endl
               << "  in total: "
               << allSequencesGradientMagnitudeCount
               << " gradient magnitudes"
               << std::endl;

      }
      else
      {

        std::cout << "  ------------------------------------------- "
                  << std::endl
                  << "  in total: "
                  << allSequencesGradientMagnitudeCount
                  << " gradient magnitudes"
                  << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the total number of diffusion-weighted contrasts for all MRI 
    // sequences, all gradient magnitudes and all gradient orientations,
    // as well as the total phase shift count for all sequences
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "computing the total number of DW contrasts : ",
                          verbose,
                          osLog );

    Int32VectorView perGradientMagnitudeOrientationCounts(
                                        "perGradientMagnitudeOrientationCounts",
                                        allSequencesGradientMagnitudeCount );
    Int32VectorView perSequenceDiffusionWeightedContrastCounts(
                                   "perSequenceDiffusionWeightedContrastCounts",
                                   mriSequenceCount );
    Int32VectorView formerAccumulatedDiffusionWeightedContrastCounts(
                             "formerAccumulatedDiffusionWeightedContrastCounts",
                             mriSequenceCount );
    Int32VectorView perSequencePhaseShiftStartingIndices(
                                         "perSequencePhaseShiftStartingIndices",
                                         mriSequenceCount );

    int32_t totalDiffusionWeightedContrastCount  = 0;
    int32_t allSequencesPhaseShiftCount = 0;
    int32_t perSequenceGradientMagnitudeIndex = 0;
    int32_t perGradientMagnitudeOrientationCount = 0;
    int32_t allSequencesGradientMagnitudeIndex = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      const gkg::RCPointer< gkg::MRISequence >&
        mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );

      perSequenceTimeStepCount = perSequenceTimeStepCounts( mriSequenceIndex );

      perSequenceGradientMagnitudeCount =
                         perSequenceGradientMagnitudeCounts( mriSequenceIndex );

      perSequenceDiffusionWeightedContrastCounts( mriSequenceIndex ) = 0;
      perSequencePhaseShiftStartingIndices( mriSequenceIndex ) =
                                                    allSequencesPhaseShiftCount;

      for ( perSequenceGradientMagnitudeIndex = 0;
            perSequenceGradientMagnitudeIndex <
                                              perSequenceGradientMagnitudeCount;
            perSequenceGradientMagnitudeIndex++ )
      {


        perGradientMagnitudeOrientationCount =
                              mriSequence->getTotalGradientOrientationCount(
                                          perSequenceGradientMagnitudeIndex );

        perGradientMagnitudeOrientationCounts(
          allSequencesGradientMagnitudeIndex ) = 
                                           perGradientMagnitudeOrientationCount;
        ++ allSequencesGradientMagnitudeIndex;

        totalDiffusionWeightedContrastCount +=
                                           perGradientMagnitudeOrientationCount;

        perSequenceDiffusionWeightedContrastCounts( mriSequenceIndex ) +=
                                           perGradientMagnitudeOrientationCount;

        // 3 hereafter because we store phase shift for x, y, and z axis
        allSequencesPhaseShiftCount += perSequenceTimeStepCount * 
                                       3 * 
                                       perGradientMagnitudeOrientationCount;

      }

    }


    formerAccumulatedDiffusionWeightedContrastCounts( 0 ) = 0;
    for ( mriSequenceIndex = 1;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      formerAccumulatedDiffusionWeightedContrastCounts( mriSequenceIndex ) =
         formerAccumulatedDiffusionWeightedContrastCounts(
                                                        mriSequenceIndex - 1 ) +
         perSequenceDiffusionWeightedContrastCounts( mriSequenceIndex - 1 );

    }

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << totalDiffusionWeightedContrastCount << std::endl;

      }
      else
      {

        std::cout << totalDiffusionWeightedContrastCount << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the vector of MRI sequence indices corresponding to each 
    // contrast index
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "computing the vector of MRI sequence indices "
                          "corresponding to each DW contrast index : ",
                          verbose,
                          osLog );

    Int32VectorView perContrastMriSequenceIndices(
                                          "perContrastMriSequenceIndices",
                                          totalDiffusionWeightedContrastCount );
    int32_t perSequenceDiffusionWeightedContrastIndex  = 0;
    int32_t diffusionWeightedContrastIndex  = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      for ( perSequenceDiffusionWeightedContrastIndex = 0;
            perSequenceDiffusionWeightedContrastIndex < 
              perSequenceDiffusionWeightedContrastCounts( mriSequenceIndex );
            perSequenceDiffusionWeightedContrastIndex++ )
      {

        perContrastMriSequenceIndices( diffusionWeightedContrastIndex ) =
                                                               mriSequenceIndex;
        ++ diffusionWeightedContrastIndex;

      }

    }
    displaySimpleMessage( "done", verbose, osLog, true );

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // computing all sequences phase shifts
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "computing all sequence phase shifts  : ",
                          verbose, osLog );


    // allocating
    FloatVectorView allSequencesPhaseShifts( "allSequencesPhaseShifts",
                                             allSequencesPhaseShiftCount );

    int32_t perSequenceTimeStepIndex = 0;
    int32_t perGradientMagnitudeOrientationIndex = 0;
    int32_t offset = 0;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {

      const gkg::RCPointer< gkg::MRISequence >&
        mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );

      perSequenceTimeStepCount = perSequenceTimeStepCounts( mriSequenceIndex );
      perSequenceGradientMagnitudeCount =
                         perSequenceGradientMagnitudeCounts( mriSequenceIndex );

      for ( perSequenceTimeStepIndex = 0;
            perSequenceTimeStepIndex < perSequenceTimeStepCount;
            perSequenceTimeStepIndex++ )
      {

        for ( perSequenceGradientMagnitudeIndex = 0;
              perSequenceGradientMagnitudeIndex <
                                              perSequenceGradientMagnitudeCount;
              perSequenceGradientMagnitudeIndex++ )
        {


          perGradientMagnitudeOrientationCount =
                              mriSequence->getTotalGradientOrientationCount(
                                            perSequenceGradientMagnitudeIndex );

          for ( perGradientMagnitudeOrientationIndex = 0;
                perGradientMagnitudeOrientationIndex <
                                           perGradientMagnitudeOrientationCount;
                perGradientMagnitudeOrientationIndex++ )
          {

            const gkg::Vector3d< float >&
              phaseShift = mriSequence->getPhaseShift(
                                         perSequenceTimeStepIndex,
                                         perSequenceGradientMagnitudeIndex,
                                         perGradientMagnitudeOrientationIndex );

            allSequencesPhaseShifts( offset ) = phaseShift.x;
            ++ offset;

            allSequencesPhaseShifts( offset ) = phaseShift.y;
            ++ offset;

            allSequencesPhaseShifts( offset ) = phaseShift.z;
            ++ offset;

          }

        }

      }

    }
    // sanity check
    if ( offset != allSequencesPhaseShiftCount )
    {

      throw std::runtime_error(
                        "mismatch between last offset and phase shift count!" );

    }
    displaySimpleMessage( " initialized", verbose, osLog, true );

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // allocating all sequences phase accumulator(s)
    ////////////////////////////////////////////////////////////////////////////

    displaySimpleMessage( "allocating phase accumulators  : ", verbose, osLog );
    int32_t allSequencesPhaseAccumulatorCount = 
                                            _particleCountPerSession *
                                            totalDiffusionWeightedContrastCount;

    std::vector< FloatVectorView >
      allSequencesPhaseAccumulators( _particleCountPerSession );

    int32_t particleIndex = 0;
    for ( particleIndex = 0; particleIndex < _particleCountPerSession;
          particleIndex++ )
    {

      std::string name = std::string( "allSequencesPhaseAccumulators"  ) + 
                                      gkg::StringConverter::toString(
                                                                particleIndex );
      allSequencesPhaseAccumulators[ particleIndex ] = FloatVectorView(
                              name,
                              totalDiffusionWeightedContrastCount );

    }

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << allSequencesPhaseAccumulatorCount
               << " phase accumulators allocated"
               << std::endl;

      }
      else
      {

        std::cout << allSequencesPhaseAccumulatorCount
                  << " phase accumulators allocated"
                  << std::endl;

      }

    }

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // storing cell permeabilities and diffusivities in 2 floating vector views
    ////////////////////////////////////////////////////////////////////////////

    int32_t populationCount = _virtualTissue->getPopulationCount();
    int32_t cellCount = _virtualTissue->getCellCount();

    FloatVectorView cellPermeabilities( "cellPermeabilities", cellCount );
    FloatVectorView cellDiffusivitiesInUm2PerUs( "cellDiffusivitiesInUm2PerUs",
                                                 cellCount );
    FloatVectorView cellTwiceStepLengthOverThreeDiffusivities(
                                    "cellTwiceStepLengthOverThreeDiffusivities",
                                    cellCount );
    FloatVectorView cellStepLengths( "cellStepLengths", cellCount );
    UInt64VectorView allPopulationCellOffsets( "allPopulationCellOffsets",
                                                populationCount );

    int32_t globalCellId = 0;
    for ( populationId = 0; populationId < populationCount; populationId++ )
    {

      allPopulationCellOffsets( populationId ) = globalCellId;
      int32_t cellCountForPopulationId = _virtualTissue->getPopulation(
                                                 populationId )->getCellCount();

      for ( cellId = 0; cellId < cellCountForPopulationId; cellId++ )
      {


        cellPermeabilities( globalCellId ) = _virtualTissue->getTissueProperty(
                                             populationId,
                                             cellId,
                                             "permeability_distribution",
                                             0.0f );
        cellDiffusivitiesInUm2PerUs( globalCellId ) =
                                   _virtualTissue->getTissueProperty(
                                     populationId,
                                     cellId,
                                     "diffusivity_in_um2_per_us_distribution",
                                     _particleBulkDiffusivityInUm2PerUs );

        cellStepLengths( globalCellId ) = ( float )std::sqrt(
                                   6.0 *
                                   cellDiffusivitiesInUm2PerUs( globalCellId ) *
                                   _timeStepInUs );

        cellTwiceStepLengthOverThreeDiffusivities( globalCellId ) =
                         2.0f * cellStepLengths( globalCellId ) /
                         ( 3.0f * cellDiffusivitiesInUm2PerUs( globalCellId ) );

        ++ globalCellId;

      }

    }

    float twiceBulkStepLengthOverThreeBulkDiffusivity =
                                  2.0f * _bulkStepLength /
                                  ( 3.0f * _particleBulkDiffusivityInUm2PerUs );


    ////////////////////////////////////////////////////////////////////////////
    // creating diffusion process Monte-Carlo and MRI simulator
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::DiffusionProcessMonteCarloAndMRISimulator >
      diffusionProcessMonteCarloAndMRISimulator(
        new gkg::DiffusionProcessMonteCarloAndMRISimulator(
                               // for Monte-Carlo simulation
                               virtualTissueKokkosContainer,
                               cellPermeabilities,
                               cellDiffusivitiesInUm2PerUs,
                               cellStepLengths,
                               cellTwiceStepLengthOverThreeDiffusivities,
                               allPopulationCellOffsets,
                               _particleBulkDiffusivityInUm2PerUs,
                               _bulkStepLength,
                               twiceBulkStepLengthOverThreeBulkDiffusivity,
                               _particleToMembraneInteractionType,
                               _particleCountPerSession,
                               particleCoordinates,
                               particleIds,
                               randPool,
                               // for MRI simulation
                               mriSequenceCount,
                               totalDiffusionWeightedContrastCount,
                               allSequencesPhaseShifts,
                               perContrastMriSequenceIndices,
                               perSequenceTimeStepCounts,
                               perSequencePhaseShiftStartingIndices,
                               perSequenceDiffusionWeightedContrastCounts,
                               formerAccumulatedDiffusionWeightedContrastCounts,
                               allSequencesPhaseAccumulators ) );


    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // looping over session(s)
    ////////////////////////////////////////////////////////////////////////////

    // allocating a vector to store the phase contributions to the attenuation: 
    //     S( c ) = Sum_over_p ( exp(-j x Phi( p, c ) ) )
    //     c: contrast
    //     p: particle
    std::vector< std::complex< float > > 
      sumOfExponentialOfJTimesAccumulatedPhase( 
        totalDiffusionWeightedContrastCount, 0.0f );

    int32_t sessionIndex = 0;
    for ( sessionIndex = 0;
          sessionIndex < _sessionCount;
          sessionIndex++ )
    {


      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << std::endl
                 << "---------------- session " << sessionIndex + 1
                 << " ----------------"
                 << std::endl;

        }
        else
        {

          std::cout << std::endl
                    << "---------------- session " << sessionIndex + 1
                    << " ----------------"
                    << std::endl;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // initializing the particle coordinate(s)
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        if ( _particleInitializationType ==
                          gkg::DiffusionProcessParticleInitializer::Everywhere )
        {

          displaySimpleMessage( "initializing particle(s) everywhere : ",
                                verbose,
                                osLog );

        }
        else if ( _particleInitializationType ==
                   gkg::DiffusionProcessParticleInitializer::OnlyIntracellular )
        {

          displaySimpleMessage( "initializing particle(s) only inside cells : ",
                                verbose,
                                osLog );

        }
        else if ( _particleInitializationType ==
                   gkg::DiffusionProcessParticleInitializer::OnlyExtracellular )
        {

          displaySimpleMessage(
                               "initializing particle(s) only outside cells : ",
                               verbose,
                               osLog );

        }
        else if ( _particleInitializationType ==
                  gkg::DiffusionProcessParticleInitializer::
                                               AllParticlesAtFieldOfViewCenter )
        {

          displaySimpleMessage(
                          "initializing particle(s) at field of view center : ",
                          verbose,
                          osLog );

        }
        else if ( _particleInitializationType ==
                  gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredXYPlane )
        {

          displaySimpleMessage(
               "initializing particle(s) only intracellular at field of view "
               "centered XY-plane : ",
               verbose,
               osLog );

        }
        else if ( _particleInitializationType ==
                  gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredYZPlane )
        {

          displaySimpleMessage(
               "initializing particle(s) only intracellular at field of view "
               "centered YZ-plane : ",
               verbose,
               osLog );

        }
        else if ( _particleInitializationType ==
                  gkg::DiffusionProcessParticleInitializer::
                     AllParticlesOnlyIntracellularAtFieldOfViewCenteredXZPlane )
        {

          displaySimpleMessage(
               "initializing particle(s) only intracellular at field of view "
               "centered XZ-plane : ",
               verbose,
               osLog );

        }

      }

      Kokkos::parallel_for( range_policy( 0, _particleCountPerSession ),
                            *diffusionProcessParticleInitializer );
      Kokkos::fence();

      displaySimpleMessage( "done", verbose, osLog, true );

      if ( !fileNameDiffusionProcessOutputParticles.empty() )
      {

        std::string fileNameOutputParticles = 
             fileNameDiffusionProcessOutputParticles + "_session" +
             gkg::StringConverter::toString( sessionIndex + 1 ) +
             ".pointcloud";
        std::ofstream os( fileNameOutputParticles.c_str() );

        if ( _particleBackupPercentage == 100.0f )
        {

          int32_t p = 0;
          for ( p = 0; p < _particleCountPerSession; p++ )
          {

            os << particleCoordinates( p, 0 ) + fieldOfViewLowerX << "    "
               << particleCoordinates( p, 1 ) + fieldOfViewLowerY << "    "
               << particleCoordinates( p, 2 ) + fieldOfViewLowerZ << std::endl;
             

          }

        }
        else
        {

          std::vector< int32_t >::const_iterator
            p = _randomParticleSelection.begin(),
            pe = _randomParticleSelection.end();
          while ( p != pe )
          {

            os << particleCoordinates( *p, 0 ) + fieldOfViewLowerX << "    "
               << particleCoordinates( *p, 1 ) + fieldOfViewLowerY << "    "
               << particleCoordinates( *p, 2 ) + fieldOfViewLowerZ << std::endl;
            ++ p;

          }

        }

        os.close();

      }


      //////////////////////////////////////////////////////////////////////////
      // resetting phase accumulators
      //////////////////////////////////////////////////////////////////////////

      displaySimpleMessage( "resetting phase accumulators : ",
                            verbose,
                            osLog );

      for ( particleIndex = 0; particleIndex < _particleCountPerSession;
            particleIndex++ )
      {

        float*  
          allSequencesPhaseAccumulatorPtr =
                       &( allSequencesPhaseAccumulators[ particleIndex ]( 0 ) );

        for ( diffusionWeightedContrastIndex = 0;
              diffusionWeightedContrastIndex <
              totalDiffusionWeightedContrastCount;
              diffusionWeightedContrastIndex++ )
        {

          *allSequencesPhaseAccumulatorPtr = 0.0f;
          ++ allSequencesPhaseAccumulatorPtr;

        }

      }

      displaySimpleMessage( "done", verbose, osLog, true );


      //////////////////////////////////////////////////////////////////////////
      // launching diffusion Monte-Carlo and MRI simulator(s) 
      //////////////////////////////////////////////////////////////////////////

      displaySimpleMessage( "performing Monte-Carlo & MRI simulation : ",
                            verbose,
                            osLog );


      gkg::RCPointer< std::vector< gkg::LightCurve3d< float > > > trajectories;
      if ( !fileNameDiffusionProcessOutputTrajectories.empty() )
      {

        if ( _particleBackupPercentage == 100.0f )
        {

          trajectories.reset( new std::vector< gkg::LightCurve3d< float > >(
                                                   _particleCountPerSession ) );
          for ( int32_t p = 0; p < _particleCountPerSession; p++ )
          {

            ( *trajectories )[ p ].reserve( _stepCount + 1 );
            ( *trajectories )[ p ].addPoint( gkg::Vector3d< float >(
                            particleCoordinates( p, 0 ) + fieldOfViewLowerX,
                            particleCoordinates( p, 1 ) + fieldOfViewLowerY,
                            particleCoordinates( p, 2 ) + fieldOfViewLowerZ ) );

          }

        }
        else
        {

          trajectories.reset( new std::vector< gkg::LightCurve3d< float > >(
                                            _randomParticleSelection.size() ) );
          std::vector< int32_t >::const_iterator
            p = _randomParticleSelection.begin(),
            pe = _randomParticleSelection.end();
          int32_t index = 0;
          while ( p != pe )
          {

            ( *trajectories )[ index ].reserve( _stepCount + 1 );
            ( *trajectories )[ index ].addPoint( gkg::Vector3d< float >(
                           particleCoordinates( *p, 0 ) + fieldOfViewLowerX,
                           particleCoordinates( *p, 1 ) + fieldOfViewLowerY,
                           particleCoordinates( *p, 2 ) + fieldOfViewLowerZ ) );

            ++ index;
            ++ p;

          }

        }

      }


      // resetting the current time step to zero between two sessions
      diffusionProcessMonteCarloAndMRISimulator->resetTimeStep();

      // looping over the time step(s)      
      int32_t step = 0;
      for ( step = 0; step < _stepCount; step++ )
      {

        if ( verbose )
        {

          if ( step != 0 )
          {

            if ( osLog )
            {

              *osLog << gkg::Eraser( 24 );

            }
            else
            {

              std::cout << gkg::Eraser( 24 );

            }

          }
          if ( osLog )
          {

            *osLog << " step[ " << std::setw( 6 ) << step + 1 
                   << " / " << std::setw( 6 ) << _stepCount
                   << " ]" << std::flush;

          }
          else
          {

            std::cout << " step[ " << std::setw( 6 ) << step + 1 
                      << " / " << std::setw( 6 ) << _stepCount
                      << " ]" << std::flush;

          }

        }

        Kokkos::parallel_for( range_policy( 0, _particleCountPerSession ),
                              *diffusionProcessMonteCarloAndMRISimulator );
        Kokkos::fence();


        if ( !fileNameDiffusionProcessOutputTrajectories.empty() )
        {

          if ( _particleBackupPercentage == 100.0f )
          {

            for ( int32_t p = 0; p < _particleCountPerSession; p++ )
            {

              ( *trajectories )[ p ].addPoint( gkg::Vector3d< float >(
                            particleCoordinates( p, 0 ) + fieldOfViewLowerX,
                            particleCoordinates( p, 1 ) + fieldOfViewLowerY,
                            particleCoordinates( p, 2 ) + fieldOfViewLowerZ ) );

            }

          }
          else
          {

            std::vector< int32_t >::const_iterator
              p = _randomParticleSelection.begin(),
              pe = _randomParticleSelection.end();
            int32_t index = 0;
            while ( p != pe )
            {

              ( *trajectories )[ index ].addPoint( gkg::Vector3d< float >(
                           particleCoordinates( *p, 0 ) + fieldOfViewLowerX,
                           particleCoordinates( *p, 1 ) + fieldOfViewLowerY,
                           particleCoordinates( *p, 2 ) + fieldOfViewLowerZ ) );
              ++ index;
              ++ p;

            }

          }

        }

        diffusionProcessMonteCarloAndMRISimulator->step();

      }

      for ( diffusionWeightedContrastIndex = 0;
            diffusionWeightedContrastIndex <
              totalDiffusionWeightedContrastCount;
            diffusionWeightedContrastIndex++ )
      {

        for ( particleIndex = 0;
              particleIndex < _particleCountPerSession;
              particleIndex++ )
        {

          sumOfExponentialOfJTimesAccumulatedPhase[
            diffusionWeightedContrastIndex ] +=
              std::polar(
                1.0f,
                ( float )allSequencesPhaseAccumulators[ particleIndex ](
                                             diffusionWeightedContrastIndex ) );
                
        }
         
      }
                
                
      displaySimpleMessage( "", verbose, osLog, true );


      if ( !fileNameDiffusionProcessOutputTrajectories.empty() )
      {

        gkg::BundleMap< std::string > bundleMapOfTrajectories;
        bundleMapOfTrajectories.addCurve3ds( "1", *trajectories );

        std::string fileNameOutputTrajectories = 
             fileNameDiffusionProcessOutputTrajectories + "_session" +
             gkg::StringConverter::toString( sessionIndex + 1 );

        gkg::Writer::getInstance().write( fileNameOutputTrajectories,
                                          bundleMapOfTrajectories );

      }

    }

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // filling with the output vector of diffusion-weighted attenuation(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::
                                       getInstance().getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    perSequenceDiffusionWeightedAttenuations.resize( mriSequenceCount );
    int32_t perSequenceDiffusionWeightedContrastCount = 0;
    int32_t totalDiffusionWeightedContrastIndex = 0;
    float noiseStandardDeviation = 0.0f;
    float sessionTimesParticleCountPerSession = ( float )_sessionCount *
                                      ( float )_particleCountPerSession;
    for ( mriSequenceIndex = 0;
          mriSequenceIndex < mriSequenceCount;
          mriSequenceIndex++ )
    {



      std::vector< float >& diffusionWeightedAttenuations =
                   perSequenceDiffusionWeightedAttenuations[ mriSequenceIndex ];

      perSequenceDiffusionWeightedContrastCount =
                 perSequenceDiffusionWeightedContrastCounts[ mriSequenceIndex ];

      diffusionWeightedAttenuations.resize(
                                    perSequenceDiffusionWeightedContrastCount );


      const gkg::RCPointer< gkg::MRISequence >&
        mriSequence = mriSequencePool->getMRISequence( mriSequenceIndex );

      noiseStandardDeviation = mriSequence->getNoiseStandardDeviation();
      if ( noiseStandardDeviation > 0.0f )
      {

        for ( perSequenceDiffusionWeightedContrastIndex = 0;
              perSequenceDiffusionWeightedContrastIndex <
                                      perSequenceDiffusionWeightedContrastCount;
              perSequenceDiffusionWeightedContrastIndex++ )
        {

          diffusionWeightedAttenuations[
                                   perSequenceDiffusionWeightedContrastIndex ] =
            std::abs( sumOfExponentialOfJTimesAccumulatedPhase[
                                        totalDiffusionWeightedContrastIndex ]  /
                                           sessionTimesParticleCountPerSession +
                      std::complex< float >(
                        ( float )factory->getGaussianRandomNumber(
                                         randomGenerator,
                                         0.0,
                                         ( double )noiseStandardDeviation ),
                        ( float )factory->getGaussianRandomNumber(
                                         randomGenerator,
                                         0.0,
                                         ( double )noiseStandardDeviation ) ) );

          ++ totalDiffusionWeightedContrastIndex;

        }


      }
      else
      {

        for ( perSequenceDiffusionWeightedContrastIndex = 0;
              perSequenceDiffusionWeightedContrastIndex <
                                      perSequenceDiffusionWeightedContrastCount;
              perSequenceDiffusionWeightedContrastIndex++ )
        {

          diffusionWeightedAttenuations[
                                   perSequenceDiffusionWeightedContrastIndex ] =
            std::abs( sumOfExponentialOfJTimesAccumulatedPhase[
                                        totalDiffusionWeightedContrastIndex ]  /
                                          sessionTimesParticleCountPerSession );

          ++ totalDiffusionWeightedContrastIndex;

        }

      }

    }

    timer.storeTime();


    ////////////////////////////////////////////////////////////////////////////
    // displaying computation time(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::list< double > 
        timeLapsInMilliseconds = timer.getTimeLapsInMilliseconds();

      double globalComputationTimeInMilliseconds = 0.0;
      double atomLutSizeComputationTimeInMilliseconds = 0.0;
      double spreadedAtomLutComputationTimeInMilliseconds = 0.0;
      double virtualTissueKokkosContainerComputationTimeInMilliseconds = 0.0;
      double mriSequencePreparationComputationTimeInMilliseconds = 0.0;
      double allPhaseShiftComputationTimeInMilliseconds = 0.0;
      double phaseAccumulatorCreationComputationTimeInMilliseconds = 0.0;
      double monteCarloAndMRISimulatorCreationComputationTimeInMilliseconds
                                                                          = 0.0;
      double monteCarloAndMRISimulatorComputationTimeInMilliseconds = 0.0;
      double diffusionAttenuationComputationTimeInMilliseconds = 0.0;

      int32_t index = 0;
      std::list< double >::const_iterator 
        l = timeLapsInMilliseconds.begin(),
        le = timeLapsInMilliseconds.end();
      while ( l != le )
      {


        globalComputationTimeInMilliseconds += *l;
        if ( index == 0 )
        {

          atomLutSizeComputationTimeInMilliseconds = *l;

        }
        else if ( index == 1 )
        {

          spreadedAtomLutComputationTimeInMilliseconds = *l;

        }
        else if ( index == 2 )
        {

          virtualTissueKokkosContainerComputationTimeInMilliseconds = *l;

        }
        else if ( index == 3 )
        {

          mriSequencePreparationComputationTimeInMilliseconds = *l;

        }
        else if ( index == 4 )
        {

          allPhaseShiftComputationTimeInMilliseconds = *l;

        }
        else if ( index == 5 )
        {

          phaseAccumulatorCreationComputationTimeInMilliseconds = *l;

        }
        else if ( index == 6 )
        {

          monteCarloAndMRISimulatorCreationComputationTimeInMilliseconds = *l;

        }
        else if ( index == 7 )
        {

          monteCarloAndMRISimulatorComputationTimeInMilliseconds = *l;

        }
        else if ( index == 8 )
        {

          diffusionAttenuationComputationTimeInMilliseconds = *l;

        }
        ++ index;
        ++ l;

      }

      if ( osLog )
      {

        *osLog << "summary of Monte-Carlo & MRI simulation timing : "
               << globalComputationTimeInMilliseconds
               << " ms including"
               << std::endl;
        *osLog << " - atom LUT size computation time : "
               << atomLutSizeComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - spreaded atom LUT computation time : "
               << spreadedAtomLutComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - virtual tissue Kokkos container computation time : "
               << virtualTissueKokkosContainerComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - MRI sequence(s) preparation computation time : "
               << mriSequencePreparationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - all phase shift computation time : "
               << allPhaseShiftComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - phase accumulator creation computation time : "
               << phaseAccumulatorCreationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - Monte-Carlo & MRI simulator creation computation time : "
               << monteCarloAndMRISimulatorCreationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - Monte-Carlo & MRI simulator computation time : "
               << monteCarloAndMRISimulatorComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        *osLog << " - diffusion attenuation computation time : "
               << diffusionAttenuationComputationTimeInMilliseconds
               << " ms"
               << std::endl;

      }
      else
      {

        std::cout
               << "summary of Monte-Carlo & MRI simulation timing : "
               << globalComputationTimeInMilliseconds
               << " ms including"
               << std::endl;
        std::cout
               << " - atom LUT size computation time : "
               << atomLutSizeComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - spreaded atom LUT computation time : "
               << spreadedAtomLutComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - virtual tissue Kokkos container computation time : "
               << virtualTissueKokkosContainerComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - MRI sequence(s) preparation computation time : "
               << mriSequencePreparationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - all phase shift computation time : "
               << allPhaseShiftComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - phase accumulator creation computation time : "
               << phaseAccumulatorCreationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - Monte-Carlo & MRI simulator creation computation time : "
               << monteCarloAndMRISimulatorCreationComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - Monte-Carlo & MRI simulator computation time : "
               << monteCarloAndMRISimulatorComputationTimeInMilliseconds
               << " ms"
               << std::endl;
        std::cout
               << " - diffusion attenuation computation time : "
               << diffusionAttenuationComputationTimeInMilliseconds
               << " ms"
               << std::endl;

      }

    }
    diffusionProcessMonteCarloAndMRISimulator.reset();

  }
  GKG_CATCH( "void gkg::MonteCarloDiffusionProcess::simulate( "
             "gkg::RCPointer< gkg::MRISequencePool > mriSequencePool, "
             "const std::string& fileNameDiffusionProcessOutputParticles, "
             "const std::string& fileNameDiffusionProcessOutputTrajectories, "
             "std::vector< std::vector< float > >& "
             "perSequenceDiffusionWeightedAttenuations, "
             "bool verbose, "
             "std::ofstream* osLog ) const" );


}


void gkg::MonteCarloDiffusionProcess::displayInformation(
                                                    std::ofstream* osLog ) const
{

  try
  {

    if ( osLog )
    {

      *osLog << "particle bulk diffusivity in um^2/us : "
             << _particleBulkDiffusivityInUm2PerUs
             << " um^2/us"
             << std::endl;
      *osLog << "particle to membrane interaction type : ";
      if ( _particleToMembraneInteractionType ==
           gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                                     EqualStepLengthRandomLeap )
      {

        *osLog << "equal-step-length-random-leap" << std::endl;

      }
      else
      {

        *osLog << "elastic-bouncing" << std::endl;

      }
      *osLog << "particle count per session : "
             << _particleCountPerSession
             << std::endl;
      *osLog << "grid resolution in um : "
             << _gridResolutionInUm
             << std::endl;
      *osLog << "time step us : "
             << _timeStepInUs
             << " us"
             << std::endl;
      *osLog << "total duration in us : "
             << _totalDurationInUs
             << " us"
             << std::endl;
      *osLog << "bulk step length in um : "
             << _bulkStepLength
             << " um"
             << std::endl;
      *osLog << "step count : "
             << _stepCount
             << std::endl;

    }
    else
    {

      std::cout << "particle bulk diffusivity in um^2/us : "
                << _particleBulkDiffusivityInUm2PerUs
                << " um^2/us"
                << std::endl;
      std::cout << "particle to membrane interaction type : ";
      if ( _particleToMembraneInteractionType ==
           gkg::DiffusionProcessMonteCarloAndMRISimulator::
                                                     EqualStepLengthRandomLeap )
      {

        std::cout << "equal-step-length-random-leap" << std::endl;

      }
      else
      {

        std::cout << "elastic-bouncing" << std::endl;

      }
      std::cout << "particle count per session : "
                << _particleCountPerSession
                << std::endl;
      std::cout << "grid resolution in um : "
                << _gridResolutionInUm
                << std::endl;
      std::cout << "time step us : "
                << _timeStepInUs
                << " us"
                << std::endl;
      std::cout << "total duration in us : "
                << _totalDurationInUs
                << " us"
                << std::endl;
      std::cout << "bulk step length in um : "
                << _bulkStepLength
                << " um"
                << std::endl;
      std::cout << "step count : "
                << _stepCount
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MonteCarloDiffusionProcess::"
             "displayInformation( std::ofstream* osLog ) const" );

}


void gkg::MonteCarloDiffusionProcess::displaySimpleMessage(
                                                     const std::string& message,
                                                     bool verbose,
                                                     std::ofstream* osLog,
                                                     bool applyEndl ) const
{

  try
  {

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog <<  message;
        if ( applyEndl )
        {

          *osLog << std::endl;

        }
        else
        {

          *osLog << std::flush;

        }

      }
      else
      {

        std::cout << message;
        if ( applyEndl )
        {

          std::cout << std::endl;

        }
        else
        {

          std::cout << std::flush;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::MonteCarloDiffusionProcess::displaySimpleMessage( "
             "const string& message, "
             "bool verbose, "
             "std::ofstream* osLog, "
             "bool applyEndl ) const" );

}


void gkg::MonteCarloDiffusionProcess::setParticleInitializationType(
                              const std::string& theParticleInitializationType )
{

  try
  {

    if ( theParticleInitializationType == "everywhere" )
    {

      _particleInitializationType =
                           gkg::DiffusionProcessParticleInitializer::Everywhere;

    }
    else if ( theParticleInitializationType == "only-intracellular" )
    {

      _particleInitializationType =
                    gkg::DiffusionProcessParticleInitializer::OnlyIntracellular;

    }
    else if ( theParticleInitializationType == "only-extracellular" )
    {

      _particleInitializationType =
                    gkg::DiffusionProcessParticleInitializer::OnlyExtracellular;

    }
    else if ( theParticleInitializationType == 
                                       "all-particles-at-field-of-view-center" )
    {

      _particleInitializationType =
      gkg::DiffusionProcessParticleInitializer::AllParticlesAtFieldOfViewCenter;

    }
    else if ( theParticleInitializationType == 
         "all-particles-only-intracellular-at-field-of-view-centered-XY-plane" )
    {

      _particleInitializationType =
      gkg::DiffusionProcessParticleInitializer::
                      AllParticlesOnlyIntracellularAtFieldOfViewCenteredXYPlane;

    }
    else if ( theParticleInitializationType == 
         "all-particles-only-intracellular-at-field-of-view-centered-YZ-plane" )
    {

      _particleInitializationType =
      gkg::DiffusionProcessParticleInitializer::
                      AllParticlesOnlyIntracellularAtFieldOfViewCenteredYZPlane;

    }
    else if ( theParticleInitializationType == 
         "all-particles-only-intracellular-at-field-of-view-centered-XZ-plane" )
    {

      _particleInitializationType =
      gkg::DiffusionProcessParticleInitializer::
                      AllParticlesOnlyIntracellularAtFieldOfViewCenteredXZPlane;

    }
    else
    {

      throw std::runtime_error( "bad particle initialization" );

    }

  }
  GKG_CATCH( "void gkg::MonteCarloDiffusionProcess::"
             "setParticleInitializationType( "
             "const std::string& theParticleInitializationType )" );
}




void gkg::MonteCarloDiffusionProcess::setParticleMembraneInteractionType(
                         const std::string& theParticleMembraneInteractionType )
{

  try
  {

    if ( theParticleMembraneInteractionType == "equal-step-length-random-leap" )
    {

      _particleToMembraneInteractionType =
      gkg::DiffusionProcessMonteCarloAndMRISimulator::EqualStepLengthRandomLeap;

    }
    else if ( theParticleMembraneInteractionType == "elastic-bouncing" )
    {

      _particleToMembraneInteractionType =
                gkg::DiffusionProcessMonteCarloAndMRISimulator::ElasticBouncing;

    }
    else
    {

      throw std::runtime_error( "bad particle to membrane interaction type" );

    }

  }
  GKG_CATCH( "void gkg::MonteCarloDiffusionProcess::"
             "setParticleMembraneInteractionType( "
             "const std::string& theParticleMembraneInteractionType )" );

}

