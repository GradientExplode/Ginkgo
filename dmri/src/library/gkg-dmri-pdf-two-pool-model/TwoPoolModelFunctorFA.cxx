#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFA.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorFA::TwoPoolModelFunctorFA()
                            : gkg::TwoPoolModelFunctor< gkg::Volume< float >,
                                                        std::vector< float > >()
{
}


gkg::TwoPoolModelFunctorFA::~TwoPoolModelFunctorFA()
{
}


std::string gkg::TwoPoolModelFunctorFA::getName() const
{

  return "fa";

}


void gkg::TwoPoolModelFunctorFA::processItem(
                 std::vector< float >& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 2 coefficient(s)
    item.resize( 2 );

    // collecting two pool model FA coefficient(s)    
    item[ 0 ] = twoPoolModel.getFractionalAnisotropy( gkg::TwoPoolModel::Fast );
    item[ 1 ] = twoPoolModel.getFractionalAnisotropy( gkg::TwoPoolModel::Slow );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorFA::processItem( "
             "float& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorFA __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< float >,
                                 std::vector< float > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                                   gkg::TwoPoolModelFunctorFA::getInstance() );
