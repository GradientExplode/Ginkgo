#include <gkg-dmri-plugin-functors/DwiDiskFormatConverter/DwiDiskFormatConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
// class DiskFormatConverterProcess
//

class DwiDiskFormatConverterProcess : public gkg::Process
{

  public:

    DwiDiskFormatConverterProcess( const std::string& fileNameOut,
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
    static void convertBundleMap( gkg::Process& process,
                                  const std::string& fileNameIn,
                                  const gkg::AnalyzedObject&,
                                  const std::string& );

    const std::string& _fileNameOut;
    const std::string& _inputFormat;
    const std::string& _outputFormat;
    bool _ascii;
    bool _verbose;

};


DwiDiskFormatConverterProcess::DwiDiskFormatConverterProcess(
                             const std::string& fileNameOut,
                             const std::string& inputFormat,
                             const std::string& outputFormat,
                             bool ascii,
                             bool verbose )
                              : gkg::Process( "BundleMap" ),
                                _fileNameOut( fileNameOut ),
                                _inputFormat( inputFormat ),
                                _outputFormat( outputFormat ),
                                _ascii( ascii ),
                                _verbose( verbose )
{

  // BundleMap
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &DwiDiskFormatConverterProcess::convertBundleMap< int16_t > );
  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &DwiDiskFormatConverterProcess::convertBundleMap<
                                                                std::string > );

}


const std::string& DwiDiskFormatConverterProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& DwiDiskFormatConverterProcess::getInputFormat() const
{

  return _inputFormat;

}


const std::string& DwiDiskFormatConverterProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool DwiDiskFormatConverterProcess::getAscii() const
{

  return _ascii;

}


bool DwiDiskFormatConverterProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void DwiDiskFormatConverterProcess::convertBundleMap(
                                                gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    DwiDiskFormatConverterProcess& dwiDiskFormatConverterProcess =
      static_cast< DwiDiskFormatConverterProcess& >( process );


    if ( dwiDiskFormatConverterProcess.getVerbose() )
    {

      std::cout << "reading input file '"
                << fileNameIn << "' : " 
                << std::flush;

    }
    gkg::BundleMap< T > inputBundleMap;
    std::string inputFormat =
      gkg::Reader::getInstance().read(
                               fileNameIn,
                               inputBundleMap,
                               dwiDiskFormatConverterProcess.getInputFormat() );

    if ( ( !dwiDiskFormatConverterProcess.getInputFormat().empty() ) &&
         ( inputFormat != dwiDiskFormatConverterProcess.getInputFormat() ) )
    {


      throw std::runtime_error( std::string( "wrong format, should be '" ) +
                                dwiDiskFormatConverterProcess.getInputFormat() +
                                "' , while input format is " +
                                inputFormat );

    }

    if ( dwiDiskFormatConverterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    std::string fileNameOut = dwiDiskFormatConverterProcess.getFileNameOut();
    if ( fileNameOut.empty() )
    {

      fileNameOut = fileNameIn;

    }

    if ( dwiDiskFormatConverterProcess.getVerbose() )
    {

      std::cout << "writing output file '"
                << fileNameOut << "' : " 
                << std::flush;

    }

    std::string outputFormat =
      gkg::Writer::getInstance().write(
                              fileNameOut,
                              inputBundleMap,
                              dwiDiskFormatConverterProcess.getAscii(),
                              dwiDiskFormatConverterProcess.getOutputFormat() );

    if ( outputFormat != dwiDiskFormatConverterProcess.getOutputFormat() )
    {

      throw std::runtime_error(
                               std::string( "wrong format, should be '" ) +
                               dwiDiskFormatConverterProcess.getOutputFormat() +
                               "' , while outputFormat format is " +
                               outputFormat );

    }

    if ( dwiDiskFormatConverterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DwiDiskFormatConverterProcess::convertBundelMap( "
             "gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiDiskFormatConverterCommand
//

gkg::DwiDiskFormatConverterCommand::DwiDiskFormatConverterCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::DwiDiskFormatConverterCommand::"
             "DwiDiskFormatConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiDiskFormatConverterCommand::DwiDiskFormatConverterCommand(
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

    execute( fileNameIn,
             fileNameOut,
             inputFormat,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDiskFormatConverterCommand::"
             "DwiDiskFormatConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& inputFormat, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiDiskFormatConverterCommand::DwiDiskFormatConverterCommand(
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
    
    execute( fileNameIn,
             fileNameOut,
             inputFormat,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDiskFormatConverterCommand::"
             "DwiDiskFormatConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiDiskFormatConverterCommand::~DwiDiskFormatConverterCommand()
{
}


std::string gkg::DwiDiskFormatConverterCommand::getStaticName()
{

  try
  {

    return "DwiDiskFormatConverter";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiDiskFormatConverterCommand::getStaticName()" );

}


void gkg::DwiDiskFormatConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut = "";
    std::string inputFormat = "";
    std::string outputFormat;
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "DWI disk format converter",
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
                                 "Output format name",
                                 outputFormat );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             inputFormat,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiDiskFormatConverterCommand::parse()" );

}


void gkg::DwiDiskFormatConverterCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& inputFormat,
                                                const std::string& outputFormat,
                                                bool ascii,
                                                bool verbose )
{

  try
  {

    DwiDiskFormatConverterProcess dwiDiskFormatConverterProcess( fileNameOut,
                                                                 inputFormat,
                                                                 outputFormat,
                                                                 ascii,
                                                                 verbose );
    dwiDiskFormatConverterProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::DwiDiskFormatConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& inputFormat, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiDiskFormatConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( inputFormat ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
