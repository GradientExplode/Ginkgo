#include <gkg-dmri-odf-msmt-csd/MSMTCSDFiberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-msmt-csd/MSMTConstrainedSphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::MSMTCSDFiberOdfCartesianFieldFunctor::MSMTCSDFiberOdfCartesianFieldFunctor(
       const gkg::MSMTCSDFiberOdfCartesianField& msmtCSDFiberOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // ODF spherical harmonics
    _odfAllSphericalHarmonicsCoefficients.reallocate(
      msmtCSDFiberOdfCartesianField.getSizeX(),
      msmtCSDFiberOdfCartesianField.getSizeY(),
      msmtCSDFiberOdfCartesianField.getSizeZ(),
      msmtCSDFiberOdfCartesianField.getAllSphericalHarmonicCoefficientCount() );
    _odfAllSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionX",
                        msmtCSDFiberOdfCartesianField.getResolutionX() );
    _odfAllSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionY",
                        msmtCSDFiberOdfCartesianField.getResolutionY() );
    _odfAllSphericalHarmonicsCoefficients.getHeader().addAttribute(
                        "resolutionZ",
                        msmtCSDFiberOdfCartesianField.getResolutionZ() );
    _odfAllSphericalHarmonicsCoefficients.fill( 0.0 );

  }
  GKG_CATCH( "gkg::MSMTCSDFiberOdfCartesianFieldFunctor::"
             "MSMTCSDFiberOdfCartesianFieldFunctor( "
             "const gkg::MSMTCSDFiberOdfCartesianField& "
             "msmtCSDFiberOdfCartesianField )" );


}



gkg::MSMTCSDFiberOdfCartesianFieldFunctor::
                                         ~MSMTCSDFiberOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::MSMTCSDFiberOdfCartesianFieldFunctor::apply(
                                    const gkg::Vector3d< int32_t >& site,
                                    gkg::OrientationDistributionFunction& odf )
{

  try
  {

    gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf* 
      msmtCsdFiberOdf =
      dynamic_cast< gkg::MSMTConstrainedSphericalDeconvolutionFiberOdf* >(
                                                                         &odf );

    if ( msmtCsdFiberOdf )
    {

      std::vector< float > odfAllMSMTCSDCoefficients =
        msmtCsdFiberOdf->getValues();

      int32_t sizeMSMTCSD = ( int32_t )odfAllMSMTCSDCoefficients.size();
      int32_t s = 0;
      for ( s = 0; s < sizeMSMTCSD; s++ )
      {

        _odfAllSphericalHarmonicsCoefficients( site, s ) =
                                                 odfAllMSMTCSDCoefficients[ s ];

      }

    }

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::MSMTCSDFiberOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::MSMTCSDFiberOdfCartesianFieldFunctor::
                                 getOdfAllSphericalHarmonicsCoefficients() const
{

  try
  {

    return _odfAllSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::MSMTCSDFiberOdfCartesianFieldFunctor::"
             "getOdfAllSphericalHarmonicsCoefficients() const" );

}

