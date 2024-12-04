#ifndef _gkg_processing_algobase_Scaler_i_h_
#define _gkg_processing_algobase_Scaler_i_h_


#include <gkg-processing-algobase/Scaler.h>
#include <gkg-processing-algobase/VolumeScalerContext_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


//
// class Scaler< T >
//

template < class T >
inline
gkg::Scaler< T >::Scaler( const T& scale )
                 : _scale( scale )
{
}


template < class T >
inline
void gkg::Scaler< T >::scale( T& in ) const
{

  in *= _scale;

}


//
// class Scaler< std::vector< T > >
//

template < class T >
inline
gkg::Scaler< std::vector< T > >::Scaler( const T& scale )
                                : _itemScaler( scale )
{
}



template < class T >
inline
void gkg::Scaler< std::vector< T > >::scale( std::vector< T >& in ) const
{

  typename std::vector< T >::iterator i = in.begin(), ie = in.end();
  while ( i != ie )
  {

    _itemScaler.scale( *i );
    ++ i;

  }

}


//
// class Scaler< Vector >
//

inline
gkg::Scaler< gkg::Vector >::Scaler( const double& scale )
                           : _itemScaler( scale )
{
}



inline
void gkg::Scaler< gkg::Vector >::scale( gkg::Vector& in ) const
{

  int32_t i = 0,
          ie = in.getSize();
  while ( i != ie )
  {

    _itemScaler.scale( in( i ) );
    ++ i;

  }

}


//
// class Scaler< Volume< T > >
//

template < class T >
inline
gkg::Scaler< gkg::Volume< T > >::Scaler( const T& scale )
                                : _itemScaler( scale )
{
}



template < class T >
inline
void gkg::Scaler< gkg::Volume< T > >::scale( gkg::Volume< T >& in ) const
{

  try
  {

    int32_t sizeZ = in.getSizeZ();

    gkg::VolumeScalerContext< T > context( in, _itemScaler );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    sizeZ,
                    in.isMemoryMapped() ? 1 : 0 );

    threadedLoop.launch( true, true );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Scaler< gkg::Volume< T > >::scale( "
             "gkg::Volume< T >& in ) const" );

}


#endif
