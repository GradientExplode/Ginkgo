#include <gkg-dmri-odf-functor/OdfFunctorNematicOrder.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorNematicOrder::OdfFunctorNematicOrder()
                            : gkg::OdfFunctor< gkg::Volume< float >, float >()
{
}


gkg::OdfFunctorNematicOrder::~OdfFunctorNematicOrder()
{
}


std::string gkg::OdfFunctorNematicOrder::getName() const
{

  return "nematic_order";

}


bool gkg::OdfFunctorNematicOrder::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorNematicOrder::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = odf.getNematicOrder();

  }
  GKG_CATCH( "void gkg::OdfFunctorNematicOrder::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorNematicOrder __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                                  gkg::OdfFunctorNematicOrder::getInstance() );
