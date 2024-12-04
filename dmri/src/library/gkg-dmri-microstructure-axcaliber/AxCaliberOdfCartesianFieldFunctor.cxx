#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::AxCaliberOdfCartesianFieldFunctor::AxCaliberOdfCartesianFieldFunctor(
         const gkg::AxCaliberOdfCartesianField& axCaliberOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {


    // AxCaliber coefficients
    _axCaliberCoefficients.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ(),
                 axCaliberOdfCartesianField.getInitialParameters().getSize() );
    _axCaliberCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _axCaliberCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _axCaliberCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _axCaliberCoefficients.fill( 0.0 );

    // hindered diffusivity
    _hinderedDiffusivity.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _hinderedDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _hinderedDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _hinderedDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _hinderedDiffusivity.fill( 0.0 );

    // intracellular fraction
    _intracellularFraction.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _intracellularFraction.fill( 0.0 );

    // intracellular perpendicular diffusivity
    _intracellularPerpendicularDiffusivity.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _intracellularPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _intracellularPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _intracellularPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _intracellularPerpendicularDiffusivity.fill( 0.0 );

    // intracellular parallel diffusivity
    _intracellularParallelDiffusivity.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _intracellularParallelDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _intracellularParallelDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _intracellularParallelDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _intracellularParallelDiffusivity.fill( 0.0 );

    // diameter mean
    _diameterMean.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _diameterMean.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _diameterMean.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _diameterMean.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _diameterMean.fill( 0.0 );

    // diameter stddev
    _diameterStdDev.reallocate(
                 axCaliberOdfCartesianField.getSizeX(),
                 axCaliberOdfCartesianField.getSizeY(),
                 axCaliberOdfCartesianField.getSizeZ() );
    _diameterStdDev.getHeader().addAttribute(
                            "resolutionX",
                            axCaliberOdfCartesianField.getResolutionX() );
    _diameterStdDev.getHeader().addAttribute(
                            "resolutionY",
                            axCaliberOdfCartesianField.getResolutionY() );
    _diameterStdDev.getHeader().addAttribute(
                            "resolutionZ",
                            axCaliberOdfCartesianField.getResolutionZ() );
    _diameterStdDev.fill( 0.0 );

  }
  GKG_CATCH( "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "AxCaliberOdfCartesianFieldFunctor( "
             "const gkg::AxCaliberOdfCartesianField& "
             "axCaliberOdfCartesianField )" );


}

gkg::AxCaliberOdfCartesianFieldFunctor::~AxCaliberOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::AxCaliberOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    int32_t coefficientCount = odf.getValueCount();
    int32_t c = 0;
    for ( c = 0; c < coefficientCount; c++ )
    {

      _axCaliberCoefficients( site, c ) = odf.getValue( c );

    } 

    _hinderedDiffusivity( site ) = odf.getValue( 1 );
    _intracellularFraction( site ) = odf.getValue( 2 );
    _intracellularPerpendicularDiffusivity( site ) = odf.getValue( 3 );
    _intracellularParallelDiffusivity( site ) = odf.getValue( 4 );
    _diameterMean( site ) = odf.getValue( 5 );
    _diameterStdDev( site ) = odf.getValue( 6 );

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::getAxCaliberCoefficients() const
{

  try
  {

    return _axCaliberCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getAxCaliberCoefficients() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::getHinderedDiffusivity() const
{

  try
  {

    return _hinderedDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getHinderedDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::getIntracellularFraction() const
{

  try
  {

    return _intracellularFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getIntracellularFraction() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::
                                getIntracellularPerpendicularDiffusivity() const
{

  try
  {

    return _intracellularPerpendicularDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getIntracellularPerpendicularDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::
                                     getIntracellularParallelDiffusivity() const
{

  try
  {

    return _intracellularParallelDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getIntracellularParallelDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::getDiameterMean() const
{

  try
  {

    return _diameterMean;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getDiameterMean() const" );

}


const gkg::Volume< float >&
gkg::AxCaliberOdfCartesianFieldFunctor::getDiameterStdDev() const
{

  try
  {

    return _diameterStdDev;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::AxCaliberOdfCartesianFieldFunctor::"
             "getDiameterStdDev() const" );

}
