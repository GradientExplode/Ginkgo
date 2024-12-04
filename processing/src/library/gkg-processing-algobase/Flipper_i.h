#ifndef _gkg_processing_algobase_Flipper_i_h_
#define _gkg_processing_algobase_Flipper_i_h_


#include <gkg-processing-algobase/Flipper.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


//
// class Flipper< T >
//

template < class T >
inline
gkg::Flipper< T >::Flipper( const std::list< gkg::Flipper< T >::Type >& types )
                  : _types( types )
{
}


template < class T >
inline
gkg::Flipper< T >::~Flipper()
{
}


template < class T >
inline
void gkg::Flipper< T >::flip( const T& in, T& out ) const
{

  out = in;

}


//
// class Flipper< Volume< T > >
//

template < class T >
inline
gkg::Flipper< gkg::Volume< T > >::Flipper( 
              const std::list< gkg::Flipper< gkg::Volume< T > >::Type >& types )
                  : _types( types )
{
}


template < class T >
inline
gkg::Flipper< gkg::Volume< T > >::~Flipper()
{
}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flip( const gkg::Volume< T >& in,
                                             gkg::Volume< T >& out ) const
{

  try
  {

    typename std::list< typename gkg::Flipper< gkg::Volume< T > >::Type >::const_iterator
      t = _types.begin(),
      te = _types.end();

    bool firstStep = true;

    gkg::Volume< T > temporary1;
    gkg::Volume< T > temporary2;

    const gkg::Volume< T >* input = &in;
    gkg::Volume< T >* output = &temporary1;

    while ( t != te )
    {

      switch ( *t )
      {

        case gkg::Flipper< gkg::Volume< T > >::SymmetryAlongX:

          symmetryAlongX( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::SymmetryAlongY:

          symmetryAlongY( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::SymmetryAlongZ:

          symmetryAlongZ( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::SymmetryAlongT:

          symmetryAlongT( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingXY:

          flippingXY( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingXZ:

          flippingXZ( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingXT:

          flippingXT( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingYZ:

          flippingYZ( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingYT:

          flippingYT( *input, *output );
          break;

        case gkg::Flipper< gkg::Volume< T > >::FlippingZT:

          flippingZT( *input, *output );
          break;

      }
      if ( firstStep )
      {

        input = output;
        output = &temporary2;
        firstStep = false;

      }
      else
      {

        input = output;
        output = ( output == &temporary1 ? &temporary2 : &temporary1 );

      }
      ++ t;

    }

    out = *input;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< T >::flip( const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::symmetryAlongX( 
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( sizeX - 1 - x, y, z, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::symmetryAlongX( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::symmetryAlongY( 
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, sizeY - 1 - y, z, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::symmetryAlongY( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::symmetryAlongZ( 
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, y, sizeZ - 1 - z, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::symmetryAlongZ( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::symmetryAlongT( 
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, y, z, sizeT - 1 - t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::symmetryAlongT( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingXY( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeY(),
                    in.getSizeX(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeX" ] = out.getSizeX();
    out.getHeader()[ "sizeY" ] = out.getSizeY();

    if ( out.getHeader().hasAttribute( "resolutionX" ) &&
         out.getHeader().hasAttribute( "resolutionY" ) )
    {

      std::swap( out.getHeader()[ "resolutionX" ],
                 out.getHeader()[ "resolutionY" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( y, x, z, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingXY( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingXZ( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeZ(),
                    in.getSizeY(),
                    in.getSizeX(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeX" ] = out.getSizeX();
    out.getHeader()[ "sizeZ" ] = out.getSizeZ();

    if ( out.getHeader().hasAttribute( "resolutionX" ) &&
         out.getHeader().hasAttribute( "resolutionZ" ) )
    {

      std::swap( out.getHeader()[ "resolutionX" ],
                 out.getHeader()[ "resolutionZ" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( z, y, x, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingXZ( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingXT( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeT(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeX() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeX" ] = out.getSizeX();
    out.getHeader()[ "sizeT" ] = out.getSizeT();

    if ( out.getHeader().hasAttribute( "resolutionX" ) &&
         out.getHeader().hasAttribute( "resolutionT" ) )
    {

      std::swap( out.getHeader()[ "resolutionX" ],
                 out.getHeader()[ "resolutionT" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( t, y, z, x );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingXT( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingYZ( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeZ(),
                    in.getSizeY(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeY" ] = out.getSizeY();
    out.getHeader()[ "sizeZ" ] = out.getSizeZ();

    if ( out.getHeader().hasAttribute( "resolutionY" ) &&
         out.getHeader().hasAttribute( "resolutionZ" ) )
    {

      std::swap( out.getHeader()[ "resolutionY" ],
                 out.getHeader()[ "resolutionZ" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, z, y, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingYZ( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingYT( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeT(),
                    in.getSizeZ(),
                    in.getSizeY() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeY" ] = out.getSizeY();
    out.getHeader()[ "sizeT" ] = out.getSizeT();

    if ( out.getHeader().hasAttribute( "resolutionY" ) &&
         out.getHeader().hasAttribute( "resolutionT" ) )
    {

      std::swap( out.getHeader()[ "resolutionY" ],
                 out.getHeader()[ "resolutionT" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, t, z, y );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingYT( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Flipper< gkg::Volume< T > >::flippingZT( const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    // reallocating
    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeT(),
                    in.getSizeZ() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "sizeZ" ] = out.getSizeZ();
    out.getHeader()[ "sizeT" ] = out.getSizeT();

    if ( out.getHeader().hasAttribute( "resolutionZ" ) &&
         out.getHeader().hasAttribute( "resolutionT" ) )
    {

      std::swap( out.getHeader()[ "resolutionZ" ],
                 out.getHeader()[ "resolutionT" ] );

    }

    int32_t sizeX = out.getSizeX();
    int32_t sizeY = out.getSizeY();
    int32_t sizeZ = out.getSizeZ();
    int32_t sizeT = out.getSizeT();

    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            out( x, y, z, t ) = in( x, y, t, z );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Flipper< gkg::Volume< T > >::flippingZT( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


#endif


