#include <gkg-dmri-plugin-functors/DwiParallelNoiseFilter/DwiParallelNoiseFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/ParallelKalmanFilter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>


#define EPSILON_INT_16_TYPE   1
#define EPSILON               1e-20
#define INITIAL_VARIANCE_C    1e-6 


gkg::DwiParallelNoiseFilterCommand::DwiParallelNoiseFilterCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::DwiParallelNoiseFilterCommand::"
             "DwiParallelNoiseFilterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiParallelNoiseFilterCommand::DwiParallelNoiseFilterCommand(
                            const std::string& fileNameT2,
                            const std::string& fileNameDW,
                            const std::string& fileNameMaskOfSignal,
                            const std::string& fileNameMaskOfNoise,
                            const std::string& fileNameAbacus,
                            int32_t neighborhoodSize,
                            const std::string& fileNameDenoisedT2,
                            const std::string& fileNameDenoisedDW,
                            const std::string& fileNameNormalizedNoiseStdDevMap,
                            int32_t maximumSphericalHarmonicOrder,
                            double laplaceBeltramiFactor,
                            double spatialWeightFactor,
                            double structuralWeightFactor,
                            float noiseStdDev,
                            bool noWeighting,
                            bool onlySpatialWeighting,
                            bool onlyStructuralWeighting,
                            bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameAbacus,
             neighborhoodSize,
             fileNameDenoisedT2,
             fileNameDenoisedDW,
             fileNameNormalizedNoiseStdDevMap,
             maximumSphericalHarmonicOrder,
             laplaceBeltramiFactor,
             spatialWeightFactor,
             structuralWeightFactor,
             noiseStdDev,
             noWeighting,
             onlySpatialWeighting,
             onlyStructuralWeighting,
             verbose );

  }
  GKG_CATCH( "gkg::DwiParallelNoiseFilterCommand::"
             "DwiParallelNoiseFilterCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameMaskOfNoise, "
             "const std::string& fileNameAbacus, "
             "int32_t neighborhoodSize, "
             "const std::string& fileNameDenoisedT2, "
             "const std::string& fileNameDenoisedDW, "
             "const std::string& fileNameNormalizedNoiseStdDevMap, "
             "int32_t maximumSphericalHarmonicOrder, "
             "double laplaceBeltramiFactor, "
             "double spatialWeightFactor, "
             "double structuralWeightFactor, "
             "float noiseStdDev, "
             "bool noWeighting, "
             "bool onlySpatialWeighting, "
             "bool onlyStructuralWeighting, "
             "bool verbose )" );

}


gkg::DwiParallelNoiseFilterCommand::DwiParallelNoiseFilterCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMaskOfSignal );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMaskOfNoise );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameAbacus );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, neighborhoodSize );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDenoisedT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDenoisedDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameNormalizedNoiseStdDevMap );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t,
                               maximumSphericalHarmonicOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double, laplaceBeltramiFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, spatialWeightFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, structuralWeightFactor );
    DECLARE_FLOATING_PARAMETER( parameters, float, noiseStdDev );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, noWeighting );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, onlySpatialWeighting );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, onlyStructuralWeighting );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameAbacus,
             neighborhoodSize,
             fileNameDenoisedT2,
             fileNameDenoisedDW,
             fileNameNormalizedNoiseStdDevMap,
             maximumSphericalHarmonicOrder,
             laplaceBeltramiFactor,
             spatialWeightFactor,
             structuralWeightFactor,
             noiseStdDev,
             noWeighting,
             onlySpatialWeighting,
             onlyStructuralWeighting,
             verbose );

  }
  GKG_CATCH( "gkg::DwiParallelNoiseFilterCommand::"
             "DwiParallelNoiseFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiParallelNoiseFilterCommand::~DwiParallelNoiseFilterCommand()
{
}


std::string gkg::DwiParallelNoiseFilterCommand::getStaticName()
{

  try
  {

    return "DwiParallelNoiseFilter";

  }
  GKG_CATCH( "std::string gkg::DwiParallelNoiseFilterCommand::getStaticName()" );

}


void gkg::DwiParallelNoiseFilterCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMaskOfSignal;
    std::string fileNameMaskOfNoise;
    std::string fileNameAbacus;
    int32_t neighborhoodSize = 3;
    std::string fileNameDenoisedT2;
    std::string fileNameDenoisedDW;
    std::string fileNameNormalizedNoiseStdDevMap;
    int32_t maximumSphericalHarmonicOrder = 4;
    double laplaceBeltramiFactor = 0.0;
    double spatialWeightFactor = 1.0;
    double structuralWeightFactor = 1.0;
    float noiseStdDev = 0.0f;
    bool noWeighting = false;
    bool onlySpatialWeighting = false;
    bool onlyStructuralWeighting = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI Rician noise filter",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2-weighted volume file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW-weighted volume file name",
                                 fileNameDW );
    application.addSingleOption( "-a",
                                 "Input Rician fit abacus volume file name",
                                 fileNameAbacus );
    application.addSingleOption( "-ms",
                                 "Input signal mask file name ",
                                 fileNameMaskOfSignal );
    application.addSingleOption( "-mn",
                                 "Input noise mask file name "
                                 "(inverse of signal mask)",
                                 fileNameMaskOfNoise,
                                 true );
    application.addSingleOption( "-n",
                                 "Neighborhood size (default=3)",
                                 neighborhoodSize,
                                 true );
    application.addSingleOption( "-sigma",
                                 "Noise standard deviation",
                                 noiseStdDev,
                                 true );
    application.addSingleOption( "-ot2",
                                 "Output denoised T2-weighted volume file name",
                                 fileNameDenoisedT2 );
    application.addSingleOption( "-odw",
                                 "Output denoised DW-weighted volume file name",
                                 fileNameDenoisedDW );
    application.addSingleOption( "-onsdev",
                                 "Output normalized noise std dev map",
                                 fileNameNormalizedNoiseStdDevMap,
                                 true );
    application.addSingleOption( "-maximumSHOrder",
                                 "Maximum of the SH order (default=4)",
                                 maximumSphericalHarmonicOrder,
                                 true );
    application.addSingleOption( "-laplaceBeltramiFactor",
                                 "Laplace-Beltrami regularization factor"
                                 " (default=0.0)",
                                 laplaceBeltramiFactor,
                                 true );
    application.addSingleOption( "-spatialWeightFactor",
                                 "Spatial weight factor"
                                 " (default=1.0)",
                                 spatialWeightFactor,
                                 true );
    application.addSingleOption( "-structuralWeightFactor",
                                 "Structural weight factor"
                                 " (default=1.0)",
                                 structuralWeightFactor,
                                 true );
    application.addSingleOption( "-noWeighting",
                                 "Do not apply any weighting (default=false)",
                                 noWeighting,
                                 true );
    application.addSingleOption( "-onlySpatialWeighting",
                                 "Only apply spatial weighting (default=false)",
                                 onlySpatialWeighting,
                                 true );
    application.addSingleOption( "-onlyStructuralWeighting",
                                 "Only apply structural weighting "
                                 "(default=false)",
                                 onlyStructuralWeighting,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameAbacus,
             neighborhoodSize,
             fileNameDenoisedT2,
             fileNameDenoisedDW,
             fileNameNormalizedNoiseStdDevMap,
             maximumSphericalHarmonicOrder,
             laplaceBeltramiFactor,
             spatialWeightFactor,
             structuralWeightFactor,
             noiseStdDev,
             noWeighting,
             onlySpatialWeighting,
             onlyStructuralWeighting,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiParallelNoiseFilterCommand::parse()" );

}


void gkg::DwiParallelNoiseFilterCommand::execute(
                            const std::string& fileNameT2,
                            const std::string& fileNameDW,
                            const std::string& fileNameMaskOfSignal,
                            const std::string& fileNameMaskOfNoise,
                            const std::string& fileNameAbacus,
                            int32_t neighborhoodSize,
                            const std::string& fileNameDenoisedT2,
                            const std::string& fileNameDenoisedDW,
                            const std::string& fileNameNormalizedNoiseStdDevMap,
                            int32_t maximumSphericalHarmonicOrder,
                            double laplaceBeltramiFactor,
                            double spatialWeightFactor,
                            double structuralWeightFactor,
                            float noiseStdDev,
                            bool noWeighting,
                            bool onlySpatialWeighting,
                            bool onlyStructuralWeighting,
                            bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( ( neighborhoodSize % 2 == 0 ) || ( neighborhoodSize < 3 ) )
    {

       throw std::runtime_error( "bad neighborhood size" );

    }
    double spatialWeightFactorSquare = spatialWeightFactor *
                                       spatialWeightFactor;
    double structuralWeightFactorSquare = structuralWeightFactor *
                                          structuralWeightFactor;

    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    int32_t sizeX = t2.getSizeX();
    int32_t sizeY = t2.getSizeY();
    int32_t sizeZ = t2.getSizeZ();
    int32_t t2Count = t2.getSizeT();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    t2.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }

    gkg::Volume< float > dw;
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( dw );
    dwVolumeReaderProcess.execute( fileNameDW );

    if ( ( dw.getSizeX() != sizeX ) ||
         ( dw.getSizeY() != sizeY ) ||
         ( dw.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error("T2 and DW volumes are not of the same size");

    }

    gkg::Vector3d< double > tmpResolution;
    dw.getResolution( tmpResolution );
    if ( tmpResolution != resolution )
    {

      throw std::runtime_error( "T2 and DW voxel resolutions are not of the "
                                "same resolution" );

    }

    int32_t orientationCount = dw.getSizeT();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mask of signal
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMaskOfSignal << "' : " << std::flush;

    }

    gkg::Volume< int16_t > maskOfSignal;
    gkg::Reader::getInstance().read( fileNameMaskOfSignal, maskOfSignal );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mask of noise
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > maskOfNoise;
    if ( !fileNameMaskOfNoise.empty() )
    {

      if ( verbose )
      {

        std::cout << "reading '" << fileNameMaskOfNoise << "' : " << std::flush;

      }

      gkg::Reader::getInstance().read( fileNameMaskOfNoise, maskOfNoise );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "building the mask of noise from the mask of signal : "
                  << std::flush;

      }

      maskOfNoise = maskOfSignal;

      gkg::Volume< int16_t >::iterator n = maskOfNoise.begin(),
                                       ne = maskOfNoise.end();
      while ( n != ne )
      {

        *n = ( *n ? 0 : 1 );
        ++ n;

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading Rician fit abacus
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameAbacus << "' : " << std::flush;

    }

    gkg::Volume< float > abacus;
    gkg::Reader::getInstance().read( fileNameAbacus, abacus );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building average T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building average T2 data : " << std::flush;

    }

    gkg::Volume< float > averageT2( sizeX, sizeY, sizeZ );
    gkg::Vector3d< int32_t > site;
    int32_t t;
    float average = 0.0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          average = 0.0;
          for ( t = 0; t < t2Count; t++ )
          {

            average += ( float )t2( site, t );

          }
          average /= ( float )t2Count;
          if ( average <= 0.0f )
          {

            average = EPSILON_INT_16_TYPE;

          }
          averageT2( site ) = average;

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing noise standard deviation
    // E(signal) = sigma * sqrt( Pi / 2 ) outside the brain mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing noise standard deviation : " << std::flush;

    }

    int32_t count = 0;
    float value = 0.0;
    
    if ( noiseStdDev <= 0.0f )
    {
    
      float noiseMean = 0.0;
      for ( t = 0; t < t2Count; t++ )
      {

        for ( site.z = 0; site.z < sizeZ; site.z++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y++ )
          {

            for ( site.x = 0; site.x < sizeX; site.x++ )
            {

              if ( maskOfNoise( site ) )
              {

        	value = ( float )t2( site, t );
        	noiseMean += value;
        	++ count;

              }

            }

          }

        }
      
      }

      noiseMean /= ( float )count;
      noiseStdDev = noiseMean / std::sqrt( M_PI / 2.0 );
     
    }
    
    float noiseStdDevSquare = noiseStdDev * noiseStdDev;

    if ( verbose )
    {

      std::cout << noiseStdDev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // retrieving vector of Rician fits for a given noise standard deviation
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Vector > gaussianMixtures;
    int32_t fitCount = abacus.getSizeX();
    int32_t parametersCount = abacus.getSizeZ();
    int32_t gaussianCount = parametersCount / 3;
    int32_t noiseStdDevInt = (int32_t)( noiseStdDev + 0.5 );
    int32_t x, z;
    for ( x = 0; x < fitCount; x++ )
    {
    
      gkg::Vector parameters( parametersCount );
      for ( z = 0; z < parametersCount; z++ )
      {
      
        parameters( z ) = abacus( x, noiseStdDevInt, z );
      
      }
      
      gaussianMixtures.push_back( parameters );
    
    }
    
    // free abacus (i.e. reallocate to only one voxel)
    abacus.reallocate();
    

    ////////////////////////////////////////////////////////////////////////////
    // computing normalized standard deviation map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing normalized standard deviation map : "
                << std::flush;

    }

    gkg::Volume< float > normalizedNoiseStdDevMap( sizeX, sizeY, sizeZ );
    float normalizedNoiseStdDev = 0.0;
    float averageNormalizedNoiseStdDev = 0.0;
    gkg::BoundingBox< int32_t > boundingBox( averageT2 );
    normalizedNoiseStdDevMap.fill( 0.0 );
    gkg::Vector3d< int32_t > offset;
    gkg::Vector3d<int32_t> neighbor;
    count = 0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {

            value = averageT2( site );
            if ( value > 0 )
            {

              normalizedNoiseStdDev = noiseStdDev / value;
              normalizedNoiseStdDevMap( site ) = normalizedNoiseStdDev;
              averageNormalizedNoiseStdDev += normalizedNoiseStdDev;

            }
            else
            {

              float localAverageT2 = 0.0;
              int32_t neighborCount = 0;
              for ( offset.z = -1; offset.z <= 1; offset.z++ )
              {

                for ( offset.y = -1; offset.y <= 1; offset.y++ )
                {

                  for ( offset.x = -1; offset.x <= 1; offset.x++ )
                  {

                    neighbor = site + offset;
                    if ( boundingBox.contains( neighbor ) )
                    {

                      localAverageT2 += averageT2( neighbor );
                      ++ neighborCount;

                    }

                  }

                }

              }
              localAverageT2 /= ( float )neighborCount;
              normalizedNoiseStdDev = noiseStdDev / localAverageT2;
              normalizedNoiseStdDevMap( site ) = normalizedNoiseStdDev;
              averageNormalizedNoiseStdDev += normalizedNoiseStdDev;

            }
            ++ count;

          }

        }

      }

    }

    averageNormalizedNoiseStdDev /= ( float )count;
    if ( verbose )
    {

      std::cout <<  averageNormalizedNoiseStdDev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing normalized noise standard deviation map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameNormalizedNoiseStdDevMap.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing normalized noise standard deviation map : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameNormalizedNoiseStdDevMap,
                                        normalizedNoiseStdDevMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing spatial weights
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "spatial weight(s) : " << std::flush;

    }

    std::vector< float > spatialWeights( neighborhoodSize *
                                         neighborhoodSize *
                                         neighborhoodSize );                            

    int32_t halfNeighborhoodSize = neighborhoodSize / 2;
    int32_t neighborIndex = 0;
    for ( offset.z = -halfNeighborhoodSize;
          offset.z <= halfNeighborhoodSize; offset.z++ )
    {

      for ( offset.y = -halfNeighborhoodSize;
            offset.y <= halfNeighborhoodSize; offset.y++ )
      {

        for ( offset.x = -halfNeighborhoodSize;
              offset.x <= halfNeighborhoodSize; offset.x++ )
        {

          spatialWeights[ neighborIndex ] = 
             ( float )std::exp( -0.5 * offset.getNorm2() / 
                                spatialWeightFactorSquare );
          if ( verbose )
          {

            std::cout << offset << "->" << spatialWeights[ neighborIndex ]
                      << " "
                      << std::flush;

          }
          ++ neighborIndex;

        }

      }

    }


    if ( verbose )
    {

      std::cout << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // denoising T2 volume from Rician noise with Aja-Fernandez method
    // with spatial means through neighborhoods
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "denoising T2 volume : " << std::flush;

    }

    gkg::Volume< float > denoisedT2( sizeX, sizeY, sizeZ );
    denoisedT2.getHeader().addAttribute( "resolutionX", resolution.x );
    denoisedT2.getHeader().addAttribute( "resolutionY", resolution.y );
    denoisedT2.getHeader().addAttribute( "resolutionZ", resolution.z );
    denoisedT2.fill( 0 );

    float localSquareMean = 0.0;
    float localFourthPowerMean = 0.0;
    float valueSquare = 0.0;
    float sumOfWeights = 0.0;
    float weight = 0.0;
    float K = 1.0;
    float denominator = 1.0;
    float denoisedT2Square = 0.0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {

            neighborIndex = 0;
            sumOfWeights = 0;
            localSquareMean = 0.0;
            localFourthPowerMean = 0.0;

            for ( offset.z = -halfNeighborhoodSize;
                  offset.z <= halfNeighborhoodSize; offset.z++ )
            {

              for ( offset.y = -halfNeighborhoodSize;
                    offset.y <= halfNeighborhoodSize; offset.y++ )
              {

                for ( offset.x = -halfNeighborhoodSize;
                      offset.x <= halfNeighborhoodSize; offset.x++ )
                {

                  neighbor = site + offset;
                  if ( boundingBox.contains( neighbor ) && 
                       maskOfSignal( neighbor ) )
                  {

                    value = averageT2( neighbor );
                    valueSquare = value * value;
		    
                    if ( noWeighting || onlyStructuralWeighting )
                    {

                      weight = 1.0;

                    }
                    else
                    {

                      weight = spatialWeights[ neighborIndex ];

                    }
		    
                    localSquareMean += weight * valueSquare;
                    localFourthPowerMean += weight * valueSquare * valueSquare;
                    sumOfWeights += weight;

                  }
		  
		  ++ neighborIndex;

                }

              }

            }

            localSquareMean /= sumOfWeights;
            localFourthPowerMean /= sumOfWeights;

            // processing noise free T2 estimate
            K = 1.0;
            denominator = localFourthPowerMean -
                          localSquareMean * localSquareMean;
            value = averageT2( site );
            if ( denominator > EPSILON )
            {

              K = 1 - ( 4 * noiseStdDevSquare * 
                        ( localSquareMean - noiseStdDevSquare ) ) /
                      denominator;
              K = std::max( K, 0.0f );
              valueSquare = value * value;
              denoisedT2Square = localSquareMean - 2 * noiseStdDevSquare +
                                 K * ( valueSquare - localSquareMean );
              if ( denoisedT2Square > 0.0 )
              {

                denoisedT2( site ) =
                            ( float )( std::sqrt( denoisedT2Square ) + 0.5f );

              }
              else
              {

                denoisedT2( site ) = ( float )value;

              }

            }
            else
            {

              denoisedT2( site ) = ( float )value;

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the denoised T2 volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing denoised T2 volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameDenoisedT2, denoisedT2 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting Q-space sampling from input DW data
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::QSpaceSampling > qSpaceSampling(
                       gkg::QSpaceSamplingFactory::getInstance().create( dw ) );
    std::string qSpaceSamplingName = "";
    bool isCartesian = qSpaceSampling->isCartesian();
    bool isSpherical = qSpaceSampling->isSpherical();

    if ( verbose )
    {

      if ( isCartesian )
      {
      
        std::cout << "sampling type : cartesian" << std::endl; 
        qSpaceSamplingName = "cartesian";

      }
      else if ( isSpherical )
      {
      
        std::cout << "sampling type : spherical" << std::flush; 
        qSpaceSamplingName = "spherical";

      }

    }

    bool isSingleShell = false;
    bool isMultipleShell = false;
    if ( isSpherical )
    {
    
      gkg::SphericalQSpaceSampling*
        sphericalQSpaceSampling = static_cast< gkg::SphericalQSpaceSampling* >(
	  qSpaceSampling.get() );
	  
      isSingleShell = sphericalQSpaceSampling->isSingleShell();
      isMultipleShell = sphericalQSpaceSampling->isMultipleShell();

      if ( isSingleShell )
      {
      
        if ( verbose )
        {

          std::cout << ", single-shell" << std::endl;

        }

        qSpaceSamplingName += "_single-shell";

      }
      else if ( isMultipleShell )
      {
      
        if ( verbose )
        {
      
          std::cout << ", multiple-shell" << std::endl;

        }

        qSpaceSamplingName += "_multiple-shell";

      }
    
    }

    // sanity checks; we actually restrict the resampling to spherical
    // single-shell Q-space acquisitions
    if ( !isSpherical || !isSingleShell )
    {

      throw std::runtime_error(
           "tool actually restricted to spherical single-shell Q-space data" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building spherical harmonics reconstruction and observation matrices
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building SH reconstruction and observation matrices : "
                << std::flush;

    }

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance(). getImplementationFactory();

    // processing maximum j according to maximum order
    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
                  ( maximumSphericalHarmonicOrder + 2 ) ) / 2;


    // building matrix of spherical harmonics on input orientation(s)
    gkg::Vector3d< float > orientation;
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;

    gkg::Matrix B( orientationCount, N );

    int32_t o, n;
    for ( o = 0; o < orientationCount; o++ )
    {

      orientation = qSpaceSampling->getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        B( o, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Laplace-Beltrami smoothness matrix for regularization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l, m;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

/*
    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L )^(-1) B^T
    gkg::Matrix Bplus( N, orientationCount );
    factory->getThikonovPseudoInverse( B, L, laplaceBeltramiFactor, Bplus );

    // building Funk-Hecke matrix P
    gkg::Matrix P( N, N );
    P.setZero();
    int32_t k;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, k );
      P( n, n ) = 2 * M_PI * ( factory->getLegendrePolynomial( l, 0.0 ) /
                               factory->getLegendrePolynomial( l, 1.0 ) );

    }

    // building inverse Funk-Hecke matrix Pinv
    gkg::Matrix Pinv( N, N );
    Pinv.setZero();
    for ( n = 0; n < N; n++ )
    {

      Pinv( n, n ) = 1 / P( n, n );

    }

    // building reconstruction matrix
    gkg::Matrix reconstructionMatrix = P.getComposition( Bplus );

    // building observation matrix
    gkg::Matrix observationsMatrix = B.getComposition( Pinv );
    // factory->getMoorePenrosePseudoInverse( reconstructionMatrix,
    //                                       observationsMatrix ); 
*/
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building Kalman structure
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building parallel Kalman structure : " << std::flush;

    }

    gkg::ParallelKalmanFilter kalmanFilter( B, gaussianCount );
    gkg::Volume< gkg::Kalman::Parameters* > kalmanLut( sizeX,
                                                       sizeY,
                                                       sizeZ );
    kalmanLut.setResolution( resolution );
    kalmanLut.fill( 0 );

    int32_t neighborhoodSizeCube = neighborhoodSize * 
                                   neighborhoodSize * 
				   neighborhoodSize;
    gkg::Volume< gkg::RCPointer< std::vector< float > > > structuralWeights( 
                                                                        sizeX, 
							                sizeY, 
							                sizeZ );

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) && ( averageT2( site ) > 0 ) )
          {

            gkg::Kalman::Parameters* parameters = new gkg::Kalman::Parameters( 
                                                                            N );

            // allocating Kalman filter parameters
            kalmanLut( site ) = parameters;

            // allocating the state vector X
            parameters->X.reallocate( N );

            // initializing the state vector X
            parameters->X.setZero();
            
            // allocating the matrix varianceOfX
            parameters->varianceOfX.reallocate( N, N );

            // initializing the matrix varianceOfX
            for ( n = 0; n < N; n++ )
            {

	      parameters->varianceOfX( n, n ) = 1.0 / ( INITIAL_VARIANCE_C +
	                                    laplaceBeltramiFactor * L( n, n ) );
	      
	    }
	    
	    structuralWeights( site ).reset( new std::vector< float >( 
	                                                   neighborhoodSizeCube, 
				                           1.0f ) );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // denoising DW volume from Rician noise with Aja-Fernandez method
    // with spatial means through neighborhoods
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "denoising DW volume : " << std::flush;

    }

    gkg::Volume< float > denoisedDW = dw;

    float denoisedDWSquare = 0.0;
    float denoisedDWvalue = 0.0;
    float valueT2 = 0.0;


    gkg::Volume< float > volumeK( sizeX, sizeY, sizeZ, orientationCount );
    volumeK.getHeader().addAttribute( "resolutionX", resolution.x );
    volumeK.getHeader().addAttribute( "resolutionY", resolution.y );
    volumeK.getHeader().addAttribute( "resolutionZ", resolution.z );
    volumeK.fill( 0 );

    // looping over orientation(s)
    for ( o = 0; o < orientationCount; o++ )
    {

      if ( verbose )
      {

        if ( o )
        {

          std::cout << gkg::Eraser(22);

        }
        std::cout << " [ " << std::setw( 7 ) << o + 1
                  << " / " << std::setw( 7 ) << orientationCount
                  << " ]" << std::flush;

      }

      // updating Kalman parameter(s)
      for ( site.z = 0; site.z < sizeZ; site.z++ )
      {

        if ( verbose )
        {

          if ( site.z )
          {

            std::cout << gkg::Eraser(22);

          }
          std::cout << " [ " << std::setw( 7 ) << site.z + 1
                    << " / " << std::setw( 7 ) << sizeZ
                    << " ]" << std::flush;

        }

        for ( site.y = 0; site.y < sizeY; site.y++ )
        {

          for ( site.x = 0; site.x < sizeX; site.x++ )
          {

            if ( maskOfSignal( site ) && ( averageT2( site ) > 0 ) )
            {

              neighborIndex = 0;
              sumOfWeights = 0.0;
              for ( offset.z = -halfNeighborhoodSize;
                    offset.z <= halfNeighborhoodSize; offset.z++ )
              {

                for ( offset.y = -halfNeighborhoodSize;
                      offset.y <= halfNeighborhoodSize; offset.y++ )
                {

                  for ( offset.x = -halfNeighborhoodSize;
                        offset.x <= halfNeighborhoodSize; offset.x++ )
                  {

                    neighbor = site + offset;
                    if ( boundingBox.contains( neighbor ) && 
                         maskOfSignal( neighbor ) )
                    {

                      value = ( float )dw( neighbor, o );
                      valueSquare = value * value;
                      if ( noWeighting )
                      {

                        weight = 1.0;

                      }
                      else if ( onlySpatialWeighting )
                      {

                        weight = spatialWeights[ neighborIndex ];

                      }
                      else if ( onlyStructuralWeighting )
                      {

                        weight = ( *structuralWeights( site ) )[
			                                        neighborIndex ];

                      }
                      else
                      {

                        weight = spatialWeights[ neighborIndex ] *
                                 ( *structuralWeights( site ) )[ 
				                                neighborIndex ];

                      }

                      localSquareMean += weight * valueSquare;
                      localFourthPowerMean += weight * valueSquare *
                                                       valueSquare;
                      sumOfWeights += weight;

                    }

                    ++ neighborIndex;

                  }

                }

              }

              localSquareMean /= sumOfWeights;
              localFourthPowerMean /= sumOfWeights;

              // processing noise free T2 estimate
              K = 1.0;
              denominator = localFourthPowerMean -
                            localSquareMean * localSquareMean;
              value = dw( site, o );
	      valueT2 = denoisedT2( site );

              if ( denominator > EPSILON )
              {

                K = 1 - ( 4 * noiseStdDevSquare * 
                        ( localSquareMean - noiseStdDevSquare ) ) /
                        denominator;
                volumeK( site, o ) = K;
                K = std::max( K, 0.0f );
                valueSquare = value * value;
                denoisedDWSquare = localSquareMean - 2 * noiseStdDevSquare +
                                   K * ( valueSquare - localSquareMean );
                if ( denoisedDWSquare > 0.0 )
                {

                  denoisedDWvalue = std::sqrt( denoisedDWSquare );

                }
                else
                {

                  denoisedDWvalue = value;

                }

              }
              else
              {

                denoisedDWvalue = value;

              }
	      
	      gkg::Vector gaussianMixtureFit = gaussianMixtures[ 
	                                ( int32_t )( denoisedDWvalue + 0.5f ) ];

              int32_t p;
	      for ( p = 0; p < parametersCount; p += 3 )
	      {
	      
	        gaussianMixtureFit( p + 1 ) -= denoisedDWvalue;
		gaussianMixtureFit( p + 1 ) /= valueT2;
		gaussianMixtureFit( p + 2 ) /= valueT2;
	      
	      }

              // iterating Kalman filter for the normalized DW observation
              kalmanFilter.iterate( *kalmanLut( site ),
	                            gaussianMixtureFit,
                                    ( double )value / valueT2,
                                    o );

            }

          }

        }

      }

      // building structural weight(s)
      if ( !noWeighting && !onlySpatialWeighting )
      {
        double difference = 0.0;
        double squareDifference = 0.0;
        for ( site.z = 0; site.z < sizeZ; site.z++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y++ )
          {

            for ( site.x = 0; site.x < sizeX; site.x++ )
            {

              if ( maskOfSignal( site ) && ( averageT2( site ) > 0 ) )
              {

                const gkg::Vector& siteX = kalmanLut( site )->X;
                neighborIndex = 0;
                for ( offset.z = -halfNeighborhoodSize;
                      offset.z <= halfNeighborhoodSize; offset.z++ )
                {

                  for ( offset.y = -halfNeighborhoodSize;
                        offset.y <= halfNeighborhoodSize; offset.y++ )
                  {

                    for ( offset.x = -halfNeighborhoodSize;
                          offset.x <= halfNeighborhoodSize; offset.x++ )
                    {

                      neighbor = site + offset;
                      if ( boundingBox.contains( neighbor ) &&
                           maskOfSignal( neighbor ) )
                      {

                        const gkg::Vector& neighborX = kalmanLut( neighbor )->X;
                        difference = 0.0;
                        squareDifference = 0.0;
                        for ( n = 0; n < N; n++ )
                        {

                          difference = neighborX( n ) - siteX( n );
                          squareDifference += difference * difference;

                        }
                        ( *structuralWeights( site ) )[ neighborIndex ] =
                              ( float )std::exp( -0.5 * squareDifference /
                                                 structuralWeightFactorSquare );

                      }
                      ++ neighborIndex;

                    }

                  }

                }

              }

            }

          }

        }

      }
      if ( verbose )
      {

        if ( site.z )
        {

          std::cout << gkg::Eraser(22);

        }

      }
/*
      int32_t op;
      for ( op = 0; op <= o; op++ )
      {

        for ( site.z = 0; site.z < sizeZ; site.z++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y++ )
          {

            for ( site.x = 0; site.x < sizeX; site.x++ )
            {

              if ( kalmanLut( site ) )
              {

                const gkg::Vector& siteX = kalmanLut( site )->X;
                value = 0.0;
                for ( n = 0; n < N; n++ )
                {

                  value += ( float )( siteX( n ) * B( op, n ) );

                }

                denoisedDW( site , op ) = value * denoisedT2( site );
                if ( denoisedDW( site, op ) < 0 )
                {

                  denoisedDW( site, op ) = 0;

                }

              }

            }

          }

        }

      }
      gkg::Writer::getInstance().write( std::string( "denoised_" ) +
                                        gkg::StringConverter::toString( o ),
                                        denoisedDW );
*/
    }

    gkg::Writer::getInstance().write( "volume_K", volumeK );

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // recomposing denoised DW signal
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "recomposing denoised DW signal : " << std::flush;

    }

    for ( o = 0; o < orientationCount; o++ )
    {

      for ( site.z = 0; site.z < sizeZ; site.z++ )
      {

        for ( site.y = 0; site.y < sizeY; site.y++ )
        {

          for ( site.x = 0; site.x < sizeX; site.x++ )
          {

            if ( kalmanLut( site ) )
            {

              const gkg::Vector& siteX = kalmanLut( site )->X;
              value = 0.0;
              for ( n = 0; n < N; n++ )
              {

                value += ( float )( siteX( n ) * B( o, n ) );

              }

              denoisedDW( site , o ) = ( float )( value * 
                                                    denoisedT2( site ) );
              if ( denoisedDW( site, o ) < 0 )
              {

                denoisedDW( site, o ) = 1;

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the denoised DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing denoised DW volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameDenoisedDW, denoisedDW );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // deallocating kalman parameter(s)
    ////////////////////////////////////////////////////////////////////////////

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( kalmanLut( site ) )
          {

            delete kalmanLut( site );

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::DwiParallelNoiseFilterCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameMaskOfNoise, "
             "const std::string& fileNameAbacus, "
             "int32_t neighborhoodSize, "
             "const std::string& fileNameDenoisedT2, "
             "const std::string& fileNameDenoisedDW, "
             "const std::string& fileNameNormalizedNoiseStdDevMap, "
             "int32_t maximumSphericalHarmonicOrder, "
             "double laplaceBeltramiFactor, "
             "double spatialWeightFactor, "
             "double structuralWeightFactor, "
             "float noiseStdDev, "
             "bool noWeighting, "
             "bool onlySpatialWeighting, "
             "bool onlyStructuralWeighting, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiParallelNoiseFilterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskOfSignal ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskOfNoise ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameAbacus ) +
    DECLARE_INTEGER_PARAMETER_HELP( neighborhoodSize ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDenoisedT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDenoisedDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameNormalizedNoiseStdDevMap ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumSphericalHarmonicOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP( laplaceBeltramiFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( spatialWeightFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( structuralWeightFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( noiseStdDev ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( noWeighting ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( onlySpatialWeighting ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( onlyStructuralWeighting ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef EPSILON_INT_16_TYPE
#undef EPSILON
#undef INITIAL_VARIANCE_C

