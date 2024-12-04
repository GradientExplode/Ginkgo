#ifndef _gkg_processing_coordinates_Vector3d_h_
#define _gkg_processing_coordinates_Vector3d_h_

#include <iostream>
#include <functional>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T >
struct Vector3d
{

  Vector3d();
  Vector3d( const Vector3d< T >& other );
  Vector3d( T theX, T theY, T theZ );

  Vector3d< T >& operator = ( const Vector3d< T >& other );

  Vector3d< T >& operator += ( const Vector3d< T >& other );
  Vector3d< T >& operator -= ( const Vector3d< T >& other );
  Vector3d< T >& operator *= ( T value );
  Vector3d< T >& operator /= ( T value );

  bool operator==( const Vector3d< T >& other ) const;
  bool operator!=( const Vector3d< T >& other ) const;

  T dot( const Vector3d< T >& other ) const;
  Vector3d< T > cross( const Vector3d< T >& other ) const;
  double getDoubleNorm() const;
  double getDoubleNorm2() const;
  T getNorm() const;
  T getNorm2() const;
  Vector3d< T >& normalize();
  Vector3d< T > mainDirection() const;
  Vector3d< T > absoluteCoordinates() const;

  T x;
  T y;
  T z;

};


template < class S >
struct Vector3dCompare /*: public std::binary_function< Vector3d< S >,
                                                      Vector3d< S >,
                                                      bool >
                         deprecated in c++17 */
{

  typedef Vector3d< S > first_argument_type;
  typedef Vector3d< S > second_argument_type;
  typedef bool result_type;

  bool operator()( const Vector3d< S >& v1, const Vector3d< S >& v2 ) const
  {

    return ( v1.z < v2.z ) ||
           ( ( v1.z == v2.z  ) && ( ( v1.y < v2.y ) ||
                                    ( ( v1.y == v2.y ) && ( v1.x < v2.x ) ) ) );
  }

};


}


template < class T >
gkg::Vector3d< T >
operator + ( const gkg::Vector3d< T >& thing );

template < class T >
gkg::Vector3d< T >
operator - ( const gkg::Vector3d< T >& thing );

template < class T >
gkg::Vector3d< T >
operator + ( const gkg::Vector3d< T >& thing1,
             const gkg::Vector3d< T >& thing2 );

template < class T >
gkg::Vector3d< T >
operator - ( const gkg::Vector3d< T >& thing1,
             const gkg::Vector3d< T >& thing2 );

template < class T >
gkg::Vector3d< T >
operator * ( const gkg::Vector3d< T >& thing1,
             const T& thing2 );

template < class T >
gkg::Vector3d< T >
operator / ( const gkg::Vector3d< T >& thing1,
             const T& thing2 );


namespace std
{

/*
bool operator == ( const gkg::Vector3d< int16_t >& thing1,
                   const gkg::Vector3d< int16_t >& thing2 );
bool operator == ( const gkg::Vector3d< int32_t >& thing1,
                   const gkg::Vector3d< int32_t >& thing2 );
bool operator == ( const gkg::Vector3d< int64_t >& thing1,
                   const gkg::Vector3d< int64_t >& thing2 );
bool operator == ( const gkg::Vector3d< uint16_t >& thing1,
                   const gkg::Vector3d< uint16_t >& thing2 );
bool operator == ( const gkg::Vector3d< uint32_t >& thing1,
                   const gkg::Vector3d< uint32_t >& thing2 );
bool operator == ( const gkg::Vector3d< uint64_t >& thing1,
                   const gkg::Vector3d< uint64_t >& thing2 );
bool operator == ( const gkg::Vector3d< float >& thing1,
                   const gkg::Vector3d< float >& thing2 );
bool operator == ( const gkg::Vector3d< double >& thing1,
                   const gkg::Vector3d< double >& thing2 );
*/

ostream& operator<<( ostream& os,
                     const gkg::Vector3d< int16_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< int32_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< int64_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< uint16_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< uint32_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< uint64_t >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< float >& thing );
ostream& operator<<( ostream& os,
                     const gkg::Vector3d< double >& thing );


istream& operator>>( istream& is,
                     gkg::Vector3d< int16_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< int32_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< int64_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< uint16_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< uint32_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< uint64_t >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< float >& thing );
istream& operator>>( istream& is,
                     gkg::Vector3d< double >& thing );


}


RegisterTemplateType( gkg::Vector3d, gkg_Vector3d );


#endif
