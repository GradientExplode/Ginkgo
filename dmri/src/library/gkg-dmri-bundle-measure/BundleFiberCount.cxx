#include <gkg-dmri-bundle-measure/BundleFiberCount.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::BundleFiberCount::BundleFiberCount()
                      : gkg::BundleMeasure< int32_t >()
{
}


gkg::BundleFiberCount::~BundleFiberCount()
{
}


int32_t gkg::BundleFiberCount::get(
                             const gkg::BundleFiberCount::Bundle& bundle ) const
{

  try
  {

    return bundle.getCurve3dCount();

  }
  GKG_CATCH( "int32_t gkg::BundleFiberCount::get( "
             "const gkg::BundleFiberCount::Bundle& bundle ) const" );

}


template < class L, class Compare >
void gkg::BundleFiberCount::spreadSheetFunctor(
            const std::vector< std::string >& /*stringParameters*/,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )
{

  try
  {

    // collecting verbosity
    bool verbose = ( ( ( int32_t )( scalarParameters[ 0 ] + 0.5 ) ) ?
                     true : false );
    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleFiberCount bundleFiberCount;

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      if ( verbose )
      {

        std::cout << gkg::StringConverter::toString( label ) << " "
                  << std::flush;

      }

      gkg::Matrix&
          measure = measures[ label ][ "bundle_fiber_count" ];
      measure.reallocate( 1, 1 );
      measure( 0, 0 ) = ( double )bundleFiberCount.get( bundle );

      ++ l;

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::BundleFiberCount::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );

}


template void gkg::BundleFiberCount::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void gkg::BundleFiberCount::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

//
// adding check information to BundleFiberCount
//

static bool initializeBundleFiberCountChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "bundle_fiber_count",
      ".  N/A",
      ".  <P1>: verbosity (1->true, 0-> false)",
      0,
      1,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeBundleFiberCountChecker()" );

}


static bool initialized __attribute__((unused)) =
                                            initializeBundleFiberCountChecker();
