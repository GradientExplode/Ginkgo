#include <gkg-dmri-odf-sdt/SDTFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sdt/SharpeningDeconvolutionTransformFiberOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::SDTFiberOdfCartesianFieldFunctor::SDTFiberOdfCartesianFieldFunctor(
	      const gkg::SDTFiberOdfCartesianField& sdtFiberOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
                       sdtFiberOdfCartesianField.getSizeX(),
                       sdtFiberOdfCartesianField.getSizeY(),
                       sdtFiberOdfCartesianField.getSizeZ(),
                       sdtFiberOdfCartesianField.getSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionX",
                       sdtFiberOdfCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionY",
                       sdtFiberOdfCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionZ",
                       sdtFiberOdfCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
                       sdtFiberOdfCartesianField.getSizeX(),
                       sdtFiberOdfCartesianField.getSizeY(),
                       sdtFiberOdfCartesianField.getSizeZ(),
                       sdtFiberOdfCartesianField.getSphericalHarmonicCount() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionX",
                       sdtFiberOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionY",
                       sdtFiberOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                       "resolutionZ",
                       sdtFiberOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );

  }
  GKG_CATCH( "gkg::SDTFiberOdfCartesianFieldFunctor::"
             "SDTFiberOdfCartesianFieldFunctor( "
             "const gkg::SDTFiberField& sdtFiberField )" );


}



gkg::SDTFiberOdfCartesianFieldFunctor::~SDTFiberOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::SDTFiberOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::SharpeningDeconvolutionTransformFiberOdf* sdtFiberOdf =
      dynamic_cast< gkg::SharpeningDeconvolutionTransformFiberOdf* >( &odf );

    if ( sdtFiberOdf )
    {

      int32_t s;
      int32_t sizeSDT = _dwSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSDTCoefficients =
                            sdtFiberOdf->getDwSphericalHarmonicsCoefficients();

      std::vector< float > odfSDTCoefficients = sdtFiberOdf->getValues();

      for ( s = 0; s < sizeSDT; s++ )
      {

        _dwSphericalHarmonicsCoefficients( site, s ) =
          ( float )dwSDTCoefficients( s );
        _odfSphericalHarmonicsCoefficients( site, s ) =
          odfSDTCoefficients[ s ];

      }

    }

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::SDTFiberOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::SDTFiberOdfCartesianFieldFunctor::getDwSphericalHarmonicsCoefficients()
                                                                          const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SDTFiberOdfCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::SDTFiberOdfCartesianFieldFunctor::getOdfSphericalHarmonicsCoefficients()
                                                                          const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SDTFiberOdfCartesianFieldFunctor::"
             "getOdfSphericalHarmonicsCoefficients() const" );

}

