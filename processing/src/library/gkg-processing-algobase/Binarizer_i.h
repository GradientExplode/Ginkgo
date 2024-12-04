#ifndef _gkg_processing_algobase_Binarizer_i_h_
#define _gkg_processing_algobase_Binarizer_i_h_


#include <gkg-processing-algobase/Binarizer.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/VolumeBinarizerContext_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class Binarizer< IN, OUT >
//

template < class IN, class OUT >
inline
gkg::Binarizer< IN, OUT >::Binarizer(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background )
                          : _testFunction( testFunction ),
                            _foreground( foreground ),
                            _background( background )
{
}


template < class IN, class OUT >
inline
void gkg::Binarizer< IN, OUT >::binarize( const IN& in, OUT& out ) const
{

  out = ( _testFunction.getState( in ) ? _foreground : _background );

}


//
// class Binarizer< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
gkg::Binarizer< std::vector< IN >, std::vector< OUT > >::Binarizer(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background )
                                                 : _itemBinarizer( testFunction,
                                                                   foreground,
                                                                   background )
{
}


template < class IN, class OUT >
inline
void gkg::Binarizer< std::vector< IN >, std::vector< OUT > >::binarize(
                                                 const std::vector< IN >& in,
                                                 std::vector< OUT >& out ) const
{

  try
  {

    reallocate( in, out );

    // converting items
    typename std::vector< IN >::const_iterator i = in.begin();
    typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
    while ( o != oe )
    {

      _itemBinarizer.binarize( *i, *o );
      ++ i;
      ++ o;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< std::vector< IN >, std::vector< OUT > >::"
             "binarize( "
             "const std::vector< IN >& in, "
             "std::vector< OUT >& out ) const" );

}


template < class IN, class OUT >
inline
void gkg::Binarizer< std::vector< IN >, std::vector< OUT > >::reallocate(
                                                 const std::vector< IN >& in,
                                                 std::vector< OUT >& out ) const
{

  try
  {

    if ( ( void* )&out != ( void* )&in )
    {

      // reallocating data
      out = std::vector< OUT >( in.size() );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< std::vector< IN >, std::vector< OUT > >::"
             "reallocate( "
             "const std::vector< IN >& in, "
             "std::vector< OUT >& out ) const" );

}


//
// class Binarizer< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::Binarizer(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background )
                                                 : _itemBinarizer( testFunction,
                                                                   foreground,
                                                                   background )
{
}


template < class IN, class OUT >
inline
void gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::binarize(
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    reallocate( in, out );

    // converting items
    int32_t sizeZ = in.getSizeZ();
    int32_t maxThreadCount = ( in.isMemoryMapped() ||
                               out.isMemoryMapped() ) ? 1 : 0;

    gkg::VolumeBinarizerContext< IN, OUT > context( in, out, _itemBinarizer );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 0, sizeZ, maxThreadCount );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::"
             "binarize( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out ) const" );

}


template < class IN, class OUT >
inline
void gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::reallocate(
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    if ( ( void* )&out != ( void* )&in )
    {

      // reallocating data
      out.reallocate( in.getSizeX(),
                      in.getSizeY(),
                      in.getSizeZ(),
                      in.getSizeT() );

      // duplicating header
      out.getHeader() = in.getHeader();
      out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >::"
             "reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out ) const" );

}


//
// class Binarizer< TextureMap< IN >, TextureMap< OUT > >
//

template < class IN, class OUT >
inline
gkg::Binarizer< gkg::TextureMap< IN >, gkg::TextureMap< OUT > >::Binarizer(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background )
                                                 : _itemBinarizer( testFunction,
                                                                   foreground,
                                                                   background )
{
}


template < class IN, class OUT >
inline
void gkg::Binarizer< gkg::TextureMap< IN >, gkg::TextureMap< OUT > >::binarize(
                                             const gkg::TextureMap< IN >& in,
                                             gkg::TextureMap< OUT >& out ) const
{

  try
  {

    reallocate( in, out );


    // converting items
    typename gkg::TextureMap< IN >::const_iterator i = in.begin();
    typename gkg::TextureMap< OUT >::iterator o = out.begin(), oe = out.end();
    while ( o != oe )
    {

      _itemBinarizer.binarize( i->second, o->second );
      ++ i;
      ++ o;

    }


  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< gkg::TextureMap< IN >, "
             "gkg::TextureMap< OUT > >::"
             "binarize( "
             "const gkg::TextureMap< IN >& in, "
             "gkg::TextureMap< OUT >& out ) const" );

}


template < class IN, class OUT >
inline
void gkg::Binarizer< gkg::TextureMap< IN >,
                     gkg::TextureMap< OUT > >::reallocate(
                                             const gkg::TextureMap< IN >& in,
                                             gkg::TextureMap< OUT >& out ) const
{

  try
  {

    if ( ( void* )&out != ( void* )&in )
    {

      // reallocating data
      std::vector< OUT > textures( in.getCount(), ( OUT )0 );
      out.setTextures( textures );

      // duplicating header
      out.getHeader() = in.getHeader();
      out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::Binarizer< gkg::TextureMap< IN >, "
             "gkg::TextureMap< OUT > >::"
             "reallocate( "
             "const gkg::TextureMap< IN >& in, "
             "gkg::TextureMap< OUT >& out ) const" );

}


#endif
