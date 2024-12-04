#include <gkg-dmri-odf-dot/DotOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-dot/DotOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::DotOdfCartesianFieldFunctor::DotOdfCartesianFieldFunctor(
                 const gkg::DotOdfCartesianField& dotOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
			 dotOdfCartesianField.getSizeX(),
                         dotOdfCartesianField.getSizeY(),
                         dotOdfCartesianField.getSizeZ(),
                         dotOdfCartesianField.getSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dotOdfCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dotOdfCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dotOdfCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // ADC spherical harmonics
    _adcSphericalHarmonicsCoefficients.reallocate(
                         dotOdfCartesianField.getSizeX(),
                         dotOdfCartesianField.getSizeY(),
                         dotOdfCartesianField.getSizeZ(),
                         dotOdfCartesianField.getSphericalHarmonicCount() );
    _adcSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dotOdfCartesianField.getResolutionX() );
    _adcSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dotOdfCartesianField.getResolutionY() );
    _adcSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dotOdfCartesianField.getResolutionZ() );
    _adcSphericalHarmonicsCoefficients.fill( 0.0 );


    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
                         dotOdfCartesianField.getSizeX(),
                         dotOdfCartesianField.getSizeY(),
                         dotOdfCartesianField.getSizeZ(),
                         dotOdfCartesianField.getSphericalHarmonicCount() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dotOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dotOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dotOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );


  }
  GKG_CATCH( "gkg::DotOdfCartesianFieldFunctor::"
             "DotOdfCartesianFieldFunctor( "
             "const gkg::DotField& dotField )" );


}

gkg::DotOdfCartesianFieldFunctor::~DotOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::DotOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::DotOdf* dotOdf =
      dynamic_cast< gkg::DotOdf* >( &odf );

    if ( dotOdf )
    {

      int32_t s;
      int32_t sizeSH = _dwSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSHCoefficients =
                                    dotOdf->getDwSphericalHarmonicsCoefficients();

      const gkg::Vector& adcSHCoefficients = 
                                    dotOdf->getAdcSphericalHarmonicsCoefficients();

      std::vector< float > odfSHCoefficients = dotOdf->getValues();

      for ( s = 0; s < sizeSH; s++ )
      {

        _dwSphericalHarmonicsCoefficients( site, s ) 
	                                         = ( float )dwSHCoefficients( s );
	_adcSphericalHarmonicsCoefficients( site, s ) 
	                                         = ( float )adcSHCoefficients( s );
        _odfSphericalHarmonicsCoefficients( site, s ) 
	                                         = odfSHCoefficients[ s ];

      }

    }
    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::DotOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::DotOdfCartesianFieldFunctor::getDwSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DotOdfCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::DotOdfCartesianFieldFunctor::getOdfSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DotOdfCartesianFieldFunctor::"
             "OdfSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::DotOdfCartesianFieldFunctor::getAdcSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _adcSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DotOdfCartesianFieldFunctor::"
             "AdcSphericalHarmonicsCoefficients() const" );

}
