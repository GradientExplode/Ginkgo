#include <gkg-processing-numericalanalysis/DensityBasedSpatialClustering.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <limits>


#define NOISE        -2
#define UNCLASSIFIED -1


gkg::DensityBasedSpatialClustering::DensityBasedSpatialClustering(
              const gkg::Matrix& matrix,
              const gkg::RCPointer< gkg::TestFunction< double > >& testFunction,
              int32_t minimumClusterSize )
                                   : _testFunction( testFunction ),
                                     _minimumClusterSize( minimumClusterSize ),
                                     _labels( matrix.getSize1(), UNCLASSIFIED )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( matrix.getSize1() != matrix.getSize2() )
    {

      throw std::runtime_error( "not a square matrix" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over matrix lines
    ////////////////////////////////////////////////////////////////////////////

    int32_t clusterId = 1;
    int32_t index = 0;
    int32_t count = ( int32_t )matrix.getSize1();

    std::cout << "[ " << std::setw( 7 ) << index
              << " / " << std::setw( 7 ) << count
              << " ]" << std::flush;

    for ( index = 0; index < count; index++ )
    {

      if ( ( index % 100 == 0 ) || ( index == count - 1 ) )
      {

        std::cout << gkg::Eraser( 37 );
        std::cout << "[ " << std::setw( 15 ) << index + 1
                  << " / " << std::setw( 15 ) << count
                  << " ]" << std::flush;

      }

      if ( _labels[ index ] == UNCLASSIFIED )
      {

        if ( this->expandCluster( matrix, index, clusterId ) )
        {

          ++ clusterId;

        }

      }

    }

    std::cout << gkg::Eraser( 37 );


    ////////////////////////////////////////////////////////////////////////////
    // filling clusters and isolated items
    ////////////////////////////////////////////////////////////////////////////

    int32_t clusterCount = clusterId - 1;

    std::vector< std::list< int32_t > > clusters( clusterCount );
    for ( index = 0; index < count; index++ )
    {

      if ( _labels[ index ] < 0 )
      {

        _isolatedItems.push_back( index );

      }
      else
      {

        clusters[ _labels[ index ] - 1 ].push_back( index );

      }

    }

    std::vector< std::list< int32_t > >::const_iterator
      c = clusters.begin(),
      ce = clusters.end();
    while ( c != ce )
    {

      if ( !c->empty() )
      {

        _clusters.push_back( *c );

      }
      ++ c;

    }

  }
  GKG_CATCH( "gkg::DensityBasedSpatialClustering::"
             "DensityBasedSpatialClustering( "
             "const gkg::Matrix& matrix, "
             "const gkg::RCPointer< gkg::TestFunction< double > >& "
             "testFunction, "
             "int32_t minimumClusterSize )" );

}


gkg::DensityBasedSpatialClustering::DensityBasedSpatialClustering(
              const gkg::SparseMatrix& sparseMatrix,
              const gkg::RCPointer< gkg::TestFunction< double > >& testFunction,
              int32_t minimumClusterSize )
                                   : _testFunction( testFunction ),
                                     _minimumClusterSize( minimumClusterSize ),
                                     _labels( sparseMatrix.getSize1(),
                                              UNCLASSIFIED )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( sparseMatrix.getSize1() != sparseMatrix.getSize2() )
    {

      throw std::runtime_error( "not a square sparse matrix" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over matrix lines
    ////////////////////////////////////////////////////////////////////////////


    int32_t clusterId = 1;
    int32_t index = 0;
    int32_t count = ( int32_t )sparseMatrix.getSize1();

    std::cout << "[ " << std::setw( 7 ) << index
              << " / " << std::setw( 7 ) << count
              << " ]" << std::flush;

    for ( index = 0; index < count; index++ )
    {

      if ( ( index % 100 == 0 ) || ( index == count - 1 ) )
      {

        std::cout << gkg::Eraser( 21 );
        std::cout << "[ " << std::setw( 7 ) << index + 1
                  << " / " << std::setw( 7 ) << count
                  << " ]" << std::flush;

      }

      if ( _labels[ index ] == UNCLASSIFIED )
      {

        if ( this->expandCluster( sparseMatrix, index, clusterId ) )
        {

          ++ clusterId;

        }

      }

    }

    std::cout << gkg::Eraser( 21 );


    ////////////////////////////////////////////////////////////////////////////
    // filling clusters and isolated items
    ////////////////////////////////////////////////////////////////////////////

    int32_t clusterCount = clusterId - 1;

    std::vector< std::list< int32_t > > clusters( clusterCount );
    for ( index = 0; index < count; index++ )
    {

      if ( _labels[ index ] < 0 )
      {

        _isolatedItems.push_back( index );

      }
      else
      {

        clusters[ _labels[ index ] - 1 ].push_back( index );

      }

    }

    std::vector< std::list< int32_t > >::const_iterator
      c = clusters.begin(),
      ce = clusters.end();
    while ( c != ce )
    {

      if ( !c->empty() )
      {

        _clusters.push_back( *c );

      }
      ++ c;

    }

  }
  GKG_CATCH( "gkg::DensityBasedSpatialClustering::"
             "DensityBasedSpatialClustering( "
             "const gkg::SparseMatrix& sparseMatrix, "
             "const gkg::RCPointer< gkg::TestFunction< double > >& "
             "testFunction, "
             "int32_t minimumClusterSize )" );

}


gkg::DensityBasedSpatialClustering::~DensityBasedSpatialClustering()
{
}


gkg::DensityBasedSpatialClustering::const_iterator 
gkg::DensityBasedSpatialClustering::begin() const
{

  try
  {

    return _clusters.begin();

  }
  GKG_CATCH( "gkg::DensityBasedSpatialClustering::const_iterator "
             "gkg::DensityBasedSpatialClustering::begin() const" );

}


gkg::DensityBasedSpatialClustering::const_iterator 
gkg::DensityBasedSpatialClustering::end() const
{

  try
  {

    return _clusters.end();

  }
  GKG_CATCH( "gkg::DensityBasedSpatialClustering::const_iterator "
             "gkg::DensityBasedSpatialClustering::end() const" );

}



int32_t gkg::DensityBasedSpatialClustering::getClusterCount() const
{

  try
  {

    return ( int32_t )_clusters.size();

  }
  GKG_CATCH( "int32_t "
             "gkg::DensityBasedSpatialClustering::getClusterCount() const" );

}


const std::list< int32_t >&
gkg::DensityBasedSpatialClustering::getIsolatedItems() const
{

  try
  {

    return _isolatedItems;

  }
  GKG_CATCH( "const std::list< int32_t >& "
             "gkg::MaximumDistanceBasedCentroidPartitioner::"
             "getIsolatedCentroids() const" );

}


int32_t gkg::DensityBasedSpatialClustering::getLabel( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) ||
         ( index >= ( int32_t )_labels.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    return _labels[ index ];

  }
  GKG_CATCH( "int32_t gkg::DensityBasedSpatialClustering::getLabel( "
             "int32_t index ) const" );

}


const std::vector< int32_t >& 
gkg::DensityBasedSpatialClustering::getLabels() const
{

  try
  {

    return _labels;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::DensityBasedSpatialClustering::getLabels() const" );

}


bool gkg::DensityBasedSpatialClustering::expandCluster(
                                                      const gkg::Matrix& matrix,
                                                      int32_t index,
                                                      int32_t clusterId )
{

  try
  {

    std::list< int32_t > seeds;
    this->computeCluster( matrix, index, seeds );
    int32_t seedCount = ( int32_t )seeds.size();

    if ( seedCount < _minimumClusterSize )
    {

      _labels[ index ] = NOISE;
      return false;

    }
    else
    {

      int32_t seedIndex = 0;
      int32_t coreSeedIndex = 0;

      std::list< int32_t >::const_iterator
        s = seeds.begin(),
        se = seeds.end();
      while ( s != se )
      {

        _labels[ *s ] = clusterId;
        if ( *s == index )
        {

          coreSeedIndex = seedIndex;

        }
        ++ seedIndex;
        ++ s;

      }
      std::list< int32_t >::iterator cs = seeds.begin();
      std::advance( cs, coreSeedIndex );
      seeds.erase( cs );
      -- seedCount;

      int32_t neighborCount = 0;
      s = seeds.begin();
      while ( s != seeds.end() )
      {

        std::list< int32_t > neighbors;
        this->computeCluster( matrix, *s, neighbors );

        neighborCount = ( int32_t )neighbors.size();

        if ( neighborCount >= _minimumClusterSize )
        {

          std::list< int32_t >::const_iterator
            n = neighbors.begin(),
            ne = neighbors.end();
          while ( n != ne )
          {

            if ( ( _labels[ *n ] == UNCLASSIFIED ) ||
                 ( _labels[ *n ] == NOISE ) )
            {

              if ( _labels[ *n ] == UNCLASSIFIED )
              {

                seeds.push_back( *n );

              }
              _labels[ *n ] = clusterId;

            }
            ++ n;

          }

        }

        ++ s;

      }

    }
    return true;

  }
  GKG_CATCH( "bool gkg::DensityBasedSpatialClustering::explandCluster( "
             "const gkg::Matrix& matrix, "
             "int32_t index, "
             "int32_t clusterId )" );

}


void gkg::DensityBasedSpatialClustering::computeCluster(
                                             const gkg::Matrix& matrix,
                                             int32_t index,
                                             std::list< int32_t >& seeds ) const
{

  try
  {

    seeds.clear();

    int32_t count = ( int32_t )matrix.getSize1();
    int32_t otherIndex = 0;
    for ( otherIndex = 0; otherIndex < count; otherIndex++ )
    {

      if ( _testFunction->getState( matrix( index, otherIndex ) ) )
      {

        seeds.push_back( otherIndex );

      }

    }

  }
  GKG_CATCH( "void gkg::DensityBasedSpatialClustering::computeCluster( "
             "const gkg::Matrix& matrix, "
             "int32_t index, "
             "std::list< int32_t >& seeds ) const" );

}


bool gkg::DensityBasedSpatialClustering::expandCluster(
                                          const gkg::SparseMatrix& sparseMatrix,
                                          int32_t index,
                                          int32_t clusterId )
{

  try
  {

    std::list< int32_t > seeds;
    this->computeCluster( sparseMatrix, index, seeds );
    int32_t seedCount = ( int32_t )seeds.size();

    if ( seedCount < _minimumClusterSize )
    {

      _labels[ index ] = NOISE;
      return false;

    }
    else
    {

      int32_t seedIndex = 0;
      int32_t coreSeedIndex = 0;

      std::list< int32_t >::const_iterator
        s = seeds.begin(),
        se = seeds.end();
      while ( s != se )
      {

        _labels[ *s ] = clusterId;
        if ( *s == index )
        {

          coreSeedIndex = seedIndex;

        }
        ++ seedIndex;
        ++ s;

      }
      std::list< int32_t >::iterator cs = seeds.begin();
      std::advance( cs, coreSeedIndex );
      seeds.erase( cs );
      -- seedCount;

      int32_t neighborCount = 0;
      s = seeds.begin();
      while ( s != seeds.end() )
      {

        std::list< int32_t > neighbors;
        this->computeCluster( sparseMatrix, *s, neighbors );

        neighborCount = ( int32_t )neighbors.size();

        if ( neighborCount >= _minimumClusterSize )
        {

          std::list< int32_t >::const_iterator
            n = neighbors.begin(),
            ne = neighbors.end();
          while ( n != ne )
          {

            if ( ( _labels[ *n ] == UNCLASSIFIED ) ||
                 ( _labels[ *n ] == NOISE ) )
            {

              if ( _labels[ *n ] == UNCLASSIFIED )
              {

                seeds.push_back( *n );

              }
              _labels[ *n ] = clusterId;

            }
            ++ n;

          }

        }

        ++ s;

      }

    }
    return true;

  }
  GKG_CATCH( "bool gkg::DensityBasedSpatialClustering::explandCluster( "
             "const gkg::Matrix& matrix, "
             "int32_t index, "
             "int32_t clusterId )" );

}


void gkg::DensityBasedSpatialClustering::computeCluster(
                                          const gkg::SparseMatrix& sparseMatrix,
                                          int32_t index,
                                          std::list< int32_t >& seeds ) const
{

  try
  {

    seeds.clear();

    int32_t count = ( int32_t )sparseMatrix.getSize1();
    int32_t otherIndex = 0;
    for ( otherIndex = 0; otherIndex < count; otherIndex++ )
    {

      if ( sparseMatrix.hasElement( index, otherIndex ) )
      {

        if ( _testFunction->getState( sparseMatrix( index, otherIndex ) ) )
        {

          seeds.push_back( otherIndex );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::DensityBasedSpatialClustering::computeCluster( "
             "const gkg::SparseMatrix& sparseMatrix, "
             "int32_t index, "
             "std::list< int32_t >& seeds ) const" );

}



#undef NOISE
#undef UNCLASSIFIED
