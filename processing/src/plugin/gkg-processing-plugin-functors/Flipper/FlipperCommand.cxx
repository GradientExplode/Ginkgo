#include <gkg-processing-plugin-functors/Flipper/FlipperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-algobase/Flipper_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class FlipperProcess : public gkg::Process
{

  public:

    FlipperProcess( const std::string& fileNameOut,
                    const std::vector< std::string >& stringTypes,
                    const bool& ascii,
                    const std::string& format,
                    const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::vector< std::string >& getStringTypes() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void flip( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::vector< std::string >& _stringTypes;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


FlipperProcess::FlipperProcess( const std::string& fileNameOut,
                                const std::vector< std::string >& stringTypes,
                                const bool& ascii,
                                const std::string& format,
                                const bool& verbose )
               : gkg::Process( "Volume" ),
                 _fileNameOut( fileNameOut ),
                 _stringTypes( stringTypes ),
                 _ascii( ascii ),
                 _format( format ),
                 _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &FlipperProcess::flip< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &FlipperProcess::flip< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &FlipperProcess::flip< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &FlipperProcess::flip< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &FlipperProcess::flip< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &FlipperProcess::flip< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &FlipperProcess::flip< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &FlipperProcess::flip< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &FlipperProcess::flip< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &FlipperProcess::flip< double > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                   &FlipperProcess::flip< std::complex< float > > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                   &FlipperProcess::flip< std::complex< double > > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &FlipperProcess::flip< gkg::RGBComponent > );

}


const std::string& FlipperProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::vector< std::string >& FlipperProcess::getStringTypes() const
{

  return _stringTypes;

}


const bool& FlipperProcess::getAscii() const
{

  return _ascii;

}


const std::string& FlipperProcess::getFormat() const
{

  return _format;

}


const bool& FlipperProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void FlipperProcess::flip( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& )
{

  try
  {

    FlipperProcess&
      flipperProcess = static_cast< FlipperProcess& >( process );

    // reading data
    if ( flipperProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( flipperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( flipperProcess.getVerbose() )
    {

      std::cout << "flipping " << std::flush;

    }

    typename std::list< typename gkg::Flipper< gkg::Volume< T > >::Type > types;
    size_t t;
    for ( t = 0; t < flipperProcess.getStringTypes().size(); t++ )
    {

      std::string stringType = flipperProcess.getStringTypes()[ t ];
      if ( stringType == "x" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::SymmetryAlongX );

      }
      else if ( stringType == "y" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::SymmetryAlongY );

      }
      else if ( stringType == "z" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::SymmetryAlongZ );

      }
      else if ( stringType == "t" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::SymmetryAlongT );

      }
      else if ( stringType == "xy" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingXY );

      }
      else if ( stringType == "xz" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingXZ );

      }
      else if ( stringType == "xt" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingXT );

      }
      else if ( stringType == "yz" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingYZ );

      }
      else if ( stringType == "yt" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingYT );

      }
      else if ( stringType == "zt" )
      {

        types.push_back( gkg::Flipper< gkg::Volume< T > >::FlippingZT );

      }

      if ( flipperProcess.getVerbose() )
      {
      
        std::cout << stringType;
        if ( t != flipperProcess.getStringTypes().size() - 1 )
        {

          std::cout << "/";

        }

      }

    }

    if ( flipperProcess.getVerbose() )
    {

      std::cout << " : " << std::flush;

    }

    gkg::Flipper< gkg::Volume< T > > flipper( types );
    gkg::Volume< T > volumeOut;

    flipper.flip( volumeIn, volumeOut );

    if ( flipperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( flipperProcess.getVerbose() )
    {

      std::cout << "writing '"
                << flipperProcess.getFileNameOut() << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( flipperProcess.getFileNameOut(),
                                      volumeOut,
                                      flipperProcess.getAscii(),
                                      flipperProcess.getFormat() );
    if ( flipperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void FlipperProcess::flip( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   FlipperCommand
//


gkg::FlipperCommand::FlipperCommand( int32_t argc,
                                     char* argv[],
                                     bool loadPlugin,
                                     bool removeFirst )
                   : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::FlipperCommand::FlipperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::FlipperCommand::FlipperCommand(
                                  const std::string& fileNameIn,
                    		  const std::string& fileNameOut,
                    		  const std::vector< std::string >& stringTypes,
                    		  bool ascii,
                    		  const std::string& format,
                    		  bool verbose )
                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, stringTypes, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::FlipperCommand::FlipperCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::vector< std::string >& stringTypes, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::FlipperCommand::FlipperCommand( const gkg::Dictionary& parameters )
                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         stringTypes );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, stringTypes, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::FlipperCommand::FlipperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::FlipperCommand::~FlipperCommand()
{
}


std::string gkg::FlipperCommand::getStaticName()
{

  try
  {

    return "Flipper";

  }
  GKG_CATCH( "std::string gkg::FlipperCommand::getStaticName()" );

}


void gkg::FlipperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::vector< std::string > stringTypes;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Flipping and symmetry operator "
                                  "of container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSeriesOption( "-t",
                                 "Flipping and symmetry type list\n"
                                 " - x -> SymmetryAlongX\n"
                                 " - y -> SymmetryAlongY\n"
                                 " - z -> SymmetryAlongZ\n"
                                 " - t -> SymmetryAlongT\n"
                                 " - xy -> FlippingXY\n"
                                 " - xz -> FlippingXZ\n"
                                 " - xt -> FlippingXT\n"
                                 " - yz -> FlippingYZ\n"
                                 " - yt -> FlippingYT\n"
                                 " - zt -> FlippingZT\n",
                                 stringTypes,
                                 1 );
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

    execute( fileNameIn, fileNameOut, stringTypes, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::FlipperCommand::parse()" );

}


void gkg::FlipperCommand::execute(
                  	          const std::string& fileNameIn,
                    		  const std::string& fileNameOut,
                    		  const std::vector< std::string >& stringTypes,
                    		  bool ascii,
                    		  const std::string& format,
                    		  bool verbose )
{

  try
  {


    size_t t;
    for ( t = 0; t < stringTypes.size(); t++ )
    {

      if ( ( stringTypes[ t ] != "x" ) &&
           ( stringTypes[ t ] != "y" ) &&
           ( stringTypes[ t ] != "z" ) &&
           ( stringTypes[ t ] != "t" ) &&
           ( stringTypes[ t ] != "xy" ) &&
           ( stringTypes[ t ] != "xz" ) &&
           ( stringTypes[ t ] != "xt" ) &&
           ( stringTypes[ t ] != "yz" ) &&
           ( stringTypes[ t ] != "yt" ) &&
           ( stringTypes[ t ] != "zt" ) )
      {

        throw std::runtime_error( "invalid flipping and symmetry type list" );

      }

    }
         
    //
    // launching process
    //
    FlipperProcess flipperProcess( fileNameOut,
                                   stringTypes,
                                   ascii,
                                   format,
                                   verbose );
    flipperProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::FlipperCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::vector< std::string >& stringTypes, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                       FlipperCommand ,
                       DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringTypes ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                       DECLARE_STRING_PARAMETER_HELP( format ) +
                       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
