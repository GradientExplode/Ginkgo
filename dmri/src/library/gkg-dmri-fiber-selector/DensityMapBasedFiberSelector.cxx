#include <gkg-dmri-fiber-selector/DensityMapBasedFiberSelector.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-dmri-fiber-selector/FiberSelectionChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


gkg::DensityMapBasedFiberSelector::DensityMapBasedFiberSelector(
                      gkg::RCPointer< gkg::Volume< float > > densityMap,
                      gkg::RCPointer< gkg::Transform3d< float > >
                                      transform3dFromDensityMapRealToFiberReal,
                      gkg::RCPointer< gkg::TestFunction< float > > testFunction,
                      float minimumIntersectionLength,
                      float inputFiberResamplingStep )
                                  : gkg::FiberSelector(),
                                    _densityMap( densityMap ),
                                    _testFunction( testFunction ),
                                    _minimumIntersectionLength(
                                      minimumIntersectionLength ),
                                    _inputFiberResamplingStep(
                                                     inputFiberResamplingStep ),
                                    _boundingBox( *densityMap )
{

  try
  {

    gkg::Vector3d< double > resolution;
    _densityMap->getResolution( resolution );

    gkg::CompositeTransform3d< float >*
      transform3dFromDensityMapVoxelToFiberReal = 
        new gkg::CompositeTransform3d< float >;

    transform3dFromDensityMapVoxelToFiberReal->compose(
                             gkg::Scaling3d< float >( ( float )resolution.x,
                                                      ( float )resolution.y,
                                                      ( float )resolution.z ) );
    transform3dFromDensityMapVoxelToFiberReal->compose(
                                    *transform3dFromDensityMapRealToFiberReal );

    _transform3dFromDensityMapVoxelToFiberReal.reset( 
                                    transform3dFromDensityMapVoxelToFiberReal );

  }
  GKG_CATCH( "gkg::DensityMapBasedFiberSelector::DensityMapBasedFiberSelector( "
             "gkg::RCPointer< gkg::Volume< float > > densityMap, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromDensityMapVoxelToFiberReal, "
             "gkg::RCPointer< gkg::TestFunction< float > > testFunction, "
             "float minimumIntersectionLength, "
             "float inputFiberResamplingStep  )" );

}


gkg::DensityMapBasedFiberSelector::~DensityMapBasedFiberSelector()
{
}



gkg::RCPointer< gkg::Volume< float > > 
gkg::DensityMapBasedFiberSelector::getDensityMap() const
{

  try
  {

    return _densityMap;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::Volume< float > > "
             "gkg::DensityMapBasedFiberSelector::getDensityMap() const" );

}


gkg::RCPointer< gkg::Transform3d< float > > 
gkg::DensityMapBasedFiberSelector::
                           getTransform3dFromDensityMapVoxelToFiberReal() const
{

  try
  {

    return _transform3dFromDensityMapVoxelToFiberReal;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::Transform3d< float > >  "
             "gkg::DensityMapBasedFiberSelector::"
             "getTransform3dFromDensityMapVoxelToFiberReal() const" );

}


gkg::RCPointer< gkg::TestFunction< float > >
gkg::DensityMapBasedFiberSelector::getTestFunction() const
{

  try
  {

    return _testFunction;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::TestFunction< float > >  "
             "gkg::DensityMapBasedFiberSelector::getTestFunction() const" );

}


bool gkg::DensityMapBasedFiberSelector::select(
                   const gkg::DensityMapBasedFiberSelector::Fiber& fiber ) const
{

  try
  {

    gkg::Vector3d< int32_t > voxel;
    int32_t pointCount = 0;
    gkg::DensityMapBasedFiberSelector::Fiber::const_iterator
       p = fiber.begin(),
       pe = fiber.end();
    while ( p != pe )
    {

      _transform3dFromDensityMapVoxelToFiberReal->getIntegerInverse( *p,
                                                                     voxel );
      if ( _boundingBox.contains( voxel ) )
      {

        if ( _testFunction->getState( ( *_densityMap )( voxel ) ) )
        {

          ++ pointCount;

        }

      }
      ++ p;

    }

    bool result = false;
    if ( pointCount )
    {

      float lengh = ( float )pointCount *
                      _inputFiberResamplingStep;
      if ( lengh >= _minimumIntersectionLength )
      {

       result = true;

      }

    }
    return result;

  }
  GKG_CATCH( "bool gkg::DensityMapBasedFiberSelector::select( "
             "const gkg::DensityMapBasedFiberSelector::Fiber& fiber ) const" );

}


template < class L, class Compare >
void gkg::DensityMapBasedFiberSelector::selector(
                       const std::vector< std::string >& stringParameters,
                       const std::vector< double >& scalarParameters,
                       gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
                       std::map< L, std::set< int32_t >, Compare >& selection )
{

  try
  {

    // reading ROI volume
    gkg::RCPointer< gkg::Volume< float > >
      densityMap( new gkg::Volume< float > );
    gkg::Reader::getInstance().read( stringParameters[ 0 ], *densityMap );

    // reading rigid 3D transform
    gkg::RCPointer< gkg::RigidTransform3d< float > >
      transform3dFromRoisVoxelToFiberReal( new gkg::RigidTransform3d< float > );
    if ( stringParameters[ 1 ] != "id" )
    {

      std::ifstream is( stringParameters[ 1 ].c_str() );
      transform3dFromRoisVoxelToFiberReal->readTrm( is );
      is.close();

    }

    // setting the test function
    gkg::RCPointer< gkg::TestFunction< float > >
      testFunction( gkg::TestFunctionFactory< float >::getInstance().
                             createTestFunction( stringParameters[ 2 ],
                                             ( float )scalarParameters[ 0 ],
                                             ( float )scalarParameters[ 1 ] ) );

    // obtaining the minimum intersection length and resampling step
    float minimumIntersectionLength = ( float )scalarParameters[ 2 ];
    float inputFiberResamplingStep = ( float )scalarParameters[ 3 ];

    // creating the density map based fiber selector
    gkg::DensityMapBasedFiberSelector
      densityMapBasedFiberSelector( densityMap,
                                    transform3dFromRoisVoxelToFiberReal,
                                    testFunction,
                                    minimumIntersectionLength,
                                    inputFiberResamplingStep );

    // performing selection
    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      int32_t fiberIndex = 0;
      float fiberLength = 0.0;
      int32_t resampledPointCount = 0;
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        // resampling the fiber at the input resolution
        typename gkg::BundleMap< L, Compare >::Fiber resampledFiber;
        fiberLength = f->getLength();
        resampledPointCount = std::max( 2, ( int32_t )( fiberLength /
                                       ( float )scalarParameters[ 3 ] + 0.5 ) );
        resampledFiber = f->getEquidistantCurve( resampledPointCount );

        if ( densityMapBasedFiberSelector.select( resampledFiber ) )
        {

          selection[ label ].insert( fiberIndex );

        }
        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::DensityMapBasedFiberSelector::"
             "selector( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, std::set< int32_t > >& selection )" );

}



template void gkg::DensityMapBasedFiberSelector::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::DensityMapBasedFiberSelector::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );


//
// adding check information to FiberSelectionChecker
//

static bool initializeDensityMapBasedFiberSelectorChecker()
{

  try
  {

    gkg::FiberSelectionChecker::getInstance().addChecker(
      "density_map_based",
      ".  <P1>: Density map volume name\n"
      ".  <P2>: from density map real to fiber real 3D transform name\n"
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
      ".        - ou -> outside\n",
      ".  <P1>: first threshold\n"
      ".  <P2>: second threshold (set 0.0 when useless)\n"
      ".  <P3>: minimum intersection length in mm (default=1mm)\n"
      ".  <P4>: input fiber resampling step in mm (default=0.1)\n",
      3,
      4,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeDensityMapBasedFiberSelectorChecker()" );

}


static bool initialized __attribute__((unused)) =
                                initializeDensityMapBasedFiberSelectorChecker();

