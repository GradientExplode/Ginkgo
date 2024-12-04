#include <gkg-simulation-glia-population/Astrocyte.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-glia-population/AstrocytePopulation.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Astrocyte::Astrocyte( int32_t id,
                           const AstrocytePopulation* parent,
                           gkg::Atom* somaAtom,
                           float totalRadius,
                           int32_t processNodeCount,
                           float processBalancingFactor,
                           float processAtomOversamplingRatio,
                           int32_t processMinimumSegmentPointCount,
                           float processRadius,
                           bool processHasTortuosity,
                           float processTortuosityMagnitude,
                           float processTortuosityAngularDispersionInDegrees,
                           float processTortuosityWaveLength )
               : gkg::GlialCell( id,
                                 parent,
                                 somaAtom,
                                 totalRadius,
                                 processNodeCount,
                                 processBalancingFactor ),
                 _processAtomOversamplingRatio( processAtomOversamplingRatio ),
                 _processMinimumSegmentPointCount(
                                              processMinimumSegmentPointCount ),
                 _processRadius( processRadius ),
                 _processHasTortuosity( processHasTortuosity ),
                 _processTortuosityMagnitude( processTortuosityMagnitude ),
                 _processTortuosityAngularDispersionInDegrees(
                                  processTortuosityAngularDispersionInDegrees ),
                 _processTortuosityWaveLength( processTortuosityWaveLength )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the soma center, being the center of the astrocyte
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > somaCenter = this->getCenter();
    float somaRadius = static_cast< gkg::SphereAtom* >( somaAtom )->getRadius();


    ////////////////////////////////////////////////////////////////////////////
    // collecting the virtual tissue field of view
    ////////////////////////////////////////////////////////////////////////////

    const gkg::BoundingBox< float >& 
      fieldOfView = _parent->getParent()->getFieldOfView();


    ////////////////////////////////////////////////////////////////////////////
    // computing the minimum spanning tree
    ////////////////////////////////////////////////////////////////////////////

    this->prepareMinimumSpanningTree( _processNodeCount );


    ////////////////////////////////////////////////////////////////////////////
    // computing the process sphere atom(s) from the minimum spanning tree
    ////////////////////////////////////////////////////////////////////////////

    const std::vector< gkg::EuclideanMinimumSpanningTree::Node >&
      nodes = _processEuclideanMinimumSpanningTree->getNodes();
    int32_t n = 0;
    int32_t c = 0;
    int32_t nodeCount = ( int32_t )nodes.size();


    std::list< gkg::Atom* > processAtoms;
    int32_t segmentPointCount = 0;
    gkg::Vector3d< float > processAtomCenter;
    float processAtomRadius = 0.0f;
    float distanceToSomaCenter = 0.0f;
    float growingFactor = 0.0f;
    int32_t childrenCount = 0;


    std::list< std::pair< int32_t, int32_t > > branches;
    // 1 because the index 0 corresponds to the soma
    int32_t processAtomIndex = 1;
    std::pair< int32_t, int32_t > branchAtomIndices;

    for ( n = 0; n < nodeCount; n++ )
    {

      const gkg::EuclideanMinimumSpanningTree::Node& node = nodes[ n ];

      if ( node.hasChildren() )
      {

        // getting the position of the current node
        const gkg::Vector3d< float >& 
          nodePosition = node.getPosition();

        // getting the children of the current node
        const std::vector< gkg::EuclideanMinimumSpanningTree::Node >&
          children = node.getChildren();
        childrenCount = ( int32_t )children.size();

        for ( c = 0; c < childrenCount; c++ )
        {

          // getting the position of the current child
          const gkg::Vector3d< float >& 
            childPosition = children[ c ].getPosition();

          if ( ( childPosition - nodePosition ).getNorm() > 0.0f )
          {

            // computing the segment correspond to [node;child]
            segmentPointCount =
              std::max( ( int32_t )(
                                   _processAtomOversamplingRatio *
                                   ( childPosition - nodePosition ).getNorm() /
                                   _processRadius + 0.5 ),
                        _processMinimumSegmentPointCount );

            gkg::LightCurve3d< float > segment( 2 );

            if ( n == 0 )
            {

              segment.setPoint( 0, ( childPosition - nodePosition ).normalize()
                                   * somaRadius );

            }
            else
            {

              segment.setPoint( 0, nodePosition );

            }
            segment.setPoint( 1, childPosition );
            segment.setEquidistantCurve( segmentPointCount );

            // adding tortuosity to the process segment
            if ( _processHasTortuosity &&
                 /*( segment.getLength() > _processTortuosityWaveLength ) && */
                 segment.getPoint( segmentPointCount - 1 ).getNorm() != 0.0f )
            {

              segment.addTortuosity(
                                   _processTortuosityMagnitude,
                                   _processTortuosityAngularDispersionInDegrees,
                                   _processTortuosityWaveLength,
                                   ( childPosition - nodePosition ).normalize(),
                                   fieldOfView,
                                   5,
                                   100000 );

            }

            branchAtomIndices.first = -1;
            branchAtomIndices.second = -1;

            gkg::LightCurve3d< float >::const_iterator
              p = segment.begin(),
              pe = segment.end();
            while ( p != pe )
            {

              processAtomCenter = somaCenter + *p;
              if ( fieldOfView.contains( processAtomCenter ) )
              {

                if ( processAtomCenter != somaCenter )
                {

                  distanceToSomaCenter = p->getNorm() - somaRadius;
                  growingFactor = 
                    std::max(
                      std::min( ( float )std::exp(
                                               - ( 2.0f * distanceToSomaCenter /
                                      ( _totalRadius - somaRadius) - 0.5f ) ),
                                1.0f ),
                      0.2f );
                  processAtomRadius = growingFactor * _processRadius;
                  processAtoms.push_back( 
                                     new gkg::SphereAtom( processAtomCenter,
                                                          processAtomRadius ) );
                  if ( branchAtomIndices.first == -1 )
                  {
                  
                    branchAtomIndices.first = processAtomIndex;

                  }
                  ++ processAtomIndex;

                }

              }
              ++ p;

            }
            
            if ( branchAtomIndices.first != -1 )
            {
            
              branchAtomIndices.second = processAtomIndex - 1;
              branches.push_back( branchAtomIndices );
              
            }

          }

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding the process atom(s) to the cell structure and one further
    // atom to take into account the soma atom
    ////////////////////////////////////////////////////////////////////////////

    _atoms.reserve( processAtoms.size() + 1U );
    
    std::list< gkg::Atom* >::const_iterator
      a = processAtoms.begin(),
      ae = processAtoms.end();
    while ( a != ae )
    {

      _atoms.push_back( *a );
      ++ a;

    }


    ////////////////////////////////////////////////////////////////////////////
    // then copying back branches to the _branches protected member
    ////////////////////////////////////////////////////////////////////////////

    _branches = std::vector< std::pair< int32_t, int32_t > >( branches.begin(),
                                                              branches.end() );


    ////////////////////////////////////////////////////////////////////////////
    // no need to store the minimum spanning tree anymore
    ////////////////////////////////////////////////////////////////////////////

    _processEuclideanMinimumSpanningTree.reset();

  }
  GKG_CATCH( "gkg::Astrocyte::Astrocyte( "
             "int32_t id, "
             "const AstrocytePopulation* parent, "
             "const gkg::Atom* somaAtom, "
             "float totalRadius, "
             "int32_t processNodeCount, "
             "float processBalancingFactor, "
             "float processAtomOversamplingRatio, "
             "int32_t processMinimumSegmentPointCount, "
             "float processRadius, "
             "bool processHasTortuosity, "
             "float processTortuosityMagnitude," 
             "float processTortuosityAngularDispersionInDegrees, "
             "float processTortuosityWaveLength )" );


}


gkg::Astrocyte::Astrocyte( const Astrocyte& other )
               : gkg::GlialCell( other ),
                 _processAtomOversamplingRatio(
                   other._processAtomOversamplingRatio ),
                 _processMinimumSegmentPointCount(
                   other._processMinimumSegmentPointCount ),
                 _processRadius(
                   other._processRadius ),
                 _processHasTortuosity(
                   other._processHasTortuosity ),
                 _processTortuosityMagnitude(
                   other._processTortuosityMagnitude ),
                 _processTortuosityAngularDispersionInDegrees(
                   other._processTortuosityAngularDispersionInDegrees ),
                 _processTortuosityWaveLength(
                   other._processTortuosityWaveLength )
{
}


gkg::Astrocyte::~Astrocyte()
{
}


gkg::Astrocyte& gkg::Astrocyte::operator=( const gkg::Astrocyte& other )
{

  try
  {

    this->::gkg::GlialCell::operator=( other );


    _processAtomOversamplingRatio = other._processAtomOversamplingRatio;
    _processMinimumSegmentPointCount = other._processMinimumSegmentPointCount;
    _processRadius = other._processRadius;
    _processHasTortuosity = other._processHasTortuosity;
    _processTortuosityMagnitude = other._processTortuosityMagnitude;
    _processTortuosityAngularDispersionInDegrees =
                             other._processTortuosityAngularDispersionInDegrees;
    _processTortuosityWaveLength = other._processTortuosityWaveLength;

    return *this;

  }
  GKG_CATCH( "gkg::Astrocyte& gkg::Astrocyte::operator=( "
             "const gkg::Astrocyte& other )" );


}


float gkg::Astrocyte::getProcessAtomOversamplingRatio() const
{

  try
  {

    return _processAtomOversamplingRatio;

  }
  GKG_CATCH( "float "
             "gkg::Astrocyte::getProcessAtomOversamplingRatio() const" );

}


int32_t gkg::Astrocyte::getProcessMinimumSegmentPointCount() const
{

  try
  {

    return _processMinimumSegmentPointCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::Astrocyte::getProcessMinimumSegmentPointCount() const" );

}


float gkg::Astrocyte::getProcessRadius() const
{

  try
  {

    return _processRadius;

  }
  GKG_CATCH( "float gkg::Astrocyte::getProcessRadius() const" );

}


bool gkg::Astrocyte::getProcessHasTortuosity() const
{

  try
  {

    return _processHasTortuosity;

  }
  GKG_CATCH( "bool gkg::Astrocyte::getProcessHasTortuosity() const" );

}


float gkg::Astrocyte::getProcessTortuosityMagnitude() const
{

  try
  {

    return _processTortuosityMagnitude;

  }
  GKG_CATCH( "float gkg::Astrocyte::getProcessTortuosityMagnitude() const" );

}


float gkg::Astrocyte::getProcessTortuosityAngularDispersionInDegrees() const
{

  try
  {

    return _processTortuosityAngularDispersionInDegrees;

  }
  GKG_CATCH( "float "
             "gkg::Astrocyte::getProcessTortuosityAngularDispersionInDegrees() "
             "const" );

}


float gkg::Astrocyte::getProcessTortuosityWaveLength() const
{

  try
  {

    return _processTortuosityWaveLength;

  }
  GKG_CATCH( "float gkg::Astrocyte::getProcessTortuosityWaveLength() const" );

}


void gkg::Astrocyte::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    // here, we create the overlapping sphere meshes, and in the future, we
    // should replace by the convex hull of the vertices of these sphere
    // meshes
    std::vector< gkg::Atom* >::const_iterator
      a = _atoms.begin(),
      ae = _atoms.end();
    while ( a != ae )
    {

      const gkg::SphereAtom* sphereAtom = static_cast< gkg::SphereAtom* >( *a );
      gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
        atomMeshMap = gkg::MeshFactory::getInstance().getSphere(
                                                        sphereAtom->getCenter(),
                                                        sphereAtom->getRadius(),
                                                        60 );

      meshAccumulators[ 0 ].add( *atomMeshMap );

      ++ a;

    }

  }
  GKG_CATCH( "void gkg::Astrocyte::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


