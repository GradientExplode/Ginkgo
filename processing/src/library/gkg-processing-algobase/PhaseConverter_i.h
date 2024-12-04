#ifndef _gkg_processing_algobase_PhaseConverter_i_h_
#define _gkg_processing_algobase_PhaseConverter_i_h_


#include <gkg-processing-algobase/PhaseConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <cmath>


//
// class PhaseConverter< IN, OUT >
//

template < class IN, class OUT >
inline
void gkg::PhaseConverter< IN, OUT >::convert( const IN& in, OUT& out,
                                              bool degree ) const
{

  if ( degree )
  {

    out = ( OUT )( in >= 0 ? 0 : 180 );

  }
  else
  {

    out = ( OUT )( in >= 0 ? 0 : M_PI );

  }

}


template < class IN, class OUT >
inline
void gkg::PhaseConverter< std::complex< IN >, OUT >::convert(
                                         const std::complex< IN >& in, OUT& out,
                                         bool degree ) const
{

  if ( degree )
  {

    out = ( OUT )( std::arg( std::complex< double >( in.real(), in.imag() ) ) *
                   180 / M_PI );

  }
  else
  {

    out = ( OUT )std::arg( std::complex< double >( in.real(), in.imag() ) );

  }

}


//
// class PhaseConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
void gkg::PhaseConverter< std::vector< IN >, std::vector< OUT > >::convert(
                           const std::vector< IN >& in, std::vector< OUT >& out,
                           bool degree ) const
{

  out = std::vector< OUT >( in.size() );

  // converting items
  gkg::PhaseConverter< IN, OUT > itemPhaseConverter;

  typename std::vector< IN >::const_iterator i = in.begin();
  typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemPhaseConverter.convert( *i, *o, degree );
    ++ i;
    ++ o;

  }

}


//
// class PhaseConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
void gkg::PhaseConverter< gkg::Volume< IN >, gkg::Volume< OUT > >::convert(
                           const gkg::Volume< IN >& in, gkg::Volume< OUT >& out,
                           bool degree ) const
{

  out = gkg::Volume< OUT >( in.getSizeX(),
                            in.getSizeY(),
                            in.getSizeZ(),
                            in.getSizeT() );

  // duplicating header
  out.getHeader() = in.getHeader();
  out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

  // converting items
  gkg::PhaseConverter< IN, OUT > itemPhaseConverter;

  typename gkg::Volume< IN >::const_iterator i = in.begin();
  typename gkg::Volume< OUT >::iterator o = out.begin(), oe = out.end();
  while ( o != oe )
  {

    itemPhaseConverter.convert( *i, *o, degree );
    ++ i;
    ++ o;

  }

}


#endif
