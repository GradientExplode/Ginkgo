#include <gkg-processing-plugin-functors/GehcSignaDisplayWave/GehcSignaDisplayWaveCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <cstring>


#define MAGIC_NUMBER              "IPG0"
#define REVISION_NUMBER                1

#define HEADER_DESCRIPTION_LENGTH     16  // 4 byte words
#define HEADER_LENGTH_UNUSED          48  // bytes
#define FOOTER_SIZE                    8


// information talen from psd/pgen_on_host/grad_ext_file.*

struct WaveHeader
{

  int8_t magicNumber[ 4 ];
  uint32_t headerLength;
  uint32_t revisionNumber;
  uint32_t fileLength;
  int8_t unused[ HEADER_LENGTH_UNUSED ];

};


std::vector< double > loadWave( const std::string& fileName )
{

  try
  {

    // byte swapping to be used ?
    bool bswap =( gkg::ByteOrder::getInstance().getCurrentType() ==
                  gkg::ByteOrder::LittleEndian );

    gkg::ItemReader< int8_t >& itemR_CHAR =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );
    gkg::ItemReader< uint32_t >& itemR_ULONG =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int16_t >& itemR_SHORT =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );

    // reading wave file
    std::ifstream is( fileName.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "unable to open '" ) + fileName +
                                + "' GEHC Signa wave file" );

    }

    WaveHeader header;
    itemR_CHAR.read( is, header.magicNumber, 4 );
    itemR_ULONG.read( is, header.headerLength );
    itemR_ULONG.read( is, header.revisionNumber );
    itemR_ULONG.read( is, header.fileLength );
    itemR_CHAR.read( is, header.unused, HEADER_LENGTH_UNUSED );

    if ( strncmp( (char *)header.magicNumber, MAGIC_NUMBER, 4 ) )
    {

      throw std::runtime_error( "wrong wave file magic number" );

    }

    //is.seekg( FOOTER_SIZE, std::ios_base::cur );

    size_t itemCount = ( header.fileLength - sizeof( WaveHeader ) -
                         FOOTER_SIZE ) / 2U;
    std::vector< int16_t > shortWaveData( itemCount );
    itemR_SHORT.read( is, &shortWaveData[ 0 ], itemCount );

    std::vector< double > doubleWaveData( itemCount );
    std::vector< double >::iterator d = doubleWaveData.begin(),
                                    de = doubleWaveData.end();
    std::vector< int16_t >::const_iterator s = shortWaveData.begin();
    while ( d != de )
    {

      *d = ( double )*s;
      ++ d;
      ++ s;

    }

    is.close();

    return doubleWaveData;

  }
  GKG_CATCH( "std::vector< double > loadWave( const std::string& fileName )" );

}


//
//   GehcSignaDisplayWaveCommand
//


gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin,
                                 bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand(
                                 	    const std::string& fileNameWaveForm,
                                 	    const std::string& fileNameOutTxt,
                                 	    bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameWaveForm, fileNameOutTxt, verbose );

  }
  GKG_CATCH( "gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand( "
             "const std::string& fileNameWaveForm, "
             "const std::string& fileNameOutTxt, bool verbose )" );

}


gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameWaveForm );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutTxt );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameWaveForm, fileNameOutTxt, verbose );

  }
  GKG_CATCH( "gkg::GehcSignaDisplayWaveCommand::GehcSignaDisplayWaveCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcSignaDisplayWaveCommand::~GehcSignaDisplayWaveCommand()
{
}


std::string gkg::GehcSignaDisplayWaveCommand::getStaticName()
{

  try
  {

    return "GehcSignaDisplayWave";

  }
  GKG_CATCH( "std::string gkg::GehcSignaDisplayWaveCommand::getStaticName()" );

}


void gkg::GehcSignaDisplayWaveCommand::parse()
{

  try
  {

    std::string fileNameWaveForm;
    std::string fileNameOutTxt;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Phase map estimation from double echoes"
                                  " GRE 2D complex acquisition",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "GEHC Signa waveform file "
                                 "(*.rho|*.frq|*.theta|*.mod|*.grd|*.gz)",
                                 fileNameWaveForm );
    application.addSingleOption( "-o",
                                 "Output text file name",
                                 fileNameOutTxt,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameWaveForm, fileNameOutTxt, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcSignaDisplayWaveCommand::parse()" );

}


void gkg::GehcSignaDisplayWaveCommand::execute(
                  	         	    const std::string& fileNameWaveForm,
                                 	    const std::string& fileNameOutTxt,
                                 	    bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "reading '" << fileNameWaveForm << "' : " << std::flush;

    }
    std::vector< double > wave = loadWave( fileNameWaveForm );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( !fileNameOutTxt.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving '" << fileNameOutTxt << "' : " << std::flush;
  
      }
      std::ofstream os( fileNameOutTxt.c_str() );
      if ( !os )
      {

        throw std::runtime_error( std::string( "unable to open '" ) +
                                  fileNameOutTxt + "' text file" );

      }
      std::vector< double >::const_iterator w = wave.begin(), we = wave.end();
      while ( w != we )
      {

        os << *w << std::endl;
        ++ w;

      }
      os.close();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    if ( verbose )
    {

      std::cout << "plotting '" << fileNameWaveForm << "' : " << std::flush;

    }
    gkg::CurvePlotter gnuPlotCurvePlotter;
    gnuPlotCurvePlotter.plot( wave, "time", "magnitude",
                              "GEHC Signa waveform" );

    gkg::keyboardHit();

  }
  GKG_CATCH( "void gkg::GehcSignaDisplayWaveCommand::execute( "
             "const std::string& fileNameWaveForm, "
             "const std::string& fileNameOutTxt, bool verbose )" );

}


RegisterCommandCreator( GehcSignaDisplayWaveCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameWaveForm ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOutTxt ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
