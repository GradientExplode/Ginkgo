#ifndef _gkg_dmri_connectivity_matrix_MeshAndTextureConnectivityMatrixFunctor_h_
#define _gkg_dmri_connectivity_matrix_MeshAndTextureConnectivityMatrixFunctor_h_



#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{



template < class L, class Compare = std::less< L > >
class MeshAndTextureConnectivityMatrixFunctor :
     public ConnectivityMatrixFunctor< L, Compare >,
     public Creator2Arg< MeshAndTextureConnectivityMatrixFunctor< L, Compare >,
                         ConnectivityMatrixFunctor< L, Compare >,
                         const std::vector< double >&,
                         const std::vector< std::string >& >

{

  public:

    ~MeshAndTextureConnectivityMatrixFunctor();

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
    class Creator2Arg< MeshAndTextureConnectivityMatrixFunctor< L, Compare >,
                       ConnectivityMatrixFunctor< L, Compare >,
                       const std::vector< double >&,
                       const std::vector< std::string >& >;

    MeshAndTextureConnectivityMatrixFunctor(
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );

    void addContribution(
     const typename BundleMap< L, Compare >::Fiber& fiber,
     const TextureMap< int16_t>& textureRois1,
     const BoundingBox< float >& rois1BoundingBox,
     Transform3d< float >* transform3dFromBundleRealFrameToROIs1RealFrame,
     const RCPointer< gkg::SceneModeler >& sceneModeler,
     const Volume< uint8_t >& rois1Mask,
     const RCPointer< gkg::EvolvingMesh >& evolvingMesh,
     SparseMatrix& connectivityMatrix,
     RCPointer< gkg::SparseMatrix >& averageLengthMatrix,
     RCPointer< gkg::BundleMap< L, Compare > >& roiToMeshBundleMap ) const;

    int16_t getRange( const std::vector< int16_t >& labels,
                      int32_t index,
                      std::pair< int32_t, int32_t >& range ) const;

    L getLabel( int16_t label1, int16_t label2 ) const;
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

