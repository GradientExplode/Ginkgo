#include <gkg-dmri-odf-functor/OdfFunctorGFA.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorGFA::OdfFunctorGFA()
                   : gkg::OdfFunctor< gkg::Volume< float >, float >()
{
}


gkg::OdfFunctorGFA::~OdfFunctorGFA()
{
}


std::string gkg::OdfFunctorGFA::getName() const
{

  return "gfa";

}


bool gkg::OdfFunctorGFA::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorGFA::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = odf.getGeneralizedFractionalAnisotropy();

  }
  GKG_CATCH( "void gkg::OdfFunctorGFA::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorGFA __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                                            gkg::OdfFunctorGFA::getInstance() );
