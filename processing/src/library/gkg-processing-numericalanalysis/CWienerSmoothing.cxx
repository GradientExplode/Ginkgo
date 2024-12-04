#include <gkg-processing-numericalanalysis/CWienerSmoothing.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::CWienerSmoothing::CWienerSmoothing( const gkg::CMatrix& covariance )
                      : gkg::CNormWeighting()
{

  try
  {

    // sanity check
    if ( !covariance.isSquare() )
    {

      throw std::runtime_error( "covariance matrix is not square" );

    }

    // allocating weighting matrix
    int32_t size = covariance.getSize1();
    _weighting = new gkg::CMatrix( size, size );

    // getting concrete factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // calculating eign system
    gkg::CMatrix V( size, size );
    gkg::Vector D( size );
    factory->getEigenSystem( covariance, D, V );
    factory->sortEigenSystem( D, V,
                      gkg::NumericalAnalysisImplementationFactory::Descending );


    // taking inverse of square root
    int32_t i;
    for ( i = 0; i < size; i++ )
    {

      D( i ) = 1.0 / std::sqrt( D( i ) );

    }

    *_weighting = D.getDiagonal().getComposition(
                                                V.getHermitianTransposition() );

  }
  GKG_CATCH( "gkg::CWienerSmoothing::CWienerSmoothing( "
             "const gkg::CMatrix& covariance )" );

}


gkg::CWienerSmoothing::~CWienerSmoothing()
{

  if ( _weighting )
  {

    delete _weighting;

  }

}


const gkg::CMatrix& gkg::CWienerSmoothing::getWeighting() const
{

  return *_weighting;

}
