#ifndef _gkg_processing_numericalanalysis_HierarchicalClustering_h_
#define _gkg_processing_numericalanalysis_HierarchicalClustering_h_


#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <vector>
#include <utility>


namespace gkg
{


class HierarchicalClustering
{

  public:

    HierarchicalClustering( const Matrix& matrix,
                            const TestFunction< double >& testFunction );
    HierarchicalClustering( const SparseMatrix& sparseMatrix,
                            const TestFunction< double >& testFunction );
    virtual ~HierarchicalClustering();

    int32_t getMaximumVertexCount() const;
    const std::vector< int32_t >& getFathers() const; 

    void getChildren(
                 std::vector< std::pair< int32_t, int32_t > >& children ) const;

    int32_t getForest( const std::vector< int32_t >& parcelVoxelCounts,
                       int32_t averageClusterSize,
                       int32_t isolatedClusterMinimumSize,
                       int32_t minimumClusterSize,
                       float minimumParcelSizeRatio,
                       std::vector< int32_t >& nodeClusterLabels );

  protected:

    // return the # of connected comp.
    int32_t computeConnectedComponents(
                      const std::vector< std::pair< int32_t, int32_t > >& edges,
                      std::vector< int32_t >& connectedComponents ) const;
    void computeAverageLink(
                      int32_t connectedComponentCount,
                      std::vector< std::pair< int32_t, int32_t > >& edges,
                      std::vector< float >& weights,
                      std::vector< float >& heights,
                      std::vector< int32_t >& fathers ) const;
    void getDescendantVertexIndices(
                   int32_t topVertexIndex,
                   const std::vector< std::pair< int32_t, int32_t > >& children,
                   std::vector< int32_t >& descendantVertexIndices ) const;
    void getLeafIndices(
                   int32_t topVertexIndex,
                   const std::vector< std::pair< int32_t, int32_t > >& children,
                   std::vector< int32_t >& leafIndices ) const;

    static int32_t coordinateCompareFunctor( const void* a, const void* b );
    static int32_t edgeIndicesCompareFunctor( const void* a,const void* b );

    int32_t _vertexCount;
    int32_t _maximumVertexCount;
    std::vector< float > _heights;
    std::vector< int32_t > _fathers;

};


}


#endif
