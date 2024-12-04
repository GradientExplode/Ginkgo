#include <gkg-mri-reconstruction-plugin-functors/DiskFormatConverter/DiskFormatConverterCommand.h>
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
// class DiskFormatConverterProcess
//

class DiskFormatConverterProcess : public gkg::Process
{

  public:

    DiskFormatConverterProcess( const std::string& fileNameOut,
                                const std::string& inputFormat,
                                const std::string& outputFormat,
                                bool ascii,
                                bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getInputFormat() const;
    const std::string& getOutputFormat() const;
    bool getAscii() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void convertVolume( gkg::Process& process,
                               const std::string& fileNameIn,
                               const gkg::AnalyzedObject&,
                               const std::string& );

    const std::string& _fileNameOut;
    const std::string& _inputFormat;
    const std::string& _outputFormat;
    bool _ascii;
    bool _verbose;

};


DiskFormatConverterProcess::DiskFormatConverterProcess(
                          const std::string& fileNameOut,
                          const std::string& inputFormat,
                          const std::string& outputFormat,
                          bool ascii,
                          bool verbose )
                           : gkg::Process( "" ),
                             _fileNameOut( fileNameOut ),
                             _inputFormat( inputFormat ),
                             _outputFormat( outputFormat ),
                             _ascii( ascii ),
                             _verbose( verbose )
{

  // Volume
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< int8_t > );

  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< int16_t > );

  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< int32_t > );

  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &DiskFormatConverterProcess::convertVolume< int64_t > );

  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &DiskFormatConverterProcess::convertVolume< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &DiskFormatConverterProcess::convertVolume< double > );


}


const std::string& DiskFormatConverterProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& DiskFormatConverterProcess::getInputFormat() const
{

  return _inputFormat;

}


const std::string& DiskFormatConverterProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool DiskFormatConverterProcess::getAscii() const
{

  return _ascii;

}


bool DiskFormatConverterProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void DiskFormatConverterProcess::convertVolume( gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    DiskFormatConverterProcess& diskFormatConverterProcess =
      static_cast< DiskFormatConverterProcess& >( process );

    if ( diskFormatConverterProcess.getVerbose() )
    {

      std::cout << "reading input file '"
                << fileNameIn << "' : " 
                << std::flush;

    }
    gkg::Volume< T > inputVolume;
    std::string inputFormat =
      gkg::Reader::getInstance().read(
                                 fileNameIn,
                                 inputVolume,
                                 diskFormatConverterProcess.getInputFormat() );

    if ( !diskFormatConverterProcess.getInputFormat().empty() &&
         ( inputFormat != diskFormatConverterProcess.getInputFormat() ) )
    {

      throw std::runtime_error( std::string( "wrong format, should be '" ) +
                                diskFormatConverterProcess.getInputFormat() +
                                "' , while inputFormat format is " +
                                inputFormat );

    }

    if ( diskFormatConverterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    std::string fileNameOut = diskFormatConverterProcess.getFileNameOut();
    if ( fileNameOut.empty() )
    {

      fileNameOut = fileNameIn;

    }

    if ( diskFormatConverterProcess.getVerbose() )
    {

      std::cout << "writing output file '"
                << fileNameOut << "' : " 
                << std::flush;

    }

    std::string outputFormat =
      gkg::Writer::getInstance().write(
                                fileNameOut,
                                inputVolume,
                                diskFormatConverterProcess.getAscii(),
                                diskFormatConverterProcess.getOutputFormat() );

    if ( !diskFormatConverterProcess.getOutputFormat().empty() &&
         ( outputFormat != diskFormatConverterProcess.getOutputFormat() ) )
    {

      throw std::runtime_error( std::string( "wrong format, should be '" ) +
                                diskFormatConverterProcess.getOutputFormat() +
                                "' , while outputFormat format is " +
                                outputFormat );

    }

    if ( diskFormatConverterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DiskFormatConverterProcess::convertVolume( "
             "gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   DiskFormatConverterCommand
//


gkg::DiskFormatConverterCommand::DiskFormatConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DiskFormatConverterCommand::DiskFormatConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DiskFormatConverterCommand::DiskFormatConverterCommand(
                                		const std::string& fileNameIn,
                                		const std::string& fileNameOut,
                                		const std::string& inputFormat,
                                		const std::string& outputFormat,
                                		bool ascii,
                                		bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, inputFormat, outputFormat, ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DiskFormatConverterCommand::DiskFormatConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& inputFormat, "
             "const std::string& outputFormat, "
             "bool ascii, bool verbose )" );

}


gkg::DiskFormatConverterCommand::DiskFormatConverterCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, inputFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, inputFormat, outputFormat, ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DiskFormatConverterCommand::DiskFormatConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DiskFormatConverterCommand::~DiskFormatConverterCommand()
{
}


std::string gkg::DiskFormatConverterCommand::getStaticName()
{

  try
  {

    return "DiskFormatConverter";

  }
  GKG_CATCH( "std::string gkg::DiskFormatConverterCommand::getStaticName()" );

}


void gkg::DiskFormatConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";
    std::string inputFormat = "";
    std::string outputFormat = "";
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file or directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file or directory name"
                                 " (default=input file name)",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-if",
                                 "Input format name (default="")",
                                 inputFormat,
                                 true );
    application.addSingleOption( "-of",
                                 "Output format name (default=extension based)",
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

    execute( fileNameIn, fileNameOut, inputFormat, outputFormat, ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DiskFormatConverterCommand::parse()" );

}


void gkg::DiskFormatConverterCommand::execute( const std::string& fileNameIn,
                                	       const std::string& fileNameOut,
                                	       const std::string& inputFormat,
                                	       const std::string& outputFormat,
                                	       bool ascii,
                                	       bool verbose )
{

  try
  {

    DiskFormatConverterProcess diskFormatConverterProcess( fileNameOut,
                                                           inputFormat,
                                                           outputFormat,
                                                           ascii,
                                                           verbose );
    diskFormatConverterProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::DiskFormatConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& inputFormat, "
             "const std::string& outputFormat, "
             "bool ascii, bool verbose )" );

}


RegisterCommandCreator( DiskFormatConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( inputFormat ) +
                        DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
