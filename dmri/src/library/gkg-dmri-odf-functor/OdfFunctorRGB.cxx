#include <gkg-dmri-odf-functor/OdfFunctorRGB.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorRGB::OdfFunctorRGB()
                   : gkg::OdfFunctor< gkg::Volume< gkg::RGBComponent >,
                                      gkg::RGBComponent >()
{
}


gkg::OdfFunctorRGB::~OdfFunctorRGB()
{
}


std::string gkg::OdfFunctorRGB::getName() const
{

  return "rgb";

}


bool gkg::OdfFunctorRGB::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorRGB::processItem(
                 gkg::RGBComponent& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    float scaling = odf.getGeneralizedFractionalAnisotropy() *
                    scalarParameters[ 0 ];

    gkg::Vector3d< float > principalOrientation = odf.getPrincipalOrientation();
    float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
    float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
    float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
    r = ( r > 255.0 ? 255.0 : r );
    g = ( g > 255.0 ? 255.0 : g );
    b = ( b > 255.0 ? 255.0 : b );

    item = gkg::RGBComponent( ( uint8_t )r, ( uint8_t )g, ( uint8_t )b );

  }
  GKG_CATCH( "void gkg::OdfFunctorRGB::processItem( "
             "gkg::RGBComponent& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorRGB __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                        gkg::RGBComponent >::getInstance().registerOdfFunctor(
                                           gkg::OdfFunctorRGB::getInstance() );
