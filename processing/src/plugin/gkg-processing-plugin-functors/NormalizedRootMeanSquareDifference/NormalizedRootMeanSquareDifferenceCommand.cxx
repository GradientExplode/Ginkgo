#include <gkg-processing-plugin-functors/NormalizedRootMeanSquareDifference/NormalizedRootMeanSquareDifferenceCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
// class NormalizedRootMeanSquareDifferenceCommand
////////////////////////////////////////////////////////////////////////////////

gkg::NormalizedRootMeanSquareDifferenceCommand::NormalizedRootMeanSquareDifferenceCommand( 
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                               : gkg::Command( argc, argv, 
                                                               loadPlugin, 
                                                               removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::NormalizedRootMeanSquareDifferenceCommand::"
             "NormalizedRootMeanSquareDifferenceCommand( int32_t argc, "
             "char* argv[], bool loadPlugin, bool removeFirst ) "
             " : gkg::Command( argc, argv, loadPlugin, removeFirst )" );

}

gkg::NormalizedRootMeanSquareDifferenceCommand::NormalizedRootMeanSquareDifferenceCommand( 
                                  const std::vector< std::string >& fileNameIns,
                    		    		  const std::string& fileNameMask,
                            		  const std::string& fileNameOut,
                    		     	    bool verbose )
{

  try
  {

    execute( fileNameIns, fileNameMask, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::NormalizedRootMeanSquareDifferenceCommand::"
             "NormalizedRootMeanSquareDifferenceCommand( "
             "const std::vector< std::string >& fileNameIns,"
             "const std::string& fileNameMask,"
             "const std::string& fileNameOut,"
             "bool verbose )" );

}

gkg::NormalizedRootMeanSquareDifferenceCommand::NormalizedRootMeanSquareDifferenceCommand( 
                                             const gkg::Dictionary& parameters )
                                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameIns );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIns, fileNameMask, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::NormalizedRootMeanSquareDifferenceCommand::"
             "NormalizedRootMeanSquareDifferenceCommand( const "
             "gkg::Dictionary& parameters ) : gkg::Command( parameters )" );

}

gkg::NormalizedRootMeanSquareDifferenceCommand::
                                    ~NormalizedRootMeanSquareDifferenceCommand()
{
}


std::string gkg::NormalizedRootMeanSquareDifferenceCommand::getStaticName()
{

  try
  {

    return "NormalizedRootMeanSquareDifference";

  }
  GKG_CATCH( "std::string gkg::"
             "NormalizedRootMeanSquareDifferenceCommand::getStaticName()" );

}

void gkg::NormalizedRootMeanSquareDifferenceCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameIns; 
    std::string fileNameMask;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Normalized Root Mean Square Difference",
                                   _loadPlugin );

    application.addSeriesOption( "-i",
                                 "Input volume file name list",
                                 fileNameIns,
                                 2 );
    application.addSingleOption( "-m",
                                 "Mask volume file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIns, fileNameMask, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::"
                     "NormalizedRootMeanSquareDifferenceCommand::parse()" );

}

void gkg::NormalizedRootMeanSquareDifferenceCommand::execute(
                 	                const std::vector< std::string >& fileNameIns,
                                  const std::string& fileNameMask,
                                  const std::string& fileNameOut,
                                  bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading input files
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIns[ 0 ] << "' : " << std::flush;

    }

    gkg::Volume< float > firstVolumeIn;
    gkg::Reader::getInstance().read( fileNameIns[ 0 ], firstVolumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIns[ 1 ] << "' : " << std::flush;

    }

    gkg::Volume< float > secondVolumeIn;
    gkg::Reader::getInstance().read( fileNameIns[ 1 ], secondVolumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( !firstVolumeIn.hasSameSize( secondVolumeIn ) ||
         !firstVolumeIn.hasSameResolution( secondVolumeIn, true ) )
    {

      throw std::runtime_error(
                  "incoherent size and resolution between input volume(s)" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }

    gkg::Volume< int16_t > maskVolume;
    gkg::Reader::getInstance().read( fileNameMask, maskVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // performing normalized root mean square difference
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "Performing normalized RMS difference" 
                << std::endl;

    }

    float sumOfSquare = 0.0f;
    float sumOfSquareDifference = 0.0f;

    int32_t sizeX = firstVolumeIn.getSizeX();
    int32_t sizeY = firstVolumeIn.getSizeY();
    int32_t sizeZ = firstVolumeIn.getSizeZ();
    int32_t sizeT = firstVolumeIn.getSizeT();
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;

    gkg::Vector3d< double > resolution;
    firstVolumeIn.getResolution( resolution );

    gkg::Volume< float > volumeOut( sizeX, sizeY, sizeZ );
    volumeOut.setResolution( resolution );

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( maskVolume( x, y, z ) == 1 )
          {

            for ( t = 0; t < sizeT; t++ )
            {

              sumOfSquare += ( firstVolumeIn( x, y, z, t ) + \
                              secondVolumeIn( x, y, z, t ) / 2.0 ) * \
                             ( firstVolumeIn( x, y, z, t ) + \
                              secondVolumeIn( x, y, z, t ) / 2.0 );

              sumOfSquareDifference += ( firstVolumeIn( x, y, z, t ) - \
                                        secondVolumeIn( x, y, z, t ) ) * \
                                       ( firstVolumeIn( x, y, z, t ) - \
                                        secondVolumeIn( x, y, z, t ) );

            }
            if ( sumOfSquare != 0 )
            {

              volumeOut( x, y, z ) = sqrt( sumOfSquareDifference / 
                                           sumOfSquare );

            }
            sumOfSquare = 0.0f;
            sumOfSquareDifference = 0.0f;

          }

        }

      }

    }

    if ( !fileNameOut.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameOut << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameOut, volumeOut );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::NormalizedRootMeanSquareDifferenceCommand::execute("
             "const std::vector< std::string >& fileNameIns,"
             "const std::string& fileNameMask,"
             "const std::string& fileNameOut,"
             "bool verbose )" );

}

RegisterCommandCreator( 
                       NormalizedRootMeanSquareDifferenceCommand,
                		   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameIns ) +
                		   DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
                		   DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                		   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
