#include <gkg-processing-plugin-functors/KMeansSegmentation/KMeansSegmentationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-numericalanalysis/KMeansClustering.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::KMeansSegmentationCommand::KMeansSegmentationCommand(
                                         int32_t argc,
                                         char* argv[],
                                         bool loadPlugin,
                                         bool removeFirst )
                               : gkg::Command( argc,
                                               argv, 
                                               loadPlugin,
                                               removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::KMeansSegmentationCommand::"
             "KMeansSegmentationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::KMeansSegmentationCommand::KMeansSegmentationCommand(
                                         const std::string& fileNameVolumeIn,
                                         const std::string& fileNameVolumeMask,
                                         const std::string& fileNameVolumeOut,
                                         int32_t clusterCount,
                                         int32_t maximumIterationCount,
                                         bool ascii,
                                         const std::string& format,
                                         bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameVolumeIn,
             fileNameVolumeMask,
             fileNameVolumeOut,
             clusterCount,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::KMeansSegmentationCommand::"
             "KMeansSegmentationCommand( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeMask, "
             "const std::string& fileNameVolumeOut, "
             "int32_t clusterCount, "
             "int32_t maximumIterationCount, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::KMeansSegmentationCommand::KMeansSegmentationCommand(
                                     const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, clusterCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumIterationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameVolumeIn,
             fileNameVolumeMask,
             fileNameVolumeOut,
             clusterCount,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::KMeansSegmentationCommand::"
             "KMeansSegmentationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::KMeansSegmentationCommand::~KMeansSegmentationCommand()
{
}


std::string gkg::KMeansSegmentationCommand::getStaticName()
{

  try
  {

    return "KMeansSegmentation";

  }
  GKG_CATCH( "std::string "
             "gkg::KMeansSegmentationCommand::getStaticName()" );

}


void gkg::KMeansSegmentationCommand::parse()
{

  try
  {

    std::string fileNameVolumeIn;
    std::string fileNameVolumeMask;
    std::string fileNameVolumeOut;
    int32_t clusterCount;
    int32_t maximumIterationCount = 300;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "K-means-based segmentation tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameVolumeIn );
    application.addSingleOption( "-m",
                                 "Input mask volume file name",
                                 fileNameVolumeMask );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameVolumeOut );
    application.addSingleOption( "-c",
                                 "Target cluster count",
                                 clusterCount );
    application.addSingleOption( "-I",
                                 "Maximum iteration count (default=300)",
                                 maximumIterationCount,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameVolumeIn,
             fileNameVolumeMask,
             fileNameVolumeOut,
             clusterCount,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::KMeansSegmentationCommand::parse()" );

}


void gkg::KMeansSegmentationCommand::execute( 
                                          const std::string& fileNameVolumeIn,
                                          const std::string& fileNameVolumeMask,
                                          const std::string& fileNameVolumeOut,
                                          int32_t clusterCount,
                                          int32_t maximumIterationCount,
                                          bool ascii,
                                          const std::string& format,
                                          bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( clusterCount <= 0 )
    {

      throw std::runtime_error( "cluster must be greater than 0" );

    }

    if ( maximumIterationCount <= 0 )
    {

      throw std::runtime_error( 
                             "maximum iteration count must be greater than 0" );

    }

 
    ////////////////////////////////////////////////////////////////////////////
    // reading the input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading input volume '" << fileNameVolumeIn << "' : "
                << std::flush;

    }

    gkg::Volume< float > volumeIn;
    gkg::TypedVolumeReaderProcess< float > 
     typedVolumeReaderProcessIn( volumeIn );
    typedVolumeReaderProcessIn.execute( fileNameVolumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inputSize( 1, 1, 1);
    int32_t inputSizeT = 1;
    volumeIn.getSize( inputSize, inputSizeT );
    if ( inputSizeT != 1 )
    {

      throw std::runtime_error( "input volume must be 3D only" );

    }

    gkg::Vector3d< double > inputResolution( 1, 1, 1);
    double inputResolutionT = 1.0;
    volumeIn.getResolution( inputResolution, inputResolutionT );

    if ( verbose )
    {

      std::cout << "input volume size / resolution : "
                << inputSize << " / "
                << inputResolution << " mm3"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading the mask volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading mask volume '" << fileNameVolumeMask << "' : "
                << std::flush;

    }

    gkg::Volume< int16_t > volumeMask;
    gkg::TypedVolumeReaderProcess< int16_t > 
     typedVolumeReaderProcessSeeds( volumeMask );
    typedVolumeReaderProcessSeeds.execute( fileNameVolumeMask );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( !volumeMask.hasSameSize( volumeIn, false ) ||
         !volumeMask.hasSameResolution( volumeIn, false ) )
    {

      throw std::runtime_error( 
                   "input and mask volume have incoherent size or resolution" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating the output volume : "
                << std::flush;

    }

    gkg::Volume< int16_t > volumeOut( inputSize );
    volumeOut.setResolution( inputResolution, inputResolutionT );
    volumeOut.fill( 0 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting all seeds from seed volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "extracting all intensities from input within mask : "
                << std::flush;

    }
    int32_t voxelCountWithinMask = 0;
    gkg::Vector3d< int32_t > voxel;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( volumeMask( voxel ) )
          {

            ++ voxelCountWithinMask;

          }

        }

      }

    }

    std::vector< std::vector< float > > points( voxelCountWithinMask );

    int32_t voxelIndex = 0;

    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( volumeMask( voxel ) )
          {

            points[ voxelIndex ].resize( 1U );
            points[ voxelIndex ][ 0 ] = volumeIn( voxel );
            ++ voxelIndex;

          }

        }

      }

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the k-means clustering
    ////////////////////////////////////////////////////////////////////////////

    gkg::KMeansClustering< float > kMeansClustering( clusterCount,
                                                     maximumIterationCount );
    std::vector< int32_t > clusterIds; 
    kMeansClustering.clusterize( points, clusterIds, verbose );


    ////////////////////////////////////////////////////////////////////////////
    // filling output volume from clustering results
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "filling output volume from clustering results : "
                << std::endl;

    }


    voxelIndex = 0;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( volumeMask( voxel ) )
          {

            volumeOut( voxel ) = ( int16_t )clusterIds[ voxelIndex ];
            ++ voxelIndex;

          }

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing segmentation output volume '"
                << fileNameVolumeOut << "' : "
                << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameVolumeOut,
                                      volumeOut,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::KMeansSegmentationCommand::execute( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeMask, "
             "const std::string& fileNameVolumeOut, "
             "int32_t clusterCount, "
             "int32_t maximumIterationCount, "
             "bool ascii, const std::string& format, bool verbose )" );

}

RegisterCommandCreator( KMeansSegmentationCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeMask ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeOut ) +
                        DECLARE_INTEGER_PARAMETER_HELP( clusterCount ) +
                        DECLARE_INTEGER_PARAMETER_HELP(
                                                       maximumIterationCount ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
