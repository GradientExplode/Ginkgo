#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/CMatrixImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::CMatrix::CMatrix( int32_t size1, int32_t size2 )
{

  try
  {

    _cmatrixImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                              createCMatrixImplementation( this, size1, size2 );

  }
  GKG_CATCH( "gkg::CMatrix::CMatrix( int size1, int size2 )" );

}


gkg::CMatrix::CMatrix( const gkg::CMatrix& other )
{

  try
  {

    _cmatrixImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createCMatrixImplementation( this,
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
  GKG_CATCH( "gkg::CMatrix::CMatrix( const gkg::CMatrix& other )" );

}


gkg::CMatrix::CMatrix( const gkg::Matrix& other )
{

  try
  {

    _cmatrixImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createCMatrixImplementation( this,
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
  GKG_CATCH( "gkg::CMatrix::CMatrix( const gkg::Matrix& other )" );

}


gkg::CMatrix::~CMatrix()
{

  delete _cmatrixImplementation;

}


gkg::CMatrix& gkg::CMatrix::operator=( const gkg::CMatrix& other )
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
  GKG_CATCH( "gkg::CMatrix& "
             "gkg::CMatrix::operator=( const gkg::CMatrix& other )" );

}


void gkg::CMatrix::reallocate( int32_t size1, int32_t size2 )
{

  try
  {

    if ( ( size1 != getSize1() ) || ( size2 != getSize2() ) )
    {
         
      delete _cmatrixImplementation;
      _cmatrixImplementation =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                               createCMatrixImplementation( this,
                                                            size1,
                                                            size2 );

    }

  }
  GKG_CATCH( "void gkg::CMatrix::reallocate( int size1, int size2 )" );

}


int32_t gkg::CMatrix::getSize1() const
{

  try
  {

    return _cmatrixImplementation->getSize1();

  }
  GKG_CATCH( "int gkg::CMatrix::getSize1() const" );

}


int32_t gkg::CMatrix::getSize2() const
{

  try
  {

    return _cmatrixImplementation->getSize2();

  }
  GKG_CATCH( "int gkg::CMatrix::getSize2() const" );

}


const std::complex< double >& 
gkg::CMatrix::operator()( int32_t i, int32_t j ) const
{

  try
  {

    return _cmatrixImplementation->operator()( i, j );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::CMatrix::operator()( int i, int j ) const" );

}


std::complex< double >& gkg::CMatrix::operator()( int32_t i, int32_t j )
{

  try
  {

    return _cmatrixImplementation->operator()( i, j );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::CMatrix::operator()( int i, int j )" );

} 


bool gkg::CMatrix::isSquare() const
{

  try
  {

    return ( _cmatrixImplementation->getSize1() == 
             _cmatrixImplementation->getSize2() );

  }
  GKG_CATCH( "bool gkg::CMatrix::isSquare() const" );

}


void gkg::CMatrix::setZero()
{

  try
  {

    _cmatrixImplementation->setZero();

  }
  GKG_CATCH( "void gkg::CMatrix::setZero()" );

}


void gkg::CMatrix::setIdentity()
{

  try
  {

    _cmatrixImplementation->setIdentity();

  }
  GKG_CATCH( "void gkg::CMatrix::setIdentity()" );

}


void gkg::CMatrix::setRow( int32_t s1, const CVector& row )
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
  GKG_CATCH( "void gkg::CMatrix::setRow( int32_t s1, const CVector& row )" );

}


void gkg::CMatrix::setColumn( int32_t s2, const CVector& column )
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
  GKG_CATCH( "void gkg::CMatrix::setColumn( "
             "int32_t s2, const CVector& column )" );

}


void gkg::CMatrix::fill( const std::complex< double >& value )
{

  try
  {

    _cmatrixImplementation->fill( value );

  }
  GKG_CATCH( "void gkg::CMatrix::fill( const std::complex< double >& value )" );

}


void gkg::CMatrix::fill( int32_t offset1,
                         int32_t offset2,
                         int32_t size1,
                         int32_t size2,
                         const std::complex< double >& value )
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
  GKG_CATCH( "void gkg::CMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "int32_t size1, "
             "int32_t size2, "
             "const std::complex< double >& value )" );

}


void gkg::CMatrix::fill( int32_t offset1,
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
  GKG_CATCH( "void gkg::CMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "const gkg::Matrix& other, "
             "int32_t size1, "
             "int32_t size2 )" );

}


void gkg::CMatrix::fill( int32_t offset1,
                         int32_t offset2,
                         const gkg::CMatrix& other,
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
  GKG_CATCH( "void gkg::CMatrix::fill( "
             "int32_t offset1, "
             "int32_t offset2, "
             "const gkg::CMatrix& other, "
             "int32_t size1, "
             "int32_t size2 )" );

}


void gkg::CMatrix::transpose()
{

  try
  {

    gkg::CMatrix tmp( *this );
    reallocate( tmp.getSize2(), tmp.getSize1() );

    for ( int32_t s1 = 0; s1 < getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < getSize2(); s2++ )
      {

        ( *this )( s1, s2 ) = tmp( s2, s1 );

      }

    }

  }
  GKG_CATCH( "void gkg::CMatrix::transpose()" );

}


void gkg::CMatrix::conjugate()
{

  try
  {

    for ( int32_t s2 = 0; s2 < getSize2(); s2++ )
    {

      for ( int32_t s1 = 0; s1 < getSize1(); s1++ )
      {

        ( *this )( s1, s2 ) = std::conj( ( *this )( s1, s2 ) );

      }

    }

  }
  GKG_CATCH( "void gkg::CMatrix::conjugate()" );

}


void gkg::CMatrix::hermitianTranspose()
{

  try
  {

    gkg::CMatrix tmp( *this );
    reallocate( tmp.getSize2(), tmp.getSize1() );

    for ( int32_t s1 = 0; s1 < getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < getSize2(); s2++ )
      {

        ( *this )( s1, s2 ) = std::conj( tmp( s2, s1 ) );

      }

    }

  }
  GKG_CATCH( "void gkg::CMatrix::hermitianTranspose()" );

}


std::complex< double > gkg::CMatrix::getTrace() const
{

  try
  {

    if ( !isSquare() )
    {

      throw std::runtime_error( "not a square matrix" );

    }

    std::complex< double > trace = 0.0;
    int32_t s;
    for ( s = 0; s < getSize1(); s++ )
    {

      trace += ( *this )( s, s );

    }

    return trace;

  }
  GKG_CATCH( "std::complex< double > gkg::CMatrix::getTrace() const" );

}


gkg::CMatrix gkg::CMatrix::getTransposition() const
{

  try
  {

    gkg::CMatrix cmatrix( *this );
    cmatrix.transpose();
    return cmatrix;

  }
  GKG_CATCH( "gkg::CMatrix gkg::CMatrix::getTransposition() const" );

}


gkg::CMatrix gkg::CMatrix::getConjugate() const
{

  try
  {

    gkg::CMatrix cmatrix( *this );
    cmatrix.conjugate();
    return cmatrix;

  }
  GKG_CATCH( "gkg::CMatrix gkg::CMatrix::getConjugate() const" );

}


gkg::CMatrix gkg::CMatrix::getHermitianTransposition() const
{

  try
  {

    gkg::CMatrix cmatrix( *this );
    cmatrix.hermitianTranspose();
    return cmatrix;

  }
  GKG_CATCH( "gkg::CMatrix gkg::CMatrix::getHermitianTransposition() const" );

}


gkg::CMatrix gkg::CMatrix::getComposition( const gkg::CMatrix& other ) const
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
             "gkg::CMatrix::getComposition( const gkg::CMatrix& other ) "
             "const" );

}


gkg::CVector gkg::CMatrix::getComposition( const gkg::CVector& other ) const
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
             "gkg::CMatrix::getComposition( const gkg::CVector& other ) "
             "const" );

}


gkg::CMatrix gkg::CMatrix::getComposition( const gkg::Matrix& other ) const
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
             "gkg::CMatrix::getComposition( const gkg::Matrix& other ) "
             "const" );

}


gkg::CVector gkg::CMatrix::getComposition( const gkg::Vector& other ) const
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
             "gkg::CMatrix::getComposition( const gkg::Vector& other ) "
             "const" );

}


gkg::Matrix gkg::CMatrix::getMagnitude() const
{

  try
  {

    gkg::Matrix magnitude( getSize1(), getSize2() );

    int32_t i1, i2;
    for ( i2 = 0; i2 < getSize2(); i2++ )
    {

      for ( i1 = 0; i1 < getSize1(); i1++ )
      {

        magnitude( i1, i2 ) = std::abs( ( *this )( i1, i2 ) );

      }

    }
    return magnitude;

  }
  GKG_CATCH( "gkg::Matrix gkg::CMatrix::getMagnitude() const" );

}


gkg::CVector gkg::CMatrix::toCVector() const
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
  GKG_CATCH( "gkg::CVector gkg::CMatrix::toCVector() const" );

}


gkg::CVector gkg::CMatrix::getRow( int32_t i ) const
{

  try
  {

    gkg::CVector row( getSize2() );
    for ( int32_t s = 0; s < getSize2(); s++ )
    {

      row( s ) = ( *this )( i, s );

    }
    return row;

  }
  GKG_CATCH( "gkg::CVector gkg::CMatrix::getRow( int32_t i ) const" );

}


gkg::CVector gkg::CMatrix::getColumn( int32_t j ) const
{

  try
  {

    gkg::CVector column( getSize1() );
    for ( int32_t s = 0; s < getSize1(); s++ )
    {

      column( s ) = ( *this )( s, j );

    }
    return column;

  }
  GKG_CATCH( "gkg::CVector gkg::CMatrix::getColumn( int32_t j ) const" );

}


gkg::CVector gkg::CMatrix::getDiagonal() const
{

  try
  {

    int32_t size = getSize1();
    if ( getSize2() != size )
    {

      throw std::runtime_error( "not a square matrix" );

    }
    gkg::CVector diagonal( size );
    for ( int32_t s = 0; s < size; s++ )
    {

      diagonal( s ) = ( *this )( s, s );

    }
    return diagonal;


  }
  GKG_CATCH( "gkg::CVector gkg::CMatrix::getDiagonal() const" );

}


gkg::CMatrixImplementation* gkg::CMatrix::getImplementation() const
{

  return _cmatrixImplementation;

}


void gkg::CMatrix::getRows( const std::vector< int32_t >& rowIndices,
                            std::vector< gkg::CVector >& rows ) const
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
    std::vector< gkg::CVector >::iterator r = rows.begin();

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
             "gkg::CMatrix::getRows( "
             "const std::vector< int32_t rowIndices, "
             "std::vector< gkg::CVector >& rows )& const" );

}


void gkg::CMatrix::getColumns( const std::vector< int32_t >& columnIndices,
                               std::vector< gkg::CVector >& columns ) const
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
    std::vector< gkg::CVector >::iterator c = columns.begin();

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
  GKG_CATCH( "void gkg::CMatrix::getColumns( "
             "const std::vector< int32_t >& columnIndices, "
             "std::vector< gkg::CVector >& columns ) const" );

}


gkg::CMatrix 
operator + ( const gkg::CMatrix& thing )
{

  try
  {

    return thing;

  }
  GKG_CATCH( "gkg::CMatrix  "
             "operator + ( const gkg::CMatrix& thing )" );

}


gkg::CMatrix 
operator - ( const gkg::CMatrix& thing )
{

  try
  {

    gkg::CMatrix result( thing.getSize1(), thing.getSize2() );
    for ( int32_t s1 = 0; s1 < thing.getSize1(); s1++ )
    {

      for ( int32_t s2 = 0; s2 < thing.getSize2(); s2++ )
      {

        result( s1, s2 ) = -thing( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator - ( const gkg::CMatrix& thing )" );

}


gkg::CMatrix
operator + ( const gkg::CMatrix& thing1,
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
             "operator + ( const gkg::CMatrix& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::CMatrix
operator + ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 )
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
             "operator + ( const gkg::CMatrix& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CMatrix
operator - ( const gkg::CMatrix& thing1,
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
             "operator - ( const gkg::CMatrix& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::CMatrix
operator - ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 )
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
             "operator - ( const gkg::CMatrix& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const gkg::CMatrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator * ( const gkg::CMatrix& thing1, "
              "const gkg::CMatrix& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CMatrix& thing1,
             const gkg::CVector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CMatrix& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CMatrix "
             "operator * ( const gkg::CMatrix& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CMatrix& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    return thing1.getComposition( thing2 );

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CMatrix& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
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
             "operator * ( const gkg::CMatrix& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::CMatrix
operator * ( const gkg::CMatrix& thing1,
             const double& thing2 )
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
             "operator * ( const gkg::CMatrix& thing1, "
             "const double& thing2 )" );

}


gkg::CMatrix
operator / ( const gkg::CMatrix& thing1,
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
             "operator * ( const gkg::CMatrix& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::CMatrix
operator / ( const gkg::CMatrix& thing1,
             const double& thing2 )
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
             "operator / ( const gkg::CMatrix& thing1, "
             "const double& thing2 )" );

}


std::ostream& std::operator << ( std::ostream& os, const gkg::CMatrix& thing )
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


