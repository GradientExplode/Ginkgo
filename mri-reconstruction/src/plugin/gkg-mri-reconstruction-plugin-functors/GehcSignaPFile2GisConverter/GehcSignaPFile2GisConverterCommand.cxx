#include <gkg-mri-reconstruction-plugin-functors/GehcSignaPFile2GisConverter/GehcSignaPFile2GisConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/PhaseConverter_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
// class GehcPFileReadProcess
//

class GehcPFileReadProcess : public gkg::Process
{

  public:

    GehcPFileReadProcess( const std::string& fileNameOut,
                          const std::string& outputFormat,
                          const std::string& fileNameMag,
                          const std::string& fileNamePha,
                          bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    const std::string& getFileNameMag() const;
    const std::string& getFileNamePha() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    const std::string& _fileNameMag;
    const std::string& _fileNamePha;
    bool _verbose;

};


GehcPFileReadProcess::GehcPFileReadProcess( const std::string& fileNameOut,
                                            const std::string& outputFormat,
                                            const std::string& fileNameMag,
                                            const std::string& fileNamePha,
                                            bool verbose )
                     : gkg::Process(),
                       _fileNameOut( fileNameOut ),
                       _outputFormat( outputFormat ),
                       _fileNameMag( fileNameMag ),
                       _fileNamePha( fileNamePha ),
                       _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                   &GehcPFileReadProcess::read< std::complex< float > > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                   &GehcPFileReadProcess::read< std::complex< double > > );

}


const std::string& GehcPFileReadProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& GehcPFileReadProcess::getOutputFormat() const
{

  return _outputFormat;

}


const std::string& GehcPFileReadProcess::getFileNameMag() const
{

  return _fileNameMag;

}


const std::string& GehcPFileReadProcess::getFileNamePha() const
{

  return _fileNamePha;

}


bool GehcPFileReadProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void GehcPFileReadProcess::read( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    GehcPFileReadProcess&
      readProcess = static_cast< GehcPFileReadProcess& >( process );

    // we may need magnitude and phase converter
    gkg::MagnitudeConverter< gkg::Volume< T >,
                             gkg::Volume< float > > magnitudeConverter;
    gkg::PhaseConverter< gkg::Volume< T >,
                             gkg::Volume< float > > phaseConverter;


    // checking that data is readable and collecting sizes
    if ( readProcess.getVerbose() )
    {

      std::cout << "reading GEHC Signa PFile header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< T > inputVolumeProxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                           gkg::Volume< T > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gehc-signa-pfile" )
    {

      throw std::runtime_error( "wrong format, should be \'gehc-signa-pfile\'");

    }

    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< T > inputCplxSubVolume( sizeX, lineCount, sliceCount );
    gkg::Volume< float > inputFloatSubVolume( sizeX, lineCount, sliceCount );

    // preparing partial write proxy for complex data
    if ( readProcess.getVerbose() )
    {

      std::cout << "preparing partial write in \'"
                << readProcess.getFileNameOut()
                << "\': "
                << std::flush;

    }
    gkg::VolumeProxy< T >
      outputCplxVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    outputCplxVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputCplxVolumeProxy.getHeader()[ "item_type" ] =
                                gkg::TypeOf< T >::getName();
    outputCplxVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
    outputCplxVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
    outputCplxVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
    outputCplxVolumeProxy.getHeader()[ "sizeT" ] = timeCount;

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX - 1,
                                                   0, lineCount - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< T > >(
                                              readProcess.getFileNameOut(),
                                              outputCplxVolumeProxy,
                                              readProcess.getOutputFormat() ) !=
         readProcess.getOutputFormat() )
    {

      throw std::runtime_error(
              std::string( "failed to prepare partial write with format \'" ) +
              readProcess.getOutputFormat() + "\'" );

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // preparing partial write proxy for magnitude data
    gkg::VolumeProxy< float >
      outputMagVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    if ( !readProcess.getFileNameMag().empty() )
    {

      if ( readProcess.getVerbose() )
      {

        std::cout << "preparing partial write in \'"
                  << readProcess.getFileNameMag()
                  << "\': "
                  << std::flush;

      }
      outputMagVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
      outputMagVolumeProxy.getHeader()[ "item_type" ] =
                                                gkg::TypeOf< float >::getName();
      outputMagVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
      outputMagVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
      outputMagVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
      outputMagVolumeProxy.getHeader()[ "sizeT" ] = timeCount;


      if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< float > >(
                                              readProcess.getFileNameMag(),
                                              outputMagVolumeProxy,
                                              readProcess.getOutputFormat() ) !=
           readProcess.getOutputFormat() )
      {

        throw std::runtime_error(
                std::string( "failed to prepare partial write in \'" ) +
                readProcess.getFileNameMag() + "\'" );

      }
      if ( readProcess.getVerbose() )
      {

        std::cout << "done " << std::endl;

      }

    }

    // preparing partial write proxy for phase data
    gkg::VolumeProxy< float >
      outputPhaVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    if ( !readProcess.getFileNamePha().empty() )
    {

      if ( readProcess.getVerbose() )
      {

        std::cout << "preparing partial write in \'"
                  << readProcess.getFileNamePha()
                  << "\': "
                  << std::flush;

      }
      outputPhaVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
      outputPhaVolumeProxy.getHeader()[ "item_type" ] =
                                                gkg::TypeOf< float >::getName();
      outputPhaVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
      outputPhaVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
      outputPhaVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
      outputPhaVolumeProxy.getHeader()[ "sizeT" ] = timeCount;


      if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< float > >(
                                              readProcess.getFileNamePha(),
                                              outputPhaVolumeProxy,
                                              readProcess.getOutputFormat() ) !=
           readProcess.getOutputFormat() )
      {

        throw std::runtime_error(
                std::string( "failed to prepare partial write in \'" ) +
                readProcess.getFileNamePha() + "\'" );

      }
      if ( readProcess.getVerbose() )
      {

        std::cout << "done " << std::endl;

      }

    }

    // looping over ranks(s)
    if ( readProcess.getVerbose() )
    {

      std::cout << "converting: "
                << std::flush;

    }

    int32_t time;
    for ( time = 0; time < timeCount; time++ )
    {

      if ( readProcess.getVerbose() )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      // reading raw data from fid Bruker file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      format =
        gkg::Reader::getInstance().template partialBinaryRead<
                                             gkg::Volume< T > >(
                                                           inputVolumeProxy,
                                                           inputCplxSubVolume,
                                                           inputBoundingBox,
                                                           "gehc-signa-pfile" );
      if ( format != "gehc-signa-pfile" )
      {

        throw std::runtime_error( "wrong format, should be "
                                  "\'gehc-signa-pfile\'");

      }

      // saving sub-volume(s) to disk
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< T > >(
                                                outputCplxVolumeProxy,
                                                inputCplxSubVolume,
                                                outputBoundingBox,
                                                readProcess.getOutputFormat() );

      if ( !readProcess.getFileNameMag().empty() )
      {

        magnitudeConverter.convert( inputCplxSubVolume, inputFloatSubVolume );
        gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< float > >(
                                                outputMagVolumeProxy,
                                                inputFloatSubVolume,
                                                outputBoundingBox,
                                                readProcess.getOutputFormat() );


      }

      if ( !readProcess.getFileNamePha().empty() )
      {

        phaseConverter.convert( inputCplxSubVolume, inputFloatSubVolume );
        gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< float > >(
                                                outputPhaVolumeProxy,
                                                inputFloatSubVolume,
                                                outputBoundingBox,
                                                readProcess.getOutputFormat() );


      }

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void GehcPFileReadProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class GehcSignaPFile2GisConverterCommand
//

gkg::GehcSignaPFile2GisConverterCommand::GehcSignaPFile2GisConverterCommand(
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
  GKG_CATCH( "gkg::GehcSignaPFile2GisConverterCommand::"
             "GehcSignaPFile2GisConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaPFile2GisConverterCommand::GehcSignaPFile2GisConverterCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                const std::string& fileNameMag,
                                                const std::string& fileNamePha,
                                                bool verbose )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             fileNameMag,
             fileNamePha,
             verbose );

  }
  GKG_CATCH( "gkg::GehcSignaPFile2GisConverterCommand::"
             "GehcSignaPFile2GisConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "const std::string& fileNameMag, "
             "const std::string& fileNamePha, "
             "bool verbose  )" );

}


gkg::GehcSignaPFile2GisConverterCommand::GehcSignaPFile2GisConverterCommand(
                                             const gkg::Dictionary& parameters )
                                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMag );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNamePha );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             fileNameMag,
             fileNamePha,
             verbose );

  }
  GKG_CATCH( "gkg::GehcSignaPFile2GisConverterCommand::"
             "GehcSignaPFile2GisConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaPFile2GisConverterCommand::~GehcSignaPFile2GisConverterCommand()
{
}


std::string gkg::GehcSignaPFile2GisConverterCommand::getStaticName()
{

  try
  {

    return "GehcSignaPFile2GisConverter";

  }
  GKG_CATCH( "std::string gkg::GehcSignaPFile2GisConverterCommand::getStaticName()" );

}


void gkg::GehcSignaPFile2GisConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    std::string fileNameMag = "";
    std::string fileNamePha = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                              "GEHC Signa PFile to GIS disk format converter",
                              _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input GEHC Signa PFile name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "GIS complex image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-m",
                                 "Magnitude GIS filename",
                                 fileNameMag,
                                 true );
    application.addSingleOption( "-p",
                                 "Phase GIS filename",
                                 fileNamePha,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             fileNameMag,
             fileNamePha,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcSignaPFile2GisConverterCommand::parse()" );

}


void gkg::GehcSignaPFile2GisConverterCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                const std::string& fileNameMag,
                                                const std::string& fileNamePha,
                                                bool verbose )
{

  try
  {

    GehcPFileReadProcess gehcPFileReadProcess( fileNameOut, outputFormat,
                                               fileNameMag, fileNamePha,
                                               verbose );
    gehcPFileReadProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::GehcSignaPFile2GisConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "const std::string& fileNameMag, "
             "const std::string& fileNamePha, "
             "bool verbose  )" );

}


RegisterCommandCreator(
    GehcSignaPFile2GisConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMag ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePha ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
