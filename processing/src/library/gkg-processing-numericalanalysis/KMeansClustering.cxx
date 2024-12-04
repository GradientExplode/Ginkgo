#include <gkg-processing-numericalanalysis/KMeansClustering.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <set>
#include <iostream>
#include <iomanip>


////////////////////////////////////////////////////////////////////////////////
// class KMeansPoint
////////////////////////////////////////////////////////////////////////////////

template < class T >
gkg::KMeansPoint< T >::KMeansPoint()
                      : _id( -1 )
{
}


template < class T >
gkg::KMeansPoint< T >::KMeansPoint( int32_t id,
                                    const std::vector< T >& values )
                      : _id( id ),
                        _values( values )
{
}


template < class T >
gkg::KMeansPoint< T >::KMeansPoint( const gkg::KMeansPoint< T >& other )
                      : _id( other._id ),
                        _values( other._values )
{
}


template < class T >
gkg::KMeansPoint< T >::~KMeansPoint()
{
}


template < class T >
gkg::KMeansPoint< T >& 
gkg::KMeansPoint< T >::operator=( const gkg::KMeansPoint< T >& other )
{

  try
  {

    _id = other._id;
    _values = other._values;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::KMeansPoint< T >& "
             "gkg::KMeansPoint::operator=( "
             "const gkg::KMeansPoint< T >& other )" );

}


template < class T >
int32_t gkg::KMeansPoint< T >::getDimension() const
{

  try
  {

    return ( int32_t )_values.size();

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansPoint< T >::getDimension() const" );

}

template < class T >
int32_t gkg::KMeansPoint< T >::getId() const
{

  try
  {

    return _id;

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansPoint< T >::getId() const" );

}


template < class T >
void gkg::KMeansPoint< T >::setId( int32_t id )
{

  try
  {

    _id = id;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansPoint< T >::setId( int32_t id )" );

}


template < class T >
const T& gkg::KMeansPoint< T >::getValue( int32_t coordinate ) const
{

  try
  {

    if ( ( coordinate < 0 ) || ( coordinate >= ( int32_t )_values.size() ) )
    {

      throw std::runtime_error( "coordinate is out of range" );

    }
    return _values[ coordinate ];

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansPoint< T >::getValue( "
             "int32_t coordinate ) const" );

}


template < class T >
void gkg::KMeansPoint< T >::setValues( const std::vector< T >& values )
{

  try
  {

    _values = values;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansPoint< T >::setValues( "
             "const std::vector< T >& values )" );


}


template < class T >
T gkg::KMeansPoint< T >::getSquareDistanceTo(
                                           const KMeansPoint< T >& other ) const
{

  try
  {

    int32_t dimension = this->getDimension();
    int32_t coordinate = 0;
    T distance = ( T )0.0;
    T difference = ( T )0.0;
    for ( coordinate = 0; coordinate< dimension; coordinate++ )
    {

      difference = _values[ coordinate ] - other.getValue( coordinate );
      distance += difference * difference;

    }

    return distance;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::KMeansPoint< T >::getSquareDistanceTo( "
             "const KMeansPoint< T >& other ) const" );


}



////////////////////////////////////////////////////////////////////////////////
// class KMeansCluster
////////////////////////////////////////////////////////////////////////////////

template < class T >
gkg::KMeansCluster< T >::KMeansCluster( int32_t id,
                                        const gkg::KMeansPoint< T >& centroid )
                        : _id( id ),
                          _centroid( centroid )
{

  try
  {

    _kMeansPoints.push_back( _centroid );

  }
  GKG_CATCH( "template < class T > "
             "gkg::KMeansCluster< T >::KMeansCluster( int32_t id, "
             "const gkg::KMeansPoint< T >& centroid )" );

}


template < class T >
gkg::KMeansCluster< T >::KMeansCluster( const KMeansCluster< T >& other )
                        : _id( other._id ),
                          _centroid( other._centroid ),
                          _kMeansPoints( other._kMeansPoints )
{
}


template < class T >
gkg::KMeansCluster< T >::~KMeansCluster()
{
}


template < class T >
gkg::KMeansCluster< T >& 
gkg::KMeansCluster< T >::operator=( const KMeansCluster< T >& other )
{

  try
  {

    _id = other._id;
    _centroid = other._centroid;
    _kMeansPoints = other._kMeansPoints;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::KMeansCluster< T >& "
             "gkg::KMeansCluster< T >::operator=( "
             "const KMeansCluster< T >& other )" );

}


template < class T >
void gkg::KMeansCluster< T >::addPoint( const KMeansPoint< T >& kMeansPoint )
{

  try
  {

    _kMeansPoints.push_back( kMeansPoint );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansCluster< T >::addPoint( "
             "const KMeansPoint< T >& kMeansPoint )" );

}


template < class T >
bool gkg::KMeansCluster< T >::removePoint( int32_t id )
{

  try
  {

    typename std::list< gkg::KMeansPoint< T > >::iterator
      i = _kMeansPoints.begin(),
      ie = _kMeansPoints.end();
    while ( i != ie )
    {

      if ( i->getId() == id )
      {

        _kMeansPoints.erase( i );
        return true;

      }
      ++ i;

    }
    return false;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansCluster< T >::addPoint( "
             "const KMeansPoint< T >& kMeansPoint )" );

}


template < class T >
int32_t gkg::KMeansCluster< T >::getId() const
{

  try
  {

    return _id;

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansCluster< T >::getId() const" );

}


template < class T >
int32_t gkg::KMeansCluster< T >::getPointCount() const
{

  try
  {

    return ( int32_t )_kMeansPoints.size();

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansCluster< T >::getPointCount() const" );

}


template < class T >
const gkg::KMeansPoint< T >& gkg::KMeansCluster< T >::getCentroid() const
{

  try
  {

    return _centroid;

  }
  GKG_CATCH( "template < class T > "
             "const KMeansPoint< T >& "
             "gkg::KMeansCluster< T >::getCentroid() const" );

}


template < class T >
void gkg::KMeansCluster< T >::setCentroid( const KMeansPoint< T >& centroid )
{

  try
  {

    _centroid = centroid;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansCluster< T >::setCentroid( "
             "const KMeansPoint< T >& centroid )" );


}


template < class T >
void gkg::KMeansCluster< T >::updateCentroid()
{

  try
  {

    int32_t dimension = _kMeansPoints.front().getDimension();
    int32_t coordinate = 0;

    std::vector< T > newCentroidValues( dimension, ( T )0 ); 

    typename std::list< KMeansPoint< T > >::const_iterator
      p = _kMeansPoints.begin(),
      pe = _kMeansPoints.end();
    while ( p != pe )
    {

      for ( coordinate = 0; coordinate < dimension; coordinate++ )
      {

        newCentroidValues[ coordinate ] += p->getValue( coordinate );

      }

      ++ p;

    }

    for ( coordinate = 0; coordinate < dimension; coordinate++ )
    {

      newCentroidValues[ coordinate ] /= ( T )_kMeansPoints.size();

    }

    _centroid = gkg::KMeansPoint< T >( 0, newCentroidValues );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansCluster< T >::updateCentroid()" );

}


////////////////////////////////////////////////////////////////////////////////
// class KMeansClustering
////////////////////////////////////////////////////////////////////////////////

template < class T >
gkg::KMeansClustering< T >::KMeansClustering( int32_t clusterCount,
                                              int32_t maximumIterationCount )
                           : _clusterCount( clusterCount ),
                             _maximumIterationCount( maximumIterationCount )
{



}


template < class T >
gkg::KMeansClustering< T >::~KMeansClustering()
{
}


template < class T >
void gkg::KMeansClustering< T >::clusterize(
                             const std::vector< std::vector< T > >& points,
                             std::vector< int32_t >& clusterIds,
                             bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the point count to be clusterized and creating the kMeansPoint
    // vector
    ////////////////////////////////////////////////////////////////////////////

    int32_t kMeansPointCount = ( int32_t )points.size();

    std::vector< gkg::KMeansPoint< T > > kMeansPoints( kMeansPointCount );

    int32_t kMeansPointIndex = 0;
    for ( kMeansPointIndex = 0; kMeansPointIndex < kMeansPointCount;
          kMeansPointIndex++ )
    {

      kMeansPoints[ kMeansPointIndex ].setId( kMeansPointIndex );
      kMeansPoints[ kMeansPointIndex ].setValues( points[ kMeansPointIndex ] );

    }


    ////////////////////////////////////////////////////////////////////////////
    // (re-)allocating the output vector of cluster ids
    ////////////////////////////////////////////////////////////////////////////

    clusterIds = std::vector < int32_t >( kMeansPointCount, 0 );


    ////////////////////////////////////////////////////////////////////////////
    // allocating the cluster container
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating " << _clusterCount << " cluster(s) : "
                << std::flush;

    }

    std::vector< gkg::RCPointer< gkg::KMeansCluster< T > > >
      clusters( _clusterCount );
    
    int32_t clusterIndex = 0;
    int32_t randomPointIndex = 0;
    std::set< int32_t > usedKMeansPointIds;

    for ( clusterIndex = 0; clusterIndex < _clusterCount; clusterIndex++ )
    {

      randomPointIndex = rand() % kMeansPointCount;

      while ( true )
      {

        if ( usedKMeansPointIds.find( randomPointIndex ) == 
             usedKMeansPointIds.end() )
        {

          clusters[ clusterIndex ].reset(
              new gkg::KMeansCluster< T >( clusterIndex + 1,
                                           kMeansPoints[ randomPointIndex ] ) );
          clusterIds[ randomPointIndex ] = clusterIndex + 1;
          usedKMeansPointIds.insert( randomPointIndex );
          break;

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // running the k-means clustering loop
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "running the k-means clustering loop : "
                << "[ " << std::setw( 5 ) << 0 << " / "
                << std::setw( 5 ) << _maximumIterationCount << " ]"
                << std::flush;

    }



    int32_t iteration = 0;
    bool done = false;
    int32_t currentClusterId = 0;
    int32_t nearestClusterId = 0;
    while ( !done && ( iteration < _maximumIterationCount ) )
    {

      done = true;

      if ( verbose )
      {

        std::cout << gkg::Eraser( 17 )
                  << "[ " << std::setw( 5 ) << iteration + 1 << " / "
                  << std::setw( 5 ) << _maximumIterationCount << " ]"
                  << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // adding all points to their nearest cluster
      //////////////////////////////////////////////////////////////////////////

      for ( kMeansPointIndex = 0; kMeansPointIndex < kMeansPointCount;
            kMeansPointIndex ++ )
      {

        currentClusterId = clusterIds[ kMeansPointIndex ];
        nearestClusterId = this->getNearestClusterId(
                                             clusters,
                                             kMeansPoints[ kMeansPointIndex ] );                  

        if ( currentClusterId != nearestClusterId )
        {

          if ( currentClusterId != 0 )
          {

            clusters[ currentClusterId - 1 ]->removePoint(
                                     kMeansPoints[ kMeansPointIndex ].getId() );

          }
          clusters[ nearestClusterId - 1 ]->addPoint( 
                                             kMeansPoints[ kMeansPointIndex ] );
          clusterIds[ kMeansPointIndex ] = nearestClusterId;
          done = false;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // recomputing the centroid of each cluster
      //////////////////////////////////////////////////////////////////////////

      for ( clusterIndex = 0; clusterIndex < _clusterCount; clusterIndex++ )
      {

        clusters[ clusterIndex ]->updateCentroid();

      }

      ++ iteration;

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::KMeansClustering< T >::clusterize( "
             "const std::vector< gkg::KMeansPoint< T > >& points, "
             "std::vector< int32_t >& clusterIds ) const" );


}


template < class T >
int32_t gkg::KMeansClustering< T >::getNearestClusterId(
       const std::vector< gkg::RCPointer< gkg::KMeansCluster< T > > >& clusters,
       const gkg::KMeansPoint< T >& kMeansPoint ) const
{

  try
  {

    int32_t clusterIndex = 0;
    T squareDistance = ( T )0.0;
    T minimumSquareDistance = 1e38;
    int32_t bestClusterId = 0;
    for ( clusterIndex = 0; clusterIndex < _clusterCount; clusterIndex++ )
    {

      squareDistance = kMeansPoint.getSquareDistanceTo(
                                      clusters[ clusterIndex ]->getCentroid() );
      if ( squareDistance < minimumSquareDistance )
      {

        minimumSquareDistance = squareDistance;
        bestClusterId = clusters[ clusterIndex ]->getId();

      }

    }

    return bestClusterId;

  }
  GKG_CATCH( "template < class T > "
             "int32_t gkg::KMeansClustering< T >::getNearestClusterId( "
             "const std::vector< gkg::RCPointer< gkg::KMeansCluster< T > > >& "
             "clusters, "
             "const gkg::KMeansPoint< T >& kMeansPoint ) const" );

}
 


template class gkg::KMeansPoint< float >;
template class gkg::KMeansPoint< double >;

template class gkg::KMeansCluster< float >;
template class gkg::KMeansCluster< double >;

template class gkg::KMeansClustering< float >;
template class gkg::KMeansClustering< double >;
