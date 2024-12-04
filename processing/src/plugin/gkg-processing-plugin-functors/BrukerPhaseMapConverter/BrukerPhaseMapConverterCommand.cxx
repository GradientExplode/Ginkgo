#include <gkg-processing-plugin-functors/BrukerPhaseMapConverter/BrukerPhaseMapConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::BrukerPhaseMapConverterCommand::BrukerPhaseMapConverterCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                   : gkg::Command( argc, argv, 
                                                   loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( 
         "gkg::BrukerPhaseMapConverterCommand::BrukerPhaseMapConverterCommand( "
         "int32_t argc, char* argv[], bool loadPlugin, bool removeFirst )" );

}


gkg::BrukerPhaseMapConverterCommand::BrukerPhaseMapConverterCommand(
                                    	 const std::string& fileNameEcho1Real,
                                    	 const std::string& fileNameEcho1Imag,
                                    	 const std::string& fileNameEcho2Real,
                                    	 const std::string& fileNameEcho2Imag,
                                    	 const std::string& fileNameB0Phase,
                                    	 const std::string& fileNameB0Magnitude,
                                    	 bool verbose )
{

  try
  {

    execute( fileNameEcho1Real, fileNameEcho1Imag,
             fileNameEcho2Real, fileNameEcho2Imag,
             fileNameB0Phase, fileNameB0Magnitude, verbose );

  }
  GKG_CATCH(
         "gkg::BrukerPhaseMapConverterCommand::BrukerPhaseMapConverterCommand( "
         "const std::string& fileNameEcho1Real, "
         "const std::string& fileNameEcho1Imag, "
         "const std::string& fileNameEcho2Real, "
         "const std::string& fileNameEcho2Imag, "
         "const std::string& fileNameB0Phase, "
         "const std::string& fileNameB0Magnitude, "
         "bool verbose )" );

}


gkg::BrukerPhaseMapConverterCommand::BrukerPhaseMapConverterCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEcho1Real );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEcho1Imag );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEcho2Real );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEcho2Imag );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameB0Phase );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameB0Magnitude );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameEcho1Real, fileNameEcho1Imag,
             fileNameEcho2Real, fileNameEcho2Imag,
             fileNameB0Phase, fileNameB0Magnitude, verbose );

  }
  GKG_CATCH( "gkg::BrukerPhaseMapConverterCommand::"
             "BrukerPhaseMapConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerPhaseMapConverterCommand::~BrukerPhaseMapConverterCommand()
{
}


std::string gkg::BrukerPhaseMapConverterCommand::getStaticName()
{

  try
  {

    return "BrukerPhaseMapConverter";

  }
  GKG_CATCH( "std::string "
             "gkg::BrukerPhaseMapConverterCommand::getStaticName()" );

}


void gkg::BrukerPhaseMapConverterCommand::parse()
{

  try
  {

    std::string fileNameEcho1Real;
    std::string fileNameEcho1Imag;
    std::string fileNameEcho2Real;
    std::string fileNameEcho2Imag;
    std::string fileNameB0Phase;
    std::string fileNameB0Magnitude;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Bruker phase map converter",
                                  _loadPlugin );
    application.addSingleOption( "-e1r",
                                 "Input file name of real part of 1st echo",
                                 fileNameEcho1Real );
    application.addSingleOption( "-e1i",
                                 "Input file name of imag part of 1st echo",
                                 fileNameEcho1Imag );
    application.addSingleOption( "-e2r",
                                 "Input file name of real part of 2nd echo",
                                 fileNameEcho2Real );
    application.addSingleOption( "-e2i",
                                 "Input file name of imag part of 2nd echo",
                                 fileNameEcho2Imag );
    application.addSingleOption( "-op",
                                 "Output phase file name",
                                 fileNameB0Phase );
    application.addSingleOption( "-om",
                                 "Output magnitude file name",
                                 fileNameB0Magnitude );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameEcho1Real, fileNameEcho1Imag,
             fileNameEcho2Real, fileNameEcho2Imag,
             fileNameB0Phase, fileNameB0Magnitude, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerPhaseMapConverterCommand::parse()" );

}


void gkg::BrukerPhaseMapConverterCommand::execute(
                  	            	 const std::string& fileNameEcho1Real,
                                    	 const std::string& fileNameEcho1Imag,
                                    	 const std::string& fileNameEcho2Real,
                                    	 const std::string& fileNameEcho2Imag,
                                    	 const std::string& fileNameB0Phase,
                                    	 const std::string& fileNameB0Magnitude,
                                    	 bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "reading '" << fileNameEcho1Real << "' : " << std::flush;

    }
    gkg::Volume< int16_t > echo1r;
    gkg::Reader::getInstance().read( fileNameEcho1Real, echo1r );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading '" << fileNameEcho1Imag << "' : " << std::flush;

    }
    gkg::Volume< int16_t > echo1i;
    gkg::Reader::getInstance().read( fileNameEcho1Imag, echo1i );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading '" << fileNameEcho2Real << "' : " << std::flush;

    }
    gkg::Volume< int16_t > echo2r;
    gkg::Reader::getInstance().read( fileNameEcho2Real, echo2r );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading '" << fileNameEcho2Imag << "' : " << std::flush;

    }
    gkg::Volume< int16_t > echo2i;
    gkg::Reader::getInstance().read( fileNameEcho2Imag, echo2i );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t sizeX = echo1r.getSizeX();
    int32_t sizeY = echo1r.getSizeY();
    int32_t sizeZ = echo1r.getSizeZ();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    echo1r.getResolution( resolution );

    gkg::Volume< float > phase( sizeX, sizeY, sizeZ );
    phase.setResolution( resolution );

    gkg::Volume< float > magnitudeFLOAT( sizeX, sizeY, sizeZ );
    magnitudeFLOAT.setResolution( resolution );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          std::complex< float > c1( echo1r( x, y, z ), echo1i( x, y, z ) );
          std::complex< float > c2( echo2r( x, y, z ), echo2i( x, y, z ) );
          std::complex< float > c = c2 * std::conj( c1 );

          phase( x, y, z ) = std::arg( c );
          magnitudeFLOAT( x, y, z ) = std::abs( c1 );

        }

      }

    }

    gkg::MinMaxFilter< gkg::Volume< float >, float > minMaxFilter;
    std::pair< float, float > minMax;
    minMaxFilter.filter( magnitudeFLOAT, minMax ); 

    gkg::Volume< int16_t > magnitude( sizeX, sizeY, sizeZ );
    magnitude.setResolution( resolution );
    gkg::Rescaler< gkg::Volume< float >, gkg::Volume< int16_t > > 
      rescaler( minMax.first, minMax.second, 0, 32767 );
    rescaler.rescale( magnitudeFLOAT, magnitude );

    if ( verbose )
    {

      std::cout << "writing '" << fileNameB0Phase << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameB0Phase, phase );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "writing '" << fileNameB0Magnitude << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameB0Magnitude, magnitude );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::BrukerPhaseMapConverterCommand::execute( "
             "const std::string& fileNameEcho1Real, "
             "const std::string& fileNameEcho1Imag, "
             "const std::string& fileNameEcho2Real, "
             "const std::string& fileNameEcho2Imag, "
             "const std::string& fileNameB0Phase, "
             "const std::string& fileNameB0Magnitude, "
             "bool verbose )" );

}


RegisterCommandCreator( BrukerPhaseMapConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameEcho1Real ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameEcho1Imag ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameEcho2Real ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameEcho2Imag ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameB0Phase ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameB0Magnitude ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
