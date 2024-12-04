#include <gkg-dmri-plugin-functors/DwiGFAHistogram/DwiGFAHistogramCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iostream>
#include <fstream>
#include <numeric>


#define HISTOGRAM_BIN_COUNT 1000


gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand( int32_t argc,
                                                     char* argv[],
                                                     bool loadPlugin,
                                                     bool removeFirst )
                            : gkg::Command( argc, argv, loadPlugin,
                                            removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand(
                                 const std::vector< std::string >& fileNameGFAs,
                                 const std::string& fileNameMask,
                                 const std::string& fileNameOutput,
                                 bool cumulative,
                                 bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameGFAs,
             fileNameMask,
             fileNameOutput,
             cumulative,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand( "
             "const std::vector< std::string >& fileNameGFAs, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "bool cumulative, "
             "bool verbose )" );

}


gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand(
                                             const gkg::Dictionary& parameters )
                            : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameGFAs );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, cumulative );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameGFAs,
             fileNameMask,
             fileNameOutput,
             cumulative,
             verbose );

  }
  GKG_CATCH( "gkg::DwiGFAHistogramCommand::DwiGFAHistogramCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiGFAHistogramCommand::~DwiGFAHistogramCommand()
{
}


std::string gkg::DwiGFAHistogramCommand::getStaticName()
{

  try
  {

    return "DwiGFAHistogram";

  }
  GKG_CATCH( "std::string gkg::DwiGFAHistogramCommand::getStaticName()" );

}


void gkg::DwiGFAHistogramCommand::parse()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::string > fileNameGFAs;
    std::string fileNameMask;
    std::string fileNameOutput;
    bool cumulative = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Building matplotlib histogram graphs of "
                                  "GFA",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input GFA volume file names",
                                 fileNameGFAs,
                                 1 );
    application.addSingleOption( "-m",
                                 "Mask of signal",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output matplotlib file",
                                 fileNameOutput );
    application.addSingleOption( "-cumulative",
                                 "Save histogram in cumulative mode",
                                 cumulative, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();


    execute( fileNameGFAs,
             fileNameMask,
             fileNameOutput,
             cumulative,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiGFAHistogramCommand::parse()" );

}


void gkg::DwiGFAHistogramCommand::execute(
                                 const std::vector< std::string >& fileNameGFAs,
                                 const std::string& fileNameMask,
                                 const std::string& fileNameOutput,
                                 bool cumulative,
                                 bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading signal mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading signal mask '" << fileNameMask << "' : "
                << std::flush;

    }

    gkg::Volume< int16_t > mask;
    gkg::Reader::getInstance().read( fileNameMask, mask );

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building GFA histograms
    ////////////////////////////////////////////////////////////////////////////

    int32_t fileCount = ( int32_t )fileNameGFAs.size();
    int32_t f = 0;
    float sumOfHistogramElements = 0.0;
    std::vector< std::vector< float > > histograms( fileCount );
    std::vector< std::vector< float > > cumulativeHistograms( fileCount );
    for ( f = 0; f < fileCount; f++ )
    {

      // reading GFA volume
      if ( verbose )
      {

        std::cout << "reading '" << fileNameGFAs[ f ] << "' : " << std::flush;

      }

      gkg::Volume< float > gfa;
      gkg::Reader::getInstance().read( fileNameGFAs[ f ], gfa );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // processing histogram
      if ( verbose )
      {

        std::cout << "processing histogram : " << std::flush;

      }

      histograms[ f ] = std::vector< float >( HISTOGRAM_BIN_COUNT, 0.0f );
      cumulativeHistograms[ f ] = std::vector< float >( HISTOGRAM_BIN_COUNT, 
                                                        0.0f );
      gkg::Volume< float >::const_iterator g = gfa.begin(),
                                           ge = gfa.end();
      gkg::Volume< int16_t >::const_iterator m = mask.begin();

      float count = 0.0f;
      while ( g != ge )
      {

        if ( *m )
        {

          if ( *g < 0.0f )
          {

            ++ histograms[ f ][ 0 ];

          }
          if ( *g > 1.0f )
          {

            ++ histograms[ f ][ HISTOGRAM_BIN_COUNT - 1 ];

          }
          else
          {

            ++ histograms[ f ]
                         [ ( int32_t )( *g * ( HISTOGRAM_BIN_COUNT - 1 ) ) ];

          }
          ++ count;

        }
        ++ g;
        ++ m;

      }

      sumOfHistogramElements = std::accumulate( histograms[ f ].begin(), 
                                                histograms[ f ].end(),
                                                0.0 );
      int32_t b = 0;
      for ( b = 0; b < HISTOGRAM_BIN_COUNT; b++ )
      {

        cumulativeHistograms[ f ][ b ] = float( 
                              std::accumulate( histograms[ f ].begin(), 
                                               histograms[ f ].begin() + b,
                                               0.0 ) / sumOfHistogramElements );

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating matplotlib file
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing matplotlib '" << fileNameOutput << "' file : "
                << std::flush;

    }

    std::ofstream os( fileNameOutput.c_str() );

    os << "from matplotlib.pyplot import figure, show" << std::endl;
    os << "from numpy import pi, sin, linspace" << std::endl;
    os << "#from matplotlib.mlab import stineman_interp" << std::endl;
    os << "from pylab import *" << std::endl;
    os << std::endl;
    os << "l = []" << std::endl;
    os << std::endl;
    os << "x = arange( 0.0, 1.0, 0.001 );" << std::endl;
    os << std::endl;

    int32_t h = 0;
    for ( f = 0; f < fileCount; f++ )
    {

      os << "l.append('" << fileNameGFAs[ f ] << "')" << std::endl;
      os << std::endl;
      os << "y" << gkg::StringConverter::toString( f + 1 ) << " = [";
      for ( h = 0; h < HISTOGRAM_BIN_COUNT; h++ )
      {

        if ( cumulative )
        {

          os << cumulativeHistograms[ f ][ h ] << ", ";

        }
        else
        {

          os << histograms[ f ][ h ] << ", ";

        }

      }
      os << "];" << std::endl;
      os << std::endl;

    }

    os << "fig = figure()" << std::endl;
    os << "ax = fig.add_subplot(111)" << std::endl;
    os << "ax.plot(" << std::flush;

    for ( f = 0; f < fileCount; f++ )
    {

      os << "x, y" << gkg::StringConverter::toString( f + 1 ) << ", ";

    }
    os << ")" << std::endl;
    os << "legend(tuple(l), loc='upper right')" << std::endl;
    os << "show()" << std::endl;

    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiGFAHistogramCommand::execute( "
             "const std::vector< std::string >& fileNameGFAs, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "bool cumulative, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiGFAHistogramCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameGFAs ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( cumulative ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef HISTOGRAM_BIN_COUNT
