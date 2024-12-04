#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorLambdaParallel.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorLambdaParallel::TwoPoolModelFunctorLambdaParallel()
                                        : gkg::TwoPoolModelFunctor<
                                                        gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorLambdaParallel::~TwoPoolModelFunctorLambdaParallel()
{
}


std::string gkg::TwoPoolModelFunctorLambdaParallel::getName() const
{

  return "lambda_parallel";

}


void gkg::TwoPoolModelFunctorLambdaParallel::processItem(
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
    item[ 0 ] = twoPoolModel.getLambdaParallel( gkg::TwoPoolModel::Fast );
    item[ 1 ] = twoPoolModel.getLambdaParallel( gkg::TwoPoolModel::Slow );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorLambdaParallel::processItem( "
             "std::vector< float >& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorLambdaParallel 
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                       gkg::TwoPoolModelFunctorLambdaParallel::getInstance() );