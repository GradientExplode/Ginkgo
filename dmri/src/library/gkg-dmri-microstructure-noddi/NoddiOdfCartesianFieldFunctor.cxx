#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdf.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::NoddiOdfCartesianFieldFunctor::NoddiOdfCartesianFieldFunctor(
         const gkg::NoddiOdfCartesianField& noddiOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // getting sphere distribution function type (Watson, Bingham, ...)
    _sphereDistributionFunction = 
                         noddiOdfCartesianField.getSphereDistributionFunction();

    // NODDI coefficients
    _noddiCoefficients.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ(),
                 noddiOdfCartesianField.getInitialParameters().getSize() );
    _noddiCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _noddiCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _noddiCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _noddiCoefficients.fill( 0.0 );

    // isotropic fraction
    _isotropicFraction.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _isotropicFraction.fill( 0.0 );

    // intracellular fraction
    _intracellularFraction.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _intracellularFraction.fill( 0.0 );

    // stationary fraction
    _stationaryFraction.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _stationaryFraction.fill( 0.0 );

    // kappa
    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {
 
      _kappa.reallocate(
                noddiOdfCartesianField.getSizeX(),
                noddiOdfCartesianField.getSizeY(),
                noddiOdfCartesianField.getSizeZ(),
                1 );

    }
    else if ( _sphereDistributionFunction ==
              gkg::NoddiOdfCartesianField::Bingham )
    {
 
      _kappa.reallocate(
                noddiOdfCartesianField.getSizeX(),
                noddiOdfCartesianField.getSizeY(),
                noddiOdfCartesianField.getSizeZ(),
                2 );

    }
    _kappa.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _kappa.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _kappa.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _kappa.fill( 0.0 );

    // orientation dispersion
    _orientationDispersion.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _orientationDispersion.fill( 0.0 );

    // parallel diffusivity
    _parallelDiffusivity.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _parallelDiffusivity.fill( 0.0 );

    // isotropic diffusivity
    _isotropicDiffusivity.reallocate(
                 noddiOdfCartesianField.getSizeX(),
                 noddiOdfCartesianField.getSizeY(),
                 noddiOdfCartesianField.getSizeZ() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            noddiOdfCartesianField.getResolutionX() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            noddiOdfCartesianField.getResolutionY() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            noddiOdfCartesianField.getResolutionZ() );
    _isotropicDiffusivity.fill( 0.0 );


  }
  GKG_CATCH( "gkg::NoddiOdfCartesianFieldFunctor::"
             "NoddiOdfCartesianFieldFunctor( "
             "const gkg::AnalyticalQBallField& analyticalQBallField )" );


}

gkg::NoddiOdfCartesianFieldFunctor::~NoddiOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::NoddiOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    int32_t coefficientCount = odf.getValueCount();
    int32_t c = 0;
    for ( c = 0; c < coefficientCount; c++ )
    {

      _noddiCoefficients( site, c ) = odf.getValue( c );

    } 

    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {
 
      _isotropicFraction( site ) = odf.getValue( 0 );
      _stationaryFraction( site ) = odf.getValue( 8 ) *
                                                   ( 1.0 - odf.getValue( 0 ) ) ;
      _intracellularFraction( site ) = odf.getValue( 1 ) *
             ( 1.0 - odf.getValue( 0 ) ) * ( 1.0 - odf.getValue( 8 ) );
      _kappa( site ) = odf.getValue( 2 );
      _orientationDispersion( site ) = ( 2.0 / M_PI ) *
                                       std::atan( 1.0 / odf.getValue( 2 ) );
      _parallelDiffusivity( site ) = odf.getValue( 6 );
      _isotropicDiffusivity( site ) = odf.getValue( 7 );


    }
    else if ( _sphereDistributionFunction ==
              gkg::NoddiOdfCartesianField::Bingham )
    {
 
      _isotropicFraction( site ) = odf.getValue( 0 );
      _stationaryFraction( site ) = odf.getValue( 8 ) *
                                                   ( 1.0 - odf.getValue( 0 ) ) ;
      _intracellularFraction( site ) = odf.getValue( 1 ) *
             ( 1.0 - odf.getValue( 0 ) ) * ( 1.0 - odf.getValue( 8 ) );
      _kappa( site, 0 ) = odf.getValue( 2 );
      _kappa( site, 1 ) = odf.getValue( 3 );

      // to be updated with the good formula
      _orientationDispersion( site ) = ( 2.0 / M_PI ) *
                                       std::atan( 1.0 / odf.getValue( 2 ) );


      _parallelDiffusivity( site ) = odf.getValue( 10 );
      _isotropicDiffusivity( site ) = odf.getValue( 11 );


    }

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::NoddiOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getNoddiCoefficients() const
{

  try
  {

    return _noddiCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getNoddiCoefficients() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getIsotropicFraction() const
{

  try
  {

    return _isotropicFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getIsotropicFraction() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getIntracellularFraction() const
{

  try
  {

    return _intracellularFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getIntracellularFraction() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getKappa() const
{

  try
  {

    return _kappa;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getKappa() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getOrientationDispersion() const
{

  try
  {

    return _orientationDispersion;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getOrientationDispersion() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getParallelDiffusivity() const
{

  try
  {

    return _parallelDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getParallelDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getIsotropicDiffusivity() const
{

  try
  {

    return _isotropicDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getIsotropicDiffusivity() const" );

}

const gkg::Volume< float >&
gkg::NoddiOdfCartesianFieldFunctor::getStationaryFraction() const
{

  try
  {

    return _stationaryFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::NoddiOdfCartesianFieldFunctor::"
             "getStationaryFraction() const" );

}


