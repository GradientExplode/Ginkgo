#ifndef _gkg_processing_numericalanalysis_KMeansClustering_h_
#define _gkg_processing_numericalanalysis_KMeansClustering_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <list>


namespace gkg
{


template < class T >
class KMeansPoint
{

  public:

    KMeansPoint();
    KMeansPoint( int32_t id,
                 const std::vector< T >& values );
    KMeansPoint( const KMeansPoint< T >& other );
    virtual ~KMeansPoint();

    KMeansPoint< T >& operator=( const KMeansPoint< T >& other );

    int32_t getDimension() const;

    int32_t getId() const;
    void setId( int32_t id );

    const T& getValue( int32_t coordinate ) const;
    void setValues( const std::vector< T >& values );

    T getSquareDistanceTo( const KMeansPoint< T >& other ) const;

  protected:

    int32_t _id;
    std::vector< T > _values;

};


template < class T >
class KMeansCluster
{

  public:

    KMeansCluster( int32_t id,
                   const KMeansPoint< T >& centroid );
    KMeansCluster( const KMeansCluster< T >& other );
    virtual ~KMeansCluster();

    KMeansCluster< T >& operator=( const KMeansCluster< T >& other );

    void addPoint( const KMeansPoint< T >& kMeansPoint );
    bool removePoint( int32_t id );

    int32_t getId() const;
    int32_t getPointCount() const;
    const KMeansPoint< T >& getCentroid() const;
    void setCentroid( const KMeansPoint< T >& centroid );

    void updateCentroid();

  protected:

    int32_t _id;
    KMeansPoint< T > _centroid;
    std::list< KMeansPoint< T > > _kMeansPoints;

};



template < class T >
class KMeansClustering
{

  public:

    KMeansClustering( int32_t clusterCount, int32_t maximumIterationCount );
    virtual ~KMeansClustering();

    void clusterize( const std::vector< std::vector< T > >& points,
                     std::vector< int32_t >& clusterIds,
                     bool verbose = false ) const;


  protected:

    int32_t getNearestClusterId(
                 const std::vector< RCPointer< KMeansCluster< T > > >& clusters,
                 const gkg::KMeansPoint< T >& kMeansPoint ) const;


    int32_t _clusterCount;
    int32_t _maximumIterationCount;


};


}


#endif

