#include <gkg-dmri-odf-functor/OdfFunctorNormalizedEntropy.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorNormalizedEntropy::OdfFunctorNormalizedEntropy()
                                 : gkg::OdfFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::OdfFunctorNormalizedEntropy::~OdfFunctorNormalizedEntropy()
{
}


std::string gkg::OdfFunctorNormalizedEntropy::getName() const
{

  return "normalized_entropy";

}


bool gkg::OdfFunctorNormalizedEntropy::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorNormalizedEntropy::processItem(
                 float& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = odf.getNormalizedEntropy();

  }
  GKG_CATCH( "void gkg::OdfFunctorNormalizedEntropy::processItem( "
             "float& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorNormalizedEntropy __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                             gkg::OdfFunctorNormalizedEntropy::getInstance() );
