#ifndef _gkg_dmri_global_tractography_GlobalTractographyAlgorithm_h_
#define _gkg_dmri_global_tractography_GlobalTractographyAlgorithm_h_


#include <gkg-dmri-tractography/TractographyAlgorithm.h>
#include <gkg-dmri-global-tractography/AnatomicalPriors.h>
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-container/CartesianField.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-pattern/UniqueIdentityGenerator.h>
#include <gkg-communication-rpc/RpcCommandWriter.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <gkg-communication-thread/LoopContext.h>
#include <deque>
#include <set>


namespace gkg
{


template < class L, class Compare > class GlobalTractographyAlgorithm;


//
// class GlobalTractographyInitializationGauge
//


class GlobalTractographyInitializationGauge : public ThreadGauge< int32_t >
{

  public:

    GlobalTractographyInitializationGauge( int32_t maxCount );

    void add( int32_t value );
    void reset();

  private:

    int32_t _count;
    int32_t _maxCount;

};


//
// class GlobalTractographyInitializationContext
//


template < class L, class Compare = std::less< L > >
class GlobalTractographyInitializationContext : public LoopContext< int32_t >
{

  public:

    GlobalTractographyInitializationContext(
                 GlobalTractographyInitializationGauge& gauge,
                 const GlobalTractographyAlgorithm< L, Compare >& 
                                                    globalTractographyAlgorithm,
                 std::map< Vector3d< float >,
                           std::pair< std::vector< float >,
                                      std::pair< float, float > >,
                           Vector3dCompare< float > >& buffer,
                 std::vector< Vector3d< int32_t > >& voxels,
                 const RCPointer< SeedingStrategy >& seedingStrategy,
                 const Vector3d< double >& roiResolution,
                 const Transform3d< float >& transform3d,
                 OdfContinuousField& odfContinuousField,
                 const NumericalAnalysisImplementationFactory* factory,
                 const RCPointer< RandomGenerator >& randomGenerator,
                 float& externalEnergy,
                 float& globalMinimumOdfProbability,
                 float& globalMaximumOdfProbability,
                 RCPointer< UniqueIdentityGenerator >& spinGlassUIDGenerator,
                 CartesianField< std::set< RCPointer< SpinGlass > > >& 
                                                        spinGlassCartesianField,
                 std::set< RCPointer< SpinGlass > >& isolatedSpinGlasses,
                 bool& verbose );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const GlobalTractographyAlgorithm< L, Compare >&
                                                   _globalTractographyAlgorithm;
    std::map< Vector3d< float >,
              std::pair< std::vector< float >,
                         std::pair< float, float > >,
              Vector3dCompare< float > >& _buffer;
    std::vector< Vector3d< int32_t > >& _voxels;
    const RCPointer< SeedingStrategy >& _seedingStrategy;
    const Vector3d< double >& _roiResolution;
    const Transform3d< float >& _transform3d;
    OdfContinuousField& _odfContinuousField;
    const NumericalAnalysisImplementationFactory* _factory;
    const RCPointer< RandomGenerator >& _randomGenerator;
    float& _externalEnergy;
    float& _globalMinimumOdfProbability;
    float& _globalMaximumOdfProbability;
    RCPointer< UniqueIdentityGenerator >& _spinGlassUIDGenerator;
    CartesianField< std::set< RCPointer< SpinGlass > > >& 
                                                       _spinGlassCartesianField;
    std::set< RCPointer< SpinGlass > >& _isolatedSpinGlasses;
    bool& _verbose;
    const gkg::OrientationSet& _outputOrientationSet;

};


//
// class GlobalTractographyAlgorithm
//


template < class L, class Compare = std::less< L > >
class GlobalTractographyAlgorithm :
                  public TractographyAlgorithm< L, Compare >,
                  public Creator4Arg< GlobalTractographyAlgorithm< L, Compare >,
                                      TractographyAlgorithm< L, Compare >,
                                      const Vector3d< int32_t >&,
                                      const Vector3d< double >&,
                                      const std::vector< double >&,
                                      const std::vector< std::string >& >
{

  public:

    enum ModificationProposal
    {

      NothingToDoProposal,
      SpinGlassConnectionProposal,
      SpinGlassOptimalMotionProposal,
      SpinGlassRandomMotionProposal,
      SpinGlassDeathProposal

    };

    enum SpinGlassConfiguration
    {

      Isolated,
      OneSideAttached,
      FullyAttached

    };

    enum ConnectionType
    {

      SimpleConnection,
      InvalidConnection,
      ValidCreationConnection,
      InvalidCreationConnection,
      ReconnectionToOlder,
      UndefinedConnection

    };

    GlobalTractographyAlgorithm(
                           const Vector3d< int32_t >& size,
                           const Vector3d< double >& resolution,
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );
    virtual ~GlobalTractographyAlgorithm();

    const std::vector< double >& getScalarParameters() const;
    const std::vector< std::string >& getStringParameters() const;

    virtual void prepare(
                  OdfCartesianField& odfCartesianField,
                  const Volume< int16_t >& computingMask,
                  const Transform3d< float >&
                                            transform3dFromRoisVoxelToOdfsReal,
                  const Transform3d< float >&
                                            transform3dFromRoisVoxelToMaskVoxel,
                  const Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                  const Transform3d< float >&
                                            transform3dFromOdfsRealToRoisVoxel,
                  bool verbose );

    virtual void track( const SiteMap< L, int32_t, Compare >& rois,
                        const SiteMap< L, int32_t, Compare >& subRois,
                        const RCPointer< SeedingStrategy >& seedingStrategy,
                        const RCPointer< StoppingStrategy >& stoppingStrategy,
                        const RCPointer< StartingOrientationStrategy >&
                                                    startingOrientationStrategy,
                        OdfContinuousField& odfContinuousField,
                        const Volume< int16_t >& computingMask,
                        const Transform3d< float >&
                                            transform3dFromRoisVoxelToOdfsReal,
                        const Transform3d< float >&
                                            transform3dFromRoisVoxelToMaskVoxel,
                        const Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                        const Transform3d< float >&
                                            transform3dFromOdfsRealToRoisVoxel,
                        BundleMap< L, Compare >& bundleMap,
                        bool verbose ) const;


    static std::string getStaticName();

  protected:

    friend class Creator4Arg< GlobalTractographyAlgorithm< L, Compare >,
                              TractographyAlgorithm< L, Compare >,
                              const Vector3d< int32_t >&,
                              const Vector3d< double >&,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;
    friend class GlobalTractographyInitializationContext< L, Compare >;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // I/O of intermediate result(s)
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // writing a set of spin glasses to disk
    void saveSpinGlassesToMesh(
                          const std::set< RCPointer< SpinGlass > >& spinGlasses,
                          const std::string& fileName,
                          bool ascii,
                          const std::string& format ) const;

    // writing a bundle map stemming from connected spin glasses to disk
    void saveSpinGlassesToBundleMap(
                          const std::set< RCPointer< SpinGlass > >& spinGlasses,
                          const L& label,
                          BundleMap< L, Compare >& bundleMap ) const;

///////////////////////////////////
    void saveDistribution( const std::string& fileName,
                           std::vector< float >& values ) const;
    void saveDistribution( const std::string& fileName,
                           std::vector< int32_t >& iterations,
                           std::vector< float >& values ) const;
    void saveDistribution( const std::string& fileName,
                           std::vector< int32_t >& iterations,
                           std::vector< int32_t >& values ) const;
    void saveDistribution( const std::string& fileName,
                           std::vector< float >& abscisses,
                           std::vector< float >& values ) const;
    void saveDistribution( const std::string& fileName,
                           std::vector< uint32_t >& iterations,
                           std::vector< uint32_t >& values ) const;
    void saveDistribution( const std::string& fileName,
                           std::vector< uint32_t >& iterations,
                           std::vector< double >& values ) const;



    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // general access or update method(s)
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // getting a random spin glass from the different sets
    SpinGlassConfiguration getRandomSpinGlass( 
           int32_t iteration,
           const std::set< RCPointer< SpinGlass > >& isolatedSpinGlasses,
           const std::set< RCPointer< SpinGlass > >& oneSideAttachedSpinGlasses,
           const std::set< RCPointer< SpinGlass > >& fullyAttachedSpinGlasses,
           int32_t globalSpinGlassCount,
           RCPointer< SpinGlass >& spinGlass ) const;

    // choosing randomly a spin glass extremity
    void getRandomSpinGlassExtremityType(
                       SpinGlass::ExtremityType& spinGlassExtremityType ) const;

    // computing the voxel corresponding to the spin glass position
    void getVoxelFromPosition( const Vector3d< float >& position,
                               const Vector3d< double >& roiResolution,
                               Vector3d< int32_t >& voxel ) const;

    // computing the ODF at the spin glass position
    void getOdf( OdfContinuousField& odfContinuousField,
                 const Vector3d< float >& spinGlassPosition,
                 const Vector3d< double >& roiResolution,
                 const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
                 RCPointer< OrientationDistributionFunction >& odf ) const;

    // computing the spin glass orientation according to ODFs and possible
    // access to the local normal to the pial surface
    void getSpinGlassOrientationAndLength(
                                     const Vector3d< float >& orientation,
                                     const OrientationSet& outputOrientationSet,
                                     const OrientationDistributionFunction& odf,
                                     const Vector3d< float >& spinGlassPosition,
                                     Vector3d< float >& spinGlassOrientation,
                                     float& spinGlassLength ) const;

    // accessing the neighbors of a given spin glass and extremity type
    void getNeighboringSpinGlassesWithinSphere(
      const RCPointer< SpinGlass >& spinGlass,
      const SpinGlass::ExtremityType& spinGlassExtremityType,
      const CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                        spinGlassCartesianField,
      const Vector3d< int32_t >& voxel,
      std::deque< std::pair< RCPointer< SpinGlass >,
                             SpinGlass::ExtremityType > >&
                                neighboringSpinGlassesAndExtremityTypes ) const;

    // adding a created spin glass to the Cartesian field
    void addSpinGlassToCartesianField( const Vector3d< int32_t >& voxel,
           const RCPointer< SpinGlass >& spinGlass,
           CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                spinGlassCartesianField ) const;

    // updating the spin glass set containers
    void updateSpinGlassSets(
           const RCPointer< SpinGlass >& selectedSpinGlass,
           const RCPointer< SpinGlass >& oldConnectedSpinGlass,
           const RCPointer< SpinGlass >& successorSpinGlass,
           std::set< RCPointer< SpinGlass > >& isolatedSpinGlasses,
           std::set< RCPointer< SpinGlass > >& oneSideAttachedSpinGlasses,
           std::set< RCPointer< SpinGlass > >& fullyAttachedSpinGlasses,
           std::set< RCPointer< SpinGlass > >&
                                       oneSideAttachedSpinGlassesInside ) const;


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // Metropolis-Hasting method(s)
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // choose the modification proposal (ie scenario)
    ModificationProposal getModificationProposal(
                                  SpinGlassConfiguration spinGlassConfiguration,
                                  int32_t iteration,
                                  int32_t globalSpinGlassCount ) const;

    // get random index from a Gibb's sampler
    int32_t getRandomSelectionFromGibbsSampler(
                                      const std::vector< float >& probabilities,
                                      float gibbsTemperature ) const;

    // computation of the spin glass external energy
    float getSpinGlassExternalEnergy(
                               std::map< Vector3d< float >,
                                         std::pair< std::vector< float >,
                                                    std::pair< float, float > >,
                                         Vector3dCompare< float > >& buffer,
                                const Vector3d< float >& spinGlassPosition,
                                const Vector3d< float >& spinGlassOrientation,
                                const OrientationDistributionFunction& odf,
                                float globalMinimumOdfProbability,
                                float globalMaximumOdfProbability ) const;

    // Metropolis-Hasting acceptance test
    bool getAcceptance( float greenRatio ) const;



    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SPIN GLASS CONNECTION PROPOSAL
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ConnectionType makeSpinGlassConnectionProposal( 
           std::map< Vector3d< float >,
                     std::pair< std::vector< float >,
                                std::pair< float, float > >,
                     Vector3dCompare< float > >& buffer,
           RCPointer< SpinGlass >& selectedSpinGlass,
           SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
           const Vector3d< int32_t >& voxel,
           const CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                        spinGlassCartesianField,
           float temperature,
           RCPointer< SpinGlassConnection >& oldSpinGlassConnection,
           RCPointer< SpinGlass >& oldConnectedSpinGlass,
           SpinGlass::ExtremityType& oldConnectedSpinGlassExtremityType,
           const BoundingBox< int32_t >& computingMaskBoundingBox,
           const Volume< int16_t >& computingMask,
           const Vector3d< double >& roiResolution,
           OdfContinuousField& odfContinuousField,
           const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
           const OrientationSet& outputOrientationSet,
           float globalMinimumOdfProbability,
           float globalMaximumOdfProbability,
           const RCPointer< UniqueIdentityGenerator >&
                                               spinGlassUniqueIdentityGenerator,
           std::deque< std::pair< RCPointer< SpinGlass >,
                                  SpinGlass::ExtremityType > >&
                                        neighboringSpinGlassesAndExtremityTypes,
           Vector3d< int32_t >& successorVoxel,
           RCPointer< SpinGlass >& successorSpinGlass,
           SpinGlass::ExtremityType& successorSpinGlassExtremityType,
           std::set< RCPointer< SpinGlass > >& isolatedSpinGlasses,
           float& oldConnectionEnergy,
           float& newSpinGlassExternalEnergy,
           float& newConnectionEnergy ) const;

    void getSuccessorSpinGlassAndComputeEnergies( 
           const RCPointer< SpinGlass >& selectedSpinGlass,
           const SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
           const RCPointer< SpinGlass >& oldConnectedSpinGlass,
           const std::deque< std::pair< RCPointer< SpinGlass >,
                             SpinGlass::ExtremityType > >&
                                        neighboringSpinGlassesAndExtremityTypes,
           float temperature,
           RCPointer< SpinGlass >& successorSpinGlass,
           SpinGlass::ExtremityType& successorSpinGlassExtremityType,
           float& oldConnectionEnergy,
           float& newConnectionEnergy ) const;

    ConnectionType createSuccessorSpinGlassAndComputeEnergies( 
           std::map< Vector3d< float >,
                     std::pair< std::vector< float >,
                                std::pair< float, float > >,
                     Vector3dCompare< float > >& buffer,
           const RCPointer< SpinGlass >& selectedSpinGlass,
           const SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
           const Vector3d< double >& roiResolution,
           const BoundingBox< int32_t >& computingMaskBoundingBox,
           const Volume< int16_t >& computingMask,
           OdfContinuousField& odfContinuousField,
           const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
           const OrientationSet& outputOrientationSet,
           float globalMinimumOdfProbability,
           float globalMaximumOdfProbability,
           const RCPointer< UniqueIdentityGenerator >&
                                               spinGlassUniqueIdentityGenerator,
           Vector3d< int32_t >& successorVoxel,
           RCPointer< SpinGlass >& successorSpinGlass,
           SpinGlass::ExtremityType& successorSpinGlassExtremityType,
           float& newSpinGlassExternalEnergy,
           float& newConnectionEnergy ) const;

    void createSpinGlassConnection(
           const RCPointer< SpinGlass >& selectedSpinGlass,
           const SpinGlass::ExtremityType& selectedSpinGlassExtremityType,
           const RCPointer< SpinGlass >& successorSpinGlass,
           const SpinGlass::ExtremityType& successorSpinGlassExtremityType,
           const RCPointer< UniqueIdentityGenerator >&
                                     spinGlassConnectionUniqueIdentityGenerator,
           std::set< RCPointer< SpinGlassConnection > >&
                                                   spinGlassConnections ) const;

    void removeSpinGlassConnection(
           const RCPointer< SpinGlassConnection >& spinGlassConnection,
           std::set< RCPointer< SpinGlassConnection > >&
                                                   spinGlassConnections ) const;


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SPIN GLASS OPTIMAL MOTION PROPOSAL
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    bool makeSpinGlassOptimalMotionProposal(
           std::map< Vector3d< float >,
                     std::pair< std::vector< float >,
                                std::pair< float, float > >,
                     Vector3dCompare< float > >& buffer,
           const RCPointer< SpinGlass >& selectedSpinGlass,
           RCPointer< SpinGlass >& alternativeSpinGlass,
           const Vector3d< double >& roiResolution,
           OdfContinuousField& odfContinuousField,
           const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
           float globalMinimumOdfProbability,
           float globalMaximumOdfProbability,
           const BoundingBox< int32_t >& computingMaskBoundingBox,
           const Volume< int16_t >& computingMask,
           Vector3d< float >& oldSpinGlassPosition,
           Vector3d< float >& oldSpinGlassOrientation,
           Vector3d< int32_t >& newVoxel,
           Vector3d< float >& newSpinGlassPosition,
           Vector3d< float >& newSpinGlassOrientation,
           float& oldSpinGlassExternalEnergy,
           float& newSpinGlassExternalEnergy,
           float& oldSpinGlassConnectionEnergy,
           float& newSpinGlassConnectionEnergy ) const;

    void chooseSpinGlassAmongClique(
           const RCPointer< SpinGlass >& selectedSpinGlass,
           const Vector3d< double >& roiResolution,
           const BoundingBox< int32_t >& computingMaskBoundingBox,
           const Volume< int16_t >& computingMask,
           RCPointer< SpinGlass >& alternativeSpinGlass ) const;

    void acceptSpinGlassOptimalMotionProposal(
           const Vector3d< int32_t >& voxel,
           CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                        spinGlassCartesianField,
           RCPointer< SpinGlass >& alternativeSpinGlass,
           const Vector3d< int32_t >& newVoxel,
           const Vector3d< float >& newSpinGlassPosition,
           const Vector3d< float >& newSpinGlassOrientation ) const;


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SPIN GLASS RANDOM MOTION PROPOSAL
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    bool makeSpinGlassRandomMotionProposal(
                std::map< Vector3d< float >,
                          std::pair< std::vector< float >,
                                     std::pair< float, float > >,
                          Vector3dCompare< float > >& buffer,
                 RCPointer< SpinGlass >& selectedSpinGlass,
                 const Vector3d< double >& roiResolution,
                 OdfContinuousField& odfContinuousField,
                 const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
                 float globalMinimumOdfProbability,
                 float globalMaximumOdfProbability,
                 const BoundingBox< int32_t >& computingMaskBoundingBox,
                 const Volume< int16_t >& computingMask,
                 Vector3d< float >& oldSpinGlassPosition,
                 Vector3d< float >& oldSpinGlassOrientation,
                 Vector3d< int32_t >& newVoxel,
                 Vector3d< float >& newSpinGlassPosition,
                 Vector3d< float >& newSpinGlassOrientation,
                 float& oldSpinGlassExternalEnergy,
                 float& newSpinGlassExternalEnergy,
                 float& oldSpinGlassConnectionEnergy,
                 float& newSpinGlassConnectionEnergy ) const;

    void setRandomMove( const Vector3d< float >& oldSpinGlassPosition,
                        const Vector3d< float >& oldSpinGlassOrientation,
                        Vector3d< float >& newSpinGlassPosition,
                        Vector3d< float >& newSpinGlassOrientation,
                        float spatialDiscrepancy,
                        float orientationalDiscrepancy ) const;

    void acceptSpinGlassRandomMotionProposal(
           const Vector3d< int32_t >& voxel,
           CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                        spinGlassCartesianField,
           const RCPointer< SpinGlass >& alternativeSpinGlass,
           const Vector3d< int32_t >& newVoxel,
           const Vector3d< float >& newSpinGlassPosition,
           const Vector3d< float >& newSpinGlassOrientation ) const;


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SPIN GLASS DEATH PROPOSAL
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void makeSpinGlassDeathProposal(
                 std::map< Vector3d< float >,
                           std::pair< std::vector< float >,
                                      std::pair< float, float > >,
                           Vector3dCompare< float > >& buffer,
                 RCPointer< SpinGlass >& selectedSpinGlass,
                 const Vector3d< double >& roiResolution,
                 OdfContinuousField& odfContinuousField,
                 const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
                 float globalMinimumOdfProbability,
                 float globalMaximumOdfProbability,
                 float& spinGlassDensity,
                 float& spinGlassExternalEnergy ) const;

    void removeSpinGlassFromCartesianField(
                 const Vector3d< int32_t >& voxel,
                 const RCPointer< SpinGlass >& spinGlass,
                 CartesianField< std::set< RCPointer< SpinGlass > > >&
                                                spinGlassCartesianField ) const;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // global tractography parameters
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > _scalarParameters;
    std::vector< std::string > _stringParameters;

    NumericalAnalysisImplementationFactory* _factory;
    RCPointer< RandomGenerator > _randomGenerator;

    RCPointer< RpcCommandWriter > _writer;

    RCPointer< AnatomicalPriors > _anatomicalPriors;

    float _minimumFiberLength;
    float _maximumFiberLength;
    int32_t _iterationCount;
    int32_t _iterationCountMinusOne;
    int32_t _iterationCountOverThree;
    float _gibbsTemperature;
    float _externalTemperature;
    float _initialInternalTemperature;
    float _finalInternalTemperature;

    float _spinGlassLength;

    int32_t _voxelSamplerCount;

    float _externalEnergyWeight;
    float _internalEnergyWeight;

    uint32_t _cliqueNeighborCount;

};


}


#endif
