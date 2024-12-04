#include <gkg-dmri-odf-functor/OdfFunctorPrincipalOrientation.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorPrincipalOrientation::OdfFunctorPrincipalOrientation()
                                    : gkg::OdfFunctor<
                                             gkg::Volume< Vector3d< float > >,
                                             gkg::Vector3d< float > >()
{
}


gkg::OdfFunctorPrincipalOrientation::~OdfFunctorPrincipalOrientation()
{
}


std::string gkg::OdfFunctorPrincipalOrientation::getName() const
{

  return "principal_orientation";

}


bool gkg::OdfFunctorPrincipalOrientation::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorPrincipalOrientation::processItem(
                 gkg::Vector3d< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = odf.getPrincipalOrientation();

  }
  GKG_CATCH( "void gkg::OdfFunctorPrincipalOrientation::processItem( "
             "gkg::Vector3d< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorPrincipalOrientation
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                        gkg::Vector3d< float > >::getInstance().
                                                           registerOdfFunctor(
                           gkg::OdfFunctorPrincipalOrientation::getInstance() );
