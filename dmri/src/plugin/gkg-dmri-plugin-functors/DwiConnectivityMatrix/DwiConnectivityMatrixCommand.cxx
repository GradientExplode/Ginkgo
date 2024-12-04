#include <gkg-dmri-plugin-functors/DwiConnectivityMatrix/DwiConnectivityMatrixCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class ConnectivityMatrixProcess : public gkg::Process
{

  public:

    ConnectivityMatrixProcess( const std::list< std::string >& 
                                                          theFileNameBundleMaps,
                               const std::string& theTypeOfROIs1,
                               const std::list< std::string >& 
                                                         theFileNameListOfROIs1,
                               const std::string& 
                                         theFileNameROIs1ToBundleMapTransform3d,
                               const std::string& theTypeOfROIs2,
                               const std::list< std::string >& 
                                                         theFileNameListOfROIs2,
                               const std::string& 
                                         theFileNameROIs2ToBundleMapTransform3d,
                               const std::vector< double >& theScalarParameters,
                               const std::vector< std::string >& 
                                                            theStringParameters,
                               const std::list< std::string >& theFunctorNames,
                               const std::list< std::string >&
                                                             theOutputFileNames,
                               const std::string& theOutputBundleMapFormat,
                               const std::string& theOutputTextureMapFormat,
                               bool theDoNotCheckLabelTypes,
                               bool theAscii,
                               bool theVerbose );

    const std::list< std::string >& fileNameBundleMaps;
    const std::string& typeOfROIs1;
    const std::list< std::string >& fileNameListOfROIs1;
    const std::string& fileNameROIs1ToBundleMapTransform3d;
    const std::string& typeOfROIs2;
    const std::list< std::string >& fileNameListOfROIs2;
    const std::string fileNameROIs2ToBundleMapTransform3d;
    const std::vector< double >& scalarParameters;
    const std::vector< std::string >& stringParameters;
    const std::list< std::string >& functorNames;
    const std::list< std::string >& outputFileNames;
    const std::string& outputBundleMapFormat;
    const std::string& outputTextureMapFormat;
    bool doNotCheckLabelTypes;
    bool ascii;
    bool verbose;

  private:

    template < class L >
    static void process( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


ConnectivityMatrixProcess::ConnectivityMatrixProcess(
                               const std::list< std::string >& 
                                                          theFileNameBundleMaps,
                               const std::string& theTypeOfROIs1,
                               const std::list< std::string >& 
                                                         theFileNameListOfROIs1,
                               const std::string& 
                                         theFileNameROIs1ToBundleMapTransform3d,
                               const std::string& theTypeOfROIs2,
                               const std::list< std::string >& 
                                                         theFileNameListOfROIs2,
                               const std::string& 
                                         theFileNameROIs2ToBundleMapTransform3d,
                               const std::vector< double >& theScalarParameters,
                               const std::vector< std::string >& 
                                                            theStringParameters,
                               const std::list< std::string >& theFunctorNames,
                               const std::list< std::string >& 
                                                             theOutputFileNames,
                               const std::string& theOutputBundleMapFormat,
                               const std::string& theOutputTextureMapFormat,
                               bool theDoNotCheckLabelTypes,
                               bool theAscii,
                               bool theVerbose )
                          : gkg::Process( "BundleMap" ),
                            fileNameBundleMaps( theFileNameBundleMaps ),
                            typeOfROIs1( theTypeOfROIs1 ),
                            fileNameListOfROIs1( theFileNameListOfROIs1 ),
                            fileNameROIs1ToBundleMapTransform3d( 
                                       theFileNameROIs1ToBundleMapTransform3d ),
                            typeOfROIs2( theTypeOfROIs2 ),
                            fileNameListOfROIs2( theFileNameListOfROIs2 ),
                            fileNameROIs2ToBundleMapTransform3d( 
                                       theFileNameROIs2ToBundleMapTransform3d ),
                            scalarParameters( theScalarParameters ),
                            stringParameters( theStringParameters ),
                            functorNames( theFunctorNames ),
                            outputFileNames( theOutputFileNames ),
                            outputBundleMapFormat( theOutputBundleMapFormat ),
                            outputTextureMapFormat( theOutputTextureMapFormat ),
                            doNotCheckLabelTypes( theDoNotCheckLabelTypes ),
                            ascii( theAscii ),
                            verbose( theVerbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &ConnectivityMatrixProcess::process< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &ConnectivityMatrixProcess::process< int16_t > );

}


template < class L >
void ConnectivityMatrixProcess::process( gkg::Process& process,
                                         const std::string& /*fileNameIn*/,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    ConnectivityMatrixProcess&
      connectivityMatrixProcess = static_cast< ConnectivityMatrixProcess& >(
                                                                      process );

    // reading input bundle map
    if ( !connectivityMatrixProcess.doNotCheckLabelTypes )
    {

      if ( connectivityMatrixProcess.verbose )
      {

       std::cout << "checking wether bundle map labels are all of type " 
                  << gkg::TypeOf< L >::getName() << " : "
                  << std::flush;

      }

      try
      {

        std::list< std::string >::const_iterator
          f = connectivityMatrixProcess.fileNameBundleMaps.begin(),
          fe = connectivityMatrixProcess.fileNameBundleMaps.end();
        while ( f != fe )
        {

          gkg::BundleMap< L, std::less< L > > inputBundleMap;
          gkg::Reader::getInstance().read( *f, inputBundleMap );

          ++ f;

        }
        if ( connectivityMatrixProcess.verbose )
        {

          std::cout << "yes" << std::endl;
 
        }

      }
      catch ( std::exception& )
      {

        std::cout << "no" << std::endl;
        throw std::runtime_error( "bad type" );

      }

    }
    else
    {

       std::cout << "do not check wether bundle map labels are all of type " 
                  << gkg::TypeOf< L >::getName() << " : "
                  << std::endl;

    }

    // analyzing connectivity matrix
    if ( connectivityMatrixProcess.verbose )
    {

      std::cout << "analyzing connectivity matrix : " << std::flush;

    }

    
    typename gkg::ConnectivityMatrixFunctor< L, std::less< L > >*
      connectivityMatrixFunctor = 
      gkg::ConnectivityMatrixFactory< L, std::less< L > >::getInstance().create(
        connectivityMatrixProcess.typeOfROIs1,
        connectivityMatrixProcess.typeOfROIs2,
        connectivityMatrixProcess.scalarParameters,
        connectivityMatrixProcess.stringParameters );

    connectivityMatrixFunctor->apply(
                  connectivityMatrixProcess.fileNameBundleMaps,
                  connectivityMatrixProcess.fileNameListOfROIs1,
                  connectivityMatrixProcess.fileNameROIs1ToBundleMapTransform3d,
                  connectivityMatrixProcess.fileNameListOfROIs2,
                  connectivityMatrixProcess.fileNameROIs2ToBundleMapTransform3d,
                  connectivityMatrixProcess.functorNames,
                  connectivityMatrixProcess.outputFileNames,
                  connectivityMatrixProcess.outputBundleMapFormat,
                  connectivityMatrixProcess.outputTextureMapFormat,
                  connectivityMatrixProcess.ascii,
                  connectivityMatrixProcess.verbose );

    delete connectivityMatrixFunctor;

    if ( connectivityMatrixProcess.verbose )
    {

      std::cout << "done " << std::endl;

    }

  }
  GKG_CATCH( "template < class L > "
             "void ConnectivityMatrixProcess::process( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiConnectivityMatrixCommand
//

gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand(
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
  GKG_CATCH( "gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand(
                         const std::list< std::string >& fileNameBundleMaps,
                         const std::string& fileNameVolumeROIs1,
                         const std::string& fileNameMeshMapROIs1,
                         const std::string& fileNameTextureMapROIs1,
                         const std::string& fileNameROIs1ToBundleMapTransform3d,
                         const std::string& fileNameVolumeROIs2,
                         const std::string& fileNameMeshMapROIs2,
                         const std::string& fileNameTextureMapROIs2,
                         const std::string& fileNameROIs2ToBundleMapTransform3d,
                         const std::vector< double >& scalarParameters,
                         const std::vector< std::string >& stringParameters,
                         const std::list< std::string >& functorNames,
                         const std::list< std::string >& outputFileNames,
                         const std::string& outputBundleMapFormat,
                         const std::string& outputTextureMapFormat,
                         bool doNotCheckLabelTypes,
                         bool ascii,
                         bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( fileNameBundleMaps,
             fileNameVolumeROIs1,
             fileNameMeshMapROIs1,
             fileNameTextureMapROIs1,
             fileNameROIs1ToBundleMapTransform3d,
             fileNameVolumeROIs2,
             fileNameMeshMapROIs2,
             fileNameTextureMapROIs2,
             fileNameROIs2ToBundleMapTransform3d,
             scalarParameters,
             stringParameters,
             functorNames,
             outputFileNames,
             outputBundleMapFormat,
             outputTextureMapFormat,
             doNotCheckLabelTypes,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand( "
             "const std::list< std::string >& fileNameBundleMaps, "
             "const std::string& fileNameVolumeROIs1, "
             "const std::string& fileNameMeshMapROIs1, "
             "const std::string& fileNameTextureMapROIs1, "
             "const std::string& fileNameROIs1ToBundleMapTransform3d, "
             "const std::string& fileNameVolumeROIs2, "
             "const std::string& fileNameMeshMapROIs2, "
             "const std::string& fileNameTextureMapROIs2, "
             "const std::string& fileNameROIs2ToBundleMapTransform3d, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "const std::list< std::string >& functorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::string& outputBundleMapFormat, "
             "const std::string& outputTextureMapFormat, "
             "bool doNotCheckLabelTypes, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       fileNameBundleMaps );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeROIs1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMapROIs1 );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameTextureMapROIs1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameROIs1ToBundleMapTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeROIs2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMapROIs2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameTextureMapROIs2 );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameROIs2ToBundleMapTransform3d );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         stringParameters );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       functorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputBundleMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputTextureMapFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, doNotCheckLabelTypes );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMaps,
             fileNameVolumeROIs1,
             fileNameMeshMapROIs1,
             fileNameTextureMapROIs1,
             fileNameROIs1ToBundleMapTransform3d,
             fileNameVolumeROIs2,
             fileNameMeshMapROIs2,
             fileNameTextureMapROIs2,
             fileNameROIs2ToBundleMapTransform3d,
             scalarParameters,
             stringParameters,
             functorNames,
             outputFileNames,
             outputBundleMapFormat,
             outputTextureMapFormat,
             doNotCheckLabelTypes,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiConnectivityMatrixCommand::DwiConnectivityMatrixCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiConnectivityMatrixCommand::~DwiConnectivityMatrixCommand()
{
}


std::string gkg::DwiConnectivityMatrixCommand::getStaticName()
{

  try
  {

    return "DwiConnectivityMatrix";

  }
  GKG_CATCH( "std::string gkg::DwiConnectivityMatrixCommand::getStaticName()" );

}


void gkg::DwiConnectivityMatrixCommand::parse()
{

  try
  {

    std::list< std::string > fileNameBundleMaps;
    std::string fileNameVolumeROIs1;
    std::string fileNameMeshMapROIs1;
    std::string fileNameTextureMapROIs1;
    std::string fileNameROIs1ToBundleMapTransform3d;
    std::string fileNameVolumeROIs2;
    std::string fileNameMeshMapROIs2;
    std::string fileNameTextureMapROIs2;
    std::string fileNameROIs2ToBundleMapTransform3d;
    std::vector< double > scalarParameters;
    std::vector< std::string > stringParameters;
    std::list< std::string > functorNames;
    std::list< std::string > outputFileNames;
    std::string outputBundleMapFormat = "aimsbundlemap";
    std::string outputTextureMapFormat = "aimstexture";
    bool doNotCheckLabelTypes = false;
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI connectivity matrix tool",
                                  _loadPlugin );
    application.addSeriesOption( "-b",
                                 "Input bundle map file name list",
                                 fileNameBundleMaps,
                                 1 );
    application.addSingleOption( "-v1",
                                 "First input ROIs volume file name",
                                 fileNameVolumeROIs1,
                                 true );
    application.addSingleOption( "-m1",
                                 "First input ROIs mesh file name",
                                 fileNameMeshMapROIs1,
                                 true );
    application.addSingleOption( "-t1",
                                 "First input ROIs texture file name",
                                 fileNameTextureMapROIs1,
                                 true );
    application.addSingleOption( "-tr1",
                                 "3D rigid transform from ROIs1 to bundle map "
                                 "referential (default=identity)",
                                 fileNameROIs1ToBundleMapTransform3d,
                                 true );
    application.addSingleOption( "-v2",
                                 "Second input ROIs volume file name",
                                 fileNameVolumeROIs2,
                                 true );
    application.addSingleOption( "-m2",
                                 "Second input ROIs mesh file name",
                                 fileNameMeshMapROIs2,
                                 true );
    application.addSingleOption( "-t2",
                                 "Second input ROIs texture file name",
                                 fileNameTextureMapROIs2,
                                 true );
     application.addSingleOption( "-tr2",
                                 "3D rigid transform from ROIs2 to bundle map "
                                 "referential (default=identity)",
                                 fileNameROIs2ToBundleMapTransform3d,
                                 true );
    application.addSeriesOption(
          "-scalarParameters",
          gkg::ConnectivityMatrixFactory< int16_t,
                                          std::less< int16_t > >::getInstance().
                                                       getScalarParameterHelp(),
          scalarParameters );
    application.addSeriesOption(
          "-stringParameters",
          gkg::ConnectivityMatrixFactory< int16_t,
                                          std::less< int16_t > >::getInstance().
                                                       getStringParameterHelp(),
          stringParameters );
    application.addSeriesOption(
          "-f",
          gkg::ConnectivityMatrixFactory< int16_t,
                                          std::less< int16_t > >::getInstance().
                                                               getFunctorHelp(),
          functorNames,
          1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSingleOption( "-outputBundleMapFormat",
                                 "Output bundle map format "
                                 "(default=aimsbundlemap)",
                                 outputBundleMapFormat,
                                 true );
    application.addSingleOption( "-outputTextureMapFormat",
                                 "Output texture map format "
                                 "(default=aimstexture)",
                                 outputTextureMapFormat,
                                 true );
    application.addSingleOption( "-doNotCheckLabelTypes",
                                 "Do not check label types (default=false)",
                                 doNotCheckLabelTypes,
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

    execute( fileNameBundleMaps,
             fileNameVolumeROIs1,
             fileNameMeshMapROIs1,
             fileNameTextureMapROIs1,
             fileNameROIs1ToBundleMapTransform3d,
             fileNameVolumeROIs2,
             fileNameMeshMapROIs2,
             fileNameTextureMapROIs2,
             fileNameROIs2ToBundleMapTransform3d,
             scalarParameters,
             stringParameters,
             functorNames,
             outputFileNames,
             outputBundleMapFormat,
             outputTextureMapFormat,
             doNotCheckLabelTypes,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiConnectivityMatrixCommand::parse()" );

}


void gkg::DwiConnectivityMatrixCommand::execute(
                         const std::list< std::string >& fileNameBundleMaps,
                         const std::string& fileNameVolumeROIs1,
                         const std::string& fileNameMeshMapROIs1,
                         const std::string& fileNameTextureMapROIs1,
                         const std::string& fileNameROIs1ToBundleMapTransform3d,
                         const std::string& fileNameVolumeROIs2,
                         const std::string& fileNameMeshMapROIs2,
                         const std::string& fileNameTextureMapROIs2,
                         const std::string& fileNameROIs2ToBundleMapTransform3d,
                         const std::vector< double >& scalarParameters,
                         const std::vector< std::string >& stringParameters,
                         const std::list< std::string >& functorNames,
                         const std::list< std::string >& outputFileNames,
                         const std::string& outputBundleMapFormat,
                         const std::string& outputTextureMapFormat,
                         bool doNotCheckLabelTypes,
                         bool ascii,
                         bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( fileNameVolumeROIs1.empty() &&
         fileNameMeshMapROIs1.empty() &&
         fileNameTextureMapROIs1.empty() )
    {

      throw std::runtime_error( "empty input configuration for ROI1" );

    }

    if ( !fileNameVolumeROIs1.empty() )
    {

       if ( !fileNameMeshMapROIs1.empty() || !fileNameTextureMapROIs1.empty() )
       {

         throw std::runtime_error( "bad input configuration for ROI1" );

       }

    }
    else
    {

      if ( fileNameMeshMapROIs1.empty() )
      {

        throw std::runtime_error( "bad input configuration for ROI1" );

      }

    }

    if ( !( fileNameVolumeROIs2.empty() &&
            fileNameMeshMapROIs2.empty() &&
            fileNameTextureMapROIs2.empty() ) )
    {

      if ( !fileNameVolumeROIs2.empty() )
      {

         if ( !fileNameMeshMapROIs2.empty() ||
              !fileNameTextureMapROIs2.empty() )
         {

           throw std::runtime_error( "bad input configuration for ROI2" );

         }

      }
      else
      {

        if ( fileNameMeshMapROIs2.empty() )
        {

          throw std::runtime_error( "bad input configuration for ROI2" );

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // analyzing the configuration of ROI1
    ////////////////////////////////////////////////////////////////////////////

    std::string typeOfROIs1 = "none";
    std::list< std::string > fileNameListOfROIs1;
    if ( !fileNameVolumeROIs1.empty() )
    {

      typeOfROIs1 = "volume";
      fileNameListOfROIs1.push_back( fileNameVolumeROIs1 );

    }
    else
    {

      if ( fileNameTextureMapROIs1.empty() )
      {

        typeOfROIs1 = "mesh";
        fileNameListOfROIs1.push_back( fileNameMeshMapROIs1 );

      }
      else
      {

        typeOfROIs1 = "mesh-and-texture";
        fileNameListOfROIs1.push_back( fileNameMeshMapROIs1 );
        fileNameListOfROIs1.push_back( fileNameTextureMapROIs1 );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // analyzing the configuration of ROI2
    ////////////////////////////////////////////////////////////////////////////

    std::string typeOfROIs2 = "none";
    std::list< std::string > fileNameListOfROIs2;
    if ( !( fileNameVolumeROIs2.empty() &&
            fileNameMeshMapROIs2.empty() &&
             fileNameTextureMapROIs2.empty() ) )
    {

      if ( !fileNameVolumeROIs2.empty() )
      {

        typeOfROIs2 = "volume";
        fileNameListOfROIs2.push_back( fileNameVolumeROIs2 );

      }
      else
      {

        if ( fileNameTextureMapROIs2.empty() )
        {

          typeOfROIs2 = "mesh";
          fileNameListOfROIs2.push_back( fileNameMeshMapROIs2 );

        }
        else
        {

          typeOfROIs2 = "mesh-and-texture";
          fileNameListOfROIs2.push_back( fileNameMeshMapROIs2 );
          fileNameListOfROIs2.push_back( fileNameTextureMapROIs2 );

        }

      }

    }

    if ( verbose )
    {

      std::cout << "type of ROIs1 : " << typeOfROIs1 << std::endl;
      if ( typeOfROIs2 != "none" )
      {

        std::cout << "type of ROIs2 : " << typeOfROIs2 << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching the connectivity matrix process
    ////////////////////////////////////////////////////////////////////////////

    ConnectivityMatrixProcess
      connectivityMatrixProcess( fileNameBundleMaps,
                                 typeOfROIs1,
                                 fileNameListOfROIs1,
                                 fileNameROIs1ToBundleMapTransform3d,
                                 typeOfROIs2,
                                 fileNameListOfROIs2,
                                 fileNameROIs2ToBundleMapTransform3d,
                                 scalarParameters,
                                 stringParameters,
                                 functorNames,
                                 outputFileNames,
                                 outputBundleMapFormat,
                                 outputTextureMapFormat,
                                 doNotCheckLabelTypes,
                                 ascii,
                                 verbose );
    connectivityMatrixProcess.execute( fileNameBundleMaps.front() );

  }
  GKG_CATCH( "void gkg::DwiConnectivityMatrixCommand::execute( "
             "const std::list< std::string >& fileNameBundleMaps, "
             "const std::string& fileNameVolumeROIs1, "
             "const std::string& fileNameMeshMapROIs1, "
             "const std::string& fileNameTextureMapROIs1, "
             "const std::string& fileNameROIs1ToBundleMapTransform3d, "
             "const std::string& fileNameVolumeROIs2, "
             "const std::string& fileNameMeshMapROIs2, "
             "const std::string& fileNameTextureMapROIs2, "
             "const std::string& fileNameROIs2ToBundleMapTransform3d, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "const std::list< std::string >& functorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::string& outputBundleMapFormat, "
             "const std::string& outputTextureMapFormat, "
             "bool doNotCheckLabelTypes, "
             "bool ascii, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    DwiConnectivityMatrixCommand,
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( fileNameBundleMaps ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameVolumeROIs1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMeshMapROIs1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTextureMapROIs1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameROIs1ToBundleMapTransform3d ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameVolumeROIs2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMeshMapROIs2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTextureMapROIs2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameROIs2ToBundleMapTransform3d ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( functorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_STRING_PARAMETER_HELP( outputBundleMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( outputTextureMapFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( doNotCheckLabelTypes ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
