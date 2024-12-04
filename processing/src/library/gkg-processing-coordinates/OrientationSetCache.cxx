#include <gkg-processing-coordinates/OrientationSetCache.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-core-exception/Exception.h>
#include <cstdio>
#include <cmath>
#include <list>
#include <iostream>


#define RESOLUTION 1
#define BIN_COUNT ( 90 / RESOLUTION )

#define RADIAN_TO_DEGREES( a ) ( a * 180.0 / M_PI )
#define RADIAN_TO_INDEX( a ) gkg::round( \
                             ( ( RADIAN_TO_DEGREES( a ) - RESOLUTION ) / \
                               RESOLUTION ) )
#define INDEX_TO_RADIAN( a ) ( float )( a * RESOLUTION * M_PI / 180.0 )


//
// Multithreading context
//


gkg::OrientationSetCacheContext::OrientationSetCacheContext(
      const OrientationSet& orientationSet,
      std::vector< std::vector< int32_t > >& neighborCounts,
      std::vector< std::vector< std::vector< int32_t > > >& neighborIndices,
      std::vector< std::vector< int32_t > >& solidAngleOrientationCounts,
      std::vector< std::vector< std::vector< int32_t > > >&
                                                   solidAngleOrientationIndices,
      std::vector< std::vector< float > >& dotProducts,
      std::vector< std::vector< float > >& vectorAngles,
      std::vector< std::vector< float > >& lineAngles )
                               : gkg::LoopContext< int32_t >(),
                                 _orientationSet( orientationSet ),
                                 _orientationCount( orientationSet.getCount() ),
                                 _neighborCounts( neighborCounts ),
                                 _neighborIndices( neighborIndices ),
                                 _solidAngleOrientationCounts( 
                                                  solidAngleOrientationCounts ),
                                 _solidAngleOrientationIndices(
                                                 solidAngleOrientationIndices ),
                                 _dotProducts( dotProducts ),
                                 _vectorAngles( vectorAngles ),
                                 _lineAngles( lineAngles )
{
}


void gkg::OrientationSetCacheContext::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    int32_t o1 = 0;
    int32_t o2 = 0;
    int32_t a = 0;
    int32_t stopIndex = startIndex + count;
    for ( o1 = startIndex; o1 < stopIndex; o1++ )
    {

      const Vector3d< float >& orientation1 = 
                                           _orientationSet.getOrientation( o1 );

      for ( o2 = 0; o2 < _orientationCount; o2++ )
      {

        // building dot product(s), vector angle(s) and line angle(s)
        const Vector3d< float >& orientation2 = 
                                           _orientationSet.getOrientation( o2 );
        _dotProducts[ o1 ][ o2 ] = orientation1.dot( orientation2 );
        _vectorAngles[ o1 ][ o2 ] = gkg::getVectorAngles( orientation1,
                                                          orientation2 );
        _lineAngles[ o1 ][ o2 ] = gkg::getLineAngles( orientation1,
                                                      orientation2 );

      }

      for ( a = 0; a < BIN_COUNT; a++ )
      {

        double cosineApertureAngle =
                                   std::cos( ( double )INDEX_TO_RADIAN( a ) );

        std::list< int32_t > neighborIndices;
        std::list< int32_t > solidAngleOrientationIndices;

        for ( o2 = 0; o2 < _orientationCount; o2++ )
        {

          // testing neighbor indices
          if ( _dotProducts[ o1 ][ o2 ] >= cosineApertureAngle )
          {

            if ( o2 != o1 )
            {

              neighborIndices.push_back( o2 );

            }
            solidAngleOrientationIndices.push_back( o2 );

          }

        }

        // building neighbor indices
        _neighborIndices[ o1 ][ a ] = std::vector< int32_t >(
                                                      neighborIndices.begin(),
                                                      neighborIndices.end() );
        _neighborCounts[ o1 ][ a ] = ( int32_t )neighborIndices.size();

        // building solid angle orientation indices
        _solidAngleOrientationIndices[ o1 ][ a ] = std::vector< int32_t >(
                                         solidAngleOrientationIndices.begin(),
                                         solidAngleOrientationIndices.end() );
        _solidAngleOrientationCounts[ o1 ][ a ] =
                               ( int32_t )solidAngleOrientationIndices.size();

      }

    }

  }
  GKG_CATCH( "void gkg::OrientationSetCacheContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class OrientationSetCache
//


gkg::OrientationSetCache::OrientationSetCache()
                         : _orientationSet( 0 ),
                           _apertureAngleIndex( -1 )
{
}


gkg::OrientationSetCache::~OrientationSetCache()
{
}


void 
gkg::OrientationSetCache::update( const gkg::OrientationSet& orientationSet,
                                  float apertureAngle,
                                  bool verbose )
{

  try
  {

    if ( _apertureAngleIndex != RADIAN_TO_INDEX( apertureAngle ) )
    {

      _apertureAngleIndex = RADIAN_TO_INDEX( apertureAngle );
      if ( _apertureAngleIndex >= BIN_COUNT )
      {

        throw std::runtime_error(
                                 "aperture angle must be limited to M_PI / 2" );

      }

    }

    if ( &orientationSet != _orientationSet )
    {

      // updating protected field(s)
      _orientationSet = &orientationSet;

      // allocating vectors and matrices
      int32_t orientationCount = _orientationSet->getCount();
      _neighborCounts.resize( orientationCount );
      _neighborIndices.resize( orientationCount );
      _solidAngleOrientationCounts.resize( orientationCount );
      _solidAngleOrientationIndices.resize( orientationCount );
      _dotProducts.resize( orientationCount );
      _vectorAngles.resize( orientationCount );
      _lineAngles.resize( orientationCount );

      // looping over orientation(s)
      int32_t o1 = 0;
      for ( o1 = 0; o1 < orientationCount; o1++ )
      {

        _neighborCounts[ o1 ].resize( BIN_COUNT );
        _neighborIndices[ o1 ].resize( BIN_COUNT );

        _solidAngleOrientationCounts[ o1 ].resize( BIN_COUNT );
        _solidAngleOrientationIndices[ o1 ].resize( BIN_COUNT );

        _dotProducts[ o1 ].resize( orientationCount );
        _vectorAngles[ o1 ].resize( orientationCount );
        _lineAngles[ o1 ].resize( orientationCount );

      }

      gkg::OrientationSetCacheContext context( orientationSet,
                                               _neighborCounts,
                                               _neighborIndices,
                                               _solidAngleOrientationCounts,
                                               _solidAngleOrientationIndices,
                                               _dotProducts,
                                               _vectorAngles,
                                               _lineAngles );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context, 0, orientationCount );

      threadedLoop.launch();

    }

    if ( verbose )
    {

      int32_t orientationCount = _orientationSet->getCount();
      std::vector< int32_t > orientationCounts( orientationCount );
      int32_t o = 0;
      for ( o = 0; o < orientationCount; o++ )
      {

        orientationCounts[ o ] = _neighborCounts[ o ][ _apertureAngleIndex ];

      }
      std::pair< int32_t, int32_t > minimumMaximumCount;
      gkg::MinMaxFilter< std::vector< int32_t >, int32_t > minMaxFilter;
      minMaxFilter.filter( orientationCounts, minimumMaximumCount );

      int32_t averageCount = 0;
      gkg::AverageFilter< std::vector< int32_t >, int32_t > averageFilter;
      averageFilter.filter( orientationCounts, averageCount );

      int32_t stdDevCount = 0;

      gkg::StdDevFilter< std::vector< int32_t >, int32_t > stdDevFilter;
      stdDevFilter.filter( orientationCounts, stdDevCount );

      std::cout << "(min=" << minimumMaximumCount.first
                << ",max=" << minimumMaximumCount.second
                << ",mean=" << averageCount
                << ",stddev=" << stdDevCount
                << ") neighbors" << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::OrientationSetCache::update( "
             "const gkg::OrientationSet& orientationSet, "
             "float apertureAngle )" );

}


float gkg::OrientationSetCache::getApertureAngle() const
{

  try
  {

    return INDEX_TO_RADIAN( _apertureAngleIndex );

  }
  GKG_CATCH( "float gkg::OrientationSetCache::getApertureAngle() const" );

}


const gkg::OrientationSet* gkg::OrientationSetCache::getOrientationSet() const
{

  try
  {

    return _orientationSet;

  }
  GKG_CATCH( "const gkg::OrientationSet* "
             "gkg::OrientationSetCache::getOrientationSet() const" );

}


int32_t 
gkg::OrientationSetCache::getNeighborCount( int32_t orientationIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }

#endif

  return _neighborCounts[ orientationIndex ][ _apertureAngleIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationSetCache::getNeighborCount( "
             "int32_t orientationIndex ) const" );

}


const std::vector< int32_t >& 
gkg::OrientationSetCache::getNeighborIndices( int32_t orientationIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }

#endif

  return _neighborIndices[ orientationIndex ][ _apertureAngleIndex ];

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::OrientationSetCache::getNeighborIndices( "
             "int32_t orientationIndex ) const" );

}


int32_t 
gkg::OrientationSetCache::getNeighborIndex( int32_t orientationIndex,
                                            int32_t neighborIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }
    if ( neighborIndex >= ( int32_t )_neighborCounts.size() )
    {

      char message[ 256 ];
      sprintf( message, "invalid neighbor index ( %d )", neighborIndex );
      throw std::runtime_error( message );

    }

#endif

  return _neighborIndices[ orientationIndex ][ _apertureAngleIndex ]
                         [ neighborIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationSetCache::getNeighborIndex( "
             "int32_t orientationIndex, "
             "int32_t neighborIndex ) const" );

}


int32_t 
gkg::OrientationSetCache::getSolidAngleOrientationCount(
                                                int32_t orientationIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }

#endif

  return _solidAngleOrientationCounts[ orientationIndex ][ _apertureAngleIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationSetCache::getSolidAngleOrientationCount( "
             "int32_t orientationIndex ) const" );

}


const std::vector< int32_t >& 
gkg::OrientationSetCache::getSolidAngleOrientationIndices(
                                                int32_t orientationIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }

#endif

  return _solidAngleOrientationIndices[ orientationIndex ]
                                      [ _apertureAngleIndex ];

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::OrientationSetCache::getSolidAngleOrientationIndices( "
             "int32_t orientationIndex ) const" );

}


int32_t 
gkg::OrientationSetCache::getSolidAngleOrientationIndex(
                                      int32_t orientationIndex,
                                      int32_t solidAngleOrientationIndex ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( orientationIndex >= _orientationSet->getCount() )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation index ( %d )", orientationIndex );
      throw std::runtime_error( message );

    }
    if ( solidAngleOrientationIndex >=
         ( int32_t )_solidAngleOrientationCounts.size() )
    {

      char message[ 256 ];
      sprintf( message, "invalid solid angle orientation index ( %d )",
               solidAngleOrientationIndex );
      throw std::runtime_error( message );

    }

#endif

  return _solidAngleOrientationIndices[ orientationIndex ]
                                      [ _apertureAngleIndex ]
                                      [ solidAngleOrientationIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationSetCache::getSolidAngleOrientationIndex( "
             "int32_t orientationIndex, "
             "int32_t solidAngleOrientationIndex ) const" );

}


float gkg::OrientationSetCache::getDotProduct( int32_t orientationIndex1,
                                               int32_t orientationIndex2 ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( orientationIndex1 >= _orientationSet->getCount() ) ||
         ( orientationIndex2 >= _orientationSet->getCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation indices ( %d, %d )",
               orientationIndex1,
               orientationIndex2 );
      throw std::runtime_error( message );

    }

#endif

  return _dotProducts[ orientationIndex1 ][ orientationIndex2 ];

  }
  GKG_CATCH( "float gkg::OrientationSetCache::getDotProduct( "
             "int32_t orientationIndex1, int32_t orientationIndex2 ) const" );

}


float 
gkg::OrientationSetCache::getVectorAngle( int32_t orientationIndex1,
                                          int32_t orientationIndex2 ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( orientationIndex1 >= _orientationSet->getCount() ) ||
         ( orientationIndex2 >= _orientationSet->getCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation indices ( %d, %d )",
               orientationIndex1,
               orientationIndex2 );
      throw std::runtime_error( message );

    }

#endif

  return _vectorAngles[ orientationIndex1 ][ orientationIndex2 ];

  }
  GKG_CATCH( "float "
             "gkg::OrientationSetCache::getVectorAngle( "
             "int32_t orientationIndex1, int32_t orientationIndex2 ) const" );

}


float 
gkg::OrientationSetCache::getLineAngle( int32_t orientationIndex1,
                                        int32_t orientationIndex2 ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( orientationIndex1 >= _orientationSet->getCount() ) ||
         ( orientationIndex2 >= _orientationSet->getCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid orientation indices ( %d, %d )",
               orientationIndex1,
               orientationIndex2 );
      throw std::runtime_error( message );

    }

#endif

  return _lineAngles[ orientationIndex1 ][ orientationIndex2 ];

  }
  GKG_CATCH( "float "
             "gkg::OrientationSetCache::getLineAngle( "
             "int32_t orientationIndex1, int32_t orientationIndex2 ) const" );

}
