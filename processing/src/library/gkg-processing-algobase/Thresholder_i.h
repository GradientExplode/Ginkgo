#ifndef _gkg_processing_algobase_Thresholder_i_h_
#define _gkg_processing_algobase_Thresholder_i_h_


#include <gkg-processing-algobase/Thresholder.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/VolumeThresholderContext_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class Thresholder< T >
//

template < class T >
inline
gkg::Thresholder< T >::Thresholder( const gkg::TestFunction< T >& testFunction,
                                    const T& background )
                      : _testFunction( testFunction ),
                        _background( background )
{
}


template < class T >
inline
void gkg::Thresholder< T >::threshold( const T& in, T& out ) const
{

  out = ( _testFunction.getState( in ) ? in : _background );

}


//
// class Thresholder< std::vector< T > >
//

template < class T >
inline
gkg::Thresholder< std::vector< T > >::Thresholder(
                                     const gkg::TestFunction< T >& testFunction,
                                     const T& background )
                                     : _itemThresholder( testFunction,
                                                         background )
{
}


template < class T >
inline
void gkg::Thresholder< std::vector< T > >::threshold(
                                                   const std::vector< T >& in,
                                                   std::vector< T >& out ) const
{

  try
  {

    reallocate( in, out );

    // converting items
    typename std::vector< T >::const_iterator i = in.begin();
    typename std::vector< T >::iterator o = out.begin(), oe = out.end();
    while ( o != oe )
    {

      _itemThresholder.threshold( *i, *o );
      ++ i;
      ++ o;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Thresholder< std::vector< T > >::threshold( "
             "const std::vector< T >& in, "
             "std::vector< T >& out ) const" );

}


template < class T >
inline
void gkg::Thresholder< std::vector< T > >::reallocate(
                                                   const std::vector< T >& in,
                                                   std::vector< T >& out ) const
{

  try
  {

    if ( &out != &in )
    {

      // reallocating data
      out = std::vector< T >( in.size() );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Thresholder< std::vector< T > >::reallocate( "
             "const std::vector< T >& in, "
             "std::vector< T >& out ) const" );

}


//
// class Thresholder< Volume< T > >
//

template < class T >
inline
gkg::Thresholder< gkg::Volume< T > >::Thresholder(
                                     const gkg::TestFunction< T >& testFunction,
                                     const T& background )
                                     : _itemThresholder( testFunction,
                                                         background )
{
}


template < class T >
inline
void gkg::Thresholder< gkg::Volume< T > >::threshold(
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    reallocate( in, out );

    // converting items
    int32_t sizeZ = in.getSizeZ();
    int32_t maxThreadCount = ( in.isMemoryMapped() ||
                               out.isMemoryMapped() ) ? 1 : 0;

    gkg::VolumeThresholderContext< T > context( in, out, _itemThresholder );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 0, sizeZ, maxThreadCount );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Thresholder< gkg::Volume< T > >::threshold( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


template < class T >
inline
void gkg::Thresholder< gkg::Volume< T > >::reallocate(
                                                   const gkg::Volume< T >& in,
                                                   gkg::Volume< T >& out ) const
{

  try
  {

    if ( &out != &in )
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
             "inline "
             "void gkg::Thresholder< gkg::Volume< T > >::reallocate( "
             "const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out ) const" );

}


//
// class Thresholder< TextureMap< T > >
//

template < class T >
inline
gkg::Thresholder< gkg::TextureMap< T > >::Thresholder(
                                     const gkg::TestFunction< T >& testFunction,
                                     const T& background )
                                     : _itemThresholder( testFunction,
                                                         background )
{
}


template < class T >
inline
void gkg::Thresholder< gkg::TextureMap< T > >::threshold(
                                               const gkg::TextureMap< T >& in,
                                               gkg::TextureMap< T >& out ) const
{

  try
  {

    reallocate( in, out );

    // converting items
    typename gkg::TextureMap< T >::const_iterator i = in.begin();
    typename gkg::TextureMap< T >::iterator o = out.begin(), oe = out.end();
    while ( o != oe )
    {

      _itemThresholder.threshold( i->second, o->second );
      ++ i;
      ++ o;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Thresholder< gkg::TextureMap< T > >::threshold( "
             "const gkg::TextureMap< T >& in, "
             "gkg::TextureMap< T >& out ) const" );

}


template < class T >
inline
void gkg::Thresholder< gkg::TextureMap< T > >::reallocate(
                                               const gkg::TextureMap< T >& in,
                                               gkg::TextureMap< T >& out ) const
{

  try
  {

    if ( &out != &in )
    {

      // reallocating data
      out = in;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Thresholder< gkg::TextureMap< T > >::reallocate( "
             "const gkg::TextureMap< T >& in, "
             "gkg::TextureMap< T >& out ) const" );

}


#endif
