#include <gkg-dmri-fiber-measure/FiberCurvatureProfile.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasureChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::FiberCurvatureProfile::FiberCurvatureProfile()
                           : gkg::FiberMeasure< std::vector< float > >()
{
}


gkg::FiberCurvatureProfile::~FiberCurvatureProfile()
{
}


std::vector< float > 
gkg::FiberCurvatureProfile::get(
                          const gkg::FiberCurvatureProfile::Fiber& fiber ) const
{

  try
  {

    std::vector< float > curvatureProfile = fiber.getCurvatureProfile();

    return curvatureProfile;

  }
  GKG_CATCH( "std::vector< float > gkg::FiberCurvatureProfile::get( "
             "const gkg::FiberCurvatureProfile::Fiber& fiber ) const" );

}


template < class L, class Compare >
void gkg::FiberCurvatureProfile::spreadSheetFunctor(
            const std::vector< std::string >& /*stringParameters*/,
            const std::vector< double >& /*scalarParameters*/,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< int32_t, std::map< std::string, gkg::Matrix > >,
                      Compare >& measures )
{

  try
  {

    gkg::FiberCurvatureProfile fiberCurvatureProfile;

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

        std::vector< float > curvatureProfile = fiberCurvatureProfile.get( *f );
        int32_t size = ( int32_t )curvatureProfile.size();
        gkg::Matrix&
          measure = measures[ label ][ fiberIndex ][ "curvature_profile" ];
        measure.reallocate( 1, size );
        measure.setRow( 0, gkg::Vector( curvatureProfile ) );

        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::FiberCurvatureProfile::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< int32_t, std::map< std::string, gkg::Matrix > >, "
             "Compare >& measures )" );


}


template void gkg::FiberCurvatureProfile::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void gkg::FiberCurvatureProfile::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


//
// adding check information to FiberMeasureChecker
//

static bool initializeFiberCurvatureProfileChecker()
{

  try
  {

    gkg::FiberMeasureChecker::getInstance().addChecker(
      "curvature_profile",
      ".  N/A",
      ".  N/A",
      0,
      0,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeFiberCurvatureProfileChecker()" );

}


static bool initialized __attribute__((unused)) =
                                       initializeFiberCurvatureProfileChecker();

