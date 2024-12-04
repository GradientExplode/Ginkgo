#include <gkg-dmri-plugin-functors/Dwi90DegreeCrossingPhantomSimulator/Dwi90DegreeCrossingPhantomSimulatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::
                                     Dwi90DegreeCrossingPhantomSimulatorCommand(
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
  GKG_CATCH( "gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::"
             "Dwi90DegreeCrossingPhantomSimulatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::
                                     Dwi90DegreeCrossingPhantomSimulatorCommand(
                                   bool verbose,
                                   int32_t orientationCount,
                                   float b,
                                   float lambda1,
                                   float lambda2,
                                   float lambda3,
                                   float horizontalVolumeFraction,
                                   float noiseStdDev,
                                   const std::string& fileNameDWI,
                                   const std::string& fileNameTractographyMask )
                              : gkg::Command()
{

  try
  {

    execute( verbose,
             orientationCount,
             b,
             lambda1,
             lambda2,
             lambda3,
             horizontalVolumeFraction,
             noiseStdDev,
             fileNameDWI,
             fileNameTractographyMask );

  }
  GKG_CATCH( "gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::"
             "Dwi90DegreeCrossingPhantomSimulatorCommand( "
             "bool verbose, "
             "int32_t orientationCount, "
             "float b, "
             "float lambda1, "
             "float lambda2, "
             "float lambda3, "
             "float horizontalVolumeFraction, "
             "float noiseStdDev, "
             "const std::string& fileNameDWI, "
             "const std::string& fileNameTractographyMask )" );

}


gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::
                                    Dwi90DegreeCrossingPhantomSimulatorCommand(
                                             const gkg::Dictionary& parameters )
                                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, orientationCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, b );
    DECLARE_FLOATING_PARAMETER( parameters, float, lambda1 );
    DECLARE_FLOATING_PARAMETER( parameters, float, lambda2 );
    DECLARE_FLOATING_PARAMETER( parameters, float, lambda3 );
    DECLARE_FLOATING_PARAMETER( parameters, float, horizontalVolumeFraction );
    DECLARE_FLOATING_PARAMETER( parameters, float, noiseStdDev );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDWI );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTractographyMask );
    
    execute( verbose,
             orientationCount,
             b,
             lambda1,
             lambda2,
             lambda3,
             horizontalVolumeFraction,
             noiseStdDev,
             fileNameDWI,
             fileNameTractographyMask );

  }
  GKG_CATCH( "gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::"
             "Dwi90DegreeCrossingPhantomSimulatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::
                                   ~Dwi90DegreeCrossingPhantomSimulatorCommand()
{
}


std::string gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::getStaticName()
{

  try
  {

    return "Dwi90DegreeCrossingPhantomSimulator";

  }
  GKG_CATCH( "std::string gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::"
             "getStaticName()" );

}


void gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::parse()
{

  try
  {

    bool verbose = false;
    int32_t orientationCount = 60;
    float b = 1500.0;
    float lambda1 = 1.7e-9;
    float lambda2 = 0.2e-9;
    float lambda3 = 0.2e-9;
    float horizontalVolumeFraction = 0.5;
    float noiseStdDev = 0;
    std::string fileNameDWI;
    std::string fileNameTractographyMask;

    gkg::Application application(
                              _argc, _argv,
                              "Simulator of 90deg crossing fiber DWI dataset",
                              _loadPlugin );
    application.addSingleOption( "-orientationCount",
                                 "Number of orientations (default=60)",
                                 orientationCount, 
                                 true );
    application.addSingleOption( "-b",
                                 "b-value (default=1500)",
                                 b, 
                                 true );
    application.addSingleOption( "-lambda1",
                                 "Tensor principal eigenvalue "
                                 "(default=1.7e-9m2/s)",
                                 lambda1, 
                                 true );
    application.addSingleOption( "-lambda2",
                                 "Tensor middle eigenvalue "
                                 "(default=0.2e-9m2/s)",
                                 lambda2,
                                 true );
    application.addSingleOption( "-lambda3",
                                 "Tensor minimum eigenvalue "
                                 "(default=0.2e-9m2/s)",
                                 lambda3,
                                 true );
    application.addSingleOption( "-horizontalVolumeFraction",
                                 "Volume fraction of horiontal bundle "
                                 "(default=0.5)",
                                 horizontalVolumeFraction,
                                 true );
    application.addSingleOption( "-noiseStdDev",
                                 "Standard deviation for noise (default=0)",
                                 noiseStdDev, 
                                 true );
    application.addSingleOption( "-od",
                                 "Output DWI Volume filename",
                                 fileNameDWI, 
                                 false );
    application.addSingleOption( "-om",
                                 "Output tractography mask filename",
                                 fileNameTractographyMask, 
                                 false );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( verbose,
             orientationCount,
             b,
             lambda1,
             lambda2,
             lambda3,
             horizontalVolumeFraction,
             noiseStdDev,
             fileNameDWI,
             fileNameTractographyMask );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::"
                     "parse()" );

}


void gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::execute(
                                   bool verbose,
                                   int32_t orientationCount,
                                   float b,
                                   float lambda1,
                                   float lambda2,
                                   float lambda3,
                                   float horizontalVolumeFraction,
                                   float noiseStdDev,
                                   const std::string& fileNameDWI,
                                   const std::string& fileNameTractographyMask )
{

  try
  {

    // initializing the factory and the random generator for rician noise
    gkg::NumericalAnalysisImplementationFactory* factory =
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // defining the output volume to construct
    gkg::Volume< int16_t > dwi( 18, 18, 18, orientationCount + 1 );

    // defining the output volume to construct
    gkg::Volume< int16_t > mask( 18, 18, 18 );
    mask.fill( 0 );

    float S = 0.0;
    float S0 = 4096;

    b *= 1e6;

    float lambda = ( lambda1 + lambda2 + lambda3 ) / 3.0;

    ////////////////////////////////////////////////////////////////////////////
    // constructing tensors D, D1 and D2
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "constructing D, D1 and D2 tensors : " << std::flush;

    }

    // constructing the D matrix
    gkg::Matrix D( 3, 3 );
    D( 0, 0 ) = lambda;
    D( 1, 1 ) = lambda;
    D( 2, 2 ) = lambda;

    // constructing the D1 matrix
    gkg::Matrix D1( 3, 3 );
    D1( 0, 0 ) = lambda1;
    D1( 1, 1 ) = lambda2;
    D1( 2, 2 ) = lambda3;

    // constructing the D2 matrix
    gkg::Matrix D2( 3, 3 );
    D2( 0, 0 ) = lambda2;
    D2( 1, 1 ) = lambda1;
    D2( 2, 2 ) = lambda3;

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building the orientation set
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building the orientation set of " << orientationCount
                << " directions : " << std::flush;

    }

    // building the orientation set
    gkg::ElectrostaticOrientationSet orientationSet( orientationCount );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // defining the bounding boxes
    ////////////////////////////////////////////////////////////////////////////
    gkg::BoundingBox< int32_t >
      verticalBundleBoundingBox( 7, 10, 1, 16, 7, 10 );
    gkg::BoundingBox< int32_t > 
      horizontalBundleBoundingBox( 1, 16, 7, 10, 7, 10 );

    // computing sizes of the volume
    int32_t sizeX = dwi.getSizeX();
    int32_t sizeY = dwi.getSizeY();
    int32_t sizeZ = dwi.getSizeZ();

    ////////////////////////////////////////////////////////////////////////////
    // building the tractography mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building the tractography mask : " << std::flush;

    }
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( verticalBundleBoundingBox.contains( x, y, z ) ||
               horizontalBundleBoundingBox.contains( x, y, z ) )
          {

            mask( x, y, z ) = 1;

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building the DWI dataset
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building the DWI dataset : " << std::flush;

    }
    int32_t o = 0;
    for ( o = 0; o < orientationCount + 1; o++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            float realChannelNoise = ( float )factory->getGaussianRandomNumber(
                                                                randomGenerator,
                                                                0,
                                                                noiseStdDev );
            float imagChannelNoise = ( float )factory->getGaussianRandomNumber(
                                                                randomGenerator,
                                                                0,
                                                                noiseStdDev );

            // signal for b = 0
            if ( o == 0 )
            {

              S = S0;

            }
            else
            {

              const gkg::Vector3d< float >& d =
                                         orientationSet.getOrientation( o - 1 );
              gkg::Vector direction( d );
              gkg::Matrix directionMatrix( direction.toMatrix() );
              directionMatrix.transpose();
              gkg::Vector directionTranspose( directionMatrix.toVector() );

             // case where we are in the crossing (both D1 and D2 contribute)
              if ( verticalBundleBoundingBox.contains( x, y, z ) &&
                   horizontalBundleBoundingBox.contains( x, y, z ) )
              {

                S = S0 * ( horizontalVolumeFraction * 
                           std::exp( -b * directionTranspose.getComposition(
                                            D1.getComposition( direction ) ) ) +
                           ( 1.0 - horizontalVolumeFraction ) * 
                           std::exp( -b * directionTranspose.getComposition(
                                           D2.getComposition( direction ) ) ) );

              }
              // case where D1 is the only one contributing
              else if ( horizontalBundleBoundingBox.contains( x, y, z ) &&
                        !verticalBundleBoundingBox.contains( x, y, z ) )
              {

                S = S0 * std::exp( -b * directionTranspose.getComposition(
                                             D1.getComposition( direction ) ) );

              }
              // case where D2 is the only one contributing
              else if ( verticalBundleBoundingBox.contains( x, y, z ) &&
                        !horizontalBundleBoundingBox.contains( x, y, z ) )
              {

                S = S0 * std::exp( -b * directionTranspose.getComposition(
                                             D2.getComposition( direction ) ) );

              }
              // isotropic diffusion in the rest of the volume
              else
              {

                S = S0 * std::exp( -b * directionTranspose.getComposition(
                                              D.getComposition( direction ) ) );

              }

            }
            dwi( x, y, z, o ) = ( int16_t )std::sqrt( ( S + realChannelNoise ) *
                                                      ( S + realChannelNoise ) +
                                                      imagChannelNoise *
                                                      imagChannelNoise );

          }

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing the DWI dataset mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "writing the DWI dataset : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameDWI, dwi );

    if ( verbose )
    {

     std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing the tractography mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

     std::cout << "writing the tractography mask volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameTractographyMask, mask );

    if ( verbose )
    {

     std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::Dwi90DegreeCrossingPhantomSimulatorCommand::execute( "
             "bool verbose, "
             "int32_t orientationCount, "
             "float b, "
             "float lambda1, "
             "float lambda2, "
             "float lambda3, "
             "float horizontalVolumeFraction, "
             "float noiseStdDev, "
             "const std::string& fileNameDWI, "
             "const std::string& fileNameTractographyMask )" );

}


RegisterCommandCreator( 
    Dwi90DegreeCrossingPhantomSimulatorCommand,
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) +
    DECLARE_INTEGER_PARAMETER_HELP( orientationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( b ) +
    DECLARE_FLOATING_PARAMETER_HELP( lambda1 ) +
    DECLARE_FLOATING_PARAMETER_HELP( lambda2 ) +
    DECLARE_FLOATING_PARAMETER_HELP( lambda3 ) +
    DECLARE_FLOATING_PARAMETER_HELP( horizontalVolumeFraction ) +
    DECLARE_FLOATING_PARAMETER_HELP( noiseStdDev ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDWI ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTractographyMask ) );
