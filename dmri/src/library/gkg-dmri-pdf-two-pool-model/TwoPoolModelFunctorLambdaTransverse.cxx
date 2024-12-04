#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorLambdaTransverse.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorLambdaTransverse::TwoPoolModelFunctorLambdaTransverse()
                                        : gkg::TwoPoolModelFunctor<
                                                        gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorLambdaTransverse::~TwoPoolModelFunctorLambdaTransverse()
{
}


std::string gkg::TwoPoolModelFunctorLambdaTransverse::getName() const
{

  return "lambda_transverse";

}


void gkg::TwoPoolModelFunctorLambdaTransverse::processItem(
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
    item[ 0 ] = twoPoolModel.getLambdaTransverse( gkg::TwoPoolModel::Fast );
    item[ 1 ] = twoPoolModel.getLambdaTransverse( gkg::TwoPoolModel::Slow );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorLambdaTransverse::processItem( "
             "std::vector< float >& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorLambdaTransverse 
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                      gkg::TwoPoolModelFunctorLambdaTransverse::getInstance() );
