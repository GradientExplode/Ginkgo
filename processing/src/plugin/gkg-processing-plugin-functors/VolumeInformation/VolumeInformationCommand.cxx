#include <gkg-processing-plugin-functors/VolumeInformation/VolumeInformationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class VolumeInformationProcess : public gkg::Process
{

  public:

    VolumeInformationProcess( const std::string& information,
                              const bool& verbose );

    const std::string& getInformation() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void extractInformation( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& );

    const std::string& _information;
    const bool& _verbose;

};


VolumeInformationProcess::VolumeInformationProcess(
                                                 const std::string& information,
                                                 const bool& verbose)
                         : gkg::Process( "Volume" ),
                           _information( information ),
                           _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &VolumeInformationProcess::extractInformation< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &VolumeInformationProcess::extractInformation< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &VolumeInformationProcess::extractInformation< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &VolumeInformationProcess::extractInformation< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &VolumeInformationProcess::extractInformation< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &VolumeInformationProcess::extractInformation< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &VolumeInformationProcess::extractInformation< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &VolumeInformationProcess::extractInformation< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &VolumeInformationProcess::extractInformation< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &VolumeInformationProcess::extractInformation< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &VolumeInformationProcess::extractInformation< 
                                                          gkg::RGBComponent > );

}


const std::string& VolumeInformationProcess::getInformation() const
{

  return _information;

}


const bool& VolumeInformationProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void VolumeInformationProcess::extractInformation(
                                                  gkg::Process& process,
                                                  const std::string& fileNameIn,
                                                  const gkg::AnalyzedObject&,
                                                  const std::string& )
{

  try
  {

    VolumeInformationProcess&
      extractInformationProcess = static_cast< VolumeInformationProcess& >(
                                                                      process );

    // reading data
    if ( extractInformationProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }

    std::string format;
    gkg::VolumeProxy< T > volumeProxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                              gkg::Volume< T > >( fileNameIn,
                                                                  volumeProxy );

    if ( extractInformationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( extractInformationProcess.getInformation() == "disk_format" )
    {

      std::cout << format << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "item_type" )
    {

      std::cout << gkg::TypeOf< T >::getName() << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "sizeX" )
    {

      std::cout << volumeProxy.getSizeX() << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "sizeY" )
    {

      std::cout << volumeProxy.getSizeY() << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "sizeZ" )
    {

      std::cout << volumeProxy.getSizeZ() << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "sizeT" )
    {

      std::cout << volumeProxy.getSizeT() << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "resolutionX" )
    {

      gkg::Vector3d< double > resolution;
      volumeProxy.getResolution( resolution );
      std::cout << resolution.x << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "resolutionY" )
    {

      gkg::Vector3d< double > resolution;
      volumeProxy.getResolution( resolution );
      std::cout << resolution.y << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "resolutionZ" )
    {

      gkg::Vector3d< double > resolution;
      volumeProxy.getResolution( resolution );
      std::cout << resolution.z << std::endl;

    }
    else if ( extractInformationProcess.getInformation() == "resolutionT" )
    {

      gkg::Vector3d< double > resolution;
      double resolutionT = 1.0;
      volumeProxy.getResolution( resolution, resolutionT );
      std::cout << resolutionT << std::endl;

    }


  }
  GKG_CATCH( "template < class T > "
             "void VolumeInformationProcess::extractInformation( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeInformationCommand
//



gkg::VolumeInformationCommand::VolumeInformationCommand( int32_t argc,
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
  GKG_CATCH( "gkg::VolumeInformationCommand::VolumeInformationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeInformationCommand::VolumeInformationCommand(
                                     		 const std::string& fileNameIn,
                                      		 const std::string& information,
                                     		 bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, information, verbose );

  }
  GKG_CATCH( "gkg::VolumeInformationCommand::VolumeInformationCommand( "
             "const std::string& fileNameIn, const std::string& information, "
             "bool verbose )" );

}


gkg::VolumeInformationCommand::VolumeInformationCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, information );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, information, verbose );

  }
  GKG_CATCH( "gkg::VolumeInformationCommand::VolumeInformationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeInformationCommand::~VolumeInformationCommand()
{
}


std::string gkg::VolumeInformationCommand::getStaticName()
{

  try
  {

    return "VolumeInformation";

  }
  GKG_CATCH( "std::string gkg::VolumeInformationCommand::getStaticName()" );

}


void gkg::VolumeInformationCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string information;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool to extract volume information",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-info",
                                 "Information\n"
                                 " - disk_format\n"
                                 " - item_type\n"
                                 " - sizeX\n"
                                 " - sizeY\n"
                                 " - sizeZ\n"
                                 " - sizeT\n"
                                 " - resolutionX\n"
                                 " - resolutionY\n"
                                 " - resolutionZ\n"
                                 " - resolutionT\n",
                                 information );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, information, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeInformationCommand::parse()" );

}


void gkg::VolumeInformationCommand::execute( const std::string& fileNameIn,
                                      	     const std::string& information,
                                     	     bool verbose )
{

  try
  {

    if ( ( information != "disk_format" ) &&
         ( information != "item_type" ) &&
         ( information != "sizeX" ) &&
         ( information != "sizeY" ) &&
         ( information != "sizeZ" ) &&
         ( information != "sizeT" ) &&
         ( information != "resolutionX" ) &&
         ( information != "resolutionY" ) &&
         ( information != "resolutionZ" ) &&
         ( information != "resolutionT" ) )
    {

      throw std::runtime_error( "invalid information" );

    }

    //
    // launching process
    //
    VolumeInformationProcess volumeInformationProcess( information, verbose );
    volumeInformationProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::VolumeInformationCommand::execute( "
             "const std::string& fileNameIn, const std::string& information, "
             "bool verbose )" );

}


RegisterCommandCreator( VolumeInformationCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +  
    			DECLARE_STRING_PARAMETER_HELP( information ) + 
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
