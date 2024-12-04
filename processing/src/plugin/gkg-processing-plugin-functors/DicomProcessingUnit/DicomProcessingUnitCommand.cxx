#include <gkg-processing-plugin-functors/DicomProcessingUnit/DicomProcessingUnitCommand.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DicomProcessingUnit.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-io/DicomServiceClassProvider.h>
#include <gkg-core-io/DicomServiceClassUser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>


// 
// /usr/local/dcmtk/bin/storescp --prefer-lossless 
//                               --exec-on-eostudy script.sh
//                               --sort-conc-studies toto
//                               --eostudy-timeout 5
//                               1024
// 
// /usr/local/dcmtk/bin/storescu --propose-lossless 
//                               132.166.143.186
//                               1024
//                               * 
// 


//
// static variables
//

static std::string tmpDirectory = "/tmp";
static std::string hostName = "station3t1";


//
// converting string to DICOM transfer syntax
//

gkg::DicomServiceClassProvider::TransferSyntax 
getDicomDpuTransferSyntax( const std::string& transferSyntax )
{

  try
  {

    gkg::DicomServiceClassProvider::TransferSyntax dicomTransferSyntax;

    if ( transferSyntax == "uncompressed" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Uncompressed;

    }
    else if ( transferSyntax == "little" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Little;

    }
    else if ( transferSyntax == "big" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Big;

    }
    else if ( transferSyntax == "lossless" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Lossless;

    }
    else if ( transferSyntax == "jpeg8" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Jpeg8;

    }
    else if ( transferSyntax == "jpeg12" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Jpeg12;

    }
    else if ( transferSyntax == "j2k-lossless" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::J2kLossless;

    }
    else if ( transferSyntax == "j2k-lossy" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::J2kLossy;

    }
    else if ( transferSyntax == "rle" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Rle;

    }
    else if ( transferSyntax == "deflated" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Deflated;

    }
    else if ( transferSyntax == "implicit" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassProvider::Implicit;

    }
    else
    {

      throw std::runtime_error( "wrong transfer syntax" );

    }

    return dicomTransferSyntax;

  }
  GKG_CATCH( "gkg::DicomServiceClassProvider::TransferSyntax  "
             "getDicomDpuTransferSyntax( const std::string& transferSyntax )" );

}

gkg::DicomServiceClassUser::TransferSyntax 
getDicomTransferSyntaxSCU( const std::string& transferSyntax )
{

  try
  {

    gkg::DicomServiceClassUser::TransferSyntax dicomTransferSyntax;

    if ( transferSyntax == "uncompressed" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Uncompressed;

    }
    else if ( transferSyntax == "little" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Little;

    }
    else if ( transferSyntax == "big" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Big;

    }
    else if ( transferSyntax == "lossless" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Lossless;

    }
    else if ( transferSyntax == "jpeg8" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Jpeg8;

    }
    else if ( transferSyntax == "jpeg12" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Jpeg12;

    }
    else if ( transferSyntax == "j2k-lossless" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::J2kLossless;

    }
    else if ( transferSyntax == "j2k-lossy" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::J2kLossy;

    }
    else if ( transferSyntax == "rle" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Rle;

    }
    else if ( transferSyntax == "deflated" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Deflated;

    }
    else if ( transferSyntax == "implicit" )
    {

      dicomTransferSyntax = gkg::DicomServiceClassUser::Implicit;

    }
    else
    {

      throw std::runtime_error( "wrong transfer syntax" );

    }

    return dicomTransferSyntax;

  }
  GKG_CATCH( "gkg::DicomServiceClassUser::TransferSyntax  "
             "getDicomTransferSyntaxSCU( const std::string& transferSyntax )" );

}


static void onReceptionCallback( const std::string& imageFileName,
                                 bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << imageFileName << std::endl;

    }

  }
  GKG_CATCH( "static void onReceptionCallback( "
             "const std::string& imageFileName, "
             "bool verbose )" );

}


static void onEndOfStudyCallback( const std::string& folderName,
                                  bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "======================================="
                << folderName
                << "======================================="
                << std::endl;

    }

    gkg::DicomProcessingUnit::getInstance().send( folderName );
    //gkg::Directory directory( folderName );
    //directory.remove();

  }
  GKG_CATCH( "static void onEndOfStudyCallback( "
             "const std::string& folderName, "
             "bool verbose )" );

}


//
//   DicomProcessingUnitCommand
//


gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand(
                                	    std::string& incomingDirectory,
                                	    const std::string& daemonConfigName,
                                	    int32_t port,
                                	    std::string& transferSyntax,
                                	    std::string& applicationTitle,
                                	    int32_t acseTimeout,
                                	    int32_t eosTimeout,
                                	    const std::string& processHost,
                                	    int32_t processPort,
                                	    bool killDPU,
                                	    bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( incomingDirectory, daemonConfigName, port, transferSyntax,
             applicationTitle, acseTimeout, eosTimeout, processHost,
             processPort, killDPU, verbose );

  }
  GKG_CATCH( "gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand( "
             "std::string& incomingDirectory, "
             "const std::string& daemonConfigName, int32_t port, "
             "std::string& transferSyntax, "
             "std::string& applicationTitle, int32_t acseTimeout, "
             "int32_t eosTimeout, const std::string& processHost, "
             "int32_t processPort, bool killDPU, bool verbose )" );

}


gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, incomingDirectory );
    DECLARE_STRING_PARAMETER( parameters, std::string, daemonConfigName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, port );
    DECLARE_STRING_PARAMETER( parameters, std::string, transferSyntax );
    DECLARE_STRING_PARAMETER( parameters, std::string, applicationTitle );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, acseTimeout );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, eosTimeout );
    DECLARE_STRING_PARAMETER( parameters, std::string, processHost );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, processPort );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, killDPU );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( incomingDirectory, daemonConfigName, port, transferSyntax,
             applicationTitle, acseTimeout, eosTimeout, processHost,
             processPort, killDPU, verbose );

  }
  GKG_CATCH( "gkg::DicomProcessingUnitCommand::DicomProcessingUnitCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DicomProcessingUnitCommand::~DicomProcessingUnitCommand()
{
}


std::string gkg::DicomProcessingUnitCommand::getStaticName()
{

  try
  {

    return "DicomProcessingUnit";

  }
  GKG_CATCH( "std::string gkg::DicomProcessingUnitCommand::getStaticName()" );

}


void gkg::DicomProcessingUnitCommand::parse()
{

  try
  {

    std::string incomingDirectory = ".";

    std::string daemonConfigName = "";

    int32_t port = 1024;
    std::string transferSyntax = "uncompressed";
    std::string applicationTitle = "GkgDPU";
    int32_t acseTimeout = 30;
    int32_t eosTimeout = 5;
    std::string processHost = "localhost";
    int32_t processPort = 8002;
    bool killDPU = false;

    bool verbose = false;


    gkg::Application application( _argc, _argv, "Dicom processing unit",
                                  _loadPlugin );

    application.addSingleOption( "-i",
                                 "Incoming directory (default=.)",
                                 incomingDirectory,
                                 true );
    application.addSingleOption( "-t",
                                 "Temporary work directory (default=/tmp)",
                                 tmpDirectory,
                                 true );
    application.addSingleOption( "-daemon",
                                 "Configuration file for deamon mode",
                                 daemonConfigName,
                                 true );
    application.addSingleOption( "-processHost",
                                 "TCP/IP address to send process on "
                                 "(default=localhost)",
                                 processHost,
                                 true );
    application.addSingleOption( "-processPort",
                                 "TCP/IP port number to send process on "
                                 "(default=8002)",
                                 processPort,
                                 true );
    application.addSingleOption( "-dicomPort",
                                 "TCP/IP port number to listen on "
                                 "(default=1024)",
                                 port,
                                 true );
    application.addSingleOption( "-transferSyntax",
                           "Prefered transfer syntax (default=uncompressed): \n"
                           "- uncompressed (explicit VR local byte order)\n"
                           "- implicit (implicit VR little endian only)\n"
                           "- little (explicit VR little endian)\n"
                           "- big (explicit VR big endian)\n"
                           "- lossless (default JPEG lossless)\n"
                           "- jpeg8 (default JPEG lossy for 8 bit data)\n"
                           "- jpeg12 (default JPEG lossy for 12 bit data)\n"
                           "- j2k-lossless (JPEG 2000 lossless)\n"
                           "- j2k-lossy (JPEG 2000 lossy)\n"
                           "- rle (RLE lossless)\n"
                           "- deflated (deflated expl. VR little endian)\n",
                                 transferSyntax,
                                 true );
    application.addSingleOption( "-title",
                                 "DICOM application title (default=GkgDPU)",
                                 applicationTitle,
                                 true );
    application.addSingleOption( "-acseTimeout",
                                 "Association Control Service Element timeout "
                                 "(default=30s)",
                                 acseTimeout,
                                 true );
    application.addSingleOption( "-eosTimeout",
                                 "End of study timeout (default=5s)",
                                 eosTimeout,
                                 true );                  
    application.addSingleOption( "-kill",
                                 "Kill DPU process",
                                 killDPU, 
                                 true );

    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( incomingDirectory, daemonConfigName, port, transferSyntax,
             applicationTitle, acseTimeout, eosTimeout, processHost,
             processPort, killDPU, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DicomProcessingUnitCommand::parse()" );

}


void gkg::DicomProcessingUnitCommand::execute(
                  	        	   std::string& incomingDirectory,
                                	   const std::string& daemonConfigName,
                                	   int32_t port,
                                	   std::string& transferSyntax,
                                	   std::string& applicationTitle,
                                	   int32_t acseTimeout,
                                	   int32_t eosTimeout,
                                	   const std::string& processHost,
                                	   int32_t processPort,
                                	   bool killDPU,
                                	   bool verbose )
{

  try
  {

    gkg::DicomProcessingUnit::getInstance().addWriter( processHost,
                                                       processPort );

    if ( killDPU )
    {

      // collecting transfer syntax
      gkg::DicomServiceClassUser::TransferSyntax
        dicomTransferSyntax = getDicomTransferSyntaxSCU( transferSyntax );

      // creating DICOM service class user
      gkg::DicomServiceClassUser scu( applicationTitle + "Kill",
                                      applicationTitle,
        			      "localhost",
        			      port,
        			      dicomTransferSyntax,
        			      acseTimeout );

      scu.initializeNetwork();
      scu.shutdown( verbose );
      scu.dropNetwork();

    }
    else
    {

      if  ( !daemonConfigName.empty() )
      {

        // reading configuration file
        double value;

        gkg::UncommentCounterInputFileStream is( daemonConfigName.c_str() );
        gkg::TypedObject< gkg::Dictionary > header;
        gkg::BaseObjectReader baseObjectReader;
        baseObjectReader.read( is, header );
        header.getAttribute( "incoming_directory", incomingDirectory );
        header.getAttribute( "temporary_work_directory", tmpDirectory );
        header.getAttribute( "dicom_port", value );
        port = ( int32_t )( value + 0.5 );
        header.getAttribute( "dicom_transfer_syntax", transferSyntax );
        header.getAttribute( "dicom_title", applicationTitle );
        header.getAttribute( "dicom_acse_timeout", value );
        acseTimeout = ( int32_t )( value + 0.5 );
        header.getAttribute( "dicom_eos_timeout", value );
        eosTimeout = ( int32_t )( value + 0.5 );

        // forking off the parent process
        pid_t pid = fork();
        if ( pid < 0 )
        {

          exit( EXIT_FAILURE );

        }

        // if we got a good PID, then we can exit the parent process
        if ( pid > 0 )
        {

          exit( EXIT_SUCCESS );

        }

        // changing the file mode mask
        umask(0);
                
        // opening any logs here

        // creating a new SID for the child process
        pid_t sid = setsid();
        if ( sid < 0 )
        {

          // logging the failure
          exit(EXIT_FAILURE);

        }
        
        // changing the current working directory
        if ( ( chdir( "/" ) ) < 0 )
        {

          // loggin the failure
          exit(EXIT_FAILURE);

        }
      
        // closing out the standard file descriptors
        if ( !verbose )
        {

          close( STDIN_FILENO );
          close( STDOUT_FILENO );
          close( STDERR_FILENO );

        }

        // daemon-specific initialization goes here

        // collecting transfer syntax
        gkg::DicomServiceClassProvider::TransferSyntax
          dicomTransferSyntax = getDicomDpuTransferSyntax( transferSyntax );

        // creating DICOM storage unit
        gkg::DicomServiceClassProvider
          dicomServiceClassProvider( applicationTitle,
                                     incomingDirectory,
                                     port,
                                     dicomTransferSyntax,
                                     acseTimeout,
                                     eosTimeout );

        dicomServiceClassProvider.registerOnReceptionCallback(
                                                         &onReceptionCallback );
        dicomServiceClassProvider.registerOnEndOfStudyCallback(
                                                        &onEndOfStudyCallback );

        dicomServiceClassProvider.initializeNetwork();
        dicomServiceClassProvider.run( verbose );
        dicomServiceClassProvider.dropNetwork();

        exit( EXIT_SUCCESS );      

      }
      else
      {

        // collecting transfer syntax
        gkg::DicomServiceClassProvider::TransferSyntax
          dicomTransferSyntax = getDicomDpuTransferSyntax( transferSyntax );

        // creating DICOM storage unit
        gkg::DicomServiceClassProvider
          dicomServiceClassProvider( applicationTitle,
                                     incomingDirectory,
                                     port,
                                     dicomTransferSyntax,
                                     acseTimeout,
                                     eosTimeout );

        dicomServiceClassProvider.registerOnReceptionCallback(
                                                         &onReceptionCallback );
        dicomServiceClassProvider.registerOnEndOfStudyCallback(
                                                        &onEndOfStudyCallback );

        dicomServiceClassProvider.initializeNetwork();
        dicomServiceClassProvider.run( verbose );
        dicomServiceClassProvider.dropNetwork();
        gkg::DicomProcessingUnit::getInstance().send( gkg::DPU_EXIT );

      }

    }

  }
  GKG_CATCH( "void gkg::DicomProcessingUnitCommand::execute( "
             "std::string& incomingDirectory, "
             "const std::string& daemonConfigName, int32_t port, "
             "std::string& transferSyntax, "
             "std::string& applicationTitle, int32_t acseTimeout, "
             "int32_t eosTimeout, const std::string& processHost, "
             "int32_t processPort, bool killDPU, bool verbose )" );

}


RegisterCommandCreator( DicomProcessingUnitCommand,
                        DECLARE_STRING_PARAMETER_HELP( incomingDirectory ) +
                        DECLARE_STRING_PARAMETER_HELP( daemonConfigName ) +
                        DECLARE_INTEGER_PARAMETER_HELP( port ) +
                        DECLARE_STRING_PARAMETER_HELP( transferSyntax ) +
                        DECLARE_STRING_PARAMETER_HELP( applicationTitle ) +
                        DECLARE_INTEGER_PARAMETER_HELP( acseTimeout ) +
                        DECLARE_INTEGER_PARAMETER_HELP( eosTimeout ) +
                        DECLARE_STRING_PARAMETER_HELP( processHost ) +
                        DECLARE_INTEGER_PARAMETER_HELP( processPort ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( killDPU ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
