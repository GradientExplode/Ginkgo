#include "DefaultInformationCallback.h"
#include "GlobalTractographyOptimizationInformation.h"
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::DefaultInformationCallback::DefaultInformationCallback( bool verbose )
                                : _verbose( verbose )
{
}


gkg::DefaultInformationCallback::~DefaultInformationCallback()
{
}


const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
gkg::DefaultInformationCallback::duplicateLastInformation(
         const gkg::GlobalTractographyService& globalTractographyService ) const
{
  try
  {

    const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
      globalTractographyOptimizationInformation =
       globalTractographyService.getGlobalTractographyOptimizationInformation();

    ///////////////////////// Iteration ////////////////////////////////////////
    if ( globalTractographyOptimizationInformation->getIterations().empty() )
    {

      globalTractographyOptimizationInformation->setIteration( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setIteration( 
                globalTractographyOptimizationInformation->getLastIteration() );

    }

    /////////////////////// Temperature ////////////////////////////////////////
    if ( globalTractographyOptimizationInformation->getTemperatures().empty() )
    {

      globalTractographyOptimizationInformation->setTemperature( 1.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setTemperature( 
              globalTractographyOptimizationInformation->getLastTemperature() );

    }

    ///////////////////////// Operation type ///////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                   getOperationTypes().empty() )
    {

      globalTractographyOptimizationInformation->setOperationType( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setOperationType( 
            globalTractographyOptimizationInformation->getLastOperationType() );

    }

    /////////////////////// Spin glass count ///////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                  getSpinGlassCounts().empty() )
    {

      globalTractographyOptimizationInformation->setSpinGlassCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setSpinGlassCount( 
           globalTractographyOptimizationInformation->getLastSpinGlassCount() );

    }

    /////////////////////// Fiber portion count ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                               getFiberPortionCounts().empty() )
    {

      globalTractographyOptimizationInformation->setFiberPortionCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setFiberPortionCount( 
        globalTractographyOptimizationInformation->getLastFiberPortionCount() );

    }

    ///////////////////////// Creation count ///////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                   getCreationCounts().empty() )
    {

      globalTractographyOptimizationInformation->setCreationCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setCreationCount( 
            globalTractographyOptimizationInformation->getLastCreationCount() );

    }

    /////////////////////// Deletion count /////////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                   getDeletionCounts().empty() )
    {

      globalTractographyOptimizationInformation->setDeletionCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setDeletionCount( 
            globalTractographyOptimizationInformation->getLastDeletionCount() );

    }

    /////////////////////// Random move count //////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                 getRandomMoveCounts().empty() )
    {

      globalTractographyOptimizationInformation->setRandomMoveCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setRandomMoveCount( 
          globalTractographyOptimizationInformation->getLastRandomMoveCount() );

    }

    /////////////////////// Optimal move count /////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                getOptimalMoveCounts().empty() )
    {

      globalTractographyOptimizationInformation->setOptimalMoveCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setOptimalMoveCount( 
         globalTractographyOptimizationInformation->getLastOptimalMoveCount() );

    }

    /////////////////////// Connection count ///////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                 getConnectionCounts().empty() )
    {

      globalTractographyOptimizationInformation->setConnectionCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setConnectionCount( 
          globalTractographyOptimizationInformation->getLastConnectionCount() );

    }

    /////////////////////// Nothing to do count ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                getNothingToDoCounts().empty() )
    {

      globalTractographyOptimizationInformation->setNothingToDoCount( 0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setNothingToDoCount( 
         globalTractographyOptimizationInformation->getLastNothingToDoCount() );

    }

    /////////////////////// New external energy ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                              getNewExternalEnergies().empty() )
    {

      globalTractographyOptimizationInformation->setNewExternalEnergy( 0.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewExternalEnergy( 
        globalTractographyOptimizationInformation->getLastNewExternalEnergy() );

    }

    /////////////////////// New internal energy ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                              getNewInternalEnergies().empty() )
    {

      globalTractographyOptimizationInformation->setNewInternalEnergy( 0.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewInternalEnergy( 
        globalTractographyOptimizationInformation->getLastNewInternalEnergy() );

    }

    /////////////////////// New global energy //////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                                getNewGlobalEnergies().empty() )
    {

      globalTractographyOptimizationInformation->setNewGlobalEnergy( 0.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewGlobalEnergy( 
         globalTractographyOptimizationInformation->getLastNewGlobalEnergy() );

    }

    /////////////////////// Delta global energy ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                              getDeltaGlobalEnergies().empty() )
    {

      globalTractographyOptimizationInformation->setDeltaGlobalEnergy( 0.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setDeltaGlobalEnergy( 
        globalTractographyOptimizationInformation->getLastDeltaGlobalEnergy() );

    }

    /////////////////////// Green ratio ////////////////////////////////////////
    if ( globalTractographyOptimizationInformation->getGreenRatios().empty() )
    {

      globalTractographyOptimizationInformation->setGreenRatio( 0.0 );

    }
    else
    {

      globalTractographyOptimizationInformation->setGreenRatio( 
               globalTractographyOptimizationInformation->getLastGreenRatio() );

    }

    /////////////////////// Accept modification ////////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                              getAcceptModifications().empty() )
    {

      globalTractographyOptimizationInformation->setAcceptModification(
                                                                        false );

    }
    else
    {

      globalTractographyOptimizationInformation->setAcceptModification( 
       globalTractographyOptimizationInformation->getLastAcceptModification() );

    }

    /////////////////////// Chosen voxel ///////////////////////////////////////
    if ( globalTractographyOptimizationInformation->getChosenVoxels().empty() )
    {

      gkg::Vector3d< int32_t > voxel( 0, 0, 0 );
      globalTractographyOptimizationInformation->setChosenVoxel( voxel );

    }
    else
    {

      globalTractographyOptimizationInformation->setChosenVoxel( 
              globalTractographyOptimizationInformation->getLastChosenVoxel() );

    }

    /////////////////////// Chosen spin glass position /////////////////////////
    if ( globalTractographyOptimizationInformation->
                                         getChosenSpinGlassPositions().empty() )
    {

      gkg::Vector3d< float > spinGlassPosition( 0.0, 0.0, 0.0 );
      globalTractographyOptimizationInformation->setChosenSpinGlassPosition(
                                                            spinGlassPosition );

    }
    else
    {

      globalTractographyOptimizationInformation->setChosenSpinGlassPosition( 
        globalTractographyOptimizationInformation->
                                             getLastChosenSpinGlassPosition() );

    }

    /////////////////////// Chosen spin glass orientation //////////////////////
    if ( globalTractographyOptimizationInformation->
                                      getChosenSpinGlassOrientations().empty() )
    {

      gkg::Vector3d< float > spinGlassOrientation( 0.0, 0.0, 0.0 );
      globalTractographyOptimizationInformation->setChosenSpinGlassOrientation(
                                                         spinGlassOrientation );

    }
    else
    {

      globalTractographyOptimizationInformation->
        setChosenSpinGlassOrientation( 
          globalTractographyOptimizationInformation->
                                          getLastChosenSpinGlassOrientation() );

    }

    /////////////////////// New voxel //////////////////////////////////////////
    if ( globalTractographyOptimizationInformation->getNewVoxels().empty() )
    {

      gkg::Vector3d< int32_t > newVoxel( 0, 0, 0 );
      globalTractographyOptimizationInformation->setNewVoxel( newVoxel );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewVoxel( 
                 globalTractographyOptimizationInformation->getLastNewVoxel() );

    }

    /////////////////////// New spin glass position ////////////////////////////
    if ( globalTractographyOptimizationInformation->
                                            getNewSpinGlassPositions().empty() )
    {

      gkg::Vector3d< float > newSpinGlassPosition( 0.0, 0.0, 0.0 );
      globalTractographyOptimizationInformation->setNewSpinGlassPosition(
                                                         newSpinGlassPosition );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewSpinGlassPosition( 
          globalTractographyOptimizationInformation->
                                                getLastNewSpinGlassPosition() );

    }

    /////////////////////// New spin glass orientation /////////////////////////
    if ( globalTractographyOptimizationInformation->
                                         getNewSpinGlassOrientations().empty() )
    {

      gkg::Vector3d< float > newSpinGlassOrientation( 0.0, 0.0, 0.0 );
      globalTractographyOptimizationInformation->setNewSpinGlassOrientation(
                                                      newSpinGlassOrientation );

    }
    else
    {

      globalTractographyOptimizationInformation->setNewSpinGlassOrientation( 
            globalTractographyOptimizationInformation->
                                             getLastNewSpinGlassOrientation() );

    }

    return globalTractographyOptimizationInformation;

  }
  GKG_CATCH( "const gkg::RCPointer< "
             "gkg::GlobalTractographyOptimizationInformation >& "
             "gkg::DefaultInformationCallback::duplicateLastInformation( "
             "const gkg::GlobalTractographyService& globalTractographyService "
              ") const" );

}


