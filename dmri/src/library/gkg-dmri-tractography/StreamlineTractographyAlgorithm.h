#ifndef _gkg_dmri_tractography_StreamlineTractographyAlgorithm_h_
#define _gkg_dmri_tractography_StreamlineTractographyAlgorithm_h_


#include <gkg-dmri-tractography/TractographyAlgorithm.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>


namespace gkg
{


template < class L, class Compare > class StreamlineTractographyAlgorithm;
template < class T > class BoundingBox;


//
// class StreamlineGauge
//

class StreamlineGauge : public ThreadGauge< int32_t >
{

  public:

    StreamlineGauge( int32_t maxCount );

    void add( int32_t value );
    void reset();

  private:

    int32_t _count;
    int32_t _maxCount;
    bool _fiberIndexAlreadyDisplayed;

};


//
// class ThreadedLoopContext< L, Compare >
//

template < class L, class Compare = std::less< L > >
class ThreadedLoopContext : public LoopContext< int32_t >
{

  public:

    ThreadedLoopContext(
      StreamlineGauge& gauge,
      const StreamlineTractographyAlgorithm< L, Compare >&
                                                streamlineTractographyAlgorithm,
      std::list< Vector3d< int32_t > >::const_iterator siteIteratorBegin,
      const RCPointer< SeedingStrategy >& seedingStrategy,
      const RCPointer< StoppingStrategy >& stoppingStrategy,
      const RCPointer< StartingOrientationStrategy >&
                                                    startingOrientationStrategy,
      OdfContinuousField& odfContinuousField,
      const Volume< int16_t >& computingMask,
      const BoundingBox< int32_t >& computingMaskBoundingBox,
      const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
      const Transform3d< float >& transform3dFromRoisVoxelToMaskVoxel,
      const Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
      const Transform3d< float >& transform3dFromOdfsRealToRoisVoxel,
      std::vector< typename BundleMap< L, Compare >::Fiber >& fibers,
      bool& verbose );
    virtual ~ThreadedLoopContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const StreamlineTractographyAlgorithm< L, Compare >& 
      _streamlineTractographyAlgorithm;
    std::list< Vector3d< int32_t > >::const_iterator _siteIteratorBegin;
    const RCPointer< SeedingStrategy >& _seedingStrategy;
    const RCPointer< StoppingStrategy >& _stoppingStrategy;
    const RCPointer< StartingOrientationStrategy >&
                                                   _startingOrientationStrategy;
    OdfContinuousField& _odfContinuousField;
    const Volume< int16_t >& _computingMask;
    const BoundingBox< int32_t >& _computingMaskBoundingBox;
    const Transform3d< float >& _transform3dFromRoisVoxelToOdfsReal;
    const Transform3d< float >& _transform3dFromRoisVoxelToMaskVoxel;
    const Transform3d< float >& _transform3dFromOdfsRealToMaskVoxel;
    const Transform3d< float >& _transform3dFromOdfsRealToRoisVoxel;
    std::vector< typename BundleMap< L, Compare >::Fiber >& _fibers;
    bool& _verbose;
    const gkg::OrientationSet& _orientationSet;
    float _minimumFiberLength;
    float _maximumFiberLength;
    int32_t _storingIncrement;
    
};


//
// class StreamlineTractographyAlgorithm< L, Compare >
//

template < class L, class Compare = std::less< L > >
class StreamlineTractographyAlgorithm : public TractographyAlgorithm< L,
                                                                      Compare >
{

  public:

    virtual ~StreamlineTractographyAlgorithm();

    float getStep() const;
    int32_t getStoringIncrement() const;
    float getMinimumFiberLength() const;
    float getMaximumFiberLength() const;
    float getApertureAngle() const;

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

  protected:

    friend class ThreadedLoopContext< L, Compare >;

    StreamlineTractographyAlgorithm( const Vector3d< int32_t >& size,
                                     const Vector3d< double >& resolution,
                                     float step,
                                     int32_t storingIncrement,
                                     float minimumFiberLength,
                                     float maximumFiberLength,
                                     float apertureAngle );

    virtual
      bool track( const Vector3d< float >& startingPointOdfs,
                  const int32_t& startingOrientationIndex,
                  OdfContinuousField& odfContinuousField,
                  const RCPointer< StoppingStrategy >& stoppingStrategy,
                  std::list< Vector3d< float > >& trajectory,
                  float& fiberLength ) const = 0;

    float _step;
    int32_t _storingIncrement;
    float _minimumFiberLength;
    float _maximumFiberLength;
    float _apertureAngle;

};


}


#endif
