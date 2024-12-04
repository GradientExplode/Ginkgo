#include <gkg-dmri-odf-functor/OdfFunctorOdfGibbsSampling.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorOdfGibbsSampling::OdfFunctorOdfGibbsSampling()
                                : gkg::OdfFunctor<
                                  gkg::TextureMap< 
                                        std::vector< gkg::Vector3d< float > > >,
                                  std::vector< gkg::Vector3d< float > > >()
{
}


gkg::OdfFunctorOdfGibbsSampling::~OdfFunctorOdfGibbsSampling()
{
}


std::string gkg::OdfFunctorOdfGibbsSampling::getName() const
{

  return "odf_gibbs_sampling";

}


bool gkg::OdfFunctorOdfGibbsSampling::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::OdfFunctorOdfGibbsSampling::isStandardBasisRequired() "
             "const" );

}


void gkg::OdfFunctorOdfGibbsSampling::processItem(
                  std::vector< gkg::Vector3d< float > >& item,
                  const gkg::OrientationDistributionFunction& odf,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    if ( _randomGenerator.isNull() )
    {

      _randomGenerator.reset( new gkg::RandomGenerator(
                                                 gkg::RandomGenerator::Taus ) );

    }

    int32_t sampleCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );
    float temperature = ( float )scalarParameters[ 6 ];

    item.resize( sampleCount );

    odf.getGibbsSampledOrientations( *_randomGenerator,
                                     item,
                                     temperature );

  }
  GKG_CATCH( "void gkg::OdfFunctorOdfGibbsSampling::processItem( "
             "std::vector< gkg::Vector3d< float > >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorOdfGibbsSampling __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::TextureMap< 
                                        std::vector< gkg::Vector3d< float > > >,
                        std::vector< gkg::Vector3d< float > > >::getInstance().
                 registerOdfFunctor(
                               gkg::OdfFunctorOdfGibbsSampling::getInstance() );
