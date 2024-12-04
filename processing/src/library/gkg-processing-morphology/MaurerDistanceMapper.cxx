#include <gkg-processing-morphology/MaurerDistanceMapper.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-morphology/MaurerEDT2Context_i.h>
#include <gkg-processing-morphology/MaurerEDT3Context_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


template < class IN >
gkg::MaurerDistanceMapper< IN >::MaurerDistanceMapper(
                                   const gkg::TestFunction< IN >& testFunction )
                               : _testFunction( testFunction )
{
}


template < class IN >
gkg::MaurerDistanceMapper< IN >::~MaurerDistanceMapper()
{
}


template < class IN > template < class OUT >
void gkg::MaurerDistanceMapper< IN >::getDistance(
                                                const gkg::Volume< IN >& volume,
                                                gkg::Volume< OUT >& distance,
                                                bool verbose ) const
{

  try
  {

    // allocating stuff for distance map
    reallocate( volume, distance );

    if ( verbose )
    {

      std::cout << "computing distance map" << std::flush;

    }

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    gkg::Vector3d< double > resolution;

    volume.getResolution( resolution );
    distance.setResolution( resolution );

    double maxDist = std::max( 2.0 * sizeX * resolution.x, 
                               std::max( 2.0 * sizeY * resolution.y, 
                                         2.0 * sizeZ * resolution.z ) );
    OUT sqMaxDist = OUT( maxDist * maxDist );

    // Process first and second direction
    int32_t maxThreadCount = ( volume.isMemoryMapped() ||
                               distance.isMemoryMapped() ) ? 1 : 0;
    gkg::MaurerEDT2Context< IN, OUT > context2( volume,
                                                _testFunction,
                                                sqMaxDist,
                                                distance );
    gkg::ThreadedLoop< int32_t > threadedLoop2( &context2,
                                                0,
                                                sizeZ,
                                                maxThreadCount );

    threadedLoop2.launch( true, true );

    // Process third direction
    gkg::MaurerEDT3Context< OUT > context3( sqMaxDist,
                                            distance );
    gkg::ThreadedLoop< int32_t > threadedLoop3(
                                     &context3,
                                     0,
                                     sizeY,
                                     distance.isMemoryMapped() ? 1 : 0 );

    threadedLoop3.launch( true, true );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::MaurerDistanceMapper< IN >::getDistance( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< OUT >& distance, "
             "bool verbose ) const" );

}


template < class IN > template < class OUT >
void gkg::MaurerDistanceMapper< IN >::reallocate( 
                                                 const gkg::Volume< IN >& in,
                                                 gkg::Volume< OUT >& out ) const
{

  try
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
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::MaurerDistanceMapper< IN >::reallocate( "
             "const gkg::Volume< IN >& in, "
             "gkg::Volume< OUT >& out )" );

}


template class gkg::MaurerDistanceMapper< int8_t >;
template class gkg::MaurerDistanceMapper< uint8_t >;
template class gkg::MaurerDistanceMapper< int16_t >;
template class gkg::MaurerDistanceMapper< uint16_t >;
template class gkg::MaurerDistanceMapper< int32_t >;
template class gkg::MaurerDistanceMapper< uint32_t >;
template class gkg::MaurerDistanceMapper< int64_t >;
template class gkg::MaurerDistanceMapper< uint64_t >;
template class gkg::MaurerDistanceMapper< float >;
template class gkg::MaurerDistanceMapper< double >;


#define InstanciateMaurerDistanceMapper( IN, OUT )           \
template void gkg::MaurerDistanceMapper< IN >::getDistance(  \
                   const gkg::Volume< IN >& volume,          \
                   gkg::Volume< OUT >& distance,             \
                   bool verbose ) const


InstanciateMaurerDistanceMapper( int8_t, uint8_t );
InstanciateMaurerDistanceMapper( uint8_t, uint8_t );
InstanciateMaurerDistanceMapper( int16_t, uint8_t );
InstanciateMaurerDistanceMapper( uint16_t, uint8_t );
InstanciateMaurerDistanceMapper( int32_t, uint8_t );
InstanciateMaurerDistanceMapper( uint32_t, uint8_t );
InstanciateMaurerDistanceMapper( int64_t, uint8_t );
InstanciateMaurerDistanceMapper( uint64_t, uint8_t );
InstanciateMaurerDistanceMapper( float, uint8_t );
InstanciateMaurerDistanceMapper( double, uint8_t );

InstanciateMaurerDistanceMapper( int8_t, int8_t );
InstanciateMaurerDistanceMapper( uint8_t, int8_t );
InstanciateMaurerDistanceMapper( int16_t, int8_t );
InstanciateMaurerDistanceMapper( uint16_t, int8_t );
InstanciateMaurerDistanceMapper( int32_t, int8_t );
InstanciateMaurerDistanceMapper( uint32_t, int8_t );
InstanciateMaurerDistanceMapper( int64_t, int8_t );
InstanciateMaurerDistanceMapper( uint64_t, int8_t );
InstanciateMaurerDistanceMapper( float, int8_t );
InstanciateMaurerDistanceMapper( double, int8_t );

InstanciateMaurerDistanceMapper( int8_t, uint16_t );
InstanciateMaurerDistanceMapper( uint8_t, uint16_t );
InstanciateMaurerDistanceMapper( int16_t, uint16_t );
InstanciateMaurerDistanceMapper( uint16_t, uint16_t );
InstanciateMaurerDistanceMapper( int32_t, uint16_t );
InstanciateMaurerDistanceMapper( uint32_t, uint16_t );
InstanciateMaurerDistanceMapper( int64_t, uint16_t );
InstanciateMaurerDistanceMapper( uint64_t, uint16_t );
InstanciateMaurerDistanceMapper( float, uint16_t );
InstanciateMaurerDistanceMapper( double, uint16_t );

InstanciateMaurerDistanceMapper( int8_t, int16_t );
InstanciateMaurerDistanceMapper( uint8_t, int16_t );
InstanciateMaurerDistanceMapper( int16_t, int16_t );
InstanciateMaurerDistanceMapper( uint16_t, int16_t );
InstanciateMaurerDistanceMapper( int32_t, int16_t );
InstanciateMaurerDistanceMapper( uint32_t, int16_t );
InstanciateMaurerDistanceMapper( int64_t, int16_t );
InstanciateMaurerDistanceMapper( uint64_t, int16_t );
InstanciateMaurerDistanceMapper( float, int16_t );
InstanciateMaurerDistanceMapper( double, int16_t );

InstanciateMaurerDistanceMapper( int8_t, uint32_t );
InstanciateMaurerDistanceMapper( uint8_t, uint32_t );
InstanciateMaurerDistanceMapper( int16_t, uint32_t );
InstanciateMaurerDistanceMapper( uint16_t, uint32_t );
InstanciateMaurerDistanceMapper( int32_t, uint32_t );
InstanciateMaurerDistanceMapper( uint32_t, uint32_t );
InstanciateMaurerDistanceMapper( int64_t, uint32_t );
InstanciateMaurerDistanceMapper( uint64_t, uint32_t );
InstanciateMaurerDistanceMapper( float, uint32_t );
InstanciateMaurerDistanceMapper( double, uint32_t );

InstanciateMaurerDistanceMapper( int8_t, int32_t );
InstanciateMaurerDistanceMapper( uint8_t, int32_t );
InstanciateMaurerDistanceMapper( int16_t, int32_t );
InstanciateMaurerDistanceMapper( uint16_t, int32_t );
InstanciateMaurerDistanceMapper( int32_t, int32_t );
InstanciateMaurerDistanceMapper( uint32_t, int32_t );
InstanciateMaurerDistanceMapper( int64_t, int32_t );
InstanciateMaurerDistanceMapper( uint64_t, int32_t );
InstanciateMaurerDistanceMapper( float, int32_t );
InstanciateMaurerDistanceMapper( double, int32_t );

InstanciateMaurerDistanceMapper( int8_t, uint64_t );
InstanciateMaurerDistanceMapper( uint8_t, uint64_t );
InstanciateMaurerDistanceMapper( int16_t, uint64_t );
InstanciateMaurerDistanceMapper( uint16_t, uint64_t );
InstanciateMaurerDistanceMapper( int32_t, uint64_t );
InstanciateMaurerDistanceMapper( uint32_t, uint64_t );
InstanciateMaurerDistanceMapper( int64_t, uint64_t );
InstanciateMaurerDistanceMapper( uint64_t, uint64_t );
InstanciateMaurerDistanceMapper( float, uint64_t );
InstanciateMaurerDistanceMapper( double, uint64_t );

InstanciateMaurerDistanceMapper( int8_t, int64_t );
InstanciateMaurerDistanceMapper( uint8_t, int64_t );
InstanciateMaurerDistanceMapper( int16_t, int64_t );
InstanciateMaurerDistanceMapper( uint16_t, int64_t );
InstanciateMaurerDistanceMapper( int32_t, int64_t );
InstanciateMaurerDistanceMapper( uint32_t, int64_t );
InstanciateMaurerDistanceMapper( int64_t, int64_t );
InstanciateMaurerDistanceMapper( uint64_t, int64_t );
InstanciateMaurerDistanceMapper( float, int64_t );
InstanciateMaurerDistanceMapper( double, int64_t );

InstanciateMaurerDistanceMapper( int8_t, float );
InstanciateMaurerDistanceMapper( uint8_t, float );
InstanciateMaurerDistanceMapper( int16_t, float );
InstanciateMaurerDistanceMapper( uint16_t, float );
InstanciateMaurerDistanceMapper( int32_t, float );
InstanciateMaurerDistanceMapper( uint32_t, float );
InstanciateMaurerDistanceMapper( int64_t, float );
InstanciateMaurerDistanceMapper( uint64_t, float );
InstanciateMaurerDistanceMapper( float, float );
InstanciateMaurerDistanceMapper( double, float );

InstanciateMaurerDistanceMapper( int8_t, double );
InstanciateMaurerDistanceMapper( uint8_t, double );
InstanciateMaurerDistanceMapper( int16_t, double );
InstanciateMaurerDistanceMapper( uint16_t, double );
InstanciateMaurerDistanceMapper( int32_t, double );
InstanciateMaurerDistanceMapper( uint32_t, double );
InstanciateMaurerDistanceMapper( int64_t, double );
InstanciateMaurerDistanceMapper( uint64_t, double );
InstanciateMaurerDistanceMapper( float, double );
InstanciateMaurerDistanceMapper( double, double );


#undef InstanciateMaurerDistanceMapper
