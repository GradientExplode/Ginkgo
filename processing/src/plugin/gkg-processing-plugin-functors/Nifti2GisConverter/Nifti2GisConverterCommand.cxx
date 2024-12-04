#include <gkg-processing-plugin-functors/Nifti2GisConverter/Nifti2GisConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
// class NiftiReadProcess
//


class NiftiReadProcess : public gkg::Process
{

  public:

    NiftiReadProcess( const std::string& fileNameOut,
                      const std::string& outputFormat,
                      bool ascii,
                      bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    bool getAscii() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    bool _ascii;
    bool _verbose;

};


NiftiReadProcess::NiftiReadProcess( const std::string& fileNameOut,
                                    const std::string& outputFormat,
                                    bool ascii,
                               bool verbose )
                : gkg::Process( "Volume" ),
                  _fileNameOut( fileNameOut ),
                  _outputFormat( outputFormat ),
                  _ascii( ascii ),
                  _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &NiftiReadProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &NiftiReadProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &NiftiReadProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &NiftiReadProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &NiftiReadProcess::read< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &NiftiReadProcess::read< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &NiftiReadProcess::read< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &NiftiReadProcess::read< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &NiftiReadProcess::read< gkg::RGBComponent > );

}


const std::string& NiftiReadProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& NiftiReadProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool NiftiReadProcess::getAscii() const
{

  return _ascii;

}


bool NiftiReadProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void NiftiReadProcess::read( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    NiftiReadProcess&
      readProcess = static_cast< NiftiReadProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( readProcess.getVerbose() )
    {

      std::cout << "reading Nifti data "
                << fileNameIn << ": "
                << std::flush;

    }
    std::string format;
    gkg::Volume< T > volume;
    format = gkg::Reader::getInstance().template read< gkg::Volume< T > >(
                                                                     fileNameIn,
                                                                     volume,
                                                                     "nifti" );

    if ( format != "nifti" )
    {

      throw std::runtime_error( "wrong format, should be \'nifti\'");

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // preparing partial write proxy
    if ( readProcess.getVerbose() )
    {

      std::cout << "writing volume in \'"
                << readProcess.getOutputFormat()
                << "\' format: "
                << std::flush;

    }
    gkg::Writer::getInstance().template write< gkg::Volume< T > >(
                                                readProcess.getFileNameOut(),
                                                volume,
                                                readProcess.getAscii(),
                                                readProcess.getOutputFormat() );
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void NiftiReadProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class Nifti2GisConverterCommand
//


gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand(
                                		const std::string& fileNameIn,
                                		const std::string& fileNameOut,
                                		const std::string& outputFormat,
                                		bool ascii,
                                		bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH( "gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool ascii, bool verbose )" );

}


gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH( "gkg::Nifti2GisConverterCommand::Nifti2GisConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Nifti2GisConverterCommand::~Nifti2GisConverterCommand()
{
}


std::string gkg::Nifti2GisConverterCommand::getStaticName()
{

  try
  {

    return "Nifti2GisConverter";

  }
  GKG_CATCH( "std::string gkg::Nifti2GisConverterCommand::getStaticName()" );

}


void gkg::Nifti2GisConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "NIFTI to GIS disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input NIFTI file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
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

    execute( fileNameIn, fileNameOut, outputFormat, ascii, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Nifti2GisConverterCommand::parse()" );

}


void gkg::Nifti2GisConverterCommand::execute( const std::string& fileNameIn,
                                	      const std::string& fileNameOut,
                                	      const std::string& outputFormat,
                                	      bool ascii,
                                	      bool verbose )
{

  try
  {

    NiftiReadProcess readProcess( fileNameOut, outputFormat, ascii, verbose );
    readProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Nifti2GisConverterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool ascii, bool verbose )" );

}


RegisterCommandCreator( Nifti2GisConverterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
