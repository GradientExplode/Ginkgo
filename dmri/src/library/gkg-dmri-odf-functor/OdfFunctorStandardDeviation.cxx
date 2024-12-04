#include <gkg-dmri-odf-functor/OdfFunctorStandardDeviation.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorStandardDeviation::OdfFunctorStandardDeviation()
                                 : gkg::OdfFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::OdfFunctorStandardDeviation::~OdfFunctorStandardDeviation()
{
}


std::string gkg::OdfFunctorStandardDeviation::getName() const
{

  return "standard_deviation";

}


bool gkg::OdfFunctorStandardDeviation::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorStandardDeviation::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = odf.getStandardDeviation();

  }
  GKG_CATCH( "void gkg::OdfFunctorStandardDeviation::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorStandardDeviation __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                             gkg::OdfFunctorStandardDeviation::getInstance() );
