#include <gkg-processing-numericalanalysis/Polynomial2dFitter.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Polynomial2dFitter::Polynomial2dFitter( const gkg::Vector& x,
                                             const gkg::Vector& y,
                                             const gkg::Vector& w,
                                             const gkg::Vector& f,
                                             int32_t maximumDegree )
{

  try
  {

    fit( x, y, w, f, maximumDegree );

  }
  GKG_CATCH( "gkg::Polynomial2dFitter::Polynomial2dFitter( "
             "const gkg::Vector& x, "
             "const gkg::Vector& y, "
             "const gkg::Vector& w, "
             "const gkg::Vector& f, "
             "int32_t maximumDegree )" );

}


gkg::Polynomial2dFitter::Polynomial2dFitter( const gkg::Vector& x,
                                             const gkg::Vector& y,
                                             const gkg::Vector& f,
                                             int32_t maximumDegree )
{

  try
  {

    gkg::Vector w( f.getSize() );
    w.fill( 1.0 );
    fit( x, y, w, f, maximumDegree );

  }
  GKG_CATCH( "gkg::Polynomial2dFitter::Polynomial2dFitter( "
             "const gkg::Vector& x, "
             "const gkg::Vector& y, "
             "const gkg::Vector& f, "
             "int32_t maximumDegree )" );

}


gkg::Polynomial2dFitter::~Polynomial2dFitter()
{
}


int32_t gkg::Polynomial2dFitter::getMaximumDegree() const
{

  return _maximumDegree;

}


const gkg::Vector& gkg::Polynomial2dFitter::getCoefficients() const
{

  return _coefficients;

}


double gkg::Polynomial2dFitter::getValueAt( double x, double y,
                                            int32_t maximumOrder ) const
{

  try
  {

    int32_t size = _coefficients.getSize();
    if ( ( maximumOrder > 0 ) && ( maximumOrder < size ) )
    {

      size = maximumOrder; 

    }
    double f = 0.0;
    int32_t i;
    for ( i = 0; i < size; i++ )
    {

      int32_t powx, powy;
      strIndxInv( i, powx, powy );
      f += _coefficients( i ) * std::pow( x, powx ) * std::pow( y, powy );

    }
    return f;

  }
  GKG_CATCH( "double gkg::Polynomial2dFitter::getValueAt( "
             "double x, double y ) const" );
}


void gkg::Polynomial2dFitter::fit( const gkg::Vector& x,
                                   const gkg::Vector& y,
                                   const gkg::Vector& w,
                                   const gkg::Vector& f,
                                   int32_t maximumDegree )
{

  try
  {

    // sanity check(s)
    int32_t size = x.getSize();
    if ( ( y.getSize() != size ) ||
         ( w.getSize() != size ) ||
         ( f.getSize() != size ) )
    {

      throw std::runtime_error( "incoherent x, y, and f size(s)" );

    }

    // getting concrete factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // the degrees of freedom as determined by the number of points.
    int32_t degree = size;

    if ( maximumDegree < 0 )
    {

      _maximumDegree = 0;
      while ( triang( _maximumDegree + 2 ) <= degree )
      {

        ++ _maximumDegree;

      }

    }
    else
    {

      _maximumDegree = maximumDegree;
      while ( triang( _maximumDegree + 1 ) > degree )
      {

        -- _maximumDegree;

      }

    }

    // the number of fitting degrees of freedom
    // we keep this an overdetermined system, n<=deg; the case of n=degree means
    // the fit is actually an interpolation (ie, exact).
    int32_t n = triang( _maximumDegree + 1 );

    // the n by n matrix of the linear system of equations
    gkg::Matrix  A( n, n );
    A.fill( 0.0 );

    // the RHS of the linear system of equations
    gkg::Vector rhs( n );
    rhs.fill( 0.0 );

    // fill the matrix and the right hand side
    int32_t row, col, k;
    for ( row = 0; row < n ;row++ )
    {

      int32_t rowi, rowj;
      strIndxInv( row, rowi, rowj );
      for ( col = 0; col < n; col++ )
      {

        int32_t coli, colj;
        strIndxInv(col,coli,colj) ;
        for ( k = 0; k < size; k++ )
        {

          A( row, col ) += std::pow( x( k ), rowi + coli ) *
                           std::pow( y( k ), rowj + colj );

        }

      }
      for ( k = 0; k < size; k++ )
      {

        rhs( row ) += w( k ) * f( k ) * std::pow( x( k ), rowi ) *
                                        std::pow( y( k ), rowj );

      }

    }

    // solve the linear system of equations
    _coefficients.reallocate( n );
    factory->getHouseHolderSolution( A, rhs, _coefficients );

  }
  GKG_CATCH( "void gkg::Polynomial2dFitter::fit( "
             "const gkg::Vector& x, "
             "const gkg::Vector& y, "
             "const gkg::Vector& w, "
             "const gkg::Vector& f, "
             "int32_t maximumDegree )" );

}


int32_t gkg::Polynomial2dFitter::triang( int32_t i ) const
{

  try
  {

    return i * ( i + 1 ) / 2;

  }
  GKG_CATCH( "int32_t gkg::Polynomial2dFitter::triang( int32_t i ) const" );

}


int32_t gkg::Polynomial2dFitter::strIndx( int32_t i, int32_t j ) const
{

  try
  {

    return triang( i + j ) + i;

  }
  GKG_CATCH( "int32_t gkg::Polynomial2dFitter::strIndx( "
             "int32_t i, int32_t j ) const" );

}


void 
gkg::Polynomial2dFitter::strIndxInv( int32_t n, int32_t& i, int32_t& j ) const
{

  try
  {

    // the triangular number that starts a sequence of  common sum i+j
    int32_t p = 0;
    while ( triang( p ) <= n )
    {

      ++ p;

    }
    -- p;
    i = n - triang( p );
    j = p - i;

  }
  GKG_CATCH( "void "
             "gkg::Polynomial2dFitter::strIndxInv( "
             "int32_t n, int32_t& i, int32_t& j ) const" );

}
