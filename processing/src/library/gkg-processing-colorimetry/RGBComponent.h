#ifndef _gkg_processing_colorimetry_RGBComponent_h_
#define _gkg_processing_colorimetry_RGBComponent_h_

#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-io/TypeOf.h>
#include <iostream>
#include <functional>


namespace gkg
{


struct RGBComponent
{

  RGBComponent( uint8_t theR = 0U,
                uint8_t theG = 0U,
                uint8_t theB = 0U );
  RGBComponent( const RGBComponent& other );

  RGBComponent& operator = ( const RGBComponent& other );
                
  RGBComponent& operator += ( const RGBComponent& other );
  RGBComponent& operator -= ( const RGBComponent& other );
  RGBComponent& operator *= ( float value );
  RGBComponent& operator /= ( float value );
  RGBComponent& operator *= ( double value );
  RGBComponent& operator /= ( double value );

  uint8_t r;
  uint8_t g;
  uint8_t b;

};


struct RGBComponentCompare /*: public std::binary_function< RGBComponent,
                                                          RGBComponent,
                                                          bool >
                             deprecated in c++17 */
{

  typedef RGBComponent first_argument_type;
  typedef RGBComponent second_argument_type;
  typedef bool result_type;

  bool operator()( const RGBComponent& v1, const RGBComponent& v2 ) const
  {

    return ( v1.b < v2.b ) ||
           ( ( v1.b == v2.b  ) && ( ( v1.g < v2.g ) ||
                                    ( ( v1.g == v2.g ) && ( v1.r < v2.r ) ) ) );
  }

};


}


gkg::RGBComponent
operator + ( const gkg::RGBComponent& thing );

gkg::RGBComponent
operator + ( const gkg::RGBComponent& thing1,
             const gkg::RGBComponent& thing2 );

gkg::RGBComponent
operator - ( const gkg::RGBComponent& thing1,
             const gkg::RGBComponent& thing2 );

gkg::RGBComponent
operator * ( const gkg::RGBComponent& thing1,
             const float& thing2 );

gkg::RGBComponent
operator * ( const float& thing1,
             const gkg::RGBComponent& thing2 );

gkg::RGBComponent
operator / ( const gkg::RGBComponent& thing1,
             const float& thing2 );

gkg::RGBComponent
operator / ( const float& thing1,
             const gkg::RGBComponent& thing2 );

gkg::RGBComponent
operator * ( const gkg::RGBComponent& thing1,
             const double& thing2 );

gkg::RGBComponent
operator * ( const double& thing1,
             const gkg::RGBComponent& thing2 );

gkg::RGBComponent
operator / ( const gkg::RGBComponent& thing1,
             const double& thing2 );

gkg::RGBComponent
operator / ( const double& thing1,
             const gkg::RGBComponent& thing2 );



namespace std
{


bool operator == ( const gkg::RGBComponent& thing1,
                   const gkg::RGBComponent& thing2 );

ostream& operator<<( ostream& os,
                     const gkg::RGBComponent& thing );

istream& operator>>( istream& is,
                     gkg::RGBComponent& thing );

}


RegisterBaseType( gkg::RGBComponent, gkg_RGBComponent );


#endif
