#include <gkg-mri-reconstruction-plugin-functors/CoilSensitivityMap/CoilSensitivityMapCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/ConditionalMask_i.h>
#include <gkg-processing-algobase/ConstantFilter_i.h>
#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-mri-reconstruction-parallel/B1ReferenceFunctor.h>
#include <gkg-mri-reconstruction-parallel/B1SensitivityFunctor.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-signal/Filter3d_i.h>
#include <gkg-processing-extrapolation/RecursiveExtrapolator3d_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <set>
#include <complex>
#include <fstream>


//
// std::string getExtraName()
//

static std::string getExtraName( const std::string& name,
                                 const std::string& extraName )
{

  try
  {

    std::set< std::string > extensions;
    extensions.insert( ".dim" );
    extensions.insert( ".ima" );
    extensions.insert( ".header" );

    std::set< std::string >::const_iterator e = extensions.begin(),
                                            ee = extensions.end();

    while ( e != ee )
    {

      if ( name.length() > e->length() )
      {

        if ( name.substr( name.length() - e->length(), e->length()) == *e )
        {

          return name.substr( 0, name.length() - e->length() ) + '_' +
                 extraName + *e;

        }

      }
      ++ e;

    }
    return name + "_" + extraName;

  }
  GKG_CATCH( "std::string getExtraName( "
             "const std::string& name, "
             "const std::string& extraName )" );

}


//
// class CoilSensitivityMapCommand
//

gkg::CoilSensitivityMapCommand::CoilSensitivityMapCommand( int32_t argc,
                                                           char* argv[],
                                                           bool loadPlugin,
                                                           bool removeFirst )
                               : gkg::Command( argc, argv, loadPlugin,
                                               removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::CoilSensitivityMapCommand::CoilSensitivityMapCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CoilSensitivityMapCommand::CoilSensitivityMapCommand(
                                   const std::string& fileNameCoilInput,
                                   const std::string& fileNameSensitivityOutput,
                                   float dilationRadiusFactor,
                                   float closingRadiusFactor,
                                   double noisePosition,
                                   double noiseRatio,
                                   int32_t thrown,
                                   int32_t kept,
                                   bool extra,
                                   bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameCoilInput,
             fileNameSensitivityOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             verbose );

  }
  GKG_CATCH( "gkg::CoilSensitivityMapCommand::CoilSensitivityMapCommand( "
             "const std::string& fileNameCoilInput, "
             "const std::string& fileNameSensitivityOutput, "
             "float dilationRadiusFactor, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "bool extra, "
             "bool verbose  )" );

}


gkg::CoilSensitivityMapCommand::CoilSensitivityMapCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCoilInput );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameSensitivityOutput );
    DECLARE_FLOATING_PARAMETER( parameters, float, dilationRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, float, closingRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, noisePosition );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseRatio );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, thrown );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, kept );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, extra );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameCoilInput,
             fileNameSensitivityOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             verbose );

  }
  GKG_CATCH( "gkg::CoilSensitivityMapCommand::"
             "CoilSensitivityMapCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CoilSensitivityMapCommand::~CoilSensitivityMapCommand()
{
}


std::string gkg::CoilSensitivityMapCommand::getStaticName()
{

  try
  {

    return "CoilSensitivityMap";

  }
  GKG_CATCH( "std::string gkg::CoilSensitivityMapCommand::getStaticName()" );

}


void gkg::CoilSensitivityMapCommand::parse()
{

  try
  {

    std::string fileNameCoilInput;
    std::string fileNameSensitivityOutput;
    float dilationRadiusFactor = 2.0;
    float closingRadiusFactor = 2.0;
    double noisePosition = 0.9;
    double noiseRatio = 0.01;
    int32_t thrown = 20;
    int32_t kept = 100;
    bool extra = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Coil sensitivity map",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Multi-coil input file name",
                                 fileNameCoilInput );
    application.addSingleOption( "-o",
                                 "Output sensitivity file name",
                                 fileNameSensitivityOutput );
    application.addSingleOption( "-d",
                                 "Dilation radius factor [default=2.0]\n"
                                 "Morphological dilation of the B1 reference "
                                 "mask is done with the maximum in-plane "
                                 "resolution times this factor",
                                 dilationRadiusFactor,
                                 true );
    application.addSingleOption( "-r",
                                 "Closing radius factor [default=2.0]\n"
                                 "Morphological closing is done with the"
                                 " minimum in-plane resolution times this "
                                 "factor",
                                 closingRadiusFactor,
                                 true );
    application.addSingleOption( "-noisePosition",
                                 "Noise position along x axis (default=0.9)",
                                 noisePosition,
                                 true );
    application.addSingleOption( "-noiseRatio",
                                 "Noise ratio (default=0.01)",
                                 noiseRatio,
                                 true );
    application.addSingleOption( "-thrown",
                                 "Maximum thrown count (default=20)",
                                 thrown,
                                 true );
    application.addSingleOption( "-kept",
                                 "Maximum kept (default=100)",
                                 kept,
                                 true );
    application.addSingleOption( "-extra",
                                 "Save extra information :\n"
                                 "- magnitude of B1 reference\n"
                                 "- magnitude of B1 sensitivity\n"
                                 "- mask of B1 reference\n"
                                 "- enlarged mask of B1 reference\n"
                                 "- extrapolated filtered B1 sensitivity\n",
                                 extra,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameCoilInput,
             fileNameSensitivityOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::CoilSensitivityMapCommand::parse()" );

}


void gkg::CoilSensitivityMapCommand::execute(
                                   const std::string& fileNameCoilInput,
                                   const std::string& fileNameSensitivityOutput,
                                   float dilationRadiusFactor,
                                   float closingRadiusFactor,
                                   double noisePosition,
                                   double noiseRatio,
                                   int32_t thrown,
                                   int32_t kept,
                                   bool extra,
                                   bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading coil input
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading coil input '" << fileNameCoilInput
                << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > coilInput;
    gkg::Reader::getInstance().read( fileNameCoilInput, coilInput );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    double coilResolutionX = 0.0, coilResolutionY = 0.0;
    if ( coilInput.getHeader().hasAttribute( "resolutionX" ) &&
         coilInput.getHeader().hasAttribute( "resolutionY" ) )
    {

      coilInput.getHeader().getAttribute( "resolutionX", coilResolutionX );
      coilInput.getHeader().getAttribute( "resolutionY", coilResolutionY );

    }
    else
    {

      throw std::runtime_error( "cannot find X and Y in plane resolution" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building B1 reference map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building B1 reference : " << std::flush;

    }
    gkg::MagnitudeConverter< gkg::Volume< std::complex< float > >,
                             gkg::Volume< float > > magnitudeConverter;
    gkg::B1ReferenceFunctor< float > b1ReferenceFunctor;
    gkg::Volume< std::complex< float > > b1Reference;
    b1ReferenceFunctor.process( coilInput, b1Reference, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing B1 sensitivity map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 sensitivity map : " << std::flush;

    }
    gkg::B1SensitivityFunctor< float > b1SensitivityFunctor;
    gkg::Volume< std::complex< float > > b1Sensitivity;
    b1SensitivityFunctor.process( coilInput,
                                  b1Reference,
                                  b1Sensitivity,
                                  verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving magnitude of B1 sensitivity : " << std::flush;

      }
      gkg::Volume< float > b1SensitivityMagnitude;
      magnitudeConverter.convert( b1Sensitivity, b1SensitivityMagnitude );
      gkg::Writer::getInstance().write(
            getExtraName( fileNameSensitivityOutput, "b1SensitivityMagnitude" ),
            b1SensitivityMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing B1 reference magnitude
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 reference magnitude : " << std::flush;

    }
    gkg::Volume< float > b1ReferenceMagnitude;
    magnitudeConverter.convert( b1Reference, b1ReferenceMagnitude );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving magnitude of B1 reference : " << std::flush;

      }
      gkg::Writer::getInstance().write(
              getExtraName( fileNameSensitivityOutput, "b1ReferenceMagnitude" ),
              b1ReferenceMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building B1 reference mask : " << std::flush;

    }
    gkg::MorphoFilledMask< float, uint8_t >
      morphoFilledMaskAlgo( closingRadiusFactor * 
                            std::min( coilResolutionX,
                                      coilResolutionY ),
                            noisePosition,
                            noiseRatio,
                            thrown,
                            kept );
    gkg::Volume< uint8_t > b1ReferenceMask;
    morphoFilledMaskAlgo.getMask( b1ReferenceMagnitude,
                                  b1ReferenceMask,
                                  verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving B1 reference mask : " << std::flush;

      }
      gkg::Writer::getInstance().write(
                   getExtraName( fileNameSensitivityOutput, "b1ReferenceMask" ),
                   b1ReferenceMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building enlarged B1 mask : " << std::flush;

    }
    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
    gkg::Dilation< uint8_t, uint8_t >
      dilation( dilationRadiusFactor * std::max( coilResolutionX,
                                                 coilResolutionY ),
                equalToFunction,
                1U,
                0U );
    gkg::Volume< uint8_t > enlargedB1ReferenceMask;
    dilation.dilate( b1ReferenceMask,
                     enlargedB1ReferenceMask,
                     verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving enlarged B1 reference mask : " << std::flush;

      }
      gkg::Writer::getInstance().write(
           getExtraName( fileNameSensitivityOutput, "enlargedB1ReferenceMask" ),
           enlargedB1ReferenceMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // median filtering sensitivity map over B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "filtering B1 sensitivity map : " << std::flush;

    }
    gkg::MedianFilter< std::vector< std::complex< float > >,
                       std::complex< float > > medianFilter;
    gkg::Filter3d< std::complex< float >,
                   uint8_t,
                   std::complex< float > > medianFilter3d;
    medianFilter3d.filter( b1Sensitivity,
                           b1ReferenceMask,
                           equalToFunction,
                           medianFilter,
                           gkg::Neighborhood3d::Neighborhood2d_8xy,
                           b1Sensitivity,
                           verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // median extrapolating sensitivity map over enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "extrapolating B1 sensitivity map : " << std::flush;

    }
    gkg::RecursiveExtrapolator3d< std::complex< float >,
                                  uint8_t,
                                  std::complex< float > >
      medianExtrapolator3d( medianFilter,
                            gkg::Neighborhood3d::Neighborhood2d_8xy );
    medianExtrapolator3d.extrapolate( b1Sensitivity,
                                      b1ReferenceMask,
                                      enlargedB1ReferenceMask,
                                      equalToFunction,
                                      b1Sensitivity,
                                      verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // masking sensitivity map outside enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "masking B1 sensitivity map : " << std::flush;

    }
    gkg::ConstantFilter< std::complex< float >, std::complex< float > >
      constantFilter( std::complex< float >( 0.0, 0.0 ) );
    gkg::DifferentFromFunction< uint8_t > differentFromFunction( 1U );
    gkg::ConditionalMask< gkg::Volume< std::complex< float > >, uint8_t >
      conditionalMask;
    conditionalMask.mask( b1Sensitivity,
                          enlargedB1ReferenceMask,
                          differentFromFunction,
                          constantFilter );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving masked sensitivity B1 map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "saving masked extrapolated B1 sensitivity : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameSensitivityOutput,
                                      b1Sensitivity );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving magnitude of masked sensitivity B1 map
    ////////////////////////////////////////////////////////////////////////////
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving magnitude of extrapolated B1 sensitivity : "
                  << std::flush;

      }
      gkg::Volume< float > b1SensitivityMagnitude;
      magnitudeConverter.convert( b1Sensitivity, b1SensitivityMagnitude );
      gkg::Writer::getInstance().write(
                           getExtraName( fileNameSensitivityOutput,
                                         "extrapolatedB1SensitivityMagnitude" ),
                           b1SensitivityMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::CoilSensitivityMapCommand::execute( "
             "const std::string& fileNameCoilInput, "
             "const std::string& fileNameSensitivityOutput, "
             "float dilationRadiusFactor, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "bool extra, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    CoilSensitivityMapCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameCoilInput ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSensitivityOutput ) +
    DECLARE_FLOATING_PARAMETER_HELP( dilationRadiusFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( closingRadiusFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( noisePosition ) +
    DECLARE_FLOATING_PARAMETER_HELP( noiseRatio ) +
    DECLARE_INTEGER_PARAMETER_HELP( thrown ) +
    DECLARE_INTEGER_PARAMETER_HELP( kept ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( extra ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
