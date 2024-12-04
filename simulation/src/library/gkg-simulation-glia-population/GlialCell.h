#ifndef _gkg_simulation_glia_population_GlialCell_h_
#define _gkg_simulation_glia_population_GlialCell_h_


#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-processing-graph/EuclideanMinimumSpanningTree.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class GliaPopulation;


class GlialCell : public Cell
{

  public:

    GlialCell( int32_t id,
               const GliaPopulation* parent,
               Atom* somaAtom,
               float totalRadius,
               int32_t processNodeCount,
               float processBalancingFactor );
    GlialCell( const GlialCell& other );
    virtual ~GlialCell();

    GlialCell& operator=( const GlialCell& other );

    Vector3d< float > getCenter() const;
    float getSomaRadius() const;
    float getTotalRadius() const;
    int32_t getProcessNodeCount() const;
    float getProcessBalancingFactor() const;

    float getVolume() const;

    void addDetails();

  protected:

    void prepareMinimumSpanningTree(
                     int32_t randomNodeCount,
                     const std::vector< Vector3d< float > >* imposedNodes = 0 );

    float _totalRadius;
    int32_t _processNodeCount;
    float _processBalancingFactor;
    RCPointer< EuclideanMinimumSpanningTree >
                                           _processEuclideanMinimumSpanningTree;

};



}


#endif
