#include <gkg-dmri-odf-dsi/DsiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dsi/DsiOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::DsiOdfCartesianFieldFunctor::DsiOdfCartesianFieldFunctor(
                         const gkg::DsiOdfCartesianField& dsiOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // Average Normalized DW Signal
    _returnToOriginProbability.reallocate(
                         dsiOdfCartesianField.getSizeX(),
                         dsiOdfCartesianField.getSizeY(),
                         dsiOdfCartesianField.getSizeZ() );
    _returnToOriginProbability.getHeader().addAttribute(
                                    "resolutionX",
                                    dsiOdfCartesianField.getResolutionX() );
    _returnToOriginProbability.getHeader().addAttribute(
                                    "resolutionY",
                                    dsiOdfCartesianField.getResolutionY() );
    _returnToOriginProbability.getHeader().addAttribute(
                                    "resolutionZ",
                                    dsiOdfCartesianField.getResolutionZ() );
    _returnToOriginProbability.fill( 0.0 );

    // Mean Squared Displacement 
    _meanSquaredDisplacement.reallocate(
                         dsiOdfCartesianField.getSizeX(),
                         dsiOdfCartesianField.getSizeY(),
                         dsiOdfCartesianField.getSizeZ() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionX",
                                    dsiOdfCartesianField.getResolutionX() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionY",
                                    dsiOdfCartesianField.getResolutionY() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionZ",
                                    dsiOdfCartesianField.getResolutionZ() );
    _meanSquaredDisplacement.fill( 0.0 );

    gkg::Matrix shMatrixPlus;
    _shMatrixPlus = shMatrixPlus;

    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
						  dsiOdfCartesianField.getSizeX(),
						  dsiOdfCartesianField.getSizeY(),
						  dsiOdfCartesianField.getSizeZ(),
						  _shMatrixPlus.getSize1() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dsiOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dsiOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dsiOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );


  }
  GKG_CATCH( "gkg::DsiOdfCartesianFieldFunctor::"
             "DsiOdfCartesianFieldFunctor( "
             "const gkg::DsiField& dsiOdfField )" );


}

gkg::DsiOdfCartesianFieldFunctor::~DsiOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::DsiOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::DsiOdf* dsiOdf =
      dynamic_cast< gkg::DsiOdf* >( &odf );

    if ( dsiOdf )
    {

      _returnToOriginProbability( site ) = dsiOdf->getAverageNormalizedDwSignal();
      _meanSquaredDisplacement( site ) = dsiOdf->getMeanSquaredDisplacement();

    }

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::DsiOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}



const gkg::Volume< float >&
gkg::DsiOdfCartesianFieldFunctor::getReturnToOriginProbability() const
{

  try
  {

    return _returnToOriginProbability;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DsiOdfCartesianFieldFunctor::"
             "getAverageNormalizedDwSignal() const" );

}


const gkg::Volume< float >&
gkg::DsiOdfCartesianFieldFunctor::getMeanSquaredDisplacement() const
{

  try
  {

    return _meanSquaredDisplacement;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DsiOdfCartesianFieldFunctor::"
             "getMeanSquaredDisplacement() const" );

}


const gkg::Volume< float >&
gkg::DsiOdfCartesianFieldFunctor::getOdfSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DsiOdfCartesianFieldFunctor::"
             "getOdfSphericalHarmonicsCoefficients() const" );

}

