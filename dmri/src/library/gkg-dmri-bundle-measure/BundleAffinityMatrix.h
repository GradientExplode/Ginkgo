#ifndef _gkg_dmri_bundle_measure_BundleAffinityMatrix_h_
#define _gkg_dmri_bundle_measure_BundleAffinityMatrix_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-dmri-fiber-distance/FiberDistanceFunctor.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>

#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <list>
#include <utility>


namespace gkg
{


class BundleAffinityMatrixGauge : public ThreadGauge< uint64_t >
{

  public:

    BundleAffinityMatrixGauge( uint64_t maxCount );

    void add( uint64_t value );
    void reset();

  private:

    void display();

    uint64_t _count;
    uint64_t _maxCount;
    bool _pairIndexAlreadyDisplayed;

};


class BundleAffinityMatrixContext : public LoopContext< uint64_t >
{

  public:
  
    BundleAffinityMatrixContext(
      BundleAffinityMatrixGauge& gauge,
      const std::vector< std::pair< int32_t, int32_t > >&
                               sparseMatrixIndexToSubjectIndexAndCentroidIndex,
      const std::vector< std::vector< LightCurve3d< float > > >&
                                                        resampledLightCurve3ds,
      const std::vector< std::vector< float > >& resampledCentroidLengths,
      const float& fourTimesMaximumDistance,
      const float& minimumAffinity,
      const float& variance,
      const RCPointer< FiberDistanceFunctor >& fiberDistanceFunctor,
      SparseMatrix& affinityMatrix,
      SparseMatrix& distanceMatrix,
      std::list< double >* nonCorrectedDistances,
      std::list< double >* distanceCorrections,
      std::list< double >* correctedDistances,
      std::list< double >* correctedAffinities,
      std::list< double >* thresholdedCorrectedDistances,
      std::list< double >* thresholdedCorrectedAffinities,
      const bool& verbose );
                               
    int32_t getFiberPairCount() const;
                               
    void doIt( uint64_t startIndex, uint64_t countIndex );
    
  private:

    const std::vector< std::pair< int32_t, int32_t > >&
      _sparseMatrixIndexToSubjectIndexAndCentroidIndex;
    const std::vector< std::vector< gkg::LightCurve3d< float > > >&
      _resampledLightCurve3ds;
    const std::vector< std::vector< float > >&
      _resampledCentroidLengths;
    const float& _fourTimesMaximumDistance;
    const float& _minimumAffinity;
    const float& _variance;
    const RCPointer< FiberDistanceFunctor >& _fiberDistanceFunctor;

    gkg::SparseMatrix& 
      _affinityMatrix;
    gkg::SparseMatrix& 
      _distanceMatrix;
    std::list< double >* _nonCorrectedDistances;
    std::list< double >* _distanceCorrections;
    std::list< double >* _correctedDistances;
    std::list< double >* _correctedAffinities;
    std::list< double >* _thresholdedCorrectedDistances;
    std::list< double >* _thresholdedCorrectedAffinities;
    
    const bool& _verbose;
    
    uint64_t _gaugeUpdatePeriod;


};


class BundleAffinityMatrix : public BundleMeasure< SparseMatrix >
{

  public:

    typedef BundleMeasure< SparseMatrix >::Bundle Bundle;

    BundleAffinityMatrix(
                       RCPointer< FiberDistanceFunctor > fiberDistanceFunctor,
                       float maximumDistance,
                       float variance,
                       int32_t resamplingPointCount );
    virtual ~BundleAffinityMatrix();

    SparseMatrix get( const Bundle& bundle ) const;
    void get( const Bundle& bundle, SparseMatrix& affinityMatrix ) const;
    void get(
       const std::vector< std::vector< LightCurve3d< float > > >& lightCurve3ds,
       const std::vector< std::pair< int32_t, int32_t > >&
                                sparseMatrixIndexToSubjectIndexAndCentroidIndex,
       int32_t sparseMatrixSize,
       SparseMatrix& affinityMatrix,
       SparseMatrix& distanceMatrix,
       bool verbose,
       Matrix* centroidMatrix = 0,
       std::list< double >* nonCorrectedDistances = 0,
       std::list< double >* distanceCorrections = 0,
       std::list< double >* correctedDistances = 0,
       std::list< double >* correctedAffinities = 0,
       std::list< double >* thresholdedCorrectedDistances = 0,
       std::list< double >* thresholdedCorrectedAffinities = 0 ) const;


  protected:

    RCPointer< FiberDistanceFunctor > _fiberDistanceFunctor;
    float _maximumDistance;
    float _variance;
    int32_t _resamplingPointCount;

};


}


#endif
