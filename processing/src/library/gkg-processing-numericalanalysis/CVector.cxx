#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/CVectorImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::CVector::CVector( int32_t size )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                      createCVectorImplementation( this, size );

  }
  GKG_CATCH( "gkg::CVector::CVector( int size )" );

}


gkg::CVector::CVector( const gkg::CVector& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                              other.getSize() );

    for ( int32_t s = 0; s < other.getSize(); s++ )
    {

      ( *this )( s ) = other( s );

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( const gkg::CVector& other )" );

}


gkg::CVector::CVector( const gkg::Vector& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                              other.getSize() );

    for ( int32_t s = 0; s < other.getSize(); s++ )
    {

      ( *this )( s ) = other( s );

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( const gkg::Vector& other )" );

}


gkg::CVector::CVector( const std::vector< int32_t >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( const std::vector< int32_t >& other )" );

}


gkg::CVector::CVector( const std::vector< float >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( const std::vector< float >& other )" );

}


gkg::CVector::CVector( const std::vector< double >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( const std::vector< double >& other )" );

}


gkg::CVector::CVector( const std::vector< std::complex< float > >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = other[ s ];

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( "
             "const std::vector< std::complex< float > >& other )" );

}


gkg::CVector::CVector( const std::vector< std::complex< double > >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                 createCVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = other[ s ];

    }

  }
  GKG_CATCH( "gkg::CVector::CVector( "
             "const std::vector< std::complex< double > >& other )" );

}


gkg::CVector::CVector( const gkg::Vector3d< float >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createCVectorImplementation( this, 3 );

    ( *this )( 0 ) = ( double )other.x;
    ( *this )( 1 ) = ( double )other.y;
    ( *this )( 2 ) = ( double )other.z;

  }
  GKG_CATCH( "gkg::CVector::CVector( const gkg::Vector3d< float >& other )" );

}


gkg::CVector::CVector( const gkg::Vector3d< double >& other )
{

  try
  {

    _cvectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createCVectorImplementation( this, 3 );

    ( *this )( 0 ) = other.x;
    ( *this )( 1 ) = other.y;
    ( *this )( 2 ) = other.z;

  }
  GKG_CATCH( "gkg::CVector::CVector( const gkg::Vector3d< double >& other )" );

}


gkg::CVector::~CVector()
{

  delete _cvectorImplementation;

}


gkg::CVector& gkg::CVector::operator=( const gkg::CVector& other )
{

  try
  {

    reallocate( other.getSize() );

    for ( int32_t s = 0; s < other.getSize(); s++ )
    {

      ( *this )( s ) = other( s );

    }
    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const gkg::CVector& other )" );

}


gkg::CVector& gkg::CVector::operator=( const std::vector< int32_t >& other )
{

  try
  {

    reallocate( ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }
    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const std::vector< int32_t >& other )" );

}


gkg::CVector& gkg::CVector::operator=( const std::vector< float >& other )
{

  try
  {

    reallocate( ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }
    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const std::vector< float >& other )" );

}


gkg::CVector& gkg::CVector::operator=( const std::vector< double >& other )
{

  try
  {

    reallocate( ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = other[ s ];

    }
    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const std::vector< double >& other )" );

}


gkg::CVector& gkg::CVector::operator=( const gkg::Vector3d< float >& other )
{

  try
  {

    reallocate( 3 );

    ( *this )( 0 ) = ( double )other.x;
    ( *this )( 1 ) = ( double )other.y;
    ( *this )( 2 ) = ( double )other.z;

    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const gkg::Vector3d< float >& other )" );

}


gkg::CVector& gkg::CVector::operator=( const gkg::Vector3d< double >& other )
{

  try
  {

    reallocate( 3 );

    ( *this )( 0 ) = other.x;
    ( *this )( 1 ) = other.y;
    ( *this )( 2 ) = other.z;

    return *this;

  }
  GKG_CATCH( "gkg::CVector& "
             "gkg::CVector::operator=( const gkg::Vector3d< double >& other )" );

}


void gkg::CVector::reallocate( int32_t size )
{

  try
  {

    if ( size != getSize() )
    {
         
      delete _cvectorImplementation;
      _cvectorImplementation =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                            createCVectorImplementation( this,
                                                                         size );

    }

  }
  GKG_CATCH( "void gkg::Vector::reallocate( int size )" );

}


int32_t gkg::CVector::getSize() const
{

  try
  {

    return _cvectorImplementation->getSize();

  }
  GKG_CATCH( "int gkg::CVector::getSize() const" );

}


const std::complex< double >& gkg::CVector::operator()( int32_t i ) const
{

  try
  {

    return _cvectorImplementation->operator()( i );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::CVector::operator()( int i ) const" );

}


std::complex< double >& gkg::CVector::operator()( int32_t i )
{

  try
  {

    return _cvectorImplementation->operator()( i );

  }
  GKG_CATCH( "const std::complex< double >& "
             "gkg::CVector::operator()( int i )" );

} 


void gkg::CVector::setZero()
{

  try
  {

    _cvectorImplementation->setZero();

  }
  GKG_CATCH( "void gkg::CVector::setZero()" );

}


void gkg::CVector::setBasis( int32_t i )
{

  try
  {

    _cvectorImplementation->setBasis( i );

  }
  GKG_CATCH( "void gkg::CVector::setBasis( int i )" );

}


void gkg::CVector::fill( const std::complex< double >& value )
{

  try
  {

    _cvectorImplementation->fill( value );

  }
  GKG_CATCH( "void gkg::CVector::fill( const std::complex< double >& value )" );

}


void gkg::CVector::conjugate()
{

  try
  {

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      ( *this )( s ) = std::conj( ( *this )( s ) );

    }

  }
  GKG_CATCH( "void gkg::CVector::conjugate()" );

}


gkg::CVector gkg::CVector::getConjugate() const
{

  try
  {

    gkg::CVector cvector( *this );
    cvector.conjugate();
    return cvector;

  }
  GKG_CATCH( "gkg::CVector gkg::CVector::getConjugate() const" );

}


gkg::CMatrix gkg::CVector::getDiagonal() const
{

  try
  {

    gkg::CMatrix cmatrix( getSize(), getSize() );
    cmatrix.setZero();
    for ( int32_t s = 0; s < getSize(); s++ )
    {

      cmatrix( s, s ) = ( *this )( s );

    }
    return cmatrix;

  }
  GKG_CATCH( "gkg::CMatrix gkg::CVector::getDiagonal() const" );

}


std::complex< double > 
gkg::CVector::getComposition( const gkg::CVector& other ) const
{

  try
  {

    if ( getSize() != other.getSize() )
    {

      std::runtime_error( "incompatible vector size(s)" );
    }

    std::complex< double > composition = 0.0;

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      composition += ( *this )( s ) * other( s );

    }
    return composition;

  }
  GKG_CATCH( "std::complex< double > gkg::CVector::getComposition( "
             "const gkg::CVector& other ) const" );

}


std::complex< double > 
gkg::CVector::getComposition( const gkg::Vector& other ) const
{

  try
  {

    if ( getSize() != other.getSize() )
    {

      std::runtime_error( "incompatible vector size(s)" );
    }

    std::complex< double > composition = 0.0;

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      composition += ( *this )( s ) * other( s );

    }
    return composition;

  }
  GKG_CATCH( "std::complex< double > gkg::CVector::getComposition( "
             "const gkg::Vector& other ) const" );

}


gkg::Vector gkg::CVector::getMagnitude() const
{

  try
  {

    gkg::Vector magnitude( getSize() );

    int32_t i;
    for ( i = 0; i < getSize(); i++ )
    {

      magnitude( i ) = std::abs( ( *this )( i ) );

    }
    return magnitude;

  }
  GKG_CATCH( "gkg::Vector gkg::CVector::getMagnitude() const" );

}


std::complex< double > gkg::CVector::getMean() const
{

  try
  {

    std::complex< double > accumulator = 0.0;
    int32_t size = getSize();
    for ( int32_t s = 0; s < size; s++ )
    {

      accumulator += ( *this )( s );

    }
    return accumulator / ( double )size;

  }
  GKG_CATCH( "std::complex< double > gkg::CVector::getMean() const" );

}


gkg::CMatrix gkg::CVector::toCMatrix() const
{

  try
  {

    int32_t size = getSize();
    gkg::CMatrix result( size, 1 );
    int32_t s;
    for ( s = 0; s < size; s++ )
    {

      result( s, 0 ) = ( *this )( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CMatrix gkg::CVector::toCMatrix() const" );

}


gkg::Vector gkg::CVector::getRealPart() const
{

  try
  {

    int32_t size = getSize();
    gkg::Vector realPart( size );

    for ( int32_t i = 0; i < size; ++i )
    {

      realPart( i ) = std::real( ( *this )( i ) );

    }
    return realPart;

  }
  GKG_CATCH( "gkg::Vector gkg::CVector::getRealPart() const" );

}


gkg::Vector gkg::CVector::getImaginaryPart() const
{

  try
  {

    int32_t size = getSize();
    gkg::Vector imaginaryPart( size );

    for ( int32_t i = 0; i < size; ++i )
    {

      imaginaryPart( i ) = std::imag( ( *this )( i ) );

    }
    return imaginaryPart;

  }
  GKG_CATCH( "gkg::Vector gkg::CVector::getImaginaryPart() const" );

}


gkg::CVectorImplementation* gkg::CVector::getImplementation() const
{

  return _cvectorImplementation;

}


gkg::CVector 
operator + ( const gkg::CVector& thing )
{

  try
  {

    return thing;

  }
  GKG_CATCH( "gkg::CVector "
             "operator + ( const gkg::CVector& thing )" );

}


gkg::CVector 
operator - ( const gkg::CVector& thing )
{

  try
  {

    gkg::CVector result( thing.getSize() );
    for ( int32_t s = 0; s < thing.getSize(); s++ )
    {

      result( s ) = -thing( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator - ( const gkg::CVector& thing )" );

}


gkg::CVector
operator + ( const gkg::CVector& thing1,
             const gkg::CVector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) + thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator + ( const gkg::CVector& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::CVector
operator + ( const gkg::CVector& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) + thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator + ( const gkg::CVector& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CVector
operator - ( const gkg::CVector& thing1,
             const gkg::CVector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) - thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator - ( const gkg::CVector& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::CVector
operator - ( const gkg::CVector& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) - thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator - ( const gkg::CVector& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CVector& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) * thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CVector& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CVector& thing1,
             const double& thing2 )
{

  try
  {

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) * thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CVector& thing1, "
             "const double& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CVector& thing1,
             const gkg::CMatrix& thing2 )
{

  try
  {

    if ( thing2.getSize1() != thing1.getSize() )
    {

      std::runtime_error( "the row count for this matrix is not equal to "
                          "the vector size" );
    }

    gkg::CVector result( thing2.getSize2() );
    for ( int32_t s2 = 0; s2 < thing2.getSize2(); s2++ )
    {

      result( s2 ) = 0.0;
      for ( int32_t s1 = 0; s1 < thing2.getSize1(); s1++ )
      {

        result( s2 ) += thing1( s1 ) * thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CVector& thing1, "
             "const gkg::CMatrix& thing2 )" );

}


gkg::CVector
operator * ( const gkg::CVector& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    if ( thing2.getSize1() != thing1.getSize() )
    {

      std::runtime_error( "the row count for this matrix is not equal to "
                          "the vector size" );
    }

    gkg::CVector result( thing2.getSize2() );
    for ( int32_t s2 = 0; s2 < thing2.getSize2(); s2++ )
    {

      result( s2 ) = 0.0;
      for ( int32_t s1 = 0; s1 < thing2.getSize1(); s1++ )
      {

        result( s2 ) += thing1( s1 ) * thing2( s1, s2 );

      }

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator * ( const gkg::CVector& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CVector
operator / ( const gkg::CVector& thing1,
             const std::complex< double >& thing2 )
{

  try
  {

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) / thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator / ( const gkg::CVector& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::CVector
operator / ( const gkg::CVector& thing1,
             const double& thing2 )
{

  try
  {

    gkg::CVector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) / thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::CVector "
             "operator / ( const gkg::CVector& thing1, "
             "const double& thing2 )" );

}


std::ostream& std::operator << ( std::ostream& os, const gkg::CVector& thing )
{

  for ( int32_t s = 0; s < thing.getSize() - 1; s++ )
  {

    os << thing( s ) << " ";

  }
  os << thing( thing.getSize() - 1 );
  return os;

}


