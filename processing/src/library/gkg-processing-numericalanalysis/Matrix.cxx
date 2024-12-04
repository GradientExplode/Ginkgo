#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/MatrixImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <algorithm>


gkg::Matrix::Matrix( int32_t size1, int32_t size2 )
{

  try
  {

    _matrixImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createMatrixImplementation( this, size1, size2 );

  }
  GKG_CATCH( "gkg::Matrix::Matrix( int size1, int size2 )" );

}


gkg::Matrix::Matrix( const gkg::Matrix& other )
{

  try
  {

    _matrixImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createMatrixImplementation( this,
                                                           other.getSize1(),
                                                           other.getSize2() );

    for ( int32_t s1 = 0; s1 < other.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < other.getSize2(); s2++ )
      {

        ( *this )( s1, s2 ) = other( s1, s2 );

      }

    }

  }
  GKG_CATCH( "gkg::Matrix::Matrix( const gkg::Matrix& other )" );

}


gkg::Matrix::~Matrix()
{

  delete _matrixImplementation;

}


gkg::Matrix& gkg::Matrix::operator=( const gkg::Matrix& other )
{

  try
  {

    reallocate( other.getSize1(), other.getSize2() );

    for ( int32_t s1 = 0; s1 < other.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < other.getSize2(); s2++ )
      {

        ( *this )( s1, s2 ) = other( s1, s2 );

      }

    }
    return *this;

  }
  GKG_CATCH( "gkg::Matrix& "
             "gkg::Matrix::operator=( const gkg::Matrix& other )" );

}


void gkg::Matrix::reallocate( int32_t size1, int32_t size2 )
{

  try
  {

    if ( ( size1 != getSize1() ) || ( size2 != getSize2() ) )
    {
         
      delete _matrixImplementation;
      _matrixImplementation =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createMatrixImplementation( this,
                                                           size1,
                                                           size2 );

    }

  }
  GKG_CATCH( "void gkg::Matrix::reallocate( int size1, int size2 )" );

}


int32_t gkg::Matrix::getSize1() const
{

  try
  {

    return _matrixImplementation->getSize1();

  }
  GKG_CATCH( "int gkg::Matrix::getSize1() const" );

}


int32_t gkg::Matrix::getSize2() const
{

  try
  {

    return _matrixImplementation->getSize2();

  }
  GKG_CATCH( "int gkg::Matrix::getSize2() const" );

}


const double& gkg::Matrix::operator()( int32_t i, int32_t j ) const
{

  try
  {

    return _matrixImplementation->operator()( i, j );

  }
  GKG_CATCH( "const double& gkg::Matrix::operator()( int i, int j ) const" );

}


double& gkg::Matrix::operator()( int32_t i, int32_t j )
{

  try
  {

    return _matrixImplementation->operator()( i, j );

  }
  GKG_CATCH( "const double& gkg::Matrix::operator()( int i, int j )" );

} 


bool gkg::Matrix::isSquare() const
{

  try
  {

    return ( _matrixImplementation->getSize1() == 
             _matrixImplementation->getSize2() );

  }
  GKG_CATCH( "bool gkg::Matrix::isSquare() const" );

}


void gkg::Matrix::setZero()
{

  try
  {

    _matrixImplementation->setZero();

  }
  GKG_CATCH( "void gkg::Matrix::setZero()" );

}


void gkg::Matrix::setIdentity()
{

  try
  {

    _matrixImplementation->setIdentity();

  }
  GKG_CATCH( "void gkg::Matrix::setIdentity()" );

}


void gkg::Matrix::setRow( int32_t s1, const Vector& row )
{

  try
  {

    if ( row.getSize() != getSize2() )
    {

      throw std::runtime_error( "incoherent size(s)" );

    }
    int32_t s2;
    for ( s2 = 0; s2 < getSize2(); s2++ )
    {

      ( *this )( s1, s2 ) = row( s2 );

    }

  }
  GKG_CATCH( "void gkg::Matrix::setRow( int32_t s1, const Vector& row )" );

}


void gkg::Matrix::setColumn( int32_t s2, const Vector& column )
{

  try
  {

    if ( column.getSize() != getSize1() )
    {

      throw std::runtime_error( "incoherent size(s)" );

    }
    int32_t s1;
    for ( s1 = 0; s1 < getSize1(); s1++ )
    {

      ( *this )( s1, s2 ) = column( s1 );

    }

  }
  GKG_CATCH( "void gkg::Matrix::setColumn( "
             "int32_t s2, const Vector& column )" );

}


void gkg::Matrix::fill( const double& value )
{

  try
  {

    _matrixImplementation->fill( value );

  }
  GKG_CATCH( "void gkg::Matrix::fill( const double& value )" );

}


void gkg::Matrix::fill( int32_t offset1,
                        int32_t offset2,
                        int32_t size1,
                        int32_t size2,
                        const double& value )
{

  try
  {

    if ( ( offset1 < 0 ) ||
         ( offset2 < 0 ) ||
         ( offset1 + size1 > getSize1() ) ||
         ( offset2 + size2 > getSize2() ) )

    {

      throw std::runtime_error( "incompatible offset(s)/size(s)" );

    }
    int32_t s1, s2;
    for ( s1 = 0; s1 < size1; s1++ )
    {

      for ( s2 = 0; s2 < size2; s2++ )
      {

        ( *this )( offset1 + s1, offset2 + s2 ) = value;

      }

    }

  }
  GKG_CATCH( "void gkg::Matrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "int32_t size1, "
             "int32_t size2, "
             "const double& value )" );

}


void gkg::Matrix::fill( int32_t offset1,
                        int32_t offset2,
                        const gkg::Matrix& other,
                        int32_t size1,
                        int32_t size2 )
{

  try
  {


    if ( ( size1 < 0 ) || ( size2 < 0 ) )
    {

      throw std::runtime_error( "size1 and size2 must be nul or positive" );

    }

    int32_t targetSize1 = ( size1 ? size1 : other.getSize1() );
    int32_t targetSize2 = ( size2 ? size2 : other.getSize2() );

    if ( ( offset1 < 0 ) ||
         ( offset2 < 0 ) ||
         ( offset1 + targetSize1 > getSize1() ) ||
         ( offset2 + targetSize2 > getSize2() ) )
    {

      throw std::runtime_error( "incompatible offset(s)/other matrix size" );

    }
    int32_t s1, s2;
    for ( s1 = 0; s1 < targetSize1; s1++ )
    {

      for ( s2 = 0; s2 < targetSize2; s2++ )
      {

        ( *this )( offset1 + s1, offset2 + s2 ) = other( s1, s2 );

      }

    }

  }
  GKG_CATCH( "void gkg::Matrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "const gkg::Matrix& other, "
             "int32_t size1, "
             "int32_t size2 )" );

}


void gkg::Matrix::setDiagonal( const double& value )
{

  try
  {

    
    _matrixImplementation->setZero();
    int32_t s;
    int32_t maximumS = std::min( getSize1(), getSize2() );
    for ( s = 0; s < maximumS; s++ )
    {

      _matrixImplementation->operator()( s, s ) = value;

    }

  }
  GKG_CATCH( "void gkg::Matrix::setDiagonal( const double& value )" );

}


void gkg::Matrix::transpose()
{

  try
  {

    gkg::Matrix tmp( *this );
    reallocate( tmp.getSize2(), tmp.getSize1() );

    for ( int32_t s1 = 0; s1 < getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < getSize2(); s2++ )
      {

        ( *this )( s1, s2 ) = tmp( s2, s1 );

      }

    }

  }
  GKG_CATCH( "void gkg::Matrix::transpose()" );

}


double gkg::Matrix::getTrace() const
{

  try
  {

    if ( !isSquare() )
    {

      throw std::runtime_error( "not a square matrix" );

    }

    double trace = 0.0;
    int32_t s;
    for ( s = 0; s < getSize1(); s++ )
    {

      trace += ( *this )( s, s );

    }

    return trace;

  }
  GKG_CATCH( "double gkg::Matrix::getTrace() const" );

}


double gkg::Matrix::getDeterminant() const
{

  try
  {

    if ( !isSquare() )
    {

      throw std::runtime_error( "not a square matrix" );

    }

    return _matrixImplementation->getDeterminant();

  }
  GKG_CATCH( "double gkg::Matrix::getDeterminant() const" );

}


gkg::Matrix gkg::Matrix::getTransposition() const
{

  try
  {

    gkg::Matrix matrix( *this );
    matrix.transpose();
    return matrix;

  }
  GKG_CATCH( "gkg::Matrix gkg::Matrix::getTransposition() const" );

}


gkg::Matrix gkg::Matrix::getComposition( const gkg::Matrix& other ) const
{

  try
  {

    if ( getSize2() != other.getSize1() )
    {

      std::runtime_error( "the row count for other matrix is not equal to "
                          "the column count for this matrix" );
    }

    gkg::Matrix composition( getSize1(), other.getSize2() );
 
    for ( int32_t s1 = 0; s1 < composition.getSize1(); s1++ )
    {
 
      for ( int32_t s2 = 0; s2 < composition.getSize2(); s2++ )
      {

        composition( s1, s2 ) = 0.0;
        for ( int32_t k = 0; k < getSize2(); k++ )
        {

          composition( s1, s2 ) += ( *this )( s1, k ) * other( k, s2 );

        }

      }

    }

    return composition;

  }
  GKG_CATCH( "gkg::Matrix "
             "gkg::Matrix::getComposition( const gkg::Matrix& other ) const" );

}


gkg::Vector gkg::Matrix::getComposition( const gkg::Vector& other ) const
{

  try
  {

    if ( getSize2() != other.getSize() )
    {

      std::runtime_error( "the vector size is not equal to "
                          "the column count for this matrix" );
    }

    gkg::Vector composition( getSize1() );

    for ( int32_t s = 0; s < composition.getSize(); s++ )
    {

      composition( s ) = 0.0;
      for ( int32_t k = 0; k < getSize2(); k++ )
      {

        composition( s ) += ( *this )( s, k ) * other( k );

      }

    }
    return composition;

  }
  GKG_CATCH( "gkg::Vector "
             "gkg::Matrix::getComposition( const gkg::Vector& other ) const" );

}


gkg::CMatrix gkg::Matrix::getComposition( const gkg::CMatrix& other ) const
{

  try
  {

    if ( getSize2() != other.getSize1() )
    {

      std::runtime_error( "the row count for other matrix is not equal to "
                          "the column count for this matrix" );
    }

    gkg::CMatrix composition( getSize1(), other.getSize2() );

    for ( int32_t s1 = 0; s1 < composition.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < composition.getSize2(); s2++ )
      {

        composition( s1, s2 ) = 0.0;
        for ( int32_t k = 0; k < getSize2(); k++ )
        {

          composition( s1, s2 ) += ( *this )( s1, k ) * other( k, s2 );

        }

      }

    }
    return composition;

  }
  GKG_CATCH( "gkg::CMatrix "
             "gkg::Matrix::getComposition( const gkg::CMatrix& other ) const" );

}


gkg::CVector gkg::Matrix::getComposition( const gkg::CVector& other ) const
{

  try
  {

    if ( getSize2() != other.getSize() )
    {

      std::runtime_error( "the vector size is not equal to "
                          "the column count for this matrix" );
    }

    gkg::CVector composition( getSize1() );

    for ( int32_t s = 0; s < composition.getSize(); s++ )
    {

      composition( s ) = 0.0;
      for ( int32_t k = 0; k < getSize2(); k++ )
      {

        composition( s ) += ( *this )( s, k ) * other( k );

      }

    }
    return composition;

  }
  GKG_CATCH( "gkg::CVector "
             "gkg::Matrix::getComposition( const gkg::CVector& other ) const" );

}


gkg::Vector gkg::Matrix::toVector() const
{

  try
  {

    gkg::Vector result;
    if ( getSize1() == 1 )
    {

      result.reallocate( getSize2() );
      for ( int32_t s = 0; s < result.getSize(); s++ )
      {

        result( s ) = ( *this )( 0, s );

      }

    }
    else if ( getSize2() == 1 )
    {

      result.reallocate( getSize1() );
      for ( int32_t s = 0; s < result.getSize(); s++ )
      {

        result( s ) = ( *this )( s, 0 );

      }

    }
    else
    {

      std::runtime_error( "none of the size is equalt to 1" );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector gkg::Matrix::toVector() const" );

}


gkg::CVector gkg::Matrix::toCVector() const
{

  try
  {

    gkg::CVector result;
    if ( getSize1() == 1 )
    {

      result.reallocate( getSize2() );
      for ( int32_t s = 0; s < result.getSize(); s++ )
      {

        result( s ) = ( *this )( 0, s );

      }

    }
    else if ( getSize2() == 1 )
    {

      result.reallocate( getSize1() );
      for ( int32_t s = 0; s < result.getSize(); s++ )
      {

        result( s ) = ( *this )( s, 0 );

      }

    }
    else
    {

      std::runtime_error( "none of the size is equalt to 1" );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector gkg::Matrix::toCVector() const" );

}


gkg::Vector gkg::Matrix::getRow( int32_t i ) const
{

  try
  {

    gkg::Vector row( getSize2() );
    for ( int32_t s = 0; s < getSize2(); s++ )
    {

      row( s ) = ( *this )( i, s );

    }
    return row;

  }
  GKG_CATCH( "gkg::Vector gkg::Matrix::getRow( int32_t i ) const" );

}


gkg::Vector gkg::Matrix::getColumn( int32_t j ) const
{

  try
  {

    gkg::Vector column( getSize1() );
    for ( int32_t s = 0; s < getSize1(); s++ )
    {

      column( s ) = ( *this )( s, j );

    }
    return column;

  }
  GKG_CATCH( "gkg::Vector gkg::Matrix::getColumn( int32_t j ) const" );

}


gkg::Vector gkg::Matrix::getDiagonal() const
{

  try
  {

    int32_t size = getSize1();
    if ( getSize2() != size )
    {

      throw std::runtime_error( "not a square matrix" );

    }
    gkg::Vector diagonal( size );
    for ( int32_t s = 0; s < size; s++ )
    {

      diagonal( s ) = ( *this )( s, s );

    }
    return diagonal;


  }
  GKG_CATCH( "gkg::Vector gkg::Matrix::getDiagonal() const" );

}


void gkg::Matrix::getRows( const std::vector< int32_t >& rowIndices,
                           std::vector< gkg::Vector >& rows ) const
{

  try
  {

    if ( rows.size() != rowIndices.size() )
    {

      rows.resize( rowIndices.size() );

    }

    std::vector< int32_t >::const_iterator
      i = rowIndices.begin(),
      ie = rowIndices.end();
    std::vector< gkg::Vector >::iterator r = rows.begin();

    while ( i != ie )
    {

      if ( ( *i < 0 ) || ( ( *i ) >= this->getSize1() ) )
      {

        *r = getRow( *i );

      }
      ++ i;
      ++ r;

    }

  }
  GKG_CATCH( "void "
             "gkg::Matrix::getRows( "
             "const std::vector< int32_t rowIndices, "
             "std::vector< gkg::Vector >& rows )& const" );

}


void gkg::Matrix::getColumns( const std::vector< int32_t >& columnIndices,
                              std::vector< gkg::Vector >& columns ) const
{

  try
  {

    if ( columns.size() != columnIndices.size() )
    {

      columns.resize( columnIndices.size() );

    }

    std::vector< int32_t >::const_iterator
      j = columnIndices.begin(),
      je = columnIndices.end();
    std::vector< gkg::Vector >::iterator c = columns.begin();

    while ( j != je )
    {

      if ( ( *j < 0 ) || ( ( *j ) >= this->getSize2() ) )
      {

        *c = getColumn( *j );

      }
      ++ j;
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::Matrix::getColumns( "
             "const std::vector< int32_t >& columnIndices, "
             "std::vector< gkg::Vector >& columns ) const" );

}


double gkg::Matrix::getSumOfValues() const
{

  try
  {

    double sum = 0;

    for ( int32_t s1 = 0; s1 < this->getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < this->getSize2(); s2++ )
      {

        sum += ( *this )( s1, s2 );

      }

    }

    return sum;

  }
  GKG_CATCH( "double gkg::Matrix::getSumOfValues() const" );

}


gkg::MatrixImplementation* gkg::Matrix::getImplementation() const
{

  return _matrixImplementation;

}


gkg::Matrix 
operator + ( const gkg::Matrix& thing )
{

  try
  {

    return thing;

  }
  GKG_CATCH( "gkg::Matrix  "
             "operator + ( const gkg::Matrix& thing )" );

}


gkg::Matrix 
operator - ( const gkg::Matrix& thing )
{

  try
  {

    gkg::Matrix result( thing.getSize1(), thing.getSize2() );
    for ( int32_t s1 = 0; s1 < thing.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing.getSize2(); s2++ )
      {

        result( s1, s2 ) = -thing( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix "
             "operator - ( const gkg::Matrix& thing )" );

}


gkg::Matrix
operator + ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::Matrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) + thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix "
             "operator + ( const gkg::Matrix& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CMatrix
operator + ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::CMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) + thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator + ( const gkg::Matrix& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::Matrix
operator - ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::Matrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) - thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix "
             "operator - ( const gkg::Matrix& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CMatrix
operator - ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::CMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) - thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator - ( const gkg::Matrix& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::Matrix
operator * ( const gkg::Matrix& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::Matrix "
             "operator * ( const gkg::Matrix& thing1, "
              "const gkg::Matrix& thing2 )" );

}


gkg::Vector
operator * ( const gkg::Matrix& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::Vector "
             "operator * ( const gkg::Matrix& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::Matrix& thing1,
             const gkg::CMatrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator * ( const gkg::Matrix& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::CVector
operator * ( const gkg::Matrix& thing1,
             const gkg::CVector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::Matrix& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::Matrix
operator * ( const gkg::Matrix& thing1,
             const double& thing2 )
{

  try
  {

    gkg::Matrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) * thing2;

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix "
             "operator * ( const gkg::Matrix& thing1, "
             "const double& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::Matrix& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::CMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) * thing2;

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator * ( const gkg::Matrix& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::Matrix
operator / ( const gkg::Matrix& thing1,
             const double& thing2 )
{

  try
  {

    gkg::Matrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) / thing2;

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix "
             "operator * ( const gkg::Matrix& thing1, "
             "const double& thing2 )" );

}


gkg::CMatrix
operator / ( const gkg::Matrix& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::CMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        result( s1, s2 ) = thing1( s1, s2 ) / thing2;

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator / ( const gkg::Matrix& thing1, "
             "const std::complex< double >& thing2 )" );

}


std::ostream& std::operator << ( std::ostream& os, const gkg::Matrix& thing )
{

  for ( int32_t s1 = 0; s1 < thing.getSize1(); s1++ )
  {

    for ( int32_t s2 = 0; s2 < thing.getSize2(); s2++ )
    {

      os << thing( s1, s2 ) << " ";

    }
    if ( s1 != thing.getSize1() - 1 )
    {

      os << std::endl;

    }

  }
  return os;

}


bool std::operator == ( const gkg::Matrix& thing1,
                        const gkg::Matrix& thing2 )
{

  int size1 = thing1.getSize1();
  int size2 = thing1.getSize2();

  if ( ( thing2.getSize1() != size1 ) ||
       ( thing2.getSize2() != size2 ) )
  {

    return false;

  }

  for ( int32_t s1 = 0; s1 < size1; s1++ )
  {

    for ( int32_t s2 = 0; s2 < size2; s2++ )
    {

      if ( thing1( s1, s2 ) != thing2( s1, s2 ) )
      {

        return false;

      }

    }

  }
  return true;

}



