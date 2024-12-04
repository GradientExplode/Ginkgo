#include <gkg-processing-container/Polygon.h>
#include <string>
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


template < uint32_t D >
gkg::Polygon< D >::Polygon()
{

  uint32_t d;
  for ( d = 0; d < D; d++ )
  {

    indices[ d ] = 0;

  }

}


template < uint32_t D >
gkg::Polygon< D >::Polygon( uint32_t indice1,
                            uint32_t indice2,
                            uint32_t indice3,
                            uint32_t indice4 )
{

  indices[ 0 ] = indice1;
  indices[ 1 ] = indice2;
  if ( D > 2 )
  {

    indices[ 2 ] = indice3;

  }
  if ( D > 3 )
  {

    indices[ 3 ] = indice4;

  }

}


template < uint32_t D >
gkg::Polygon< D >::Polygon( const uint32_t* values )
{

  uint32_t d;
  for ( d = 0; d < D; d++ )
  {

    indices[ d ] = values[ d ];

  }

}


template < uint32_t D >
gkg::Polygon< D >::Polygon( const gkg::Polygon< D >& other )
{

  uint32_t d;
  for ( d = 0; d < D; d++ )
  {

    indices[ d ] = other.indices[ d ];

  }

}


template < uint32_t D >
gkg::Polygon< D >& 
gkg::Polygon< D >::operator = ( const gkg::Polygon< D >& other )
{

  uint32_t d;
  for ( d = 0; d < D; d++ )
  {

    indices[ d ] = other.indices[ d ];

  }
  return *this;

}


template < uint32_t D >
bool gkg::Polygon< D >::operator == ( const gkg::Polygon< D >& other ) const
{

  uint32_t d;
  for ( d = 0; d < D; d++ )
  {

    if ( indices[ d ] != other.indices[ d ] )
    {

      return false;

    }

  }
  return true;

}


template < uint32_t D >
bool gkg::Polygon< D >::operator != ( const gkg::Polygon< D >& other ) const
{

  return !this->operator==( other );

}


template < uint32_t D >
template < class S >
S gkg::Polygon< D >::getSurfaceArea( 
                   const std::vector< gkg::Vector3d< S > >& vertices ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );


  }
  GKG_CATCH( "template < uint32_t D > "
             "template < class S > "
             "float gkg::Polygon< D >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< S > >& vertices ) const" );

}



template<>
template<>
float gkg::Polygon< 2U >::getSurfaceArea( 
               const std::vector< gkg::Vector3d< float > >& /*vertices*/ ) const
{

  try
  {

    return 0.0f;

  }
  GKG_CATCH( "float gkg::Polygon< 2U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< float > >& vertices ) const" );

}


template<>
template<>
double gkg::Polygon< 2U >::getSurfaceArea( 
              const std::vector< gkg::Vector3d< double > >& /*vertices*/ ) const
{

  try
  {

    return 0.0;

  }
  GKG_CATCH( "double gkg::Polygon< 2U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< double > >& vertices ) const" );

}


template<>
template<>
float gkg::Polygon< 3U >::getSurfaceArea( 
                   const std::vector< gkg::Vector3d< float > >& vertices ) const
{

  try
  {

    double side01 = ( vertices[ indices[ 1 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side12 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 1 ] ] ).getDoubleNorm();
    double side20 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();

    double s = ( side01 + side12 + side20 ) / 2.0;
    // Heron's formula
    return ( float )std::sqrt( s *
                               ( s - side01 ) *
                               ( s - side12 ) *
                               ( s - side20 ) );

  }
  GKG_CATCH( "float gkg::Polygon< 3U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< float > >& vertices ) const" );

}


template<>
template<>
double gkg::Polygon< 3U >::getSurfaceArea( 
                  const std::vector< gkg::Vector3d< double > >& vertices ) const
{

  try
  {

    double side01 = ( vertices[ indices[ 1 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side12 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 1 ] ] ).getDoubleNorm();
    double side20 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();

    double s = ( side01 + side12 + side20 ) / 2.0;
    // Heron's formula
    return ( double )std::sqrt( s *
                                ( s - side01 ) *
                                ( s - side12 ) *
                                ( s - side20 ) );

  }
  GKG_CATCH( "float gkg::Polygon< 3U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< double > >& vertices ) const" );

}


template<>
template<>
float gkg::Polygon< 4U >::getSurfaceArea( 
                   const std::vector< gkg::Vector3d< float > >& vertices ) const
{

  try
  {

    double side01 = ( vertices[ indices[ 1 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side12 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 1 ] ] ).getDoubleNorm();
    double side20 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side03 = ( vertices[ indices[ 3 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side32 = ( vertices[ indices[ 3 ] ] -
                      vertices[ indices[ 2 ] ] ).getDoubleNorm();

    double s012 = ( side01 + side12 + side20 ) / 2.0;
    double s032 = ( side03 + side32 + side20 ) / 2.0;

    // Heron's formula
    return ( float )( std::sqrt( s012 * 
                                 ( s012 - side01 ) *
                                 ( s012 - side12 ) *
                                 ( s012 - side20 ) ) +
                      std::sqrt( s032 *
                                 ( s032 - side03 ) *
                                 ( s032 - side32 ) *
                                 ( s032 - side20 ) ) );

  }
  GKG_CATCH( "float gkg::Polygon< 4U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< float > >& vertices ) const" );

}


template<>
template<>
double gkg::Polygon< 4U >::getSurfaceArea( 
                  const std::vector< gkg::Vector3d< double > >& vertices ) const
{

  try
  {

    double side01 = ( vertices[ indices[ 1 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side12 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 1 ] ] ).getDoubleNorm();
    double side20 = ( vertices[ indices[ 2 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side03 = ( vertices[ indices[ 3 ] ] -
                      vertices[ indices[ 0 ] ] ).getDoubleNorm();
    double side32 = ( vertices[ indices[ 3 ] ] -
                      vertices[ indices[ 2 ] ] ).getDoubleNorm();

    double s012 = ( side01 + side12 + side20 ) / 2.0;
    double s032 = ( side03 + side32 + side20 ) / 2.0;

    // Heron's formula
    return ( double )( std::sqrt( s012 * 
                                  ( s012 - side01 ) *
                                  ( s012 - side12 ) *
                                  ( s012 - side20 ) ) +
                       std::sqrt( s032 *
                                  ( s032 - side03 ) *
                                  ( s032 - side32 ) *
                                  ( s032 - side20 ) ) );

  }
  GKG_CATCH( "double gkg::Polygon< 4U >::getSurfaceArea( "
             "const std::vector< gkg::Vector3d< double > >& vertices ) const" );

}





/*
bool std::operator == ( const gkg::Polygon< 2U >& thing1,
                        const gkg::Polygon< 2U >& thing2 )
{

  return ( thing1.indices[ 0 ] == thing2.indices[ 0 ] ) &&
         ( thing1.indices[ 1 ] == thing2.indices[ 1 ] );

}


bool std::operator == ( const gkg::Polygon< 3U >& thing1,
                        const gkg::Polygon< 3U >& thing2 )
{

  return ( thing1.indices[ 0 ] == thing2.indices[ 0 ] ) &&
         ( thing1.indices[ 1 ] == thing2.indices[ 1 ] ) &&
         ( thing1.indices[ 2 ] == thing2.indices[ 2 ] );

}


bool std::operator == ( const gkg::Polygon< 4U >& thing1,
                        const gkg::Polygon< 4U >& thing2 )
{

  return ( thing1.indices[ 0 ] == thing2.indices[ 0 ] ) &&
         ( thing1.indices[ 1 ] == thing2.indices[ 1 ] ) &&
         ( thing1.indices[ 2 ] == thing2.indices[ 2 ] ) &&
         ( thing1.indices[ 3 ] == thing2.indices[ 3 ] );

}
*/

std::ostream& std::operator << ( std::ostream& os, const gkg::Polygon< 2U >& thing )
{


  os << "( "
     << thing.indices[ 0 ] << ", "
     << thing.indices[ 1 ]
     << " )";
             
  return os;

}


std::ostream& std::operator << ( std::ostream& os, const gkg::Polygon< 3U >& thing )
{


  os << "( "
     << thing.indices[ 0 ] << ", "
     << thing.indices[ 1 ] << ", "
     << thing.indices[ 2 ]
     << " )";
             
  return os;

}


std::ostream& std::operator << ( std::ostream& os, const gkg::Polygon< 4U >& thing )
{


  os << "( "
     << thing.indices[ 0 ] << ", "
     << thing.indices[ 1 ] << ", "
     << thing.indices[ 2 ] << ", "
     << thing.indices[ 3 ]
     << " )";
             
  return os;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Polygon< 2U >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.indices[ 0 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 1 ];
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Polygon< 3U >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.indices[ 0 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 1 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 2 ];
  skip( is );
  is >> character;

  return is;

}


std::istream& std::operator>>( std::istream& is,
                               gkg::Polygon< 4U >& thing )
{

  char character = 0;

  skip( is );

  is >> character; // reading '('
  skip( is );
  is >> thing.indices[ 0 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 1 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 2 ];
  skip( is );
  is >> character; // reading ','
  skip( is );
  is >> thing.indices[ 3 ];
  skip( is );
  is >> character;

  return is;

}


template struct gkg::Polygon< 2U >;
template struct gkg::Polygon< 3U >;
template struct gkg::Polygon< 4U >;

