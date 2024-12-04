#include <gkg-simulation-glia-population/GlialCell.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-glia-population/GliaPopulation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::GlialCell::GlialCell( 
                      int32_t id,
                      const GliaPopulation* parent,
                      gkg::Atom* somaAtom, 
                      float totalRadius,
                      int32_t processNodeCount,
                      float processBalancingFactor )
               : gkg::Cell( id, parent ),
                 _totalRadius( totalRadius ),
                 _processNodeCount( processNodeCount ),
                 _processBalancingFactor( processBalancingFactor )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // adding the soma sphere atom
    ////////////////////////////////////////////////////////////////////////////

    _atoms.push_back( somaAtom );
    
    ////////////////////////////////////////////////////////////////////////////
    // adding the soma atom index to the set of atom indices
    // not belonging to branches
    ////////////////////////////////////////////////////////////////////////////
    
    _notBelongingToBranchAtomIndices.insert( 0 );

  }
  GKG_CATCH( "gkg::GlialCell::GlialCell( "
             "int32_t id, "
             "const gkg::Atom* somaAtom, "
             "float totalRadius, "
             "int32_t processNodeCount, "
             "float processBalancingFactor )" );


}


gkg::GlialCell::GlialCell( const GlialCell& other )
               : gkg::Cell( other ),
                 _totalRadius( other._totalRadius ),
                 _processNodeCount( other._processNodeCount ),
                 _processBalancingFactor( other._processBalancingFactor ),
                 _processEuclideanMinimumSpanningTree(
                                    other._processEuclideanMinimumSpanningTree )
{
}


gkg::GlialCell::~GlialCell()
{
}


gkg::GlialCell& gkg::GlialCell::operator=( const gkg::GlialCell& other )
{

  try
  {

    this->::gkg::Cell::operator=( other );

    _totalRadius = other._totalRadius;
    _processNodeCount = other._processNodeCount;
    _processBalancingFactor = other._processBalancingFactor;
    _processEuclideanMinimumSpanningTree = 
                                     other._processEuclideanMinimumSpanningTree;

    return *this;

  }
  GKG_CATCH( "gkg::GlialCell& gkg::GlialCell::operator=( "
             "const gkg::GlialCell& other )" );


}


gkg::Vector3d< float > gkg::GlialCell::getCenter() const
{

  try
  {

    return _atoms[ 0 ]->getCenter();

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::GlialCell::getCenter() const" );

}


float gkg::GlialCell::getSomaRadius() const
{

  try
  {

    return static_cast< gkg::SphereAtom* >( _atoms[ 0 ] )->getRadius();

  }
  GKG_CATCH( "float gkg::GlialCell::getSomaRadius() const" );

}


float gkg::GlialCell::getTotalRadius() const
{

  try
  {

    return _totalRadius;

  }
  GKG_CATCH( "float gkg::GlialCell::getTotalRadius() const" );

}


int32_t gkg::GlialCell::getProcessNodeCount() const
{

  try
  {

    return _processNodeCount;

  }
  GKG_CATCH( "int32_t gkg::GlialCell::getProcessNodeCount() const" );

}


float gkg::GlialCell::getProcessBalancingFactor() const
{

  try
  {

    return _processBalancingFactor;

  }
  GKG_CATCH( "float gkg::GlialCell::getProcessBalancingFactor() const" );

}


float gkg::GlialCell::getVolume() const
{

  try
  {

    // approximate the volume to the soma only (needs to be improved)
    return _atoms[ 0 ]->getVolume();

  }
  GKG_CATCH( "float gkg::GlialCell::getVolume() const" );

}


void gkg::GlialCell::addDetails()
{

  try
  {

    // nothing to do

  }
  GKG_CATCH( "void gkg::GlialCell::addDetails()" );


}


void gkg::GlialCell::prepareMinimumSpanningTree(
                     int32_t randomNodeCount,
                     const std::vector< gkg::Vector3d< float > >* imposedNodes )
{

  try
  {

    // pointing to the num. analysis factory and creating a random generator
    gkg::NumericalAnalysisImplementationFactory* 
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // collecting imposed node count
    int32_t imposedNodeCount = 0;
    if ( imposedNodes )
    {

      imposedNodeCount = ( int32_t )imposedNodes->size();

    }

    // computing total node count
    int32_t totalNodeCount = randomNodeCount + imposedNodeCount;

    // allocating node vector
    std::vector< gkg::Vector3d< float > > nodes( totalNodeCount );

    // setting first node to be (0,0,0)
    gkg::Vector3d< float >& firstNode = nodes[ 0 ];
    firstNode.x = 0.0f;
    firstNode.y = 0.0f;
    firstNode.z = 0.0f;

    // computing the random node(s)
    int32_t n = 0;
    float phi = 0.0f;
    float theta = 0.0f;
    float radius = 0.0f;
    for ( n = 1; n < randomNodeCount; n++ )
    {

      phi = ( float )factory->getUniformRandomNumber( randomGenerator,
                                                       0.0,
                                                       2.0 * M_PI );
      theta = std::acos( ( float )factory->getUniformRandomNumber(
                                                       randomGenerator,
                                                       -1.0,
                                                       +1.0 ) );
      radius = static_cast< gkg::SphereAtom* >( _atoms[ 0 ] )->getRadius() +
               ( _totalRadius - 
                 static_cast< gkg::SphereAtom* >( _atoms[ 0 ] )->getRadius() ) *
               std::cbrt( ( float )factory->getUniformRandomNumber(
                                                       randomGenerator,
                                                       0.0,
                                                       1.0 ) );
      gkg::Vector3d< float >& node = nodes[ n ];
      node.x = radius * std::sin( theta ) * std::cos( phi );
      node.y = radius * std::sin( theta ) * std::sin( phi );
      node.z = radius * std::cos( theta );

    }
    
    // copying the imposed node(s)
    for ( n = 0; n < imposedNodeCount; n++ )
    {

      nodes[ randomNodeCount + n ] = ( *imposedNodes )[ n ];

    }

    _processEuclideanMinimumSpanningTree.reset(
             new gkg::EuclideanMinimumSpanningTree( nodes,
                                                    _processBalancingFactor ) );

  }
  GKG_CATCH( "void gkg::GlialCell::prepareMinimumSpanningTree( "
             "int32_t randomNodeCount, "
             "const std::vector< gkg::Vector3d< float > >* imposedNodes )" );


}
