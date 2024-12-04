#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <map>
#include <cmath>


#define EPSILON  ( double )1e-3
#define NBINS    4096


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string inputFileName;
    std::string noisyFileName;
    int32_t neighborhoodSize = 5;
    bool verbose = false;
    
    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc,
                                  argv, "Estimate the noise std deviation on"
                                  " an image assuming a gaussian"
                                  " noise " );
    application.addSingleOption( "-r",
                                 "Reference file name",
                                 inputFileName );
    application.addSingleOption( "-i",
                                 "Noisy file name",
                                 noisyFileName );
    application.addSingleOption( "-n",
                                 "Neighborhood size (default=5)",
                                 neighborhoodSize,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();


    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << inputFileName << "' : " << std::flush;

    }
    gkg::Volume< int16_t > inputVolume;
    gkg::Reader::getInstance().read( inputFileName, inputVolume );

    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading noisy data
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

     std::cout << "reading '" << noisyFileName << "' : " << std::flush;

    }
    gkg::Volume< int16_t > noisyVolume;
    gkg::Reader::getInstance().read( noisyFileName, noisyVolume );

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    noisyVolume.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////

    int32_t nb_neigh = neighborhoodSize / 2;

    if ( nb_neigh < 1 )
    {

      nb_neigh = 1;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing noise standard deviation: variance method
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > localVarianceMap( sizeX, sizeY, sizeZ );
    gkg::Volume< float > lmmseMap( sizeX, sizeY, sizeZ );
    gkg::Vector3d< int32_t > neighbor;
    gkg::BoundingBox< int32_t > bBox( noisyVolume );
    gkg::Vector3d< int32_t > site;
    gkg::Vector3d< int32_t > offset;
    std::map< int32_t, uint64_t > histoVariances;
    double min = std::numeric_limits< double >::max();
    double max = std::numeric_limits< double >::lowest();
    double mse = 0.0;
    double Nmse = (double)inputVolume.getSizeXYZ();

    localVarianceMap.fill( 0.0 );
    lmmseMap.fill( 0.0 );

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          double neighborCount = 0.0;
          double localMean = 0.0;
          double localSquaredMean = 0.0;
          double m = (double)noisyVolume( site );
          double e = m - (double)inputVolume( site );

          mse += e * e;

          for ( offset.y = -nb_neigh; offset.y <= nb_neigh; offset.y++ )
          {

            for ( offset.x = -nb_neigh; offset.x <= nb_neigh; offset.x++ )
            {

              neighbor = site + offset;

              if ( bBox.contains( neighbor ) )
              {

                double value = ( double )noisyVolume( site + offset );
                localMean += value;
                localSquaredMean += value * value;
                neighborCount += 1.0;

              }

            }

          }

          if ( neighborCount > 0.0 )
          {

            localMean /= neighborCount;
            localSquaredMean /= neighborCount;

            double localVariance = localSquaredMean - localMean * localMean;

            localVarianceMap( site ) = float( localVariance );
            lmmseMap( site ) = float( ( m - localMean ) / 
                                      ( localVariance + EPSILON ) );

            if ( localVariance < min )
            {

              min = localVariance;

            }

            if ( localVariance > max )
            {

              max = localVariance;

            }

          }

        }

      }

    }

    uint64_t histo[ NBINS + 1 ];
    double fact = double( NBINS ) / ( max - min );
    std::memset( histo, 0, ( NBINS + 1 ) * sizeof( uint64_t ) );

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          histo[ int32_t( fact * ( localVarianceMap( site ) - min ) ) ]++;

        }

      }

    }

    uint64_t hmax = 0;
    int32_t imax = 0;
    int32_t i;

    for ( i = 0; i < NBINS; i++ )
    {

      if ( histo[ i ] > hmax )
      {

        hmax = histo[ i ];
        imax = i;

      }

    }

    double varianceMode = double( imax * ( max - min ) / 
                                  double( NBINS ) + min );
    double noiseStdDev1 = std::sqrt( varianceMode );

    std::cout << "estimated sigma from local variances = " 
              << noiseStdDev1 << std::endl;
    std::cout << "RMSD(noisy): epsilon = " << std::sqrt( mse / Nmse ) 
              << std::endl;


    ////////////////////////////////////////////////////////////////////////////
    // Gaussian denoising
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > denoisedVolume( sizeX, sizeY, sizeZ );
    denoisedVolume.setResolution( resolution );

    int32_t dim = 2 * nb_neigh + 1;
    double* g = new double[ dim * dim ];

    if ( g )
    {

      double d0 = std::sqrt( 2.0 * M_PI ) * noiseStdDev1;
      double d1 = -2.0 * varianceMode;
      int32_t x, y;

      for ( i = 0, y = -nb_neigh; y <= nb_neigh; y++ )
      {

        for ( x = -nb_neigh; x <= nb_neigh; x++, i++ )
        {

          g[ i ] = std::exp( double( x * x + y * y ) / d1 ) / d0;

        }

      }

      denoisedVolume.fill( 0 );
      mse = 0.0;

      for ( site.z = 0; site.z < sizeZ; site.z++ )
      {

        for ( site.y = 0; site.y < sizeY; site.y++ )
        {

          for( site.x = 0; site.x < sizeX; site.x++ )
          {

            double convol = 0.0;
            int32_t j = 0;

            for ( offset.y = -nb_neigh; offset.y <= nb_neigh; offset.y++ )
            {

              for ( offset.x = -nb_neigh; 
                    offset.x <= nb_neigh; 
                    offset.x++, j++ )
              {

                neighbor = site + offset;

                if ( bBox.contains( neighbor ) )
                {

                  convol += ( double )noisyVolume( neighbor ) * g[ j ];

                }

              }

            }

            denoisedVolume( site ) = int16_t( convol + 0.5 );

            double e = convol - (double)inputVolume( site );
            mse += e * e;

          }

        }

      }

      std::cout << "RMSD(gaussian): epsilon = " << std::sqrt( mse / Nmse )
                << std::endl;
      gkg::Writer::getInstance().write( "denoisedGauss.ima", denoisedVolume );

    }


    ////////////////////////////////////////////////////////////////////////////
    // lmmse denoising
    ////////////////////////////////////////////////////////////////////////////

    denoisedVolume.fill( 0 );
    mse = 0.0;

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          double value = ( double )noisyVolume( site ) -
                          ( varianceMode + EPSILON ) * lmmseMap( site );

          denoisedVolume( site ) = int16_t( value + 0.5 );

          double e = value - (double)inputVolume( site );
          mse += e * e;

        }

      }

    }

    std::cout << "RMSD(lmmse): epsilon = " << std::sqrt( mse / Nmse ) 
              << std::endl;
    gkg::Writer::getInstance().write( "denoisedLMMSE.ima", denoisedVolume );


  }  
  GKG_CATCH_COMMAND( result );

  return result;

}
