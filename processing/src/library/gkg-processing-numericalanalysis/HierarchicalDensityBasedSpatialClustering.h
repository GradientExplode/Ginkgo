#ifndef _gkg_processing_numericalanalysis_HierarchicalDensityBasedSpatialClustering_h_
#define _gkg_processing_numericalanalysis_HierarchicalDensityBasedSpatialClustering_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <vector>
#include <list>
#include <tuple> 
#include <set>


namespace gkg
{


////////////////////////////////////////////////////////////////////////////////
// class OutlierScore
////////////////////////////////////////////////////////////////////////////////

class OutlierScore
{

  public:

    OutlierScore( double theScore,
                  double coreDistance,
                  int32_t theId ) : score( theScore ),
                                    id( theId ),
                                    _coreDistance( coreDistance )
    {
    }
    OutlierScore() : score( 0.0 ),
                     id( -1 ),
                     _coreDistance( 0.0 )
    {
    }


    bool operator<( const OutlierScore& other ) const
    {

      return ( std::tie( score, _coreDistance, id ) <
               std::tie( other.score, other._coreDistance, other.id ) );

    }

    double score;
    int32_t id;

  private:

    double _coreDistance;


};


////////////////////////////////////////////////////////////////////////////////
// class UndirectedGraph
////////////////////////////////////////////////////////////////////////////////

class UndirectedGraph
{

  public:

    UndirectedGraph( int32_t vertexCount, 
                     const std::vector< int32_t >& aVertices,
                     const std::vector< int32_t >& bVertices,
                     const std::vector< double >& edgeWeights );

    void quickSortByEdgeWeight();

    int32_t getVertexCount() const;
    int32_t getEdgeCount() const;

    int32_t getFirstVertexAtIndex( int32_t index ) const;
    int32_t getSecondVertexAtIndex( int32_t index ) const;

    double getEdgeWeightAtIndex( int32_t index ) const;

    const std::vector< int32_t >&
      getEdgeListForVertex( int32_t index ) const;

    void removeEdgeFromList( int32_t edgeIndex, int32_t index );

  protected:

    int32_t selectPivotIndex( int32_t startIndex,
                              int32_t endIndex ) const;
    int32_t partition( int32_t startIndex,
                       int32_t endIndex,
                       int32_t pivotIndex );
    void swapEdges( int32_t firstIndex, int32_t secondIndex );

    int32_t _vertexCount;
    std::vector< int32_t > _aVertices;
    std::vector< int32_t > _bVertices;
    std::vector< double > _edgeWeights;
    std::vector< std::vector< int32_t > > _edges;

};


////////////////////////////////////////////////////////////////////////////////
// class BitSet
////////////////////////////////////////////////////////////////////////////////

class BitSet
{

  public:

    BitSet()
    {
    }

    void set( int32_t position )
    {

      if ( position >= ( int32_t )_bits.size() )
      {

        _bits.resize( position + 64, false );

      }          
      _bits[ position ] = true;

    }

    bool get( int32_t position ) const
    {

      return ( ( position < ( int32_t )_bits.size() ) &&
               _bits[ position ] );

    }

  private:

    std::vector< bool > _bits;

};


////////////////////////////////////////////////////////////////////////////////
// class Cluster
////////////////////////////////////////////////////////////////////////////////

class Cluster
{

  public:

    Cluster( int32_t& clusterCounter );
    Cluster( int32_t& clusterCounter,
             int32_t label,
             Cluster* parent,
             double birthLevel,
             int32_t itemCount );

    bool operator==( const Cluster& other ) const;

    void detachPoints( int32_t itemCount, double level );
    void propagate();
    void addItemsToVirtualChildCluster( const std::set< int32_t >& items );
    bool virtualChildClusterConstraintsItem( int32_t itemIndex ) const;
    void addVirtualChildConstraintsSatisfied( int32_t constraintCount );
    void addConstraintsSatisfied( int32_t constraintCount );
    void releaseVirtualChildCluster();

    int32_t getClusterId() const;

    Cluster* getParent() const;

    void setHasChildren( bool value );
    bool hasChildren() const;

    void setPropagatedLowestChildDeathLevel( double value );
    double getPropagatedLowestChildDeathLevel() const;

    void addPropagatedConstraintsSatisfiedCount( int32_t count );

    void addPropagatedStability( double value );

    void addPropagatedDescendants( Cluster* other );
    void addPropagatedDescendants( const std::vector< Cluster* >& 
                                                    propagatedDescendants );

    void setHierarchyPosition( int32_t hierarchyPosition );
    int32_t getHierarchyPosition() const;

    int32_t getLabel() const;
    double getStability() const;

    const std::vector< Cluster* >& getPropagatedDescendants() const; 

    void display() const;

  private:

    int32_t& _clusterCounter;

    int32_t _id;
    double _birthLevel;
    double _deathLevel;
    int32_t _itemCount;
    double _propagatedStability;
    int32_t _constraintsSatisfiedCount;
    int32_t _propagatedConstraintsSatisfiedCount;
    std::set< int32_t > _virtualChildCluster;


    std::vector< Cluster* > _propagatedDescendants;
    double _propagatedLowestChildDeathLevel;
    Cluster* _parent;
    double _stability;
    bool _hasChildren;
    int32_t _label;
    int32_t _hierarchyPosition;   // first level where points with this 
                                  // cluster's label appear


};


////////////////////////////////////////////////////////////////////////////////
// class Constraint
////////////////////////////////////////////////////////////////////////////////

class Constraint
{

  public:

    enum Type
    {

      MustLink,
      CannotLink

    };

    Constraint( int32_t itemA, int32_t itemB, Type type ) 
              : _itemA( itemA ),
                _itemB( itemB ),
                _type( type )
    {
    }

    int32_t getItemA() const { return _itemA; }
    int32_t getItemB() const { return _itemB; }
    Type getType() const { return _type; }

  private:

    int32_t _itemA;
    int32_t _itemB;
    Type _type;

};



////////////////////////////////////////////////////////////////////////////////
// class DistanceOperator
////////////////////////////////////////////////////////////////////////////////

template < class I >
class DistanceOperator
{

  public:

    DistanceOperator();

    double get( const I& item1, const I& item2 ) const;

};


////////////////////////////////////////////////////////////////////////////////
// class CoreDistanceGauge
////////////////////////////////////////////////////////////////////////////////

class CoreDistanceGauge : public ThreadGauge< int32_t >
{

  public:

    CoreDistanceGauge( int32_t maxCount );

    void add( int32_t value );
    void reset();

  private:

    void display();

    int32_t _count;
    int32_t _maxCount;
    bool _alreadyDisplayed;

};


////////////////////////////////////////////////////////////////////////////////
// class CoreDistanceContext
////////////////////////////////////////////////////////////////////////////////

template < class I >
class CoreDistanceContext : public LoopContext< int32_t >
{

  public:

    CoreDistanceContext( CoreDistanceGauge& gauge,
                         const std::vector< I >& items,
                         const int32_t& neighborCount,
                         const gkg::DistanceOperator< I >& distanceOperator,
                         std::vector< double >& coreDistances,
                         const bool& verbose );

    void doIt( int32_t startIndex, int32_t count );


  private:

    const std::vector< I >& _items;
    const int32_t& _neighborCount;
    const gkg::DistanceOperator< I >& _distanceOperator;
    std::vector< double >& _coreDistances;
    const bool& _verbose;

    int32_t _gaugeUpdatePeriod;

};


////////////////////////////////////////////////////////////////////////////////
// class MutualReachabilityContext
////////////////////////////////////////////////////////////////////////////////

template < class I >
class MutualReachabilityContext : public LoopContext< int32_t >
{

  public:

    MutualReachabilityContext(
                             const int32_t& currentItem,
                             const std::vector< I >& items,
                             const gkg::BitSet& attachedItems,
                             const gkg::DistanceOperator< I >& distanceOperator,
                             const std::vector< double >& coreDistances,
                             std::vector< double >& nearestMRDDistances,
                             std::vector< int32_t >& nearestMRDNeighbors,
                             double& nearestMRDDistance,
                             int32_t& nearestMRDItem );

    void doIt( int32_t startIndex, int32_t count );


  private:

    const int32_t& _currentItem;
    const std::vector< I >& _items;
    const gkg::BitSet& _attachedItems;
    const gkg::DistanceOperator< I >& _distanceOperator;
    const std::vector< double >& _coreDistances;
    std::vector< double >& _nearestMRDDistances;
    std::vector< int32_t >& _nearestMRDNeighbors;
    double& _nearestMRDDistance;
    int32_t& _nearestMRDItem;

};


////////////////////////////////////////////////////////////////////////////////
// class HierarchicalDensityBasedSpatialClustering
////////////////////////////////////////////////////////////////////////////////


template < class I >
class HierarchicalDensityBasedSpatialClustering
{

  public:

    typedef std::list< std::list< int32_t > >::const_iterator const_iterator;

    HierarchicalDensityBasedSpatialClustering(
                        const std::vector< I >& items,
                        const DistanceOperator< I >& distanceOperator,
                        int32_t minimumPointCount,
                        int32_t minimumClusterSize,
                        const std::vector< Constraint >* constraints = 0,
                        bool verbose = false );
    virtual ~HierarchicalDensityBasedSpatialClustering();

    const_iterator begin() const;
    const_iterator end() const;

    int32_t getClusterCount() const;
    int32_t getNoisyItemCount() const;
    const std::list< int32_t >& getIsolatedItems() const;

    int32_t getLabel( int32_t index ) const;
    const std::vector< int32_t >& getLabels() const;

    bool hasInfiniteStability() const;

    void displayResults() const;

  protected:

    void calculateConstraintsSatisfiedCount(
                                 const std::vector< Constraint >& constraints,
                                 std::set< int32_t >& newClusterLabels,
                                 std::vector< Cluster* >& clusters,
                                 std::vector< int32_t >& clusterLabels );
    Cluster* createNewCluster( int32_t& clusterCounter,
                               const std::set< int32_t >& items,
                               std::vector< int32_t >& clusterLabels,
                               Cluster* parent,
                               int32_t clusterLabel,
                               double edgeWeight );
    void propagateTree( const std::vector< Cluster* >& clusters );
    void findProminentClusters(
                       const std::vector< Cluster* >& clusters,
                       const std::vector< std::vector< int32_t > >& hierarchy );
    void computeMembershipProbabilities(
                           const std::vector< int32_t >& clusterIds,
                           const std::vector< double >& coreDistances );

    int32_t _minimumPointCount;
    int32_t _minimumClusterSize;

    std::vector< int32_t > _labels;

    bool _hasInfiniteStability;
    std::vector< double > _membershipProbabilities;
    std::vector< OutlierScore > _outlierScores;

    uint32_t _noisyItemCount;
    uint32_t _clusterCount;

    std::list< std::list< int32_t > > _clusters;
    std::list< int32_t > _isolatedItems;

};



}



#endif

