#include <gkg-dmri-plugin-functors/DwiSubVolume/DwiSubVolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/GkgSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/SmsVb15SingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/GehcSingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>


gkg::DwiSubVolumeCommand::DwiSubVolumeCommand( int32_t argc,
                                               char* argv[],
                                               bool loadPlugin,
                                               bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiSubVolumeCommand::DwiSubVolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSubVolumeCommand::DwiSubVolumeCommand(
                                         const std::string& fileNameIn,
                                         const std::string& fileNameOut,
                                         std::vector< int32_t >& ignoredIndices,
                                         int32_t subSetOrientationCount,
                                         int32_t maximumNeighborCount,
                                         bool ascii,
                                         const std::string& outputFormat,
                                         bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             ignoredIndices,
             subSetOrientationCount,
             maximumNeighborCount,
             ascii,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSubVolumeCommand::DwiSubVolumeCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "std::vector< int32_t >& ignoredIndices, "
             "int32_t subSetOrientationCount, int32_t maximumNeighborCount, "
             "bool ascii, const std::string& outputFormat, bool verbose )" );

}


gkg::DwiSubVolumeCommand::DwiSubVolumeCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          ignoredIndices );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, subSetOrientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumNeighborCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             ignoredIndices,
             subSetOrientationCount,
             maximumNeighborCount,
             ascii,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSubVolumeCommand::DwiSubVolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSubVolumeCommand::~DwiSubVolumeCommand()
{
}


std::string gkg::DwiSubVolumeCommand::getStaticName()
{

  try
  {

    return "DwiSubVolume";

  }
  GKG_CATCH( "std::string gkg::DwiSubVolumeCommand::getStaticName()" );

}


void gkg::DwiSubVolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::vector< int32_t > ignoredIndices;
    int32_t subSetOrientationCount = -1;
    int32_t maximumNeighborCount = 10;
    bool ascii = false;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Remove diffusion gradient orientations from"
                                  " diffusion data",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input diffusion-weighted data file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output diffusion-weighted data file name",
                                 fileNameOut );
    application.addSeriesOption( "-ignoredIndices",
                                 "Ignored indices",
                                 ignoredIndices );
    application.addSingleOption( "-subSetOrientationCount",
                                 "Sub-set orientation count",
                                 subSetOrientationCount,
                                 true );
    application.addSingleOption( "-maximumNeighborCount",
                                 "Maximum orientation neighbor count "
                                 "(default=10)",
                                 maximumNeighborCount,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 outputFormat,
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

    execute( fileNameIn,
             fileNameOut,
             ignoredIndices,
             subSetOrientationCount,
             maximumNeighborCount,
             ascii,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiSubVolumeCommand::parse()" );

}


void gkg::DwiSubVolumeCommand::execute( const std::string& fileNameIn,
                       		        const std::string& fileNameOut,
                      		        std::vector< int32_t >& ignoredIndices,
                      		        int32_t subSetOrientationCount,
                      		        int32_t maximumNeighborCount,
                      		        bool /* ascii */,
                      		        const std::string& outputFormat,
                      		        bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////
    if ( subSetOrientationCount > 0 && !ignoredIndices.empty() )
    {

      throw std::runtime_error( "'-subSetOrientationCount' and "
                                "'-ignoredIndices' cannot be used together" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input proxy
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading GIS file header " << fileNameIn << " : "
                << std::flush;

    }
    std::string format;

    gkg::VolumeProxy< float > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                            gkg::Volume< float > >( fileNameIn,
                                                      inputVolumeProxy );
    if ( format != "gis" )
    {

      throw std::runtime_error( "wrong format, should be \'gis\'" );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting size information and q-space sampling information
    ////////////////////////////////////////////////////////////////////////////
    int32_t sizeT = inputVolumeProxy.getSizeT();

    gkg::RCPointer< gkg::QSpaceSampling >
      inputQSpaceSampling( gkg::QSpaceSamplingFactory::getInstance().create(
                                                           inputVolumeProxy ) );
    gkg::RCPointer< gkg::QSpaceSampling > outputQSpaceSampling;
    int32_t newSizeT = 0;
    std::set< int32_t > ignoredIndicesSet;

    if ( ( dynamic_cast< gkg::GkgSingleShellQSpaceSampling* >( 
                                         inputQSpaceSampling.getPointer() ) ) ||
         ( dynamic_cast< gkg::SmsVb15SingleShellQSpaceSampling* >( 
                                         inputQSpaceSampling.getPointer() ) ) ||
         ( dynamic_cast< gkg::GehcSingleShellQSpaceSampling* >( 
                                         inputQSpaceSampling.getPointer() ) ) ||
         ( dynamic_cast< gkg::CustomSingleShellQSpaceSampling* >( 
                                         inputQSpaceSampling.getPointer() ) ) )
    {

      // collecting information from input q-space sampling
      gkg::Dictionary
        inputGradientCharacteristics = 
                          inputQSpaceSampling->getRawGradientCharacteristics();
      const gkg::RCPointer< gkg::HomogeneousTransform3d< float > >& 
        inputTransform3d = inputQSpaceSampling->getRawTransform3d();
      const std::vector< float >& 
        inputBValues = inputQSpaceSampling->getRawBValues();
      const std::vector< float >& 
        inputDiffusionTimes = inputQSpaceSampling->getRawDiffusionTimes();
      const std::vector< Vector3d< float > >& 
        inputMotionRotation3ds = 
                        inputQSpaceSampling->getRawMotionRotation3ds();
      const std::vector< Vector3d< float > >& 
        inputOrientations = inputQSpaceSampling->getRawOrientations();


      // computing the ignoredIndices if a subset of orientations is wanted
      if ( subSetOrientationCount > 0 )
      {

        gkg::OrientationSet inputOrientationSet( inputOrientations );

        // building new orientation set
        gkg::ElectrostaticOrientationSet
          outputOrientationSet( subSetOrientationCount );

        // building kept indice(s)
        std::set< int32_t > keptIndices;
        gkg::OrientationSet::const_iterator o = outputOrientationSet.begin(),
                                            oe = outputOrientationSet.end();
        int32_t index = 0;
        while ( o != oe )
        {

          std::vector< int32_t > nearestNeighborIndices =
                               inputOrientationSet.getNearestOrientationIndices(
                                                     *o, maximumNeighborCount );
          index = 0;
          while ( ( index < maximumNeighborCount ) &&
                  ( keptIndices.find( nearestNeighborIndices[ index ] ) !=
                    keptIndices.end() ) )
          {

            ++ index;

          }
          if ( index < maximumNeighborCount )
          {

            keptIndices.insert( nearestNeighborIndices[ index ] );

          }
          else
          {

            throw std::runtime_error( "maximum neighbor count too small" );

          }
          ++ o;

        }
      
        // building ignored indice(s)
        ignoredIndices.reserve( sizeT - ( int32_t )keptIndices.size() );
        for ( index = 0; index < sizeT; index++ )
        {

          if ( keptIndices.find( index ) == keptIndices.end() )
          {

            ignoredIndices.push_back( index );

          }

        }

      }

      // computing new T size and putting ignored indices in a set
      int32_t ignoredIndiceCount = ( int32_t )ignoredIndices.size();
      newSizeT = sizeT - ignoredIndiceCount;

      // building set from vector of indices
      std::vector< int32_t >::const_iterator i = ignoredIndices.begin(),
                                             ie = ignoredIndices.end();
      while ( i != ie )
      {

        ignoredIndicesSet.insert( *i );
        ++ i;

      }

      // computing output q-space sampling parameters
      std::vector< float > outputBValues( newSizeT );
      std::vector< float > outputDiffusionTimes( newSizeT );
      std::vector< Vector3d< float > > outputMotionRotation3ds( newSizeT );
      std::vector< gkg::Vector3d< float > > outputOrientations( newSizeT );

      gkg::HomogeneousTransform3d< float >
        outputTransform3d = *inputTransform3d;


      int32_t index = 0;
      int32_t newIndex = 0;
      for ( index = 0; index < sizeT; index++ )
      {

        if ( ignoredIndicesSet.find( index ) == ignoredIndicesSet.end() )
        {

          outputBValues[ newIndex ] = inputBValues[ index ];
          outputDiffusionTimes[ newIndex ] = inputDiffusionTimes[ index ];
          outputMotionRotation3ds[ newIndex ] = inputMotionRotation3ds[ index ];
          outputOrientations[ newIndex ] = inputOrientations[ index ];

          ++ newIndex;

        }

      }

      gkg::Dictionary outputGradientCharacteristics;
      if ( inputGradientCharacteristics[ "type" ]->getString() == "unknown" )
      {

        outputGradientCharacteristics = inputGradientCharacteristics;
        


      }
      else if ( inputGradientCharacteristics[ "type" ]->getString() == "PGSE" )
      {

        gkg::GenericObjectList inputLittleDeltas =
                                  inputGradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

        gkg::GenericObjectList inputBigDeltas =
                                     inputGradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

        gkg::GenericObjectList inputGradientMagnitudes =
                           inputGradientCharacteristics[ "gradient-magnitudes" ]
                                         ->getValue< gkg::GenericObjectList >();

        gkg::GenericObjectList outputLittleDeltas( newSizeT );
        gkg::GenericObjectList outputBigDeltas( newSizeT );
        gkg::GenericObjectList outputGradientMagnitude( newSizeT );

        for ( index = 0; index < sizeT; index++ )
        {

          if ( ignoredIndicesSet.find( index ) == ignoredIndicesSet.end() )
          {

            outputLittleDeltas[ newIndex ] = inputLittleDeltas[ index ];
            outputBigDeltas[ newIndex ] = inputBigDeltas[ index ];
            outputGradientMagnitude[ newIndex ] =
                                               inputGradientMagnitudes[ index ];
            ++ newIndex;

          }

        }

        outputGradientCharacteristics[ "type" ] = std::string( "PGSE" );
        outputGradientCharacteristics[ "little-delta" ] = outputLittleDeltas;
        outputGradientCharacteristics[ "big-delta" ] = outputBigDeltas;
        outputGradientCharacteristics[ "gradient-magnitudes" ] = 
                                                        outputGradientMagnitude;

      }
      else
      {

        throw std::runtime_error( "invalid input gradient characteristics" );

      }

      // creating a custom single shell output q-space sampling
      outputQSpaceSampling.reset(
        new gkg::CustomSingleShellQSpaceSampling(
                      outputBValues,
                      outputOrientations,
                      outputTransform3d,
                      outputGradientCharacteristics ) );


      for ( newIndex = 0; newIndex < newSizeT; newIndex++ )
      {

        outputQSpaceSampling->setMotionRotation3d(
                                          newIndex,
                                          outputMotionRotation3ds[ newIndex ] );

      }

    }
    else
    {

      throw std::runtime_error( "not a managed q-space type" );


    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing sub DW partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "preparing sub DW partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< float > outputVolumeProxy( inputVolumeProxy.getSizeX(),
                                                 inputVolumeProxy.getSizeY(),
                                                 inputVolumeProxy.getSizeZ(),
                                                 newSizeT );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "sizeT" ] = newSizeT;
    outputQSpaceSampling->addInformationToHeader( outputVolumeProxy );

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                    gkg::Volume< float > >( fileNameOut,
                                                            outputVolumeProxy,
                                                            outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
              std::string( "failed to prepare sub DW partial write with " ) +
              "format \' " + outputFormat + "\'" );

    }
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }



    ////////////////////////////////////////////////////////////////////////////
    // reading input DW data to output DW data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building sub DW data: " << std::flush;

    }
    gkg::BoundingBox< int32_t >
      inputBoundingBox( 0, inputVolumeProxy.getSizeX() - 1,
                        0, inputVolumeProxy.getSizeY() - 1,
                        0, inputVolumeProxy.getSizeZ() - 1,
                        0, 0 );
    gkg::BoundingBox< int32_t >
      outputBoundingBox( 0, inputVolumeProxy.getSizeX() - 1,
                         0, inputVolumeProxy.getSizeY() - 1,
                         0, inputVolumeProxy.getSizeZ() - 1,
                         0, 0 );
    gkg::Volume< float > inputSubVolume;
    int32_t index = 0, newIndex = 0;
    for ( index = 0; index < sizeT; index++ )
    {

      if ( verbose )
      {

        if ( index != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << index + 1
                  << " / " << std::setw( 4 ) << sizeT
                  << " ] " << std::flush;

      }

      if ( ignoredIndicesSet.find( index ) == ignoredIndicesSet.end() )
      {

        inputBoundingBox.setLowerT( index );
        inputBoundingBox.setUpperT( index );

        std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                 gkg::Volume< float > >(
                                                               inputVolumeProxy,
                                                               inputSubVolume,
                                                               inputBoundingBox,
                                                               "gis" );
        if ( format != "gis" )
        {

          throw std::runtime_error( "wrong format, should be \'gis\'");

        }

        outputBoundingBox.setLowerT( newIndex );
        outputBoundingBox.setUpperT( newIndex );
        gkg::Writer::getInstance().partialBinaryWrite<
                                    gkg::Volume< float > >( outputVolumeProxy,
                                                              inputSubVolume,
                                                              outputBoundingBox,
                                                              outputFormat );

        ++ newIndex;

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiSubVolumeCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "std::vector< int32_t >& ignoredIndices, "
             "int32_t subSetOrientationCount, int32_t maximumNeighborCount, "
             "bool ascii, const std::string& outputFormat, bool verbose )" );

}


RegisterCommandCreator(
    DwiSubVolumeCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( ignoredIndices ) +
    DECLARE_INTEGER_PARAMETER_HELP( subSetOrientationCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumNeighborCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
