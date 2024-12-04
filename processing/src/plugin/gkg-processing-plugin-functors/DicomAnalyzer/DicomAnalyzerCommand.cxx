#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomAnalyzerCommand.h>
#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomAnalyzer.h>
#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomSender.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DicomAnalyzerCommand::DicomAnalyzerCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DicomAnalyzerCommand::DicomAnalyzerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DicomAnalyzerCommand::DicomAnalyzerCommand(
                                      const std::string& inputDirectory,
                                      const std::string& outputDirectory,
                                      const std::string& defaultInstitutionName,
                                      bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( inputDirectory, outputDirectory, defaultInstitutionName, verbose );

  }
  GKG_CATCH( "gkg::DicomAnalyzerCommand::DicomAnalyzerCommand( "
             "const std::string& inputDirectory, "
             "const std::string& outputDirectory, "
             "const std::string& defaultInstitutionName, bool verbose )" );

}


gkg::DicomAnalyzerCommand::DicomAnalyzerCommand( const gkg::Dictionary& parameters )
                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, inputDirectory );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputDirectory );
    DECLARE_STRING_PARAMETER( parameters, std::string, defaultInstitutionName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( inputDirectory, outputDirectory, defaultInstitutionName, verbose );

  }
  GKG_CATCH( "gkg::DicomAnalyzerCommand::DicomAnalyzerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DicomAnalyzerCommand::~DicomAnalyzerCommand()
{
}


std::string gkg::DicomAnalyzerCommand::getStaticName()
{

  try
  {

    return "DicomAnalyzer";

  }
  GKG_CATCH( "std::string gkg::DicomAnalyzerCommand::getStaticName()" );

}


void gkg::DicomAnalyzerCommand::parse()
{

  try
  {

    std::string inputDirectory;
    std::string outputDirectory;
    std::string defaultInstitutionName = "UnknownInstitution";
    bool verbose = false;

    gkg::Application application( _argc, _argv, 
                                  "Dicom storage unit",
                                  _loadPlugin );

    application.addSingleOption( "-i",
                                 "Input directory",
                                 inputDirectory );
    application.addSingleOption( "-o",
                                 "Output directory",
                                 outputDirectory );
    application.addSingleOption( "-defaultInstitutionName",
                                 "Default institution name "
                                 "(default=UnknownInstitution)",
                                 defaultInstitutionName,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( inputDirectory, outputDirectory, defaultInstitutionName, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DicomAnalyzerCommand::parse()" );

}


void gkg::DicomAnalyzerCommand::execute(
                                      const std::string& inputDirectory,
                                      const std::string& outputDirectory,
                                      const std::string& defaultInstitutionName,
                                      bool verbose )
{

  try
  {

    // checking the existence and creating the output directory
    gkg::Directory directory( outputDirectory );
    if ( !directory.isValid() )
    {

      directory.mkdir();

    }
    
    // analyzing input folder
    gkg::DicomAnalyzer analyzer( inputDirectory, defaultInstitutionName );

    // reorganizing series in the output directory
    std::list< gkg::DicomSeries > series = analyzer.getSeries();
    std::list< gkg::DicomSeries >::const_iterator s = series.begin(),
                                                  se = series.end();
    while ( s != se )
    {

      if ( verbose )
      {

        std::cout << *s << std::endl;

      }

      gkg::DicomSender sender( *s );
      sender.send( outputDirectory );

      ++ s;

    }

  }
  GKG_CATCH( "void gkg::DicomAnalyzerCommand::execute( "
             "const std::string& inputDirectory, "
             "const std::string& outputDirectory, "
             "const std::string& defaultInstitutionName, bool verbose )" );

}


RegisterCommandCreator(
                       DicomAnalyzerCommand,
                       DECLARE_STRING_PARAMETER_HELP( inputDirectory ) +
                       DECLARE_STRING_PARAMETER_HELP( outputDirectory ) +
                       DECLARE_STRING_PARAMETER_HELP( defaultInstitutionName ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
