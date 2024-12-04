#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel1.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::DiffusionGaussianModel1::DiffusionGaussianModel1(
                                           double volumeFraction,
                                           double fastRatio,
                                           const gkg::Matrix& fastDirectTensor,
                                           const gkg::Matrix& slowDirectTensor,
                                           double bValue )
                             : _volumeFraction( volumeFraction ),
                               _fastRatio( fastRatio ),
                               _fastDirectTensor( fastDirectTensor ),
                               _slowDirectTensor( slowDirectTensor ),
                               _bValue( bValue )
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

  }
  GKG_CATCH( "gkg::DiffusionGaussianModel1::DiffusionGaussianModel1( "
             "double volumeFraction, "
             "double fastRatio, "
             "const gkg::Matrix& fastDirectTensor, "
             "const gkg::Matrix& slowDirectTensor, "
             "double bValue )" );

}


gkg::DiffusionGaussianModel1::~DiffusionGaussianModel1()
{
}


float gkg::DiffusionGaussianModel1::getSignalContribution(
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
  GKG_CATCH( "float gkg::DiffusionGaussianModel1::getSignalContribution( "
             "const gkg::Vector& u ) const" );

}


float gkg::DiffusionGaussianModel1::getOdfContribution(
                                                    const gkg::Vector& u ) const
{

  try
  {

    double fastDotProduct = u.getComposition( _fastInverseTensor.getComposition(
                                                                          u ) );
    double slowDotProduct = u.getComposition( _slowInverseTensor.getComposition(
                                                                          u ) );

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
  GKG_CATCH( "float gkg::DiffusionGaussianModel1::getOdfContribution( "
             "const gkg::Vector& u ) const" );

}
