#include <gkg-processing-coordinates/Vector2d.h>
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
// class Vector2d< T >
//

template < class T >
gkg::Vector2d< T >::Vector2d()
                   : x( 0 ), y( 0 )
{
}


template < class T >
gkg::Vector2d< T >::Vector2d( T theX, T theY )
                   : x( theX ), y( theY )
{
}


template < class T >
gkg::Vector2d< T >::Vector2d( const gkg::Vector2d< T >& other )
                 : x( other.x ), y( other.y )
{
}


template < class T >
gkg::Vector2d< T >& gkg::Vector2d< T >::operator = (
                                               const gkg::Vector2d< T >& other )
{

  x = other.x;
  y = other.y;

  return *this;

}


template < class T >
gkg::Vector2d< T >& 
gkg::Vector2d< T >::operator += ( const gkg::Vector2d< T >& other )
{

  x += other.x;
  y += other.y;

  return *this;

}


template < class T >
gkg::Vector2d< T >& 
gkg::Vector2d< T >::operator -= ( const gkg::Vector2d< T >& other )
{

  x -= other.x;
  y -= other.y;

  return *this;

}


template < class T >
gkg::Vector2d< T >& 
gkg::Vector2d< T >::operator *= ( T value )
{

  x *= value;
  y *= value;

  return *this;

}


template < class T >
gkg::Vector2d< T >& 
gkg::Vector2d< T >::operator /= ( T value )
{

  assert( value != 0 );
  x /= value;
  y /= value;

  return *this;

}


template < class T >
bool gkg::Vector2d< T >::operator==( const gkg::Vector2d< T >& other ) const
{

  return ( ( x == other.x ) &&
           ( y == other.y ) );

}


template < class T >
bool gkg::Vector2d< T >::operator!=( const gkg::Vector2d< T >& other ) const
{

  return ( ( x != other.x ) ||
           ( y != other.y ) );

}


template < class T >
T 
gkg::Vector2d< T >::dot( const gkg::Vector2d< T >& other ) const
{

  return x * other.x + y * other.y;

}


template < class T >
double gkg::Vector2d< T >::getDoubleNorm() const
{

  return std::sqrt( getDoubleNorm2() );

}


template < class T >
double gkg::Vector2d< T >::getDoubleNorm2() const
{

  return ( double )getNorm2();

}


template < class T >
T gkg::Vector2d< T >::getNorm() const
{

  return ( T )getDoubleNorm();

}


template < class T >
T gkg::Vector2d< T >::getNorm2() const
{

  return x * x + y * y;

}


template < class T >
gkg::Vector2d< T >& gkg::Vector2d< T >::normalize()
{

  return this->operator/=( getNorm() );

}


//
// other functions
//


template < class T >
gkg::Vector2d< T >
operator + ( const gkg::Vector2d< T >& thing )
{

  return thing;

}


template < class T >
gkg::Vector2d< T >
operator - ( const gkg::Vector2d< T >& thing )
{

  return gkg::Vector2d< T >( -thing.x,
                             -thing.y );

}


template < class T >
gkg::Vector2d< T >
operator + ( const gkg::Vector2d< T >& thing1,
             const gkg::Vector2d< T >& thing2 )
{

  return gkg::Vector2d< T >( thing1.x + thing2.x,
                             thing1.y + thing2.y );

}


template < class T >
gkg::Vector2d< T >
operator - ( const gkg::Vector2d< T >& thing1,
             const gkg::Vector2d< T >& thing2 )
{

  return gkg::Vector2d< T >( thing1.x - thing2.x,
                             thing1.y - thing2.y );

}


template < class T >
gkg::Vector2d< T >
operator * ( const gkg::Vector2d< T >& thing1,
             const T& thing2 )
{

  return gkg::Vector2d< T >( thing1.x * thing2,
                             thing1.y * thing2 );

}


template < class T >
gkg::Vector2d< T >
operator / ( const gkg::Vector2d< T >& thing1,
             const T& thing2 )
{

  return gkg::Vector2d< T >( thing1.x / thing2,
                             thing1.y / thing2 );

}


//
// std functions
//

/*
bool std::operator == ( const gkg::Vector2d< int16_t >& thing1,
                        const gkg::Vector2d< int16_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< int32_t >& thing1,
                        const gkg::Vector2d< int32_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< int64_t >& thing1,
                        const gkg::Vector2d< int64_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< uint16_t >& thing1,
                        const gkg::Vector2d< uint16_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< uint32_t >& thing1,
                        const gkg::Vector2d< uint32_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< uint64_t >& thing1,
                        const gkg::Vector2d< uint64_t >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< float >& thing1,
                        const gkg::Vector2d< float >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}


bool std::operator == ( const gkg::Vector2d< double >& thing1,
                        const gkg::Vector2d< double >& thing2 )
{

  return thing1.x == thing2.x &&
         thing1.y == thing2.y;

}
*/

std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< int16_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< int32_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< int64_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< uint16_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< uint32_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< uint64_t >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< float >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Vector2d< double >& thing )
{

  os << "( " << thing.x << ", " << thing.y << " )";
  return os;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< int16_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< int32_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< int64_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< uint16_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< uint32_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< uint64_t >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< float >& thing )
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
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Vector2d< double >& thing )
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
  is >> character;

  return is;

}


//
// forcing instanciations
//

template struct gkg::Vector2d< int16_t >;
template struct gkg::Vector2d< int32_t >;
template struct gkg::Vector2d< int64_t >;
template struct gkg::Vector2d< uint16_t >;
template struct gkg::Vector2d< uint32_t >;
template struct gkg::Vector2d< uint64_t >;
template struct gkg::Vector2d< float >;
template struct gkg::Vector2d< double >;


template gkg::Vector2d< int16_t >
operator + ( const gkg::Vector2d< int16_t >& thing );
template gkg::Vector2d< int32_t >
operator + ( const gkg::Vector2d< int32_t >& thing );
template gkg::Vector2d< int64_t >
operator + ( const gkg::Vector2d< int64_t >& thing );
template gkg::Vector2d< uint16_t >
operator + ( const gkg::Vector2d< uint16_t >& thing );
template gkg::Vector2d< uint32_t >
operator + ( const gkg::Vector2d< uint32_t >& thing );
template gkg::Vector2d< uint64_t >
operator + ( const gkg::Vector2d< uint64_t >& thing );
template gkg::Vector2d< float >
operator + ( const gkg::Vector2d< float >& thing );
template gkg::Vector2d< double >
operator + ( const gkg::Vector2d< double >& thing );


template gkg::Vector2d< int16_t >
operator - ( const gkg::Vector2d< int16_t >& thing );
template gkg::Vector2d< int32_t >
operator - ( const gkg::Vector2d< int32_t >& thing );
template gkg::Vector2d< int64_t >
operator - ( const gkg::Vector2d< int64_t >& thing );
template gkg::Vector2d< uint16_t >
operator - ( const gkg::Vector2d< uint16_t >& thing );
template gkg::Vector2d< uint32_t >
operator - ( const gkg::Vector2d< uint32_t >& thing );
template gkg::Vector2d< uint64_t >
operator - ( const gkg::Vector2d< uint64_t >& thing );
template gkg::Vector2d< float >
operator - ( const gkg::Vector2d< float >& thing );
template gkg::Vector2d< double >
operator - ( const gkg::Vector2d< double >& thing );


template gkg::Vector2d< int16_t >
operator + ( const gkg::Vector2d< int16_t >& thing1,
             const gkg::Vector2d< int16_t >& thing2 );
template gkg::Vector2d< int32_t >
operator + ( const gkg::Vector2d< int32_t >& thing1,
             const gkg::Vector2d< int32_t >& thing2 );
template gkg::Vector2d< int64_t >
operator + ( const gkg::Vector2d< int64_t >& thing1,
             const gkg::Vector2d< int64_t >& thing2 );
template gkg::Vector2d< uint16_t >
operator + ( const gkg::Vector2d< uint16_t >& thing1,
             const gkg::Vector2d< uint16_t >& thing2 );
template gkg::Vector2d< uint32_t >
operator + ( const gkg::Vector2d< uint32_t >& thing1,
             const gkg::Vector2d< uint32_t >& thing2 );
template gkg::Vector2d< uint64_t >
operator + ( const gkg::Vector2d< uint64_t >& thing1,
             const gkg::Vector2d< uint64_t >& thing2 );
template gkg::Vector2d< float >
operator + ( const gkg::Vector2d< float >& thing1,
             const gkg::Vector2d< float >& thing2 );
template gkg::Vector2d< double >
operator + ( const gkg::Vector2d< double >& thing1,
             const gkg::Vector2d< double >& thing2 );


template gkg::Vector2d< int16_t >
operator - ( const gkg::Vector2d< int16_t >& thing1,
             const gkg::Vector2d< int16_t >& thing2 );
template gkg::Vector2d< int32_t >
operator - ( const gkg::Vector2d< int32_t >& thing1,
             const gkg::Vector2d< int32_t >& thing2 );
template gkg::Vector2d< int64_t >
operator - ( const gkg::Vector2d< int64_t >& thing1,
             const gkg::Vector2d< int64_t >& thing2 );
template gkg::Vector2d< uint16_t >
operator - ( const gkg::Vector2d< uint16_t >& thing1,
             const gkg::Vector2d< uint16_t >& thing2 );
template gkg::Vector2d< uint32_t >
operator - ( const gkg::Vector2d< uint32_t >& thing1,
             const gkg::Vector2d< uint32_t >& thing2 );
template gkg::Vector2d< uint64_t >
operator - ( const gkg::Vector2d< uint64_t >& thing1,
             const gkg::Vector2d< uint64_t >& thing2 );
template gkg::Vector2d< float >
operator - ( const gkg::Vector2d< float >& thing1,
             const gkg::Vector2d< float >& thing2 );
template gkg::Vector2d< double >
operator - ( const gkg::Vector2d< double >& thing1,
             const gkg::Vector2d< double >& thing2 );


template gkg::Vector2d< int16_t >
operator * ( const gkg::Vector2d< int16_t >& thing1,
             const int16_t& thing2 );
template gkg::Vector2d< int32_t >
operator * ( const gkg::Vector2d< int32_t >& thing1,
             const int32_t& thing2 );
template gkg::Vector2d< int64_t >
operator * ( const gkg::Vector2d< int64_t >& thing1,
             const int64_t& thing2 );
template gkg::Vector2d< uint16_t >
operator * ( const gkg::Vector2d< uint16_t >& thing1,
             const uint16_t& thing2 );
template gkg::Vector2d< uint32_t >
operator * ( const gkg::Vector2d< uint32_t >& thing1,
             const uint32_t& thing2 );
template gkg::Vector2d< uint64_t >
operator * ( const gkg::Vector2d< uint64_t >& thing1,
             const uint64_t& thing2 );
template gkg::Vector2d< float >
operator * ( const gkg::Vector2d< float >& thing1,
             const float& thing2 );
template gkg::Vector2d< double >
operator * ( const gkg::Vector2d< double >& thing1,
             const double& thing2 );


template gkg::Vector2d< int16_t >
operator / ( const gkg::Vector2d< int16_t >& thing1,
             const int16_t& thing2 );
template gkg::Vector2d< int32_t >
operator / ( const gkg::Vector2d< int32_t >& thing1,
             const int32_t& thing2 );
template gkg::Vector2d< int64_t >
operator / ( const gkg::Vector2d< int64_t >& thing1,
             const int64_t& thing2 );
template gkg::Vector2d< uint16_t >
operator / ( const gkg::Vector2d< uint16_t >& thing1,
             const uint16_t& thing2 );
template gkg::Vector2d< uint32_t >
operator / ( const gkg::Vector2d< uint32_t >& thing1,
             const uint32_t& thing2 );
template gkg::Vector2d< uint64_t >
operator / ( const gkg::Vector2d< uint64_t >& thing1,
             const uint64_t& thing2 );
template gkg::Vector2d< float >
operator / ( const gkg::Vector2d< float >& thing1,
             const float& thing2 );
template gkg::Vector2d< double >
operator / ( const gkg::Vector2d< double >& thing1,
             const double& thing2 );
