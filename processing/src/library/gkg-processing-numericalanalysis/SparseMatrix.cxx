#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-core-exception/Exception.h>


gkg::SparseMatrix::SparseMatrix( int32_t size1, int32_t size2 )
                  : _matrix( size1, size2, 0 )
{
}


gkg::SparseMatrix::SparseMatrix( const gkg::SparseMatrix& other )
                  : _matrix( other._matrix )
{
}


gkg::SparseMatrix::SparseMatrix( const gkg::Matrix& other )
{

  try
  {

    int32_t s1 = 0;
    int32_t s2 = 0;
    int32_t size1 = other.getSize1();
    int32_t size2 = other.getSize2();

    _matrix.resize( size1, size2 );

    for ( s1 = 0; s1 < size1; s1++ )
    {

      for ( s2 = 0; s2 < size2; s2++ )
      {

        _matrix( ( uint32_t )s1, ( uint32_t )s2 ) = other( s1, s2 );

      }

    }

  }
  GKG_CATCH( "gkg::SparseMatrix::SparseMatrix( const gkg::Matrix& other )" );

}


gkg::SparseMatrix::~SparseMatrix()
{
}



gkg::SparseMatrix& 
gkg::SparseMatrix::operator=( const gkg::SparseMatrix& other )
{

  try
  {

    _matrix = other._matrix;

    return *this;

  }
  GKG_CATCH( "gkg::SparseMatrix& "
             "gkg::SparseMatrix::operator=( "
             "const gkg::SparseMatrix& other )" );

}


gkg::SparseMatrix& 
gkg::SparseMatrix::operator=( const gkg::Matrix& other )
{

  try
  {

    int32_t size1 = other.getSize1();
    int32_t size2 = other.getSize2();

    _matrix.resize( size1, size2 );

    int32_t s1 = 0;
    int32_t s2 = 0;
    for ( s1 = 0; s1 < size1; s1++ )
    {

      for ( s2 = 0; s2 < size2; s2++ )
      {

        _matrix( ( uint32_t )s1, ( uint32_t )s2 ) = other( s1, s2 );

      }

    }

    return *this;

  }
  GKG_CATCH( "gkg::SparseMatrix& "
             "gkg::SparseMatrix::operator=( "
             "const gkg::Matrix& other )" );

}


void gkg::SparseMatrix::reallocate( int32_t size1, int32_t size2 )
{

  try
  {

    // do not preservce because not implemented in boost !
    _matrix.resize( size1, size2, false );

  }
  GKG_CATCH( "void gkg::SparseMatrix::reallocate( "
             "int32_t size1, int32_t size2 )" );

}


gkg::SparseMatrix::iterator1 gkg::SparseMatrix::begin1()
{

  try
  {

    return _matrix.begin1();

  }
  GKG_CATCH( "gkg::SparseMatrix::iterator1 gkg::SparseMatrix::begin1()" );

}


gkg::SparseMatrix::iterator1 gkg::SparseMatrix::end1()
{

  try
  {

    return _matrix.end1();

  }
  GKG_CATCH( "gkg::SparseMatrix::iterator1 gkg::SparseMatrix::end1()" );

}


gkg::SparseMatrix::const_iterator1 gkg::SparseMatrix::begin1() const
{

  try
  {

    return _matrix.begin1();

  }
  GKG_CATCH( "gkg::SparseMatrix::iterator1 "
             "gkg::SparseMatrix::begin1() const" );

}


gkg::SparseMatrix::const_iterator1 gkg::SparseMatrix::end1() const
{

  try
  {

    return _matrix.end1();

  }
  GKG_CATCH( "gkg::SparseMatrix::const_iterator1 "
             "gkg::SparseMatrix::end1() const" );

}


int32_t gkg::SparseMatrix::getSize1() const
{

  try
  {

    return ( int32_t )_matrix.size1();

  }
  GKG_CATCH( "int32_t gkg::SparseMatrix::getSize1() const" );

}


int32_t gkg::SparseMatrix::getSize2() const
{

  try
  {

    return ( int32_t )_matrix.size2();

  }
  GKG_CATCH( "int32_t gkg::SparseMatrix::getSize2() const" );

}


int32_t gkg::SparseMatrix::getNonZeroElementCount() const
{

  try
  {

    return ( int32_t )_matrix.nnz();

  }
  GKG_CATCH( "int32_t gkg::SparseMatrix::getNonZeroElementCount() const" );

}


int32_t gkg::SparseMatrix::getNonZeroElementCountForLine( int32_t i ) const
{

  try
  {

    int32_t size1 = getSize1();

    if ( ( i < 0 ) || ( i >= size1 ) )
    {

      throw std::runtime_error( "invalid line index" );

    }
    int32_t size2 = getSize2();

    int32_t j = 0;
    int32_t count = 0;
    for ( j = 0; j < size2; j++ )
    {

      if ( hasElement( i, j ) )
      {

        ++ count;

      }

    }
    return count;

  }
  GKG_CATCH( "int32_t gkg::SparseMatrix::getNonZeroElementCountForLine( "
             "int32_t i ) const" );

}


int32_t gkg::SparseMatrix::getNonZeroElementCountForColumn( int32_t j ) const
{

  try
  {

    int32_t size2 = getSize2();

    if ( ( j < 0 ) || ( j >= size2 ) )
    {

      throw std::runtime_error( "invalid column index" );

    }
    int32_t size1 = getSize1();

    int32_t i = 0;
    int32_t count = 0;
    for ( i = 0; i < size1; i++ )
    {

      if ( hasElement( i, j ) )
      {

        ++ count;

      }

    }
    return count;

  }
  GKG_CATCH( "int32_t gkg::SparseMatrix::getNonZeroElementCountForColumn( "
             "int32_t j ) const" );

}


bool gkg::SparseMatrix::hasElement( int32_t i, int32_t j ) const
{

  try
  {

    return _matrix.find_element( ( uint32_t )i, ( uint32_t )j );

  }
  GKG_CATCH( "bool gkg::SparseMatrix::hasElement( "
             "int32_t i, int32_t j ) const" );


}


gkg::SparseMatrix::const_reference 
gkg::SparseMatrix::operator()( int32_t i, int32_t j ) const
{

  try
  {

    return _matrix( ( uint32_t )i, ( uint32_t )j );

  }
  GKG_CATCH( "const double& gkg::SparseMatrix::operator()( "
             "int32_t i, int32_t j ) const" );

}


gkg::SparseMatrix::reference 
gkg::SparseMatrix::operator()( int32_t i, int32_t j )
{

  try
  {

    return _matrix( ( uint32_t )i, ( uint32_t )j );

  }
  GKG_CATCH( "double& gkg::SparseMatrix::operator()( "
             "int32_t i, int32_t j )" );

}


void gkg::SparseMatrix::eraseElement( int32_t i, int32_t j )
{

  try
  {

    _matrix.erase_element( ( uint32_t )i, ( uint32_t )j );

  }
  GKG_CATCH( "void gkg::SparseMatrix::eraseElement( int32_t i, int32_t j )" );

}


bool gkg::SparseMatrix::isSquare() const
{

  try
  {

    return ( _matrix.size1() == _matrix.size2() );

  }
  GKG_CATCH( "bool gkg::SparseMatrix::isSquare() const" );

}


void gkg::SparseMatrix::setZero()
{

  try
  {

    std::map< size_t, double >::iterator
      s = _matrix.data().begin(),
      se = _matrix.data().end();
    while ( s != se )
    {

      s->second = 0.0;
      ++ s;

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::setZero()" );

}


void gkg::SparseMatrix::setIdentity()
{

  try
  {

    if ( !isSquare() )
    {

      throw std::runtime_error( "not a square matrix" );

    }

    _matrix.clear();

    int32_t size = getSize1();

    int32_t s = 0;
    for ( s = 0; s < size; s++ )
    {

      _matrix( ( uint32_t )s, ( uint32_t )s ) = 1.0;

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::setIdentity()" );

}


void gkg::SparseMatrix::setRow( int32_t s1, const gkg::Vector& row )
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
  GKG_CATCH( "void gkg::SparseMatrix::setRow( int32_t s1, "
             "const gkg::Vector& row )" );

}


void gkg::SparseMatrix::setColumn( int32_t s2, const gkg::Vector& column )
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
  GKG_CATCH( "void gkg::SparseMatrix::setColumn( "
             "int32_t s2, const gkg::Vector& column )" );

}


void gkg::SparseMatrix::fill( const double& value )
{

  try
  {

    int32_t size1 = getSize1();
    int32_t size2 = getSize2();

    int32_t s1 = 0;
    int32_t s2 = 0;
    for ( s1 = 0; s1 < size1; s1++ )
    {

      for ( s2 = 0; s2 < size2; s2++ )
      {

        _matrix( ( uint32_t )s1, ( uint32_t )s2 ) = value;

      }

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::fill( const double& value )" );

}


void gkg::SparseMatrix::fill( int32_t offset1,
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
  GKG_CATCH( "void gkg::SparseMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "int32_t size1, "
             "int32_t size2, "
             "const double& value )" );

}


void gkg::SparseMatrix::fill( int32_t offset1,
                              int32_t offset2,
                              const gkg::SparseMatrix& other,
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

        if ( hasElement( s1, s2 ) )
        {

          ( *this )( offset1 + s1, offset2 + s2 ) = other( s1, s2 );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "const gkg::SparseMatrix& other, "
             "int32_t size1, "
             "int32_t size2 )" );

}


void gkg::SparseMatrix::fill( int32_t offset1,
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
  GKG_CATCH( "void gkg::SparseMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "const gkg::Matrix& other, "
             "int32_t size1, "
             "int32_t size2 )" );

}


void gkg::SparseMatrix::setDiagonal( const double& value )
{

  try
  {

    _matrix.clear();
    int32_t s;
    int32_t maximumS = std::min( getSize1(), getSize2() );
    for ( s = 0; s < maximumS; s++ )
    {

      _matrix( ( uint32_t )s, ( uint32_t )s ) = value;

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::setDiagonal( const double& value )" );

}


void gkg::SparseMatrix::transpose()
{

  try
  {

    gkg::SparseMatrix tmp( *this );
    reallocate( tmp.getSize2(), tmp.getSize1() );

    for ( int32_t s1 = 0; s1 < getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < getSize2(); s2++ )
      {

        if ( tmp.hasElement( s2, s1 ) )
        {

          ( *this )( s1, s2 ) = tmp( s2, s1 );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::SparseMatrix::transpose()" );

}


double gkg::SparseMatrix::getTrace() const
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

      if ( hasElement( s, s ) )
      {

        trace += ( *this )( s, s );

      }

    }

    return trace;

  }
  GKG_CATCH( "double gkg::SparseMatrix::getTrace() const" );

}


gkg::SparseMatrix gkg::SparseMatrix::getTransposition() const
{

  try
  {

    gkg::SparseMatrix matrix( *this );
    matrix.transpose();
    return matrix;

  }
  GKG_CATCH( "gkg::SparseMatrix gkg::SparseMatrix::getTransposition() const" );

}


gkg::SparseMatrix 
gkg::SparseMatrix::getComposition( const gkg::SparseMatrix& other ) const
{

  try
  {

    if ( getSize2() != other.getSize1() )
    {

      std::runtime_error( "the row count for other matrix is not equal to "
                          "the column count for this matrix" );
    }

    gkg::SparseMatrix composition( getSize1(), other.getSize2() );

    for ( int32_t s1 = 0; s1 < composition.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < composition.getSize2(); s2++ )
      {

        double value = 0.0;
        bool nonZero = false;
        for ( int32_t k = 0; k < getSize2(); k++ )
        {

          if ( this->hasElement( s1, k ) && other.hasElement( k, s2 ) )
          {

            value += ( *this )( s1, k ) * other( k, s2 );
            nonZero = true;

          }

        }

        if ( nonZero )
        {

          composition( s1, s2 ) = value;

        }

      }

    }
    return composition;

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "gkg::SparseMatrix::getComposition( "
             "const gkg::SparseMatrix& other ) const" );

}


gkg::Vector 
gkg::SparseMatrix::getComposition( const gkg::Vector& other ) const
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
             "gkg::SparseMatrix::getComposition( "
             "const gkg::Vector& other ) const" );

}


gkg::SparseCMatrix 
gkg::SparseMatrix::getComposition( const gkg::SparseCMatrix& other ) const
{

  try
  {

    if ( getSize2() != other.getSize1() )
    {

      std::runtime_error( "the row count for other matrix is not equal to "
                          "the column count for this matrix" );
    }

    gkg::SparseCMatrix composition( getSize1(), other.getSize2() );

    for ( int32_t s1 = 0; s1 < composition.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < composition.getSize2(); s2++ )
      {

        std::complex< double > value = 0.0;
        bool nonZero = false;
        for ( int32_t k = 0; k < getSize2(); k++ )
        {

          if ( this->hasElement( s1, k ) && other.hasElement( k, s2 ) )
          {

            value += ( *this )( s1, k ) * other( k, s2 );
            nonZero = true;

          }

        }

        if ( nonZero )
        {

          composition( s1, s2 ) = value;

        }

      }

    }
    return composition;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "gkg::SparseMatrix::getComposition( "
             "const gkg::SparseCMatrix& other ) const" );

}


gkg::CVector 
gkg::SparseMatrix::getComposition( const gkg::CVector& other ) const
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
             "gkg::SparseMatrix::getComposition( "
             "const gkg::CVector& other ) const" );

}


gkg::Vector gkg::SparseMatrix::toVector() const
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

      std::runtime_error( "none of the size is equal to 1" );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector gkg::SparseMatrix::toVector() const" );

}


gkg::CVector gkg::SparseMatrix::toCVector() const
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

      std::runtime_error( "none of the size is equal to 1" );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector gkg::SparseMatrix::toCVector() const" );

}


gkg::Matrix gkg::SparseMatrix::toMatrix() const
{

  try
  {

    gkg::Matrix matrix( getSize1(), getSize2() );
    int32_t s1 = 0;
    int32_t s2 = 0;

    for ( s1 = 0; s1 < getSize1(); s1++ )
    {

      for ( s2 = 0; s2 < getSize2(); s2++ )
      {

        matrix( s1, s2 ) = _matrix( ( uint32_t )s1, ( uint32_t )s2 );

      }

    }

    return matrix;

  }
  GKG_CATCH( "gkg::Matrix gkg::SparseMatrix::toMatrix() const" );

}



gkg::Vector gkg::SparseMatrix::getRow( int32_t i ) const
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
  GKG_CATCH( "gkg::Vector gkg::SparseMatrix::getRow( int32_t i ) const" );

}


gkg::Vector gkg::SparseMatrix::getColumn( int32_t j ) const
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
  GKG_CATCH( "gkg::Vector gkg::SparseMatrix::getColumn( int32_t j ) const" );

}


std::map< size_t, double >& gkg::SparseMatrix::getData()
{

  try
  {

    return _matrix.data();

  }
  GKG_CATCH( "std::map< size_t, double >& "
             "gkg::SparseMatrix::getData()" );

}


const std::map< size_t, double >& gkg::SparseMatrix::getData() const
{

  try
  {

    return _matrix.data();

  }
  GKG_CATCH( "const std::map< size_t, double >& "
             "gkg::SparseMatrix::getData() const" );

}


//
// unary operators + & -
//

// + SparseMatrix
gkg::SparseMatrix 
operator + ( const gkg::SparseMatrix& thing )
{

  try
  {

    return thing;

  }
  GKG_CATCH( "gkg::SparseMatrix  "
             "operator + ( const gkg::SparseMatrix& thing )" );

}

// - SparseMatrix
gkg::SparseMatrix 
operator - ( const gkg::SparseMatrix& thing )
{

  try
  {

    gkg::SparseMatrix result( thing.getSize1(), thing.getSize2() );
    for ( int32_t s1 = 0; s1 < thing.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing.getSize2(); s2++ )
      {

        if ( thing.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = -thing( s1, s2 );

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "operator - ( const gkg::SparseMatrix& thing )" );

}


//
// binary operator +
//

// SparseMatrix + SparseMatrix
gkg::SparseMatrix
operator + ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::SparseMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) ||
             thing2.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) + thing2( s1, s2 );

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "operator + ( const gkg::SparseMatrix& thing1, "
             "const gkg::SparseMatrix& thing2 )" );

}

// SparseMatrix + SparseCMatrix
gkg::SparseCMatrix
operator + ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::SparseCMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) ||
             thing2.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) + thing2( s1, s2 );

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator + ( const gkg::SparseMatrix& thing1, "
             "const gkg::SparseCMatrix& thing2 )" );

}


//
// binary operator -
//

// SparseMatrix - SparseMatrix
gkg::SparseMatrix
operator - ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::SparseMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) ||
             thing2.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) - thing2( s1, s2 );

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "operator - ( const gkg::SparseMatrix& thing1, "
             "const gkg::SparseMatrix& thing2 )" );

}

// SparseMatrix - SparseCMatrix
gkg::SparseCMatrix
operator - ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 )
{

  try
  {

    if ( ( thing1.getSize1() != thing2.getSize1() ) ||
         ( thing1.getSize2() != thing2.getSize2() ) )
    {

      throw std::runtime_error( "incompatible matrix size(s)" );

    }

    gkg::SparseCMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < result.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < result.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) ||
             thing2.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) - thing2( s1, s2 );

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator - ( const gkg::SparseMatrix& thing1, "
             "const gkg::SparseCMatrix& thing2 )" );

}


//
// binary operator *
//

// SparseMatrix * SparseMatrix
gkg::SparseMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "operator * ( const gkg::SparseMatrix& thing1, "
              "const gkg::SparseMatrix& thing2 )" );

}

// SparseMatrix * SparseCMatrix
gkg::SparseCMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator * ( const gkg::SparseMatrix& thing1, "
              "const gkg::SparseCMatrix& thing2 )" );

}

// SparseMatrix * Vector
gkg::Vector
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::Vector "
             "operator * ( const gkg::SparseMatrix& thing1, "
              "const gkg::Vector& thing2 )" );

}

// SparseMatrix * CVector
gkg::CVector
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::CVector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::SparseMatrix& thing1, "
              "const gkg::CVector& thing2 )" );

}

// SparseMatrix * double
gkg::SparseMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const double& thing2 )
{

  try
  {

    gkg::SparseMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) * thing2;

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseMatrix "
             "operator * ( const gkg::SparseMatrix& thing1, "
             "const double& thing2 )" );

}

// SparseMatrix * complex< double >
gkg::SparseCMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::SparseCMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) * thing2;

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator * ( const gkg::SparseMatrix& thing1, "
             "const std::complex< double >& thing2 )" );

}

//
// binary operator /
//

// SparseMatrix / double
gkg::SparseMatrix
operator / ( const gkg::SparseMatrix& thing1,
             const double& thing2 )
{

  try
  {

    gkg::SparseMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) / thing2;

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator / ( const gkg::SparseMatrix& thing1, "
             "const double& thing2 )" );

}

// SparseMatrix / complex< double >
gkg::SparseCMatrix
operator / ( const gkg::SparseMatrix& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::SparseCMatrix result( thing1.getSize1(), thing1.getSize2() );
    for ( int32_t s1 = 0; s1 < thing1.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing1.getSize2(); s2++ )
      {

        if ( thing1.hasElement( s1, s2 ) )
        {

          result( s1, s2 ) = thing1( s1, s2 ) / thing2;

        }

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::SparseCMatrix "
             "operator / ( const gkg::SparseMatrix& thing1, "
             "const std::complex< double >& thing2 )" );

}


//
// standard operator(s)
//

std::ostream& std::operator << ( std::ostream& os, 
                                 const gkg::SparseMatrix& thing )
{

  for ( int32_t s1 = 0; s1 < thing.getSize1(); s1++ )
  {

    for ( int32_t s2 = 0; s2 < thing.getSize2(); s2++ )
    {

      if ( thing.hasElement( s1, s2 ) )
      {

        os << thing( s1, s2 ) << " ";

      }
      else
      {

        os << "- ";

      }

    }
    if ( s1 != thing.getSize1() - 1 )
    {

      os << std::endl;

    }

  }
  return os;

}


bool std::operator == ( const gkg::SparseMatrix& thing1,
                        const gkg::SparseMatrix& thing2 )
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