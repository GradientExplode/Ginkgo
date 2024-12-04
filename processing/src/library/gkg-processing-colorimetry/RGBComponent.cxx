#include <gkg-processing-colorimetry/RGBComponent.h>
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
// class RGBComponent
//

gkg::RGBComponent::RGBComponent( uint8_t theR, uint8_t theG, uint8_t theB )
                  : r( theR ), g( theG ), b( theB )
{
}


gkg::RGBComponent::RGBComponent( const gkg::RGBComponent& other )
                  : r( other.r ), g( other.g ), b( other.b )
{
}


gkg::RGBComponent&
gkg::RGBComponent::operator = ( const gkg::RGBComponent& other )
{

  r = other.r;
  g = other.g;
  b = other.b;

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator += ( const gkg::RGBComponent& other )
{

  r += other.r;
  g += other.g;
  b += other.b;

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator -= ( const gkg::RGBComponent& other )
{

  r -= other.r;
  g -= other.g;
  b -= other.b;

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator *= ( float value )
{

  r = ( uint8_t )( value * r );
  g = ( uint8_t )( value * g );
  b = ( uint8_t )( value * b );

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator /= ( float value )
{

  assert( value != 0 );
  r = ( uint8_t )( value / r );
  g = ( uint8_t )( value / g );
  b = ( uint8_t )( value / b );

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator *= ( double value )
{

  r = ( uint8_t )( value * r );
  g = ( uint8_t )( value * g );
  b = ( uint8_t )( value * b );

  return *this;

}


gkg::RGBComponent& 
gkg::RGBComponent::operator /= ( double value )
{

  assert( value != 0 );
  r = ( uint8_t )( value / r );
  g = ( uint8_t )( value / g );
  b = ( uint8_t )( value / b );

  return *this;

}


//
// other functions
//


gkg::RGBComponent
operator + ( const gkg::RGBComponent& thing )
{

  return thing;

}


gkg::RGBComponent
operator + ( const gkg::RGBComponent& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( thing1.r + thing2.r,
                            thing1.g + thing2.g,
                            thing1.b + thing2.b );

}


gkg::RGBComponent
operator - ( const gkg::RGBComponent& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( thing1.r - thing2.r,
                            thing1.g - thing2.g,
                            thing1.b - thing2.b );

}


gkg::RGBComponent
operator * ( const gkg::RGBComponent& thing1,
             const float& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing1.r * thing2 ),
                            ( uint8_t )( thing1.g * thing2 ),
                            ( uint8_t )( thing1.b * thing2 ) );

}


gkg::RGBComponent
operator * ( const float& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing2.r * thing1 ),
                            ( uint8_t )( thing2.g * thing1 ),
                            ( uint8_t )( thing2.b * thing1 ) );

}


gkg::RGBComponent
operator / ( const gkg::RGBComponent& thing1,
             const float& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing1.r / thing2 ),
                            ( uint8_t )( thing1.g / thing2 ),
                            ( uint8_t )( thing1.b / thing2 ) );

}


gkg::RGBComponent
operator / ( const float& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing2.r / thing1 ),
                            ( uint8_t )( thing2.g / thing1 ),
                            ( uint8_t )( thing2.b / thing1 ) );

}


gkg::RGBComponent
operator * ( const gkg::RGBComponent& thing1,
             const double& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing1.r * thing2 ),
                            ( uint8_t )( thing1.g * thing2 ),
                            ( uint8_t )( thing1.b * thing2 ) );

}


gkg::RGBComponent
operator * ( const double& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing2.r * thing1 ),
                            ( uint8_t )( thing2.g * thing1 ),
                            ( uint8_t )( thing2.b * thing1 ) );

}


gkg::RGBComponent
operator / ( const gkg::RGBComponent& thing1,
             const double& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing1.r / thing2 ),
                            ( uint8_t )( thing1.g / thing2 ),
                            ( uint8_t )( thing1.b / thing2 ) );

}


gkg::RGBComponent
operator / ( const double& thing1,
             const gkg::RGBComponent& thing2 )
{

  return gkg::RGBComponent( ( uint8_t )( thing2.r / thing1 ),
                            ( uint8_t )( thing2.g / thing1 ),
                            ( uint8_t )( thing2.b / thing1 ) );

}


//
// std functions
//

bool std::operator == ( const gkg::RGBComponent& thing1,
                        const gkg::RGBComponent& thing2 )
{

  return ( thing1.r == thing2.r ) &&
         ( thing1.g == thing2.g ) &&
         ( thing1.b == thing2.b );

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::RGBComponent& thing )
{

  os << "( " << ( uint32_t )thing.r << ", "
             << ( uint32_t )thing.g << ", "
             << ( uint32_t )thing.b << " )";
  return os;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::RGBComponent& thing )
{

  char character = 0;
  uint32_t tmp;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> tmp;
  thing.r = tmp;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> tmp;
  thing.g = tmp;
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> tmp;
  thing.b = tmp;
  skip( is );
  is >> character;

  return is;

}

