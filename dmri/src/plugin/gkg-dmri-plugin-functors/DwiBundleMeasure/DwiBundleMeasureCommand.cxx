#include <gkg-dmri-plugin-functors/DwiBundleMeasure/DwiBundleMeasureCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <map>
#include <iostream>
#include <fstream>


gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand(
                             const std::string& fileNameBundleMap,
                             const std::string& fileNameMeasure,
                             const std::vector< std::string >& measureNames,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameMeasure,
             measureNames,
             stringParameters,
             scalarParameters,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameMeasure, "
             "const std::vector< std::string >& measureNames, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeasure );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         measureNames );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         stringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameMeasure,
             measureNames,
             stringParameters,
             scalarParameters,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMeasureCommand::DwiBundleMeasureCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMeasureCommand::~DwiBundleMeasureCommand()
{
}


std::string gkg::DwiBundleMeasureCommand::getStaticName()
{

  try
  {

    return "DwiBundleMeasure";

  }
  GKG_CATCH( "std::string gkg::DwiBundleMeasureCommand::getStaticName()" );

}


void gkg::DwiBundleMeasureCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameMeasure;

    std::vector< std::string > measureNames;
    std::vector< std::string > stringParameters;
    std::vector< double > scalarParameters;

    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI bundle measure tool",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map file name",
                                 fileNameBundleMap );
    application.addSingleOption( "-o",
                                 "Output measure text file name",
                                 fileNameMeasure );
    application.addSeriesOption( "-m",
                                 std::string(
                                   "List of measures to be performed : \n" ) +
                                  gkg::BundleMeasureChecker::getInstance().
                                                              getNameListHelp(),
                                 measureNames );

    std::set< std::string >
      helpMeasureNames = gkg::BundleMeasureChecker::getInstance().getNames();

    std::string stringParametersHelp = 
      "String parameters for the measures as a vector of string "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::string scalarParametersHelp = 
      "Scalar parameters for the measures as a vector of double "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::set< std::string >::const_iterator
      n = helpMeasureNames.begin(),
      ne = helpMeasureNames.end();
    while ( n != ne )
    {

      stringParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::BundleMeasureChecker::getInstance().
                                            getStringParameterHelp( *n ) + "\n";

      scalarParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::BundleMeasureChecker::getInstance().
                                            getScalarParameterHelp( *n ) + "\n";

      ++ n;

    }

    application.addSeriesOption( "-stringParameters",
                                 stringParametersHelp,
                                 stringParameters );
    application.addSeriesOption( "-scalarParameters",
                                 scalarParametersHelp,
                                 scalarParameters );

    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameBundleMap,
             fileNameMeasure,
             measureNames,
             stringParameters,
             scalarParameters,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMeasureCommand::parse()" );

}


void gkg::DwiBundleMeasureCommand::execute(
                             const std::string& fileNameBundleMap,
                             const std::string& fileNameMeasure,
                             const std::vector< std::string >& measureNames,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string, std::vector< std::string > > stringParameterMap;
    std::map< std::string, std::vector< double > > scalarParameterMap;

    std::set< std::string >
      helpMeasureNames = gkg::BundleMeasureChecker::getInstance().getNames();


    int32_t stringOffset = 0;
    int32_t scalarOffset = 0;
    std::vector< std::string >::const_iterator
      mn = measureNames.begin(),
      mne = measureNames.end();
    while ( mn != mne )
    {

      // checking that measure name is valid
      if ( helpMeasureNames.find( *mn ) == helpMeasureNames.end() )
      {

        throw std::runtime_error( 
                         std::string( "measure '" ) + *mn + "' is not valid" );

      }

      // getting the number of string parameters
      int32_t stringParameterCount =
        gkg::BundleMeasureChecker::getInstance().getStringParameterCount( *mn );

      // getting the number of scalar parameters
      int32_t scalarParameterCount =
        gkg::BundleMeasureChecker::getInstance().getScalarParameterCount( *mn );

      // reserving vectors in string and scalar parameter maps
      stringParameterMap[ *mn ].reserve( stringParameterCount );
      scalarParameterMap[ *mn ].reserve( scalarParameterCount );

      // building specific string parameters for current measure
      int32_t p = 0;
      for ( p = 0; p < stringParameterCount; p++ )
      {

        if ( stringOffset + p < ( int32_t )stringParameters.size() )
        {

          stringParameterMap[ *mn ].push_back(
                                         stringParameters[ stringOffset + p ] );

        }
        else
        {

          throw std::runtime_error( "string vector of parameters too short" );

        }

      }
      // building specific scalar parameters for current measure
      for ( p = 0; p < scalarParameterCount; p++ )
      {

        if ( scalarOffset + p < ( int32_t )scalarParameters.size() )
        {

          scalarParameterMap[ *mn ].push_back(
                                         scalarParameters[ scalarOffset + p ] );

        }
        else
        {

          throw std::runtime_error( "scalar vector of parameters too short" );

        }

      }

      // performing sanity checks on string parameters
      gkg::BundleMeasureChecker::getInstance().doSanityChecks(
                                                    *mn,
                                                    stringParameterMap[ *mn ],
                                                    scalarParameterMap[ *mn ] );

      stringOffset += stringParameterCount;
      scalarOffset += scalarParameterCount;

      ++ mn;

    }
    

    ////////////////////////////////////////////////////////////////////////////
    // reading bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameBundleMap << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::BundleMap< std::string > > bundleMapString;
    gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMapInt16;
    try
    {

      // first case where the file format on disk fit with a bundle map
      // labelled with strings
      gkg::BundleMap< std::string >* 
        bundleMap = new gkg::BundleMap< std::string >;
      gkg::Reader::getInstance().read( fileNameBundleMap, *bundleMap );
      bundleMapString.reset( bundleMap );

    }
    catch ( std::exception& )
    {


      try
      {

        // second case where the file format on disk fit with a bundle map
        // labelled with int16_t
        gkg::BundleMap< int16_t >*
          bundleMap = new gkg::BundleMap< int16_t >;
        gkg::Reader::getInstance().read( fileNameBundleMap, *bundleMap );
        bundleMapInt16.reset( bundleMap );

      }
      catch ( std::exception& )
      {

        throw std::runtime_error( "unable to read bundle map" );

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building a fiber measure spreadsheet and adding measures to it
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::BundleMeasureSpreadSheet< std::string > >
      bundleMeasureSpreadSheetString;
    gkg::RCPointer< gkg::BundleMeasureSpreadSheet< int16_t > >
      bundleMeasureSpreadSheetInt16;
    if ( !bundleMapString.isNull() )
    {

      bundleMeasureSpreadSheetString.reset(
        new gkg::BundleMeasureSpreadSheet< std::string >( bundleMapString ) );

      mn = measureNames.begin();
      mne = measureNames.end();
      while ( mn != mne )
      {

        bundleMeasureSpreadSheetString->addMeasure( *mn,
                                                    stringParameterMap[ *mn ],
                                                    scalarParameterMap[ *mn ] );
        ++ mn;

      }

    }
    else if ( !bundleMapInt16.isNull() )
    {

      bundleMeasureSpreadSheetInt16.reset(
        new gkg::BundleMeasureSpreadSheet< int16_t >( bundleMapInt16 ) );

      mn = measureNames.begin();
      mne = measureNames.end();
      while ( mn != mne )
      {

        bundleMeasureSpreadSheetInt16->addMeasure( *mn,
                                                   stringParameterMap[ *mn ],
                                                   scalarParameterMap[ *mn ] );
        ++ mn;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving spreadsheet
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "writing '" << fileNameMeasure << "' spreadsheet: "
               << std::flush;

    }

    if ( !bundleMeasureSpreadSheetString.isNull() )
    {

      gkg::Writer::getInstance().write( fileNameMeasure,
                                        *bundleMeasureSpreadSheetString );

    }
    else if ( !bundleMeasureSpreadSheetInt16.isNull() )
    {

      gkg::Writer::getInstance().write( fileNameMeasure,
                                        *bundleMeasureSpreadSheetInt16 );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiBundleMeasureCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameMeasure, "
             "const std::vector< std::string >& measureNames, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMeasureCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMeasure ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( measureNames ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
