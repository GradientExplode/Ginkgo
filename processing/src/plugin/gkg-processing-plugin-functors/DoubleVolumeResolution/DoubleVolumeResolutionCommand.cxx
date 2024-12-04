#include <gkg-processing-plugin-functors/DoubleVolumeResolution/DoubleVolumeResolutionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class DoubleVolumeResolutionProcess : public gkg::Process
{

  public:

    DoubleVolumeResolutionProcess( const std::string& fileNameOut,
                                   const bool& ascii,
                                   const std::string& format,
                                   const bool& verbose );

    const std::string& getFileNameOut() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void process( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameOut;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


DoubleVolumeResolutionProcess::DoubleVolumeResolutionProcess(
                                                 const std::string& fileNameOut,
                                                 const bool& ascii,
                                                 const std::string& format,
                                                 const bool& verbose )
                              : gkg::Process( "Volume" ),
                                _fileNameOut( fileNameOut ),
                                _ascii( ascii ),
                                _format( format ),
                                _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &DoubleVolumeResolutionProcess::process< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &DoubleVolumeResolutionProcess::process< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &DoubleVolumeResolutionProcess::process< double > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                   &DoubleVolumeResolutionProcess::process<
                                                      std::complex< float > > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                   &DoubleVolumeResolutionProcess::process< 
                                                     std::complex< double > > );

}


const std::string& DoubleVolumeResolutionProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const bool& DoubleVolumeResolutionProcess::getAscii() const
{

  return _ascii;

}


const std::string& DoubleVolumeResolutionProcess::getFormat() const
{

  return _format;

}


const bool& DoubleVolumeResolutionProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void DoubleVolumeResolutionProcess::process( gkg::Process& process,
                                            const std::string& fileNameIn,
                                            const gkg::AnalyzedObject&,
                                            const std::string& )
{

  try
  {

    DoubleVolumeResolutionProcess&
      doubleVolumeResolutionProcess = 
                       static_cast< DoubleVolumeResolutionProcess& >( process );

    // reading input
    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );
    gkg::Vector3d< int32_t > inputSize;
    int32_t inputSizeT = 1;
    inputVolume.getSize( inputSize, inputSizeT );
    gkg::Vector3d< double > inputResolution;
    double inputResolutionT = 1.0;
    inputVolume.getResolution( inputResolution, inputResolutionT );

    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    
    // doubling resolution
    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "doubling resolution : " << std::flush;

    }

    gkg::Vector3d< int32_t > outputSize( inputSize * 2 );
    int32_t outputSizeT = inputSizeT;
    gkg::Vector3d< double > outputResolution( inputResolution / 2.0 );
    double outputResolutionT = inputResolutionT;

    gkg::Volume< T > outputVolume( outputSize, outputSizeT );
    outputVolume.getHeader().addAttribute( "resolutionX", outputResolution.x );
    outputVolume.getHeader().addAttribute( "resolutionY", outputResolution.y );
    outputVolume.getHeader().addAttribute( "resolutionZ", outputResolution.z );
    outputVolume.getHeader().addAttribute( "resolutionT", outputResolutionT );

    int32_t x, y, z, t;
    for ( t = 0; t < outputSizeT; t++ )
    {

      for ( z = 0; z < outputSize.z; z++ )
      {

        for ( y = 0; y < outputSize.y; y++ )
        {

          for ( x = 0; x < outputSize.x; x++ )
          {

            outputVolume( x, y, z, t ) = inputVolume( x / 2, y / 2, z / 2, t );

          }

        }

      }

    }

    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    // writing result
    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "writing '" << doubleVolumeResolutionProcess.getFileNameOut()
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write(
                                 doubleVolumeResolutionProcess.getFileNameOut(),
                                 outputVolume,
                                 doubleVolumeResolutionProcess.getAscii(),
                                 doubleVolumeResolutionProcess.getFormat() );

    if ( doubleVolumeResolutionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void DoubleVolumeResolutionProcess::process( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   DoubleVolumeResolutionCommand
//


gkg::DoubleVolumeResolutionCommand::DoubleVolumeResolutionCommand(
                                                   int32_t argc,
                                 		   char* argv[],
                                 		   bool loadPlugin,
                                 		   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DoubleVolumeResolutionCommand::DoubleVolumeResolutionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DoubleVolumeResolutionCommand::DoubleVolumeResolutionCommand(
                           		    const std::string& fileNameIn,
                           		    const std::string& fileNameOut,
                           		    bool ascii,
                           		    const std::string& format,
                           		    bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DoubleVolumeResolutionCommand::DoubleVolumeResolutionCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::DoubleVolumeResolutionCommand::DoubleVolumeResolutionCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DoubleVolumeResolutionCommand::"
             "DoubleVolumeResolutionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DoubleVolumeResolutionCommand::~DoubleVolumeResolutionCommand()
{
}


std::string gkg::DoubleVolumeResolutionCommand::getStaticName()
{

  try
  {

    return "DoubleVolumeResolution";

  }
  GKG_CATCH( "std::string "
             "gkg::DoubleVolumeResolutionCommand::getStaticName()" );

}


void gkg::DoubleVolumeResolutionCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool for double volume resolution along "
                                  "x, y, and z axis (t resolution "
                                  "is preserved)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
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

    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DoubleVolumeResolutionCommand::parse()" );

}


void gkg::DoubleVolumeResolutionCommand::execute(
                                          const std::string& fileNameIn,
                           		  const std::string& fileNameOut,
                           		  bool ascii,
                           		  const std::string& format,
                           		  bool verbose )
{

  try
  {

    //
    // launching sub-volume process
    //
    DoubleVolumeResolutionProcess doubleVolumeResolutionProcess( fileNameOut,
                                                                 ascii,
                                                                 format,
                                                                 verbose );
    doubleVolumeResolutionProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::DoubleVolumeResolutionCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( DoubleVolumeResolutionCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
