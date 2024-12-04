#ifndef _gkg_processing_algobase_Rescaler_i_h_
#define _gkg_processing_algobase_Rescaler_i_h_


#include <gkg-processing-algobase/Rescaler.h>
#include <gkg-processing-algobase/VolumeRescalerContext_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class Rescaler< IN, OUT >
//

template < class IN, class OUT >
inline
gkg::Rescaler< IN, OUT >::Rescaler( const IN& imin,
                                    const IN& imax,
                                    const OUT& omin,
                                    const OUT& omax )
                         : _imin( imin ),
                           _imax( imax ),
                           _omin( omin ),
                           _omax( omax )
{

  _scale = ( ( double )_omax - ( double )_omin ) /
           ( ( double )_imax - ( double )_imin );

}


template < class IN, class OUT >
inline
gkg::Rescaler< IN, OUT >::Rescaler( const gkg::Rescaler< IN, OUT >& other )
                         : _imin( other._imin ),
                           _imax( other._imax ),
                           _omin( other._omin ),
                           _omax( other._omax )
{

  _scale = ( ( double )_omax - ( double )_omin ) /
           ( ( double )_imax - ( double )_imin );

}


template < class IN, class OUT >
inline
gkg::Rescaler< IN, OUT >& 
gkg::Rescaler< IN, OUT >::operator=( const gkg::Rescaler< IN, OUT >& other )
{

  _imin = other._imin;
  _imax = other._imax;
  _omin = other._omin;
  _omax = other._omax;
  _scale = ( ( double )_omax - ( double )_omin ) /
           ( ( double )_imax - ( double )_imin );

  return *this;

}


template < class IN, class OUT >
inline
void gkg::Rescaler< IN, OUT >::rescale( const IN& in, OUT& out ) const
{

  if ( in < _imin )
  {

    out = _omin;

  }
  else if ( in > _imax )
  {

    out = _omax;

  }
  else
  {

    out = ( OUT )( ( double )_omin + ( ( ( double )in - ( double )_imin ) *
                                       _scale ) );

  }

}


//
// class Rescaler< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
inline
gkg::Rescaler< std::vector< IN >, std::vector< OUT > >::Rescaler(
                                                        const IN& imin,
                                                        const IN& imax,
                                                        const OUT& omin,
                                                        const OUT& omax )
                                             : _itemRescaler( imin, imax,
                                                              omin, omax )
{
}



template < class IN, class OUT >
inline
void gkg::Rescaler< std::vector< IN >, std::vector< OUT > >::rescale(
                    const std::vector< IN >& in, std::vector< OUT >& out ) const
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

    _itemRescaler.rescale( *i, *o );
    ++ i;
    ++ o;

  }

}


//
// class Rescaler< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
inline
gkg::Rescaler< gkg::Volume< IN >, gkg::Volume< OUT > >::Rescaler(
                                                        const IN& imin,
                                                        const IN& imax,
                                                        const OUT& omin,
                                                        const OUT& omax )
                                             : _itemRescaler( imin, imax,
                                                              omin, omax )
{
}



template < class IN, class OUT >
inline
void gkg::Rescaler< gkg::Volume< IN >, gkg::Volume< OUT > >::rescale(
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
    gkg::VolumeRescalerContext< IN, OUT > context( in, out, _itemRescaler );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 0, sizeZ, maxThreadCount );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Rescaler< gkg::Volume< IN >, "
             "gkg::Volume< OUT > >::rescale( "
             "const gkg::Volume< IN >& in, gkg::Volume< OUT >& out ) const" );

}


#endif
