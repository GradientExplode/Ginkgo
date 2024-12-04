#include <gkg-dmri-odf-sd/SDFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-sd/SphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::SDFiberOdfCartesianFieldFunctor::SDFiberOdfCartesianFieldFunctor(
                 const gkg::SDFiberOdfCartesianField& sdFiberOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // DW spherical harmonics
    _dwSphericalHarmonicsCoefficients.reallocate(
                        sdFiberOdfCartesianField.getSizeX(),
                        sdFiberOdfCartesianField.getSizeY(),
                        sdFiberOdfCartesianField.getSizeZ(),
                        sdFiberOdfCartesianField.getSphericalHarmonicCount() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionX",
                        sdFiberOdfCartesianField.getResolutionX() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionY",
                        sdFiberOdfCartesianField.getResolutionY() );
    _dwSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionZ",
                        sdFiberOdfCartesianField.getResolutionZ() );
    _dwSphericalHarmonicsCoefficients.fill( 0.0 );

    // ODF spherical harmonics
    _odfSphericalHarmonicsCoefficients.reallocate(
                        sdFiberOdfCartesianField.getSizeX(),
                        sdFiberOdfCartesianField.getSizeY(),
                        sdFiberOdfCartesianField.getSizeZ(),
                        sdFiberOdfCartesianField.getSphericalHarmonicCount() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionX",
                        sdFiberOdfCartesianField.getResolutionX() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionY",
                        sdFiberOdfCartesianField.getResolutionY() );
    _odfSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionZ",
                        sdFiberOdfCartesianField.getResolutionZ() );
    _odfSphericalHarmonicsCoefficients.fill( 0.0 );

  }
  GKG_CATCH( "gkg::SDFiberOdfCartesianFieldFunctor::"
             "SDFiberOdfCartesianFieldFunctor( "
             "const gkg::SDFiberField& shFiberField )" );


}



gkg::SDFiberOdfCartesianFieldFunctor::~SDFiberOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::SDFiberOdfCartesianFieldFunctor::apply(
                                    const gkg::Vector3d< int32_t >& site,
                                    gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::SphericalDeconvolutionFiberOdf* sdFiberOdf =
      dynamic_cast< gkg::SphericalDeconvolutionFiberOdf* >( &odf );

    if ( sdFiberOdf )
    {

      int32_t s;
      int32_t sizeSD = _dwSphericalHarmonicsCoefficients.getSizeT();

      const gkg::Vector& dwSDCoefficients =
        sdFiberOdf->getDwSphericalHarmonicsCoefficients();
      std::vector< float > odfSDCoefficients =
        sdFiberOdf->getValues();

      for ( s = 0; s < sizeSD; s++ )
      {

        if ( dwSDCoefficients( s ) < 0.0 ) 
        {

          _dwSphericalHarmonicsCoefficients( site, s ) = 0.0;

        }
        else
        {

          _dwSphericalHarmonicsCoefficients( site, s ) =
            ( float )dwSDCoefficients( s );

        }
        _odfSphericalHarmonicsCoefficients( site, s ) = odfSDCoefficients[ s ];

      }

    }

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::SDFiberOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::SDFiberOdfCartesianFieldFunctor::getDwSphericalHarmonicsCoefficients()
                                                                          const
{

  try
  {

    return _dwSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SDFiberOdfCartesianFieldFunctor::"
             "getDwSphericalHarmonicsCoefficients() const" );

}


const gkg::Volume< float >&
gkg::SDFiberOdfCartesianFieldFunctor::getOdfSphericalHarmonicsCoefficients()
                                                                          const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::SDFiberOdfCartesianFieldFunctor::"
             "getOdfSphericalHarmonicsCoefficients() const" );

}

