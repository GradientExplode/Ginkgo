#include <gkg-processing-plugin-functors/Gis2NiftiConverter/Gis2NiftiConverterCommand.h>
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
// class Gis2NiftiProcess
//


class Gis2NiftiProcess : public gkg::Process
{

  public:

    Gis2NiftiProcess( const std::string& fileNameOut,
                      bool verbose );

    const std::string& getFileNameOut() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    bool _verbose;

};


Gis2NiftiProcess::Gis2NiftiProcess( const std::string& fileNameOut,
                                    bool verbose )
                : gkg::Process( "Volume" ),
                  _fileNameOut( fileNameOut ),
                  _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &Gis2NiftiProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &Gis2NiftiProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &Gis2NiftiProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &Gis2NiftiProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &Gis2NiftiProcess::read< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &Gis2NiftiProcess::read< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &Gis2NiftiProcess::read< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &Gis2NiftiProcess::read< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &Gis2NiftiProcess::read< gkg::RGBComponent > );

}


const std::string& Gis2NiftiProcess::getFileNameOut() const
{

  return _fileNameOut;

}


bool Gis2NiftiProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void Gis2NiftiProcess::read( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    Gis2NiftiProcess&
      readProcess = static_cast< Gis2NiftiProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( readProcess.getVerbose() )
    {

      std::cout << "reading Gis data "
                << fileNameIn << ": "
                << std::flush;

    }
    std::string format;
    gkg::Volume< T > volume;
    format = gkg::Reader::getInstance().template read< gkg::Volume< T > >(
                                                                     fileNameIn,
                                                                     volume,
                                                                     "gis" );

    if ( format != "gis" )
    {

      throw std::runtime_error( "wrong format, should be \'gis\'");

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // preparing partial write proxy
    if ( readProcess.getVerbose() )
    {

      std::cout << "writing Nifti volume " << std::flush;

    }
    gkg::Writer::getInstance().template write< gkg::Volume< T > >(
                                              readProcess.getFileNameOut(),
                                              volume,
                                              false,
                                              "nifti" );
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void Gis2NiftiProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   Gis2NiftiConverterCommand
//


gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::Gis2NiftiConverterCommand::Gis2NiftiConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Gis2NiftiConverterCommand::~Gis2NiftiConverterCommand()
{
}


std::string gkg::Gis2NiftiConverterCommand::getStaticName()
{

  try
  {

    return "Gis2NiftiConverter";

  }
  GKG_CATCH( "std::string gkg::Gis2NiftiConverterCommand::getStaticName()" );

}


void gkg::Gis2NiftiConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "GIS to NIFTI disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input GIS file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output image file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Gis2NiftiConverterCommand::parse()" );

}


void gkg::Gis2NiftiConverterCommand::execute( const std::string& fileNameIn,
                                              const std::string& fileNameOut,
                                              bool verbose )
{

  try
  {

    Gis2NiftiProcess readProcess( fileNameOut, verbose );
    readProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Gis2NiftiConverterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( Gis2NiftiConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
