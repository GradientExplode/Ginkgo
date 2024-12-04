#include <gkg-mri-reconstruction-plugin-functors/GehcSignaGenesis22GisConverter/GehcSignaGenesis22GisConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
// class GehcGenesis2ReadProcess
//

class GehcGenesis2ReadProcess : public gkg::Process
{

  public:

    GehcGenesis2ReadProcess( const std::string& fileNameOut,
                 const std::string& outputFormat,
                 bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    bool _verbose;

};


GehcGenesis2ReadProcess::GehcGenesis2ReadProcess(
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                bool verbose )
                        : gkg::Process( "Volume" ),
                          _fileNameOut( fileNameOut ),
                          _outputFormat( outputFormat ),
                          _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &GehcGenesis2ReadProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &GehcGenesis2ReadProcess::read< int16_t > );

}


const std::string& GehcGenesis2ReadProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& GehcGenesis2ReadProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool GehcGenesis2ReadProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void GehcGenesis2ReadProcess::read( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    GehcGenesis2ReadProcess&
      readProcess = static_cast< GehcGenesis2ReadProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( readProcess.getVerbose() )
    {

      std::cout << "reading Gehc Signa Genesis 2 file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< T > inputVolumeProxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                           gkg::Volume< T > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gehc-signa-genesis2" )
    {

      throw std::runtime_error(
                             "wrong format, should be \'gehc-signa-genesis2\'");

    }

    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    // collecting resolution(s)
    double resolutionX;
    double resolutionY;
    double resolutionZ;
    double resolutionT;
    inputVolumeProxy.getHeader().getAttribute( "resolutionX", resolutionX );
    inputVolumeProxy.getHeader().getAttribute( "resolutionY", resolutionY );
    inputVolumeProxy.getHeader().getAttribute( "resolutionZ", resolutionZ );
    inputVolumeProxy.getHeader().getAttribute( "resolutionT", resolutionT );


    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< T > inputSubVolume( sizeX, lineCount, sliceCount );

    // preparing partial write proxy
    if ( readProcess.getVerbose() )
    {

      std::cout << "preparing partial write in \'"
                << readProcess.getOutputFormat()
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< T >
      outputVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
                                gkg::TypeOf< T >::getName();
    outputVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
    outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
    outputVolumeProxy.getHeader()[ "resolutionX" ] = resolutionX;
    outputVolumeProxy.getHeader()[ "resolutionY" ] = resolutionY;
    outputVolumeProxy.getHeader()[ "resolutionZ" ] = resolutionZ;
    outputVolumeProxy.getHeader()[ "resolutionT" ] = resolutionT;

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX - 1,
                                                   0, lineCount - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< T > >(
                                              readProcess.getFileNameOut(),
                                              outputVolumeProxy,
                                              readProcess.getOutputFormat() ) !=
         readProcess.getOutputFormat() )
    {

      throw std::runtime_error(
      std::string( "failed to prepare partial write with format \' " ) +
      readProcess.getOutputFormat() + "\'" );

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

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

      // reading reco data from 2dsequ Bruker file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      format =
        gkg::Reader::getInstance().template partialBinaryRead<
                                             gkg::Volume< T > >(
                                                        inputVolumeProxy,
                                                        inputSubVolume,
                                                        inputBoundingBox,
                                                        "gehc-signa-genesis2" );

      if ( format != "gehc-signa-genesis2" )
      {

        throw std::runtime_error(
                             "wrong format, should be \'gehc-signa-genesis2\'");

      }

      // saving sub-volume(s) to disk
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< T > >(
                                                outputVolumeProxy,
                                                inputSubVolume,
                                                outputBoundingBox,
                                                readProcess.getOutputFormat() );

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void GehcGenesis2ReadProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// classGehcSignaGenesis22GisConverterCommand
//

gkg::GehcSignaGenesis22GisConverterCommand::
                                          GehcSignaGenesis22GisConverterCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                           : gkg::Command( argc, argv,
                                                           loadPlugin,
                                                           removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GehcSignaGenesis22GisConverterCommand::"
             "GehcSignaGenesis22GisConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaGenesis22GisConverterCommand::
                                          GehcSignaGenesis22GisConverterCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                bool verbose )
                                           : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::GehcSignaGenesis22GisConverterCommand::"
             "GehcSignaGenesis22GisConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::GehcSignaGenesis22GisConverterCommand::
                                          GehcSignaGenesis22GisConverterCommand(
                                             const gkg::Dictionary& parameters )
                                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::GehcSignaGenesis22GisConverterCommand::"
             "GehcSignaGenesis22GisConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaGenesis22GisConverterCommand::
                                        ~GehcSignaGenesis22GisConverterCommand()
{
}


std::string gkg::GehcSignaGenesis22GisConverterCommand::getStaticName()
{

  try
  {

    return "GehcSignaGenesis22GisConverter";

  }
  GKG_CATCH( "std::string "
             "gkg::GehcSignaGenesis22GisConverterCommand::getStaticName()" );

}


void gkg::GehcSignaGenesis22GisConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "GEHC Signa Genesis to "
                                  "GIS disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input GEHC Signa Genesis 2 directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "GIS image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::GehcSignaGenesis22GisConverterCommand::parse()" );

}


void gkg::GehcSignaGenesis22GisConverterCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                bool verbose )
{

  try
  {

    GehcGenesis2ReadProcess 
      gehcGenesis2ReadProcess( fileNameOut, outputFormat, verbose );
    gehcGenesis2ReadProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::GehcSignaGenesis22GisConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    GehcSignaGenesis22GisConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
