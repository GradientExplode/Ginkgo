#include <gkg-dmri-odf-functor/OdfFunctorMinMaxNormalizedVolume.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMinMaxNormalizedVolume::OdfFunctorMinMaxNormalizedVolume()
                                      : gkg::OdfFunctor< gkg::Volume< float >,
                                                       std::vector< float > >()
{
}


gkg::OdfFunctorMinMaxNormalizedVolume::~OdfFunctorMinMaxNormalizedVolume()
{
}


std::string gkg::OdfFunctorMinMaxNormalizedVolume::getName() const
{

  return "minmax_normalized_volume";

}


bool gkg::OdfFunctorMinMaxNormalizedVolume::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMinMaxNormalizedVolume::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // preparing min max filter(s)
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float minimum;
    float maximum;

    // processing min-max scaling
    item = odf.getOrientationProbabilities();

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, 1.0 );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMinMaxNormalizedVolume::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMinMaxNormalizedVolume 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                        gkg::OdfFunctorMinMaxNormalizedVolume::getInstance() );
