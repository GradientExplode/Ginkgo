#include <gkg-mri-reconstruction-plugin-functors/Anonymizer/AnonymizerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-processing-io/GisDiskFormat_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <complex>


//
// class AnonymizationProcess
//

class AnonymizationProcess : public gkg::Process
{

  public:

    AnonymizationProcess( const std::string& anonymousName,
                          const bool& verbose );

    const std::string& getAnonymousName() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void anonymize( gkg::Process& process,
                           const std::string& fileName,
                           const gkg::AnalyzedObject&,
                           const std::string& );

    const std::string& _anonymousName;
    const bool& _verbose;

};


const std::string& AnonymizationProcess::getAnonymousName() const
{

  return _anonymousName;

}


const bool& AnonymizationProcess::getVerbose() const
{

  return _verbose;

}


AnonymizationProcess::AnonymizationProcess( const std::string& anonymousName,
                                            const bool& verbose )
                     : gkg::Process(),
                       _anonymousName( anonymousName ),
                       _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &AnonymizationProcess::anonymize< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &AnonymizationProcess::anonymize< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &AnonymizationProcess::anonymize< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &AnonymizationProcess::anonymize< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &AnonymizationProcess::anonymize< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &AnonymizationProcess::anonymize< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &AnonymizationProcess::anonymize< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &AnonymizationProcess::anonymize< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &AnonymizationProcess::anonymize< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &AnonymizationProcess::anonymize< double > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< float > >::getName(),
                   &AnonymizationProcess::anonymize< std::complex< float > > );
  registerProcess( "Volume", gkg::TypeOf< std::complex< double > >::getName(),
                   &AnonymizationProcess::anonymize< std::complex< double > > );

}


template < class T >
void AnonymizationProcess::anonymize( gkg::Process& process,
                                      const std::string& fileName,
                                      const gkg::AnalyzedObject&,
                                      const std::string& )
{

  try
  {

    AnonymizationProcess&
      anonymizationProcess = static_cast< AnonymizationProcess& >( process );

    // reading data
    if ( anonymizationProcess.getVerbose() )
    {

      std::cout << "reading '" << fileName << "' : " << std::flush;

    }

    std::string format;
    gkg::VolumeProxy< T > proxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                      gkg::Volume< T > > (
                                                          fileName, proxy );

    if ( format != "gis" )
    {

      throw std::runtime_error(
                              "data to be anonymized must be in 'gis' format" );

    }

    std::string headerFileName =
      gkg::GisDiskFormat< T >::getInstance().getNameWithoutExtension(
                                                             fileName ) +
      gkg::GisDiskFormat< T >::getInstance().getHeaderExtension();

    if ( anonymizationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( anonymizationProcess.getVerbose() )
    {

      std::cout << "anonymizing : " << std::flush;

    }

    // BRUKER 
    if ( proxy.getHeader().hasAttribute( "PULPROG" ) )
    {

      // patient name belongs to subject file that is not used in Bruker
      // raw or reco disk format
      if ( anonymizationProcess.getVerbose() )
      {

        std::cout << "N/A" << std::flush;

      }

    }
    // GEHC PFile
    else if ( proxy.getHeader().hasAttribute(
                                          "pool_header.rdb_hdr_exam.patname" ) )
    {

      std::string name;
      proxy.getHeader().getAttribute( "pool_header.rdb_hdr_exam.patname",
                                      name );

      proxy.getHeader()[ "pool_header.rdb_hdr_exam.patname" ] = 
        anonymizationProcess.getAnonymousName();
      proxy.getHeader()[ "pool_header.rdb_hdr_exam.patnameff" ] = 
        anonymizationProcess.getAnonymousName();

      if ( anonymizationProcess.getVerbose() )
      {

        std::cout << name << " -> " << anonymizationProcess.getAnonymousName()
                  << std::flush;

      }

    }
    // GEHC Genesis
    else if ( proxy.getHeader().hasAttribute(
                                       "genesis_header.rdb_hdr_exam.patname" ) )
    {

      std::string name;
      proxy.getHeader().getAttribute( "genesis_header.rdb_hdr_exam.patname",
                                      name );

      proxy.getHeader()[ "genesis_header.rdb_hdr_exam.patname" ] = 
        anonymizationProcess.getAnonymousName();

      if ( anonymizationProcess.getVerbose() )
      {

        std::cout << name << " -> " << anonymizationProcess.getAnonymousName()
                  << std::flush;

      }

    }
    // DICOM
    else if ( proxy.getHeader().hasAttribute( "(0010,0010)" ) )
    {

      std::string name;
      proxy.getHeader().getAttribute( "(0010,0010)",
                                      name );

      proxy.getHeader()[ "(0010,0010)" ] = 
        anonymizationProcess.getAnonymousName();

      if ( anonymizationProcess.getVerbose() )
      {

        std::cout << name << " -> " << anonymizationProcess.getAnonymousName()
                  << std::flush;

      }

    }

    if ( anonymizationProcess.getVerbose() )
    {

      std::cout << std::endl;

    }

    if ( anonymizationProcess.getVerbose() )
    {

      std::cout << "modifying header : " << std::flush;

    }

    std::ofstream os( headerFileName.c_str() );
    gkg::BaseObjectWriter baseObjectWriter;
    baseObjectWriter.write( os, proxy.getHeader() );
    os.close();


  }
  GKG_CATCH( "template < class T > "
             "void AnonymizationProcess::anonymize( gkg::Process& process, "
             "const std::string& fileName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}



//
// class AnonymizerCommand
//

gkg::AnonymizerCommand::AnonymizerCommand( int32_t argc,
                                           char* argv[],
                                           bool loadPlugin,
                                           bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::AnonymizerCommand::AnonymizerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AnonymizerCommand::AnonymizerCommand( const std::string& fileName,
                                           const std::string& anonymousName,
                                           bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileName,
             anonymousName,
             verbose );

  }
  GKG_CATCH( "gkg::AnonymizerCommand::AnonymizerCommand( "
             "const std::string& fileName, "
             "const std::string& anonymousName, "
             "bool verbose  )" );

}


gkg::AnonymizerCommand::AnonymizerCommand( const gkg::Dictionary& parameters )
                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, anonymousName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileName,
             anonymousName,
             verbose );

  }
  GKG_CATCH( "gkg::AnonymizerCommand::"
             "AnonymizerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AnonymizerCommand::~AnonymizerCommand()
{
}


std::string gkg::AnonymizerCommand::getStaticName()
{

  try
  {

    return "Anonymizer";

  }
  GKG_CATCH( "std::string gkg::AnonymizerCommand::getStaticName()" );

}


void gkg::AnonymizerCommand::parse()
{

  try
  {

    std::string fileName;
    std::string anonymousName = "anonymous";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Anonymization of data by removing "
                                  "any patient/volunteer name",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input data",
                                 fileName );
    application.addSingleOption( "-a",
                                 "Anonymous name",
                                 anonymousName,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();


    execute( fileName,
             anonymousName,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AnonymizerCommand::parse()" );

}


void gkg::AnonymizerCommand::execute( const std::string& fileName,
                                      const std::string& anonymousName,
                                      bool verbose )
{

  try
  {

    //
    // launching process
    //
    AnonymizationProcess anonymizationProcess( anonymousName, verbose );
    anonymizationProcess.execute( fileName );

  }
  GKG_CATCH( "void gkg::AnonymizerCommand::execute( "
             "const std::string& fileName, "
             "const std::string& anonymousName, "
             "bool verbose  )" );

}


RegisterCommandCreator(
    AnonymizerCommand,
    DECLARE_STRING_PARAMETER_HELP( fileName ) +
    DECLARE_STRING_PARAMETER_HELP( anonymousName ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
