#include <gkg-processing-numericalanalysis/HypercubeIndexMapper.h>
#include <gkg-core-exception/Exception.h>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <unordered_map>


#ifdef DEBUGGING_HPDBSCAN

#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-numericalanalysis/BundleMap_i.h>
#include <string>

#endif

////////////////////////////////////////////////////////////////////////////////
// OMP reduction function(s)
////////////////////////////////////////////////////////////////////////////////

// vectorMinimum() /////////////////////////////////////////////////////////////

static void vectorMinimum( std::vector< double >& omp_in,
                           std::vector< double >& omp_out )
{

  size_t i = 0;
  for ( i = 0; i < omp_out.size(); i++ )
  {

    omp_out[ i ] = std::min( omp_in[ i ], omp_out[ i ] );

  }

}

#pragma omp declare reduction( vectorMinimum: std::vector< double >:  \
                               vectorMinimum( omp_in, omp_out ) ) \
  initializer( omp_priv = omp_orig )

 
// vectorMaximum() /////////////////////////////////////////////////////////////

static void vectorMaximum( std::vector< double >& omp_in,
                           std::vector< double >& omp_out )
{

  size_t i = 0; 
  for ( i = 0; i < omp_out.size(); i++ )
  {

    omp_out[ i ] = std::max( omp_in[ i ], omp_out[ i ] );

  }

}

#pragma omp declare reduction( vectorMaximum: std::vector< double >: \
                               vectorMaximum( omp_in, omp_out ) ) \
  initializer( omp_priv = omp_orig )


// mergeHistograms() ///////////////////////////////////////////////////////////

static void mergeHistograms( std::map< int64_t, int32_t >& omp_in,
                             std::map< int64_t, int32_t >& omp_out)
{

  std::map< int64_t, int32_t >::const_iterator
    i = omp_in.begin(),
    ie = omp_in.end();
  while ( i != ie )
  {

    omp_out[ i->first ] += i->second;
    ++ i;

  }

}
#pragma omp declare reduction( mergeHistograms: std::map< int64_t, int32_t >: \
                               mergeHistograms( omp_in, omp_out ) ) \
  initializer( omp_priv = omp_orig )



////////////////////////////////////////////////////////////////////////////////
// class HypercubeIndexMapper
////////////////////////////////////////////////////////////////////////////////

gkg::HypercubeIndexMapper::HypercubeIndexMapper(
                        const gkg::Matrix& samples,
                        const std::vector< int32_t >& chosenGridCoordinates,
                        double epsilon,
                        double maximumDistance,
                        gkg::RCPointer< gkg::DistanceFunctor > distanceFunctor )
                          : _sampleCount( samples.getSize1() ),
                            _coordinateCount( samples.getSize2() ),
                            _chosenGridCoordinates( chosenGridCoordinates ),
                            _chosenCoordinateCount(
                                      ( int32_t )chosenGridCoordinates.size() ),
                            _maximumDistanceSquare( maximumDistance * 
                                                    maximumDistance ),
                            _distanceFunctor( distanceFunctor ),
                            _initialSampleOrder( _sampleCount, 0 ),
                            _minimumCoordinateValues(
                                         _chosenCoordinateCount,
                                         std::numeric_limits< double >::max() ),
                            _maximumCoordinateValues(
                                         _chosenCoordinateCount,
                                         std::numeric_limits< double >::min() ),
                            _cellCounts( _chosenCoordinateCount, 0 ),
                            _globalCellCount( 1 ),
                            _lastCellIndex( 0 ),
                            _ascendingCellCountBasedCoordinates(
                                                    _chosenCoordinateCount, 0 ),
                            _sampleToCellIndices( _sampleCount, 0 ),
                            _reorderedSampleToCellIndices( _sampleCount, 0 ),
                            _reorderedSampleOrder( _sampleCount, 0 ),
                            _reorderedSamples( _sampleCount, _coordinateCount )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // computing the initial sample order
    ////////////////////////////////////////////////////////////////////////////

    _initialSampleOrder.resize( _sampleCount, 0 );
    int32_t s = 0;
    #pragma omp parallel for
    for ( s = 0; s < _sampleCount; s++ )
    {

      _initialSampleOrder[ s ] = s;    // += s originally ???

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the boundaries of each coordinate for all the samples
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double >& minimumCoordinateValues = _minimumCoordinateValues;
    std::vector< double >& maximumCoordinateValues = _maximumCoordinateValues;

    #pragma omp parallel for reduction( vectorMinimum: \
                                        minimumCoordinateValues ) \
                             reduction( vectorMaximum: \
                                        maximumCoordinateValues )
    for ( s = 0; s < _sampleCount; s++ )
    {

      int32_t coordinate = 0;
      for ( coordinate = 0; coordinate < _chosenCoordinateCount; coordinate++ )
      {

        const double& value = samples( s, _chosenGridCoordinates[ coordinate ] );
        minimumCoordinateValues[ coordinate ] = 
                      std::min( minimumCoordinateValues[ coordinate ], value );
        maximumCoordinateValues[ coordinate ] = 
                      std::max( maximumCoordinateValues[ coordinate ], value );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the cell count for each coordinate and the global cell count
    ////////////////////////////////////////////////////////////////////////////


    int32_t coordinate = 0;
    for ( coordinate = 0; coordinate < _chosenCoordinateCount; coordinate++ )
    {

      int32_t cellCountAlongCoordinate = static_cast< int32_t >(
                      std::ceil( ( _maximumCoordinateValues[ coordinate ] -
                                   _minimumCoordinateValues[ coordinate ] ) /
                                 epsilon ) ); // + 1;
      _cellCounts[ coordinate ] = cellCountAlongCoordinate;
      _globalCellCount *= ( int64_t )cellCountAlongCoordinate;

    }
    _lastCellIndex = _globalCellCount;

    std::cout << "_globalCellCount=" << _globalCellCount << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // swapping  the cell count for each coordinate and the global cell count
    ////////////////////////////////////////////////////////////////////////////

    std::iota( _ascendingCellCountBasedCoordinates.begin(), 
               _ascendingCellCountBasedCoordinates.end(), 0 );

    // reordering using ascending order of cell count(s)
    std::sort( _ascendingCellCountBasedCoordinates.begin(),
               _ascendingCellCountBasedCoordinates.end(),
               [ & ]( int32_t a, int32_t b )
               { return _cellCounts[ a ] < _cellCounts[ b ]; } );


    ////////////////////////////////////////////////////////////////////////////
    // computing cell histogram(s) and attributing a cell index to each sample
    ////////////////////////////////////////////////////////////////////////////

    std::map< int64_t, int32_t > cellHistogram;
    #pragma omp parallel for reduction( mergeHistograms: cellHistogram )
    for ( s = 0; s < _sampleCount; s++ )
    {

      int64_t cellIndex = 0;
      int64_t accumulator = 1;

      std::vector< int32_t >::const_iterator
        c = _ascendingCellCountBasedCoordinates.begin(),
        ce = _ascendingCellCountBasedCoordinates.end();
      while (  c != ce )
      {

        int32_t cellIndexAlongCoordinate = static_cast< int32_t >(
            std::floor( ( samples( s, _chosenGridCoordinates[ *c ] ) -
                          _minimumCoordinateValues[ *c ] ) /
                        epsilon ) );
        cellIndex += ( int64_t )cellIndexAlongCoordinate *
                     ( int64_t )accumulator;
        accumulator *= ( int64_t )_cellCounts[ *c ];

        ++ c;

      }

      _sampleToCellIndices[ s ] = cellIndex;
      ++ cellHistogram[ cellIndex ];

    }
    _cellHistogram.swap( cellHistogram );


    ////////////////////////////////////////////////////////////////////////////
    // for each cell, gives a starting sample index and store the sample
    // count within each cell
    ////////////////////////////////////////////////////////////////////////////

    int64_t accumulator = 0;
 
    std::map< int64_t, int32_t >::const_iterator
      h = _cellHistogram.begin(),
      he = _cellHistogram.end();
    while ( h != he )
    {

      std::pair< int32_t, int32_t >& pairIndex = 
                            _cellIndexToSampleStartingIndexAndCount[ h->first ];

      pairIndex.first = accumulator;
      pairIndex.second = h->second;
      accumulator += ( int64_t )h->second;

      ++ h;

    }

    _cellIndexToSampleStartingIndexAndCount[ _lastCellIndex ].first =
                                                 ( int64_t )_sampleCount;
    _cellIndexToSampleStartingIndexAndCount[ _lastCellIndex ].second = 0;


    ////////////////////////////////////////////////////////////////////////////
    // sorting samples by cell
    ////////////////////////////////////////////////////////////////////////////

    // memory for offset of already placed items
    std::unordered_map< int32_t, std::atomic< int32_t > > offsets;
    std::map< int64_t, std::pair< int32_t, int32_t > >::const_iterator
      ci = _cellIndexToSampleStartingIndexAndCount.begin(),
      cie = _cellIndexToSampleStartingIndexAndCount.end();
    while ( ci != cie )
    {

      offsets[ ci->first ].store( 0 );
      ++ ci;

    }

    // sorting the samples and cells out-of-place, memorize the original order
    #pragma omp parallel for
    for ( s = 0; s < _sampleCount; s++ )
    {

      const int64_t& cellIndex = _sampleToCellIndices[ s ];
      const std::pair< int32_t, int32_t >& 
        sampleStartingIndexAndCount = _cellIndexToSampleStartingIndexAndCount[
                                                                    cellIndex ];
      int32_t sPrime = sampleStartingIndexAndCount.first + offsets[ cellIndex ];
      ++ offsets[ cellIndex ];

      _reorderedSampleToCellIndices[ sPrime ] = cellIndex;
      _reorderedSampleOrder[ sPrime ] = _initialSampleOrder[ s ];
      for ( int32_t coordinate = 0; coordinate < _coordinateCount;
            coordinate++ )
      {

        _reorderedSamples( sPrime, coordinate ) = samples( s, coordinate );

      }

    }


#ifdef DEBUGGING_HPDBSCAN

    ////
    gkg::BundleMap< std::string > bundleMap;
    ci = _cellIndexToSampleStartingIndexAndCount.begin();
    cie = _cellIndexToSampleStartingIndexAndCount.end();
    -- cie;
    while ( ci != cie )
    {

      int32_t startIndex = ci->second.first;
      int32_t count = ci->second.second;


      std::list< gkg::LightCurve3d< float > > fibers;

      int32_t index = 0;
      for ( index = 0; index < count; index++ )
      {

        std::list< gkg::Vector3d< float > > points;
        int32_t p = 0;
        for ( p = 0; p < 21; p++ )
        {

          points.push_back( gkg::Vector3d< float >(  
                ( float )_reorderedSamples( startIndex + index,
                                            p * 3 ),
                ( float )_reorderedSamples( startIndex + index,
                                            p * 3 + 1 ),
                ( float )_reorderedSamples( startIndex + index,
                                            p * 3 + 2 ) ) );

        }
        fibers.push_back( gkg::LightCurve3d< float >( points ) );

      }
      bundleMap.addCurve3ds( gkg::StringConverter::toString( ci->first ),
                             fibers );


      ++ ci;

    }
    gkg::Writer::getInstance().write(
                        "cellIndexBundles",
                        bundleMap,
                        false,
                        "aimsbundlemap" );

#endif

  }
  GKG_CATCH( "gkg::HypercubeIndexMapper::HypercubeIndexMapper( "
             "const gkg::Matrix& samples, "
             "const std::vector< int32_t >& chosenGridCoordinates, "
             "double epsilon, "
             "double maximumDistance, "
             "gkg::RCPointer< gkg::DistanceFunctor > distanceFunctor )" );

}


gkg::HypercubeIndexMapper::~HypercubeIndexMapper()
{
}


int32_t gkg::HypercubeIndexMapper::getSampleCount() const
{

  try
  {

    return _sampleCount;

  }
  GKG_CATCH( "int32_t gkg::HypercubeIndexMapper::getSampleCount() const" );

}


int32_t gkg::HypercubeIndexMapper::getCoordinateCount() const
{

  try
  {

    return _coordinateCount;

  }
  GKG_CATCH( "int32_t gkg::HypercubeIndexMapper::getCoordinateCount() const" );

}


int32_t gkg::HypercubeIndexMapper::getChosenCoordinateCount() const
{

  try
  {

    return _chosenCoordinateCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::HypercubeIndexMapper::getChosenCoordinateCount() const" );

}


double gkg::HypercubeIndexMapper::getMaximumDistanceSquare() const
{

  try
  {

    return _maximumDistanceSquare;

  }
  GKG_CATCH( "double "
             "gkg::HypercubeIndexMapper::getMaximumDistanceSquare() const" );

}


std::vector< int32_t > gkg::HypercubeIndexMapper::getNeighborSampleIndices( 
                                               const int64_t & cellIndex ) const
{

  try
  {

    std::vector< int64_t > neighborCellIndices;
    //neighborCellIndices.reserve( std::pow( 3, _coordinateCount );
    neighborCellIndices.push_back( cellIndex );

    // cell accumulators
    int64_t deltaCellCountAlongLowerCoordinate = 1;
    int64_t deltaCellCountAlongCurrentCoordinate = 1;
    int32_t sampleCount = _cellIndexToSampleStartingIndexAndCount.find(
                                                     cellIndex )->second.second;

    // fetch all existing neighboring cells
    int32_t n = 0;
    int32_t neighborCount = 0;

    int64_t prevCellIndex = 0;
    int64_t nextCellIndex = 0;

    // looping over the reordered cell coordinates
    std::vector< int32_t >::const_iterator
      c = _ascendingCellCountBasedCoordinates.begin(),
      ce = _ascendingCellCountBasedCoordinates.end();
    while ( c != ce )
    {

      deltaCellCountAlongCurrentCoordinate *= ( int64_t )_cellCounts[ *c ];


      // looping over neighbor cell indices
      neighborCount = ( int32_t )neighborCellIndices.size();
      for ( n = 0; n < neighborCount; n++ )
      {

        int64_t currentCellIndex = neighborCellIndices[ n ];

        // computing the previous neighbor cell index along coordinate axis
        prevCellIndex = currentCellIndex - deltaCellCountAlongLowerCoordinate;

        // getting access to the starting sample index and sample count
        // of the before
        std::map< int64_t, std::pair< int32_t, int32_t > >::const_iterator
          pc = _cellIndexToSampleStartingIndexAndCount.find( prevCellIndex );
        if ( ( currentCellIndex % deltaCellCountAlongCurrentCoordinate ) >= 
             deltaCellCountAlongLowerCoordinate )
        {

          neighborCellIndices.push_back( prevCellIndex );
          if ( pc != _cellIndexToSampleStartingIndexAndCount.end() )
          {

            sampleCount += pc->second.second;

          }

        }


        // computing the next neighbor cell index along coordinate axis
        nextCellIndex = currentCellIndex + deltaCellCountAlongLowerCoordinate;

        // getting access to the starting sample index and sample count
        // of the next
        std::map< int64_t, std::pair< int32_t, int32_t > >::const_iterator
          nc = _cellIndexToSampleStartingIndexAndCount.find( nextCellIndex );
        if ( ( currentCellIndex % deltaCellCountAlongCurrentCoordinate ) < 
             ( deltaCellCountAlongCurrentCoordinate -
               deltaCellCountAlongLowerCoordinate ) )
        {

          neighborCellIndices.push_back( nextCellIndex );
          if ( nc != _cellIndexToSampleStartingIndexAndCount.end() )
          {

            sampleCount += nc->second.second;

          }

        }

      }

      deltaCellCountAlongLowerCoordinate = deltaCellCountAlongCurrentCoordinate;

      ++ c;

    }

    // copy the points from the neighboring cells
    std::vector< int32_t > neighborSampleIndices;
    neighborSampleIndices.reserve( sampleCount );

    std::vector< int64_t >::const_iterator
      i = neighborCellIndices.begin(),
      ie = neighborCellIndices.end();
    while ( i != ie )
    {

      std::map< int64_t, std::pair< int32_t, int32_t > >::const_iterator
        sic = _cellIndexToSampleStartingIndexAndCount.find( *i );

      if ( sic != _cellIndexToSampleStartingIndexAndCount.end() )
      {

        const int32_t& furtherSampleStartingIndex = sic->second.first;
        const int32_t& furtherSampleCount = sic->second.second;
        neighborSampleIndices.resize( neighborSampleIndices.size() + 
                                      furtherSampleCount );
        std::iota( neighborSampleIndices.end() - furtherSampleCount,
                   neighborSampleIndices.end(), furtherSampleStartingIndex );

      }


      ++ i;

    } 

    return neighborSampleIndices;

  }
  GKG_CATCH( "std::vector< int32_t > "
             "gkg::HypercubeIndexMapper::getNeighborSampleIndices( " 
             "const int64_t & cellIndex ) const" );

}


int64_t gkg::HypercubeIndexMapper::getCellIndex( int32_t sampleIndex ) const
{

  try
  {

    return _sampleToCellIndices[ sampleIndex ];

  }
  GKG_CATCH( "int32_t gkg::HypercubeIndexMapper::getCellIndex( "
             "int32_t sampleIndex ) const" );

}


int64_t gkg::HypercubeIndexMapper::getReorderedCellIndex( int32_t sampleIndex ) const
{

  try
  {

    return _reorderedSampleToCellIndices[ sampleIndex ];

  }
  GKG_CATCH( "int32_t gkg::HypercubeIndexMapper::getReorderedCellIndex( "
             "int32_t sampleIndex ) const" );

}


const gkg::Matrix& gkg::HypercubeIndexMapper::getReorderedSamples() const
{

  try
  {

    return _reorderedSamples;

  }
  GKG_CATCH( "const gkg::Matrix& "
             "gkg::HypercubeIndexMapper::getReorderedSamples() const" );

}


int32_t gkg::HypercubeIndexMapper::regionQuery(
                           int32_t sampleIndex,
                           const std::vector< int32_t >& neighborSampleIndices,
                           const std::vector< int32_t >& clusterLabels,
                           std::vector< int32_t >& minSampleArea ) const
{

  try
  {

    int32_t clusterLabel = sampleIndex + 1;

    double distanceSquare = 0.0;
    std::vector< int32_t >::const_iterator
      n = neighborSampleIndices.begin(),
      ne = neighborSampleIndices.end();
    while ( n != ne )
    {

      // computing distance to other point
      distanceSquare = _distanceFunctor->getDistanceSquare( _reorderedSamples,
                                                            sampleIndex,
                                                            *n );

      // add it to the vector with in range points
      if ( distanceSquare <= _maximumDistanceSquare )
      {

        int32_t neighborSampleLabel = clusterLabels[ *n ];
        minSampleArea.push_back( *n );

        // if neighbor point has an assigned label and it is a core, determine 
        // what label to take
        if ( ( neighborSampleLabel !=
               gkg::HypercubeIndexMapper::Unclassified ) &&
             ( neighborSampleLabel < 0 ) )
        {

          clusterLabel = std::min( clusterLabel,
                                   std::abs( neighborSampleLabel ) );

        }

      }

      ++ n;

    }

    return clusterLabel;

  }
  GKG_CATCH( "int32_t gkg::HypercubeIndexMapper::regionQuery( "
             "int32_t sampleIndex, "
             "const std::vector< int32_t >& neighborSampleIndices, "
             "const std::vector< int32_t >& clusterLabels, "
             "std::vector< int32_t >& min_points_area ) const" );

}


void gkg::HypercubeIndexMapper::recoverInitialOrder(
                                   std::vector< int32_t >& clusterLabels ) const
 {

  try
  {

    // creating a temporary vector of cluster id(s)
    std::vector< int32_t >
      localClusterLabels( _sampleCount,
                          gkg::HypercubeIndexMapper::Unclassified );

    // copying back the clustering results taking into account the reordering
    #pragma omp parallel for
    for ( int32_t s = 0; s < _sampleCount; s++ )
    {

      localClusterLabels[ _reorderedSampleOrder[ s ] ] = clusterLabels[ s ];

    }

    // copying back the local cluster vector to the output cluster vector
    clusterLabels.swap( localClusterLabels );

  }
  GKG_CATCH( "void gkg::HypercubeIndexMapper::recoverInitialOrder( "
             "std::vector< int32_t >& clusterLabels )" );

}
