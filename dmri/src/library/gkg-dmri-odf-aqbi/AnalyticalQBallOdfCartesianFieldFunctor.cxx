#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::AnalyticalQBallOdfCartesianFieldFunctor::
                                        AnalyticalQBallOdfCartesianFieldFunctor(
         const gkg::AnalyticalQBallOdfCartesianField& analyticalQBallOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
                 analyticalQBallOdfCartesianField.getSizeX(),
                 analyticalQBallOdfCartesianField.getSizeY(),
                 analyticalQBallOdfCartesianField.getSizeZ(),
                 analyticalQBallOdfCartesianField.getSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            analyticalQBallOdfCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            analyticalQBallOdfCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            analyticalQBallOdfCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
                 analyticalQBallOdfCartesianField.getSizeX(),
                 analyticalQBallOdfCartesianField.getSizeY(),
                 analyticalQBallOdfCartesianField.getSizeZ(),
                 analyticalQBallOdfCartesianField.getSphericalHarmonicCount() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            analyticalQBallOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            analyticalQBallOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            analyticalQBallOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );


  }
  GKG_CATCH( "gkg::AnalyticalQBallOdfCartesianFieldFunctor::"
             "AnalyticalQBallOdfCartesianFieldFunctor( "
             "const gkg::AnalyticalQBallField& analyticalQBallField )" );


}

gkg::AnalyticalQBallOdfCartesianFieldFunctor::
                                      ~AnalyticalQBallOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::AnalyticalQBallOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::AnalyticalQBallOdf* analyticalQBallOdf =
      dynamic_cast< gkg::AnalyticalQBallOdf* >( &odf );

    if ( analyticalQBallOdf )
    {

      int32_t s;
      int32_t sizeSH = _dwSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSHCoefficients =
        analyticalQBallOdf->getDwSphericalHarmonicsCoefficients();
      std::vector< float > odfSHCoefficients =
        analyticalQBallOdf->getValues();

      for ( s = 0; s < sizeSH; s++ )
      {

        _dwSphericalHarmonicsCoefficients( site, s ) 
	  = ( float )dwSHCoefficients( s );
        _odfSphericalHarmonicsCoefficients( site, s ) = odfSHCoefficients[ s ];

      }

    }
    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::AnalyticalQBallOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::AnalyticalQBallOdfCartesianFieldFunctor::
                                     getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AnalyticalQBallOdfCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::AnalyticalQBallOdfCartesianFieldFunctor::
                                    getOdfSphericalHarmonicsCoefficients() const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AnalyticalQBallOdfCartesianFieldFunctor::"
             "getOdfSphericalHarmonicsCoefficients() const" );

}
