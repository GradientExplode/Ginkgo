#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorSlowRGB.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorSlowRGB::TwoPoolModelFunctorSlowRGB()
                                : gkg::TwoPoolModelFunctor<
                                               gkg::Volume< gkg::RGBComponent >,
                                               gkg::RGBComponent >()
{
}


gkg::TwoPoolModelFunctorSlowRGB::~TwoPoolModelFunctorSlowRGB()
{
}


std::string gkg::TwoPoolModelFunctorSlowRGB::getName() const
{

  return "slow_rgb";

}


void gkg::TwoPoolModelFunctorSlowRGB::processItem(
                 RGBComponent& item,
                 const gkg::TwoPoolModel& twoPoolModel,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    float scaling = twoPoolModel.getFractionalAnisotropy(
                                                     gkg::TwoPoolModel::Slow ) *
                    scalarParameters[ 0 ];

    gkg::Vector3d< float >
      principalOrientation = twoPoolModel.getEigenVector(
                                               gkg::TwoPoolModel::SlowMaximum );
    float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
    float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
    float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
    r = ( r > 255.0 ? 255.0 : r );
    g = ( g > 255.0 ? 255.0 : g );
    b = ( b > 255.0 ? 255.0 : b );

    item = gkg::RGBComponent( ( uint8_t )r, ( uint8_t )g, ( uint8_t )b );

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorSlowRGB::processItem( "
             "float& item, "
             "const gkg::TwoPoolModel& twoPoolModel, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorSlowRGB __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                           gkg::RGBComponent >::getInstance().
                                                    registerTwoPoolModelFunctor(
                               gkg::TwoPoolModelFunctorSlowRGB::getInstance() );
