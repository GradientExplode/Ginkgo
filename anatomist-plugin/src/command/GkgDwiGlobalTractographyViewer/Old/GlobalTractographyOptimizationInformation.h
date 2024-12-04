#ifndef _gkg_dmri_GkgDwiGlobalTractographyViewer_GlobalTractographyOptimizationInformation_h_
#define _gkg_dmri_GkgDwiGlobalTractographyViewer_GlobalTractographyOptimizationInformation_h_


#include <gkg-communication-socket/SocketService.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <list>


namespace gkg
{


class GlobalTractographyOptimizationInformation
{

  public:

    GlobalTractographyOptimizationInformation();
    virtual ~GlobalTractographyOptimizationInformation();

    // Iteration
    void setIteration( int32_t iteration );
    int32_t getLastIteration() const;
    const std::list< int32_t >& getIterations() const;

    // Temperature
    void setTemperature( float temperature );
    float getLastTemperature() const;
    const std::list< float >& getTemperatures() const;

    // Operation type
    void setOperationType( int32_t operationType );
    int32_t getLastOperationType() const;
    const std::list< int32_t >& getOperationTypes() const;

    // Spin glass count
    void setSpinGlassCount( int32_t spinGlassCount );
    int32_t getLastSpinGlassCount() const;
    const std::list< int32_t >& getSpinGlassCounts() const;

    // Fiber portion count
    void setFiberPortionCount( int32_t fiberPortionCount );
    int32_t getLastFiberPortionCount() const;
    const std::list< int32_t >& getFiberPortionCounts() const;

    // Creation count
    void setCreationCount( int32_t creationCount );
    int32_t getLastCreationCount() const;
    const std::list< int32_t >& getCreationCounts() const;

    // Deletion count
    void setDeletionCount( int32_t deletionCount );
    int32_t getLastDeletionCount() const;
    const std::list< int32_t >& getDeletionCounts() const;

    // Random move count
    void setRandomMoveCount( int32_t randomMoveCount );
    int32_t getLastRandomMoveCount() const;
    const std::list< int32_t >& getRandomMoveCounts() const;

    // Optimal move count
    void setOptimalMoveCount( int32_t optimalMoveCount );
    int32_t getLastOptimalMoveCount() const;
    const std::list< int32_t >& getOptimalMoveCounts() const;

    // Connection count
    void setConnectionCount( int32_t connectionCount );
    int32_t getLastConnectionCount() const;
    const std::list< int32_t >& getConnectionCounts() const;

    // Nothing to do count
    void setNothingToDoCount( int32_t nothingToDoCount );
    int32_t getLastNothingToDoCount() const;
    const std::list< int32_t >& getNothingToDoCounts() const;

    // New external energy
    void setNewExternalEnergy( float newExternalEnergy );
    float getLastNewExternalEnergy() const;
    const std::list< float >& getNewExternalEnergies() const;

    // New internal energy
    void setNewInternalEnergy( float newInternalEnergy );
    float getLastNewInternalEnergy() const;
    const std::list< float >& getNewInternalEnergies() const;

    // New global energy
    void setNewGlobalEnergy( float newGlobalEnergy );
    float getLastNewGlobalEnergy() const;
    const std::list< float >& getNewGlobalEnergies() const;

    // Delta global energy
    void setDeltaGlobalEnergy( float deltaGlobalEnergy );
    float getLastDeltaGlobalEnergy() const;
    const std::list< float >& getDeltaGlobalEnergies() const;

    // Green ratio
    void setGreenRatio( float greenRatio );
    float getLastGreenRatio() const;
    const std::list< float >& getGreenRatios() const;

    // Accept modification
    void setAcceptModification( bool acceptModification );
    bool getLastAcceptModification() const;
    const std::list< bool >& getAcceptModifications() const;

    // Chosen voxel
    void setChosenVoxel( const Vector3d< int32_t >& chosenVoxel );
    const Vector3d< int32_t >& getLastChosenVoxel() const;
    void setChosenVoxelValidity( bool validity );
    bool isLastChosenVoxelValid() const;
    const std::list< Vector3d< int32_t > >& getChosenVoxels() const;

    // Chosen spin glass position
    void setChosenSpinGlassPosition(
                             const Vector3d< float >& chosenSpinGlassPosition );
    const Vector3d< float >& getLastChosenSpinGlassPosition() const;
    const std::list< Vector3d< float > >& getChosenSpinGlassPositions() const;

    // Chosen spin glass orientation
    void setChosenSpinGlassOrientation(
                          const Vector3d< float >& chosenSpinGlassOrientation );
    const Vector3d< float >& getLastChosenSpinGlassOrientation() const;
    const std::list< Vector3d< float > >&
                                         getChosenSpinGlassOrientations() const;

    // New voxel
    void setNewVoxel( const Vector3d< int32_t >& newVoxel );
    const Vector3d< int32_t >& getLastNewVoxel() const;
    const std::list< Vector3d< int32_t > >& getNewVoxels() const;

    // New spin glass position
    void setNewSpinGlassPosition(
                                const Vector3d< float >& newSpinGlassPosition );
    const Vector3d< float >& getLastNewSpinGlassPosition() const;
    const std::list< Vector3d< float > >& getNewSpinGlassPositions() const;

    // New spin glass orientation
    void setNewSpinGlassOrientation(
                             const Vector3d< float >& newSpinGlassOrientation );
    const Vector3d< float >& getLastNewSpinGlassOrientation() const;
    const std::list< Vector3d< float > >& getNewSpinGlassOrientations() const;

  protected:

    Vector3d< double > _resolution;

    std::list< int32_t > _iterations;
    std::list< float > _temperatures;

    std::list< int32_t > _operationTypes;

    std::list< int32_t > _spinGlassCounts;
    std::list< int32_t > _fiberPortionCounts;

    std::list< int32_t > _creationCounts;
    std::list< int32_t > _deletionCounts;
    std::list< int32_t > _randomMoveCounts;
    std::list< int32_t > _optimalMoveCounts;
    std::list< int32_t > _connectionCounts;
    std::list< int32_t > _nothingToDoCounts;

    std::list< float > _newExternalEnergies;
    std::list< float > _newInternalEnergies;
    std::list< float > _newGlobalEnergies;
    std::list< float > _deltaGlobalEnergies;

    std::list< float > _greenRatios;
    std::list< bool > _acceptModifications;

    std::list< Vector3d< int32_t > > _chosenVoxels;
    std::list< Vector3d< float > > _chosenSpinGlassPositions;
    std::list< Vector3d< float > > _chosenSpinGlassOrientations;
    bool _isLastChosenVoxelValid;

    std::list< Vector3d< int32_t > > _newVoxels;
    std::list< Vector3d< float > > _newSpinGlassPositions;
    std::list< Vector3d< float > > _newSpinGlassOrientations;

};


}


#endif

