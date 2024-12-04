#include <gkg-dmri-odf-functor/OdfFunctorOdfPeakMap.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-odf-functor/SphericalHarmonicsOdfPeakFinder.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorOdfPeakMap::OdfFunctorOdfPeakMap()
                          : gkg::OdfFunctor<
                              gkg::TextureMap< 
                                        std::vector< gkg::Vector3d< float > > >,
                              std::vector< gkg::Vector3d< float > > >()
{
}


gkg::OdfFunctorOdfPeakMap::~OdfFunctorOdfPeakMap()
{
}


std::string gkg::OdfFunctorOdfPeakMap::getName() const
{

  return "odf_peak_map";

}


bool gkg::OdfFunctorOdfPeakMap::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::OdfFunctorOdfPeakMap::isStandardBasisRequired() const" );

}


void gkg::OdfFunctorOdfPeakMap::processItem(
                  std::vector< gkg::Vector3d< float > >& item,
                  const gkg::OrientationDistributionFunction& odf,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    int32_t peakCount = ( int32_t )( scalarParameters[ 5 ] + 0.5 );
    float colatitudeDerivativeThreshold = ( float )scalarParameters[ 6 ];
    float resolution = ( float )scalarParameters[ 7 ];
    float clusteringThreshold = ( float )scalarParameters[ 8 ];
    float angularThreshold = ( float )scalarParameters[ 9 ];
    float minimumIncrement = ( float )scalarParameters[ 10 ];
    float maximumIncrement = ( float )scalarParameters[ 11 ];

    gkg::SphericalHarmonicsOdfPeakFinder
      sphericalHarmonicsOdfPeakFinder( peakCount,
                                       colatitudeDerivativeThreshold,
                                       resolution,
                                       clusteringThreshold,
                                       angularThreshold,
                                       minimumIncrement,
                                       maximumIncrement );
    std::vector< float > peakMagnitudes;
    sphericalHarmonicsOdfPeakFinder.getPeaks( odf, item, peakMagnitudes );

  }
  GKG_CATCH( "void gkg::OdfFunctorOdfPeakMap::processItem( "
             "std::vector< gkg::Vector3d< float > >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorOdfPeakMap __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::TextureMap< 
                                        std::vector< gkg::Vector3d< float > > >,
                        std::vector< gkg::Vector3d< float > > >::getInstance().
                 registerOdfFunctor( gkg::OdfFunctorOdfPeakMap::getInstance() );
