#include <gkg-processing-coordinates/Vector3d.h>
#include <cmath>
#include <cassert>
#include <cstdio>



static void skip( std::istream& is )
{

  std::string chars = "\t\n\r";
  int32_t n = 0;

  int c = is.get();

  ++ n;
  while ( ( c != EOF ) && ( chars.find_first_of(c) != std::string::npos ) )
  {

    c = is.get();
    ++ n;

  }
  if ( c != EOF )
  {

    is.putback( c );
    -- n;

  }

}


//
// class Vector3d< T >
//

template < class T >
gkg::Vector3d< T >::Vector3d()
                   : x( 0 ), y( 0 ), z( 0 )
{
}


template < class T >
gkg::Vector3d< T >::Vector3d( T theX, T theY, T theZ )
                   : x( theX ), y( theY ), z( theZ )
{
}


template < class T >
gkg::Vector3d< T >::Vector3d( const gkg::Vector3d< T >& other )
                 : x( other.x ), y( other.y ), z( other.z )
{
}


template < class T >
gkg::Vector3d< T >& gkg::Vector3d< T >::operator = (
                                               const gkg::Vector3d< T >& other )
{

  x = other.x;
  y = other.y;
  z = other.z;

  return *this;

}


template < class T >
gkg::Vector3d< T >& 
gkg::Vector3d< T >::operator += ( const gkg::Vector3d< T >& other )
{

  x += other.x;
  y += other.y;
  z += other.z;

  return *this;

}


template < class T >
gkg::Vector3d< T >& 
gkg::Vector3d< T >::operator -= ( const gkg::Vector3d< T >& other )
{

  x -= other.x;
  y -= other.y;
  z -= other.z;

  return *this;

}


template < class T >
gkg::Vector3d< T >& 
gkg::Vector3d< T >::operator *= ( T value )
{

  x *= value;
  y *= value;
  z *= value;

  return *this;

}


template < class T >
gkg::Vector3d< T >& 
gkg::Vector3d< T >::operator /= ( T value )
{

  assert( value != 0 );
  x /= value;
  y /= value;
  z /= value;

  return *this;

}


template < class T >
bool gkg::Vector3d< T >::operator==( const gkg::Vector3d< T >& other ) const
{

  return ( ( x == other.x ) &&
           ( y == other.y ) &&
           ( z == other.z ) );

}


template < class T >
bool gkg::Vector3d< T >::operator!=( const gkg::Vector3d< T >& other ) const
{

  return ( ( x != other.x ) ||
           ( y != other.y ) ||
           ( z != other.z ) );

}


template < class T >
T 
gkg::Vector3d< T >::dot( const gkg::Vector3d< T >& other ) const
{

  return x * other.x + y * other.y + z * other.z;

}


template < class T >
gkg::Vector3d< T > 
gkg::Vector3d< T >::cross( const gkg::Vector3d< T >& other ) const
{

  
  return gkg::Vector3d< T >( y * other.z - z * other.y,
                             z * other.x - x * other.z,
                             x * other.y - y * other.x );

}


template < class T >
double gkg::Vector3d< T >::getDoubleNorm() const
{

  return std::sqrt( getDoubleNorm2() );

}


template < class T >
double gkg::Vector3d< T >::getDoubleNorm2() const
{

  return ( double )getNorm2();

}


template < class T >
T gkg::Vector3d< T >::getNorm() const
{

  return ( T )getDoubleNorm();

}


template < class T >
T gkg::Vector3d< T >::getNorm2() const
{

  return x * x + y * y + z * z;

}


template < class T >
gkg::Vector3d< T >& gkg::Vector3d< T >::normalize()
{

  return this->operator/=( getNorm() );

}


template < class T >
gkg::Vector3d< T > gkg::Vector3d< T >::mainDirection() const
{

  gkg::Vector3d< T > mainVector( 0, 0, 0 );
  double absX = std::fabs( x );
  double absY = std::fabs( y );
  double absZ = std::fabs( z );

  if ( absX > absY )
  {

    if ( absX > absZ )
    {
  
      mainVector.x = x / absX;
  
    }
    else
    {
  
      mainVector.z = z / absZ;
  
    }

  }
  else
  {

    if ( absY > absZ )
    {
  
      mainVector.y = y / absY;
  
    }
    else
    {
   
      mainVector.z = z / absZ;
  
    }

  }

  return mainVector;

}


template < class T >
gkg::Vector3d< T > gkg::Vector3d< T >::absoluteCoordinates() const
{

  return gkg::Vector3d< T >( std::fabs( x ), std::fabs( y ), std::fabs( z ) );

}


//
// other functions
//


template < class T >
gkg::Vector3d< T >
operator + ( const gkg::Vector3d< T >& thing )
{

  return thing;

}


template < class T >
gkg::Vector3d< T >
operator - ( const gkg::Vector3d< T >& thing )
{

  return gkg::Vector3d< T >( -thing.x,
                             -thing.y,
                             -thing.z );

}


template < class T >
gkg::Vector3d< T >
operator + ( const gkg::Vector3d< T >& thing1,
             const gkg::Vector3d< T >& thing2 )
{

  return gkg::Vector3d< T >( thing1.x + thing2.x,
                             thing1.y + thing2.y,
                             thing1.z + thing2.z );

}


template < class T >
gkg::Vector3d< T >
operator - ( const gkg::Vector3d< T >& thing1,
             const gkg::Vector3d< T >& thing2 )
{

  return gkg::Vector3d< T >( thing1.x - thing2.x,
                             thing1.y - thing2.y,
                             thing1.z - thing2.z );

}


template < class T >
gkg::Vector3d< T >
operator * ( const gkg::Vector3d< T >& thing1,
             const T& thing2 )
{

  return gkg::Vector3d< T >( thing1.x * thing2,
                             thing1.y * thing2,
                             thing1.z * thing2 );

}


template < class T >
gkg::Vector3d< T >
operator / ( const gkg::Vector3d< T >& thing1,
             const T& thing2 )
{

  return gkg::Vector3d< T >( thing1.x / thing2,
                             thing1.y / thing2,
                             thing1.z / thing2 );

}


//
// std functions
//


/*
bool std::operator == ( const gkg::Vector3d< int16_t >& thing1,
                        const gkg::Vector3d< int16_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< int32_t >& thing1,
                        const gkg::Vector3d< int32_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< int64_t >& thing1,
                        const gkg::Vector3d< int64_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< uint16_t >& thing1,
                        const gkg::Vector3d< uint16_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< uint32_t >& thing1,
                        const gkg::Vector3d< uint32_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< uint64_t >& thing1,
                        const gkg::Vector3d< uint64_t >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< float >& thing1,
                        const gkg::Vector3d< float >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}


bool std::operator == ( const gkg::Vector3d< double >& thing1,
                        const gkg::Vector3d< double >& thing2 )
{

  return ( thing1.x == thing2.x ) &&
         ( thing1.y == thing2.y ) &&
         ( thing1.z == thing2.z );

}

*/

std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< int16_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< int32_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< int64_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< uint16_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< uint32_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< uint64_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< float >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector3d< double >& thing )
{

  os << "( " << thing.x << ", " << thing.y << ", " << thing.z << " )";
  return os;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< int16_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< int32_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< int64_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< uint16_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< uint32_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< uint64_t >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< float >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector3d< double >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.x;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.y;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.z;
  skip( is );
  is >> character;

  return is;

}


// 
// forcing instanciations
//

template struct gkg::Vector3d< int16_t >;
template struct gkg::Vector3d< int32_t >;
template struct gkg::Vector3d< int64_t >;
template struct gkg::Vector3d< uint16_t >;
template struct gkg::Vector3d< uint32_t >;
template struct gkg::Vector3d< uint64_t >;
template struct gkg::Vector3d< float >;
template struct gkg::Vector3d< double >;


template gkg::Vector3d< int16_t >
operator + ( const gkg::Vector3d< int16_t >& thing );
template gkg::Vector3d< int32_t >
operator + ( const gkg::Vector3d< int32_t >& thing );
template gkg::Vector3d< int64_t >
operator + ( const gkg::Vector3d< int64_t >& thing );
template gkg::Vector3d< uint16_t >
operator + ( const gkg::Vector3d< uint16_t >& thing );
template gkg::Vector3d< uint32_t >
operator + ( const gkg::Vector3d< uint32_t >& thing );
template gkg::Vector3d< uint64_t >
operator + ( const gkg::Vector3d< uint64_t >& thing );
template gkg::Vector3d< float >
operator + ( const gkg::Vector3d< float >& thing );
template gkg::Vector3d< double >
operator + ( const gkg::Vector3d< double >& thing );


template gkg::Vector3d< int16_t >
operator - ( const gkg::Vector3d< int16_t >& thing );
template gkg::Vector3d< int32_t >
operator - ( const gkg::Vector3d< int32_t >& thing );
template gkg::Vector3d< int64_t >
operator - ( const gkg::Vector3d< int64_t >& thing );
template gkg::Vector3d< uint16_t >
operator - ( const gkg::Vector3d< uint16_t >& thing );
template gkg::Vector3d< uint32_t >
operator - ( const gkg::Vector3d< uint32_t >& thing );
template gkg::Vector3d< uint64_t >
operator - ( const gkg::Vector3d< uint64_t >& thing );
template gkg::Vector3d< float >
operator - ( const gkg::Vector3d< float >& thing );
template gkg::Vector3d< double >
operator - ( const gkg::Vector3d< double >& thing );


template gkg::Vector3d< int16_t >
operator + ( const gkg::Vector3d< int16_t >& thing1,
             const gkg::Vector3d< int16_t >& thing2 );
template gkg::Vector3d< int32_t >
operator + ( const gkg::Vector3d< int32_t >& thing1,
             const gkg::Vector3d< int32_t >& thing2 );
template gkg::Vector3d< int64_t >
operator + ( const gkg::Vector3d< int64_t >& thing1,
             const gkg::Vector3d< int64_t >& thing2 );
template gkg::Vector3d< uint16_t >
operator + ( const gkg::Vector3d< uint16_t >& thing1,
             const gkg::Vector3d< uint16_t >& thing2 );
template gkg::Vector3d< uint32_t >
operator + ( const gkg::Vector3d< uint32_t >& thing1,
             const gkg::Vector3d< uint32_t >& thing2 );
template gkg::Vector3d< uint64_t >
operator + ( const gkg::Vector3d< uint64_t >& thing1,
             const gkg::Vector3d< uint64_t >& thing2 );
template gkg::Vector3d< float >
operator + ( const gkg::Vector3d< float >& thing1,
             const gkg::Vector3d< float >& thing2 );
template gkg::Vector3d< double >
operator + ( const gkg::Vector3d< double >& thing1,
             const gkg::Vector3d< double >& thing2 );


template gkg::Vector3d< int16_t >
operator - ( const gkg::Vector3d< int16_t >& thing1,
             const gkg::Vector3d< int16_t >& thing2 );
template gkg::Vector3d< int32_t >
operator - ( const gkg::Vector3d< int32_t >& thing1,
             const gkg::Vector3d< int32_t >& thing2 );
template gkg::Vector3d< int64_t >
operator - ( const gkg::Vector3d< int64_t >& thing1,
             const gkg::Vector3d< int64_t >& thing2 );
template gkg::Vector3d< uint16_t >
operator - ( const gkg::Vector3d< uint16_t >& thing1,
             const gkg::Vector3d< uint16_t >& thing2 );
template gkg::Vector3d< uint32_t >
operator - ( const gkg::Vector3d< uint32_t >& thing1,
             const gkg::Vector3d< uint32_t >& thing2 );
template gkg::Vector3d< uint64_t >
operator - ( const gkg::Vector3d< uint64_t >& thing1,
             const gkg::Vector3d< uint64_t >& thing2 );
template gkg::Vector3d< float >
operator - ( const gkg::Vector3d< float >& thing1,
             const gkg::Vector3d< float >& thing2 );
template gkg::Vector3d< double >
operator - ( const gkg::Vector3d< double >& thing1,
             const gkg::Vector3d< double >& thing2 );


template gkg::Vector3d< int16_t >
operator * ( const gkg::Vector3d< int16_t >& thing1,
             const int16_t& thing2 );
template gkg::Vector3d< int32_t >
operator * ( const gkg::Vector3d< int32_t >& thing1,
             const int32_t& thing2 );
template gkg::Vector3d< int64_t >
operator * ( const gkg::Vector3d< int64_t >& thing1,
             const int64_t& thing2 );
template gkg::Vector3d< uint16_t >
operator * ( const gkg::Vector3d< uint16_t >& thing1,
             const uint16_t& thing2 );
template gkg::Vector3d< uint32_t >
operator * ( const gkg::Vector3d< uint32_t >& thing1,
             const uint32_t& thing2 );
template gkg::Vector3d< uint64_t >
operator * ( const gkg::Vector3d< uint64_t >& thing1,
             const uint64_t& thing2 );
template gkg::Vector3d< float >
operator * ( const gkg::Vector3d< float >& thing1,
             const float& thing2 );
template gkg::Vector3d< double >
operator * ( const gkg::Vector3d< double >& thing1,
             const double& thing2 );


template gkg::Vector3d< int16_t >
operator / ( const gkg::Vector3d< int16_t >& thing1,
             const int16_t& thing2 );
template gkg::Vector3d< int32_t >
operator / ( const gkg::Vector3d< int32_t >& thing1,
             const int32_t& thing2 );
template gkg::Vector3d< int64_t >
operator / ( const gkg::Vector3d< int64_t >& thing1,
             const int64_t& thing2 );
template gkg::Vector3d< uint16_t >
operator / ( const gkg::Vector3d< uint16_t >& thing1,
             const uint16_t& thing2 );
template gkg::Vector3d< uint32_t >
operator / ( const gkg::Vector3d< uint32_t >& thing1,
             const uint32_t& thing2 );
template gkg::Vector3d< uint64_t >
operator / ( const gkg::Vector3d< uint64_t >& thing1,
             const uint64_t& thing2 );
template gkg::Vector3d< float >
operator / ( const gkg::Vector3d< float >& thing1,
             const float& thing2 );
template gkg::Vector3d< double >
operator / ( const gkg::Vector3d< double >& thing1,
             const double& thing2 );
