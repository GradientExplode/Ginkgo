#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorVolumeFractions.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorVolumeFractions::TwoPoolModelFunctorVolumeFractions()
                                        : gkg::TwoPoolModelFunctor<
                                                        gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorVolumeFractions::~TwoPoolModelFunctorVolumeFractions()
{
}


std::string gkg::TwoPoolModelFunctorVolumeFractions::getName() const
{

  return "volume_fractions";

}


void gkg::TwoPoolModelFunctorVolumeFractions::processItem(
                 std::vector< float >& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 2 coefficient(s)
    item.resize( 2 );

    // collecting two pool model coefficient(s)
    item[ 0 ] = twoPoolModel.getFastVolumeFraction();
    item[ 1 ] = twoPoolModel.getSlowVolumeFraction();

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorVolumeFractions::processItem( "
             "std::vector< float >& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorVolumeFractions 
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                       gkg::TwoPoolModelFunctorVolumeFractions::getInstance() );
