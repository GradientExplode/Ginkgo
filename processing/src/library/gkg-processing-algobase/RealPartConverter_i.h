#ifndef _gkg_processing_algobase_RealPartConverter_i_h_
#define _gkg_processing_algobase_RealPartConverter_i_h_


#include <gkg-processing-algobase/RealPartConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>


//
// class RealPartConverter< IN, OUT >
//

template < class IN, class OUT >
inline
void gkg::RealPartConverter< IN, OUT >::convert( const IN& in, OUT& out ) const
{

  out = ( OUT )in;

}


template < class IN, class OUT >
inline
void gkg::RealPartConverter< std::complex< IN >, OUT >::convert(
                                  const std::complex< IN >& in, OUT& out ) const
{

  out = ( OUT )std::real( in );

}


//
// class RealPartConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
void gkg::RealPartConverter< std::vector< IN >, std::vector< OUT > >::convert(
                    const std::vector< IN >& in, std::vector< OUT >& out ) const
{

  out = std::vector< OUT >( in.size() );

  // converting items
  gkg::RealPartConverter< IN, OUT > itemRealPartConverter;

  typename std::vector< IN >::const_iterator i = in.begin();
  typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemRealPartConverter.convert( *i, *o );
    ++ i;
    ++ o;

  }

}


//
// class RealPartConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
void gkg::RealPartConverter< gkg::Volume< IN >, gkg::Volume< OUT > >::convert(
                    const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const
{

  out = gkg::Volume< OUT >( in.getSizeX(),
                            in.getSizeY(),
                            in.getSizeZ(),
                            in.getSizeT() );

  // duplicating header
  out.getHeader() = in.getHeader();
  out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  // converting items
  gkg::RealPartConverter< IN, OUT > itemRealPartConverter;

  typename gkg::Volume< IN >::const_iterator i = in.begin();
  typename gkg::Volume< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemRealPartConverter.convert( *i, *o );
    ++ i;
    ++ o;

  }

}


#endif
