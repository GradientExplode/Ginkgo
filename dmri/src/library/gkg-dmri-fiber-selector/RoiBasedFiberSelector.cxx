#include <gkg-dmri-fiber-selector/RoiBasedFiberSelector.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-dmri-fiber-selector/FiberSelectionChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>



//
// class RoiBasedFiberSelectorContext< L, Compare >
//

template < class T, class L, class Compare >
gkg::RoiBasedFiberSelectorContext< T, L, Compare >::
                                                   RoiBasedFiberSelectorContext(
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle,
      const gkg::RoiBasedFiberSelector< T >& roiBasedFiberSelector,
      const float& inputFiberResamplingStep,
      std::set< int32_t >& selection )
                                        : gkg::LoopContext< int32_t >(),
                                          _bundle( bundle ),
                                          _roiBasedFiberSelector(
                                                        roiBasedFiberSelector ),
                                          _inputFiberResamplingStep(
                                                    inputFiberResamplingStep ),
                                          _selection( selection )
{
}


template < class T, class L, class Compare >
gkg::RoiBasedFiberSelectorContext< T, L, Compare >::
                                                 ~RoiBasedFiberSelectorContext()
{
}


template < class T, class L, class Compare >
void gkg::RoiBasedFiberSelectorContext< T, L, Compare >::doIt(
                                                             int32_t startIndex,
                                                             int32_t count )
{

  try
  {

    std::set< int32_t > localSelection;

    typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;

    // looping over fibers of the current bundle
    typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
      f = _bundle.begin();
    std::advance( f, startIndex );
    typename gkg::BundleMap< L, Compare >::Bundle::const_iterator fe = f;
    std::advance( fe, count );

    int32_t resampledPointCount = 0;
    int32_t fiberIndex = startIndex;

    while ( f != fe )
    {


      // resampling the fiber at the input resolution
      resampledPointCount = std::max( 2, ( int32_t )( f->getLength() /
                                          _inputFiberResamplingStep + 0.5 ) );
      resampledFiber = f->getEquidistantCurve( resampledPointCount );

      if ( _roiBasedFiberSelector.select( resampledFiber ) )
      {

        localSelection.insert( fiberIndex );

      }
      ++ f;
      ++ fiberIndex;

    }

    lock();

    std::set< int32_t >::const_iterator
      s = localSelection.begin(),
      se = localSelection.end();
    while ( s != se )
    {

      _selection.insert( *s );
      ++ s;

    }

    unlock();

  }
  GKG_CATCH( "template < class T, class L, class Compare > "
             "void gkg::RoiBasedFiberSelectorContext< T, L, Compare >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class RoiBasedFiberSelector< T >
//

template < class T >
gkg::RoiBasedFiberSelector< T >::RoiBasedFiberSelector(
                          gkg::RCPointer< gkg::Volume< T > > rois,
                          gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromRoisRealToFiberReal,
                          gkg::RCPointer< gkg::TestFunction< T > > testFunction,
                          gkg::RoiBasedFiberSelector< T >::Type type,
                          float minimumIntersectionLength,
                          float inputFiberResamplingStep )
                                : gkg::FiberSelector(),
                                  _rois( rois ),
                                  _testFunction( testFunction ),
                                  _type( type ),
                                  _minimumIntersectionLength(
                                    minimumIntersectionLength ),
                                  _inputFiberResamplingStep(
                                                     inputFiberResamplingStep ),
                                  _boundingBox( *rois )
{

  try
  {

    gkg::Vector3d< double > resolution;
    _rois->getResolution( resolution );

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

  }
  GKG_CATCH( "template < class T > "
             "gkg::RoiBasedFiberSelector< T >::RoiBasedFiberSelector( "
             "gkg::RCPointer< gkg::Volume< T > > rois, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "      transform3dFromRoisRealToFiberReal, "
             "gkg::RCPointer< gkg::TestFunction< T > > testFunction, "
             "gkg::RoiBasedFiberSelector< T >::Type type, "
             "float minimumIntersectionLength, "
             "float inputFiberResamplingStep  )" );

}


template < class T >
gkg::RoiBasedFiberSelector< T >::~RoiBasedFiberSelector()
{
}



template < class T >
gkg::RCPointer< gkg::Volume< T > > 
gkg::RoiBasedFiberSelector< T >::getRois() const
{

  try
  {

    return _rois;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Volume< T > > "
             "gkg::RoiBasedFiberSelector< T >::getRois() const" );

}


template < class T >
gkg::RCPointer< gkg::Transform3d< float > > 
gkg::RoiBasedFiberSelector< T >::getTransform3dFromRoisVoxelToFiberReal() const
{

  try
  {

    return _transform3dFromRoisVoxelToFiberReal;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< float > >  "
             "gkg::RoiBasedFiberSelector< T >::"
             "getTransform3dFromRoisVoxelToFiberReal() const" );

}


template < class T >
gkg::RCPointer< gkg::TestFunction< T > > 
gkg::RoiBasedFiberSelector< T >::getTestFunction() const
{

  try
  {

    return _testFunction;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::TestFunction< T > >  "
             "gkg::RoiBasedFiberSelector< T >::getTestFunction() const" );

}


template < class T >
bool gkg::RoiBasedFiberSelector< T >::select(
            const typename gkg::RoiBasedFiberSelector< T >::Fiber& fiber ) const
{

  try
  {

    gkg::Vector3d< int32_t > voxel;
    int32_t intersectingPointCount = 0;
    typename gkg::RoiBasedFiberSelector< T >::Fiber::const_iterator
       p = fiber.begin(),
       pe = fiber.end();
    while ( p != pe )
    {

      _transform3dFromRoisVoxelToFiberReal->getIntegerInverse( *p, voxel );
      if ( _boundingBox.contains( voxel ) )
      {

        if ( _testFunction->getState( ( *_rois )( voxel ) ) )
        {

          ++ intersectingPointCount;

        }

      }
      ++ p;

    }

    bool result = false;
    if ( _type == gkg::RoiBasedFiberSelector< T >::Intersection )
    {

      if ( !intersectingPointCount )
      {

        result = false;

      }
      else
      {

        float lengh = ( float )intersectingPointCount *
                       _inputFiberResamplingStep;
        if ( lengh >= _minimumIntersectionLength )
        {

         result = true;

        }
        else
        {

          result = false;

        }

      }

    }
    else
    {

      if ( !intersectingPointCount )
      {

        result = true;

      }
      else
      {

        result = false;

      }

    }

    return result;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::RoiBasedFiberSelector< T >::select( "
             "const typename gkg::RoiBasedFiberSelector< T >::Fiber& "
             "fiber ) const" );

}


template < class T >
template < class L, class Compare >
void gkg::RoiBasedFiberSelector< T >::selector(
                       const std::vector< std::string >& stringParameters,
                       const std::vector< double >& scalarParameters,
                       gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
                       std::map< L, std::set< int32_t >, Compare >& selection )
{

  try
  {

    // analyzing item type of scalar volume
    gkg::AnalyzedObject analyzedObject;
    std::string format;
    gkg::DiskFormatAnalyzer::getInstance().analyze( stringParameters[ 0 ],
                                                    format,
                                                    analyzedObject,
                                                    "Volume" );
    if ( analyzedObject.getObjectType() != "Volume" )
    {

      throw std::runtime_error( "not the good object type" );

    }
    std::string itemType = "";
    try
    {

      itemType = analyzedObject.getItemType();

    }
    catch( std::exception& )
    {

      throw std::runtime_error( "no item type found" );

    }
    if ( itemType != gkg::TypeOf< T >::getName() )
    {

      throw std::runtime_error( "not the good item type" );

    }

    // reading ROI volume
    gkg::RCPointer< gkg::Volume< T > > rois( new gkg::Volume< T > );
    gkg::Reader::getInstance().read( stringParameters[ 0 ], *rois );

    // reading rigid or affine 3D transform
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromRoisVoxelToFiberReal;
    if ( stringParameters[ 1 ] != "id" )
    {

      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( stringParameters[ 1 ].c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromRoisVoxelToFiberReal = rigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineTransform3d< float > >
            affineTransform3d( new gkg::AffineTransform3d< float > );
          std::ifstream is( stringParameters[ 1 ].c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3dFromRoisVoxelToFiberReal = affineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "invalid 3D ROIs->fibers transform" );

        }

      }

    }
    else
    {

      transform3dFromRoisVoxelToFiberReal.reset(
                                        new gkg::IdentityTransform3d< float > );

    }

    // setting the test function
    gkg::RCPointer< gkg::TestFunction< T > >
      testFunction( gkg::TestFunctionFactory< T >::getInstance().
                             createTestFunction( stringParameters[ 2 ],
                                                 ( T )scalarParameters[ 0 ],
                                                 ( T )scalarParameters[ 1 ] ) );

    // setting the selection type
    typename gkg::RoiBasedFiberSelector< T >::Type type;
    if ( stringParameters[ 3 ] == "intersection" )
    {

      type = gkg::RoiBasedFiberSelector< T >::Intersection;

    }
    else if ( stringParameters[ 3 ] == "no_intersection" )
    {

      type = gkg::RoiBasedFiberSelector< T >::NoIntersection;

    }
    else
    {

      throw std::runtime_error( "not a valid type" );

    }

    // obtaining the minimum intersection length and resampling step
    float minimumIntersectionLength = ( float )scalarParameters[ 2 ];
    float inputFiberResamplingStep = ( float )scalarParameters[ 3 ];

    // creating the ROI based fiber selector
    gkg::RoiBasedFiberSelector< T >
      roiBasedFiberSelector( rois,
                             transform3dFromRoisVoxelToFiberReal,
                             testFunction,
                             type,
                             minimumIntersectionLength,
                             inputFiberResamplingStep );

    // performing selection
    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& 
        bundle = l->second;

      gkg::RoiBasedFiberSelectorContext< T, L, Compare >
        roiBasedFiberSelectorContext( bundle,
                                      roiBasedFiberSelector,
                                      inputFiberResamplingStep,
                                      selection[ label ] );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &roiBasedFiberSelectorContext,
                      0,                                   // start index
                      ( int32_t )bundle.getCurve3dCount(), // count
                      0 );                                 // parallel
      threadedLoop.launch();

      ++ l;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class L, class Compare > "
             "void gkg::RoiBasedFiberSelector< T >::"
             "selector( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, std::set< int32_t > >& selection )" );

}




template class gkg::RoiBasedFiberSelector< int8_t >;
template class gkg::RoiBasedFiberSelector< uint8_t >;
template class gkg::RoiBasedFiberSelector< int16_t >;
template class gkg::RoiBasedFiberSelector< uint16_t >;
template class gkg::RoiBasedFiberSelector< int32_t >;
template class gkg::RoiBasedFiberSelector< uint32_t >;
template class gkg::RoiBasedFiberSelector< int64_t >;
template class gkg::RoiBasedFiberSelector< uint64_t >;
template class gkg::RoiBasedFiberSelector< float >;
template class gkg::RoiBasedFiberSelector< double >;

template void gkg::RoiBasedFiberSelector< int8_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int8_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint8_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint8_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int16_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int16_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint16_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint16_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int32_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int32_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint32_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint32_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int64_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< int64_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint64_t >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< uint64_t >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< float >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< float >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< double >::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::RoiBasedFiberSelector< double >::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );


//
// adding check information to FiberSelectionChecker
//

static bool initializeRoiBasedFiberSelectorChecker()
{

  try
  {

    gkg::FiberSelectionChecker::getInstance().addChecker(
      "roi_based",
      ".  <P1>: ROI volume name\n"
      ".  <P2>: from ROIs real to fiber real 3D transform name\n"
      ".        (set 'id' if no transform)\n"
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
      ".        - no_intersection\n",
      ".  <P1>: first threshold\n"
      ".  <P2>: second threshold (set 0.0 when useless)\n"
      ".  <P3>: minimum intersection length in mm (default=1mm)\n"
      ".  <P4>: input fiber resampling step in mm (default=0.1)\n",
      4,
      4,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeRoiBasedFiberSelectorChecker()" );

}


static bool initialized __attribute__((unused)) =
                                       initializeRoiBasedFiberSelectorChecker();

