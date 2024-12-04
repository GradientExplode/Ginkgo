#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorRoiBasedSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorRoiBasedSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRoiBasedSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class RoiBasedSelectionContext< L, Compare >
//

template < class L, class Compare >
gkg::RoiBasedSelectionContext< L, Compare >::RoiBasedSelectionContext(
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle,
      const float& inputFiberResamplingStep,
      const gkg::RCPointer< gkg::Volume< int32_t > >& rois,
      const gkg::BoundingBox< int32_t >& boundingBox,
      const gkg::RCPointer< gkg::TestFunction< int32_t > >& testFunction,
      const gkg::RCPointer< gkg::Transform3d< float > >& 
                                            transform3dFromRoisVoxelToFiberReal,
      int32_t& globalBundlePointCount,
      int32_t& bundleIntersectingPointCount )
                                        : gkg::LoopContext< int32_t >(),
                                          _bundle( bundle ),
                                          _inputFiberResamplingStep(
                                                    inputFiberResamplingStep ),
                                          _rois( rois ),
                                          _boundingBox( boundingBox ),
                                          _testFunction( testFunction ),
                                          _transform3dFromRoisVoxelToFiberReal(
                                          transform3dFromRoisVoxelToFiberReal ),
                                          _globalBundlePointCount(
                                                       globalBundlePointCount ),
                                          _bundleIntersectingPointCount(
                                                  bundleIntersectingPointCount )
{
}


template < class L, class Compare >
gkg::RoiBasedSelectionContext< L, Compare >::~RoiBasedSelectionContext()
{
}


template < class L, class Compare >
void gkg::RoiBasedSelectionContext< L, Compare >::doIt( int32_t startIndex,
                                                         int32_t count )
{

  try
  {

    int32_t localGlobalBundlePointCount = 0;
    int32_t localBundleIntersectingPointCount = 0;

    gkg::Vector3d< int32_t > siteInRoiFrame;
    int32_t resampledPointCount = 0;
    typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;

    // looping over fibers of the current bundle
    typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
      f = _bundle.begin();
    std::advance( f, startIndex );
    typename gkg::BundleMap< L, Compare >::Bundle::const_iterator fe = f;
    std::advance( fe, count );

    while ( f != fe )
    {


      // resampling the fiber at the input resolution
      resampledPointCount = std::max( 2, ( int32_t )( f->getLength() /
                                          _inputFiberResamplingStep + 0.5 ) );
      resampledFiber = f->getEquidistantCurve( resampledPointCount );

      localGlobalBundlePointCount += resampledPointCount;

      // looping over points of the current fiber and processing the vector of
      // labels
      typename gkg::BundleMap< L, Compare >::Fiber::const_iterator
        p = resampledFiber.begin(),
        pe = resampledFiber.end();
      while ( p != pe )
      {

        _transform3dFromRoisVoxelToFiberReal->getIntegerInverse(
                                                             *p,
                                                             siteInRoiFrame );
        if( _boundingBox.contains( siteInRoiFrame ) )
        {

          if ( _testFunction->getState( ( *_rois )( siteInRoiFrame ) ) )
          {

            ++ localBundleIntersectingPointCount;

          }

        }

        ++ p;

      }

      ++ f;

    }

    lock();

    _globalBundlePointCount += localGlobalBundlePointCount;
    _bundleIntersectingPointCount += localBundleIntersectingPointCount;

    unlock();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::RoiBasedSelectionContext< L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
//
//

template < class L, class Compare >
inline
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::
                                             BundleMapOperatorRoiBasedSelection(
                           const std::vector< std::string >& stringParameters,
                           const std::vector< double >& scalarParameters,
                           bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( ( stringParameters.size() != 4U ) )
    {

      throw std::runtime_error( "string parameters must have 4 arguments" );

    }

    _fileNameRois = stringParameters[ 0 ];
    _fileNameTransform3d = stringParameters[ 1 ];


    // setting the test function
    _testFunction.reset( gkg::TestFunctionFactory< int32_t >::getInstance().
                             createTestFunction( stringParameters[ 2 ],
                                ( int32_t )( scalarParameters[ 0 ] + 0.5 ),
                                ( int32_t )( scalarParameters[ 1 ] + 0.5 ) ) );

    if ( stringParameters[ 3 ] == "intersection" )
    {

      _type = gkg::BundleMapOperatorRoiBasedSelection< L,
                                                       Compare >::Intersection;

    }
    else if ( stringParameters[ 3 ] == "no_intersection" )
    {

      _type = gkg::BundleMapOperatorRoiBasedSelection< L,
                                                      Compare >::NoIntersection;

    }
    else
    {

      throw std::runtime_error( "bad intersection type" );

    }


    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 4U  )
    {

      throw std::runtime_error( "scalar parameters must have 4 arguments" );

    }

    _intersectionRatio = scalarParameters[ 2 ];
    _inputFiberResamplingStep = scalarParameters[ 3 ];

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "BundleMapOperatorRoiBasedSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::
                                           ~BundleMapOperatorRoiBasedSelection()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // allocating the ROI volume
    _rois.reset( new gkg::Volume< int32_t > );

    // reading the ROI from file
    gkg::TypedVolumeReaderProcess< int32_t > typedVolumeReaderProcess( *_rois );
    typedVolumeReaderProcess.execute( _fileNameRois );

    gkg::Vector3d< double > resolution;
    _rois->getResolution( resolution );

    // reading or building the 3D transform from ROI to DWI frame
    gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromRoisRealToFiberReal;
    if ( _fileNameTransform3d != "id" )
    {

     try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( _fileNameTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromRoisRealToFiberReal = rigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineTransform3d< float > >
            affineTransform3d( new gkg::AffineTransform3d< float > );
          std::ifstream is( _fileNameTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3dFromRoisRealToFiberReal = affineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "invalid 3D ROIs->fibers transform" );

        }

      }

    }
    else
    {

      transform3dFromRoisRealToFiberReal.reset(
                                        new gkg::IdentityTransform3d< float > );

    }

    // building the 3D transform from ROI voxel coordinates to DWI real 
    // coordinate
    gkg::CompositeTransform3d< float >* transform3dFromRoisVoxelToFiberReal = 
      new gkg::CompositeTransform3d< float >;

    transform3dFromRoisVoxelToFiberReal->compose(
                             gkg::Scaling3d< float >( ( float )resolution.x,
                                                      ( float )resolution.y,
                                                      ( float )resolution.z ) );
    transform3dFromRoisVoxelToFiberReal->compose(
                                          *transform3dFromRoisRealToFiberReal );

    _transform3dFromRoisVoxelToFiberReal.reset( 
                                          transform3dFromRoisVoxelToFiberReal );

    // adding contribution of the input bundle map to the output bundle map
    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // sanity checks
    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }

    // building the bounding box of the ROI mask
    gkg::BoundingBox< int32_t > boundingBox( *_rois );


    // looping over input bundles
    int32_t globalBundlePointCount = 0;
    int32_t bundleIntersectingPointCount = 0;
    float ratio = 0.0;

    typename gkg::BundleMap< L, Compare >::const_iterator
      b = inputBundleMap->begin(),
      be = inputBundleMap->end();
    while ( b != be )
    {

      // resetting the global bundle and bundle intersecting point count(s)
      globalBundlePointCount = 0;
      bundleIntersectingPointCount = 0;

      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = b->second;

      gkg::RoiBasedSelectionContext< L, Compare >
        roiBasedSelectionContext( bundle,
                                  _inputFiberResamplingStep,
                                  _rois,
                                  boundingBox,
                                  _testFunction,
                                  _transform3dFromRoisVoxelToFiberReal,
                                  globalBundlePointCount,
                                  bundleIntersectingPointCount );


      gkg::ThreadedLoop< int32_t >
        threadedLoop( &roiBasedSelectionContext,
                      0,                                   // start index
                      ( int32_t )bundle.getCurve3dCount(), // count
                      0 );                                 // parallel
      threadedLoop.launch();


      ratio = ( float )bundleIntersectingPointCount /
              ( float )globalBundlePointCount;

      if ( _type == 
           gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::Intersection )
      {

        if ( ratio >= _intersectionRatio )
        {

          outputBundleMap->addBundle( b->first, b->second );

        }

      }
      else
      {

        if ( ratio <= _intersectionRatio )
        {

          outputBundleMap->addBundle( b->first, b->second );

        }

      }
      
      ++ b;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->addContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::getStaticName()
{

  try
  {

    return "roi-based-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: ROI mask file name\n"
      ".  <P2>: ROI to DWI frame transform3d filename (default=id)\n"
      ".  <P3>: test mode among\n"
      ".        - lt -> lower than\n"
      ".        - le -> lower or equal to\n"
      ".        - eq -> equal to\n"
      ".        - di -> different from\n"
      ".        - ge -> greater or equal to\n"
      ".        - gt -> greater than\n"
      ".        - be -> between or equal\n"
      ".        - bt -> between\n"
      ".        - oe -> outside or equal\n"
      ".        - ou -> outside\n"
      ".  <P4>: intersection type among\n"
      ".        - intersection\n"
      ".        - no_intersection\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: first threshold\n"
      ".  <P2>: second threshold (set 0.0 when useless)\n"
      ".  <P3>: ratio of points of the bundle intersecting the\n"
      ".        selected ROI (it is a minimum ratio when choosing\n"
      ".        the 'intersection' mode and a maximum ratio when\n"
      ".        choosing the 'no_intersection' mode\n"
      ".  <P4>: input fiber resampling step in mm (default=0.1mm)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRoiBasedSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
