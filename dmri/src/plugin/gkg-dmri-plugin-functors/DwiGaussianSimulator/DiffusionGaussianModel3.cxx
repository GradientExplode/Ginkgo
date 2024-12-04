#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel3.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define DEFAULT_ORIENTATION_COUNT 4000
#define EPSILON 1e-20


gkg::DiffusionGaussianModel3::DiffusionGaussianModel3(
                                           double volumeFraction,
                                           double fastRatio,
                                           const gkg::Matrix& fastDirectTensor,
                                           const gkg::Matrix& slowDirectTensor,
                                           double bValue,
                                           double snr )
                             : _volumeFraction( volumeFraction ),
                               _fastRatio( fastRatio ),
                               _bValue( bValue ),
                               _snr( snr )
{

  try
  {

    // processing noise contaminated tensor(s)
    _fastDirectTensor = getNoiseContaminatedTensor( fastDirectTensor );
    _slowDirectTensor = getNoiseContaminatedTensor( slowDirectTensor );

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    gkg::Matrix U( 3, 3 );
    gkg::Vector S( 3 );
    gkg::Matrix V( 3, 3 );
    gkg::Matrix Sinv( 3, 3 );

    // processing fast inverse tensor
    factory->getSingularValueDecomposition2( _fastDirectTensor,
                                             U, S, V );
    _fastDirectDeterminant = S( 0 ) * S( 1 ) * S( 2 );
    Sinv.setZero();
    Sinv( 0, 0 ) = 1.0 / S( 0 );
    Sinv( 1, 1 ) = 1.0 / S( 1 );
    Sinv( 2, 2 ) = 1.0 / S( 2 );
    _fastInverseTensor = V.getComposition( Sinv ).getComposition(
                                                         U.getTransposition() );

    // processing slow inverse tensor
    factory->getSingularValueDecomposition2( _slowDirectTensor,
                                             U, S, V );
    _slowDirectDeterminant = S( 0 ) * S( 1 ) * S( 2 );
    Sinv.setZero();
    Sinv( 0, 0 ) = 1.0 / S( 0 );
    Sinv( 1, 1 ) = 1.0 / S( 1 );
    Sinv( 2, 2 ) = 1.0 / S( 2 );
    _slowInverseTensor = V.getComposition( Sinv ).getComposition(
                                                         U.getTransposition() );

/*
    gkg::Vector eigenValues( 3 );
    gkg::Matrix eigenVectors( 3, 3 );
    gkg::Matrix diag( 3, 3 );

    factory->getEigenSystem( _fastInverseTensor,
                             eigenValues, eigenVectors );
    if ( eigenValues( 0 ) < 0.0 )
    {

       eigenValues( 0 ) = EPSILON;

    }
    if ( eigenValues( 1 ) < 0.0 )
    {

       eigenValues( 1 ) = EPSILON;

    }
    if ( eigenValues( 2 ) < 0.0 )
    {

       eigenValues( 2 ) = EPSILON;

    }
    diag.setZero();
    diag( 0, 0 ) = eigenValues( 0 );
    diag( 1, 1 ) = eigenValues( 1 );
    diag( 2, 2 ) = eigenValues( 2 );
    _fastInverseTensor = eigenVectors.getComposition( diag ).
                             getComposition( eigenVectors.getTransposition() );

    factory->getEigenSystem( _slowInverseTensor,
                             eigenValues, eigenVectors );
    if ( eigenValues( 0 ) < 0.0 )
    {

       eigenValues( 0 ) = EPSILON;

    }
    if ( eigenValues( 1 ) < 0.0 )
    {

       eigenValues( 1 ) = EPSILON;

    }
    if ( eigenValues( 2 ) < 0.0 )
    {

       eigenValues( 2 ) = EPSILON;

    }
    diag.setZero();
    diag( 0, 0 ) = eigenValues( 0 );
    diag( 1, 1 ) = eigenValues( 1 );
    diag( 2, 2 ) = eigenValues( 2 );
    _slowInverseTensor = eigenVectors.getComposition( diag ).
                             getComposition( eigenVectors.getTransposition() );
*/
  }
  GKG_CATCH( "gkg::DiffusionGaussianModel3::DiffusionGaussianModel3( "
             "double volumeFraction, "
             "double fastRatio, "
             "const gkg::Matrix& fastDirectTensor, "
             "const gkg::Matrix& slowDirectTensor, "
             "double bValue )" );

}


gkg::DiffusionGaussianModel3::~DiffusionGaussianModel3()
{
}


float gkg::DiffusionGaussianModel3::getSignalContribution(
                                                    const gkg::Vector& u ) const
{

  try
  {

    double fastDotProduct = u.getComposition( _fastDirectTensor.getComposition(
                                                                          u ) );
    double slowDotProduct = u.getComposition( _slowDirectTensor.getComposition(
                                                                          u ) );

    return ( float )( _volumeFraction *
                      ( _fastRatio *
                        std::exp( -_bValue * fastDotProduct ) +
                        ( 1.0 - _fastRatio ) *
                        std::exp( -_bValue * slowDotProduct )
                      )
                    );

  }
  GKG_CATCH( "float gkg::DiffusionGaussianModel3::getSignalContribution( "
             "const gkg::Vector& u ) const" );

}


float gkg::DiffusionGaussianModel3::getOdfContribution(
                                                    const gkg::Vector& u ) const
{

  try
  {

    double fastDotProduct = u.getComposition( _fastInverseTensor.getComposition(
                                                                          u ) );
    double slowDotProduct = u.getComposition( _slowInverseTensor.getComposition(
                                                                          u ) );
/*
std::cout << _fastRatio << " "
          << _fastDirectDeterminant << " " 
          << _slowDirectDeterminant << " "
          << slowDotProduct << " "
          << std::sqrt( _fastDirectDeterminant ) << " "
          << std::sqrt( _slowDirectDeterminant ) << " "
          << ( _fastRatio /
                          std::sqrt( _fastDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / fastDotProduct ) << " "
          << ( ( 1.0 - _fastRatio ) /
                          std::sqrt( _slowDirectDeterminant ) ) << " "
          << std::sqrt( ( M_PI * _bValue ) / slowDotProduct ) << " "
          << ( ( 1.0 - _fastRatio ) /
                          std::sqrt( _slowDirectDeterminant ) ) *
                        std::sqrt( ( 3.14 * _bValue ) / slowDotProduct ) << " "
          << ( _volumeFraction *
                      (
                        ( _fastRatio /
                          std::sqrt( _fastDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / fastDotProduct ) +
                        ( ( 1.0 - _fastRatio ) /
                          std::sqrt( _slowDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / slowDotProduct )
                      )
                    ) << std::endl;
*/
    return ( float )( _volumeFraction *
                      (
                        ( _fastRatio /
                          std::sqrt( _fastDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / fastDotProduct ) +
                        ( ( 1.0 - _fastRatio ) /
                          std::sqrt( _slowDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / slowDotProduct )
                      )
                    );

  }
  GKG_CATCH( "float gkg::DiffusionGaussianModel3::getOdfContribution( "
             "const gkg::Vector& u ) const" );

}


gkg::Matrix gkg::DiffusionGaussianModel3::getNoiseContaminatedTensor(
                                         const gkg::Matrix& directTensor ) const
{

  try
  {

    std::cout << "D_noise_free=" << std::endl << directTensor << std::endl;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building local orientation set
    gkg::ElectrostaticOrientationSet 
      orientationSet( DEFAULT_ORIENTATION_COUNT );

    // building U matrix
    gkg::Matrix U( DEFAULT_ORIENTATION_COUNT, 6 );
    gkg::Vector S( 6 );
    gkg::Matrix V( 6, 6 );
    int32_t o;
    for ( o = 0; o < U.getSize1(); o++ )
    {

      gkg::Vector3d< float >
        orientation = orientationSet.getOrientation( o );

      U( o, 0 ) = _bValue * orientation.x * orientation.x;
      U( o, 1 ) = 2 * _bValue * orientation.x * orientation.y;
      U( o, 2 ) = 2 * _bValue * orientation.x * orientation.z;
      U( o, 3 ) = _bValue * orientation.y * orientation.y;
      U( o, 4 ) = 2 * _bValue * orientation.y * orientation.z;
      U( o, 5 ) = _bValue * orientation.z * orientation.z;

    }

    // performing SVD decomposition
    factory->getSingularValueDecomposition( U, V, S );


    // performing noise contamination
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    double dwi, noise;
    gkg::Vector normalizedSignal( DEFAULT_ORIENTATION_COUNT );
    gkg::Vector coefficients( 6 );
    for ( o = 0; o < DEFAULT_ORIENTATION_COUNT; o++ )
    {

      gkg::Vector u( 3 );
      u( 0 ) = ( double )orientationSet.getOrientation( o ).x;
      u( 1 ) = ( double )orientationSet.getOrientation( o ).y;
      u( 2 ) = ( double )orientationSet.getOrientation( o ).z;
                       
      dwi = std::exp( -_bValue *
                      u.getComposition( directTensor.getComposition( u ) ) );
      noise = factory->getGaussianRandomNumber( randomGenerator,
                                                0.0,
                                                std::sqrt( 2.0/M_PI ) / _snr );
      normalizedSignal( o ) = std::log( 1.0 / std::sqrt( dwi * dwi +
                                                         noise * noise ) );

    }

    // fitting coefficients
    factory->getSingularValueSolution( U, S, V,
                                       normalizedSignal,
                                       coefficients );

    gkg::Matrix noiseContaminatedDirectTensor( 3, 3 );
    noiseContaminatedDirectTensor( 0, 0 ) = coefficients( 0 );
    noiseContaminatedDirectTensor( 0, 1 ) = coefficients( 1 );
    noiseContaminatedDirectTensor( 0, 2 ) = coefficients( 2 );
    noiseContaminatedDirectTensor( 1, 0 ) = coefficients( 1 );
    noiseContaminatedDirectTensor( 1, 1 ) = coefficients( 3 );
    noiseContaminatedDirectTensor( 1, 2 ) = coefficients( 4 );
    noiseContaminatedDirectTensor( 2, 0 ) = coefficients( 2 );
    noiseContaminatedDirectTensor( 2, 1 ) = coefficients( 4 );
    noiseContaminatedDirectTensor( 2, 2 ) = coefficients( 5 );

    std::cout << "D_noise_contaminated=" << std::endl
              << noiseContaminatedDirectTensor << std::endl;
/*
    gkg::Vector eigenValues( 3 );
    gkg::Matrix eigenVectors( 3, 3 );
    factory->getEigenSystem( noiseContaminatedDirectTensor,
                             eigenValues, eigenVectors );

    if ( eigenValues( 0 ) < 0.0 )
    {

       eigenValues( 0 ) = EPSILON;

    }
    if ( eigenValues( 1 ) < 0.0 )
    {

       eigenValues( 1 ) = EPSILON;

    }
    if ( eigenValues( 2 ) < 0.0 )
    {

       eigenValues( 2 ) = EPSILON;

    }

    gkg::Matrix diag( 3, 3 );
    diag.setZero();
    diag( 0, 0 ) = eigenValues( 0 );
    diag( 1, 1 ) = eigenValues( 1 );
    diag( 2, 2 ) = eigenValues( 2 );

    noiseContaminatedDirectTensor = eigenVectors.getComposition( diag ).
                             getComposition( eigenVectors.getTransposition() );
*/
    return noiseContaminatedDirectTensor;

  }
  GKG_CATCH( "gkg::Matrix "
             "gkg::DiffusionGaussianModel3::getNoiseContaminatedTensor( "
             "const gkg::Matrix& directTensor ) const" );

}


#undef DEFAULT_ORIENTATION_COUNT
#undef EPSILON
