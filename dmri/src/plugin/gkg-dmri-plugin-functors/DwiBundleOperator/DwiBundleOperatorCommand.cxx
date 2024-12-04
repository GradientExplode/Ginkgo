#include <gkg-dmri-plugin-functors/DwiBundleOperator/DwiBundleOperatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <map>
#include <iostream>
#include <fstream>


//
//
//


static std::string getNameWithoutExtension( const std::string& name )
{

  try
  {

    std::set< std::string > extensions;
    extensions.insert( ".bundles" );
    extensions.insert( ".bundlesdata" );
    extensions.insert( ".bundlemap" );
    extensions.insert( ".bundlemap.minf" );
    extensions.insert( ".trk" );
    extensions.insert( ".fib" );

    std::set< std::string >::const_iterator e = extensions.begin(),
                                            ee = extensions.end();
    while ( e != ee )
    {

      if ( name.length() > e->length() )
      {

         if ( name.substr( name.length() - e->length(), e->length() ) == *e )
         {

           return name.substr( 0, name.length() - e->length() );

         }

      }
      ++ e;

    }
    return name;

  }
  GKG_CATCH( "static std::string getNameWithoutExtension( "
             "const std::string& name )" );

}



//
// class BundleMapOperatorProcess
//

class BundleMapOperatorProcess : public gkg::Process
{

  public:

    BundleMapOperatorProcess( const std::vector< std::string >&
                                                       fileNameInputBundleMaps,
                              const std::vector< std::string >&
                                                       fileNameOutputBundleMaps,
                              const std::string& operatorName,
                              const std::vector< std::string >&
                                                               stringParameters,
                              const std::vector< double >& scalarParameters,
                              const std::string& outputFormat,
                              const bool& ascii,
                              const bool& verbose );

    const std::vector< std::string >& getFileNameInputBundleMaps() const;
    const std::vector< std::string >& getFileNameOutputBundleMaps() const;
    const std::string& getOperatorName() const;
    const std::vector< std::string >& getStringParameters() const;
    const std::vector< double >& getScalarParameters() const;
    const std::string& getOutputFormat() const;
    const bool& getAscii() const;
    const bool& getVerbose() const;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::vector< std::string >& _fileNameInputBundleMaps;
    const std::vector< std::string >& _fileNameOutputBundleMaps;
    const std::string& _operatorName;
    const std::vector< std::string >& _stringParameters;
    const std::vector< double >& _scalarParameters;
    const std::string& _outputFormat;
    const bool& _ascii;
    const bool& _verbose;

};


BundleMapOperatorProcess::BundleMapOperatorProcess(
                     const std::vector< std::string >& fileNameInputBundleMaps,
                     const std::vector< std::string >& fileNameOutputBundleMaps,
                     const std::string& operatorName,
                     const std::vector< std::string >& stringParameters,
                     const std::vector< double >& scalarParameters,
                     const std::string& outputFormat,
                     const bool& ascii,
                     const bool& verbose )
                        : gkg::Process( "BundleMap" ),
                          _fileNameInputBundleMaps( fileNameInputBundleMaps ),
                          _fileNameOutputBundleMaps( fileNameOutputBundleMaps ),
                          _operatorName( operatorName ),
                          _stringParameters( stringParameters ),
                          _scalarParameters( scalarParameters ),
                          _outputFormat( outputFormat ),
                          _ascii( ascii ),
                          _verbose( verbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapOperatorProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapOperatorProcess::operate< int16_t > );

}


const std::vector< std::string >& 
BundleMapOperatorProcess::getFileNameInputBundleMaps() const
{

  return _fileNameInputBundleMaps;

}


const std::vector< std::string >& 
BundleMapOperatorProcess::getFileNameOutputBundleMaps() const
{

  return _fileNameOutputBundleMaps;

}


const std::string& 
BundleMapOperatorProcess::getOperatorName() const
{

  return _operatorName;

}


const std::vector< std::string >& 
BundleMapOperatorProcess::getStringParameters() const
{

  return _stringParameters;

}


const std::vector< double >& 
BundleMapOperatorProcess::getScalarParameters() const
{

  return _scalarParameters;

}


const std::string& 
BundleMapOperatorProcess::getOutputFormat() const
{

  return _outputFormat;

}


const bool& 
BundleMapOperatorProcess::getAscii() const
{

  return _ascii;

}


const bool& 
BundleMapOperatorProcess::getVerbose() const
{

  return _verbose;

}


template < class L >
void BundleMapOperatorProcess::operate( gkg::Process& process,
                                        const std::string& /* fileNameIn */,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    BundleMapOperatorProcess&
      bundleMapOperatorProcess = static_cast< BundleMapOperatorProcess& >(
                                                                      process );

    bool fusionBundles = false;
    if ( bundleMapOperatorProcess.getFileNameOutputBundleMaps().size() == 1 )
    {

      fusionBundles = true;

    }

    // allocating the bundle map operator
    if ( bundleMapOperatorProcess.getVerbose() )
    {

       std::cout << "creating " << bundleMapOperatorProcess.getOperatorName()
                 << " operator : "
                 << std::flush;

    }
    gkg::RCPointer< gkg::BundleMapOperator< L, std::less< L > > >
      bundleMapOperator( 
        gkg::BundleMapOperatorFactory< L, std::less< L > >::getInstance().
          create( bundleMapOperatorProcess.getOperatorName(),
                  bundleMapOperatorProcess.getStringParameters(),
                  bundleMapOperatorProcess.getScalarParameters(),
                  bundleMapOperatorProcess.getVerbose() ) );
    if ( bundleMapOperatorProcess.getVerbose() )
    {

       std::cout << "done" << std::endl;

    }


    // allocating the output bundle map
    if ( bundleMapOperatorProcess.getVerbose() )
    {

       std::cout << "allocating the output bundle map : "
                 << std::flush;

    }
    gkg::RCPointer< gkg::BundleMap< L, std::less< L > > > 
      outputBundleMap( new gkg::BundleMap< L, std::less< L > > );
    if ( bundleMapOperatorProcess.getVerbose() )
    {

       std::cout << "done" << std::endl;

    }

    // looping over input bundle maps
    int32_t bundleMapCount = 0;
    int32_t inputFileCount =
      ( int32_t )bundleMapOperatorProcess.getFileNameInputBundleMaps().size();
    std::vector< std::string >::const_iterator
      f = bundleMapOperatorProcess.getFileNameInputBundleMaps().begin(),
      fe = bundleMapOperatorProcess.getFileNameInputBundleMaps().end(),
      of = bundleMapOperatorProcess.getFileNameOutputBundleMaps().begin();

    while ( f != fe )
    {

      // reading the current bundle map
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "reading '" << *f << "' input bundle map : "
                   << std::flush;

      }
      gkg::RCPointer< gkg::BundleMap< L, std::less< L > > > 
        inputBundleMap( new gkg::BundleMap< L, std::less< L > > );
      gkg::Reader::getInstance().read( *f, *inputBundleMap );
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "done" << std::endl;

      }

      // in case of 'split-labels' operator, then save splitted bundles 
      // within operator, so add the required information to the input header
      if ( bundleMapOperatorProcess.getOperatorName() == "split-labels" )
      {

        inputBundleMap->getHeader().addAttribute(
                              "split_output_base_name",
                              getNameWithoutExtension( *of ) );
        inputBundleMap->getHeader().addAttribute(
                              "split_output_format",
                              bundleMapOperatorProcess.getOutputFormat() );
        inputBundleMap->getHeader().addAttribute(
                              "split_ascii",
                              bundleMapOperatorProcess.getAscii() ? 1.0 : 0.0 );

      }


      // adding contribution of current bundle map
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "adding contribution of '" << *f
                   << "' input bundle map : "
                   << std::flush;

      }

      if ( !fusionBundles && bundleMapCount != 0 )
      {

        outputBundleMap.reset( new gkg::BundleMap< L, std::less< L > > );

        if ( inputBundleMap->getHeader().hasAttribute( "sizeX" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeX" ) )
        {

          double sizeX = 1;
          inputBundleMap->getHeader().getAttribute( "sizeX", sizeX );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeX",
                                                   ( int32_t )( sizeX + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "sizeY" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeY" ) )
        {

          double sizeY = 1;
          inputBundleMap->getHeader().getAttribute( "sizeY", sizeY );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeY",
                                                   ( int32_t )( sizeY + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "sizeZ" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeZ" ) )
        {

          double sizeZ = 1;
          inputBundleMap->getHeader().getAttribute( "sizeZ", sizeZ );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeZ",
                                                   ( int32_t )( sizeZ + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionX" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionX" ) )
        {

          double resolutionX = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionX",
                                                    resolutionX );
          outputBundleMap->getHeader().addAttribute( "resolutionX",
                                                     resolutionX );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionY" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionY" ) )
        {

          double resolutionY = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionY",
                                                    resolutionY );
          outputBundleMap->getHeader().addAttribute( "resolutionY",
                                                     resolutionY );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionZ" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionZ" ) )
        {

          double resolutionZ = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionZ",
                                                    resolutionZ );
          outputBundleMap->getHeader().addAttribute( "resolutionZ",
                                                     resolutionZ );

        }

      }

      if ( bundleMapCount == 0 )
      {

        bundleMapOperator->startContribution( inputBundleMap, outputBundleMap );
        if ( inputBundleMap->getHeader().hasAttribute( "sizeX" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeX" ) )
        {

          double sizeX = 1;
          inputBundleMap->getHeader().getAttribute( "sizeX", sizeX );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeX",
                                                   ( int32_t )( sizeX + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "sizeY" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeY" ) )
        {

          double sizeY = 1;
          inputBundleMap->getHeader().getAttribute( "sizeY", sizeY );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeY",
                                                   ( int32_t )( sizeY + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "sizeZ" ) &&
             !outputBundleMap->getHeader().hasAttribute( "sizeZ" ) )
        {

          double sizeZ = 1;
          inputBundleMap->getHeader().getAttribute( "sizeZ", sizeZ );
          outputBundleMap->getHeader().addAttribute(
                                                   "sizeZ",
                                                   ( int32_t )( sizeZ + 0.5 ) );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionX" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionX" ) )
        {

          double resolutionX = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionX",
                                                    resolutionX );
          outputBundleMap->getHeader().addAttribute( "resolutionX",
                                                     resolutionX );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionY" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionY" ) )
        {

          double resolutionY = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionY",
                                                    resolutionY );
          outputBundleMap->getHeader().addAttribute( "resolutionY",
                                                     resolutionY );

        }
        if ( inputBundleMap->getHeader().hasAttribute( "resolutionZ" ) &&
             !outputBundleMap->getHeader().hasAttribute( "resolutionZ" ) )
        {

          double resolutionZ = 1.0;
          inputBundleMap->getHeader().getAttribute( "resolutionZ",
                                                    resolutionZ );
          outputBundleMap->getHeader().addAttribute( "resolutionZ",
                                                     resolutionZ );

        }

      }
      
      else if ( bundleMapCount == inputFileCount - 1 )
      {

        bundleMapOperator->endContribution( inputBundleMap, outputBundleMap );

      }
      else
      {

        bundleMapOperator->addContribution( inputBundleMap, outputBundleMap );

      }
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "done" << std::endl;

      }


      if ( !fusionBundles &&
           ( bundleMapOperatorProcess.getOperatorName() != "split-labels" ) )
      {
        // writing output bundle map
        if ( bundleMapOperatorProcess.getVerbose() )
        {

           std::cout << "writing '"
                     << *of
                     << "' output bundle map : "
                     << std::flush;

        }
        gkg::Writer::getInstance().write(
                                   *of,
                                   *outputBundleMap,
                                   bundleMapOperatorProcess.getAscii(),
                                   bundleMapOperatorProcess.getOutputFormat() );
        if ( bundleMapOperatorProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ bundleMapCount;
      ++ f;
      ++ of;

    }

    if ( fusionBundles &&
         ( bundleMapOperatorProcess.getOperatorName() != "split-labels" ) )
    {

      // writing output bundle map
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "writing ' "
                  << bundleMapOperatorProcess.getFileNameOutputBundleMaps()[ 0 ]
                  << "' output bundle map : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write(
                    bundleMapOperatorProcess.getFileNameOutputBundleMaps()[ 0 ],
                    *outputBundleMap,
                    bundleMapOperatorProcess.getAscii(),
                    bundleMapOperatorProcess.getOutputFormat() );
      if ( bundleMapOperatorProcess.getVerbose() )
      {

         std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapOperatorProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleOperatorCommand
//

gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand(
                     const std::vector< std::string >& fileNameInputBundleMaps,
                     const std::vector< std::string >& fileNameOutputBundleMaps,
                     const std::string& operatorName,
                     const std::vector< std::string >& stringParameters,
                     const std::vector< double >& scalarParameters,
                     const std::string& outputFormat,
                     bool ascii,
                     bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameInputBundleMaps,
             fileNameOutputBundleMaps,
             operatorName,
             stringParameters,
             scalarParameters,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand( "
             "const std::vector< std::string >& fileNameInputBundleMaps, "
             "const std::vector< std::string >& fileNameOutputBundleMaps, "
             "const std::string& operatorName, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameInputBundleMaps );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameOutputBundleMaps );
    DECLARE_STRING_PARAMETER( parameters, std::string, operatorName );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         stringParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputBundleMaps,
             fileNameOutputBundleMaps,
             operatorName,
             stringParameters,
             scalarParameters,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleOperatorCommand::DwiBundleOperatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleOperatorCommand::~DwiBundleOperatorCommand()
{
}


std::string gkg::DwiBundleOperatorCommand::getStaticName()
{

  try
  {

    return "DwiBundleOperator";

  }
  GKG_CATCH( "std::string gkg::DwiBundleOperatorCommand::getStaticName()" );

}


void gkg::DwiBundleOperatorCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameInputBundleMaps;
    std::vector< std::string > fileNameOutputBundleMaps;

    std::string operatorName;
    std::vector< std::string > stringParameters;
    std::vector< double > scalarParameters;

    std::string outputFormat;
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI bundle measure tool",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input bundle map file names",
                                 fileNameInputBundleMaps,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output bundle map file name",
                                 fileNameOutputBundleMaps );
    application.addSingleOption( "-op",
                                 std::string(
                                   "Operator on bundles to be performed : \n" ) +
                                  gkg::BundleMapOperatorFactory< std::string >::
                                                getInstance().getNameListHelp(),
                                 operatorName );

    std::set< std::string >
      helpOperatorNames = gkg::BundleMapOperatorFactory< std::string >::
                                                       getInstance().getNames();

    std::string stringParametersHelp = 
      "String parameters for the measures as a vector of string "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::string scalarParametersHelp = 
      "Scalar parameters for the measures as a vector of double "
      "<P1(M1)> ... <Pn(M1)> ..... <P1(Mm)> ... <Pn(Mm)>:\n\n";
    std::set< std::string >::const_iterator
      n = helpOperatorNames.begin(),
      ne = helpOperatorNames.end();
    while ( n != ne )
    {

      stringParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::BundleMapOperatorFactory< std::string >::
                              getInstance().getStringParameterHelp( *n ) + "\n";

      scalarParametersHelp += "- in case of " + *n + ", \n" +
                              gkg::BundleMapOperatorFactory< std::string >::
                              getInstance().getScalarParameterHelp( *n ) + "\n";

      ++ n;

    }

    application.addSeriesOption( "-stringParameters",
                                 stringParametersHelp,
                                 stringParameters );
    application.addSeriesOption( "-scalarParameters",
                                 scalarParametersHelp,
                                 scalarParameters );

    application.addSingleOption( "-of",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameInputBundleMaps,
             fileNameOutputBundleMaps,
             operatorName,
             stringParameters,
             scalarParameters,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleOperatorCommand::parse()" );

}


void gkg::DwiBundleOperatorCommand::execute(
                     const std::vector< std::string >& fileNameInputBundleMaps,
                     const std::vector< std::string >& fileNameOutputBundleMaps,
                     const std::string& operatorName,
                     const std::vector< std::string >& stringParameters,
                     const std::vector< double >& scalarParameters,
                     const std::string& outputFormat,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////

    if ( !gkg::BundleMapOperatorFactory< std::string >::getInstance().
           hasBundleMapOperator( operatorName ) )
    {

      throw std::runtime_error( "invalid operator name" );

    }

    if ( fileNameOutputBundleMaps.size() != 1 &&  
            fileNameInputBundleMaps.size() != fileNameOutputBundleMaps.size() )
      {

        throw std::runtime_error( 
                            "different number of input and output bundleMaps" );

      }

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapOperatorProcess bundleMapOperatorProcess( fileNameInputBundleMaps,
                                                       fileNameOutputBundleMaps,
                                                       operatorName,
                                                       stringParameters,
                                                       scalarParameters,
                                                       outputFormat,
                                                       ascii,
                                                       verbose );
    bundleMapOperatorProcess.execute( fileNameInputBundleMaps[ 0 ] );

  }
  GKG_CATCH( "void gkg::DwiBundleOperatorCommand::execute( "
             "const std::vector< std::string >& fileNameInputBundleMaps, "
             "const std::vector< std::string >& fileNameOutputBundleMaps, "
             "const std::string& operatorName, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleOperatorCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameInputBundleMaps ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameOutputBundleMaps ) +
    DECLARE_STRING_PARAMETER_HELP( operatorName ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
