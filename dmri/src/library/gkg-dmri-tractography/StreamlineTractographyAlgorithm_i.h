#ifndef _gkg_dmri_tractography_StreamlineTractographyAlgorithm_i_h_
#define _gkg_dmri_tractography_StreamlineTractographyAlgorithm_i_h_


#include <gkg-dmri-tractography/StreamlineTractographyAlgorithm.h>
#include <gkg-dmri-tractography/TractographyAlgorithm_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/VoxelSampler.h>
#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-processing-container/ContinuousField_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>

#include <gkg-core-io/Writer_i.h>


//
// class StreamlineGauge
//

inline gkg::StreamlineGauge::StreamlineGauge( int32_t maxCount )
                           : _count( 0 ),
                             _maxCount( maxCount ),
                             _fiberIndexAlreadyDisplayed( false )
{
}



inline void gkg::StreamlineGauge::add( int32_t value )
{

  try
  {

    _count += value;

    if ( _fiberIndexAlreadyDisplayed )
    {

      std::cout << gkg::Eraser( 38 );

    }

    std::cout << " fiber [ " << std::setw( 12 ) << _count
              << " / " << std::setw( 12 ) << _maxCount
              << " ]" << std::flush;

    _fiberIndexAlreadyDisplayed = true;

  }
  GKG_CATCH( "inline void gkg::StreamlineGauge::add( int32_t value )" );

}


inline void gkg::StreamlineGauge::reset()
{

  try
  {

    _fiberIndexAlreadyDisplayed = false;
    _count = 0;

  }
  GKG_CATCH( "inline void gkg::StreamlineGauge::reset(" );

}


//
// class StreamlineTractographyAlgorithm< L, Compare >::ThreadedLoopContext
//


template < class L, class Compare >
inline
gkg::ThreadedLoopContext< L, Compare >::ThreadedLoopContext(
 gkg::StreamlineGauge& gauge,
 const gkg::StreamlineTractographyAlgorithm< L, Compare >&
                                                streamlineTractographyAlgorithm,
 std::list< gkg::Vector3d< int32_t > >::const_iterator siteIteratorBegin,
 const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy,
 const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy,
 const gkg::RCPointer< gkg::StartingOrientationStrategy >&
                                                    startingOrientationStrategy,
 gkg::OdfContinuousField& odfContinuousField,
 const gkg::Volume< int16_t >& computingMask,
 const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
 const Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
 const Transform3d< float >& transform3dFromRoisVoxelToMaskVoxel,
 const Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
 const Transform3d< float >& transform3dFromOdfsRealToRoisVoxel,
 std::vector< typename gkg::BundleMap< L, Compare >::Fiber >& fibers,
 bool& verbose )
                                       :  gkg::LoopContext< int32_t >( &gauge ),
                                          _streamlineTractographyAlgorithm(
                                              streamlineTractographyAlgorithm ),
                                          _siteIteratorBegin(
                                                            siteIteratorBegin ),
                                          _seedingStrategy( seedingStrategy ),
                                          _stoppingStrategy( stoppingStrategy ),
                                          _startingOrientationStrategy(
                                                  startingOrientationStrategy ),
                                          _odfContinuousField(
                                                           odfContinuousField ),
                                          _computingMask( computingMask ),
                                          _computingMaskBoundingBox(
                                                     computingMaskBoundingBox ),
                                          _transform3dFromRoisVoxelToOdfsReal(
                                          transform3dFromRoisVoxelToOdfsReal ),
                                          _transform3dFromRoisVoxelToMaskVoxel(
                                          transform3dFromRoisVoxelToMaskVoxel ),
                                          _transform3dFromOdfsRealToMaskVoxel(
                                          transform3dFromOdfsRealToMaskVoxel ),
                                          _transform3dFromOdfsRealToRoisVoxel(
                                          transform3dFromOdfsRealToRoisVoxel ),
                                          _fibers( fibers ),
                                          _verbose( verbose ),
					  _orientationSet(
			          odfContinuousField.getOutputOrientationSet() )
{

  try
  {

    _minimumFiberLength =
                        streamlineTractographyAlgorithm.getMinimumFiberLength();
    _maximumFiberLength = 
                        streamlineTractographyAlgorithm.getMaximumFiberLength();
    _storingIncrement = streamlineTractographyAlgorithm.getStoringIncrement();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::ThreadedLoopContext< L, Compare >::ThreadedLoopContext( "
             "gkg::StreamlineGauge& gauge, "
             "const gkg::StreamlineTractographyAlgorithm< L, Compare >& "
             "  streamlineTractographyAlgorithm, "
             "std::list< gkg::Vector3d< int32_t > >::const_iterator "
             "siteIteratorBegin, "
             "const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy, "
             "const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy, "
             "const gkg::RCPointer< gkg::StartingOrientationStrategy >& "
             "startingOrientationStrategy, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const Transform3d< float >& "
             "                        transform3dFromRoisVoxelToOdfsReal, "
             "const Transform3d< float >& "
             "                        transform3dFromRoisVoxelToMaskVoxel, "
             "const Transform3d< float >& "
             "                        transform3dFromOdfsRealToMaskVoxel, "
             "const Transform3d< float >& "
             "                        transform3dFromOdfsRealToRoisVoxel, "
             "std::vector< typename gkg::BundleMap< L, Compare >::Fiber >& " 
             "                        fibers, "
             "bool& verbose )" );

}


template < class L, class Compare >
inline
gkg::ThreadedLoopContext< L, Compare >::~ThreadedLoopContext()
{
}


template < class L, class Compare >
inline
void gkg::ThreadedLoopContext< L, Compare >::doIt( int32_t startIndex,
                                                   int32_t count )
{

  try
  {

    std::list< typename gkg::BundleMap< L, Compare >::Fiber > fibers;

    // computing site count
    int32_t N = count / 5;

    // looping over site(s) in voxel coordinates
    gkg::Vector3d< float > pointOdfs;
    gkg::Vector3d< float > floatVoxelRois;
    gkg::Vector3d< int32_t > voxelMask;
    int32_t g = 0;
    float fiberLength = 0.0;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = _siteIteratorBegin;
    std::advance( s, startIndex );
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      se = _siteIteratorBegin;
    std::advance( se, startIndex + count );

    int32_t maskValue = 0;
    int32_t seedIndex = 0;
    int32_t seedCount = 0;
    while ( s != se )
    {

      if ( _verbose && _gauge )
      {

        if ( g && !( g % N ) )
        {

          lock();
          _gauge->add( N );
          unlock();

        }

      }


      // processing current voxel in the ROI voxel frame
      floatVoxelRois.x = ( float )s->x;
      floatVoxelRois.y = ( float )s->y;
      floatVoxelRois.z = ( float )s->z;

      // processing current point in the ODF real frame
      _transform3dFromRoisVoxelToOdfsReal.getDirect( floatVoxelRois,
                                                     pointOdfs );

      // processing current voxel in the MASK voxel frame
      _transform3dFromRoisVoxelToMaskVoxel.getIntegerDirect( floatVoxelRois,
                                                             voxelMask );

      if ( _computingMaskBoundingBox.contains( voxelMask ) )
      {

        maskValue = _computingMask( voxelMask );

        if ( maskValue )
        {

          gkg::SeedingStrategy::RegionId
            regionId = _seedingStrategy->getRegionId( voxelMask );
          seedCount = _seedingStrategy->getSeedCount( regionId );

          // looping over seed(s) in real coordinates
          for ( seedIndex = 0; seedIndex < seedCount; seedIndex++ )
          {

            // calculating starting point in the ODF real frame
            gkg::Vector3d< float > startingPointOdfs;
            _seedingStrategy->getSeed( regionId,
                                       pointOdfs,
                                       seedIndex,
                                       startingPointOdfs );

            if ( _odfContinuousField.isValid( startingPointOdfs ) )
            {

              // getting ODF at starting point
              gkg::OrientationDistributionFunction*
                odf = _odfContinuousField.getItem( startingPointOdfs );

              // calculating starting forward and backward orientation(s)
              int32_t forwardStartingOrientationIndex =
                _startingOrientationStrategy->getStartingOrientation( *odf );

              int32_t backwardStartingOrientationIndex =
                _orientationSet.getNearestSymmetricalOrientationIndex(
                                              forwardStartingOrientationIndex );

              // deleting ODF
              delete odf;

              // initializing fiber length
              fiberLength = 0.0;

              // allocating the list of point(s) for the forward trajectory
              std::list< gkg::Vector3d< float > > forwardTrajectory;

              // tracking forwards while adding new seed points
              if ( _streamlineTractographyAlgorithm.track(
                                        startingPointOdfs,
                                        forwardStartingOrientationIndex,
                                        _odfContinuousField,
                                        _stoppingStrategy,
                                        forwardTrajectory,
                                        fiberLength ) )
              {

                // allocating the list of point(s) for the backward trajectory
                std::list< gkg::Vector3d< float > > backwardTrajectory;

                // tracking backwards while adding new seed points
                if ( _streamlineTractographyAlgorithm.track(
                                         startingPointOdfs,
                                         backwardStartingOrientationIndex,
                                         _odfContinuousField,
                                         _stoppingStrategy,
                                         backwardTrajectory,
                                         fiberLength ) &&
                     ( fiberLength >= _minimumFiberLength ) &&
                     ( fiberLength <= _maximumFiberLength ) )
                {

                  // allocating the list of point(s) for the undersampled 
                  // trajectory
                  std::list< gkg::Vector3d< float > > trajectory;

                  // merging forward+backward trajectories and sub-sampling
                  std::list< gkg::Vector3d< float > >::const_iterator
                    b = backwardTrajectory.begin(),
                    be = backwardTrajectory.end();
                  int32_t backwardIndex = 1;
                  trajectory.push_front( *b );
                  ++ b;
                  -- be;
                  while ( b != be )
                  {

                    if ( ( backwardIndex % _storingIncrement ) == 0 )
                    {

                      trajectory.push_front( *b );

                    }
                    ++ b;
                    ++ backwardIndex;

                  }
                  trajectory.push_front( *b );

                  // adding a new fiber with processed trajectory
                  fibers.push_back(
                                   gkg::LightCurve3d< float >( trajectory ) );
                  gkg::LightCurve3d< float >& fiber = fibers.back();

                  std::list< gkg::Vector3d< float > >::const_iterator
                    f = forwardTrajectory.begin(),
                    fe = forwardTrajectory.end();
                  int32_t forwardIndex = 1;
                  ++ f;  // skipping 1st point already added 
                         // during bck tracking
                  -- fe;
                  while ( f != fe )
                  {

                    if ( ( forwardIndex % _storingIncrement ) == 0 )
                    {

                      fiber.addPoint( *f );

                    }
                    ++ f;
                    ++ forwardIndex;

                  }
                  fiber.addPoint( *f );

                }

              }

            }

          }

        }

      }
      ++s;
      ++g;

    }

    lock();

    // adding fibers to bundle
    typename std::list< 
                  typename gkg::BundleMap< L, Compare >::Fiber >::const_iterator
      f = fibers.begin(),
      fe = fibers.end();

    while ( f != fe )
    {

      if ( f->getPointCount() )
      {

        _fibers.push_back( *f );

      }

      ++f;

    }

    if ( _verbose && _gauge )
    {

      _gauge->add( count % N );

    }
    
    unlock();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::ThreadedLoopContext< L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );


}


//
// class StreamlineTractographyAlgorithm< L, Compare >
//

template < class L, class Compare >
inline
gkg::StreamlineTractographyAlgorithm< L,
                                     Compare >::StreamlineTractographyAlgorithm(
                                      const gkg::Vector3d< int32_t >& size,
                                      const gkg::Vector3d< double >& resolution,
                                      float step,
                                      int32_t storingIncrement,
                                      float minimumFiberLength,
                                      float maximumFiberLength,
                                      float apertureAngle )
             : gkg::TractographyAlgorithm< L, Compare >( size, resolution ),
               _step( step ),
               _storingIncrement( storingIncrement),
               _minimumFiberLength( minimumFiberLength ),
               _maximumFiberLength( maximumFiberLength ),
               _apertureAngle( apertureAngle )
{
}


template < class L, class Compare >
inline
gkg::StreamlineTractographyAlgorithm< L,
                                   Compare >::~StreamlineTractographyAlgorithm()
{
}


template < class L, class Compare >
inline
float gkg::StreamlineTractographyAlgorithm< L, Compare >::getStep() const
{

  return _step;

}


template < class L, class Compare >
inline
int32_t 
gkg::StreamlineTractographyAlgorithm< L, Compare >::getStoringIncrement() const
{

  return _storingIncrement;

}


template < class L, class Compare >
inline
float 
gkg::StreamlineTractographyAlgorithm< L,
                                      Compare >::getMinimumFiberLength() const
{

  return _minimumFiberLength;

}


template < class L, class Compare >
inline
float 
gkg::StreamlineTractographyAlgorithm< L,
                                      Compare >::getMaximumFiberLength() const
{

  return _maximumFiberLength;

}


template < class L, class Compare >
inline
float 
gkg::StreamlineTractographyAlgorithm< L,
                                      Compare >::getApertureAngle() const
{

  return _apertureAngle;

}


template < class L, class Compare >
inline
void gkg::StreamlineTractographyAlgorithm< L, Compare >::track(
              const gkg::SiteMap< L, int32_t, Compare >& rois,
              const gkg::SiteMap< L, int32_t, Compare >& subRois,
              const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy,
              const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy,
              const gkg::RCPointer< gkg::StartingOrientationStrategy >&
                                                    startingOrientationStrategy,
              gkg::OdfContinuousField& odfContinuousField,
              const gkg::Volume< int16_t >& computingMask,
              const Transform3d< float >&
                             transform3dFromRoisVoxelToOdfsReal,
              const Transform3d< float >&
                             transform3dFromRoisVoxelToMaskVoxel,
              const Transform3d< float >&
                             transform3dFromOdfsRealToMaskVoxel,
              const Transform3d< float >&
                             transform3dFromOdfsRealToRoisVoxel,
              gkg::BundleMap< L, Compare >& bundleMap,
              bool verbose ) const
{

  try
  {

    // evaluating ROI size(s)
    gkg::Vector3d< int32_t > roiSize( 1, 1, 1 );
    typename gkg::SiteMap< L, int32_t, Compare >::const_iterator
      r = rois.begin(),
      re = rois.end();
    std::list< gkg::Vector3d< int32_t > >::const_iterator s, se;
    while ( r != re )
    {

      s = r->second.begin(),
      se = r->second.end();
      while ( s != se )
      {

        if ( s->x + 1 > roiSize.x )
        {

          roiSize.x = s->x + 1;

        }
        if ( s->y + 1 > roiSize.y )
        {

          roiSize.y = s->y + 1;

        }
        if ( s->z + 1 > roiSize.z )
        {

          roiSize.z = s->z + 1;

        }
        ++ s;

      }
      ++ r;

    }

    // creating bouding box for computing mask
    gkg::BoundingBox< int32_t > computingMaskBoundingBox( computingMask );

    // looping over ROI(s)
    r = subRois.begin();
    re = subRois.end();
    int32_t roiIndex = 1;
    while ( r != re )
    {

      // displaying current ROI index
      if ( verbose )
      {

        if ( roiIndex != 1 )
        {

          std::cout << gkg::Eraser( 53 );

        }
        std::cout << " ROI[ " << std::setw( 7 ) << roiIndex
                  << " / " << std::setw( 7 ) << rois.getRanks().size()
                  << " ]" << std::flush;

      }

      // obtaining the label of the ROI
      L label = r->first;

      // allocating a new fiber set
      std::vector< typename gkg::BundleMap< L, Compare >::Fiber > fibers;

      // allocating a thread context
      gkg::StreamlineGauge gauge( ( int32_t )r->second.size() );

      gkg::ThreadedLoopContext< L, Compare >
        context( gauge,
                 *this,
                 r->second.begin(),
                 seedingStrategy,
                 stoppingStrategy,
                 startingOrientationStrategy,
                 odfContinuousField,
                 computingMask,
                 computingMaskBoundingBox,
                 transform3dFromRoisVoxelToOdfsReal,
                 transform3dFromRoisVoxelToMaskVoxel,
                 transform3dFromOdfsRealToMaskVoxel,
                 transform3dFromOdfsRealToRoisVoxel,
                 fibers,
                 verbose );

      // creating a threaded loop
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context,
                      0,                          // start index
                      ( int32_t )r->second.size(),// count
                      0 );                        // parallel

      // launching the threaded loop
      gauge.add( 0 );
      threadedLoop.launch();

      // adding the bundle to the bundle map
      bundleMap.addCurve3ds( label, fibers ); 

      ++ r;
      ++ roiIndex;

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 53 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class L, class Compares > "
             "inline "
             "void gkg::StreamlineTractographyAlgorithm< L, Compare >::track( "
             "const gkg::SiteMap< L, int32_t, Compare >& rois, "
             "const gkg::SiteMap< L, int32_t, Compare >& subRois, "
             "const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy, "
             "const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy, "
             "const gkg::RCPointer< gkg::StartingOrientationStrategy >& "
             "startingOrientationStrategy, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "const Transform3d< float >& "
             "transform3dFromRoisVoxelToMaskVoxel, "
             "const Transform3d< float >& "
             "transform3dFromOdfsRealToMaskVoxel, "
             "const Transform3d< float >& "
             "transform3dFromOdfsRealToRoisVoxel, "
             "gkg::BundleMap< L, Compare >& bundleMap, "
             "bool verbose ) const" );

}


#endif
