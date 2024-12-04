#ifndef _gkg_dmri_connectivity_matrix_VolumeConnectivityMatrixFunctor_h_
#define _gkg_dmri_connectivity_matrix_VolumeConnectivityMatrixFunctor_h_



#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{

template < class L, class Compare > class VolumeConnectivityMatrixFunctor;
class ConnectivityMatrixGauge;


template < class L, class Compare = std::less< L > >
class VolumeLoopContext : public LoopContext< int32_t >
{

  public:

    VolumeLoopContext(
      ConnectivityMatrixGauge& gauge,
      const VolumeConnectivityMatrixFunctor< L, Compare >& 
                                                volumeConnectivityMatrixFunctor,
      const typename BundleMap< L, Compare >::Bundle& bundle,
      const Volume< int32_t >& rois1,
      const BoundingBox< int32_t >& rois1BoundingBox,
      Transform3d< float >* transform3dFromBundleRealFrameToROIs1VoxelFrame,
      const RCPointer< SceneModeler >& sceneModeler,
      const BoundingBox< float >& meshBoundingBox,
      const Volume< uint8_t >& meshMask,
      Transform3d< float >* transform3dFromBundleRealFrameToMeshRealFrame,
      SparseMatrix& connectivityMatrix,
      RCPointer< SparseMatrix >& averageLengthMatrix,
      RCPointer< BundleMap< L, Compare > >& roiToRoiBundleMap,
      const bool& verbose );
    virtual ~VolumeLoopContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    int32_t resampleFiber(
      const typename BundleMap< L, Compare >::Fiber& fiber,
      typename BundleMap< L, Compare >::Fiber& resampledFiber ) const;

    void findLabels(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      std::vector< int32_t >& labels ) const;

    void getRangesAndRemovingShortOnes(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      std::vector< int32_t >& labels,
      std::vector< std::pair< int32_t, int32_t > >& ranges ) const;

    int16_t getRange( const std::vector< int32_t >& labels,
                      int32_t index,
                      std::pair< int32_t, int32_t >& range ) const;

    void splitFiberAndSortFiberPieces(
      const typename BundleMap< L, Compare >::Fiber& resampledFiber,
      const std::vector< int32_t >& labels,
      const std::vector< std::pair< int32_t, int32_t > >& ranges,
      std::map< L, std::list< LightCurve3d< float > > >& labelledFibers,
      std::map< L, std::pair< int32_t, int32_t > >& correspondingRois ) const;

    void fillOutputs(
      const std::map< L, std::list< LightCurve3d< float > > >& labelledFibers,
      const std::map< L, std::pair< int32_t, int32_t > >& correspondingRois );

    L getLabel( int32_t label1, int32_t label2 ) const;

    const VolumeConnectivityMatrixFunctor< L, Compare >& 
                                               _volumeConnectivityMatrixFunctor;
    const typename BundleMap< L, Compare >::Bundle& _bundle;
    const Volume< int32_t >& _rois1;
    const BoundingBox< int32_t >& _rois1BoundingBox;
    Transform3d< float >* _transform3dFromBundleRealFrameToROIs1VoxelFrame;
    const RCPointer< SceneModeler >& _sceneModeler;
    const BoundingBox< float >& _meshBoundingBox;
    const Volume< uint8_t >& _meshMask;
    Transform3d< float >* _transform3dFromBundleRealFrameToMeshRealFrame;
    SparseMatrix& _connectivityMatrix;
    RCPointer< SparseMatrix >& _averageLengthMatrix;
    RCPointer< BundleMap< L, Compare > >& _roiToRoiBundleMap;
    int16_t _offset;
    const bool& _verbose;
    bool _fiberIndexAlreadyDisplayed;

};


template < class L, class Compare = std::less< L > >
class VolumeConnectivityMatrixFunctor :
     public ConnectivityMatrixFunctor< L, Compare >,
     public Creator2Arg< VolumeConnectivityMatrixFunctor< L, Compare >,
                         ConnectivityMatrixFunctor< L, Compare >,
                         const std::vector< double >&,
                         const std::vector< std::string >& >

{

  public:

    ~VolumeConnectivityMatrixFunctor();

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

    int16_t _offset;

  protected:

    friend 
    class Creator2Arg< VolumeConnectivityMatrixFunctor< L, Compare >,
                       ConnectivityMatrixFunctor< L, Compare >,
                       const std::vector< double >&,
                       const std::vector< std::string >& >;

    friend class VolumeLoopContext< L, Compare >;

    VolumeConnectivityMatrixFunctor(
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );

    float _minimumRoiIntersectionLength;
    float _inputFiberResamplingStep;
    float _outputFiberResamplingStep;
    bool _takeIntoAccountFiberTangent;
    bool _takeIntoAccountInterface;
    std::string _fileNameInterface;
    std::string _fileNameInterfaceToBundleMapTransform3d;
    float _maximumDistanceToMesh;
    Vector3d< int32_t > _cacheSize;    

};


}


#endif

