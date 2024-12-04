#include "SpinGlassRandomMoveInformationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyParameterIds.h"
#include "GlobalTractographyEvent.h"
#include "GlobalTractographyOptimizationInformation.h"
//#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassRandomMoveInformationCallback::
                                         SpinGlassRandomMoveInformationCallback(
                                                 void* parameter, bool verbose )
                           : gkg::RpcCallback(
                               gkg::SPIN_GLASS_RANDOM_MOVE_INFORMATION_CALLBACK,
                               parameter ),
                             gkg::DefaultInformationCallback( verbose )
{
}


gkg::SpinGlassRandomMoveInformationCallback::
                                       ~SpinGlassRandomMoveInformationCallback()
{
}


void gkg::SpinGlassRandomMoveInformationCallback::receive(
                                                     gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    if ( globalTractographyService )
    {

      const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
        globalTractographyOptimizationInformation = 
                   this->duplicateLastInformation( *globalTractographyService );

      ////// iteration /////////////////////////////////////////////////////////
      int32_t iteration = 0;
      stream.read( ( char* )&iteration, sizeof( int32_t ) );
      globalTractographyOptimizationInformation->setIteration( iteration );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_ITERATION );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_ITERATION );

      /////// temperature //////////////////////////////////////////////////////
      float temperature = 0.0;
      stream.read( ( char* )&temperature, sizeof( float ) );
      globalTractographyOptimizationInformation->setTemperature( temperature );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_TEMPERATURE );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_TEMPERATURE );

      ///////// operation type /////////////////////////////////////////////////
      globalTractographyOptimizationInformation->setOperationType(
                                                 SPIN_GLASS_RANDOM_MOVE_LABEL );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_OPERATION_TYPE );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_OPERATION_TYPE );

      ///////// random move count //////////////////////////////////////////////
      globalTractographyOptimizationInformation->
        setRandomMoveCount( globalTractographyOptimizationInformation->
                                                 getLastRandomMoveCount() + 1 );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_RANDOM_MOVE_COUNT );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_RANDOM_MOVE_COUNT );

      ///////// new external energy ////////////////////////////////////////////
      float newExternalEnergy = 0.0;
      stream.read( ( char* )&newExternalEnergy, sizeof( float ) );
      globalTractographyOptimizationInformation->setNewExternalEnergy(
                                                            newExternalEnergy );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_NEW_EXTERNAL_ENERGY );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_NEW_EXTERNAL_ENERGY );

      ///////// new internal energy ////////////////////////////////////////////
      float newInternalEnergy = 0.0;
      stream.read( ( char* )&newInternalEnergy, sizeof( float ) );
      globalTractographyOptimizationInformation->setNewInternalEnergy(
                                                            newInternalEnergy );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_NEW_INTERNAL_ENERGY );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_NEW_INTERNAL_ENERGY );

      ///////// new global energy //////////////////////////////////////////////
      float newGlobalEnergy = 0.0;
      stream.read( ( char* )&newGlobalEnergy, sizeof( float ) );
      globalTractographyOptimizationInformation->setNewGlobalEnergy(
                                                              newGlobalEnergy );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_NEW_GLOBAL_ENERGY );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_NEW_GLOBAL_ENERGY );

      ///////// delta global energy ////////////////////////////////////////////
      float deltaGlobalEnergy = 0.0;
      stream.read( ( char* )&deltaGlobalEnergy, sizeof( float ) );  
      globalTractographyOptimizationInformation->setDeltaGlobalEnergy(
                                                            deltaGlobalEnergy );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_DELTA_GLOBAL_ENERGY );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_DELTA_GLOBAL_ENERGY );

      ///////// green Ratio ////////////////////////////////////////////////////
      float greenRatio = 0.0;
      stream.read( ( char* )&greenRatio, sizeof( float ) );
      globalTractographyOptimizationInformation->setGreenRatio( greenRatio );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_GREEN_RATIO );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_GREEN_RATIO );

      ///////// accept modification ////////////////////////////////////////////
      int32_t acceptModificationInteger = 0;
      stream.read( ( char* )&acceptModificationInteger, sizeof( int32_t ) );
      bool acceptModification;
      if ( acceptModificationInteger == 0 )
      {

        acceptModification = false;

      }
      else if ( acceptModificationInteger == 1 )
      {

        acceptModification = true;

      }  
      globalTractographyOptimizationInformation->setAcceptModification(
                                                           acceptModification );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_ACCEPT_MODIFICATION );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_ACCEPT_MODIFICATION );

      ///////// chosen voxel ///////////////////////////////////////////////////
      gkg::Vector3d< int32_t > chosenVoxel( 0, 0, 0 );
      stream.read( ( char* )&( chosenVoxel.x ), sizeof( int32_t ) );
      stream.read( ( char* )&( chosenVoxel.y ), sizeof( int32_t ) );
      stream.read( ( char* )&( chosenVoxel.z ), sizeof( int32_t ) );
      globalTractographyOptimizationInformation->setChosenVoxel( chosenVoxel );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_CHOSEN_VOXEL );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_CHOSEN_VOXEL );

      ///////// old spin glass position ////////////////////////////////////////
      gkg::Vector3d< float > oldSpinGlassPosition( 0.0, 0.0, 0.0 );
      stream.read( ( char* )&( oldSpinGlassPosition.x ), sizeof( float ) );
      stream.read( ( char* )&( oldSpinGlassPosition.y ), sizeof( float ) );
      stream.read( ( char* )&( oldSpinGlassPosition.z ), sizeof( float ) );
      globalTractographyOptimizationInformation->setChosenSpinGlassPosition(
                                                         oldSpinGlassPosition );
      globalTractographyService->postEvent(
                                      GT_PARAMETER_OFFSET +
                                      GT_PARAMETER_CHOSEN_SPIN_GLASS_POSITION );
      globalTractographyService->postEvent(
                                      GT_PLOT_OFFSET +
                                      GT_PARAMETER_CHOSEN_SPIN_GLASS_POSITION );

      ///////// old spin glass orientation /////////////////////////////////////
      gkg::Vector3d< float > oldSpinGlassOrientation( 0.0, 0.0, 0.0 );
      stream.read( ( char* )&( oldSpinGlassOrientation.x ), sizeof( float ) );
      stream.read( ( char* )&( oldSpinGlassOrientation.y ), sizeof( float ) );
      stream.read( ( char* )&( oldSpinGlassOrientation.z ), sizeof( float ) );
      globalTractographyOptimizationInformation->setChosenSpinGlassOrientation(
                                                      oldSpinGlassOrientation );
      globalTractographyService->postEvent(
                                   GT_PARAMETER_OFFSET +
                                   GT_PARAMETER_CHOSEN_SPIN_GLASS_ORIENTATION );
      globalTractographyService->postEvent(
                                   GT_PLOT_OFFSET +
                                   GT_PARAMETER_CHOSEN_SPIN_GLASS_ORIENTATION );

      ///////// new voxel //////////////////////////////////////////////////////
      gkg::Vector3d< int32_t > newVoxel( 0, 0, 0 );
      stream.read( ( char* )&( newVoxel.x ), sizeof( int32_t ) );
      stream.read( ( char* )&( newVoxel.y ), sizeof( int32_t ) );
      stream.read( ( char* )&( newVoxel.z ), sizeof( int32_t ) );
      globalTractographyOptimizationInformation->setNewVoxel( newVoxel );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_NEW_VOXEL );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_NEW_VOXEL );

      ///////// new spin glass position ////////////////////////////////////////
      gkg::Vector3d< float > newSpinGlassPosition( 0.0, 0.0, 0.0 );
      stream.read( ( char* )&( newSpinGlassPosition.x ), sizeof( float ) );
      stream.read( ( char* )&( newSpinGlassPosition.y ), sizeof( float ) );
      stream.read( ( char* )&( newSpinGlassPosition.z ), sizeof( float ) );  
      globalTractographyOptimizationInformation->setNewSpinGlassPosition(
                                                         newSpinGlassPosition );
      globalTractographyService->postEvent(
                                         GT_PARAMETER_OFFSET +
                                         GT_PARAMETER_NEW_SPIN_GLASS_POSITION );
      globalTractographyService->postEvent(
                                         GT_PLOT_OFFSET +
                                         GT_PARAMETER_NEW_SPIN_GLASS_POSITION );

      ///////// new spin glass orientation /////////////////////////////////////
      gkg::Vector3d< float > newSpinGlassOrientation( 0.0, 0.0, 0.0 );
      stream.read( ( char* )&( newSpinGlassOrientation.x ), sizeof( float ) );
      stream.read( ( char* )&( newSpinGlassOrientation.y ), sizeof( float ) );
      stream.read( ( char* )&( newSpinGlassOrientation.z ), sizeof( float ) );  
      globalTractographyOptimizationInformation->setNewSpinGlassOrientation(
                                                      newSpinGlassOrientation );
      globalTractographyService->postEvent(
                                      GT_PARAMETER_OFFSET +
                                      GT_PARAMETER_NEW_SPIN_GLASS_ORIENTATION );
      globalTractographyService->postEvent(
                                      GT_PLOT_OFFSET +
                                      GT_PARAMETER_NEW_SPIN_GLASS_ORIENTATION );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassRandomMoveInformationCallback::receive( "
             "gkg::SocketStream& stream )" );

}

