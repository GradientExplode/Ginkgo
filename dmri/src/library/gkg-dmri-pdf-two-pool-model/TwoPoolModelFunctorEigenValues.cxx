#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorEigenValues.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorEigenValues::TwoPoolModelFunctorEigenValues()
                            : gkg::TwoPoolModelFunctor< gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorEigenValues::~TwoPoolModelFunctorEigenValues()
{
}


std::string gkg::TwoPoolModelFunctorEigenValues::getName() const
{

  return "eigenvalues";

}


void gkg::TwoPoolModelFunctorEigenValues::processItem(
                 std::vector< float >& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 6 coefficient(s)
    item.resize( 6 );

    // collecting two pool model coefficient(s)
    item[  0 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::FastMaximum );
    item[  1 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::FastMiddle );
    item[  2 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::FastMinimum );
    item[  3 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::SlowMaximum );
    item[  4 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::SlowMiddle );
    item[  5 ] = twoPoolModel.getEigenValue( gkg::TwoPoolModel::SlowMinimum );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorEigenValues::processItem( "
             "std::vector< float >& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorEigenValues
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                           gkg::TwoPoolModelFunctorEigenValues::getInstance() );
