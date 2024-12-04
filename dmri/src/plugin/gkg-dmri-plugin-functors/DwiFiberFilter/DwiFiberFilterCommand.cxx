#include <gkg-dmri-plugin-functors/DwiFiberFilter/DwiFiberFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-fiber-selector/FiberSelectionChecker.h>
#include <gkg-dmri-fiber-selector/FiberSelection_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>
#include <iostream>
#include <fstream>


gkg::DwiFiberFilterCommand::DwiFiberFilterCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiFiberFilterCommand::DwiFiberFilterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiFiberFilterCommand::DwiFiberFilterCommand( 
                          const std::list< std::string >& fileNameBundleMapIns,
                          const std::list< std::string >& fileNameBundleMapOuts,
                          const std::vector< std::string >& selectorNames,
                          const std::vector< std::string >& stringParameters,
                          const std::vector< double >& scalarParameters,
                          const std::string& bundleMapFormat,
                          bool ascii,
                          bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( fileNameBundleMapIns,
             fileNameBundleMapOuts,
             selectorNames,
             stringParameters,
             scalarParameters,
             bundleMapFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiFiberFilterCommand::DwiFiberFilterCommand( "
             "const std::list< std::string >& fileNameBundleMapIns, "
             "const std::list< std::string >& fileNameBundleMapOuts, "
             "const std::vector< std::string >& selectorNames, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "const std::string& bundleMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiFiberFilterCommand::DwiFiberFilterCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       fileNameBundleMapIns );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       fileNameBundleMapOuts );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         selectorNames );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         stringParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           scalarParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, bundleMapFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMapIns,
             fileNameBundleMapOuts,
             selectorNames,
             stringParameters,
             scalarParameters,
             bundleMapFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiFiberFilterCommand::DwiFiberFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiFiberFilterCommand::~DwiFiberFilterCommand()
{
}


std::string gkg::DwiFiberFilterCommand::getStaticName()
{

  try
  {

    return "DwiFiberFilter";

  }
  GKG_CATCH( "std::string gkg::DwiFiberFilterCommand::getStaticName()" );

}


void gkg::DwiFiberFilterCommand::parse()
{

  try
  {

    std::list< std::string > fileNameBundleMapIns;
    std::list< std::string > fileNameBundleMapOuts;

    std::vector< std::string > selectorNames;
    std::vector< std::string > stringParameters;
    std::vector< double > scalarParameters;

    std::string bundleMapFormat = "bundlemap";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI fiber filter tool",
                                  _loadPlugin );
    application.addSeriesOption( "-b",
                                 "Input bundle map file name list",
                                 fileNameBundleMapIns,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output bundle map file name",
                                 fileNameBundleMapOuts );
    application.addSeriesOption( "-s",
                                 std::string(
                                   "List of selectors to be applied : \n" ) +
                                  gkg::FiberSelectionChecker::getInstance().
                                                              getNameListHelp(),
                                 selectorNames );

    std::set< std::string >
      helpSelectorNames = gkg::FiberSelectionChecker::getInstance().getNames();

    std::string stringParametersHelp = 
      "String parameters for the selectors as a vector of string "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::string scalarParametersHelp = 
      "Scalar parameters for the selector as a vector of double "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::set< std::string >::const_iterator
      n = helpSelectorNames.begin(),
      ne = helpSelectorNames.end();
    while ( n != ne )
    {

      stringParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::FiberSelectionChecker::getInstance().
                                            getStringParameterHelp( *n ) + "\n";

      scalarParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::FiberSelectionChecker::getInstance().
                                            getScalarParameterHelp( *n ) + "\n";

      ++ n;

    }

    application.addSeriesOption( "-stringParameters",
                                 stringParametersHelp,
                                 stringParameters );
    application.addSeriesOption( "-scalarParameters",
                                 scalarParametersHelp,
                                 scalarParameters );

    application.addSingleOption( "-bundleMapFormat",
                                 "Ouput BundleMap format (default=bundlemap)",
                                 bundleMapFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameBundleMapIns,
             fileNameBundleMapOuts,
             selectorNames,
             stringParameters,
             scalarParameters,
             bundleMapFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiFiberFilterCommand::parse()" );

}


void gkg::DwiFiberFilterCommand::execute(
                          const std::list< std::string >& fileNameBundleMapIns,
                          const std::list< std::string >& fileNameBundleMapOuts,
                          const std::vector< std::string >& selectorNames,
                          const std::vector< std::string >& stringParameters,
                          const std::vector< double >& scalarParameters,
                          const std::string& bundleMapFormat,
                          bool ascii,
                          bool verbose )
{

  try
  {

    bool fusionFibers = false;

    std::set< std::string >
      helpSelectorNames = gkg::FiberSelectionChecker::getInstance().getNames();

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////


    if ( fileNameBundleMapOuts.size() == 1 )
    {

      fusionFibers = true;

    }
    else
    {

      if ( fileNameBundleMapIns.size() != fileNameBundleMapOuts.size() )
      {

        throw std::runtime_error( 
                            "different number of input and output bundleMaps" );

      }

    }

    std::map< std::string, std::vector< std::string > > stringParameterMap;
    std::map< std::string, std::vector< double > > scalarParameterMap;

    int32_t stringOffset = 0;
    int32_t scalarOffset = 0;
    std::vector< std::string >::const_iterator
      sn = selectorNames.begin(),
      sne = selectorNames.end();
    while ( sn != sne )
    {

      // checking that selector name is valid
      if ( helpSelectorNames.find( *sn ) == helpSelectorNames.end() )
      {

        throw std::runtime_error( 
                         std::string( "selector '" ) + *sn + "' is not valid" );

      }

      // getting the number of string parameters
      int32_t stringParameterCount =
      gkg::FiberSelectionChecker::getInstance().getStringParameterCount( *sn );

      // getting the number of scalar parameters
      int32_t scalarParameterCount =
      gkg::FiberSelectionChecker::getInstance().getScalarParameterCount( *sn );

      // reserving vectors in string and scalar parameter maps
      stringParameterMap[ *sn ].reserve( stringParameterCount );
      scalarParameterMap[ *sn ].reserve( scalarParameterCount );

      // building specific string parameters for current measure
      int32_t p = 0;
      for ( p = 0; p < stringParameterCount; p++ )
      {

        if ( stringOffset + p < ( int32_t )stringParameters.size() )
        {

          stringParameterMap[ *sn ].push_back(
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

          scalarParameterMap[ *sn ].push_back(
                                         scalarParameters[ scalarOffset + p ] );

        }
        else
        {

          throw std::runtime_error( "scalar vector of parameters too short" );

        }

      }

      // performing sanity checks on string parameters
      gkg::FiberSelectionChecker::getInstance().doSanityChecks(
                                                    *sn,
                                                    stringParameterMap[ *sn ],
                                                    scalarParameterMap[ *sn ] );

      stringOffset += stringParameterCount;
      scalarOffset += scalarParameterCount;

      ++ sn;

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over input bundle map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::FiberSelection< std::string > > fiberSelectionString;
    gkg::RCPointer< gkg::FiberSelection< int16_t > > fiberSelectionInt16;

    std::list< std::string >::const_iterator 
      b = fileNameBundleMapIns.begin(),
      be = fileNameBundleMapIns.end();

    std::list< std::string >::const_iterator 
      ob = fileNameBundleMapOuts.begin();
 
    while ( b != be )
    {

      std::string fileNameBundleMapOut = *ob;
      std::string fileNameBundleMapIn = *b;


      //////////////////////////////////////////////////////////////////////////
      // reading bundle map
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

       std::cout << "reading '" << fileNameBundleMapIn << "' : " << std::flush;

      }
      gkg::RCPointer< gkg::BundleMap< std::string > > bundleMapString;
      gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMapInt16;
      try
      {

        // first case where the file format on disk fit with a bundle map
        // labelled with strings
        gkg::BundleMap< std::string >* 
          bundleMap = new gkg::BundleMap< std::string >;
        gkg::Reader::getInstance().read( fileNameBundleMapIn, *bundleMap );
        bundleMapString.reset( bundleMap );

        if ( fiberSelectionString.isNull() || !fusionFibers )
        {

          fiberSelectionString.reset(
                    new gkg::FiberSelection< std::string >( bundleMapString ) );

        }
        else
        {

          fiberSelectionString->setInputBundleMap( bundleMapString );

        }

      }
      catch ( std::exception& )
      {


        try
        {

          // second case where the file format on disk fit with a bundle map
          // labelled with int16_t
          gkg::BundleMap< int16_t >*
            bundleMap = new gkg::BundleMap< int16_t >;
          gkg::Reader::getInstance().read( fileNameBundleMapIn, *bundleMap );
          bundleMapInt16.reset( bundleMap );

          if ( fiberSelectionInt16.isNull() || !fusionFibers )
          {

            fiberSelectionInt16.reset(
                    new gkg::FiberSelection< int16_t >( bundleMapInt16 ) );

          }
          else
          {

            fiberSelectionInt16->setInputBundleMap( bundleMapInt16 );

          }

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

      //////////////////////////////////////////////////////////////////////////
      // adding selectors to the fiber selection
      //////////////////////////////////////////////////////////////////////////

      if ( !bundleMapString.isNull() )
      {

        sn = selectorNames.begin();
        sne = selectorNames.end();
        while ( sn != sne )
        {

          if ( verbose )
          {

             std::cout << "applying selector " << *sn << " : " << std::flush;

          }

          fiberSelectionString->addSelector( *sn,
                                             stringParameterMap[ *sn ],
                                             scalarParameterMap[ *sn ] );
          if ( verbose )
          {

            std::cout << "done" << std::endl;

          }
          ++ sn;

        }

      }
      else if ( !bundleMapInt16.isNull() )
      {

        sn = selectorNames.begin();
        sne = selectorNames.end();
        while ( sn != sne )
        {

          fiberSelectionInt16->addSelector( *sn,
                                            stringParameterMap[ *sn ],
                                            scalarParameterMap[ *sn ] );
          ++ sn;

        }

      }

      // saving bundle map of selected fibers
      if ( !fusionFibers )
      {

        if ( verbose )
        {

           std::cout << "writing '" << fileNameBundleMapOut << "' selection: "
                     << std::flush;

        }

        if ( !fiberSelectionString.isNull() )
        {

          gkg::Writer::getInstance().write(
                                   fileNameBundleMapOut,
                                   fiberSelectionString->getSelectedBundleMap(),
                                   ascii,
                                   bundleMapFormat );

        }
        else if ( !fiberSelectionInt16.isNull() )
        {

          gkg::Writer::getInstance().write(
                                   fileNameBundleMapOut,
                                   fiberSelectionInt16->getSelectedBundleMap(),
                                   ascii,
                                   bundleMapFormat );

        }
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ ob;
      ++ b;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving bundle map of selected fibers
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameBundleMapOut = *( fileNameBundleMapOuts.begin() );
    if ( fusionFibers )
    {

      if ( verbose )
      {

       std::cout << "writing '" << fileNameBundleMapOut << "' selection: "
                 << std::flush;

      }

      if ( !fiberSelectionString.isNull() )
      {

        gkg::Writer::getInstance().write(
                                   fileNameBundleMapOut,
                                   fiberSelectionString->getSelectedBundleMap(),
                                   ascii,
                                   bundleMapFormat );

      }
      else if ( !fiberSelectionInt16.isNull() )
      {

        gkg::Writer::getInstance().write(
                                   fileNameBundleMapOut,
                                   fiberSelectionInt16->getSelectedBundleMap(),
                                   ascii,
                                   bundleMapFormat );

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::DwiFiberFilterCommand::execute( "
             "const std::list< std::string >& fileNameBundleMapIns, "
             "const std::list< std::string >& fileNameBundleMapOuts, "
             "const std::vector< std::string >& selectorNames, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "const std::string& bundleMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiFiberFilterCommand,
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( fileNameBundleMapIns ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( fileNameBundleMapOuts ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( selectorNames ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    DECLARE_STRING_PARAMETER_HELP( bundleMapFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
