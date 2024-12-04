#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-registration/RegistrationAlgorithm_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>

#include <sys/time.h>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string t1FileName;
    std::string t2FileName;
    std::string mniFileName;
    std::string aalFileName;
    std::string transformName;

    gkg::Application application( argc, argv,
                                  "AAL template to T2 transform tester" );
    application.addSingleOption( "-t1", "T1 image", t1FileName );
    application.addSingleOption( "-t2", "T2 image", t2FileName );
    application.addSingleOption( "-m", "T1 MNI template", mniFileName );
    application.addSingleOption( "-a", "AAL template", aalFileName );
    application.addSingleOption( "-o",
                                 "Output resampled AAL file name",
                                 transformName );

    application.initialize();

    gkg::RCPointer< gkg::Volume< uint16_t > > t1Vol(
                                                  new gkg::Volume< uint16_t > );
    gkg::Volume< uint16_t > t2Vol;
    gkg::Volume< int16_t > aalVol;
    gkg::RCPointer< gkg::Volume< int16_t > > mniVol( 
                                                   new gkg::Volume< int16_t > );
    gkg::Vector initialParameters( 12 );
    std::vector< double > optimizerParameters( 14 );
    std::vector< double > similarityParameters( 2 );
    std::list< int32_t > samplingMaxSizes;
    gkg::RCPointer< gkg::VectorFieldTransform3d< float > > 
      vectorFieldTransform3d;
    gkg::RCPointer< gkg::NonLinearTransform3d< float > > nonLinearTransform3d;
    gkg::RCPointer< gkg::ParameterizedAffineTransform3d< float > >
      affineTransform3d;

    affineTransform3d.reset( new gkg::ParameterizedAffineTransform3d< float > );
    nonLinearTransform3d.reset(
             new gkg::NonLinearTransform3d< float >( affineTransform3d,
                                                     vectorFieldTransform3d ) );

    optimizerParameters[ 0 ] = 1000;
    optimizerParameters[ 1 ] = 0.01;
    optimizerParameters[ 2 ] = 0.05;
    optimizerParameters[ 3 ] = 0.05;
    optimizerParameters[ 4 ] = 0.05;
    optimizerParameters[ 5 ] = 0.02;
    optimizerParameters[ 6 ] = 0.02;
    optimizerParameters[ 7 ] = 0.02;
    optimizerParameters[ 8 ] = M_PI / 18.0;
    optimizerParameters[ 9 ] = M_PI / 18.0;
    optimizerParameters[ 10 ] = M_PI / 18.0;
    optimizerParameters[ 11 ] = 50.0;
    optimizerParameters[ 12 ] = 50.0;
    optimizerParameters[ 13 ] = 50.0;
    
    similarityParameters[ 0 ] = 64.0;
    similarityParameters[ 1 ] = 1.0;

    samplingMaxSizes.push_back( 64 );

    std::cout << "Read T1 volume : " << t1FileName << std::flush;
    gkg::Reader::getInstance().read( t1FileName, *t1Vol );
    std::cout << " - done" << std::endl;

    std::cout << "Read T2 volume : " << t2FileName << std::flush;
    gkg::Reader::getInstance().read( t2FileName, t2Vol );
    std::cout << " - done" << std::endl;

    std::cout << "Read MNI template : " << mniFileName << std::flush;
    gkg::TypedVolumeReaderProcess< int16_t > readerProcess( *mniVol );
    readerProcess.execute( mniFileName );
    std::cout << " - done" << std::endl;

    std::cout << "Read AAL template : " << aalFileName << std::flush;
    gkg::Reader::getInstance().read( aalFileName, aalVol );
    std::cout << " - done" << std::endl;

    initialParameters( 0 ) = 1.0;
    initialParameters( 1 ) = 1.0;
    initialParameters( 2 ) = 1.0;
    initialParameters( 3 ) = 0.0;
    initialParameters( 4 ) = 0.0;
    initialParameters( 5 ) = 0.0;
    initialParameters( 6 ) = 0.0;
    initialParameters( 7 ) = 0.0;
    initialParameters( 8 ) = 0.0;
    initialParameters( 9 ) = 0.0;
    initialParameters( 10 ) = 0.0;
    initialParameters( 11 ) = 0.0;

    struct timeval tv_start_recal;
    struct timeval tv_stop_recal;
    struct timeval tv_stop_sampling;

    std::cout << "Registration T1 -> MNI" << std::flush;
    gettimeofday( &tv_start_recal, NULL );
    gkg::RegistrationAlgorithm< int16_t, uint16_t > algo( "mutual-information",
                                                          "nelder-mead",
                                                          1,
                                                          samplingMaxSizes,
                                                          similarityParameters,
                                                          optimizerParameters,
                                                          false );
    algo.match( mniVol, 
                t1Vol,
                initialParameters, 
                0.0, 
                0.0, 
                nonLinearTransform3d, 
                "" );
    std::cout << " - done" << std::endl;
    gettimeofday( &tv_stop_recal, NULL );

    if ( t1Vol->getHeader().hasAttribute( "transformations" ) &&
         t2Vol.getHeader().hasAttribute( "transformations" ) )
    {

      std::cout << "Registration T2 -> T1" << std::flush;
      gkg::GenericObjectList transfo;
      std::vector< float > coefs( 16 );

      t1Vol->getHeader().getAttribute( "transformations", transfo );
      gkg::GenericObjectList tr = 
                             transfo[ 0 ]->getValue< gkg::GenericObjectList >();

      for ( int32_t i = 0; i < 16; ++ i )
      {

        coefs[ i ] = tr[ i ]->getScalar();

      }

      gkg::HomogeneousTransform3d< float > R1( coefs, false );

      t2Vol.getHeader().getAttribute( "transformations", transfo );
      tr = transfo[ 0 ]->getValue< gkg::GenericObjectList >();

      for ( int32_t i = 0; i < 16; ++ i )
      {

        coefs[ i ] = tr[ i ]->getScalar();

      }

      gkg::HomogeneousTransform3d< float > R2( coefs, true );
      gkg::CompositeTransform3d< float > composite;

      composite.compose( R2 );
      composite.compose( R1 );
      composite.compose( 
                    nonLinearTransform3d->getParameterizedAffineTransform3d() );
      std::cout << " - done" << std::endl;

      gkg::Resampler< int16_t >* resampler = 
              gkg::ResamplerFactory< int16_t >::getInstance().getResampler( 0 );

      gkg::Volume< int16_t > outVol( t2Vol.getSizeX(),
                                     t2Vol.getSizeY(),
                                     t2Vol.getSizeZ() );
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      gkg::SplineResampler< int16_t >* splineResampler =
                  dynamic_cast< gkg::SplineResampler< int16_t >* >( resampler );
      if ( splineResampler )
      {

        splineResampler->reset();

      }

      t2Vol.getResolution( resolution );
      outVol.setResolution( resolution );

      std::cout << "Resampling" << std::flush;
      resampler->resample(
                  aalVol,
                  composite.getComposition().getInverseHomogeneousTransform3d(),
                  0,
                  outVol, true );
      std::cout << " - done" << std::endl;

      gettimeofday( &tv_stop_sampling, NULL );

      std::cout << "Save resampled AAL template : " 
                << transformName << std::flush;
      gkg::Writer::getInstance().write( transformName, outVol );
      std::cout << " - done" << std::endl;

      double t_recal = (double)( tv_stop_recal.tv_sec + 
                                 tv_stop_recal.tv_usec / 1000000.0 - 
                                 tv_start_recal.tv_sec - 
                                 tv_start_recal.tv_usec / 1000000.0 );
      double t_sampling = (double)( tv_stop_sampling.tv_sec + 
                                    tv_stop_sampling.tv_usec / 1000000.0 -
                                    tv_stop_recal.tv_sec + 
                                    tv_stop_recal.tv_usec / 1000000.0 );
      std::cout << "Registation : " << t_recal << " s" << std::endl;
      std::cout << "Resampling  : " << t_sampling << " s" << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
