#include <gkg-processing-numericalanalysis/SVDInverser.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/FilterFactor.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


#define EPSILON 1e-18


gkg::SVDInverser::SVDInverser( const gkg::FilterFactor& filterFactor )
                 : gkg::IllMatrixInverser(),
                   _filterFactor( filterFactor )
{
}


gkg::SVDInverser::~SVDInverser()
{
}


const gkg::FilterFactor& gkg::SVDInverser::getFilterFactor() const
{

  return _filterFactor;

}


void gkg::SVDInverser::inverse( const gkg::CMatrix& A,
                                const gkg::CVector& b,
                                gkg::CVector& x ) const
{

  try
  {

    int32_t aSize1 = A.getSize1();
    int32_t aSize2 = A.getSize2();
    int32_t dSize = std::min( aSize1, aSize2 );
    int32_t bSize = b.getSize();
    int32_t xSize = x.getSize();

    // sanity check
    if ( bSize != aSize1 )
    {

      throw std::runtime_error( "inconsistent b and A size(s)" );

    }

    // reallocating
    if ( xSize != aSize2 )
    {

      x.reallocate( aSize2 );
      xSize = aSize2;

    }

    // processing SVD decomposition
    gkg::CMatrix U( aSize1, aSize1 );
    gkg::Vector d( dSize );
    gkg::CMatrix V( aSize2, aSize2 );

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    factory->getESingularValueDecomposition( A, U, d, V );

    x.fill( std::complex< double >( 0.0, 0.0 ) );

    std::complex< double > fourierCoefficient( 0.0, 0.0 ), tmp;
    std::complex< double > f( 0.0, 0.0 );
    gkg::CVector Ui( aSize1 );

    int32_t i, j, k;
    for ( j = 0; j < dSize; j++ )
    {

      if ( d( j ) > EPSILON )
      {

        for ( k = 0; k < bSize; k++ )
        {

          Ui( k ) = std::conj( U( k, j ) );

        }

        fourierCoefficient = Ui.getComposition( b );
        // Picard's condition for regularization
        f = std::complex< double >( _filterFactor.getFactor( d( j ) ), 0.0 );
        //if ( std::norm( fourierCoefficient ) < 4.0 * d( j ) )
        {

          tmp = f * fourierCoefficient  / d( j );
          for ( i = 0; i < xSize; i++ )
          {

            x( i ) += tmp * V( i, j );

          }

        }

      }  

    }

  }
  GKG_CATCH( "void gkg::SVDInverser::inverse( "
             "const gkg::CMatrix& A, "
             "const gkg::CVector& b, "
             "gkg::CVector& x ) const" );

}


#undef EPSILON
