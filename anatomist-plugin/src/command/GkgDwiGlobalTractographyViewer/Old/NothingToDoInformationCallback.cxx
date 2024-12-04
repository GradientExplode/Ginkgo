#include "NothingToDoInformationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyParameterIds.h"
#include "GlobalTractographyEvent.h"
#include "GlobalTractographyOptimizationInformation.h"
//#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::NothingToDoInformationCallback::NothingToDoInformationCallback(
                                                 void* parameter, bool verbose )
                                    : gkg::RpcCallback(
                                        gkg::NOTHING_TO_DO_INFORMATION_CALLBACK,
                                        parameter ),
                                      gkg::DefaultInformationCallback( verbose )
{
}


gkg::NothingToDoInformationCallback::~NothingToDoInformationCallback()
{
}


void gkg::NothingToDoInformationCallback::receive( gkg::SocketStream& stream )
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

      ///////// iteration //////////////////////////////////////////////////////
      int32_t iteration = 0;
      stream.read( ( char* )&iteration, sizeof( int32_t ) );
      globalTractographyOptimizationInformation->setIteration( iteration );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_ITERATION );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_ITERATION );

      ///////// temperature ////////////////////////////////////////////////////
      float temperature = 0.0;
      stream.read( ( char* )&temperature, sizeof( float ) );
      globalTractographyOptimizationInformation->setTemperature( temperature );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_TEMPERATURE );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_TEMPERATURE );

      ///////// operation type /////////////////////////////////////////////////
      globalTractographyOptimizationInformation->setOperationType(
                                                          NOTHING_TO_DO_LABEL );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_OPERATION_TYPE );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_OPERATION_TYPE );

      ///////// nothing to do count ////////////////////////////////////////////
      globalTractographyOptimizationInformation->
        setNothingToDoCount( globalTractographyOptimizationInformation->
                                                getLastNothingToDoCount() + 1 );
      globalTractographyService->postEvent( GT_PARAMETER_OFFSET +
                                            GT_PARAMETER_NOTHING_TO_DO_COUNT );
      globalTractographyService->postEvent( GT_PLOT_OFFSET +
                                            GT_PARAMETER_NOTHING_TO_DO_COUNT );

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

      ///////// is chosen voxel valid //////////////////////////////////////////
      int32_t isChosenVoxelValid = 0;
      stream.read( ( char* )&isChosenVoxelValid, sizeof( int32_t ) );
      globalTractographyOptimizationInformation->setChosenVoxelValidity(
                                        ( isChosenVoxelValid ? true : false ) );

    }

  }
  GKG_CATCH( "void gkg::NothingToDoInformationCallback::receive( "
             "gkg::SocketStream& stream )" );

}

