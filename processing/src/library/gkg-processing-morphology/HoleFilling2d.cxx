#include <gkg-processing-morphology/HoleFilling2d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>


template < class T >
gkg::HoleFilling2d< T >::HoleFilling2d( const T& foreground,
                                        const T& background )
                        : _foreground( foreground ),
                          _background( background )
{
}


template < class T >
gkg::HoleFilling2d< T >::~HoleFilling2d()
{
}


template < class T >
void gkg::HoleFilling2d< T >::fill( const gkg::Volume< T >& volume,
                                    gkg::Volume< T >& filled ) const
{

  try
  {

    gkg::Volume< T > tmpVolume( volume );
    reallocate( volume, filled );

    filled.fill( _foreground );

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    int32_t x, y, z, t;
    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( ( tmpVolume( x, 0, z, t ) == _background ) &&
               ( filled( x, 0, z, t ) == _foreground ) )
          {

            propagate( tmpVolume, filled, x, 0, z, t );

          }

          if ( ( tmpVolume( x, sizeY - 1, z, t ) == _background ) &&
               ( filled( x, sizeY - 1, z, t ) == _foreground ) )
          {

            propagate( tmpVolume, filled, x, 0, z, t );

          }

        }

        for ( y = 0; y < sizeY; y++ )
        {

          if ( ( tmpVolume( 0, y, z, t ) == _background ) &&
               ( filled( 0, y, z, t ) == _foreground ) )
          {

            propagate( tmpVolume, filled, 0, y, z, t );

          }

          if ( ( tmpVolume( sizeX - 1, y, z, t ) == _background ) &&
               ( filled( sizeX - 1, y, z, t ) == _foreground ) )
          {

            propagate( tmpVolume, filled, 0, y, z, t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HoleFilling2d< T >::fill( "
             "const gkg::Volume< T >& volume, "
             "gkg::Volume< T >& filled ) const" );

}


template < class T >
void gkg::HoleFilling2d< T >::reallocate( const gkg::Volume< T >& in,
                                          gkg::Volume< T >& out ) const
{

  try
  {

    if ( &in != &out )
    {

      // reallocating data
      out.reallocate( in.getSizeX(),
                      in.getSizeY(),
                      in.getSizeZ(),
                      in.getSizeT() );

      // duplicating header
      out.getHeader() = in.getHeader();

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HoleFilling2d< T >::reallocate( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out )" );

}


template < class T >
void gkg::HoleFilling2d< T >::propagate(
                             const gkg::Volume< T >& volume,
                             gkg::Volume< T >& filled,
                             int32_t x, int32_t y, int32_t z, int32_t t ) const
{

  try
  {

    if ( ( x >= 0 ) && ( x < volume.getSizeX() ) &&
         ( y >= 0 ) && ( y < volume.getSizeY() ) &&
         ( filled( x, y, z, t ) == _foreground ) &&
         ( volume( x, y, z, t ) == _background ) )
    {

      filled( x, y, z, t ) = _background;

      propagate( volume, filled, x, y - 1, z, t );
      propagate( volume, filled, x, y + 1, z, t );
      propagate( volume, filled, x - 1, y, z, t );
      propagate( volume, filled, x + 1, y, z, t );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HoleFilling2d< T >::propagate( "
             "const gkg::Volume< T >& volume, "
             "gkg::Volume< T >& filled, "
             "int x, int y, int z, int t ) const" );

}


// force instanciation for standard types

template class gkg::HoleFilling2d< int8_t >;
template class gkg::HoleFilling2d< uint8_t >;
template class gkg::HoleFilling2d< int16_t >;
template class gkg::HoleFilling2d< uint16_t >;
template class gkg::HoleFilling2d< int32_t >;
template class gkg::HoleFilling2d< uint32_t >;
template class gkg::HoleFilling2d< int64_t >;
template class gkg::HoleFilling2d< uint64_t >;
template class gkg::HoleFilling2d< float >;
template class gkg::HoleFilling2d< double >;
