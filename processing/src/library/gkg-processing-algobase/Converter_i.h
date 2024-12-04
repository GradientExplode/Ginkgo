#ifndef _gkg_processing_algobase_Converter_i_h_
#define _gkg_processing_algobase_Converter_i_h_


#include <gkg-processing-algobase/Converter.h>
#include <gkg-processing-algobase/VolumeConverterContext_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>


//
// class Converter< IN, OUT >
//

template < class IN, class OUT >
inline
void gkg::Converter< IN, OUT >::convert( const IN& in, OUT& out ) const
{

  out = ( OUT )in;

}


//
// class Converter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
void gkg::Converter< std::vector< IN >, std::vector< OUT > >::convert(
                    const std::vector< IN >& in, std::vector< OUT >& out ) const
{

  out = std::vector< OUT >( in.size() );

  // converting items
  gkg::Converter< IN, OUT > itemConverter;

  typename std::vector< IN >::const_iterator i = in.begin();
  typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemConverter.convert( *i, *o );
    ++ i;
    ++ o;

  }

}


// when type is the same, we just do a copy
template < class T >
inline
void gkg::Converter< std::vector< T >, std::vector< T > >::convert(
                       const std::vector< T >& in, std::vector< T >& out ) const
{

  out = in;

}


//
// class Converter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
void gkg::Converter< gkg::Volume< IN >, gkg::Volume< OUT > >::convert(
                    const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const
{

  out.reallocate( in.getSizeX(),
                  in.getSizeY(),
                  in.getSizeZ(),
                  in.getSizeT() );

  // duplicating header
  out.getHeader() = in.getHeader();
  out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  // converting items
  int32_t sizeZ = in.getSizeZ();
  int32_t maxThreadCount = ( in.isMemoryMapped() ||
                             out.isMemoryMapped() ) ? 1 : 0;
  gkg::VolumeConverterContext< IN, OUT > context( in, out );
  gkg::ThreadedLoop< int32_t >
    threadedLoop( &context, 0, sizeZ, maxThreadCount );

  threadedLoop.launch( true, true );

}


// when type is the same, we just do a copy
template < class T >
inline
void gkg::Converter< gkg::Volume< T >, gkg::Volume< T > >::convert(
                       const gkg::Volume< T >& in, gkg::Volume< T >& out ) const
{

  out = in;

}


#endif
