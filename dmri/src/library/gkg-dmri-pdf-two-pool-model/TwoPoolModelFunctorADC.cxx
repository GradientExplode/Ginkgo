#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorADC.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorADC::TwoPoolModelFunctorADC()
                            : gkg::TwoPoolModelFunctor< gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorADC::~TwoPoolModelFunctorADC()
{
}


std::string gkg::TwoPoolModelFunctorADC::getName() const
{

  return "adc";

}


void gkg::TwoPoolModelFunctorADC::processItem(
                 std::vector< float >& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 2 coefficient(s)
    item.resize( 2 );

    // collecting two pool model ADC coefficient(s)
    item[ 0 ] = twoPoolModel.getApparentDiffusionCoefficient(
                                                      gkg::TwoPoolModel::Fast );
    item[ 1 ] = twoPoolModel.getApparentDiffusionCoefficient(
                                                      gkg::TwoPoolModel::Slow );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorADC::processItem( "
             "float& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorADC __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                                   gkg::TwoPoolModelFunctorADC::getInstance() );
