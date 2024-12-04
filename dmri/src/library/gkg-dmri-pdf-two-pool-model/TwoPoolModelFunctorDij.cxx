#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorDij.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorDij::TwoPoolModelFunctorDij()
                            : gkg::TwoPoolModelFunctor< gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorDij::~TwoPoolModelFunctorDij()
{
}


std::string gkg::TwoPoolModelFunctorDij::getName() const
{

  return "dij";

}


void gkg::TwoPoolModelFunctorDij::processItem(
                 std::vector< float >& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 12 coefficient(s)
    item.resize( 12 );

    // collecting two pool model coefficient(s)
    item[  0 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDxx );
    item[  1 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDxy );
    item[  2 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDxz );
    item[  3 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDyy );
    item[  4 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDyz );
    item[  5 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::FastDzz );
    item[  6 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDxx );
    item[  7 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDxy );
    item[  8 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDxz );
    item[  9 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDyy );
    item[ 10 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDyz );
    item[ 11 ] = twoPoolModel.getCoefficient( gkg::TwoPoolModel::SlowDzz );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorDij::processItem( "
             "std::vector< float >& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorDij __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                                   gkg::TwoPoolModelFunctorDij::getInstance() );
