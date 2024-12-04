#ifndef _gkg_processing_algobase_MagnitudeConverter_i_h_
#define _gkg_processing_algobase_MagnitudeConverter_i_h_


#include <gkg-processing-algobase/MagnitudeConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <cmath>


//
// class MagnitudeConverter< IN, OUT >
//

template < class IN, class OUT >
inline
void gkg::MagnitudeConverter< IN, OUT >::convert( const IN& in, OUT& out ) const
{

  out = ( OUT )std::abs( in );

}


template < class IN, class OUT >
inline
void gkg::MagnitudeConverter< std::complex< IN >, OUT >::convert(
                                  const std::complex< IN >& in, OUT& out ) const
{

  out = ( OUT )std::sqrt( std::norm( std::complex< double >( in.real(),
                                                             in.imag() ) ) );

}


//
// class MagnitudeConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
void gkg::MagnitudeConverter< std::vector< IN >, std::vector< OUT > >::convert(
                    const std::vector< IN >& in, std::vector< OUT >& out ) const
{

  out = std::vector< OUT >( in.size() );

  // converting items
  gkg::MagnitudeConverter< IN, OUT > itemMagnitudeConverter;

  typename std::vector< IN >::const_iterator i = in.begin();
  typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemMagnitudeConverter.convert( *i, *o );
    ++ i;
    ++ o;

  }

}


//
// class MagnitudeConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
void gkg::MagnitudeConverter< gkg::Volume< IN >, gkg::Volume< OUT > >::convert(
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
  gkg::MagnitudeConverter< IN, OUT > itemMagnitudeConverter;

  typename gkg::Volume< IN >::const_iterator i = in.begin();
  typename gkg::Volume< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemMagnitudeConverter.convert( *i, *o );
    ++ i;
    ++ o;

  }

}


#endif
