#include <gkg-dmri-pdf-functor/EapFunctorGFA.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorGFA::EapFunctorGFA()
                   : gkg::EapFunctor< gkg::Volume< float >, float >()
{
}


gkg::EapFunctorGFA::~EapFunctorGFA()
{
}


std::string gkg::EapFunctorGFA::getName() const
{

  return "gfa";

}


bool gkg::EapFunctorGFA::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorGFA::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& eap,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = eap.getGeneralizedFractionalAnisotropy();

  }
  GKG_CATCH( "void gkg::EapFunctorGFA::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& eap, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorGFA __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerEapFunctor(
                                            gkg::EapFunctorGFA::getInstance() );

