#include "GlobalTractographyService.h"
#include "GlobalTractographyServiceThread.h"
#include "GlobalTractographyViewer.h"
#include "GlobalTractographyParameterIds.h"
#include "AnatomicalPriorsCallback.h"
#include "InitialSpinGlassConfigurationCallback.h"
#include "NothingToDoInformationCallback.h"
#include "SpinGlassConnectionCreationCallback.h"
#include "SpinGlassConnectionDeletionCallback.h"
#include "SpinGlassConnectionInformationCallback.h"
#include "SpinGlassCreationInformationCallback.h"
#include "SpinGlassCreationCallback.h"
#include "SpinGlassDeletionCallback.h"
#include "SpinGlassDeletionInformationCallback.h"
#include "SpinGlassMoveCallback.h"
#include "SpinGlassRandomMoveInformationCallback.h"
#include "SpinGlassOptimalMoveInformationCallback.h"
#include "GlobalTractographyEvent.h"
#include "GlobalTractographyOptimizationInformation.h"
#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-communication-rpc/RpcCommandReader.h>
#include <gkg-core-exception/Exception.h>

#include <QtGui/QApplication>

#define GT_SERVICE_PORT 8002


gkg::GlobalTractographyService::GlobalTractographyService( 
                       gkg::GlobalTractographyViewer* globalTractographyViewer )
                               : gkg::SocketService( GT_SERVICE_PORT ),
                                 _globalTractographyViewer(
                                                      globalTractographyViewer )
{

  try
  {

    _spinGlassAndConnectionMap.reset( new gkg::SpinGlassAndConnectionMap() );

    _globalTractographyOptimizationInformation.reset(
                         new gkg::GlobalTractographyOptimizationInformation() );

    _globalTractographyServiceThread =
                               new gkg::GlobalTractographyServiceThread( this );
    _globalTractographyServiceThread->launch();

  }
  GKG_CATCH( "gkg::GlobalTractographyService::GlobalTractographyService( "
             "gkg::GlobalTractographyViewer* globalTractographyViewer )" );

}


gkg::GlobalTractographyService::~GlobalTractographyService()
{

  try
  {

    _globalTractographyServiceThread->cancel();
    delete _globalTractographyServiceThread;

  }
  GKG_CATCH( "gkg::GlobalTractographyService::~GlobalTractographyService()" );

}


void gkg::GlobalTractographyService::postEvent( int32_t eventId )
{

  try
  {

    QApplication::postEvent( _globalTractographyViewer,
                             new gkg::GlobalTractographyEvent( eventId ) );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::postEvent( "
             "int32_t eventId )" )

}


void gkg::GlobalTractographyService::createSpinGlass(
                            uint64_t spinGlassIdentity,
                            const gkg::Vector3d< float >& spinGlassPosition,
                            const gkg::Vector3d< float >& spinGlassOrientation )
{

  try
  {

    _spinGlassAndConnectionMap->addSpinGlass( spinGlassIdentity,
                                              spinGlassPosition,
                                              spinGlassOrientation );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::createSpinGlass( "
             "uint64_t spinGlassIdentity, "
             "const gkg::Vector3d< float >& spinGlassPosition, "
             "const gkg::Vector3d< float >& spinGlassOrientation )" );

}


void gkg::GlobalTractographyService::deleteSpinGlass(
                                                    uint64_t spinGlassIdentity )
{

  try
  {

    _spinGlassAndConnectionMap->removeSpinGlass( spinGlassIdentity );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::deleteSpinGlass( "
             "uint64_t spinGlassIdentity )" );

}


void gkg::GlobalTractographyService::moveSpinGlass(
                         uint64_t spinGlassIdentity,
                         const gkg::Vector3d< float >& newSpinGlassPosition,
                         const gkg::Vector3d< float >& newSpinGlassOrientation )
{

  try
  {

    _spinGlassAndConnectionMap->moveSpinGlass( spinGlassIdentity,
                                               newSpinGlassPosition,
                                               newSpinGlassOrientation );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::moveSpinGlass( "
             "uint64_t spinGlassIdentity, "
             "const gkg::Vector3d< float >& newSpinGlassPosition, "
             "const gkg::Vector3d< float >& newSpinGlassOrientation )" );

}


void gkg::GlobalTractographyService::createSpinGlassConnection(
                         uint64_t spinGlassConnectionIdentity,
                         uint64_t spinGlass1Identity,
                         gkg::SpinGlass::ExtremityType spinGlassExtremityType1,
                         uint64_t spinGlass2Identity,
                         gkg::SpinGlass::ExtremityType spinGlassExtremityType2 )
{

  try
  {

    _spinGlassAndConnectionMap->addSpinGlassConnection(
                                                    spinGlassConnectionIdentity,
                                                    spinGlass1Identity,
                                                    spinGlassExtremityType1,
                                                    spinGlass2Identity,
                                                    spinGlassExtremityType2 );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::createSpinGlassConnection( "
             "uint64_t spinGlassConnectionIdentity, "
             "uint64_t spinGlass1Identity, "
             "int32_t spinGlassExtremityType1, "
             "uint64_t spinGlass2Identity, "
             "int32_t spinGlassExtremityType2  )" );

}


void gkg::GlobalTractographyService::deleteSpinGlassConnection(
                                          uint64_t spinGlassConnectionIdentity )
{

  try
  {

    _spinGlassAndConnectionMap->removeSpinGlassConnection(
                                                  spinGlassConnectionIdentity );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::deleteSpinGlassConnection( "
             "uint64_t spinGlassConnectionIdentity )" );

}


const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >&
gkg::GlobalTractographyService::getSpinGlassAndConnectionMap() const
{

  try
  {

    return _spinGlassAndConnectionMap;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >& "
             "gkg::GlobalTractographyService::getSpinGlassAndConnectionMap() "
             "const" );

}


const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
gkg::GlobalTractographyService::getGlobalTractographyOptimizationInformation()
                                                                           const
{

  try
  {

    return _globalTractographyOptimizationInformation;

  }
  GKG_CATCH( "const gkg::RCPointer< "
             "gkg::GlobalTractographyOptimizationInformation >& "
             "gkg::GlobalTractographyService::"
             "getGlobalTractographyOptimizationInformation() const" );

}


void gkg::GlobalTractographyService::createReader( int32_t fd )
{

  try
  {

    _rpcCommandReader.reset( new gkg::RpcCommandReader( fd ) );

    ////////////////////////////////////////////////////////////////////////////
    _rpcCommandReader->add( new gkg::AnatomicalPriorsCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );

    ////////////////////////////////////////////////////////////////////////////
    _rpcCommandReader->add( new gkg::InitialSpinGlassConfigurationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );

    ////////////////////////////////////////////////////////////////////////////
    _rpcCommandReader->add( new gkg::SpinGlassCreationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassDeletionCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassMoveCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassConnectionCreationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassConnectionDeletionCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );

    ////////////////////////////////////////////////////////////////////////////
    _rpcCommandReader->add( new gkg::SpinGlassCreationInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassDeletionInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassRandomMoveInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassOptimalMoveInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::SpinGlassConnectionInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );
    _rpcCommandReader->add( new gkg::NothingToDoInformationCallback(
                                     ( void* )this,
                                     _globalTractographyViewer->isVerbose() ) );

  }
  GKG_CATCH( "void gkg::GlobalTractographyService::createReader( "
             "int32_t fd )" );

}


#undef GT_SERVICE_PORT
