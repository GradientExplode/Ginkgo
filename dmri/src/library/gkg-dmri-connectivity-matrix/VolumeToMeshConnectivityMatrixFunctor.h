#ifndef _gkg_dmri_connectivity_matrix_VolumeToMeshConnectivityMatrixFunctor_h_
#define _gkg_dmri_connectivity_matrix_VolumeToMeshConnectivityMatrixFunctor_h_



#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-communication-thread/LoopContext.h>
#include <vector>


namespace gkg
{


template < class L, class Compare > class VolumeToMeshConnectivityMatrixFunctor;
class ConnectivityMatrixGauge;


template < class L, class Compare = std::less< L > >
class VolumeToMeshLoopContext : public LoopContext< int32_t >
{

  public:

    VolumeToMeshLoopContext(
      ConnectivityMatrixGauge& gauge,
      const VolumeToMeshConnectivityMatrixFunctor< L, Compare >& 
                                          volumeToMeshConnectivityMatrixFunctor,
      const typename BundleMap< L, Compare >::Bundle& bundle,
      const Volume< int32_t >& rois1,
      const BoundingBox< int32_t >& rois1BoundingBox,
      Transform3d< float >* transform3dFromBundleRealFrameToROIs1VoxelFrame,
      const RCPointer< SceneModeler >& sceneModeler,
      const BoundingBox< float >& rois2BoundingBox,
      const Volume< uint8_t >& rois2Mask,
      const RCPointer< EvolvingMesh >& evolvingMesh,
      Transform3d< float >* transform3dFromBundleRealFrameToROIs2RealFrame,
      SparseMatrix& connectivityMatrix,
      RCPointer< SparseMatrix >& averageLengthMatrix,
      RCPointer< BundleMap< L, Compare > >& roiToMeshBundleMap,
      const bool& verbose );
    virtual ~VolumeToMeshLoopContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    int32_t resampleFiber(
      const typename BundleMap< L, Compare >::Fiber& fiber,
      typename BundleMap< L, Compare >::Fiber& resampledFiber ) const;

    void findLabels(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      std::vector< int32_t >& labels,
      std::vector< Polygon< 3U > >& polygons,
      std::vector< float >& distances,
      std::vector< Vector3d< float > >& projectionPoints ) const;

    void getRangesAndRemovingShortOnes(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      std::vector< int32_t >& labels,
      std::vector< std::pair< int32_t, int32_t > >& ranges ) const;

    int32_t getRange( const std::vector< int32_t >& labels,
                      int32_t index,
                      std::pair< int32_t, int32_t >& range ) const;

    void splitFiberAndSortFiberPieces(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      const std::vector< int32_t >& labels,
      const std::vector< std::pair< int32_t, int32_t > >& ranges,
      const std::vector< Polygon< 3U > >& polygons,
      const std::vector< float >& distances,
      const std::vector< Vector3d< float > >& projectionPoints,
      std::map< L, std::list< LightCurve3d< float > > >& labelledFibers,
      std::map< int32_t, std::list< Polygon< 3U > > >& polygonMap,
      std::map< int32_t, std::list< Vector3d< float > > >& projectionPointMap,
      std::map< int32_t, L >& correspondingRois1Label ) const;

    void fillOutputs(
      const std::map< L, std::list< LightCurve3d< float > > >&  labelledFibers,
      const std::map< int32_t, std::list< Polygon< 3U > > >&  polygonMap,
      const std::map< int32_t, std::list< Vector3d< float > > >& 
                                                             projectionPointMap,
      const std::map< int32_t, L >& correspondingRois1Label );

    L getLabel( int32_t label ) const;

    const VolumeToMeshConnectivityMatrixFunctor< L, Compare >& 
                                         _volumeToMeshConnectivityMatrixFunctor;
    const typename BundleMap< L, Compare >::Bundle& _bundle;
    const Volume< int32_t >& _rois1;
    const BoundingBox< int32_t >& _rois1BoundingBox;
    Transform3d< float >* _transform3dFromBundleRealFrameToROIs1VoxelFrame;
    const RCPointer< SceneModeler >& _sceneModeler;
    const BoundingBox< float >& _rois2BoundingBox;
    const Volume< uint8_t >& _rois2Mask;
    const RCPointer< EvolvingMesh >& _evolvingMesh;
    Transform3d< float >* _transform3dFromBundleRealFrameToROIs2RealFrame;
    SparseMatrix& _connectivityMatrix;
    RCPointer< SparseMatrix >& _averageLengthMatrix;
    RCPointer< BundleMap< L, Compare > >& _roiToMeshBundleMap;
    const bool& _verbose;
    bool _fiberIndexAlreadyDisplayed;

};


template < class L, class Compare = std::less< L > >
class VolumeToMeshConnectivityMatrixFunctor :
     public ConnectivityMatrixFunctor< L, Compare >,
     public Creator2Arg< VolumeToMeshConnectivityMatrixFunctor< L, Compare >,
                         ConnectivityMatrixFunctor< L, Compare >,
                         const std::vector< double >&,
                         const std::vector< std::string >& >
{

  public:

    ~VolumeToMeshConnectivityMatrixFunctor();

    void apply( const std::list< std::string >& fileNameBundleMaps,
                const std::list< std::string >& fileNameListOfROIs1,
                const std::string& fileNameROIs1ToBundleMapTransform3d,
                const std::list< std::string >& fileNameListOfROIs2,
                const std::string& fileNameROIs2ToBundleMapTransform3d,
                const std::list< std::string >& functorNames,
                const std::list< std::string >& outputFileNames,
                const std::string& theOutputBundleMapFormat,
                const std::string& theOutputTextureMapFormat,
                bool ascii,
                bool verbose ) const;

    static std::string getScalarParameterHelp();
    static std::string getStringParameterHelp();
    static std::string getFunctorHelp();

  protected:

    friend 
    class Creator2Arg< VolumeToMeshConnectivityMatrixFunctor< L, Compare >,
                       ConnectivityMatrixFunctor< L, Compare >,
                       const std::vector< double >&,
                       const std::vector< std::string >& >;
    friend class VolumeToMeshLoopContext< L, Compare >;

    VolumeToMeshConnectivityMatrixFunctor(
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );

    float _minimumRoiIntersectionLength;
    float _inputFiberResamplingStep;
    float _maximumDistanceToMesh;
    float _outputFiberResamplingStep;
    float _maximumFiberTangentToMeshNormalAngleInDegrees;
    bool _smoothConnectivityMap;
    float _smoothingStandardDeviation;
    Vector3d< int32_t > _cacheSize;

};


}


#endif

