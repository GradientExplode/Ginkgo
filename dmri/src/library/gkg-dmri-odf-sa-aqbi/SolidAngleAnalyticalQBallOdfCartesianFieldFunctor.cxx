#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sa-aqbi/SolidAngleAnalyticalQBallOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::
                              SolidAngleAnalyticalQBallOdfCartesianFieldFunctor(
         const gkg::SolidAngleAnalyticalQBallOdfCartesianField& 
	                            solidAngleAnalyticalQBallOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
       solidAngleAnalyticalQBallOdfCartesianField.getSizeX(),
       solidAngleAnalyticalQBallOdfCartesianField.getSizeY(),
       solidAngleAnalyticalQBallOdfCartesianField.getSizeZ(),
       solidAngleAnalyticalQBallOdfCartesianField.getSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionX",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionY",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionZ",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
       solidAngleAnalyticalQBallOdfCartesianField.getSizeX(),
       solidAngleAnalyticalQBallOdfCartesianField.getSizeY(),
       solidAngleAnalyticalQBallOdfCartesianField.getSizeZ(),
       solidAngleAnalyticalQBallOdfCartesianField.getSphericalHarmonicCount() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionX",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionY",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
       "resolutionZ",
       solidAngleAnalyticalQBallOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );


  }
  GKG_CATCH( "gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::"
             "SolidAngleAnalyticalQBallOdfCartesianFieldFunctor( "
             "const gkg::SolidAngleAnalyticalQBallField& "
	     "solidAngleAnalyticalQBallField )" );


}

gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::
                            ~SolidAngleAnalyticalQBallOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::SolidAngleAnalyticalQBallOdf* solidAngleAnalyticalQBallOdf =
      dynamic_cast< gkg::SolidAngleAnalyticalQBallOdf* >( &odf );

    if ( solidAngleAnalyticalQBallOdf )
    {

      int32_t s;
      int32_t sizeSH = _dwSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSHCoefficients =
        solidAngleAnalyticalQBallOdf->getDwSphericalHarmonicsCoefficients();
      std::vector< float > odfSHCoefficients =
        solidAngleAnalyticalQBallOdf->getValues();

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
             "gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::
                                     getDwSphericalHarmonicsCoefficients() const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::
                                    getOdfSphericalHarmonicsCoefficients() const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SolidAngleAnalyticalQBallOdfCartesianFieldFunctor::"
             "getOdfSphericalHarmonicsCoefficients() const" );

}
