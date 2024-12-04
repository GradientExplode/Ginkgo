#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-dmri-pdf-dpi/DpiEap.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::DpiEapCartesianFieldFunctor::DpiEapCartesianFieldFunctor(
                 const gkg::DpiEapCartesianField& dpiEapCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
			 dpiEapCartesianField.getSizeX(),
                         dpiEapCartesianField.getSizeY(),
                         dpiEapCartesianField.getSizeZ(),
                         dpiEapCartesianField.getDataSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dpiEapCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dpiEapCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dpiEapCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // EAP spherical harmonics
    _eapSphericalHarmonicsCoefficients.reallocate(
                         dpiEapCartesianField.getSizeX(),
                         dpiEapCartesianField.getSizeY(),
                         dpiEapCartesianField.getSizeZ(),
                         dpiEapCartesianField.getEapSphericalHarmonicCount() );
    _eapSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionX",
                                    dpiEapCartesianField.getResolutionX() );
    _eapSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionY",
                                    dpiEapCartesianField.getResolutionY() );
    _eapSphericalHarmonicsCoefficients.getHeader().addAttribute(
                                    "resolutionZ",
                                    dpiEapCartesianField.getResolutionZ() );
    _eapSphericalHarmonicsCoefficients.fill( 0.0 );

    // Average Normalized DW Signal
    _averageNormalizedDwSignal.reallocate(
                         dpiEapCartesianField.getSizeX(),
                         dpiEapCartesianField.getSizeY(),
                         dpiEapCartesianField.getSizeZ() );
    _averageNormalizedDwSignal.getHeader().addAttribute(
                                    "resolutionX",
                                    dpiEapCartesianField.getResolutionX() );
    _averageNormalizedDwSignal.getHeader().addAttribute(
                                    "resolutionY",
                                    dpiEapCartesianField.getResolutionY() );
    _averageNormalizedDwSignal.getHeader().addAttribute(
                                    "resolutionZ",
                                    dpiEapCartesianField.getResolutionZ() );
    _averageNormalizedDwSignal.fill( 0.0 );

    // Analytical EAP @ zero := Return to original probabililty
    _analyticalEapAtZero.reallocate(
                         dpiEapCartesianField.getSizeX(),
                         dpiEapCartesianField.getSizeY(),
                         dpiEapCartesianField.getSizeZ() );
    _analyticalEapAtZero.getHeader().addAttribute(
                                    "resolutionX",
                                    dpiEapCartesianField.getResolutionX() );
    _analyticalEapAtZero.getHeader().addAttribute(
                                    "resolutionY",
                                    dpiEapCartesianField.getResolutionY() );
    _analyticalEapAtZero.getHeader().addAttribute(
                                    "resolutionZ",
                                    dpiEapCartesianField.getResolutionZ() );
    _analyticalEapAtZero.fill( 0.0 );

    // Analytical MSD (mean squared displacement)
    _meanSquaredDisplacement.reallocate(
                         dpiEapCartesianField.getSizeX(),
                         dpiEapCartesianField.getSizeY(),
                         dpiEapCartesianField.getSizeZ() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionX",
                                    dpiEapCartesianField.getResolutionX() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionY",
                                    dpiEapCartesianField.getResolutionY() );
    _meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionZ",
                                    dpiEapCartesianField.getResolutionZ() );
    _meanSquaredDisplacement.fill( 0.0 );


  }
  GKG_CATCH( "gkg::DpiEapCartesianFieldFunctor::"
             "DpiEapCartesianFieldFunctor( "
             "const gkg::DpiField& dpiEapField )" );


}

gkg::DpiEapCartesianFieldFunctor::~DpiEapCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::DpiEapCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& eap )
{

  try
  {

    gkg::DpiEap* dpiEap =
      dynamic_cast< gkg::DpiEap* >( &eap );

    if ( dpiEap )
    {

      int32_t s;
      int32_t sizeDataSH = _dwSphericalHarmonicsCoefficients.getSizeT();
      int32_t sizeEapSH =  _eapSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSHCoefficients =
        dpiEap->getDwSphericalHarmonicsCoefficients();
      std::vector< float > eapSHCoefficients = dpiEap->getValues();
      
      _averageNormalizedDwSignal( site ) = dpiEap->getAverageNormalizedDwSignal();
      _analyticalEapAtZero( site )       = dpiEap->getAnalyticalEapAtZero();
      _meanSquaredDisplacement( site )   = 
	                           dpiEap->getAnalyticalMeanSquaredDisplacement();
    
      for ( s = 0; s < sizeDataSH; s++ )
      {

        _dwSphericalHarmonicsCoefficients( site, s ) 
	                                          = ( float )dwSHCoefficients( s );
      
      }

      for ( s = 0; s < sizeEapSH; s++ )
      {
      
	_eapSphericalHarmonicsCoefficients( site, s ) = eapSHCoefficients[ s ];

      }

    }
    return eap;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::DpiEapCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& eap )" );

}


const gkg::Volume< float >&
gkg::DpiEapCartesianFieldFunctor::getDwSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DpiEapCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::DpiEapCartesianFieldFunctor::getEapSphericalHarmonicsCoefficients()
                                                                           const
{

  try
  {

    return _eapSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DpiEapCartesianFieldFunctor::"
             "getEapSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::DpiEapCartesianFieldFunctor::getAverageNormalizedDwSignal()
                                                                           const
{

  try
  {

    return _averageNormalizedDwSignal;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DpiEapCartesianFieldFunctor::"
             "getAverageNormalizedDwSignal() const" );

}


const gkg::Volume< float >&
gkg::DpiEapCartesianFieldFunctor::getAnalyticalEapAtZero()
                                                                           const
{

  try
  {

    return _analyticalEapAtZero;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DpiEapCartesianFieldFunctor::"
             "getAverageNormalizedDwSignal() const" );

}

const gkg::Volume< float >&
gkg::DpiEapCartesianFieldFunctor::getMeanSquaredDisplacement()
                                                                           const
{

  try
  {

    return _meanSquaredDisplacement;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::DpiEapCartesianFieldFunctor::"
             "getAverageNormalizedDwSignal() const" );

}
