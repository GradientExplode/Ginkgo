#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel2.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::DiffusionGaussianModel2::DiffusionGaussianModel2(
                                           double volumeFraction,
                                           double fastRatio,
                                           const gkg::Matrix& fastDirectTensor,
                                           const gkg::Matrix& slowDirectTensor,
                                           double bValue,
                                           double bigDelta,
                                           double littleDelta,
                                           double radius )
                             : _volumeFraction( volumeFraction ),
                               _fastRatio( fastRatio ),
                               _fastDirectTensor( fastDirectTensor ),
                               _slowDirectTensor( slowDirectTensor ),
                               _bValue( bValue ),
                               _bigDelta( bigDelta ),
                               _littleDelta( littleDelta ),
                               _radius( radius )
{

  try
  {

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

    // processing q
    _q = ( 1.0 / ( 2 * M_PI ) ) * std::sqrt( _bValue /
                                             ( _bigDelta - _littleDelta / 3 ) );

  }
  GKG_CATCH( "gkg::DiffusionGaussianModel2::DiffusionGaussianModel2( "
             "double volumeFraction, "
             "double fastRatio, "
             "const gkg::Matrix& fastDirectTensor, "
             "const gkg::Matrix& slowDirectTensor, "
             "double bValue, "
             "double bigDelta, "
             "double littleDelta, "
             "double radius )" );

}


gkg::DiffusionGaussianModel2::~DiffusionGaussianModel2()
{
}


float gkg::DiffusionGaussianModel2::getSignalContribution(
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
  GKG_CATCH( "float gkg::DiffusionGaussianModel2::getSignalContribution( "
             "const gkg::Vector& u ) const" );

}


float gkg::DiffusionGaussianModel2::getOdfContribution(
                                                    const gkg::Vector& u ) const
{

  try
  {

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    double fastDotProduct = u.getComposition( _fastInverseTensor.getComposition(
                                                                          u ) );
    double slowDotProduct = u.getComposition( _slowInverseTensor.getComposition(
                                                                          u ) );

    return ( float )( _volumeFraction *
                      (
                        ( _fastRatio /
                          std::sqrt( _fastDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / fastDotProduct ) *
                        factory->getErfC( std::sqrt( fastDotProduct /
                                                     ( 4 * _bValue ) ) *
                                          _q * _radius ) +
                        ( ( 1.0 - _fastRatio ) /
                          std::sqrt( _slowDirectDeterminant ) ) *
                        std::sqrt( ( M_PI * _bValue ) / slowDotProduct ) *
                        factory->getErfC( std::sqrt( slowDotProduct /
                                                     ( 4 * _bValue ) ) *
                                          _q * _radius )
                      )
                    );

  }
  GKG_CATCH( "float gkg::DiffusionGaussianModel2::getOdfContribution( "
             "const gkg::Vector& u ) const" );

}
