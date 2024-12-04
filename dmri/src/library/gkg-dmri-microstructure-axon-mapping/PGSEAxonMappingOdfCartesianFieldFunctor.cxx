#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdf.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEDefines.h>
#include <gkg-dmri-container/DiffusionCartesianFieldFunctor_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/CartesianField_i.h>


gkg::PGSEAxonMappingOdfCartesianFieldFunctor::
                                        PGSEAxonMappingOdfCartesianFieldFunctor(
         const gkg::AxonMappingOdfCartesianField& axonMappingOdfCartesianField )
 : gkg::DiffusionCartesianFieldFunctor< gkg::OrientationDistributionFunction >()
{

  try
  {

    // PGSE axon mapping coefficients
    _axonMappingCoefficients.reallocate(
                axonMappingOdfCartesianField.getSizeX(),
                axonMappingOdfCartesianField.getSizeY(),
                axonMappingOdfCartesianField.getSizeZ(),
                axonMappingOdfCartesianField.getInitialParameters().getSize() );
    _axonMappingCoefficients.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _axonMappingCoefficients.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _axonMappingCoefficients.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _axonMappingCoefficients.fill( 0.0 );

    // isotropic fraction
    _isotropicFraction.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _isotropicFraction.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _isotropicFraction.fill( 0.0 );

    // isotropic diffusivity
    _isotropicDiffusivity.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _isotropicDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _isotropicDiffusivity.fill( 0.0 );

    // extra perpendicular diffusivity
    _extraPerpendicularDiffusivity.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _extraPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _extraPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _extraPerpendicularDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _extraPerpendicularDiffusivity.fill( 0.0 );

    // stationary fraction
    _stationaryFraction.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _stationaryFraction.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _stationaryFraction.fill( 0.0 );

    // intracellular fraction
    _intracellularFraction.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _intracellularFraction.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _intracellularFraction.fill( 0.0 );

    // kappa
    _kappa.reallocate(
                axonMappingOdfCartesianField.getSizeX(),
                axonMappingOdfCartesianField.getSizeY(),
                axonMappingOdfCartesianField.getSizeZ() );
    _kappa.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _kappa.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _kappa.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _kappa.fill( 0.0 );

    // orientation dispersion
    _orientationDispersion.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _orientationDispersion.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _orientationDispersion.fill( 0.0 );

    // parallel diffusivity
    _parallelDiffusivity.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _parallelDiffusivity.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _parallelDiffusivity.fill( 0.0 );

    // axon diameter
    _axonDiameter.reallocate(
                 axonMappingOdfCartesianField.getSizeX(),
                 axonMappingOdfCartesianField.getSizeY(),
                 axonMappingOdfCartesianField.getSizeZ() );
    _axonDiameter.getHeader().addAttribute(
                            "resolutionX",
                            axonMappingOdfCartesianField.getResolutionX() );
    _axonDiameter.getHeader().addAttribute(
                            "resolutionY",
                            axonMappingOdfCartesianField.getResolutionY() );
    _axonDiameter.getHeader().addAttribute(
                            "resolutionZ",
                            axonMappingOdfCartesianField.getResolutionZ() );
    _axonDiameter.fill( 0.0 );


  }
  GKG_CATCH( "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "PGSEAxonMappingOdfCartesianFieldFunctor( "
             "const gkg::AnalyticalQBallField& analyticalQBallField )" );


}

gkg::PGSEAxonMappingOdfCartesianFieldFunctor::
                                      ~PGSEAxonMappingOdfCartesianFieldFunctor()
{
}


gkg::OrientationDistributionFunction&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::apply(
                                     const gkg::Vector3d< int32_t >& site,
                                     gkg::OrientationDistributionFunction& odf )
{

  try
  {

    int32_t coefficientCount = odf.getValueCount();
    int32_t c = 0;
    for ( c = 0; c < coefficientCount; c++ )
    {

      _axonMappingCoefficients( site, c ) = odf.getValue( c );

    } 

    _isotropicFraction( site ) = odf.getValue( PGSE_IsotropicFraction );
    _isotropicDiffusivity( site ) = odf.getValue( PGSE_IsotropicDiffusivity );
    _extraPerpendicularDiffusivity( site ) = odf.getValue(
                                           PGSE_ExtraPerpendicularDiffusivity );
    _stationaryFraction( site ) = odf.getValue( PGSE_StationaryFraction ) *
             ( 1.0 - odf.getValue( PGSE_IsotropicFraction ) );

    _intracellularFraction( site ) = 
                             odf.getValue( PGSE_IntracellularFraction ) *
                             ( 1.0 - odf.getValue( PGSE_IsotropicFraction ) ) *
                             ( 1.0 - odf.getValue( PGSE_StationaryFraction ) );
    _kappa( site ) = odf.getValue( PGSE_Kappa );
    _orientationDispersion( site ) = ( 2.0 / M_PI ) *
                                     std::atan( 1.0 /
                                                odf.getValue( PGSE_Kappa ) );
    _parallelDiffusivity( site ) = odf.getValue( PGSE_ParallelDiffusivity );
    _axonDiameter( site ) = odf.getValue( PGSE_AxonDiameter );

    std::cout << " _isotropicFraction( site ): " 
              << _isotropicFraction( site ) << std::endl;
    std::cout << " _intracellularFraction( site ) : " 
              << _intracellularFraction( site ) << std::endl;
    std::cout << " _kappa( site ) : " << _kappa( site ) << std::endl;
    std::cout << " _orientationDispersion( site ) : " 
              << _orientationDispersion( site ) << std::endl;
    std::cout << " _parallelDiffusivity( site ) : "
              << _parallelDiffusivity( site ) << std::endl;
    std::cout << " _isotropicDiffusivity( site ) : " 
              << _isotropicDiffusivity( site ) << std::endl;
    std::cout << " _axonDiameter( site ) : " << _axonDiameter( site ) 
              << std::endl;

    return odf;

  }
  GKG_CATCH( "gkg:OrientationDistributionFunction& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::apply( "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg:OrientationDistributionFunction& odf )" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getAxonMappingCoefficients() const
{

  try
  {

    return _axonMappingCoefficients;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getAxonMappingCoefficients() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getIsotropicFraction() const
{

  try
  {

    return _isotropicFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getIsotropicFraction() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getIsotropicDiffusivity() const
{

  try
  {

    return _isotropicDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getIsotropicDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::
                                        getExtraPerpendicularDiffusivity() const
{

  try
  {

    return _extraPerpendicularDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getExtraPerpendicularDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getStationaryFraction() const
{

  try
  {

    return _stationaryFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getStationaryFraction() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getIntracellularFraction() const
{

  try
  {

    return _intracellularFraction;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getIntracellularFraction() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getKappa() const
{

  try
  {

    return _kappa;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getKappa() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getOrientationDispersion() const
{

  try
  {

    return _orientationDispersion;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getOrientationDispersion() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getParallelDiffusivity() const
{

  try
  {

    return _parallelDiffusivity;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getParallelDiffusivity() const" );

}


const gkg::Volume< float >&
gkg::PGSEAxonMappingOdfCartesianFieldFunctor::getAxonDiameter() const
{

  try
  {

    return _axonDiameter;

  }
  GKG_CATCH( "const gkg::Volume< float >& "
             "gkg::PGSEAxonMappingOdfCartesianFieldFunctor::"
             "getAxonDiameter() const" );

}


