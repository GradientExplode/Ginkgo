#ifndef _gkg_processing_algobase_RescalerWithSlopeAndIntercept_i_h_
#define _gkg_processing_algobase_RescalerWithSlopeAndIntercept_i_h_


#include <gkg-processing-algobase/RescalerWithSlopeAndIntercept.h>
#include <gkg-processing-algobase/VolumeRescalerWithSlopeAndInterceptContext_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class RescalerWithSlopeAndIntercept< IN, OUT >
//

template < class IN, class OUT >
inline
gkg::RescalerWithSlopeAndIntercept< IN, OUT >::RescalerWithSlopeAndIntercept(
                                                double slope, double intercept )
                                              : _slope( slope ),
                                                _intercept( intercept )
{
}


template < class IN, class OUT >
inline
gkg::RescalerWithSlopeAndIntercept< IN, OUT >::RescalerWithSlopeAndIntercept(
                    const gkg::RescalerWithSlopeAndIntercept< IN, OUT >& other )
                                              : _slope( other._slope ),
                                                _intercept( other._intercept )
{
}


template < class IN, class OUT >
inline
gkg::RescalerWithSlopeAndIntercept< IN, OUT >& 
gkg::RescalerWithSlopeAndIntercept< IN, OUT >::operator=(
                    const gkg::RescalerWithSlopeAndIntercept< IN, OUT >& other )
{

  try
  {

    _slope = other._slope;
    _intercept = other._intercept;

    return *this;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "gkg::RescalerWithSlopeAndIntercept< IN, OUT >& "
             "gkg::RescalerWithSlopeAndIntercept< IN, OUT >::operator=( "
             "const gkg::RescalerWithSlopeAndIntercept< IN, OUT >& other )" );

}


template < class IN, class OUT >
inline
void gkg::RescalerWithSlopeAndIntercept< IN, OUT >::rescale(
                                                  const IN& in, OUT& out ) const
{

  try
  {

    double value = _slope * ( double )in + _intercept;

    if ( value < ( double )std::numeric_limits< OUT >::lowest() )
    {

      out = std::numeric_limits< OUT >::lowest();

    }
    else if ( value > ( double )std::numeric_limits< OUT >::max() )
    {

      out = std::numeric_limits< OUT >::max();

    }
    else
    {

      out = ( OUT )value;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::RescalerWithSlopeAndIntercept< IN, OUT >::rescale( "
             "const IN& in, OUT& out ) const" );

}


//
// class RescalerWithSlopeAndIntercept< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
gkg::RescalerWithSlopeAndIntercept< std::vector< IN >,
                                    std::vector< OUT > >
               ::RescalerWithSlopeAndIntercept( double slope, double intercept )
                : _itemRescalerWithSlopeAndIntercept( slope, intercept )
{
}



template < class IN, class OUT >
inline
void gkg::RescalerWithSlopeAndIntercept< std::vector< IN >,
                                         std::vector< OUT > >::rescale(
                    const std::vector< IN >& in, std::vector< OUT >& out ) const
{

  try
  {


    if ( ( void* )&out != ( void* )&in )
    {

      out = std::vector< OUT >( in.size() );

    }

    // converting items
    typename std::vector< IN >::const_iterator i = in.begin();
    typename std::vector< OUT >::iterator o = out.begin(), oe = out.end();
    while ( o != oe )
    {

      _itemRescalerWithSlopeAndIntercept.rescale( *i, *o );
      ++ i;
      ++ o;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::RescalerWithSlopeAndIntercept< std::vector< IN >, "
             "std::vector< OUT > >::rescale( "
             "const std::vector< IN >& in, std::vector< OUT >& out ) const" );

}


//
// class RescalerWithSlopeAndIntercept< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
gkg::RescalerWithSlopeAndIntercept< gkg::Volume< IN >, gkg::Volume< OUT > >
               ::RescalerWithSlopeAndIntercept( double slope, double intercept )
                : _itemRescalerWithSlopeAndIntercept( slope, intercept )
{
}



template < class IN, class OUT >
inline
void gkg::RescalerWithSlopeAndIntercept< gkg::Volume< IN >,
                                         gkg::Volume< OUT > >::rescale(
                    const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const
{

  try
  {

    out.reallocate( in.getSizeX(),
                    in.getSizeY(),
                    in.getSizeZ(),
                    in.getSizeT() );

    // duplicating header
    out.getHeader() = in.getHeader();
    out.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    int32_t sizeZ = in.getSizeZ();
    int32_t maxThreadCount = ( in.isMemoryMapped() ||
                               out.isMemoryMapped() ) ? 1 : 0;
    gkg::VolumeRescalerWithSlopeAndInterceptContext< IN, OUT > 
      context( in, out, _itemRescalerWithSlopeAndIntercept );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 0, sizeZ, maxThreadCount );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::RescalerWithSlopeAndIntercept< gkg::Volume< IN >, "
             "gkg::Volume< OUT > >::rescale( "
             "const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const" );

}


#endif
