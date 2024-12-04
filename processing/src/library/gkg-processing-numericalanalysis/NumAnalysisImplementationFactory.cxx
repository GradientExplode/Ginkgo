#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <algorithm>
#include <limits>

#include <gkg-processing-numericalanalysis/PowellNewUOA_i.h>
#include <gkg-processing-numericalanalysis/UnitPseudoNormalVector_i.h>


gkg::NumericalAnalysisImplementationFactory::
     NumericalAnalysisImplementationFactory()
{
}


gkg::NumericalAnalysisImplementationFactory::
     ~NumericalAnalysisImplementationFactory()
{
}


double gkg::NumericalAnalysisImplementationFactory::getMinimumItem(
                                                      const gkg::Vector& vector,
                                                      int32_t* index ) const
{

  try
  {

    int32_t i, size = vector.getSize();
    int32_t bestIndex = 0;
    double bestValue = vector( 0 );
    
    for ( i = 1; i < size; i++ )
    {

      if ( vector( i ) < bestValue )
      {

        bestIndex = i;
        bestValue = vector( i );

      }

    }
    if ( index )
    {

      *index = bestIndex;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getMinimumItem( "
             "const gkg::Vector& vector, "
             "int* index ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getMaximumItem(
                                                      const gkg::Vector& vector,
                                                      int32_t* index ) const
{

  try
  {

    int32_t i, size = vector.getSize();
    int32_t bestIndex = 0;
    double bestValue = vector( 0 );
    
    for ( i = 1; i < size; i++ )
    {

      if ( vector( i ) > bestValue )
      {

        bestIndex = i;
        bestValue = vector( i );

      }

    }
    if ( index )
    {

      *index = bestIndex;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getMaximumItem( "
             "const gkg::Vector& vector, "
             "int* index ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getAbsMinimumItem(
                                                      const gkg::Vector& vector,
                                                      int32_t* index ) const
{

  try
  {

    int32_t i, size = vector.getSize();
    int32_t bestIndex = 0;
    double bestValue = std::fabs( vector( 0 ) );
    
    for ( i = 1; i < size; i++ )
    {

      if ( std::fabs( vector( i ) ) < bestValue )
      {

        bestIndex = i;
        bestValue = std::fabs( vector( i ) );

      }

    }
    if ( index )
    {

      *index = bestIndex;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getAbsMinimumItem( "
             "const gkg::Vector& vector, "
             "int* index ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getAbsMaximumItem(
                                                      const gkg::Vector& vector,
                                                      int32_t* index ) const
{

  try
  {

    int32_t i, size = vector.getSize();
    int32_t bestIndex = 0;
    double bestValue = std::fabs( vector( 0 ) );
    
    for ( i = 1; i < size; i++ )
    {

      if ( std::fabs( vector( i ) ) > bestValue )
      {

        bestIndex = i;
        bestValue = std::fabs( vector( i ) );

      }

    }
    if ( index )
    {

      *index = bestIndex;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getAbsMaximumItem( "
             "const gkg::Vector& vector, "
             "int* index ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getMinimumItem(
                                                      const gkg::Matrix& matrix,
                                                      int32_t* index1,
                                                      int32_t* index2 ) const
{

  try
  {

    int32_t i1, i2, size1 = matrix.getSize1(), size2 = matrix.getSize2();
    int32_t bestIndex1 = 0;
    int32_t bestIndex2 = 0;
    double bestValue = matrix( 0, 0 );
    
    for ( i1 = 1; i1 < size1; i1++ )
    {

      for ( i2 = 1; i2 < size2; i2++ )
      {

        if ( matrix( i1, i2 ) < bestValue )
        {

          bestIndex1 = i1;
          bestIndex2 = i2;
          bestValue = matrix( i1, i2 );

        }

      }

    }
    if ( index1 && index2 )
    {

      *index1 = bestIndex1;
      *index2 = bestIndex2;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getMinimumItem( "
             "const gkg::Matrix& matrix, "
             "int* index1, "
             "int* index2 ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getMaximumItem(
                                                      const gkg::Matrix& matrix,
                                                      int32_t* index1,
                                                      int32_t* index2 ) const
{

  try
  {

    int32_t i1, i2, size1 = matrix.getSize1(), size2 = matrix.getSize2();
    int32_t bestIndex1 = 0;
    int32_t bestIndex2 = 0;
    double bestValue = matrix( 0, 0 );
    
    for ( i1 = 1; i1 < size1; i1++ )
    {

      for ( i2 = 1; i2 < size2; i2++ )
      {

        if ( matrix( i1, i2 ) > bestValue )
        {

          bestIndex1 = i1;
          bestIndex2 = i2;
          bestValue = matrix( i1, i2 );

        }

      }

    }
    if ( index1 && index2 )
    {

      *index1 = bestIndex1;
      *index2 = bestIndex2;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getMaximumItem( "
             "const gkg::Matrix& matrix, "
             "int* index1, "
             "int* index2 ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getAbsMinimumItem(
                                                      const gkg::Matrix& matrix,
                                                      int32_t* index1,
                                                      int32_t* index2 ) const
{

  try
  {

    int32_t i1, i2, size1 = matrix.getSize1(), size2 = matrix.getSize2();
    int32_t bestIndex1 = 0;
    int32_t bestIndex2 = 0;
    double bestValue = std::fabs( matrix( 0, 0 ) );
    
    for ( i1 = 1; i1 < size1; i1++ )
    {

      for ( i2 = 1; i2 < size2; i2++ )
      {

        if ( std::fabs( matrix( i1, i2 ) ) < bestValue )
        {

          bestIndex1 = i1;
          bestIndex2 = i2;
          bestValue = std::fabs( matrix( i1, i2 ) );

        }

      }

    }
    if ( index1 && index2 )
    {

      *index1 = bestIndex1;
      *index2 = bestIndex2;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getAbsMinimumItem( "
             "const gkg::Matrix& matrix, "
             "int* index1, "
             "int* index2 ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getAbsMaximumItem(
                                                      const gkg::Matrix& matrix,
                                                      int32_t* index1,
                                                      int32_t* index2 ) const
{

  try
  {

    int32_t i1, i2, size1 = matrix.getSize1(), size2 = matrix.getSize2();
    int32_t bestIndex1 = 0;
    int32_t bestIndex2 = 0;
    double bestValue = std::fabs( matrix( 0, 0 ) );
    
    for ( i1 = 1; i1 < size1; i1++ )
    {

      for ( i2 = 1; i2 < size2; i2++ )
      {

        if ( std::fabs( matrix( i1, i2 ) ) > bestValue )
        {

          bestIndex1 = i1;
          bestIndex2 = i2;
          bestValue = std::fabs( matrix( i1, i2 ) );

        }

      }

    }
    if ( index1 && index2 )
    {

      *index1 = bestIndex1;
      *index2 = bestIndex2;

    }
    return bestValue;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::getAbsMaximumItem( "
             "const gkg::Matrix& matrix, "
             "int* index1, "
             "int* index2 ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getSingularValueNthOrderFit(
                                                          const gkg::Vector& x,
                                                          const gkg::Vector& y,
                                                          gkg::Vector& a ) const
{

  try
  {

    int32_t lineCount = y.getSize();
    int32_t columnCount = a.getSize();

    // sanity check
    if ( lineCount < columnCount )
    {

      throw std::runtime_error(
              "'y' vector should have at least as much samples as vector 'a'" );

    }

    gkg::Matrix X( lineCount, columnCount );

    int32_t line, column;
    for ( line = 0; line < lineCount; line++ )
    {

      for ( column = 0; column < columnCount; column++ )
      {


        if ( ( column == 0 ) && ( std::fabs( x( line ) ) < 1e-6 ) )
        {

          // check to be sure we do not call pow( 0, 0 )
          X( line, column ) = 1.0;
 
        }
        else
        {

          X( line, column ) = std::pow( x( line ), ( double )column );

        }


      }

    }

    gkg::Matrix V( columnCount, columnCount );
    gkg::Vector S( columnCount );

    getSingularValueDecomposition( X, V, S );
    getSingularValueSolution( X, S, V, y, a );

  }
  GKG_CATCH( "int gkg::NumericalAnalysisImplementationFactory::"
             "getSingularValueNthOrderFit( "
             "const gkg::Vector& x, "
             "const gkg::Vector& y, "
             "gkg::Vector& a ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getConditionNumber(
                                                    const gkg::Matrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::Matrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::Matrix V( size2, size2 );

    getSingularValueDecomposition2( A, U, S, V );

    double conditionNumber;
    
    if ( S( minimumSize - 1 ) < ( double )0.000000000000000001 )
    {

      conditionNumber = 0.0;

    }
    else
    {

      conditionNumber = S( 0 ) / S( minimumSize - 1 );

    }
    return conditionNumber;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getConditionNumber( "
             "const gkg::Matrix& A ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getFrobeniusNorm(
                                                    const gkg::Matrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::Matrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::Matrix V( size2, size2 );

    getSingularValueDecomposition2( A, U, S, V );

    double frobeniusNorm = 0.0;
    int32_t i = 0;
    for ( i = 0; i < minimumSize; i++ )
    {

      frobeniusNorm += S( i ) * S( i );

    }
    return frobeniusNorm;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getFrobeniusNorm( "
             "const gkg::Matrix& A ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getEuclideanNorm( 
                                                    const gkg::Matrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::Matrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::Matrix V( size2, size2 );

    getSingularValueDecomposition2( A, U, S, V );

    return S( 0 );

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getEuclideanNorm( "
             "const gkg::Matrix& A ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getMoorePenrosePseudoInverse(
                                                      const gkg::Matrix& A,
                                                      gkg::Matrix& Aplus ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t size = std::min( size1, size2 );

    gkg::Matrix U( size1, size1 );
    gkg::Vector S( size );
    gkg::Matrix V( size2, size2 );

    getSingularValueDecomposition2( A, U, S, V );

    gkg::Matrix Splus( size2, size1 );
    Splus.setZero();

    int32_t s;
    for ( s = 0; s < size; s++ )
    {

      Splus( s, s ) = ( S( s ) == 0.0 ? 0.0 : ( 1.0 / S( s ) ) );

    }

    Aplus = V.getComposition( Splus ).getComposition( U.getTransposition() );

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getMoorePenrosePseudoInverse( "
             "const gkg::Matrix& A, "
             "gkg::Matrix& Aplus ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getMoorePenrosePseudoInverse(
                                                      const gkg::CMatrix& A,
                                                      gkg::CMatrix& Aplus ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t size = std::min( size1, size2 );

    gkg::CMatrix U( size1, size1 );
    gkg::Vector S( size );
    gkg::CMatrix V( size2, size2 );

    getESingularValueDecomposition( A, U, S, V );

    gkg::Matrix Splus( size2, size1 );
    Splus.setZero();

    int32_t s;
    for ( s = 0; s < size; s++ )
    {

      Splus( s, s ) = ( S( s ) == 0.0 ? 0.0 : ( 1.0 / S( s ) ) );

    }

    Aplus = V.getComposition( Splus ).getComposition(
                                                U.getHermitianTransposition() );

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getMoorePenrosePseudoInverse( "
             "const gkg::CMatrix& A, "
             "gkg::CMatrix& Aplus ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getThikonovPseudoInverse(
                                                    const gkg::Matrix& A,
                                                    const gkg::Matrix& R,
                                                    double regularizationFactor,
                                                    gkg::Matrix& Aplus ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();

    // processing Aprime = ( AtA+lL)
    gkg::Matrix Aprime( size2, size2 );
    int32_t i1, i2, k;
    for ( i1 = 0; i1 < size2; i1++ )
    {

      for ( i2 = 0; i2 < size2; i2++ )
      {

        Aprime( i1, i2 ) = 0;
        for ( k = 0; k < size1; k++ )
        {

          Aprime( i1, i2 ) += A( k, i1 ) * A( k, i2 );


        }
        Aprime( i1, i2 ) += regularizationFactor * R( i1, i2 );

      }


    }

    // processing Ainv, inverse of Aprime
    gkg::Matrix U( size2, size2 );
    gkg::Vector S( size2 );
    gkg::Matrix V( size2, size2 );

    getSingularValueDecomposition2( Aprime, U, S, V );

    gkg::Matrix Splus( size2, size2 );
    Splus.setZero();
    int32_t s;
    for ( s = 0; s < size2; s++ )
    {

      Splus( s, s ) = ( S( s ) == 0.0 ? 0.0 : ( 1.0 / S( s ) ) );

    }

    gkg::Matrix Ainv;
    Ainv = V.getComposition( Splus ).getComposition( U.getTransposition() );

    // processing Aplus, regularized inverse of A
    Aplus = Ainv.getComposition( A.getTransposition() );

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getThikonovPseudoInverse( "
             "const gkg::Matrix& A, "
             "const gkg::Matrix& R, "
             "double regularizationFactor, "
             "gkg::Matrix& Aplus ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getPower( 
						     const gkg::Matrix& A,
						     double power,
						     gkg::Matrix& Apower ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );

    if ( size1 >= size2 )
    {

      Apower.reallocate( size1, size2 );

      gkg::Matrix U( size1, size1 );
      gkg::Vector S( minimumSize );
      gkg::Matrix V( size2, size2 );

      getSingularValueDecomposition2( A, U, S, V );

      int32_t s = 0;
      if ( power < 0 )
      {

        for ( s = 0; s < minimumSize; s++ )
        {

          S( s ) = ( ( S( s ) > 1e-50 ) ? ( 1.0 / S( s ) ) : 0.0 );

        }
        power = -power;

      }
      if ( !gkg::equal( power - 1.0, 0.0, 1e-300 ) ) 
      {

        for ( s = 0; s < minimumSize; s++ )
        {

          S( s ) = std::pow( S( s ), power );

        }

      }

      Apower = U.getComposition( S.getDiagonal() ).getComposition(
                                                         V.getTransposition() );

    }
    else
    {

      gkg::Matrix At = A.getTransposition();
      getPower( At, power, Apower );
      Apower.transpose();

    }

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getPower( "
             "const gkg::Matrix& A, "
             "double power, "
             "gkg::Matrix& Apower ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getConditionNumber(
                                                   const gkg::CMatrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::CMatrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::CMatrix V( size2, size2 );

    getESingularValueDecomposition( A, U, S, V );

    double conditionNumber;
    
    if ( S( minimumSize - 1 ) < ( double )0.000000000000000001 )
    {

      conditionNumber = 0.0;

    }
    else
    {

      conditionNumber = S( 0 ) / S( minimumSize - 1 );

    }
    return conditionNumber;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getConditionNumber( "
             "const gkg::CMatrix& A ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getFrobeniusNorm(
                                                   const gkg::CMatrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::CMatrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::CMatrix V( size2, size2 );

    getESingularValueDecomposition( A, U, S, V );

    double frobeniusNorm = 0.0;
    int32_t i = 0;
    for ( i = 0; i < minimumSize; i++ )
    {

      frobeniusNorm += S( i ) * S( i );

    }
    return frobeniusNorm;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getFrobeniusNorm( "
             "const gkg::CMatrix& A ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getEuclideanNorm(
                                                   const gkg::CMatrix& A ) const
{

  try
  {

    int32_t size1 = A.getSize1();
    int32_t size2 = A.getSize2();
    int32_t minimumSize = std::min( size1, size2 );
    
    // processing SVD decomposition
    gkg::CMatrix U( size1, size1 );
    gkg::Vector S( minimumSize );
    gkg::CMatrix V( size2, size2 );

    getESingularValueDecomposition( A, U, S, V );

    return S( 0 );

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getEuclideanNorm( "
             "const gkg::CMatrix& A ) const" );

}


std::complex< float >
gkg::NumericalAnalysisImplementationFactory::arccos(
                                          const std::complex< float >& z ) const
{

  try
  {

    std::complex< double >
      zout = this->arccos( std::complex< double >( z.real(), z.imag() ) );
    return std::complex< float >( ( float )zout.real(), ( float )zout.imag() );

  }
  GKG_CATCH( "std::complex< float > "
             "gkg::NumericalAnalysisImplementationFactory::"
             "arccos( const std::complex< float >& z ) const" );

}


std::complex< float >
gkg::NumericalAnalysisImplementationFactory::arcsin(
                                          const std::complex< float >& z ) const
{

  try
  {

    std::complex< double >
      zout = this->arccos( std::complex< double >( z.real(), z.imag() ) );
    return std::complex< float >( ( float )zout.real(), ( float )zout.imag() );

  }
  GKG_CATCH( "std::complex< float > "
             "gkg::NumericalAnalysisImplementationFactory::"
             "arcsin( const std::complex< float >& z ) const" );

}


std::complex< float >
gkg::NumericalAnalysisImplementationFactory::arctan(
                                          const std::complex< float >& z ) const
{

  try
  {

    std::complex< double >
      zout = this->arccos( std::complex< double >( z.real(), z.imag() ) );
    return std::complex< float >( ( float )zout.real(), ( float )zout.imag() );

  }
  GKG_CATCH( "std::complex< float > "
             "gkg::NumericalAnalysisImplementationFactory::"
             "arctan( const std::complex< float >& z ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getFactorialFunction(
                                                               double x ) const
{

  try
  {

    double result = 1.0;

    double i = 0;
    for ( i = 1.0; i <= x; i += 1.0 )
    {

        result *= i;

    }

    return result;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getFactorialFunction( double x ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::getDoubleFactorialFunction(
                                                               double x ) const
{

  try
  {

    double result = 1.0;

    int32_t i = 0;
    for ( i = ( int32_t )( x + 0.5 ); i >= 1; i -= 2 )
    {

        result *= ( double )i;

    }

    return result;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getDoubleFactorialFunction( double x ) const" );

}


double
gkg::NumericalAnalysisImplementationFactory::getHermitePolynomialFunction(
                                                         int32_t order,
                                                         const double& x ) const
{

  try
  {

    double result = 0.0;
    if ( order == 0 )
    {

      result = 1.0;

    }
    else if ( order == 1 )
    { 

      result = 2.0 * x;

    }
    else 
    {

      result = ( 2.0 * x * this->getHermitePolynomialFunction( order - 1, x )
                 - 2.0 * ( order - 1 ) * this->getHermitePolynomialFunction(
                                                               order - 2, x ) );

    }

    return result;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getHermitePolynomialFunction( "
             "int32_t order, "
             "const double& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getHermitePolynomialFunction(
                                                         int32_t order,
                                                         const float& x ) const
{

  try
  {

    float result = 0.0;
    if ( order == 0 )
    {

      result = 1.0f;

    }
    else if ( order == 1 )
    { 

      result = 2.0f * x;

    }
    else 
    {

      result = ( 2.0f * x * this->getHermitePolynomialFunction( order - 1, x )
                 - 2.0f * ( order - 1 ) * this->getHermitePolynomialFunction(
                                                               order - 2, x ) );

    }

    return result;

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getHermitePolynomialFunction( "
             "int32_t order, "
             "const float& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getRegularBesselFunctionJ0(
                                                          const float& x ) const
{

  try
  {

    return ( float )this->getRegularBesselFunctionJ0( ( double )x );

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionJ0( const float& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getRegularBesselFunctionJ1(
                                                          const float& x ) const
{

  try
  {

    return ( float )this->getRegularBesselFunctionJ1( ( double )x );

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionJ1( const float& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getRegularBesselFunctionI0(
                                                          const float& x ) const
{

  try
  {

    return ( float )this->getRegularBesselFunctionI0( ( double )x );

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionI0( const float& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getRegularBesselFunctionI1(
                                                          const float& x ) const
{

  try
  {

    return ( float )this->getRegularBesselFunctionI1( ( double )x );

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionI1( const float& x ) const" );

}


float
gkg::NumericalAnalysisImplementationFactory::getRegularBesselFunctionIn(
                                        const int32_t& n, const float& x ) const
{

  try
  {

    return ( float )this->getRegularBesselFunctionIn( n, ( double )x );

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionIn( "
             "const int32_t& n, const float& x ) const" );

}


float 
gkg::NumericalAnalysisImplementationFactory::getZeroBesselDerivativeFunctionJ0(
                                           const uint32_t& rootIndex ) const
{

  try
  {

    if ( rootIndex > 20 )
    {

      throw std::runtime_error(
                               "root index must remains lower or equal to 20" );

    }

    static float table[ 21 ] =
    {
 
      0.0,                   // unused
      3.831705970207512,
      7.015586669815613,
      10.17346813506272,
      13.32369193631422,
      16.47063005087763,
      19.61585851046824,
      22.76008438059277,
      25.90367208761838,
      29.04682853491686,
      32.18967991097440,
      35.33230755008387,
      38.47476623477162,
      41.61709421281445,
      44.75931899765282,
      47.90146088718545,
      51.04353518357151,
      54.18555364106132,
      57.32752543790101,
      60.46945784534749,
      63.61135669848123

    };

    return table[ rootIndex ];


  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getZeroBesselDerivativeFunctionJ0( "
             "const uint32_t& rootOrder ) const" );

}


float 
gkg::NumericalAnalysisImplementationFactory::getZeroBesselDerivativeFunctionJ1(
                                           const uint32_t& rootIndex ) const
{

  try
  {

    if ( rootIndex > 20 )
    {

      throw std::runtime_error(
                               "root index must remains lower or equal to 20" );

    }

    static float table[ 21 ] =
    {
 
      0.0,                   // unused
      1.841183781340659,
      5.331442773525033,
      8.536316366346285,
      11.70600490259206,
      14.86358863390903,
      18.01552786268180,
      21.16436985918879,
      24.31132685721078,
      27.45705057105925,
      30.60192297266909,
      33.74618289866738,
      36.88998740923681,
      40.03344405335068,
      43.17662896544882,
      46.31959756117391,
      49.46239113970276,
      52.60504111155668,
      55.74757179225101,
      58.89000229918570,
      62.03234787066199

    };

    return table[ rootIndex ];

  }
  GKG_CATCH( "float "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getZeroBesselDerivativeFunctionJ1( "
             "const uint32_t& rootOrder ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getZeroBesselDerivative(
                                              const uint32_t& n,
                                              const uint32_t& rootIndex ) const
{

  try
  {

    if ( n >= 50 )
    {

      throw std::runtime_error(
                               "order must remains lower than 50" );

    }

    if ( rootIndex >= 50 )
    {

      throw std::runtime_error(
                               "root index must remains lower than 50" );

    }

    static float table[][ 50 ] =
    {

      { 3.83170597,   7.01558667,  10.17346814,  13.32369194,
        16.47063005,  19.61585851,  22.76008438,  25.90367209,
        29.04682853,  32.18967991,  35.33230755,  38.47476623,
        41.61709421,  44.759319  ,  47.90146089,  51.04353518,
        54.18555364,  57.32752544,  60.46945785,  63.6113567 ,
        66.75322673,  69.89507184,  73.03689523,  76.17869958,
        79.32048718,  82.46225991,  85.60401944,  88.74576714,
        91.88750425,  95.02923181,  98.17095073, 101.31266182,
       104.45436579, 107.59606326, 110.73775478, 113.87944085,
       117.0211219 , 120.16279833, 123.30447049, 126.4461387 ,
       129.58780325, 132.72946439, 135.87112236, 139.01277739,
       142.15442966, 145.29607935, 148.43772662, 151.57937163,
       154.72101452, 157.8626554 },
      { 1.84118378,   5.33144277,   8.53631637,  11.7060049 ,
        14.86358863,  18.01552786,  21.16436986,  24.31132686,
        27.45705057,  30.60192297,  33.7461829 ,  36.88998741,
        40.03344405,  43.17662897,  46.31959756,  49.46239114,
        52.60504111,  55.74757179,  58.8900023 ,  62.03234787,
        65.1746208 ,  68.31683113,  71.45898711,  74.60109561,
        77.74316241,  80.88519235,  84.02718959,  87.16915764,
        90.31109957,  93.45301801,  96.59491525,  99.7367933 ,
       102.87865391, 106.02049864, 109.16232885, 112.30414577,
       115.44595048, 118.58774396, 121.72952706, 124.87130058,
       128.01306522, 131.15482162, 134.29657036, 137.43831196,
       140.58004691, 143.72177563, 146.86349853, 150.00521597,
       153.1469283 , 156.28863581 },
      { 3.05423693,   6.70613319,   9.96946782,  13.17037086,
        16.34752232,  19.51291278,  22.67158177,  25.82603714,
        28.97767277,  32.12732702,  35.27553505,  38.42265482,
        41.56893494,  44.71455353,  47.85964161,  51.00429767,
        54.14859724,  57.29259919,  60.43635008,  63.57988724,
        66.72324095,  69.86643601,  73.00949296,  76.15242892,
        79.2952583 ,  82.43799331,  85.58064435,  88.72322036,
        91.86572905,  95.0081771 ,  98.15057035, 101.2929139 ,
       104.43521224, 107.57746933, 110.71968869, 113.86187345,
       117.00402639, 120.14615001, 123.28824656, 126.43031806,
       129.57236632, 132.71439301, 135.85639961, 138.99838749,
       142.1403579 , 145.28231196, 148.42425071, 151.56617512,
       154.70808604, 157.8499843 },
      { 4.20118894,   8.0152366 ,  11.34592431,  14.58584829,
        17.78874787,  20.97247694,  24.14489743,  27.31005793,
        30.47026881,  33.62694918,  36.78102068,  39.93310862,
        43.08365266,  46.23297108,  49.38130009,  52.52881874,
        55.67566523,  58.821948  ,  61.9677533 ,  65.1131506 ,
        68.25819654,  71.40293768,  74.54741272,  77.69165407,
        80.83568905,  83.97954092,  87.12322953,  90.26677197,
        93.41018304,  96.55347558,  99.69666083, 102.83974863,
       105.98274768, 109.12566565, 112.26850936, 115.41128489,
       118.55399765, 121.69665253, 124.83925389, 127.98180569,
       131.12431149, 134.26677452, 137.40919772, 140.55158376,
       143.69393509, 146.83625393, 149.97854233, 153.12080216,
       156.26303515, 159.40524288 },
      { 5.31755313,   9.28239629,  12.68190844,  15.96410704,
        19.1960288 ,  22.40103227,  25.58975968,  28.76783622,
        31.93853934,  35.10391668,  38.26531699,  41.4236665 ,
        44.57962314,  47.73366752,  50.88615915,  54.03737242,
        57.18752046,  60.3367714 ,  63.48525967,  66.63309405,
        69.78036353,  72.92714162,  76.0734896 ,  79.21945893,
        82.36509317,  85.51042944,  88.65549957,  91.800331  ,
        94.94494751,  98.08936983, 101.23361611, 104.3777023 ,
       107.52164246, 110.66544908, 113.80913324, 116.95270484,
       120.09617273, 123.23954486, 126.38282839, 129.52602978,
       132.66915487, 135.81220898, 138.95519692, 142.09812309,
       145.24099151, 148.38380585, 151.52656948, 154.66928549,
       157.81195673, 160.95458583 },
      { 6.41561638,  10.51986087,  13.98718863,  17.31284249,
        20.57551452,  23.80358148,  27.0103079 ,  30.20284908,
        33.3854439 ,  36.56077769,  39.73064023,  42.89627316,
        46.05856627,  49.21817461,  52.37559153,  55.53119588,
        58.68528359,  61.83808923,  64.98980119,  68.14057257,
        71.29052908,  74.43977491,  77.58839718,  80.7364693 ,
        83.88405355,  87.03120316,  90.17796384,  93.32437513,
        96.47047134,  99.61628246, 102.76183477, 105.9071514 ,
       109.05225282, 112.19715718, 115.34188065, 118.48643767,
       121.63084118, 124.77510283, 127.91923309, 131.06324144,
       134.20713647, 137.35092598, 140.49461707, 143.6382162 ,
       146.7817293 , 149.92516179, 153.06851864, 156.21180443,
       159.35502337, 162.49817934 },
      { 7.50126614,  11.73493595,  15.26818146,  18.63744301,
        21.93171502,  25.1839256 ,  28.40977636,  31.61787572,
        34.81339298,  37.9996409 ,  41.17884947,  44.3525792 ,
        47.52195691,  50.68781778,  53.85079464,  57.01137608,
        60.16994561,  63.32680859,  66.48221126,  69.63635446,
        72.78940366,  75.94149646,  79.09274823,  82.24325646,
        85.39310406,  88.54236204,  91.69109156,  94.83934557,
        97.98717018, 101.13460569, 104.28168751, 107.42844688,
       110.57491145, 113.72110579, 116.86705181, 120.01276912,
       123.1582753 , 126.30358618, 129.44871605, 132.59367782,
       135.73848322, 138.88314289, 142.02766654, 145.17206303,
       148.31634048, 151.46050631, 154.60456734, 157.74852986,
       160.89239965, 164.03618206 },
      { 8.57783649,  12.93238624,  16.52936588,  19.94185337,
        23.26805293,  26.54503206,  29.79074858,  33.01517864,
        36.22438055,  39.42227458,  42.61152217,  45.79399966,
        48.97107095,  52.14375297,  55.31282033,  58.47887403,
        61.64238785,  64.80374053,  67.96323864,  71.12113304,
        74.27763106,  77.43290562,  80.58710208,  83.74034356,
        86.89273506,  90.04436665,  93.19531609,  96.34565085,
        99.49542974, 102.64470429, 105.79351984, 108.94191644,
       112.08992959, 115.23759089, 118.38492855, 121.53196784,
       124.67873146, 127.82523986, 130.97151151, 134.11756314,
       137.26340993, 140.40906571, 143.55454307, 146.69985354,
       149.84500765, 152.99001507, 156.1348847 , 159.27962473,
       162.42424269, 165.56874557 },
      { 9.64742165,  14.11551891,  17.77401237,  21.22906262,
        24.58719749,  27.88926943,  31.15532656,  34.39662855,
        37.62007804,  40.83017868,  44.03001034,  47.22175847,
        50.40702097,  53.58699544,  56.76259848,  59.93454431,
        63.1033982 ,  66.26961367,  69.43355902,  72.59553655,
        75.75579686,  78.91454945,  82.07197091,  85.22821114,
        88.38339823,  91.53764234,  94.69103869,  97.84367007,
       100.99560878, 104.14691826, 107.29765441, 110.44786667,
       113.59759895, 116.74689035, 119.89577586, 123.04428682,
       126.19245144, 129.34029514, 132.48784093, 135.63510964,
       138.78212022, 141.92888993, 145.07543449, 148.22176832,
       151.3679046 , 154.51385546, 157.65963202, 160.80524457,
       163.95070256, 167.09601476 },
      { 10.71143397,  15.28673767,  19.00459354,  22.50139873,
        25.89127728,  29.2185635 ,  32.50524735,  35.76379293,
        39.00190281,  42.22463843,  45.4354831 ,  48.63692265,
        51.83078393,  55.01844255,  58.20095582,  61.37915081,
        64.55368443,  67.72508544,  70.89378457,  74.06013637,
        77.22443549,  80.38692888,  83.54782516,  86.70730178,
        89.86551073,  93.02258289,  96.17863168,  99.3337558 ,
       102.48804162, 105.64156505, 108.79439302, 111.94658487,
       115.09819332, 118.24926541, 121.39984324, 124.54996459,
       127.69966351, 130.8489707 , 133.99791395, 137.14651851,
       140.29480729, 143.44280122, 146.5905194 , 149.7379793 ,
       152.88519695, 156.03218709, 159.17896326, 162.32553797,
       165.47192277, 168.61812831 },
      { 11.77087667,  16.44785275,  20.22303141,  23.76071586,
        27.18202153,  30.53450475,  33.84196578,  37.11800042,
        40.37106891,  43.6067649 ,  46.82895945,  50.04042897,
        53.24322321,  56.43889206,  59.62863131,  62.81337965,
        65.99388505,  69.17075142,  72.34447202,  75.51545393,
        78.68403628,  81.85050394,  85.01509806,  88.1780242 ,
        91.33945869,  94.49955372,  97.65844131, 100.8162366 ,
       103.97304045, 107.12894162, 110.28401853, 113.43834073,
       116.59197013, 119.74496202, 122.89736594, 126.04922639,
       129.2005835 , 132.35147351, 135.50192928, 138.6519806 ,
       141.80165465, 144.95097616, 148.09996779, 151.24865026,
       154.3970426 , 157.54516231, 160.69302549, 163.84064702,
       166.98804062, 170.13521901 },
      { 12.82649123,  17.60026656,  21.43085424,  25.0085187 ,
        28.46085728,  31.83842446,  35.16671443,  38.46038872,
        41.72862556,  44.97752625,  48.21133384,  51.43310517,
        54.64510624,  57.84905686,  61.04628851,  64.23784974,
        67.4245785 ,  70.6071532 ,  73.78612937,  76.9619666 ,
        80.13504857,  83.30569828,  86.47418969,  89.64075672,
        92.80560044,  95.96889462,  99.13079026, 102.29141924,
       105.45089726, 108.60932625, 111.7667964 , 114.92338778,
       118.07917175, 121.23421211, 124.38856609, 127.54228514,
       130.6954157 , 133.84799977, 137.00007541, 140.15167724,
       143.30283677, 146.45358278, 149.60394161, 152.75393739,
       155.90359227, 159.05292666, 162.20195932, 165.35070763,
       168.4991876 , 171.64741409 },
      { 13.87884307,  18.74509092,  22.6293003 ,  26.24604777,
        29.72897817,  33.13144995,  36.4805483 ,  39.79194072,
        43.0754868 ,  46.3377721 ,  49.58339642,  52.81568683,
        56.03711869,  59.24957708,  62.454526  ,  65.65312168,
        68.84629065,  72.03478491,  75.21922169,  78.40011274,
        81.57788618,  84.75290311,  87.92547036,  91.0958504 ,
        94.26426915,  97.43092221, 100.59597977, 103.75959067,
       106.92188567, 110.08298013, 113.2429762 , 116.40196472,
       119.5600267 , 122.71723465, 125.87365369, 129.02934241,
       132.18435374, 135.33873558, 138.49253142, 141.64578078,
       144.79851973, 147.95078119, 151.10259532, 154.25398977,
       157.40498996, 160.55561928, 163.70589931, 166.85584997,
       170.00548971, 173.1548356 },
      { 14.92837449,  19.88322436,  23.81938909,  27.47433975,
        30.98739433,  34.41454566,  37.78437851,  41.11351238,
        44.41245452,  47.68825285,  50.94584925,  54.18883107,
        57.41987615,  60.64103003,  63.85388583,  67.05970512,
        70.25950133,  73.4540989 ,  76.64417598,  79.83029598,
        83.01293114,  86.19248048,  89.36928364,  92.54363161,
        95.71577527,  98.88593209, 102.0542916 , 105.22101975,
       108.38626251, 111.55014879, 114.71279291, 117.87429661,
       121.03475072, 124.19423664, 127.3528275 , 130.51058919,
       133.66758129, 136.82385774, 139.97946755, 143.13445532,
       146.28886175, 149.44272406, 152.59607631, 155.74894981,
       158.90137331, 162.05337332, 165.20497428, 168.3561988 ,
       171.50706779, 174.65760064 },
      { 15.97543881,  21.01540493,  25.0019715 ,  28.69427122,
        32.23696941,  35.68854409,  39.07899819,  42.42585443,
        45.74023678,  49.02963506,  52.29931939,  55.55312778,
        58.79393376,  62.02393848,  65.24486077,  68.45806499,
        71.6646497 ,  74.86551046,  78.06138541,  81.25288898,
        84.44053706,  87.6247663 ,  90.80594897,  93.98440448,
        97.16040861, 100.33420074, 103.50598977, 106.67595882,
       109.84426916, 113.01106338, 116.17646804, 119.34059592,
       122.50354778, 125.66541401, 128.82627588, 131.98620672,
       135.14527282, 138.30353431, 141.46104582, 144.61785714,
       147.7740137 , 150.92955708, 154.0845254 , 157.23895364,
       160.39287403, 163.54631625, 166.69930769, 169.85187372,
       173.00403778, 176.15582166 },
      { 17.02032327,  22.14224735,  26.1777662 ,  29.90659108,
        33.47844849,  36.95416965,  40.36510275,  43.72962958,
        47.0594624 ,  50.362514  ,  53.64436964,  56.90910879,
        60.1597941 ,  63.3987776 ,  66.62790029,  69.84862681,
        73.06213892,  76.26940192,  79.47121293,  82.6682366 ,
        85.86103195,  89.05007287,  92.23576396,  95.41845289,
        98.59844022, 101.7759872 , 104.95132204, 108.12464507,
       111.29613289, 114.46594181, 117.63421073, 120.8010635 ,
       123.96661091, 127.13095242, 130.29417754, 133.45636709,
       136.61759421, 139.77792526, 142.93742061, 146.09613529,
       149.25411956, 152.41141945, 155.56807718, 158.72413153,
       161.87961824, 165.03457024, 168.18901794, 171.34298952,
       174.49651104, 177.64960672 },
      { 18.06326499,  23.26426978,  27.34738651,  31.11194494,
        34.71247959,  38.21205723,  41.64330585,  45.02542621,
        48.37069283,  51.68742381,  54.98150733,  58.25725555,
        61.51791395,  64.76598075,  68.0034157 ,  71.23178115,
        74.45234013,  77.66612622,  80.8739944 ,  84.07665874,
        87.27472076,  90.46869112,  93.65900642,  96.84604234,
       100.03012405, 103.21153453, 106.39052128, 109.56730178,
       112.74206794, 115.91498982, 119.08621866, 122.25588944,
       125.42412303, 128.59102801, 131.75670219, 134.92123395,
       138.08470332, 141.24718298, 144.40873909, 147.56943199,
       150.72931686, 153.88844422, 157.04686045, 160.20460821,
       163.36172678, 166.51825242, 169.67421863, 172.82965642,
       175.98459455, 179.13905971 },
      { 19.10446224,  24.3819137 ,  28.51136068,  32.31089394,
        35.93963034,  39.46276685,  42.91415216,  46.31376949,
        49.67443171,  53.0048459 ,  56.31119165,  59.59800556,
        62.86870984,  66.12594446,  69.37178453,  72.60788756,
        75.8355959 ,  79.05600992,  82.27004135,  85.47845278,
        88.68188756,  91.88089261,  95.07593614,  98.26742155,
       101.45569839, 104.64107123, 107.82380672, 111.00413944,
       114.18227656, 117.35840186, 120.53267893, 123.70525388,
       126.87625768, 130.04580806, 133.21401118, 136.380963  ,
       139.54675051, 142.71145276, 145.87514173, 149.03788313,
       152.19973708, 155.36075869, 158.52099853, 161.68050318,
       164.83931554, 167.99747522, 171.15501884, 174.31198034,
       177.46839118, 180.62428058 },
      { 20.1440827 ,  25.49555871,  29.67014737,  33.50392932,
        37.16040124,  40.70679543,  44.17812771,  47.59513048,
        50.97113292,  54.31521595,  57.6338398 ,  60.93175771,
        64.21256287,  67.47903264,  70.73335426,  73.97727786,
        77.21222318,  80.43935593,  83.65964336,  86.87389573,
        90.08279749,  93.28693128,  96.4867965 ,  99.68282391,
       102.87538721, 106.06481235, 109.25138502, 112.43535674,
       115.61694991, 118.79636197, 121.97376881, 125.14932768,
       128.32317965, 131.49545161, 134.66625806, 137.8357026 ,
       141.00387919, 144.17087326, 147.33676268, 150.50161857,
       153.66550603, 156.82848475, 159.99060959, 163.15193106,
       166.3124957 , 169.47234655, 172.63152339, 175.7900631 ,
       178.94799989, 182.10536555 },
      { 21.18226963,  26.60553392,  30.8241478 ,  34.69148395,
        38.37523617,  41.9445862 ,  45.4356683 ,  48.86993352,
        52.26120709,  55.61892969,  58.94983212,  62.25887691,
        65.54982274,  68.82558023,  72.08844556,  75.34025903,
        78.58251593,  81.81644575,  85.04307016,  88.26324612,
        91.47769848,  94.68704502,  97.89181585, 101.09246876,
       104.28940132, 107.48296063, 110.67345123, 113.86114149,
       117.04626891, 120.22904452, 123.40965647, 126.58827313,
       129.76504562, 132.94010999, 136.1135891 , 139.28559418,
       142.45622617, 145.62557693, 148.79373022, 151.96076258,
       155.12674413, 158.2917392 , 161.45580693, 164.61900179,
       167.78137405, 170.94297014, 174.10383307, 177.26400267,
       180.42351594, 183.58240728 },
      { 22.21914648,  27.71212684,  31.97371522,  35.8739415 ,
        39.58453089,  43.17653646,  46.68716642,  50.13856248,
        53.54502718,  56.91634787,  60.25951651,  63.57969798,
        66.88081125,  70.16589626,  73.43735505,  76.69711577,
        79.94674733,  83.1875416 ,  86.42057347,  89.64674568,
        92.86682276,  96.081457  ,  99.29120879, 102.49656256,
       105.69793942, 108.89570741, 112.09018972, 115.2816714 ,
       118.47040496, 121.65661491, 124.84050162, 128.02224448,
       131.20200466, 134.37992735, 137.55614375, 140.7307727 ,
       143.90392215, 147.07569037, 150.24616703, 153.41543413,
       156.5835668 , 159.75063402, 162.91669926, 166.08182098,
       169.24605316, 172.4094457 , 175.5720448 , 178.73389334,
       181.8950311 , 185.0554951 },
      { 23.25482051,  28.81559026,  33.11916251,  37.05164392,
        40.78864004,  44.40300395,  47.93297704,  51.40136592,
        54.82293308,  58.20780036,  61.5632121 ,  64.89452898,
        68.20582532,  71.50026659,  74.7803578 ,  78.04811263,
        81.30517182,  84.55288816,  87.79238862,  91.02462083,
        94.25038819,  97.47037697, 100.68517731, 103.89529989,
       107.10118907, 110.30323355, 113.50177494, 116.69711486,
       119.88952065, 123.07923023, 126.26645606, 129.45138853,
       132.63419879, 135.81504114, 138.99405508, 142.17136707,
       145.34709205, 148.5213347 , 151.6941906 , 154.8657472 ,
       158.03608467, 161.20527667, 164.37339098, 167.54049008,
       170.70663167, 173.87186914, 177.03625193, 180.19982593,
       183.36263377, 186.52471511 },
      { 24.28938554,  29.91614781,  34.2607683 ,  38.22489744,
        41.98788278,  45.62431208,  49.1734224 ,  52.65866144,
        56.09523556,  59.49358979,  62.86121252,  66.2036541 ,
        69.52513956,  72.82895627,  76.11770942,  79.39349598,
        82.65802681,  85.91271419,  89.15873605,  92.39708397,
        95.62859953,  98.85400232, 102.07391178, 105.28886442,
       108.49932752, 111.70571016, 114.9083722 , 118.10763159,
       121.30377038, 124.49703977, 127.68766423, 130.87584508,
       134.06176338, 137.24558249, 140.42745019, 143.60750052,
       146.78585539, 149.96262588, 153.13791348, 156.3118111 ,
       159.48440394, 162.65577033, 165.82598238, 168.9951066 ,
       172.16320445, 175.33033279, 178.49654432, 181.66188798,
       184.82640922, 187.99015039 },
      { 25.32292421,  31.01399843,  35.39878197,  39.39397753,
        43.18254757,  46.84075435,  50.40879603,  53.91073936,
        57.36221966,  60.77399447,  64.15378858,  67.50733617,
        70.83900854,  74.15221157,  77.44964789,  80.7334957 ,
        84.00553424,  87.26723391,  90.51982252,  93.76433469,
        97.00164943, 100.23251908, 103.45759186, 106.6774297 ,
       109.89252247, 113.10329937, 116.31013826, 119.51337323,
       122.7133009 , 125.9101856 , 129.10426374, 132.29574744,
       135.48482763, 138.67167667, 141.85645057, 145.03929093,
       148.22032658, 151.39967502, 154.5774436 , 157.75373069,
       160.92862653, 164.10221411, 167.27456989, 170.4457644 ,
       173.61586284, 176.78492558, 179.9530086 , 183.12016386,
       186.2864397 , 189.45188112 },
      { 26.35550968,  32.10931996,  36.5334277 ,  40.55913306,
        44.37289611,  48.05259808,  51.63936621,  55.15786592,
        58.62414749,  62.04927114,  65.44119072,  68.80581881,
        72.14766879,  75.47026176,  78.77639527,  82.06832667,
        85.34790193,  88.61664829,  91.87584232,  95.12656081,
        98.36971948, 101.60610281, 104.83638732, 108.06115997,
       111.28093283, 114.49615496, 117.70722198, 120.91448394,
       124.11825184, 127.31880304, 130.51638575, 133.7112228 ,
       136.90351492, 140.0934434 , 143.28117243, 146.46685114,
       149.65061526, 152.83258863, 156.01288451, 159.19160666,
       162.36885039, 165.54470332, 168.71924626, 171.89255376,
       175.06469479, 178.23573321, 181.40572827, 184.574735  ,
       187.74280459, 190.90998472 },
      { 27.38720713,  33.20227214,  37.6649079 ,  41.7205897 ,
        45.55916665,  49.2600875 ,  52.86537891,  56.4002859 ,
        59.88126077,  63.31965714,  66.72365106,  70.09932836,
        73.45134051,  76.78332077,  80.09815911,  83.3981901 ,
        86.68532484,  89.96114613,  93.22697826,  96.48393932,
        99.73298103, 102.97491943, 106.21045882, 109.44021077,
       112.66470928, 115.88442292, 119.09976484, 122.31110086,
       125.5187562 , 128.72302109, 131.9241554 , 135.12239262,
       138.31794317, 141.5109972 , 144.70172705, 147.89028929,
       151.07682652, 154.26146893, 157.44433561, 160.62553576,
       163.80516966, 166.98332963, 170.16010077, 173.33556168,
       176.50978506, 179.6828383 , 182.8547839 , 186.02567996,
       189.19558054, 192.36453602 },
      { 28.41807483,  34.29299903,  38.79340601,  42.87855284,
        46.74157681,  50.46344645,  54.08706022,  57.638225  ,
        61.13378297,  64.58537241,  68.00138519,  71.38807549,
        74.75022911,  78.0915885 ,  81.41513374,  84.72327469,
        88.01798612,  91.30090509,  94.57340261,  97.83663724,
       101.091596  , 104.33912588, 107.57995851, 110.81472967,
       114.04399489, 117.26824202, 120.48790147, 123.70335463,
       126.91494076, 130.12296281, 133.32769222, 136.52937301,
       139.72822519, 142.92444772, 146.118221  , 149.30970902,
       152.49906121, 155.68641404, 158.87189243, 162.05561097,
       165.23767493, 168.41818127, 171.59721941, 174.77487196,
       177.95121538, 181.12632055, 184.30025324, 187.47307462,
       190.64484162, 193.8156073 },
      { 29.44816521,  35.38163108,  39.91908887,  44.03320998,
        47.920326  ,  51.6628807 ,  55.30461857,  58.87189183,
        62.38192112,  65.84662124,  69.2745938 ,  72.67225671,
        76.04452654,  79.39525213,  82.7275014 ,  86.04375773,
        89.34605807,  92.63609256,  95.91527793,  99.18481239,
       102.44571756, 105.69887085, 108.94503071, 112.18485673,
       115.41892564, 118.64774427, 121.87176007, 125.09136975,
       128.30692647, 131.51874575, 134.72711045, 137.93227501,
       141.13446897, 144.33389999, 147.53075649, 150.7252098 ,
       153.91741614, 157.10751823, 160.29564679, 163.48192174,
       166.66645337, 169.84934322, 173.03068503, 176.21056542,
       179.38906458, 182.56625688, 185.74221137, 188.91699228,
       192.09065941, 195.26326851 },
      { 30.47752554,  36.46828685,  41.04210875,  45.18473289,
        49.09559743,  52.85857983,  56.51824657,  60.10147963,
        63.6258675 ,  67.10359375,  70.54346404,  73.95205564,
        77.33441251,  80.69448717,  84.0354333 ,  87.35980602,
        90.66970305,  93.96686649,  97.25275779, 100.52861404,
       103.79549081, 107.05429533, 110.30581242, 113.55072507,
       116.7896309 , 120.0230554 , 123.25146283, 126.47526503,
       129.69482885, 132.91048223, 136.12251935, 139.33120494,
       142.53677796, 145.73945472, 148.93943156, 152.13688711,
       155.33198432, 158.52487213, 161.71568699, 164.90455419,
       168.09158892, 171.27689736, 174.46057752, 177.64272   ,
       180.82340874, 184.00272158, 187.18073084, 190.3575038 ,
       193.53310312, 196.70758727 },
      { 31.50619868,  37.55307443,  42.16260504,  46.33327932,
        50.26755989,  54.05071903,  57.72812261,  61.32716786,
        64.86580101,  68.35646723,  71.80817077,  75.22764416,
        78.62005552,  81.98945851,  85.33909047,  88.67157668,
        91.98907425,  95.29337609,  98.58598749, 101.86818361,
       105.14105333, 108.40553316, 111.66243383, 114.91246135,
       118.15623386, 121.39429526, 124.62712635, 127.85515391,
       131.0787583 , 134.29827972, 137.51402352, 140.72626464,
       143.93525135, 147.14120852, 150.34434034, 153.54483268,
       156.74285516, 159.9385629 , 163.13209806, 166.32359121,
       169.51316249, 172.70092264, 175.88697392, 179.07141093,
       182.25432132, 185.4357864 , 188.61588175, 191.79467768,
       194.97223974, 198.14862907 },
      { 32.53422356,  38.6360927 ,  43.2807057 ,  47.47899456,
        51.43636931,  55.23946052,  58.9344123 ,  62.54912345,
        66.10188848,  69.6054073 ,  73.06887767,  76.49918344,
        79.90161382,  83.28032121,  86.63862466,  89.97921796,
        93.30431637,  96.61576251,  99.91510461, 103.20365517,
       106.48253572, 109.75271154, 113.01501881, 116.27018616,
       119.51885194, 122.76157818, 125.99886196, 129.23114481,
       132.4588204 , 135.68224106, 138.90172317, 142.11755174,
       145.32998429, 148.53925413, 151.74557324, 154.94913467,
       158.15011466, 161.34867443, 164.54496185, 167.73911272,
       170.93125207, 174.1214952 , 177.30994861, 180.49671087,
       183.68187331, 186.86552071, 190.04773189, 193.22858022,
       196.40813407, 199.58645728 },
      { 33.56163572,  39.71743238,  44.39652854,  48.62201276,
        52.60217002,  56.42495487,  60.13726972,  63.76750203,
        67.33428573,  70.85056897,  74.32573818,  77.76682482,
        81.17923628,  84.56722136,  87.93417897,  91.28286992,
        94.61556629,  97.93415943, 101.24023964, 104.535156  ,
       107.8200621 , 111.09595148, 114.3636853 , 117.62401445,
       120.87759717, 124.12501332, 127.36677606, 130.60334141,
       133.83511622, 137.06246477, 140.28571435, 143.50515992,
       146.72106811, 149.93368065, 153.14321719, 156.34987791,
       159.55384558, 162.75528754, 165.95435723, 169.15119572,
       172.34593288, 175.53868853, 178.7295734 , 181.91868998,
       185.1061333 , 188.29199157, 191.47634686, 194.65927554,
       197.84084886, 201.02113333 },
      { 34.58846766,  40.7971769 ,  45.51018228,  49.7624581 ,
        53.76509596,  57.60734209,  61.33683844,  64.98244897,
        68.56313857,  72.09209754,  75.57889638,  79.03071064,
        82.45306311,  85.85029678,  89.22588862,  92.58266499,
        95.92295364,  99.2486936 , 102.5615164 , 105.86280703,
       109.15375054, 112.43536818, 115.7085457 , 118.97405588,
       122.23257651, 125.48470503, 128.7309704 , 131.97184299,
       135.20774259, 138.43904532, 141.66608921, 144.88917907,
       148.10859058, 151.32457372, 154.5373558 , 157.74714401,
       160.95412764, 164.15848005, 167.36036026, 170.55991452,
       173.75727753, 176.9525736 , 180.14591766, 183.3374161 ,
       186.52716762, 189.71526387, 192.90179012, 196.08682577,
       199.27044491, 202.4527167 },
      { 35.61474922,  41.87540324,  46.6217675 ,  50.90044573,
        54.92527166,  58.78675269,  62.53325256,  66.19410026,
        69.78858366,  73.33012943,  76.82848778,  80.29097497,
        83.72322656,  87.12967762,  90.51388147,  93.87872862,
        97.2266013 , 100.55948531, 103.87905257, 107.1867233 ,
       110.48371343, 113.77107144, 117.04970726, 120.32041515,
       123.58389218, 126.84075306, 130.09154239, 133.3367446 ,
       136.57679233, 139.8120733 , 143.04293621, 146.26969559,
       149.49263603, 152.71201572, 155.92806951, 159.14101157,
       162.35103763, 165.55832701, 168.76304428, 171.96534081,
       175.16535612, 178.36321896, 181.55904842, 184.75295481,
       187.94504044, 191.13540039, 194.32412313, 197.51129107,
       200.69698111, 203.88126509 },
      { 36.64050785,  42.95218254,  47.73137747,  52.03608272,
        56.08281316,  59.9633085 ,  63.72663752,  67.40258336,
        71.01074932,  74.56479294,  78.07463998,  81.54774425,
        84.98985154,  88.40548697,  91.79827859,  95.1711797 ,
        98.52662592, 101.86664885, 105.19296007, 108.50701434,
       111.8100579 , 115.10316602, 118.38727234, 121.6631923 ,
       124.93164192, 128.19325294, 131.44858532, 134.6981374 ,
       137.94235445, 141.18163569, 144.41634032, 147.64679248,
       150.87328558, 154.09608591, 157.31543581, 160.53155633,
       163.74464959, 166.95490081, 170.16248007, 173.36754384,
       176.57023636, 179.77069085, 182.96903052, 186.16536954,
       189.35981386, 192.55246194, 195.74340542, 198.93272973,
       202.12051457, 205.30683442 },
      { 37.66576885,  44.02758073,  48.83909883,  53.16946879,
        57.23782876,  61.13712347,  64.91711085,  68.6080179 ,
        72.22975619,  75.79620886,  79.31747334,  82.80113789,
        86.25305616,  89.67784138,  93.07919474,  96.46013108,
        99.82313832, 103.17029296, 106.50334547, 109.82378452,
       113.13288615, 116.43175191, 119.72133877, 123.00248302,
       126.27591929, 129.54229613, 132.80218861, 136.0561088 ,
       139.30451442, 142.54781604, 145.78638322, 149.0205496 ,
       152.2506173 , 155.47686064, 158.69952934, 161.91885128,
       165.13503492, 168.34827131, 171.55873598, 174.76659046,
       177.97198372, 181.17505335, 184.37592667, 187.57472171,
       190.771548  , 193.96650739, 197.15969468, 200.35119827,
       203.54110065, 206.72947896 },
      { 38.69055565,  45.10165905,  49.94501228,  54.30069698,
        58.39041974,  62.30830435,  66.10478288,  69.81051636,
        73.44571783,  77.02449111,  80.55710149,  84.05126881,
        87.51295225,  90.94685132,  94.35673887,  97.74568998,
       101.11624387, 104.4705211 , 107.81031029, 111.1371334 ,
       114.45229573, 117.75692466, 121.05200012, 124.33837887,
       127.61681392, 130.88797031, 134.15243805, 137.41074273,
       140.66335432, 143.91069464, 147.15314345, 150.39104378,
       153.62470636, 156.85441345, 160.08042206, 163.30296686,
       166.52226253, 169.73850595, 172.95187803, 176.16254532,
       179.37066146, 182.5763684 , 185.77979756, 188.98107076,
       192.18030111, 195.37759381, 198.57304681, 201.76675148,
       204.95879309, 208.14925139 },
      { 39.71488993,  46.17447445,  51.04919307,  55.42985428,
        59.54068094,  63.47695133,  67.2897573 ,  71.01018463,
        74.65874137,  78.24974724,  81.79363188,  85.29824389,
        88.7696458 ,  92.21262163,  95.63101442,  99.02795836,
       102.40604293, 105.76743192, 109.11395139, 112.44715603,
       115.76837986, 119.07877564, 122.37934591, 125.67096757,
       128.95441169, 132.23035959, 135.49941598, 138.76211977,
       142.01895307, 145.27034871, 148.5166966 , 151.75834901,
       154.99562519, 158.22881523, 161.45818339, 164.683971  ,
       167.90639895, 171.12566988, 174.34197002, 177.5554709 ,
       180.7663308 , 183.974696  , 187.18070195, 190.38447427,
       193.58612961, 196.78577651, 199.98351605, 203.17944252,
       206.37364399, 209.56620281 },
      { 40.73879184,  47.24608006,  52.15171154,  56.55702215,
        60.68870138,  64.64315855,  68.4721317 ,  72.20712253,
        75.86892789,  79.47207895,  83.02716621,  86.54216443,
        90.02323738,  93.47525189,  96.90211981, 100.30703326,
       103.69263108, 107.06111946, 110.41436124, 113.75394322,
       117.08122766, 120.39739231, 123.70346191, 127.00033317,
       130.28879499, 133.56954469, 136.84320148, 140.1103174 ,
       143.37138652, 146.62685258, 149.87711545, 153.12253658,
       156.3634436 , 159.60013436, 162.83288028, 166.06192931,
       169.28750846, 172.50982606, 175.72907364, 178.94542766,
       182.15905098, 185.37009419, 188.57869675, 191.78498802,
       194.9890882 , 198.19110913, 201.39115499, 204.589323  ,
       207.78570399, 210.9803829 },
      { 41.76228012,  48.31652548,  53.25263353,  57.68227699,
        61.83456466,  65.80701462,  69.6519981 ,  73.4014243 ,
        77.07637299,  80.69158247,  84.25780087,  87.78312656,
        91.27382253,  94.73483679,  98.17014868, 101.58300717,
       104.9760995 , 108.35167351, 111.71162815, 115.05758178,
       118.39092443, 121.71285841, 125.02443029, 128.32655631,
       131.62004289, 134.90560315, 138.18387056, 141.45541012,
       144.7207277 , 147.98027779, 151.23447013, 154.48367518,
       157.72822892, 160.96843683, 164.20457741, 167.43690516,
       170.66565316, 173.89103537, 177.11324857, 180.33247409,
       183.54887936, 186.76261921, 189.97383709, 193.1826661 ,
       196.38922993, 199.59364369, 202.79601467, 205.996443  ,
       209.19502222, 212.39183987 },
      { 42.78537226,  49.38585712,  54.35202077,  58.80569058,
        62.97834948,  66.96860305,  70.82944334,  74.593179  ,
        78.28116712,  81.908349  ,  85.48562731,  89.02122157,
        92.52149207,  95.99146646,  99.43519028, 102.85596828,
       106.25653523, 109.63917984, 113.00583659, 116.35815482,
       119.69755187, 123.0252542 , 126.34232988, 129.64971438,
       132.94823134, 136.23860947, 139.5214963 , 142.79746961,
       146.0670469 , 149.33069329, 152.58882822, 155.84183111,
       159.09004613, 162.33378634, 165.57333724, 168.80895979,
       172.04089311, 175.2693567 , 178.49455255, 181.71666683,
       184.93587148, 188.15232556, 191.36617645, 194.57756096,
       197.78660625, 200.9934307 , 204.19814466, 207.40085115,
       210.60164643, 213.80062061 },
      { 43.80808458,  50.45411848,  55.44993122,  59.92733042,
        64.12012999,  68.12800265,  72.00454949,  75.78247092,
        79.48339598,  83.12246507,  86.71073237,  90.25653626,
        93.76633247,  97.24522677, 100.69732971, 104.12600076,
       107.53402139, 110.92372048, 114.29706737, 117.65574191,
       121.00118826, 124.33465667, 127.65723634, 130.96988168,
       134.27343333, 137.56863532, 140.85614905, 144.1365649 ,
       147.41041185, 150.6781655 , 153.94025491, 157.19706829,
       160.44895794, 163.69624441, 166.93922011, 170.17815242,
       173.41328638, 176.64484703, 179.87304149, 183.09806073,
       186.32008117, 189.53926603, 192.75576663, 195.96972345,
       199.18126709, 202.39051917, 205.59759308, 208.8025947 ,
       212.00562302, 215.20677068 },
      { 44.83043235,  51.52135039,  56.54641941,  61.04726012,
        65.25997613,  69.28528788,  73.17739421,  76.96937991,
        80.68314089,  84.33401283,  87.93319861,  91.48915323,
        95.00842607,  98.4961996 , 101.95664816, 105.39318502,
       108.80863747, 112.2053739 , 115.58539789, 118.9504193 ,
       122.30190869, 125.64113968, 128.96922233, 132.28712965,
       135.59571904, 138.89574963, 142.18789651, 145.47276247,
       148.75088782, 152.02275852, 155.28881309, 158.54944846,
       161.80502496, 165.05587052, 168.3022844 , 171.54454031,
       174.78288918, 178.01756152, 181.24876953, 184.47670894,
       187.70156058, 190.92349185, 194.14265794, 197.35920296,
       200.57326093, 203.7849567 , 206.99440667, 210.20171958,
       213.40699708, 216.61033438 },
      { 45.85242988,  52.58759122,  57.64153668,  62.1655397 ,
        66.39795402,  70.4405292 ,  74.34805104,  78.15398172,
        81.88047903,  85.54307039,  89.15310461,  92.71915116,
        96.2478514 ,  99.7444631 , 103.21322324, 106.65759794,
       110.08045954, 113.48421524, 116.87090232, 120.24226013,
       123.59978521, 126.9447742 , 130.27835766, 133.60152696,
       136.915156  , 140.22001879, 143.51680391, 146.80612642,
       150.08853778, 153.36453417, 156.63456348, 159.89903125,
       163.15830572, 166.41272214, 169.66258653, 172.90817888,
       176.14975593, 179.3875536 , 182.62178914, 185.85266297,
       189.08036032, 192.3050527 , 195.52689915, 198.74604739,
       201.96263484, 205.17678953, 208.38863086, 211.59827041,
       214.80581249, 218.01135482 },
      { 46.87409061,  53.6528771 ,  58.73533146,  63.28222582,
        67.53412615,  71.59379334,  75.51658976,  79.33634823,
        83.07548378,  86.74971206,  90.3705252 ,  93.94660509,
        97.4846834 , 100.99009189, 104.4671291 , 107.91931306,
       111.34956042, 114.76031651, 118.15365178, 121.53133458,
       124.89488702, 128.2456284 , 131.58470948, 134.9131397 ,
       138.23180923, 141.54150674, 144.84293413, 148.13671854,
       151.42342245, 154.70355214, 157.97756474, 161.24587428,
       164.50885683, 167.76685488, 171.02018114, 174.26912178,
       177.51393932, 180.75487503, 183.99215115, 187.22597276,
       190.45652945, 193.68399678, 196.90853763, 200.13030329,
       203.34943455, 206.56606259, 209.78030982, 212.99229059,
       216.20211189, 219.40987392 },
      { 47.89542716,  54.71724208,  59.82784946,  64.39737208,
        68.66855175,  72.74514357,  76.68307658,  80.51654782,
        84.26822496,  87.95400864,  91.58553174,  95.1715866 ,
        98.71899365, 102.23315733, 105.7184367 , 109.17840082,
       112.61600989, 116.03374677, 119.43371452, 122.81771003,
       126.1872806 , 129.54376783, 132.88834237, 136.22203148,
       139.54574134, 142.86027511, 146.16634779, 149.46459846,
       152.75560049, 156.03987009, 159.31787353, 162.59003325,
       165.85673304, 169.11832254, 172.37512108, 175.62742097,
       178.87549041, 182.11957599, 185.35990488, 188.59668678,
       191.83011558, 195.0603709 , 198.28761937, 201.51201586,
       204.73370449, 207.95281958, 211.16948648, 214.38382234,
       217.59593679, 220.8059325 },
      { 48.91645142,  55.78071831,  60.91913392,  65.51102923,
        69.80128692,  73.89463994,  77.84757445,  81.69464548,
        85.45876907,  89.15602759,  92.79819229,  96.39416407,
        99.95085056, 103.47372769, 106.96721397, 110.43492869,
       113.87987487, 117.30457228, 120.71115607, 124.10145126,
       127.47702989, 130.83925557, 134.18931852, 137.52826358,
       140.8570127 , 144.17638333, 147.4871034 , 150.78982376,
       154.08512851, 157.37354371, 160.65554464, 163.93156202,
       167.20198731, 170.46717721, 173.72745757, 176.98312679,
       180.2344587 , 183.48170513, 186.72509816, 189.96485204,
       193.20116495, 196.43422049, 199.66418906, 202.89122904,
       206.11548785, 209.33710295, 212.5562026 , 215.77290674,
       218.98732759, 222.1995703 },
      { 49.93717458,  56.84333616,  62.00922575,  66.62324535,
        70.93238495,  75.04233952,  79.01014324,  82.87070314,
        86.64717947,  90.35583329,  94.00857188,  97.61440284,
       101.18031959, 104.7118683 , 108.21352602, 111.68896138,
       115.14121956, 118.57285667, 121.98603942, 125.38262051,
       128.7641964 , 132.13215233, 135.48769783, 138.83189505,
       142.16568151, 145.48988874, 148.80525742, 152.11245002,
       155.41206125, 158.70462687, 161.99063106, 165.27051275,
       168.54467093, 171.81346931, 175.07724022, 178.33628802,
       181.59089215, 184.84130964, 188.08777738, 191.33051418,
       194.56972242, 197.80558969, 201.03829009, 204.2679855 ,
       207.49482664, 210.71895401, 213.94049883, 217.15958376,
       220.37632361, 223.590826 },
      { 50.95760723,  57.90512436,  63.09816371,  67.7340661 ,
        72.06189644,  76.18829659,  80.17083993,  84.0447798 ,
        87.83351663,  91.55348723,  95.21673264,  98.83236544,
       102.40746339, 105.94764179, 109.45743525, 112.94056098,
       116.40010562, 119.83866105, 123.2584251 , 126.66127769,
       130.04883933, 133.42251659, 136.78353803, 140.13298284,
       143.47180393, 146.80084668, 150.12086438, 153.43253096,
       156.73645161, 160.03317164, 163.32318406, 166.60693587,
       169.88483357, 173.15724773, 176.4245171 , 179.68695199,
       182.94483733, 186.1984353 , 189.4479876 , 192.69371751,
       195.9358316 , 199.1745214 , 202.40996468, 205.64232679,
       208.87176169, 212.09841296, 215.32241471, 218.54389231,
       221.76296316, 224.9797373 },
      { 51.97775935,  58.96611015,  64.1859846 ,  68.84353482,
        73.18986951,  77.33256282,  81.32971886,  85.21693177,
        89.01783829,  92.74904815,  96.42273401, 100.0481117 ,
       103.63234198, 107.18110818, 110.69900154, 114.18978708,
       117.65659228, 121.10204422, 124.52837136, 127.93748046,
       131.33101571, 134.7104047 , 138.07689476, 141.4315819 ,
       144.77543415, 148.10931061, 151.43397698, 154.75011852,
       158.05835073, 161.35922842, 164.65325326, 167.94088027,
       171.22252332, 174.49855983, 177.76933484, 181.03516456,
       184.29633938, 187.55312656, 190.80577256, 194.05450508,
       197.29953486, 200.5410573 , 203.77925385, 207.01429327,
       210.24633275, 213.47551892, 216.70198873, 219.92587029,
       223.14728353, 226.36634091 },
      { 52.99764039,  60.02631933,  65.27272333,  69.95169273,
        74.31634998,  78.47518745,  82.48683185,  86.38721278,
        90.20019962,  93.94257225,  97.62663288, 101.26169896,
       104.85501293, 108.41232507, 111.93828238, 115.43669695,
       118.91073648, 122.3630627 , 125.79593425, 129.21128434,
       132.61078049, 135.99587102, 139.36782174, 142.72774525,
       146.07662452, 149.41533217, 152.74464616, 156.06526291,
       159.37780814, 162.68284599, 165.98088673, 169.27239329,
       172.55778682, 175.83745151, 179.11173866, 182.38097027,
       185.64544215, 188.90542659, 192.16117475, 195.41291872,
       198.66087337, 201.90523795, 205.14619751, 208.38392423,
       211.6185785 , 214.85030996, 218.0792584 , 221.30555462,
       224.52932107, 227.75067263 }

    };

    return table[ n ][ rootIndex ];

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getZeroBesselDerivative( "
             "const uint32_t& n, "
             "const uint32_t& rootIndex ) const" );

}


double
gkg::NumericalAnalysisImplementationFactory::getWatsonDistribution(
                        double kappa,
                        const gkg::Vector3d< float >& principalOrientation,
                        const gkg::Vector3d< float >& currentOrientation ) const
{

  try
  {
  
    double result = 0.0;

    if ( kappa < 0.0 )
    {

      throw std::runtime_error( "kappa parameter must be positive" );

    }

    double hypergeometricFunction1F1 = this->getHypergeometricFunction1F1(
                                                                       0.5,
                                                                       1.5,
                                                                       kappa );
    if ( hypergeometricFunction1F1 )
    {

      double dotProduct = ( double )principalOrientation.x *
                          ( double )currentOrientation.x +
                          ( double )principalOrientation.y *
                          ( double )currentOrientation.y +
                          ( double )principalOrientation.z *
                          ( double )currentOrientation.z;

      result = ( 1.0 / hypergeometricFunction1F1 ) * 
               std::exp( kappa * dotProduct * dotProduct );
						 
    }
    
    return result;
  
  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getWatsonDistribution( "
             "double kappa, "
             "const gkg::Vector3d< float >& principalOrientation, "
             "const gkg::Vector3d< float >& currentOrientation ) const" );
}


double
gkg::NumericalAnalysisImplementationFactory::getWatsonDistribution(
                       double kappa,
                       const gkg::Vector3d< double >& principalOrientation,
                       const gkg::Vector3d< double >& currentOrientation ) const
{

  try
  {
  
    double result = 0.0;

    if ( kappa < 0.0 )
    {

      throw std::runtime_error( "kappa parameter must be positive" );

    }

    double hypergeometricFunction1F1 = this->getHypergeometricFunction1F1(
                                                                       0.5,
                                                                       1.5,
                                                                       kappa );
    if ( hypergeometricFunction1F1 )
    {

      double dotProduct = principalOrientation.dot( currentOrientation );
      result = ( 1.0 / hypergeometricFunction1F1 ) * 
               std::exp( kappa * dotProduct * dotProduct );
						 
    }
    
    return result;
  
  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getWatsonDistribution( "
             "double kappa, "
             "const gkg::Vector3d< double >& principalOrientation, "
             "const gkg::Vector3d< double >& currentOrientation ) const" );
}


gkg::Vector
gkg::NumericalAnalysisImplementationFactory::
                              getCanonicalWatsonSphericalHarmonicsDecomposition(
               double kappa,
               int32_t maximumSphericalHarmonicsOrder,
               const std::vector< gkg::Vector3d< float > >& orientations ) const
{

  try
  {

    int32_t orientationCount = ( int32_t )orientations.size();

    gkg::Matrix B( orientationCount, maximumSphericalHarmonicsOrder + 1 );

    int32_t o = 0;
    int32_t l = 0;

    // building matrix of spherical harmonics basis
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;
    for ( o = 0; o < orientationCount; o++ )
    {

      const gkg::Vector3d< float >& orientation = orientations[ o ];
      this->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                ( double )orientation.y,
                                                ( double )orientation.z,
                                                radial,
                                                theta,
                                                phi );

      for ( l = 0; l <= maximumSphericalHarmonicsOrder; l++ )
      {

        B( o, l ) = 
                 std::real( this->getSphericalHarmonic( l, 0, phi, theta ) );

      }

    }

    // computing the matrix for spherical harmonics decomposition
    gkg::Matrix Bplus;
    this->getMoorePenrosePseudoInverse( B, Bplus );

    // computing values of canonical Watson distribution along provided
    // orientations
    gkg::Vector watsonDistribution( orientationCount );
    gkg::Vector3d< float > principalOrientation( 0.0, 0.0, 1.0 );
    for ( o = 0; o < orientationCount; o++ )
    {

      const gkg::Vector3d< float >& orientation = orientations[ o ];
      watsonDistribution( o ) = this->getWatsonDistribution(
                                                           kappa,
                                                           principalOrientation,
                                                           orientation );

    }

    return Bplus.getComposition( watsonDistribution );

  }
  GKG_CATCH( "gkg::Vector "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getCanonicalWatsonSphericalHarmonicsDecomposition( "
             "double kappa, "
             "int32_t maximumSphericalHarmonicsOrder, "
             "const std::vector< gkg::Vector3d< float > >& orientations ) "
             "const" );

}


void gkg::NumericalAnalysisImplementationFactory::getRandomWatsonOrientation(
                            double kappa,
                            const gkg::Vector3d< float >& principalOrientation,
                            const gkg::RandomGenerator& randomGenerator,
                            gkg::Vector3d< float >& orientation ) const
{

  try
  {

    gkg::Vector3d< double > orientationDouble;
    this->getRandomWatsonOrientation( kappa,
                                      gkg::Vector3d< double >(
                                          ( double )principalOrientation.x,
                                          ( double )principalOrientation.y,
                                          ( double )principalOrientation.z ),
                                      randomGenerator,
                                      orientationDouble );
    orientation.x = ( float )orientationDouble.x;
    orientation.y = ( float )orientationDouble.y;
    orientation.z = ( float )orientationDouble.z;

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getRandomWatsonOrientation( "
             "double kappa, "
             "const gkg::Vector3d< float >& principalOrientation, "
             "const gkg::RandomGenerator& randomGenerator, "
             "gkg::Vector3d< float >& orientation ) const" );

}


void gkg::NumericalAnalysisImplementationFactory::getRandomWatsonOrientation(
                            double kappa,
                            const gkg::Vector3d< double >& principalOrientation,
                            const gkg::RandomGenerator& randomGenerator,
                            gkg::Vector3d< double >& orientation ) const
{

  try
  {

    // get mean orientation in spherical coordinates 
    double alpha  = std::atan2( principalOrientation.y,
                                principalOrientation.z );
    double beta  = std::atan2(  principalOrientation.x,
                                std::sqrt( principalOrientation.z *
                                           principalOrientation.z + 
                                           principalOrientation.y *
                                           principalOrientation.y ) );

    // simulate Watson distribution
    double c = 1.0 / ( std::exp( ( double )kappa ) - 1.0 ); 
    double u = 0.0;
    double v = 0.0;
    double s = 0.0;
    double e = -1.0;

    while ( v > e )
    {

      u = this->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );

      v = this->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );

      s = ( 1.0 / kappa ) * std::log( u / c + 1.0 );

      e = std::exp( kappa * s * ( s - 1.0 ) );

    }

    double theta = std::acos( s );

    double r0 = this->getUniformRandomNumber( randomGenerator, 0.0, 1.0 );
    double phi = 2.0 * M_PI * r0;

    // generate simulated orientation 
    double x = std::sin( theta ) * std::cos( phi );
    double y = std::sin( theta ) * std::sin( phi );
    double z = std::cos( theta );

    // rotate simulated orientation according to input mean orientation
    orientation.x = std::cos( beta ) * x +
                    std::sin( beta ) * z;
    orientation.y = std::sin( alpha ) * std::sin( beta ) * x + 
                    std::cos( alpha ) * y -
                    std::cos( beta ) * std::sin( alpha ) * z;
    orientation.z = - std::sin( beta ) * std::cos( alpha ) * x +
                    std::sin( alpha ) * y +
                    std::cos( alpha ) * std::cos( beta ) * z; 

    orientation.normalize();

    if ( kappa >= 1e6 )
    {

      orientation = principalOrientation;

    }

    // make sure that the orientation is the same direction as mean
    // due to z axis indetermination
    if ( ( principalOrientation.x * orientation.x ) < 0.0 )
    {

      orientation.x *= -1.0;

    }

  }
  GKG_CATCH( "void gkg::NumericalAnalysisImplementationFactory::"
             "getRandomWatsonOrientation( "
             "double kappa, "
             "const gkg::Vector3d< double >& principalOrientation, "
             "const gkg::RandomGenerator& randomGenerator, "
             "gkg::Vector3d< double >& orientation ) const" );

}


double gkg::NumericalAnalysisImplementationFactory::
                                   getWatsonKappaParameterFromAngularDispersion(
                                                double angularDispersion, 
                                                bool degrees,
                                                int32_t orientationCount,
                                                double epsilon ) const
{

  try
  {

    double targetAngularDispersion = angularDispersion;
    double targetEpsilon = epsilon;
    if ( degrees )
    {

      targetAngularDispersion *= M_PI / 180.0;
      targetEpsilon *= M_PI / 180.0;

    }

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    gkg::Vector3d< float > zAxis( 0.0f, 0.0f, 1.0f );
    gkg::Vector3d< float > orientation;
    int32_t o = 0;
    double lowerKappa = 0.0;
    double medianKappa = 500.0;
    double upperKappa = 1000.0;

    double currentAngularDispersion = targetAngularDispersion + 
                                      2 * targetEpsilon;
    double currentAngularDispersionForLowerKappa = 0.0;
    double currentAngularDispersionForMedianKappa = 0.0;
    double currentAngularDispersionForUpperKappa = 0.0;

    double discrepancyOfAngularDispersionForLowerKappa = 0.0;
    double discrepancyOfAngularDispersionForMedianKappa = 0.0;
    double discrepancyOfAngularDispersionForUpperKappa = 0.0;

    bool lowerKappaIsBetter = false;
    double bestKappa = 0.0;

    while ( std::fabs( targetAngularDispersion - currentAngularDispersion ) >
            targetEpsilon )
    {

      // computing orientation distribution for lower kappa
      currentAngularDispersionForLowerKappa = 0.0; 
      for ( o = 0; o < orientationCount; o++ )
      {

        this->getRandomWatsonOrientation( lowerKappa,
                                          zAxis,
                                          randomGenerator,
                                          orientation );
        currentAngularDispersionForLowerKappa += 
                                       gkg::getLineAngles( orientation, zAxis );

      }
      currentAngularDispersionForLowerKappa /= ( double )orientationCount;

      // computing orientation distribution for median kappa
      currentAngularDispersionForMedianKappa = 0.0; 
      for ( o = 0; o < orientationCount; o++ )
      {

        this->getRandomWatsonOrientation( medianKappa,
                                          zAxis,
                                          randomGenerator,
                                          orientation );
        currentAngularDispersionForMedianKappa +=
                                       gkg::getLineAngles( orientation, zAxis );

      }
      currentAngularDispersionForMedianKappa /= ( double )orientationCount;

      // computing orientation distribution for upper kappa
      currentAngularDispersionForUpperKappa = 0.0; 
      for ( o = 0; o < orientationCount; o++ )
      {

        this->getRandomWatsonOrientation( upperKappa,
                                          zAxis,
                                          randomGenerator,
                                          orientation );
        currentAngularDispersionForUpperKappa += 
                                       gkg::getLineAngles( orientation, zAxis );

      }
      currentAngularDispersionForUpperKappa /= ( double )orientationCount;

      discrepancyOfAngularDispersionForLowerKappa =
        std::fabs( currentAngularDispersionForLowerKappa - 
                     targetAngularDispersion );
      discrepancyOfAngularDispersionForMedianKappa =
        std::fabs( currentAngularDispersionForMedianKappa - 
                     targetAngularDispersion );
      discrepancyOfAngularDispersionForUpperKappa =
        std::fabs( currentAngularDispersionForUpperKappa - 
                     targetAngularDispersion );

      if ( targetAngularDispersion >
           currentAngularDispersionForMedianKappa )
      {

        upperKappa = medianKappa;
        lowerKappaIsBetter = true;

      }
      else
      {

        lowerKappa = medianKappa;
        lowerKappaIsBetter = false;

      }      
      medianKappa = ( lowerKappa + upperKappa ) / 2.0;

      if ( lowerKappaIsBetter )
      {

        if ( discrepancyOfAngularDispersionForLowerKappa <
             discrepancyOfAngularDispersionForMedianKappa )
        {

          currentAngularDispersion = currentAngularDispersionForLowerKappa;
          bestKappa = lowerKappa;

        }
        else
        {

          currentAngularDispersion = currentAngularDispersionForMedianKappa;
          bestKappa = medianKappa;

        }

      }
      else
      {

        if ( discrepancyOfAngularDispersionForUpperKappa <
             discrepancyOfAngularDispersionForMedianKappa )
        {

          currentAngularDispersion = currentAngularDispersionForUpperKappa;
          bestKappa = upperKappa;

        }
        else
        {

          currentAngularDispersion = currentAngularDispersionForMedianKappa;
          bestKappa = medianKappa;

        }

      }

      //std::cout << "target=" << targetAngularDispersion << " "
      //          << "current=" << currentAngularDispersion << std::endl;
                

    }

    return bestKappa;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getWatsonKappaParameterFromAngularDispersion( "
             "double angularDispersion, "
             "bool degrees ) const" );


}


double gkg::NumericalAnalysisImplementationFactory::
                        getPrecomputedWatsonKappaParameterFromAngularDispersion(
                                                double angularDispersion, 
                                                bool degrees ) const
{

  try
  {

    double kappas[ 531 ] = { 0.343323,
                             0.305176,
                             0.37384,
                             0.38147,
                             0.404358,
                             0.457764,
                             0.434875,
                             0.442505,
                             0.465393,
                             0.244141,
                             0.488281,
                             0.50354,
                             0.518799,
                             0.518799,
                             0.549316,
                             0.572205,
                             0.595093,
                             0.602722,
                             0.671387,
                             0.63324,
                             0.656128,
                             0.663757,
                             0.701904,
                             0.694275,
                             0.686646,
                             0.709534,
                             0.610352,
                             0.732422,
                             0.747681,
                             0.762939,
                             0.762939,
                             0.793457,
                             0.82016,
                             0.839233,
                             0.854492,
                             0.915527,
                             0.87738,
                             0.900269,
                             0.907898,
                             0.946045,
                             0.938416,
                             0.930786,
                             0.953674,
                             0.976562,
                             0.98484,
                             0.995636,
                             1.02234,
                             1.00708,
                             1.06812,
                             1.06049,
                             1.05286,
                             1.07574,
                             1.09863,
                             1.15967,
                             1.11389,
                             1.13297,
                             1.12915,
                             1.15967,
                             1.18256,
                             1.20544,
                             1.21307,
                             1.09863,
                             1.22833,
                             1.23596,
                             1.25885,
                             1.25122,
                             1.28555,
                             1.30463,
                             1.32751,
                             1.33514,
                             1.28174,
                             1.3504,
                             1.35803,
                             1.38092,
                             1.43433,
                             1.41907,
                             1.43433,
                             1.44958,
                             1.45721,
                             1.70898,
                             1.48773,
                             1.4801,
                             1.50299,
                             1.5564,
                             1.53351,
                             1.54114,
                             1.56593,
                             1.58691,
                             1.64795,
                             1.6098,
                             1.63269,
                             1.64032,
                             1.67847,
                             1.65558,
                             1.66321,
                             1.6861,
                             1.58691,
                             1.71661,
                             1.73187,
                             1.75476,
                             1.7395,
                             1.77002,
                             1.79291,
                             1.8158,
                             1.80817,
                             1.77002,
                             1.83868,
                             1.84631,
                             1.8692,
                             1.86157,
                             1.90353,
                             1.91498,
                             1.93787,
                             1.9455,
                             1.96069,
                             1.97601,
                             1.9989,
                             2.00653,
                             2.01364,
                             2.02179,
                             2.02942,
                             2.05231,
                             2.0752,
                             2.08282,
                             2.10571,
                             2.12097,
                             2.13623,
                             2.16675,
                             2.16293,
                             2.18201,
                             2.18582,
                             2.31934,
                             2.22015,
                             2.24304,
                             2.25067,
                             2.22778,
                             2.26593,
                             2.30408,
                             2.29645,
                             2.2583,
                             2.32697,
                             2.33459,
                             2.35748,
                             2.38037,
                             2.388,
                             2.39563,
                             2.42233,
                             2.44141,
                             2.68555,
                             2.45667,
                             2.48337,
                             2.50244,
                             2.53296,
                             2.53296,
                             2.53677,
                             2.56348,
                             2.62451,
                             2.59399,
                             2.59781,
                             2.62451,
                             2.65503,
                             2.65503,
                             2.66647,
                             2.68555,
                             2.68555,
                             2.71606,
                             2.71988,
                             2.74658,
                             2.74658,
                             2.76184,
                             2.78854,
                             2.80762,
                             2.86865,
                             2.82288,
                             2.84576,
                             2.83813,
                             2.87628,
                             2.88391,
                             2.9068,
                             2.92969,
                             2.92969,
                             2.97546,
                             2.97546,
                             3.02124,
                             3.0098,
                             3.0365,
                             3.04031,
                             3.11279,
                             3.07846,
                             3.0899,
                             3.10898,
                             3.13568,
                             3.12805,
                             3.1662,
                             3.05176,
                             3.19672,
                             3.2196,
                             3.22342,
                             3.26538,
                             3.25394,
                             3.27301,
                             3.23486,
                             3.31879,
                             3.34167,
                             3.34167,
                             3.38745,
                             3.37601,
                             3.39508,
                             3.41797,
                             3.44086,
                             3.46375,
                             3.46756,
                             3.48663,
                             3.49426,
                             3.53241,
                             3.479,
                             3.55911,
                             3.58582,
                             3.60107,
                             3.62396,
                             3.64685,
                             3.65067,
                             3.66211,
                             3.67737,
                             3.71552,
                             3.75366,
                             3.74985,
                             3.77655,
                             3.79314,
                             3.81088,
                             3.82233,
                             3.8147,
                             3.8681,
                             3.88336,
                             3.90312,
                             3.92914,
                             3.93677,
                             3.93677,
                             3.99017,
                             4.00925,
                             4.08936,
                             4.05121,
                             4.07028,
                             4.11987,
                             4.11224,
                             4.1275,
                             4.02832,
                             4.16565,
                             4.18854,
                             4.21143,
                             4.2572,
                             4.21143,
                             4.28009,
                             4.30298,
                             4.30298,
                             4.34875,
                             4.37546,
                             4.63867,
                             4.40979,
                             4.45557,
                             4.4632,
                             4.48608,
                             4.57764,
                             4.53186,
                             4.55475,
                             4.58145,
                             4.62341,
                             4.63867,
                             4.66156,
                             4.67682,
                             4.71115,
                             4.74548,
                             4.82178,
                             4.776,
                             4.82178,
                             4.82941,
                             4.86374,
                             4.88281,
                             4.92859,
                             4.97437,
                             4.95911,
                             4.94385,
                             5.02777,
                             5.05829,
                             5.07355,
                             5.10406,
                             5.00488,
                             5.15747,
                             5.18799,
                             5.23376,
                             5.31006,
                             5.2948,
                             5.34058,
                             5.35583,
                             5.61523,
                             5.41687,
                             5.46265,
                             5.47791,
                             5.49316,
                             5.53131,
                             5.5542,
                             5.59235,
                             5.7373,
                             5.66864,
                             5.69153,
                             5.67627,
                             5.75256,
                             5.82886,
                             5.82886,
                             5.85938,
                             5.89752,
                             5.93567,
                             5.92041,
                             6.01196,
                             6.05011,
                             6.09589,
                             6.14929,
                             6.19507,
                             6.2027,
                             6.24084,
                             6.31714,
                             6.32477,
                             6.36292,
                             6.43921,
                             6.44684,
                             6.5155,
                             6.53076,
                             6.5918,
                             6.62231,
                             6.66809,
                             6.7749,
                             6.74438,
                             6.80542,
                             6.8512,
                             6.92749,
                             6.89697,
                             7.01141,
                             7.06482,
                             7.09534,
                             7.15637,
                             7.26318,
                             7.23267,
                             7.32422,
                             7.37762,
                             7.4234,
                             7.47681,
                             7.5531,
                             7.61414,
                             7.64465,
                             7.70569,
                             7.76672,
                             7.8125,
                             7.90405,
                             7.94983,
                             8.01086,
                             8.0719,
                             8.16345,
                             8.22449,
                             8.05664,
                             8.3313,
                             8.48389,
                             8.5144,
                             8.56018,
                             8.65173,
                             8.69751,
                             9.27734,
                             8.86536,
                             8.95691,
                             9.15527,
                             9.1095,
                             9.20105,
                             9.52148,
                             9.35364,
                             9.42993,
                             9.53674,
                             9.70459,
                             8.78906,
                             9.79614,
                             9.93347,
                             10.0098,
                             10.1929,
                             10.2539,
                             10.3455,
                             10.4065,
                             10.5591,
                             10.6659,
                             10.7422,
                             10.8643,
                             10.9863,
                             11.1084,
                             11.4746,
                             11.3525,
                             11.4746,
                             11.6425,
                             11.7188,
                             11.8713,
                             11.9934,
                             12.1765,
                             12.2681,
                             12.5732,
                             12.619,
                             12.7258,
                             12.8174,
                             13.1226,
                             13.2141,
                             13.3972,
                             13.6108,
                             13.7024,
                             14.0381,
                             14.0686,
                             14.3433,
                             14.4348,
                             14.1602,
                             14.8621,
                             15.0146,
                             15.3198,
                             15.4419,
                             15.6555,
                             15.8691,
                             16.3574,
                             16.4795,
                             16.1133,
                             16.9678,
                             16.8457,
                             17.4561,
                             17.5781,
                             17.8223,
                             18.158,
                             18.4937,
                             18.6768,
                             19.0125,
                             19.4092,
                             19.5312,
                             19.8975,
                             20.1416,
                             20.5078,
                             20.9045,
                             21.2402,
                             21.6675,
                             22.2168,
                             22.3694,
                             22.7661,
                             23.3154,
                             23.6206,
                             23.9868,
                             24.9023,
                             24.6582,
                             24.4141,
                             25.8789,
                             25.8789,
                             26.6113,
                             27.2969,
                             27.8931,
                             28.5034,
                             29.1748,
                             29.6021,
                             29.7852,
                             30.9448,
                             31.6162,
                             31.7383,
                             32.9285,
                             33.5999,
                             34.3933,
                             35.1562,
                             36.0107,
                             36.8652,
                             37.5977,
                             38.5132,
                             39.4287,
                             40.4053,
                             41.3818,
                             42.4194,
                             43.4265,
                             44.6777,
                             45.7764,
                             47.6875,
                             48.2178,
                             49.5605,
                             50.8281,
                             52.1851,
                             53.2227,
                             54.9316,
                             56.6406,
                             58.5938,
                             60.3027,
                             61.7676,
                             63.7207,
                             65.6738,
                             67.9932,
                             70.3125,
                             72.7539,
                             74.707,
                             77.3926,
                             80.2002,
                             83.0078,
                             86.0596,
                             89.1113,
                             93.2617,
                             96.4355,
                             99.8535,
                             104.248,
                             108.887,
                             112.549,
                             117.432,
                             122.803,
                             128.174,
                             134.033,
                             140.812,
                             146.973,
                             155.273,
                             163.086,
                             170.654,
                             183.594,
                             190.43,
                             199.707,
                             210.938,
                             223.633,
                             236.328,
                             252.93,
                             267.578,
                             286.133,
                             307.617,
                             329.59,
                             353.516,
                             382.812,
                             414.062,
                             447.266,
                             490.234,
                             534.18,
                             589.844,
                             644.531 };



    double angularDispersionInDegrees = angularDispersion;
    if ( degrees == false )
    {

      angularDispersionInDegrees = angularDispersion * 180.0 / M_PI;

    }

    double angularDispersionInDegreesTimes10 = angularDispersionInDegrees *
                                               10.0;


    int32_t lowerIntegerAngularDispersionInDegreesTimes10 =
                                   ( int32_t )angularDispersionInDegreesTimes10;


    int32_t lowestIntegerAngularDispersionInDegreesTimes10 = 20;
    int32_t uppestIntegerAngularDispersionInDegreesTimes10 = 550;


    double kappa = 0.0;

    if ( lowerIntegerAngularDispersionInDegreesTimes10 <
         lowestIntegerAngularDispersionInDegreesTimes10 )
    {

      kappa =  700.0;

    }
    else if ( lowerIntegerAngularDispersionInDegreesTimes10 + 1 > 
              uppestIntegerAngularDispersionInDegreesTimes10 )
    {

      this->getWatsonKappaParameterFromAngularDispersion(
                                                angularDispersion, 
                                                degrees,
                                                100000,
                                                angularDispersion / 1000.0 );

    }
    else
    {

      double kappa1 = kappas[
                           529 -
                           ( lowerIntegerAngularDispersionInDegreesTimes10 -
                             lowestIntegerAngularDispersionInDegreesTimes10 ) ];
      double kappa2 = kappas[
                           529 -
                           ( lowerIntegerAngularDispersionInDegreesTimes10 -
                             lowestIntegerAngularDispersionInDegreesTimes10 +
                              1 ) ];
      double deltaAngularDispersion =
                        angularDispersionInDegreesTimes10 - 
                        ( double )lowerIntegerAngularDispersionInDegreesTimes10;

      kappa =  kappa1 + deltaAngularDispersion * ( kappa2 - kappa1 );

    }
    return kappa;

  }
  GKG_CATCH( "double gkg::NumericalAnalysisImplementationFactory::"
             "getPrecomputedWatsonKappaParameterFromAngularDispersion( "
             "double angularDispersion, "
             "bool degrees ) const" );

}


double
gkg::NumericalAnalysisImplementationFactory::getBinghamDistribution(
                  double /* kappa1 */,
                  double /* kappa2 */,
                  const gkg::Vector3d< float >& /* principalOrientation */,
                  const gkg::Vector3d< float >& /* secondaryOrientation */,
                  const gkg::Vector3d< float >& /* currentOrientation */ ) const
{

  try
  {
  
    double result = 0.0;

    ////to be written
    
    return result;
  
  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getBinghamDistribution( "
             "double kappa1, "
             "double kappa2, "
             "const gkg::Vector3d< float >& principalOrientation, "
             "const gkg::Vector3d< float >& secondaryOrientation, "
             "const gkg::Vector3d< float >& currentOrientation ) const" );
}


double
gkg::NumericalAnalysisImplementationFactory::getBinghamDistribution(
                 double /* kappa1 */,
                 double /* kappa2 */,
                 const gkg::Vector3d< double >& /* principalOrientation */,
                 const gkg::Vector3d< double >& /* secondaryOrientation */,
                 const gkg::Vector3d< double >& /* currentOrientation */ ) const
{

  try
  {
  
    double result = 0.0;

    ////to be written
    
    return result;
  
  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getBinghamDistribution( "
             "double kappa1, "
             "double kappa2, "
             "const gkg::Vector3d< double >& principalOrientation, "
             "const gkg::Vector3d< double >& secondaryOrientation, "
             "const gkg::Vector3d< double >& currentOrientation ) const" );
}


void gkg::NumericalAnalysisImplementationFactory::getRandomESAGOrientation( 
                            const std::pair< double, double >& gammas,
                            const gkg::Vector3d< double >& principalOrientation,
                            const gkg::RandomGenerator& randomGenerator,
                            gkg::Vector3d< double >& orientation ) const
{

  try
  {

    double muN = principalOrientation.getNorm();
    double mu1 = principalOrientation.x;
    double mu2 = principalOrientation.y;
    double mu3 = principalOrientation.z;
 
    double muNSq = muN * muN;
    double mu1Sq = mu1 * mu1;
    double mu2Sq = mu2 * mu2;
    double mu3Sq = mu3 * mu3;
    double mu0Sq = mu2 * mu2 + mu3 * mu3;

    double gamma1 = gammas.first;
    double gamma2 = gammas.second;

    double alpha = std::sqrt( gamma1 * gamma1 + gamma2 * gamma2 + 1.0) - 1.0;

    // compute 3x3 variance-covariance matrix V according to 
    // principalOrientation and gammas
    gkg::Matrix Vinv(3,3);

    Vinv( 0, 0 ) = 1.0 + ( gamma1 + alpha ) * mu0Sq / muNSq;
    Vinv( 0, 1 ) = gamma2 * mu3 / muN - ( gamma1 + alpha ) * mu1 * mu2 / muNSq;
    Vinv( 0, 2 ) = -( gamma1 + alpha ) * mu1 * mu3 / muNSq -
                   gamma2 * ( mu2 / muN + mu1 * mu3 / muNSq );
    Vinv( 1, 0 ) = -( gamma1 + alpha ) * mu1 * mu2 / muNSq + gamma2 * mu3 / muN;
    Vinv( 1, 1 ) = 1.0 + gamma1 * ( mu1Sq* mu2Sq / ( mu0Sq * muNSq ) - 
                                    mu3Sq / mu0Sq ) -
                   2.0 * gamma2 * mu1 * mu2 * mu3 / ( mu0Sq * muN ) + 
                   alpha * ( mu1Sq * mu2Sq / ( mu0Sq * muNSq ) + mu3Sq / mu0Sq);
    Vinv( 1, 2 ) = gamma1 * ( mu1Sq * mu2 * mu3 / ( mu0Sq * muNSq ) +
                              mu2 * mu3 / mu0Sq ) +
                   gamma2 * ( ( mu1 * mu2Sq - mu1 * mu3Sq ) /
                              ( mu0Sq  * muN ) ) +
                   alpha * ( mu1Sq * mu2 * mu3 / ( mu0Sq * muNSq ) -
                             mu2 * mu3 / mu0Sq );
    Vinv( 2, 0 ) = -( gamma1 + alpha ) * mu1 * mu3 / muNSq - gamma2 * mu2 / muN;
    Vinv( 2, 1 ) = gamma1 * ( mu1Sq * mu2 * mu3 / ( mu0Sq * muNSq ) +
                              mu2 * mu3 / mu0Sq ) +
                   gamma2 * ( ( mu1 * mu2Sq - mu1 * mu3Sq ) / ( mu0Sq * muN ) )+
                   alpha * ( mu1Sq * mu2 * mu3 / ( mu0Sq * muNSq ) -
                             mu2 * mu3 / mu0Sq );
    Vinv( 2, 2 ) = 1.0 + gamma1 * ( mu1Sq * mu3Sq / ( mu0Sq * muNSq ) - 
                                    mu2Sq / mu0Sq ) + 
                   2.0 * gamma2 * mu1 * mu2 * mu3 / ( mu0Sq * muN ) + 
                   alpha * ( mu1Sq * mu3Sq / ( mu0Sq * muNSq ) + 
                             mu2Sq / mu0Sq );


    gkg::Matrix V(3,3);
    this->getMoorePenrosePseudoInverse( Vinv, V );


    // simulate z ~ N( principalOrientation, V )
    int32_t seed =  ( int32_t )this->getUniformRandomUInt32(
                        randomGenerator,
                        ( uint32_t )( std::numeric_limits< int32_t >::max() ) );

    std::vector< double > a( 9 );
    a[ 0 ] = V( 0, 0 );
    a[ 1 ] = V( 0, 1 );
    a[ 2 ] = V( 0, 2 );
    a[ 3 ] = V( 1, 0 );
    a[ 4 ] = V( 1, 1 );
    a[ 5 ] = V( 1, 2 );
    a[ 6 ] = V( 2, 0 );
    a[ 7 ] = V( 2, 1 );
    a[ 8 ] = V( 2, 2 );

    std::vector< double > mu( 3 );
    mu[ 0 ] = principalOrientation.x;
    mu[ 1 ] = principalOrientation.y;
    mu[ 2 ] = principalOrientation.z;


    // first computing the Cholesky R factor of the variance-covariance matrix
    std::vector< double > R( 9 );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    double s = 0.0;

    for ( j = 0; j < 3; j++ )
    {

      for ( i = 0; i < 3; i++ )
      {

        R[ i + j * 3 ] = a[ i + j * 3 ];

      }

    }

    for ( j = 0; j < 3; j++ )
    {

      for ( k = 0; k <= j - 1; k++ )
      {

        for ( i = 0; i <= k - 1; i++ )
        {

          R[ k + j * 3 ] = R[ k + j * 3 ] - R[ i + k * 3 ] * R[ i + j * 3 ];

        }
        R[ k + j * 3 ] = R[ k + j * 3 ] / R[ k + k * 3 ];

      }

      s = R[ j + j * 3 ];
      for ( i = 0; i <= j - 1; i++ )
      {

        s = s - R[ i + j * 3 ] * R[ i + j * 3 ];

      }

      if ( s <= 0.0 )
      {

        throw std::runtime_error(
                       "covariance matrix is not positive definite symmetric" );

      }

      R[ j + j * 3 ] = std::sqrt( s );

    }

    //  zero out the lower triangle.
    for ( i = 0; i < 3; i++ )
    {

      for ( j = 0; j < i; j++ )
      {

        R[ i + j * 3 ] = 0.0;

      }

    }

    //  Y = MxN matrix of samples of the 1D normal dist. with mean/var=0.0/1.0  
    double* y = r8vec_normal_01_new ( 3, &seed );


    // computing X = MU + R' * Y.
    std::vector< double > x( 3 );
    for ( i = 0; i < 3; i++ )
    {

      x[ i ] = mu[i];
      for ( k = 0; k < 3; k++ )
      {

        x[ i ] = x[ i ] + R[ k + i * 3 ] * y[ k ];

      }

    }

    delete [] y;

    // now copying x to the ouput orientation
    orientation.x = x[ 0 ];
    orientation.y = x[ 1 ];
    orientation.z = x[ 2 ];

    // set orientation = z / ||z||
    orientation.normalize();

  }
  GKG_CATCH( "void "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getRandomESAGOrientation( "
             "const std::pair< double, double >& gammas, "
             "const gkg::Vector3d< double >& principalOrientation, "
             "const gkg::RandomGenerator& randomGenerator, "
             "gkg::Vector3d< double >& orientation ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getPowellNewUOAOptimumParameters(
                                                gkg::Vector& parameters,
                                                gkg::PowellNewUOAFunction& f,
                                                double initialTrustRegionRadius,
                                                int32_t maximumIterationCount,
                                                double maximumTestSize,
                                                bool verbose ) const
{

  try
  {

    // collecting the parameter count
    int32_t parameterCount = parameters.getSize();
    if ( f.getParameterCount() != parameterCount )
    {

      throw std::runtime_error( "Powell's New UOA function and parameters have "
                                "incompatible size(s)" );

    }

    int32_t pointCount = 2 * parameterCount + 1;
    int32_t rnf = 0;

    std::vector< double > w( ( pointCount + 13 ) *
                             ( pointCount + parameterCount ) +
                             3 * parameterCount * ( parameterCount + 3 ) / 2 +
                             11 );

    return newuoa( parameterCount,
                   pointCount,
                   &parameters( 0 ),
                   initialTrustRegionRadius,
                   maximumTestSize,
                   &rnf,
                   maximumIterationCount,
                   &w[ 0 ],
                   f,
                   verbose );

  }
  GKG_CATCH( "int32_t "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getPowellNewUOAOptimumParameters( "
             "gkg::Vector& parameters, "
             "gkg::PowellNewUOAFunction& f, "
             "double initialTrustRegionRadius, "
             "int32_t maximumIterationCount, "
             "double maximumTestSize, "
             "bool verbose ) const" );

}



void 
gkg::NumericalAnalysisImplementationFactory::getCartesianToSphericalCoordinates(
                                                      double x,
                                                      double y,
                                                      double z,
                                                      double& radial,
                                                      double& azimuth,
                                                      double& colatitude ) const
{

  try
  {

    radial = std::sqrt( x * x + y * y + z * z );
    
    double zOverRadial = z / radial;
    if ( zOverRadial > 1.0 )
    {

      colatitude = 0.0;

    }
    else if ( zOverRadial < -1.0 )
    {

      colatitude = M_PI;

    }
    else
    {

      colatitude = std::acos( zOverRadial );
 
    }

    if ( x == 0.0 )
    {

      if ( y >= 0.0 )
      {

        azimuth = + M_PI / 2;

      }
      else
      {

        azimuth = - M_PI / 2;

      }

    }
    else
    {

      if ( ( y >= 0.0 ) && ( x > 0.0 ) )
      {

        azimuth = std::atan( y / x );

      }
      else if ( ( y <= 0.0 ) && ( x < 0.0 ) )
      {

        azimuth = std::atan( y / x ) + M_PI;

      } 
      else if ( ( y <= 0.0 ) && ( x > 0.0 ) )
      {

        azimuth = std::atan( y / x ) + 2 * M_PI;

      } 
      else if ( ( y >= 0.0 ) && ( x < 0.0 ) )
      {

        azimuth = std::atan( y / x ) + M_PI;

      } 

    }

  }
  GKG_CATCH( "void "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getCartesianToSphericalCoordinates( "
             "double x, "
             "double y, "
             "double z, "
             "double& radial, "
             "double& azimuth, "
             "double& colatitude ) const" );

}


void 
gkg::NumericalAnalysisImplementationFactory::getSphericalToCartesianCoordinates(
                                                      double radial,
                                                      double azimuth,
                                                      double colatitude,
                                                      double& x,
                                                      double& y,
                                                      double& z ) const
{

  try
  {

    x = radial * std::sin( colatitude ) * std::cos( azimuth );
    y = radial * std::sin( colatitude ) * std::sin( azimuth );
    z = radial * std::cos( colatitude );

  }
  GKG_CATCH( "void "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getSphericalToCartesianCoordinates( "
             "double radial, "
             "double azimuth, "
             "double colatitude, "
             "double& x, "
             "double& y, "
             "double& z ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getParzenWindowInterpolation(
                                                double x,
                                                const std::vector< double >& xs,
                                                const std::vector< double >& ys,
                                                double kernelWidth ) const
{

  try
  {

    // sanity check(s)
    if ( xs.size() != ys.size() )
    {

      throw std::runtime_error( "xs and ys have different size(s)" );

    }
    int32_t sampleCount = ( int32_t )xs.size();

    // computing the PDF value
    double value = 0;
    int32_t s = 0;
    for ( s = 0; s < sampleCount; s++ )
    {

      value += ys[ s ] *
               this->getGaussianPdf( ( x - xs[ s ] ) / kernelWidth,
                                     0.0, 1.0 );

    }
    return value / kernelWidth;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getParzenWindowInterpolation( "
             "double x, "
             "const std::vector< double >& xs, "
             "const std::vector< double >& ys, "
             "double kernelWidth ) const" );

}


double 
gkg::NumericalAnalysisImplementationFactory::getParzenWindowInterpolation(
                                                double /* x */,
                                                const gkg::Vector& xs,
                                                const gkg::Vector& ys,
                                                double kernelWidth ) const
{

  try
  {

    // sanity check(s)
    if ( xs.getSize() != ys.getSize() )
    {

      throw std::runtime_error( "xs and ys have different size(s)" );

    }
    int32_t sampleCount = ( int32_t )xs.getSize();

    // computing the PDF value
    double value = 0;
    int32_t s = 0;
    for ( s = 0; s < sampleCount; s++ )
    {

      value += ys( s ) * 
               this->getGaussianPdf( ( value - xs( s ) ) / kernelWidth,
                                     0.0, 1.0 );

    }
    return value / kernelWidth;

  }
  GKG_CATCH( "double "
             "gkg::NumericalAnalysisImplementationFactory::"
             "getParzenWindowInterpolation( "
             "double x, "
             "const std::vector< double >& xs, "
             "const std::vector< double >& ys, "
             "double kernelWidth ) const" );

}

