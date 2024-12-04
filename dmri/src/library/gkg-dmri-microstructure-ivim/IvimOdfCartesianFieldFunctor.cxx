#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-ivim/IvimOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::IvimOdfCartesianFieldFunctor::IvimOdfCartesianFieldFunctor(
         const gkg::IvimOdfCartesianField& ivimOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // IVIM coefficients
    _ivimCoefficients.reallocate( ivimOdfCartesianField.getSizeX(),
                                  ivimOdfCartesianField.getSizeY(),
                                  ivimOdfCartesianField.getSizeZ(),
                                  9U );
    _ivimCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _ivimCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _ivimCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _ivimCoefficients.fill( 0.0 );

    // IVIM fraction
    _ivimFraction.reallocate( ivimOdfCartesianField.getSizeX(),
                              ivimOdfCartesianField.getSizeY(),
                              ivimOdfCartesianField.getSizeZ() );
    _ivimFraction.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _ivimFraction.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _ivimFraction.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _ivimFraction.fill( 0.0 );

    // IVIM diffusivity
    _ivimDiffusivity.reallocate( ivimOdfCartesianField.getSizeX(),
                                 ivimOdfCartesianField.getSizeY(),
                                 ivimOdfCartesianField.getSizeZ() );
    _ivimDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _ivimDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _ivimDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _ivimDiffusivity.fill( 0.0 );

    // parenchyma diffusivity
    _parenchymaDiffusivity.reallocate( ivimOdfCartesianField.getSizeX(),
                                       ivimOdfCartesianField.getSizeY(),
                                       ivimOdfCartesianField.getSizeZ() );
    _parenchymaDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _parenchymaDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _parenchymaDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _parenchymaDiffusivity.fill( 0.0 );

    // parenchyma fractional anisotropy
    _parenchymaFractionalAnisotropy.reallocate(
                                       ivimOdfCartesianField.getSizeX(),
                                       ivimOdfCartesianField.getSizeY(),
                                       ivimOdfCartesianField.getSizeZ() );
    _parenchymaFractionalAnisotropy.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _parenchymaFractionalAnisotropy.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _parenchymaFractionalAnisotropy.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _parenchymaFractionalAnisotropy.fill( 0.0 );

    // ivim fitted volume
    _ivimFittedVolume.reallocate( ivimOdfCartesianField.getSizeX(),
                                  ivimOdfCartesianField.getSizeY(),
                                  ivimOdfCartesianField.getSizeZ(),
                                  9U ); //shell count
    _ivimFittedVolume.getHeader().addAttribute(
                            "resolutionX",
                            ivimOdfCartesianField.getResolutionX() );
    _ivimFittedVolume.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _ivimFittedVolume.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _ivimFittedVolume.fill( 0.0 );

    // ivim powder average
    _ivimPowderAverage.reallocate( ivimOdfCartesianField.getSizeX(),
                                   ivimOdfCartesianField.getSizeY(),
                                   ivimOdfCartesianField.getSizeZ(),
                                   9U ); //shell count
    _ivimPowderAverage.setResolution( ivimOdfCartesianField.getResolution() );
    _ivimPowderAverage.getHeader().addAttribute(
                            "resolutionY",
                            ivimOdfCartesianField.getResolutionY() );
    _ivimPowderAverage.getHeader().addAttribute(
                            "resolutionZ",
                            ivimOdfCartesianField.getResolutionZ() );
    _ivimPowderAverage.fill( 0.0 );

  }
  GKG_CATCH( "gkg::IvimOdfCartesianFieldFunctor::"
             "IvimOdfCartesianFieldFunctor( "
             "const gkg::AnalyticalQBallField& analyticalQBallField )" );


}

gkg::IvimOdfCartesianFieldFunctor::~IvimOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::IvimOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    int32_t coefficientCount = odf.getValueCount();
    int32_t c = 0;
    for ( c = 0; c < coefficientCount; c++ )
    {

      _ivimCoefficients( site, c ) = odf.getValue( c );

    } 

    _ivimFraction( site ) = odf.getValue( 0 );
    _ivimDiffusivity( site ) = odf.getValue( 1 );
    _parenchymaDiffusivity( site ) = odf.getValue( 2 );
    _parenchymaFractionalAnisotropy( site ) = static_cast< gkg::IvimOdf* >(
                        &odf )->getParenchymaTensor().getFractionalAnisotropy();

    // ivim fitted volume
    gkg::Vector fittedMeasurementsVector = static_cast< gkg::IvimOdf* >( 
                                                &odf )->getFittedMeasurements();
    int32_t shellCount = fittedMeasurementsVector.getSize();
    int32_t s = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      _ivimFittedVolume( site, s ) = fittedMeasurementsVector( s );
      _ivimPowderAverage( site, s ) = static_cast< gkg::IvimOdf* >(
                                           &odf )->getPowderAverage()[ s ];
    } 

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::IvimOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getIvimCoefficients() const
{

  try
  {

    return _ivimCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getIvimCoefficients() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getIvimFraction() const
{

  try
  {

    return _ivimFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getIvimFraction() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getIvimDiffusivity() const
{

  try
  {

    return _ivimDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getIvimDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getParenchymaDiffusivity() const
{

  try
  {

    return _parenchymaDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getParenchymaDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getParenchymaFractionalAnisotropy() const
{

  try
  {

    return _parenchymaFractionalAnisotropy;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getParenchymaFractionalAnisotropy() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getIvimFittedVolume() const
{

  try
  {

    return _ivimFittedVolume;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getIvimFittedVolume() const" );

}


const gkg::Volume< float >&
gkg::IvimOdfCartesianFieldFunctor::getIvimPowderAverage() const
{

  try
  {

    return _ivimPowderAverage;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::IvimOdfCartesianFieldFunctor::"
             "getIvimFittedVolume() const" );

}
