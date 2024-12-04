#include <gkg-processing-plugin-functors/ComplexSplitter/ComplexSplitterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/RealPartConverter_i.h>
#include <gkg-processing-algobase/ImagPartConverter_i.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <fstream>
#include <iostream>


gkg::ComplexSplitterCommand::ComplexSplitterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::ComplexSplitterCommand::ComplexSplitterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::ComplexSplitterCommand::ComplexSplitterCommand(
                  				  const std::string& fileNameIn,
                  				  std::string& fileNameOut, 
                  				  bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::ComplexSplitterCommand::ComplexSplitterCommand( "
             "const std::string& fileNameIn, std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::ComplexSplitterCommand::ComplexSplitterCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::ComplexSplitterCommand::ComplexSplitterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ComplexSplitterCommand::~ComplexSplitterCommand()
{
}


std::string gkg::ComplexSplitterCommand::getStaticName()
{

  try
  {

    return "ComplexSplitter";

  }
  GKG_CATCH( "std::string gkg::ComplexSplitterCommand::getStaticName()" );

}


void gkg::ComplexSplitterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Split a complex data into both float data",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input complex data filename",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output float data filename",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::ComplexSplitterCommand::parse()" );

}


void gkg::ComplexSplitterCommand::execute( const std::string& fileNameIn,
                                           std::string& fileNameOut, 
                                           bool verbose )
{

  try
  {

    if ( fileNameOut.empty() )
    {

      fileNameOut = fileNameIn;

    }

    // reading complex data
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > complexData;
    gkg::Reader::getInstance().read( fileNameIn, complexData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // taking real part
    if ( verbose )
    {

      std::cout << "taking real part : " << std::flush;

    }
    gkg::Volume< float > realPartData;
    gkg::RealPartConverter< gkg::Volume< std::complex< float > >,
                            gkg::Volume< float > > realPartConverter;
    realPartConverter.convert( complexData, realPartData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // taking imag part
    if ( verbose )
    {

      std::cout << "taking imag part : " << std::flush;

    }
    gkg::Volume< float > imagPartData;
    gkg::ImagPartConverter< gkg::Volume< std::complex< float > >,
                            gkg::Volume< float > > imagPartConverter;
    imagPartConverter.convert( complexData, imagPartData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // saving real part
    if ( verbose )
    {

      std::cout << "saving '" << fileNameOut + "_real" << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut + "_real", realPartData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // saving imag part
    if ( verbose )
    {

      std::cout << "saving '" << fileNameOut + "_imag" << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut + "_imag", imagPartData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::ComplexSplitterCommand::execute( "
             "const std::string& fileNameIn, std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( ComplexSplitterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
