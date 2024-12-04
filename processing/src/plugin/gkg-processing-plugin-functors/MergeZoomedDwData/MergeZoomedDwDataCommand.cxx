#include <gkg-processing-plugin-functors/MergeZoomedDwData/MergeZoomedDwDataCommand.h>
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
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>


#define EPSILON 1e-6


gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand(
                              	   const std::vector< std::string >& fileNameIn,
                              	   const std::string& fileNameOut,
                              	   bool ascii,
                              	   const std::string& outputFormat,
                              	   bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand( "
             "const std::vector< std::string >& fileNameIn, "
             "const std::string& fileNameOut, bool ascii, "
             "const std::string& outputFormat, bool verbose )" );

}


gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::MergeZoomedDwDataCommand::MergeZoomedDwDataCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MergeZoomedDwDataCommand::~MergeZoomedDwDataCommand()
{
}


std::string gkg::MergeZoomedDwDataCommand::getStaticName()
{

  try
  {

    return "MergeZoomedDwData";

  }
  GKG_CATCH( "std::string gkg::MergeZoomedDwDataCommand::getStaticName()" );

}


void gkg::MergeZoomedDwDataCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameIn;
    std::string fileNameOut;
    bool ascii = false;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Merge DW data acquired with Zoom feature "
                                  "of DW EPI SE pulse sequence in order to "
                                  "get subresolution image",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input file name(s)",
                                 fileNameIn,
                                 1U );
    application.addSingleOption( "-o",
                                 "Output file name(s)",
                                 fileNameOut );
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

    execute( fileNameIn, fileNameOut, ascii, outputFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MergeZoomedDwDataCommand::parse()" );

}


void gkg::MergeZoomedDwDataCommand::execute(
                  	      	   const std::vector< std::string >& fileNameIn,
                              	   const std::string& fileNameOut,
                              	   bool /* ascii */,
                              	   const std::string& outputFormat,
                              	   bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting file count and performing sanity checks
    ////////////////////////////////////////////////////////////////////////////
    int32_t fileCount = ( int32_t )fileNameIn.size();

    if ( !fileCount )
    {

      throw std::runtime_error( "there should be at least one file" );

    }

    int32_t zoomCount = ( int32_t )std::pow( ( double )fileCount, 1.0 / 3.0 );
    if ( verbose )
    {

      std::cout << "zoom count : " << zoomCount << std::endl;

    }
    if ( std::fabs( ( std::pow( ( double )fileCount, 1.0 / 3.0 ) - zoomCount ) )
         > EPSILON )
    {

      throw std::runtime_error( "wrong number of input files" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // checking that data is readable and collecting size(s)
    ////////////////////////////////////////////////////////////////////////////
    std::vector< gkg::VolumeProxy< int16_t > > inputVolumeProxy( fileCount );

    int32_t f;
    int32_t sizeX = 0;
    int32_t lineCount = 0;
    int32_t sliceCount = 0;
    int32_t timeCount = 0;
    double resolutionX;
    double resolutionY;
    double resolutionZ;
    for ( f = 0; f < fileCount; f++ )
    {

      if ( verbose )
      {

        std::cout << "reading GIS file header: "
                  << fileNameIn[ f ] << std::endl;

      }
      std::string format;

      format = gkg::Reader::getInstance().preparePartialBinaryRead<
                              gkg::Volume< int16_t > >( fileNameIn[ f ],
                                                        inputVolumeProxy[ f ] );
      if ( format != "gis" )
      {

        throw std::runtime_error( "wrong format, should be \'gis\'" );

      }

      if ( f == 0 )
      {

        sizeX = inputVolumeProxy[ f ].getSizeX();
        lineCount = inputVolumeProxy[ f ].getSizeY();
        sliceCount = inputVolumeProxy[ f ].getSizeZ();
        timeCount = inputVolumeProxy[ f ].getSizeT();
        inputVolumeProxy[ f ].getHeader().getAttribute(
                                                   "resolutionX", resolutionX );
        inputVolumeProxy[ f ].getHeader().getAttribute(
                                                   "resolutionY", resolutionY );
        inputVolumeProxy[ f ].getHeader().getAttribute(
                                                   "resolutionZ", resolutionZ );

      }
      else
      {

        if ( ( inputVolumeProxy[ f ].getSizeX() != sizeX ) ||
             ( inputVolumeProxy[ f ].getSizeY() != lineCount ) ||
             ( inputVolumeProxy[ f ].getSizeZ() != sliceCount ) ||
             ( inputVolumeProxy[ f ].getSizeT() != timeCount ) )
        {

          throw std::runtime_error( "inconsistent X, Y, Z, T size(s)" );

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // getting information about pulse sequence name
    ////////////////////////////////////////////////////////////////////////////
    std::vector< std::string > psd_iname( fileCount );

    for ( f = 0; f < fileCount; f++ )
    {

      if ( inputVolumeProxy[ f ].getHeader().hasAttribute( 
                                       "pool_header.rdb_hdr_image.psd_iname" ) )
      {

        inputVolumeProxy[ f ].getHeader().getAttribute(
                        "pool_header.rdb_hdr_image.psd_iname", psd_iname[ f ] );

      }
      else
      {

        throw std::runtime_error( "cannot read psd_iname" );

      }

      if ( f != 0 )
      {

        if ( psd_iname[ f ] != psd_iname[ 0 ] )
        {

          throw std::runtime_error( "inconsistent pulse sequence name" );

        }

      }

    }

    if ( verbose )
    {

      std::cout <<  "pulse sequence name : " << psd_iname[ 0 ] << std::endl;

    }

    if ( ( psd_iname[ 0 ] != "GkgDwSeEpi" ) &&
         ( psd_iname[ 0 ] != "NmrDwSeEpi" ) )
    {

      throw std::runtime_error( "bad pulse sequence name" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // determining file order 
    ////////////////////////////////////////////////////////////////////////////
    std::vector< int32_t > order( fileCount );

    for ( f = 0; f < fileCount; f++ )
    {

      if ( inputVolumeProxy[ f ].getHeader().hasAttribute( 
                                       "pool_header.rdb_hdr_image.user18" ) )
      {

        double user18;
        inputVolumeProxy[ f ].getHeader().getAttribute(
                            "pool_header.rdb_hdr_image.user18", user18 );
        int32_t zoomIndex = ( ( int32_t )( user18 + 0.5 ) % 1000 ) / 10;
        int32_t localZoomCount = ( int )( user18 + 0.5 ) / 1000;
        if ( localZoomCount != zoomCount )
        {

          throw std::runtime_error( "incoherent file and zoom count" );

        }
        if ( zoomIndex >= fileCount )
        {

          throw std::runtime_error(
                               "index of zoom greater or equal to zoom count" );

        }
        order[ zoomIndex ] = f;


      }
      else
      {

        throw std::runtime_error( "cannot read user18" );

      }

    }

    if ( verbose )
    {

      std::cout << "file order : ";
      for ( f = 0; f < fileCount; f++ )
      {

        std::cout << order[ f ] << " ";

      }
      std::cout << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing a bounding box and a sub-volume for reading
    ////////////////////////////////////////////////////////////////////////////
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< int16_t > inputSubVolume;


    ////////////////////////////////////////////////////////////////////////////
    // preparing output partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    int32_t newSizeX = sizeX * zoomCount;
    int32_t newLineCount = lineCount * zoomCount;
    int32_t newSliceCount = sliceCount * zoomCount;
    if ( verbose )
    {

      std::cout << "new X size : " << newSizeX << std::endl;
      std::cout << "new line count : " << newLineCount << std::endl;
      std::cout << "new  slice count : " << newSliceCount << std::endl;
      std::cout << "new time count : " << timeCount << std::endl;

    }

    if ( verbose )
    {

      std::cout << "preparing output partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< int16_t > outputVolumeProxy( newSizeX,
                                                   newLineCount,
                                                   newSliceCount,
                                                   timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy[ 0 ].getHeader();
    outputVolumeProxy.getHeader()[ "sizeX" ] = newSizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = newLineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = newSliceCount;
    outputVolumeProxy.getHeader()[ "resolutionX" ] = resolutionX / zoomCount;
    outputVolumeProxy.getHeader()[ "resolutionY" ] = resolutionY / zoomCount;
    outputVolumeProxy.getHeader()[ "resolutionZ" ] = resolutionZ / zoomCount;

    gkg::Volume< int16_t > outputSubVolume( newSizeX,
                                            newLineCount,
                                            newSliceCount );

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, newSizeX - 1,
                                                   0, newLineCount - 1,
                                                   0, newSliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                gkg::Volume< int16_t > >( fileNameOut,
                                                          outputVolumeProxy,
                                                          outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
              std::string( "failed to prepare output partial write with " ) +
              "format \' " + outputFormat + "\'" );

    }
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building output data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building output data: " << std::flush;

    }
    int32_t x, line, slice, time;
    int32_t xOffset, lineOffset, sliceOffset;
    for ( time = 0; time < timeCount; time++ )
    {

      if ( verbose )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );

      for ( f = 0; f < fileCount; f++ )
      {

        std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                   gkg::Volume< int16_t > >(
                                                 inputVolumeProxy[ order[ f ] ],
                                                 inputSubVolume,
                                                 inputBoundingBox,
                                                 "gis" );
        if ( format != "gis" )
        {

          throw std::runtime_error( "wrong format, should be \'gis\'");

        }


        xOffset = ( f % ( zoomCount * zoomCount ) ) % zoomCount;
        lineOffset = ( f % ( zoomCount * zoomCount ) ) / zoomCount;
        sliceOffset = f / ( zoomCount * zoomCount );

        for ( slice = 0; slice < sliceCount; slice++ )
        {

          for ( line = 0; line < lineCount; line++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              outputSubVolume( x * zoomCount + zoomCount - 1 - xOffset,
                              line * zoomCount + zoomCount - 1 - lineOffset,
                              slice * zoomCount + zoomCount - 1 - sliceOffset ) =
                inputSubVolume( x, line, slice );

            }

          }

        }

      }

      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().partialBinaryWrite< 
	                                         gkg::Volume< int16_t > >(
                                                              outputVolumeProxy,
                                                              outputSubVolume,
                                                              outputBoundingBox,
                                                              outputFormat );


    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MergeZoomedDwDataCommand::execute( "
             "const std::vector< std::string >& fileNameIn, "
             "const std::string& fileNameOut, bool ascii, "
             "const std::string& outputFormat, bool verbose )" );

}


RegisterCommandCreator( MergeZoomedDwDataCommand,
                        DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
