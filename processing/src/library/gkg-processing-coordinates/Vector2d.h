#ifndef _gkg_processing_coordinates_Vector2d_h_
#define _gkg_processing_coordinates_Vector2d_h_

#include <iostream>
#include <functional>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T >
struct Vector2d
{

  Vector2d();
  Vector2d( const Vector2d< T >& other );
  Vector2d( T theX, T theY );

  Vector2d< T >& operator = ( const Vector2d< T >& other );

  Vector2d< T >& operator += ( const Vector2d< T >& other );
  Vector2d< T >& operator -= ( const Vector2d< T >& other );
  Vector2d< T >& operator *= ( T value );
  Vector2d< T >& operator /= ( T value );

  bool operator==( const Vector2d< T >& other ) const;
  bool operator!=( const Vector2d< T >& other ) const;

  T dot( const Vector2d< T >& other ) const;
  double getDoubleNorm() const;
  double getDoubleNorm2() const;
  T getNorm() const;
  T getNorm2() const;
  Vector2d< T >& normalize();

  T x;
  T y;

};


}


template < class T >
gkg::Vector2d< T >
operator + ( const gkg::Vector2d< T >& thing );

template < class T >
gkg::Vector2d< T >
operator - ( const gkg::Vector2d< T >& thing );

template < class T >
gkg::Vector2d< T >
operator + ( const gkg::Vector2d< T >& thing1,
             const gkg::Vector2d< T >& thing2 );

template < class T >
gkg::Vector2d< T >
operator - ( const gkg::Vector2d< T >& thing1,
             const gkg::Vector2d< T >& thing2 );

template < class T >
gkg::Vector2d< T >
operator * ( const gkg::Vector2d< T >& thing1,
             const T& thing2 );

template < class T >
gkg::Vector2d< T >
operator / ( const gkg::Vector2d< T >& thing1,
             const T& thing2 );


namespace std
{

/*
bool operator == ( const gkg::Vector2d< int16_t >& thing1,
                   const gkg::Vector2d< int16_t >& thing2 );
bool operator == ( const gkg::Vector2d< int32_t >& thing1,
                   const gkg::Vector2d< int32_t >& thing2 );
bool operator == ( const gkg::Vector2d< int64_t >& thing1,
                   const gkg::Vector2d< int64_t >& thing2 );
bool operator == ( const gkg::Vector2d< uint16_t >& thing1,
                   const gkg::Vector2d< uint16_t >& thing2 );
bool operator == ( const gkg::Vector2d< uint32_t >& thing1,
                   const gkg::Vector2d< uint32_t >& thing2 );
bool operator == ( const gkg::Vector2d< uint64_t >& thing1,
                   const gkg::Vector2d< uint64_t >& thing2 );
bool operator == ( const gkg::Vector2d< float >& thing1,
                   const gkg::Vector2d< float >& thing2 );
bool operator == ( const gkg::Vector2d< double >& thing1,
                   const gkg::Vector2d< double >& thing2 );
*/

ostream& operator<<( ostream& os,
                     const gkg::Vector2d< int16_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< int32_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< int64_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< uint16_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< uint32_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< uint64_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< float >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector2d< double >& thing );


istream& operator>>( istream& is,
                     gkg::Vector2d< int16_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< int32_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< int64_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< uint16_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< uint32_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< uint64_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< float >& thing );
istream& operator>>( istream& is,
                     gkg::Vector2d< double >& thing );


}


RegisterTemplateType( gkg::Vector2d, gkg_Vector2d );


#endif
