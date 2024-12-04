#include <gkg-processing-plugin-functors/Minf2SomaConverter/Minf2SomaConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-io/GisDiskFormat_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   MinfConverterProcess
//


class MinfConverterProcess : public gkg::Process
{

  public:

    MinfConverterProcess( bool verbose );

    bool getVerbose() const;

  private:

    template < class T >
    static void convert( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    bool _verbose;

};


MinfConverterProcess::MinfConverterProcess( bool verbose )
                    : gkg::Process(),
                      _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MinfConverterProcess::convert< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MinfConverterProcess::convert< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MinfConverterProcess::convert< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MinfConverterProcess::convert< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MinfConverterProcess::convert< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MinfConverterProcess::convert< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MinfConverterProcess::convert< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MinfConverterProcess::convert< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MinfConverterProcess::convert< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MinfConverterProcess::convert< double > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                   &MinfConverterProcess::convert< std::complex< float > > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                   &MinfConverterProcess::convert< std::complex< double > > );

}


bool MinfConverterProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void MinfConverterProcess::convert( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    MinfConverterProcess&
      converterProcess = static_cast< MinfConverterProcess& >( process );

    // first try to get header information
    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "reading '" << fileNameIn << "' proxy : " << std::flush;
    
    }
    std::string format;
    gkg::Volume< T > volumeProxy;
    format = gkg::Reader::getInstance().readHeader( fileNameIn, volumeProxy );
    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "done" << std::endl;
    
    }

    // adding SOMA fields for compatibility
    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "adding 'volume_dimension' field" << std::flush;
    
    }
    std::vector< int32_t > volume_dimension( 4 );
    volume_dimension[ 0 ] = volumeProxy.getSizeX();
    volume_dimension[ 1 ] = volumeProxy.getSizeY();
    volume_dimension[ 2 ] = volumeProxy.getSizeZ();
    volume_dimension[ 3 ] = volumeProxy.getSizeT();

    volumeProxy.getHeader().addAttribute( "volume_dimension",
                                          volume_dimension );

    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "done" << std::endl;
    
    }

    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "adding 'voxel_size' field" << std::flush;
    
    }
    std::vector< double > voxel_size( 4, 1.0 );
    
    if ( volumeProxy.getHeader().hasAttribute( "resolutionX" ) )
    {
    
      volumeProxy.getHeader().getAttribute( "resolutionX", voxel_size[ 0 ] );
    
    }
    if ( volumeProxy.getHeader().hasAttribute( "resolutionY" ) )
    {
    
      volumeProxy.getHeader().getAttribute( "resolutionY", voxel_size[ 1 ] );
    
    }
    if ( volumeProxy.getHeader().hasAttribute( "resolutionZ" ) )
    {
    
      volumeProxy.getHeader().getAttribute( "resolutionZ", voxel_size[ 2 ] );
    
    }
    if ( volumeProxy.getHeader().hasAttribute( "resolutionT" ) )
    {
    
      volumeProxy.getHeader().getAttribute( "resolutionT", voxel_size[ 3 ] );
    
    }

    volumeProxy.getHeader().addAttribute( "voxel_size",
                                          voxel_size );

    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "done" << std::endl;
    
    }

    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "writing '" << fileNameIn << "' proxy : " << std::flush;
    
    }
    gkg::GisDiskFormat< T >::getInstance().writeHeader( fileNameIn,
                                                        volumeProxy );

    if ( converterProcess.getVerbose() )
    {
    
      std::cout << "done" << std::endl;
    
    }
  
  }
  GKG_CATCH( "template < class T > "
             "void MinfConverterProcess::convert( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   Minf2SomaConverterCommand
//


gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand( 
                                                  const std::string& fileNameIn,
                                                  bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, verbose );

  }
  GKG_CATCH( "gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand( "
             "const std::string& fileNameIn, bool verbose )" );

}


gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, verbose );

  }
  GKG_CATCH( "gkg::Minf2SomaConverterCommand::Minf2SomaConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Minf2SomaConverterCommand::~Minf2SomaConverterCommand()
{
}


std::string gkg::Minf2SomaConverterCommand::getStaticName()
{

  try
  {

    return "Minf2SomaConverter";

  }
  GKG_CATCH( "std::string gkg::Minf2SomaConverterCommand::getStaticName()" );

}


void gkg::Minf2SomaConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    bool verbose = false;

    gkg::Application application( 
                                 _argc, _argv,
                                 "Compatibility converter for GKG/SOMA volumes",
                                 _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Minf2SomaConverterCommand::parse()" );

}


void gkg::Minf2SomaConverterCommand::execute( const std::string& fileNameIn,
                                              bool verbose )
{

  try
  {

    MinfConverterProcess converterProcess( verbose );
    converterProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Minf2SomaConverterCommand::execute( "
             "const std::string& fileNameIn, bool verbose )" );

}


RegisterCommandCreator( Minf2SomaConverterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
