#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/VectorImplementation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Vector::Vector( int32_t size )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                       createVectorImplementation( this, size );

  }
  GKG_CATCH( "gkg::Vector::Vector( int size )" );

}


gkg::Vector::Vector( const gkg::Vector& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                  createVectorImplementation( this,
                                                              other.getSize() );

    for ( int32_t s = 0; s < other.getSize(); s++ )
    {

      ( *this )( s ) = other( s );

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const gkg::Vector& other )" );

}


gkg::Vector::Vector( const std::vector< int32_t >& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const std::vector< int32_t >& other )" );

}


gkg::Vector::Vector( const std::vector< float >& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const std::vector< float >& other )" );

}


gkg::Vector::Vector( const std::vector< double >& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createVectorImplementation( this,
                                                      ( int32_t )other.size() );

    for ( int32_t s = 0; s < ( int32_t )other.size(); s++ )
    {

      ( *this )( s ) = other[ s ];

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const std::vector< double >& other )" );

}


gkg::Vector::Vector( const float* other, int32_t count )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                      createVectorImplementation( this, count );

    for ( int32_t s = 0; s < count; s++ )
    {

      ( *this )( s ) = ( double )other[ s ];

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const float* other, int32_t count )" );

}


gkg::Vector::Vector( const double* other, int32_t count )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                      createVectorImplementation( this, count );

    for ( int32_t s = 0; s < count; s++ )
    {

      ( *this )( s ) = other[ s ];

    }

  }
  GKG_CATCH( "gkg::Vector::Vector( const double* other, int32_t count )" );

}


gkg::Vector::Vector( const gkg::Vector3d< float >& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createVectorImplementation( this, 3 );

    ( *this )( 0 ) = ( double )other.x;
    ( *this )( 1 ) = ( double )other.y;
    ( *this )( 2 ) = ( double )other.z;

  }
  GKG_CATCH( "gkg::Vector::Vector( const gkg::Vector3d< float >& other )" );

}


gkg::Vector::Vector( const gkg::Vector3d< double >& other )
{

  try
  {

    _vectorImplementation =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                          createVectorImplementation( this, 3 );

    ( *this )( 0 ) = other.x;
    ( *this )( 1 ) = other.y;
    ( *this )( 2 ) = other.z;

  }
  GKG_CATCH( "gkg::Vector::Vector( const gkg::Vector3d< double >& other )" );

}


gkg::Vector::~Vector()
{

  delete _vectorImplementation;

}


gkg::Vector& gkg::Vector::operator=( const gkg::Vector& other )
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
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const gkg::Vector& other )" );

}


gkg::Vector& gkg::Vector::operator=( const std::vector< int32_t >& other )
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
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const std::vector< int32_t >& other )" );

}


gkg::Vector& gkg::Vector::operator=( const std::vector< float >& other )
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
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const std::vector< float >& other )" );

}


gkg::Vector& gkg::Vector::operator=( const std::vector< double >& other )
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
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const std::vector< double >& other )" );

}


gkg::Vector& gkg::Vector::operator=( const gkg::Vector3d< float >& other )
{

  try
  {

    reallocate( 3 );

    ( *this )( 0 ) = ( double )other.x;
    ( *this )( 1 ) = ( double )other.y;
    ( *this )( 2 ) = ( double )other.z;

    return *this;

  }
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const gkg::Vector3d< float >& other )" );

}


gkg::Vector& gkg::Vector::operator=( const gkg::Vector3d< double >& other )
{

  try
  {

    reallocate( 3 );

    ( *this )( 0 ) = other.x;
    ( *this )( 1 ) = other.y;
    ( *this )( 2 ) = other.z;

    return *this;

  }
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator=( const gkg::Vector3d< double >& other )" );

}


gkg::Vector& gkg::Vector::operator *= ( const double& thing )
{

  try
  {

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      ( *this )( s ) *= thing;

    }
    return *this;

  }
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator *= ( const double& thing )" );

}


gkg::Vector& gkg::Vector::operator /= ( const double& thing )
{

  try
  {

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      ( *this )( s ) /= thing;

    }
    return *this;

  }
  GKG_CATCH( "gkg::Vector& "
             "gkg::Vector::operator /= ( const double& thing )" );

}


void gkg::Vector::reallocate( int32_t size )
{

  try
  {

    if ( size != getSize() )
    {
         
      delete _vectorImplementation;
      _vectorImplementation =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                                             createVectorImplementation( this,
                                                                         size );

    }

  }
  GKG_CATCH( "void gkg::Vector::reallocate( int size )" );

}


int32_t gkg::Vector::getSize() const
{

  try
  {

    return _vectorImplementation->getSize();

  }
  GKG_CATCH( "int gkg::Vector::getSize() const" );

}


const double& gkg::Vector::operator()( int32_t i ) const
{

  try
  {

    return _vectorImplementation->operator()( i );

  }
  GKG_CATCH( "const double& gkg::Vector::operator()( int i ) const" );

}


double& gkg::Vector::operator()( int32_t i )
{

  try
  {

    return _vectorImplementation->operator()( i );

  }
  GKG_CATCH( "const double& gkg::Vector::operator()( int i )" );

} 


void gkg::Vector::setZero()
{

  try
  {

    _vectorImplementation->setZero();

  }
  GKG_CATCH( "void gkg::Vector::setZero()" );

}


void gkg::Vector::setBasis( int32_t i )
{

  try
  {

    _vectorImplementation->setBasis( i );

  }
  GKG_CATCH( "void gkg::Vector::setBasis( int32_t i )" );

}


void gkg::Vector::fill( const double& value )
{

  try
  {

    _vectorImplementation->fill( value );

  }
  GKG_CATCH( "void gkg::Vector::fill( const double& value )" );

}


gkg::Matrix gkg::Vector::getDiagonal() const
{

  try
  {

    gkg::Matrix matrix( getSize(), getSize() );
    matrix.setZero();
    for ( int32_t s = 0; s < getSize(); s++ )
    {

      matrix( s, s ) = ( *this )( s );

    }
    return matrix;

  }
  GKG_CATCH( "gkg::Matrix gkg::Vector::getDiagonal() const" );

}


double gkg::Vector::getComposition( const gkg::Vector& other ) const
{

  try
  {

    if ( getSize() != other.getSize() )
    {

      std::runtime_error( "incompatible vector size(s)" );
    }

    double composition = 0.0;

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      composition += ( *this )( s ) * other( s );

    }
    return composition;

  }
  GKG_CATCH( "double gkg::Vector::getComposition( "
             "const gkg::Vector& other ) const" );

}


std::complex< double >
gkg::Vector::getComposition( const gkg::CVector& other ) const
{

  try
  {

    if ( getSize() != other.getSize() )
    {

      std::runtime_error( "incompatible vector size(s)" );
    }

    std::complex< double > composition( 0, 0 );

    for ( int32_t s = 0; s < getSize(); s++ )
    {

      composition += ( *this )( s ) * other( s );

    }
    return composition;

  }
  GKG_CATCH( "std::complex< double > gkg::Vector::getComposition( "
             "const gkg::CVector& other ) const" );

}


double gkg::Vector::getMean() const
{

  try
  {

    double accumulator = 0.0;
    int32_t size = getSize();
    for ( int32_t s = 0; s < size; s++ )
    {

      accumulator += ( *this )( s );

    }
    return accumulator / ( double )size;

  }
  GKG_CATCH( "double gkg::Vector::getMean() const" );

}


double gkg::Vector::getNorm() const
{

  try
  {

    return std::sqrt( this->getSumOfSquareValues() );

  }
  GKG_CATCH( "double gkg::Vector::getNorm() const" );

}


double gkg::Vector::getStdDev() const
{

  try
  {

    int32_t size = getSize();

    if ( size == 1 )
    {

      return 0.0;

    }

    double mean = getMean();
    double accumulator = 0.0;
    double deviation = 0.0;
    for ( int32_t s = 0; s < size; s++ )
    {

      deviation = ( *this )( s ) - mean;
      accumulator += deviation * deviation;

    }
    return std::sqrt( accumulator / ( double )( size - 1 ) );

  }
  GKG_CATCH( "double gkg::Vector::getStdDev() const" );

}


gkg::Vector gkg::Vector::cross( const gkg::Vector& other ) const
{

  try
  {

    if ( ( other.getSize() != 3 ) || ( getSize() != 3 ) )
    {

      throw std::runtime_error( "vector size should be 3" );

    }

    gkg::Vector result( 3 );
    result( 0 ) = ( *this )( 1 ) * other( 2 ) - ( *this )( 2 ) * other( 1 );
    result( 1 ) = ( *this )( 2 ) * other( 0 ) - ( *this )( 0 ) * other( 2 );
    result( 2 ) = ( *this )( 0 ) * other( 1 ) - ( *this )( 1 ) * other( 0 );

    return result;

  }
  GKG_CATCH( "gkg::Vector::cross( const gkg::Vector& other ) const" );

}


double gkg::Vector::getSumOfValues() const
{

  try
  {

    double sum = 0;

    for ( int32_t s = 0; s < getSize(); ++ s )
    {

      sum += ( *this )( s );

    }

    return sum;

  }
  GKG_CATCH( "double gkg::Vector::getSumOfValues() const" );

}


double gkg::Vector::getSumOfAbsoluteValues() const
{

  try
  {

    double sumAbs = 0;

    for ( int32_t s = 0; s < getSize(); ++ s )
    {

      sumAbs += std::fabs( ( *this )( s ) );

    }

    return sumAbs;

  }
  GKG_CATCH( "double gkg::Vector::getSumOfAbsoluteValues() const" );

}


double gkg::Vector::getSumOfSquareValues() const
{

  try
  {

    double sumSquare = 0;

    for ( int32_t s = 0; s < getSize(); ++ s )
    {

      sumSquare += ( ( *this )( s ) * ( *this )( s ) );

    }

    return sumSquare;

  }
  GKG_CATCH( "double gkg::Vector::getSumOfSquareValues() const" );

}


gkg::Vector& gkg::Vector::normalize()
{

  try
  {

    double norm = this->getNorm();

    for ( int32_t s = 0; s < this->getSize(); ++ s )
    {

      ( *this )( s ) /= norm;

    }

    return *this;

  }
  GKG_CATCH( "gkg::Vector& gkg::Vector::normalize()" );

}


gkg::Matrix gkg::Vector::toMatrix() const
{

  try
  {

    int32_t size = getSize();
    gkg::Matrix result( size, 1 );
    int32_t s;
    for ( s = 0; s < size; s++ )
    {

      result( s, 0 ) = ( *this )( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::Matrix gkg::Vector::toMatrix() const" );

}


gkg::VectorImplementation* gkg::Vector::getImplementation() const
{

  return _vectorImplementation;

}


gkg::Vector 
operator + ( const gkg::Vector& thing )
{

  try
  {

    return thing;

  }
  GKG_CATCH( "gkg::Vector "
             "operator + ( const gkg::Vector& thing )" );

}


gkg::Vector 
operator - ( const gkg::Vector& thing )
{

  try
  {

    gkg::Vector result( thing.getSize() );
    for ( int32_t s = 0; s < thing.getSize(); s++ )
    {

      result( s ) = -thing( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator - ( const gkg::Vector& thing )" );

}


gkg::Vector
operator + ( const gkg::Vector& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::Vector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) + thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator + ( const gkg::Vector& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CVector
operator + ( const gkg::Vector& thing1,
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
             "operator + ( const gkg::Vector& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::Vector
operator - ( const gkg::Vector& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    if ( thing1.getSize() != thing2.getSize() )
    {

      throw std::runtime_error( "incompatible vector size(s)" );

    }

    gkg::Vector result( thing1.getSize() );
    for ( int32_t s = 0; s < result.getSize(); s++ )
    {

      result( s ) = thing1( s ) - thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator - ( const gkg::Vector& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::CVector
operator - ( const gkg::Vector& thing1,
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
             "operator - ( const gkg::Vector& thing1, "
             "const gkg::CVector& thing2 )" );

}


gkg::Vector
operator * ( const gkg::Vector& thing1,
             const double& thing2 )
{

  try
  {

    gkg::Vector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) * thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator * ( const gkg::Vector& thing1, "
             "const double& thing2 )" );

}


gkg::Vector
operator * ( const double& thing1,
             const gkg::Vector& thing2 )
{

  try
  {

    gkg::Vector result( thing2.getSize() );
    for ( int32_t s = 0; s < thing2.getSize(); s++ )
    {

      result( s ) = thing1 * thing2( s );

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator * ( const double& thing1, "
             "const gkg::Vector& thing2 )" );

}


gkg::Vector
operator * ( const gkg::Vector& thing1,
             const gkg::Matrix& thing2 )
{

  try
  {

    if ( thing2.getSize1() != thing1.getSize() )
    {

      std::runtime_error( "the row count for this matrix is not equal to "
                          "the vector size" );
    }

    gkg::Vector result( thing2.getSize2() );
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
  GKG_CATCH( "gkg::Vector "
             "operator * ( const gkg::Vector& thing1, "
             "const gkg::Matrix& thing2 )" );

}


gkg::CVector
operator * ( const gkg::Vector& thing1,
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
             "operator * ( const gkg::Vector& thing1, "
             "const std::complex< double >& thing2 )" );

}


gkg::Vector
operator / ( const gkg::Vector& thing1,
             const double& thing2 )
{

  try
  {

    gkg::Vector result( thing1.getSize() );
    for ( int32_t s = 0; s < thing1.getSize(); s++ )
    {

      result( s ) = thing1( s ) / thing2;

    }
    return result;

  }
  GKG_CATCH( "gkg::Vector "
             "operator / ( const gkg::Vector& thing1, "
             "const double& thing2 )" );

}


gkg::CVector
operator / ( const gkg::Vector& thing1,
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
             "operator / ( const gkg::Vector& thing1, "
             "const std::complex< double >& thing2 )" );

}


std::ostream& std::operator << ( std::ostream& os, const gkg::Vector& thing )
{

  for ( int32_t s = 0; s < thing.getSize() - 1; s++ )
  {

    os << thing( s ) << " ";

  }
  os << thing( thing.getSize() - 1 );
  return os;

}


bool std::operator == ( const gkg::Vector& thing1,
                        const gkg::Vector& thing2 )
{

  int size = thing1.getSize();

  if ( thing2.getSize() != size )
  {

    return false;

  }

  for ( int32_t s = 0; s < size; s++ )
  {

    if ( thing1( s ) != thing2( s ) )
    {

      return false;

    }

  }
  return true;

}



