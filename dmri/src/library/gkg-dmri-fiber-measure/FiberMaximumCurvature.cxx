#include <gkg-dmri-fiber-measure/FiberMaximumCurvature.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasureChecker.h>
#include <gkg-dmri-fiber-selector/FiberSelectionChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-dmri-fiber-selector/MeasureBasedFiberSelector_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::FiberMaximumCurvature::FiberMaximumCurvature()
                           : gkg::FiberMeasure< float >()
{
}


gkg::FiberMaximumCurvature::~FiberMaximumCurvature()
{
}


float
gkg::FiberMaximumCurvature::get(
                          const gkg::FiberMaximumCurvature::Fiber& fiber ) const
{

  try
  {

    std::vector< float > curvatureProfile = fiber.getCurvatureProfile();

    gkg::MaximumFilter< std::vector< float >, float >
      maximumFilter;
    float maximumValue = 0.0;
    maximumFilter.filter( curvatureProfile, maximumValue );

    return maximumValue;

  }
  GKG_CATCH( "float gkg::FiberMaximumCurvature::get( "
             "const gkg::FiberMaximumCurvature::Fiber& fiber ) const" );

}


template < class L, class Compare >
void gkg::FiberMaximumCurvature::spreadSheetFunctor(
            const std::vector< std::string >& /*stringParameters*/,
            const std::vector< double >& /*scalarParameters*/,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< int32_t, std::map< std::string, gkg::Matrix > >,
                      Compare >& measures )
{

  try
  {

    gkg::FiberMaximumCurvature fiberMaximumCurvature;

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      int32_t fiberIndex = 0;
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        float maximumCurvature = fiberMaximumCurvature.get( *f );
        gkg::Matrix&
          measure = measures[ label ][ fiberIndex ][ "maximum_curvature" ];
        measure.reallocate( 1, 1 );
        measure( 0, 0 ) = maximumCurvature;

        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::FiberMaximumCurvature::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< int32_t, std::map< std::string, gkg::Matrix > >, "
             "Compare >& measures )" );


}


template < class L, class Compare >
void gkg::FiberMaximumCurvature::selector(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L, std::set< int32_t >, Compare >& selection )
{

  try
  {

    gkg::RCPointer< gkg::FiberMaximumCurvature >
      fiberMaximumCurvature( new gkg::FiberMaximumCurvature );
    gkg::RCPointer< gkg::TestFunction< float > >
      testFunction;
    if ( scalarParameters.size() > 1U )
    {

      testFunction.reset( gkg::TestFunctionFactory< float >::getInstance().
                            createTestFunction(
                              stringParameters[ 0 ],
                              ( float )scalarParameters[ 0 ],
                              ( float )scalarParameters[ 1 ] ) );

    }
    else
    {

      testFunction.reset( gkg::TestFunctionFactory< float >::getInstance().
                            createTestFunction(
                              stringParameters[ 0 ],
                              ( float )scalarParameters[ 0 ] ) );

    }
    gkg::MeasureBasedFiberSelector< float >
      measureBasedFiberSelector( fiberMaximumCurvature,
                                 testFunction );

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      int32_t fiberIndex = 0;
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        if ( measureBasedFiberSelector.select( *f ) )
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
             "void gkg::FiberMaximumCurvature::selector( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, std::set< int32_t >, Compare >& selection )" );

}


template void gkg::FiberMaximumCurvature::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void gkg::FiberMaximumCurvature::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void gkg::FiberMaximumCurvature::selector< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string, std::set< int32_t > >& selection );

template void gkg::FiberMaximumCurvature::selector< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t, std::set< int32_t > >& selection );


//
// adding check information to FiberMeasureChecker & FiberSelectionChecker
//

static bool initializeFiberMaximumCurvatureChecker()
{

  try
  {

    gkg::FiberMeasureChecker::getInstance().addChecker(
      "maximum_curvature",
      ".  N/A",
      ".  N/A",
      0,
      0,
      0 );

    gkg::FiberSelectionChecker::getInstance().addChecker(
      "maximum_curvature",
      ".  <P1>: test mode among\n"
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
      ".  <P2>: second threshold (set 0.0 when useless)\n",
      1,
      2,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeFiberMaximumCurvatureMeasureChecker()" );

}


static bool initialized __attribute__((unused)) =
                                       initializeFiberMaximumCurvatureChecker();

