#include <gkg-mri-reconstruction-plugin-functors/CoilInhomogeneityMap/CoilInhomogeneityMapCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-mri-reconstruction-parallel/B1ReferenceFunctor.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-signal/Filter3d_i.h>
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
// class CoilInhomogeneityMapCommand
//

gkg::CoilInhomogeneityMapCommand::CoilInhomogeneityMapCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::CoilInhomogeneityMapCommand::CoilInhomogeneityMapCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CoilInhomogeneityMapCommand::CoilInhomogeneityMapCommand(
                                 const std::string& fileNameMultiCoilInput,
                                 const std::string& fileNameBodyCoilInput,
                                 const std::string& fileNameInhomogeneityOutput,
                                 float closingRadiusFactor,
                                 double noisePosition,
                                 double noiseRatio,
                                 int32_t thrown,
                                 int32_t kept,
                                 bool extra,
                                 const std::string& filter,
                                 int32_t filterIterationCount,
                                 bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameMultiCoilInput,
             fileNameBodyCoilInput,
             fileNameInhomogeneityOutput,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             filter,
             filterIterationCount,
             verbose );

  }
  GKG_CATCH( "gkg::CoilInhomogeneityMapCommand::CoilInhomogeneityMapCommand( "
             "const std::string& fileNameMultiCoilInput, "
             "const std::string& fileNameBodyCoilInput, "
             "const std::string& fileNameInhomogeneityOutput, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "bool extra, "
             "const std::string& filter, "
             "int32_t filterIterationCount, "
             "bool verbose )" );

}


gkg::CoilInhomogeneityMapCommand::CoilInhomogeneityMapCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMultiCoilInput );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBodyCoilInput );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameInhomogeneityOutput );
    DECLARE_FLOATING_PARAMETER( parameters, float, closingRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, noisePosition );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseRatio );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, thrown );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, kept );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, extra );
    DECLARE_STRING_PARAMETER( parameters, std::string, filter );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, filterIterationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameMultiCoilInput,
             fileNameBodyCoilInput,
             fileNameInhomogeneityOutput,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             filter,
             filterIterationCount,
             verbose );

  }
  GKG_CATCH( "gkg::CoilInhomogeneityMapCommand::"
             "CoilInhomogeneityMapCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CoilInhomogeneityMapCommand::~CoilInhomogeneityMapCommand()
{
}


std::string gkg::CoilInhomogeneityMapCommand::getStaticName()
{

  try
  {

    return "CoilInhomogeneityMap";

  }
  GKG_CATCH( "std::string gkg::CoilInhomogeneityMapCommand::getStaticName()" );

}


void gkg::CoilInhomogeneityMapCommand::parse()
{

  try
  {

    std::string fileNameMultiCoilInput;
    std::string fileNameBodyCoilInput;
    std::string fileNameInhomogeneityOutput;
    float closingRadiusFactor = 2.0;
    double noisePosition = 0.9;
    double noiseRatio = 0.01;
    int32_t thrown = 20;
    int32_t kept = 100;
    bool extra = false;
    std::string filter = "none";
    int32_t filterIterationCount = 1;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Coil inhomogeneity map",
                                  _loadPlugin );
    application.addSingleOption( "-c",
                                 "Multi-coil input file name",
                                 fileNameMultiCoilInput );
    application.addSingleOption( "-b",
                                 "Body-coil input file name",
                                 fileNameBodyCoilInput );
    application.addSingleOption( "-o",
                                 "Output inhomogeneity file name",
                                 fileNameInhomogeneityOutput );
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
                                 "- magnitude of multi-coil B1\n"
                                 "- magnitude of body-coil B1\n",
                                 extra,
                                 true );
    application.addSingleOption( "-filter",
                                 "Post filter type (default=none):\n"
                                 "- none\n"
                                 "- median\n"
                                 "- average\n",
                                 filter,
                                 true );
    application.addSingleOption( "-filterIterationCount",
                                 "Filter iteration count (default=1)\n",
                                 filterIterationCount,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameMultiCoilInput,
             fileNameBodyCoilInput,
             fileNameInhomogeneityOutput,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             extra,
             filter,
             filterIterationCount,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::CoilInhomogeneityMapCommand::parse()" );

}


void gkg::CoilInhomogeneityMapCommand::execute(
                                 const std::string& fileNameMultiCoilInput,
                                 const std::string& fileNameBodyCoilInput,
                                 const std::string& fileNameInhomogeneityOutput,
                                 float closingRadiusFactor,
                                 double noisePosition,
                                 double noiseRatio,
                                 int32_t thrown,
                                 int32_t kept,
                                 bool extra,
                                 const std::string& filter,
                                 int32_t filterIterationCount,
                                 bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    if ( ( filter != "none" ) &&
         ( filter != "median" ) &&
         ( filter != "average" ) )
    {

      throw std::runtime_error( "invalid filter type" );

    }    


    ////////////////////////////////////////////////////////////////////////////
    // reading multi-coil input
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading multi-coil input '" << fileNameMultiCoilInput
                << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > multiCoilInput;
    gkg::Reader::getInstance().read( fileNameMultiCoilInput, multiCoilInput );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    int32_t sizeX = multiCoilInput.getSizeX();
    int32_t sizeY = multiCoilInput.getSizeY();
    int32_t sizeZ = multiCoilInput.getSizeZ();
    double resolutionX = 0.0;
    double resolutionY = 0.0;
    double resolutionZ = 0.0;
    if ( multiCoilInput.getHeader().hasAttribute( "resolutionX" ) &&
         multiCoilInput.getHeader().hasAttribute( "resolutionY" ) &&
         multiCoilInput.getHeader().hasAttribute( "resolutionZ" ) )
    {

      multiCoilInput.getHeader().getAttribute( "resolutionX", resolutionX );
      multiCoilInput.getHeader().getAttribute( "resolutionY", resolutionY );
      multiCoilInput.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    else
    {

      throw std::runtime_error(
        "cannot find multi-coil X, Y and Z in plane resolution" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading body-coil input
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading body-coil input '" << fileNameBodyCoilInput
                << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > bodyCoilInput;
    gkg::Reader::getInstance().read( fileNameBodyCoilInput, bodyCoilInput );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( bodyCoilInput.getSizeT() != 1 )
    {

      throw std::runtime_error(
        "body-coil must have a T size equal to 1" );

    }
    if ( ( bodyCoilInput.getSizeX() != sizeX ) ||
         ( bodyCoilInput.getSizeY() != sizeY ) ||
         ( bodyCoilInput.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error(
        "incompatible multi- and body- coil size(s)" );

    }
    if ( bodyCoilInput.getHeader().hasAttribute( "resolutionX" ) &&
         bodyCoilInput.getHeader().hasAttribute( "resolutionY" ) &&
         bodyCoilInput.getHeader().hasAttribute( "resolutionZ" ) )
    {

      double tmpResolutionX = 0.0;
      double tmpResolutionY = 0.0;
      double tmpResolutionZ = 0.0;
      bodyCoilInput.getHeader().getAttribute( "resolutionX",tmpResolutionX );
      bodyCoilInput.getHeader().getAttribute( "resolutionY",tmpResolutionY );
      bodyCoilInput.getHeader().getAttribute( "resolutionZ",tmpResolutionZ );

      if ( ( tmpResolutionX != resolutionX ) ||
           ( tmpResolutionY != resolutionY ) ||
           ( tmpResolutionZ != resolutionZ ) )
      {

        throw std::runtime_error(
          "incompatible multi- and body- coil resolution(s)" );

      }

    }
    else
    {

      throw std::runtime_error(
        "cannot find body-coil X, Y and Z in plane resolution" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building B1 multi-coil reference map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building B1 multi-coil reference : " << std::flush;

    }
    gkg::B1ReferenceFunctor< float > b1ReferenceFunctor;
    gkg::Volume< std::complex< float > > b1Reference;
    b1ReferenceFunctor.process( multiCoilInput, b1Reference, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing B1 reference magnitude
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 reference magnitude : " << std::flush;

    }
    gkg::MagnitudeConverter< gkg::Volume< std::complex< float > >,
                                      gkg::Volume< float > > magnitudeConverter;
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
            getExtraName( fileNameInhomogeneityOutput, "b1ReferenceMagnitude" ),
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
                            std::min( resolutionX,
                                      resolutionY ),
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
                 getExtraName( fileNameInhomogeneityOutput, "b1ReferenceMask" ),
                 b1ReferenceMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing B1 inhomogeneity map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 inhomogeneity map : " << std::flush;

    }
    gkg::Volume< float > b1Inhomogeneity( sizeX, sizeY, sizeZ );
    b1Inhomogeneity.getHeader().addAttribute( "resolutionX", resolutionX );
    b1Inhomogeneity.getHeader().addAttribute( "resolutionY", resolutionY );
    b1Inhomogeneity.getHeader().addAttribute( "resolutionZ", resolutionZ );


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    gkg::Volume< float >::iterator i = b1Inhomogeneity.begin(),
                                   ie = b1Inhomogeneity.end();
    gkg::Volume< std::complex< float > >::const_iterator
      r = b1Reference.begin();
    gkg::Volume< std::complex< float > >::const_iterator
      b = bodyCoilInput.begin();
    gkg::Volume< uint8_t >::const_iterator
      m = b1ReferenceMask.begin();
    float denominator = 0.0;
    while ( i != ie )
    {

      if ( *m )
      {

        denominator = std::sqrt( std::norm( *b ) );
        if ( denominator != 0.0 )
        {

          *i = std::sqrt( std::norm( *r ) ) / denominator;

        }
        else
        {

          *i = 0.0;

        }

      }
      else
      {

        *i = 0.0;

      }
      ++ i;
      ++ r;
      ++ b;
      ++ m;

    }

/*
    gkg::StatisticAnalyzer< gkg::Volume< float > > statisticAnalyzer;
    float maximum = statisticAnalyzer.getAutomaticThreshold( b1Inhomogeneity,
                                                             noisePosition,
                                                             noiseRatio,
                                                             thrown,
                                                             kept );

    gkg::MaximumFilter< gkg::Volume< float >, float >
      maximumFilter;
    float maximum = 0.0;
    maximumFilter.filter( b1Inhomogeneity, maximum );

    std::cout << "maximum = " << maximum << std::endl;

    if ( maximum != 0.0 )
    {

      gkg::Scaler< gkg::Volume< float > > scaler( 1.0 / maximum );
      scaler.scale( b1Inhomogeneity );

    }
*/

    ////////////////////////////////////////////////////////////////////////////
    // filtering inhomogeneity map over B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( filter != "none" )
    {

      if ( verbose )
      {

        std::cout << "filtering inhomogeneity map : " << std::flush;

      }

      gkg::Filter3d< float, uint8_t, float > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );

      if ( filter == "median" )
      {

        gkg::MedianFilter< std::vector< float >, float > medianFilter;

        int32_t i;
        for ( i = 0; i < filterIterationCount; i++ )
        {

          filter3d.filter( b1Inhomogeneity,
                           b1ReferenceMask,
                           equalToFunction,
                           medianFilter,
                           gkg::Neighborhood3d::Neighborhood3d_26,
                           b1Inhomogeneity,
                           verbose );

        }

      }
      else if ( filter == "average" )
      {

        gkg::AverageFilter< std::vector< float >, float > averageFilter;

        int32_t i;
        for ( i = 0; i < filterIterationCount; i++ )
        {

          filter3d.filter( b1Inhomogeneity,
                           b1ReferenceMask,
                           equalToFunction,
                           averageFilter,
                           gkg::Neighborhood3d::Neighborhood3d_26,
                           b1Inhomogeneity,
                           verbose );

        }

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving inhomogeneity map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "saving inhomogeneity map : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameInhomogeneityOutput,
                                      b1Inhomogeneity );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::CoilInhomogeneityMapCommand::execute( "
             "const std::string& fileNameMultiCoilInput, "
             "const std::string& fileNameBodyCoilInput, "
             "const std::string& fileNameInhomogeneityOutput, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "bool extra, "
             "const std::string& filter, "
             "int32_t filterIterationCount, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    CoilInhomogeneityMapCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameMultiCoilInput ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameBodyCoilInput ) +
    DECLARE_STRING_PARAMETER_HELP(  fileNameInhomogeneityOutput ) +
    DECLARE_STRING_PARAMETER_HELP( closingRadiusFactor ) +
    DECLARE_STRING_PARAMETER_HELP( noisePosition ) +
    DECLARE_STRING_PARAMETER_HELP( noiseRatio ) +
    DECLARE_STRING_PARAMETER_HELP( thrown ) +
    DECLARE_STRING_PARAMETER_HELP( kept ) +
    DECLARE_STRING_PARAMETER_HELP( extra ) +
    DECLARE_STRING_PARAMETER_HELP( filter ) +
    DECLARE_STRING_PARAMETER_HELP( filterIterationCount ) +
    DECLARE_STRING_PARAMETER_HELP( verbose ) );
