#include <gkg-mri-reconstruction-plugin-functors/DwBalancedSSFPPhaseOffsetCombiner/DwBalancedSSFPPhaseOffsetCombinerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <string> 
#include <iostream>
#include <cmath>


gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::
                                       DwBalancedSSFPPhaseOffsetCombinerCommand( 
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
  GKG_CATCH( "gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::"
             "DwBalancedSSFPPhaseOffsetCombinerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::
                                       DwBalancedSSFPPhaseOffsetCombinerCommand(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& axis,
                                   bool ascii,
                                   const std::string& format,
                                   bool verbose )
                                             : gkg::Command()
{

  try
  {

    execute( inputNames, outputName, axis, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::"
             "DwBalancedSSFPPhaseOffsetCombinerCommand( "
             "const std::string& inputNames, "
             "const std::string& outputName, "
             "const std::string& axis, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::
                                       DwBalancedSSFPPhaseOffsetCombinerCommand(
                                             const gkg::Dictionary& parameters )
                                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >, 
                                         inputNames );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputName );
    DECLARE_STRING_PARAMETER( parameters, std::string, axis );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( inputNames, outputName, axis, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::"
             "DwBalancedSSFPPhaseOffsetCombinerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::
                                     ~DwBalancedSSFPPhaseOffsetCombinerCommand()
{
}


std::string gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::getStaticName()
{

  try
  {

    return "DwBalancedSSFPPhaseOffsetCombiner";

  }
  GKG_CATCH( "std::string gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::"
             "getStaticName()" );

}


void gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::parse()
{

  try
  {

    std::vector< std::string > inputNames;
    std::string outputName;
    std::string axis = "y";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool for combiner phase cycled balanced-"
                                  "SSFP sequences acquired with different phase"
                                  " offsets to correct for banding artifacts",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input file name list",
                                 inputNames );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 outputName );
    application.addSingleOption( "-axis", "Phase axis x, y, z or t (default=y)",
                                 axis, true );
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

    execute( inputNames, outputName, axis, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::parse()" );

}


void gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::execute( 
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& axis,
                                   bool ascii,
                                   const std::string& format,
                                   bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    // sanity check over axis
    if ( ( axis != "x" ) && ( axis != "y" ) && ( axis != "z" ) )
    {

      throw std::runtime_error( "not a valid axis" );

    }

    // getting first volume size and resolution
    int32_t volumeCount = ( int32_t )inputNames.size();
    gkg::Volume< float > firstVolume;
    gkg::Reader::getInstance().read( inputNames[ 0 ], firstVolume );

    gkg::Vector3d< int32_t > volumeSize;
    int32_t volumeSizeT = 1;
    firstVolume.getSize( volumeSize, volumeSizeT );

    gkg::Vector3d< double > volumeResolution;
    double volumeResolutionT = 1.0;
    firstVolume.getResolution( volumeResolution, volumeResolutionT );


    // looping over other volumes to check for incompatible sizes or resolutions
    gkg::Volume< float > proxy;

    int32_t v = 0;
    for ( v = 1; v < volumeCount; v++ )
    {

      gkg::Volume< float > proxy;
      // reading input files 
      gkg::Reader::getInstance().readHeader( inputNames[ v ], proxy );

      // collecting size
      gkg::Vector3d< int32_t > proxySize;
      int32_t proxySizeT = 1;
      proxy.getSize( proxySize, proxySizeT );

      // collecting resolution
      gkg::Vector3d< double > proxyResolution;
      double proxyResolutionT = 1.0;
      proxy.getResolution( proxyResolution, proxyResolutionT );

      if ( ( volumeSize.x != proxySize.x ) ||
           ( volumeSize.y != proxySize.y ) ||
           ( volumeSize.z != proxySize.z ) ||
           ( volumeResolution != proxyResolution ) )
      {

        throw std::runtime_error( "incompatible sizes or resolutions" );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing half size in phase direction
    ////////////////////////////////////////////////////////////////////////////
    int32_t halfPhaseSize = 0;
    if ( axis == "x" )
    {

      halfPhaseSize = volumeSize.x / 2;
      
    }
    else if ( axis == "y" )
    {

      halfPhaseSize = volumeSize.y / 2;

    }
    else
    {

      halfPhaseSize = volumeSize.z / 2;

    }

    if ( verbose )
    {

      std::cout << "half phase size : " << halfPhaseSize << std::endl;

    }

    std::vector< float > phaseOffsets( volumeCount - 1 );
    phaseOffsets[ 0 ] = 1.0f / 2.0f;
    phaseOffsets[ 1 ] = 1.0f / 4.0f;
    phaseOffsets[ 2 ] = 3.0f / 4.0f;

    //  and what if volumeCount>4 ???? not managed ????


    ////////////////////////////////////////////////////////////////////////////
    // computing and writing reordered volumes
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing and writing reordered volumes : " 
                << std::flush;

    }

    std::vector< gkg::Volume< float > > reorderedVolumes( volumeCount );
    reorderedVolumes[ 0 ] = firstVolume;

    for ( v = 1; v < volumeCount; v++ )
    {

      // allocating current reordered volume and setting adequate resolution
      gkg::Volume< float >& reorderedVolume = reorderedVolumes[ v ];

      reorderedVolume.reallocate( volumeSize, volumeSizeT );
      reorderedVolume.setResolution( volumeResolution, volumeResolutionT );


      // reading volume
      gkg::Volume< float > currentVolume;
      gkg::Reader::getInstance().read( inputNames[ v ], currentVolume );
      gkg::Vector3d< int32_t > site;
      int32_t t = 0;

      float phaseOffset = phaseOffsets[ v - 1 ];
      float oneMinusPhaseOffset = 1.0f - phaseOffsets[ v - 1 ];

      if ( axis == "x" )
      {

        for ( t = 0; t < volumeSizeT; t++ )
        {

          for ( site.z = 0; site.z < volumeSize.z; site.z++ )
          {

            for ( site.y = 0; site.y < volumeSize.y; site.y++ )
            {

              for ( site.x = 0; site.x < volumeSize.x; site.x++ )
              {

                if ( site.x <  ( int32_t )( ( float )volumeSize.x * 
                                            oneMinusPhaseOffset ) )
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x +
                                   ( int32_t )( ( float )volumeSize.x * 
                                                phaseOffset ), 
                                   site.y,
                                   site.z,
                                   t );

                }
                else
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x -
                                   ( int32_t )( ( float )volumeSize.x * 
                                                oneMinusPhaseOffset ), 
                                   site.y, 
                                   site.z, 
                                   t );

                }

              }

            }

          }

        }

      }
      else if ( axis == "y" )
      {

        for ( t = 0; t < volumeSizeT; t++ )
        {

          for ( site.z = 0; site.z < volumeSize.z; site.z++ )
          {

            for ( site.y = 0; site.y < volumeSize.y; site.y++ )
            {

              for ( site.x = 0; site.x < volumeSize.x; site.x++ )
              {

                if ( site.y <  ( int32_t )( ( float )volumeSize.y * 
                                            oneMinusPhaseOffset ) )
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x, 
                                   site.y +
                                   ( int32_t )( ( float )volumeSize.y * 
                                                phaseOffset ), 
                                   site.z, 
                                   t );

                }
                else
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x, 
                                   site.y - 
                                   ( int32_t )( ( float )volumeSize.y * 
                                                oneMinusPhaseOffset ), 
                                   site.z, 
                                   t );

                }

              }

            }

          }

        }

      }
      else
      {

        for ( t = 0; t < volumeSizeT; t++ )
        {

          for ( site.z = 0; site.z < volumeSize.z; site.z++ )
          {

            for ( site.y = 0; site.y < volumeSize.y; site.y++ )
            {

              for ( site.x = 0; site.x < volumeSize.x; site.x++ )
              {

                if ( site.z <  ( int32_t )( ( float )volumeSize.z * 
                                            oneMinusPhaseOffset ) )
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x, 
                                   site.y, 
                                   site.z +
                                   ( int32_t )( ( float )volumeSize.z * 
                                                phaseOffset ), 
                                   t );

                }
                else
                {

                  reorderedVolume( site, t ) =
                    currentVolume( site.x, 
                                   site.y, 
                                   site.z -
                                   ( int32_t )( ( float )volumeSize.z * 
                                                oneMinusPhaseOffset ), 
                                   t );

                }

              }

            }

          }

        }

      }

      // writing current reordered volume
      std::string reorderedVolumeName = \
                                   outputName.substr( 0, 
                                                      outputName.size() - 4 ) + 
                                   "_" + std::to_string( v ) + "_reordered";

      gkg::Writer::getInstance().write( reorderedVolumeName,
                                        reorderedVolume,
                                        ascii,
                                        format );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over volumes to compute the root mean square volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing quadratic mean : " 
                << std::flush;

    }

    gkg::Volume< float > rootMeanSquareVolume( volumeSize, volumeSizeT );
    rootMeanSquareVolume.setResolution( volumeResolution, volumeResolutionT );

    gkg::Vector3d< int32_t > site;
    int32_t t = 0;

    for ( t = 0; t < volumeSizeT; t++ )
    {

      for ( site.z = 0; site.z < volumeSize.z; site.z++ )
      {

        for ( site.y = 0; site.y < volumeSize.y; site.y++ )
        {

          for ( site.x = 0; site.x < volumeSize.x; site.x++ )
          {

            float& currentValue = rootMeanSquareVolume( site, t );
            for ( v = 0; v < volumeCount; v ++ )
            {

              float reorderedValue = reorderedVolumes[ v ]( site, t );
              currentValue += reorderedValue * reorderedValue;

            }

            currentValue /= ( float )volumeCount;
            currentValue = std::sqrt( currentValue / ( float )volumeCount );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //////////////////////////////////////////////////////////////////////////
    // writing reordered volumes
    //////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing output root mean square volume : " 
                << std::flush;

    }

    gkg::Writer::getInstance().write( outputName,
                                      rootMeanSquareVolume,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::DwBalancedSSFPPhaseOffsetCombinerCommand::execute( "
             "const std::string& inputNames, "
             "const std::string& outputName, "
             "const std::string& axis, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( DwBalancedSSFPPhaseOffsetCombinerCommand,
    			DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( inputNames ) +
    			DECLARE_STRING_PARAMETER_HELP( outputName ) +
    			DECLARE_STRING_PARAMETER_HELP( axis ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
