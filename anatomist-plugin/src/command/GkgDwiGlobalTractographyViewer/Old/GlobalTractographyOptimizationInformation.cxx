#include "GlobalTractographyOptimizationInformation.h"
#include "GlobalTractographyParameterIds.h"
#include <gkg-core-exception/Exception.h>

#include <QtGui/QApplication>


gkg::GlobalTractographyOptimizationInformation::
                                     GlobalTractographyOptimizationInformation()
{
}


gkg::GlobalTractographyOptimizationInformation::
                                    ~GlobalTractographyOptimizationInformation()
{
}


void gkg::GlobalTractographyOptimizationInformation::setIteration(
                                                             int32_t iteration )
{

  try
  {

    _iterations.push_back( iteration );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::setIteration( "
             "int32_t iteration )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastIteration() const
{

  try
  {

    return _iterations.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastIteration() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getIterations() const
{

  try
  {

    return _iterations;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::getIterations() "
             "const" );

}


void gkg::GlobalTractographyOptimizationInformation::setTemperature(
                                                             float temperature )
{

  try
  {

    _temperatures.push_back( temperature );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::setTemperature( "
             "float temperature )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastTemperature() const
{

  try
  {

    return _temperatures.back();

  }
  GKG_CATCH( "float "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastTemperature() const" );

}


const std::list< float >& 
gkg::GlobalTractographyOptimizationInformation::getTemperatures() const
{

  try
  {

    return _temperatures;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getTemperatures() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setOperationType(
                                                         int32_t operationType )
{

  try
  {

    _operationTypes.push_back( operationType );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::"
             "setOperationType( int32_t operationType )" );

}


int32_t
gkg::GlobalTractographyOptimizationInformation::getLastOperationType() const
{

  try
  {

    return _operationTypes.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastOperationType() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getOperationTypes() const
{

  try
  {

    return _operationTypes;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getOperationTypes() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setSpinGlassCount(
                                                        int32_t spinGlassCount )
{

  try
  {

    _spinGlassCounts.push_back( spinGlassCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setSpinGlassCount( int32_t spinGlassCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastSpinGlassCount()
                                                                           const
{

  try
  {

    return _spinGlassCounts.back();

  }
  GKG_CATCH( "int32_t "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastSpinGlassCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getSpinGlassCounts() const
{

  try
  {

    return _spinGlassCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getSpinGlassCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setFiberPortionCount(
                                                     int32_t fiberPortionCount )
{

  try
  {

    _fiberPortionCounts.push_back( fiberPortionCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setFiberPortionCount( int32_t fiberPortionCount )" );

}


int32_t
gkg::GlobalTractographyOptimizationInformation::getLastFiberPortionCount() const
{

  try
  {

    return _fiberPortionCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastFiberPortionCount()const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getFiberPortionCounts() const
{

  try
  {

    return _fiberPortionCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getFiberPortionCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setCreationCount(
                                                         int32_t creationCount )
{

  try
  {

    _creationCounts.push_back( creationCount );


  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setCreationCount( int32_t creationCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastCreationCount()
                                                                           const
{

  try
  {

    return _creationCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastCreationCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getCreationCounts() const
{

  try
  {

    return _creationCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getCreationCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setDeletionCount(
                                                         int32_t deletionCount )
{

  try
  {

    _deletionCounts.push_back( deletionCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setDeletionCount( int32_t deletionCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastDeletionCount()
                                                                           const
{

  try
  {

    return _deletionCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastDeletionCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getDeletionCounts() const
{

  try
  {

    return _deletionCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getDeletionCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setRandomMoveCount(
                                                       int32_t randomMoveCount )
{

  try
  {

    _randomMoveCounts.push_back( randomMoveCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setRandomMoveCount( int32_t randomMoveCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastRandomMoveCount()
                                                                           const
{

  try
  {

    return _randomMoveCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastRandomMoveCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getRandomMoveCounts() const
{

  try
  {

    return _randomMoveCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getRandomMoveCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setOptimalMoveCount(
                                                      int32_t optimalMoveCount )
{

  try
  {

    _optimalMoveCounts.push_back( optimalMoveCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setOptimalMoveCount( int32_t optimalMoveCount )" );

}


int32_t
gkg::GlobalTractographyOptimizationInformation::getLastOptimalMoveCount() const
{

  try
  {

    return _optimalMoveCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastOptimalMoveCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getOptimalMoveCounts() const
{

  try
  {

    return _optimalMoveCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getOptimalMoveCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setConnectionCount(
                                                       int32_t connectionCount )
{

  try
  {

    _connectionCounts.push_back( connectionCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setConnectionCount( int32_t connectionCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastConnectionCount()
                                                                           const
{

  try
  {

    return _connectionCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastConnectionCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getConnectionCounts() const
{

  try
  {

    return _connectionCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getConnectionCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNothingToDoCount(
                                                       int32_t nothingToDoCount )
{

  try
  {

    _nothingToDoCounts.push_back( nothingToDoCount );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setNothingToDoCount( int32_t nothingToDoCount )" );

}


int32_t gkg::GlobalTractographyOptimizationInformation::getLastNothingToDoCount()
                                                                           const
{

  try
  {

    return _nothingToDoCounts.back();

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyOptimizationInformation::"
             "getLastNothingToDoCount() const" );

}


const std::list< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getNothingToDoCounts() const
{

  try
  {

    return _nothingToDoCounts;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNothingToDoCounts() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewExternalEnergy(
                                                       float newExternalEnergy )
{

  try
  {

    _newExternalEnergies.push_back( newExternalEnergy );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setNewExternalEnergy( float newExternalEnergy )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastNewExternalEnergy()
                                                                           const
{

  try
  {

    return _newExternalEnergies.back();

  }
  GKG_CATCH( "float gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewExternalEnergy() const" );

}


const std::list< float >&
gkg::GlobalTractographyOptimizationInformation::getNewExternalEnergies() const
{

  try
  {

    return _newExternalEnergies;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNewExternalEnergies() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewInternalEnergy(
                                                       float newInternalEnergy )
{

  try
  {

    _newInternalEnergies.push_back( newInternalEnergy );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setNewInternalEnergy( float newInternalEnergy )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastNewInternalEnergy()
                                                                           const
{

  try
  {

    return _newInternalEnergies.back();

  }
  GKG_CATCH( "float gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewInternalEnergy() const" );

}


const std::list< float >&
gkg::GlobalTractographyOptimizationInformation::getNewInternalEnergies() const
{

  try
  {

    return _newInternalEnergies;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNewInternalEnergies() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewGlobalEnergy(
                                                         float newGlobalEnergy )
{

  try
  {

    _newGlobalEnergies.push_back( newGlobalEnergy );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setNewGlobalEnergy( float newGlobalEnergy )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastNewGlobalEnergy()
                                                                           const
{

  try
  {

    return _newGlobalEnergies.back();

  }
  GKG_CATCH( "float gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewGlobalEnergy() const" );

}


const std::list< float >&
gkg::GlobalTractographyOptimizationInformation::getNewGlobalEnergies() const
{

  try
  {

    return _newGlobalEnergies;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNewGlobalEnergies() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setDeltaGlobalEnergy(
                                                       float deltaGlobalEnergy )
{

  try
  {

    _deltaGlobalEnergies.push_back( deltaGlobalEnergy );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setDeltaGlobalEnergy( float deltaGlobalEnergy )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastDeltaGlobalEnergy()
                                                                           const
{

  try
  {

    return _deltaGlobalEnergies.back();

  }
  GKG_CATCH( "float gkg::GlobalTractographyOptimizationInformation::"
             "getLastDeltaGlobalEnergy( const" );

}


const std::list< float >&
gkg::GlobalTractographyOptimizationInformation::getDeltaGlobalEnergies() const
{

  try
  {

    return _deltaGlobalEnergies;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getDeltaGlobalEnergies() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setGreenRatio(
                                                              float greenRatio )
{

  try
  {

    _greenRatios.push_back( greenRatio );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::setGreenRatio( "
             "float greenRatio )" );

}


float gkg::GlobalTractographyOptimizationInformation::getLastGreenRatio() const
{

  try
  {

    return _greenRatios.back();

  }
  GKG_CATCH( "float "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastGreenRatio() const" );

}


const std::list< float >&
gkg::GlobalTractographyOptimizationInformation::getGreenRatios() const
{

  try
  {

    return _greenRatios;

  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::GlobalTractographyOptimizationInformation::getGreenRatios()"
             "const" );

}


void gkg::GlobalTractographyOptimizationInformation::setAcceptModification(
                                                       bool acceptModification )
{

  try
  {

    _acceptModifications.push_back( acceptModification );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setAcceptModification( bool acceptModification )" );

}


bool gkg::GlobalTractographyOptimizationInformation::getLastAcceptModification()
                                                                           const
{

  try
  {

    return _acceptModifications.back();

  }
  GKG_CATCH( "bool gkg::GlobalTractographyOptimizationInformation::"
             "getLastAcceptModification() const" );

}


const std::list< bool >&
gkg::GlobalTractographyOptimizationInformation::getAcceptModifications() const
{

  try
  {

    return _acceptModifications;

  }
  GKG_CATCH( "const std::list< bool >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getAcceptModifications() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setChosenVoxel(
                                   const gkg::Vector3d< int32_t >& chosenVoxel )
{

  try
  {

    _chosenVoxels.push_back( chosenVoxel );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::setChosenVoxel( "
             "const gkg::Vector3d< int32_t >& chosenVoxel )" );

}


const gkg::Vector3d< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getLastChosenVoxel() const
{

  try
  {

    return _chosenVoxels.back();

  }
  GKG_CATCH( "const gkg::Vector3d< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastChosenVoxel() const" );

}


void 
gkg::GlobalTractographyOptimizationInformation::setChosenVoxelValidity(
                                                                 bool validity )
{

  try
  {

    _isLastChosenVoxelValid = validity;

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::"
             "setChosenVoxelValidity( bool validity )" );

}


bool 
gkg::GlobalTractographyOptimizationInformation::isLastChosenVoxelValid() const
{

  try
  {

    return _isLastChosenVoxelValid;

  }
  GKG_CATCH( "bool "
             "gkg::GlobalTractographyOptimizationInformation::"
             "isLastChosenVoxelValid() const" );

}


const std::list< gkg::Vector3d< int32_t > >&
gkg::GlobalTractographyOptimizationInformation::getChosenVoxels() const
{

  try
  {

    return _chosenVoxels;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< int32_t > >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getChosenVoxels() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setChosenSpinGlassPosition(
                         const gkg::Vector3d< float >& chosenSpinGlassPosition )
{

  try
  {

    _chosenSpinGlassPositions.push_back( chosenSpinGlassPosition );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setChosenSpinGlassPosition( "
             "const gkg::Vector3d< float >& chosenSpinGlassPosition )" );

}


const gkg::Vector3d< float >&
gkg::GlobalTractographyOptimizationInformation::getLastChosenSpinGlassPosition()
                                                                           const
{

  try
  {

    return _chosenSpinGlassPositions.back();

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastChosenSpinGlassPosition() const" );

}


const std::list< gkg::Vector3d< float > >&
gkg::GlobalTractographyOptimizationInformation::getChosenSpinGlassPositions()
                                                                           const
{

  try
  {

    return _chosenSpinGlassPositions;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< float > >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getChosenSpinGlassPositions() const" );

}


void gkg::GlobalTractographyOptimizationInformation::
                                                  setChosenSpinGlassOrientation(
                      const gkg::Vector3d< float >& chosenSpinGlassOrientation )
{

  try
  {

    _chosenSpinGlassOrientations.push_back( chosenSpinGlassOrientation );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::"
             "setChosenSpinGlassOrientation( "
             "const gkg::Vector3d< float >& chosenSpinGlassOrientation )" );

}


const gkg::Vector3d< float >&
gkg::GlobalTractographyOptimizationInformation::
                                       getLastChosenSpinGlassOrientation() const
{

  try
  {

    return _chosenSpinGlassOrientations.back();

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastChosenSpinGlassOrientation() const" );

}


const std::list< gkg::Vector3d< float > >&
gkg::GlobalTractographyOptimizationInformation::getChosenSpinGlassOrientations()
                                                                           const
{

  try
  {

    return _chosenSpinGlassOrientations;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< float > >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getChosenSpinGlassOrientations() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewVoxel(
                                      const gkg::Vector3d< int32_t >& newVoxel )
{

  try
  {

    _newVoxels.push_back( newVoxel );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::setNewVoxel( "
             "const gkg::Vector3d< int32_t >& newVoxel )" );

}


const gkg::Vector3d< int32_t >&
gkg::GlobalTractographyOptimizationInformation::getLastNewVoxel() const
{

  try
  {

    return _newVoxels.back();

  }
  GKG_CATCH( "const gkg::Vector3d< int32_t >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewVoxel() const" );

}


const std::list< gkg::Vector3d< int32_t > >&
gkg::GlobalTractographyOptimizationInformation::getNewVoxels() const
{

  try
  {

    return _newVoxels;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< int32_t > >& "
             "gkg::GlobalTractographyOptimizationInformation::getNewVoxels() "
             "const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewSpinGlassPosition(
                            const gkg::Vector3d< float >& newSpinGlassPosition )
{

  try
  {

    _newSpinGlassPositions.push_back( newSpinGlassPosition );

  }
  GKG_CATCH( "void gkg::GlobalTractographyOptimizationInformation::"
             "setNewSpinGlassPosition( "
             "const gkg::Vector3d< float >& newSpinGlassPosition )" );

}


const gkg::Vector3d< float >&
gkg::GlobalTractographyOptimizationInformation::getLastNewSpinGlassPosition()
                                                                           const
{

  try
  {

    return _newSpinGlassPositions.back();

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewSpinGlassPosition() const" );

}


const std::list< gkg::Vector3d< float > >&
gkg::GlobalTractographyOptimizationInformation::getNewSpinGlassPositions() const
{

  try
  {

    return _newSpinGlassPositions;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< float > >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNewSpinGlassPositions() const" );

}


void gkg::GlobalTractographyOptimizationInformation::setNewSpinGlassOrientation(
                         const gkg::Vector3d< float >& newSpinGlassOrientation )
{

  try
  {

    _newSpinGlassOrientations.push_back( newSpinGlassOrientation );

  }
  GKG_CATCH( "void "
             "gkg::GlobalTractographyOptimizationInformation::"
             "setNewSpinGlassOrientation( "
             "const gkg::Vector3d< float >& newSpinGlassOrientation )" );

}


const gkg::Vector3d< float >&
gkg::GlobalTractographyOptimizationInformation::getLastNewSpinGlassOrientation()
                                                                           const
{

  try
  {

    return _newSpinGlassOrientations.back();

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getLastNewSpinGlassOrientation() " 
             "const" );

}


const std::list< gkg::Vector3d< float > >&
gkg::GlobalTractographyOptimizationInformation::getNewSpinGlassOrientations()
                                                                           const
{

  try
  {

    return _newSpinGlassOrientations;

  }
  GKG_CATCH( "const std::list< gkg::Vector3d< float > >& "
             "gkg::GlobalTractographyOptimizationInformation::"
             "getNewSpinGlassOrientations() const" );

}


