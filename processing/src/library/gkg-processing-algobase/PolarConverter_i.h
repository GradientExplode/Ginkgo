#ifndef _gkg_processing_algobase_PolarConverter_i_h_
#define _gkg_processing_algobase_PolarConverter_i_h_


#include <gkg-processing-algobase/PolarConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <cmath>


//
// class PolarConverter< IN, OUT >
//

template < class IN, class OUT >
inline
void gkg::PolarConverter< IN, OUT >::convert( const IN& in,
                                              OUT& magnitude, OUT& phase ) const
{

  magnitude = ( OUT )std::fabs( in );
  phase = ( in >= 0 ? ( OUT )0 : ( OUT )M_PI );

}


template < class IN, class OUT >
inline
void gkg::PolarConverter< std::complex< IN >, OUT >::convert(
                                              const std::complex< IN >& in,
                                              OUT& magnitude, OUT& phase ) const
{

  magnitude = ( OUT )std::sqrt( std::norm( in ) );
  phase = ( OUT )std::arg( in );

}


//
// class PolarConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
void gkg::PolarConverter< std::vector< IN >, std::vector< OUT > >::convert(
                                               const std::vector< IN >& in,
                                               std::vector< OUT >& magnitude,
                                               std::vector< OUT >& phase ) const
{

  magnitude = std::vector< OUT >( in.size() );
  phase = std::vector< OUT >( in.size() );

  // converting items
  gkg::PolarConverter< IN, OUT > itemPolarConverter;

  typename std::vector< IN >::const_iterator i = in.begin();
  typename std::vector< OUT >::iterator m = magnitude.begin(),
                                        me = magnitude.end();
  typename std::vector< OUT >::iterator p = phase.begin();
  while ( m != me )
  {

    itemPolarConverter.convert( *i, *m, *p );
    ++ i;
    ++ m;
    ++ p;

  }

}


//
// class PolarConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
void gkg::PolarConverter< gkg::Volume< IN >, gkg::Volume< OUT > >::convert(
                                               const gkg::Volume< IN >& in,
                                               gkg::Volume< OUT >& magnitude,
                                               gkg::Volume< OUT >& phase ) const
{

  magnitude = gkg::Volume< OUT >( in.getSizeX(),
                                  in.getSizeY(),
                                  in.getSizeZ(),
                                  in.getSizeT() );
  phase = gkg::Volume< OUT >( in.getSizeX(),
                              in.getSizeY(),
                              in.getSizeZ(),
                              in.getSizeT() );

  // duplicating header(s)
  magnitude.getHeader() = in.getHeader();
  magnitude.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();
  phase.getHeader() = in.getHeader();
  phase.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  // converting items
  gkg::PolarConverter< IN, OUT > itemPolarConverter;

  typename gkg::Volume< IN >::const_iterator i = in.begin();
  typename gkg::Volume< OUT >::iterator m = magnitude.begin(),
                                        me = magnitude.end();
  typename gkg::Volume< OUT >::iterator p = phase.begin();
  while ( m != me )
  {

    itemPolarConverter.convert( *i, *m, *p );
    ++ i;
    ++ m;
    ++ p;

  }

}


#endif
