#ifndef _gkg_processing_numericalanalysis_DensityBasedSpatialClustering_h_
#define _gkg_processing_numericalanalysis_DensityBasedSpatialClustering_h_


#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <list>


namespace gkg
{

class DensityBasedSpatialClustering
{

  public:

    typedef std::list< std::list< int32_t > >::const_iterator const_iterator;

    DensityBasedSpatialClustering(
                        const Matrix& matrix,
                        const RCPointer< TestFunction< double > >& testFunction,
                        int32_t minimumClusterSize );
    DensityBasedSpatialClustering(
                        const SparseMatrix& sparseMatrix,
                        const RCPointer< TestFunction< double > >& testFunction,
                        int32_t minimumClusterSize );
    virtual ~DensityBasedSpatialClustering();

    const_iterator begin() const;
    const_iterator end() const;

    int32_t getClusterCount() const;
    const std::list< int32_t >& getIsolatedItems() const;

    int32_t getLabel( int32_t index ) const;
    const std::vector< int32_t >& getLabels() const;

  protected:

    bool expandCluster( const Matrix& matrix,
                        int32_t index,
                        int32_t clusterId );
    void computeCluster( const Matrix& matrix,
                         int32_t index,
                         std::list< int32_t >& seeds ) const;

    bool expandCluster( const SparseMatrix& sparseMatrix,
                        int32_t index,
                        int32_t clusterId );
    void computeCluster( const SparseMatrix& sparseMatrix,
                         int32_t index,
                         std::list< int32_t >& seeds ) const;

    RCPointer< TestFunction< double > > _testFunction;
    int32_t _minimumClusterSize;
    std::vector< int32_t > _labels;

    std::list< std::list< int32_t > > _clusters;
    std::list< int32_t > _isolatedItems;


};


}


#endif
