#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-virtual-tissue/AtomInterpolatorFactory.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
#include <gkg-core-exception/Exception.h>
#include <vector>


gkg::Cell::Cell()
          : _id( -1 ),
            _parent( 0 )
{
}


gkg::Cell::Cell( int32_t id, const gkg::Population* parent  )
          : _id( id ),
            _parent( parent )
{
}


gkg::Cell::Cell( const gkg::Cell& other )
          : _id( other._id ),
            _parent( other._parent ),
            _notBelongingToBranchAtomIndices(
                   other._notBelongingToBranchAtomIndices ),
            _branches( other._branches ) 
{

  try
  {

    _atoms.resize( other._atoms.size() );
    std::vector< gkg::Atom* >::iterator a = _atoms.begin();
    std::vector< gkg::Atom* >::const_iterator
      oa = other._atoms.begin(),
      oae = other._atoms.end();
    while ( oa != oae )
    {

      *a = ( *oa )->clone();
      ++ oa;
      ++ a;

    }

  }
  GKG_CATCH( "gkg::Cell::Cell( const gkg::Cell& other )" );

}


gkg::Cell::~Cell()
{

  // deleting all atom(s)
  std::vector< gkg::Atom* >::iterator
    a = _atoms.begin(),
    ae = _atoms.end();
  while ( a != ae )
  {

    delete *a;
    ++ a;

  }

}



gkg::Cell&
gkg::Cell::operator=( const gkg::Cell& other )
{

  try
  {

    _id = other._id;
    _parent = other._parent;

    _atoms.resize( other._atoms.size() );
    std::vector< gkg::Atom* >::iterator a = _atoms.begin();
    std::vector< gkg::Atom* >::const_iterator
      oa = other._atoms.begin(),
      oae = other._atoms.end();
    while ( oa != oae )
    {

      *a = ( *oa )->clone();
      ++ oa;
      ++ a;

    }

    _notBelongingToBranchAtomIndices = other._notBelongingToBranchAtomIndices;
    _branches = other._branches;
    
    return *this;

  }
  GKG_CATCH( "inline gkg::Cell& "
             "gkg::Cell::operator=( const gkg::Cell& other )" );

}


gkg::Cell::const_iterator gkg::Cell::begin() const
{

  try
  {

    return _atoms.begin();

  }
  GKG_CATCH( "inline gkg::Cell::const_iterator gkg::Cell::begin() const" );

}


gkg::Cell::const_iterator gkg::Cell::end() const
{

  try
  {

    return _atoms.end();

  }
  GKG_CATCH( "inline gkg::Cell::const_iterator gkg::Cell::end() const" );

}


gkg::Cell::iterator gkg::Cell::begin()
{

  try
  {

    return _atoms.begin();

  }
  GKG_CATCH( "inline gkg::Cell::iterator gkg::Cell::begin()" );

}


gkg::Cell::iterator gkg::Cell::end()
{

  try
  {

    return _atoms.end();

  }
  GKG_CATCH( "inline gkg::Cell::iterator gkg::Cell::end()" );

}


void gkg::Cell::setId( int32_t id )
{

  try
  {

    _id = id;

  }
  GKG_CATCH( "void gkg::Cell::setId( int32_t id )" );

}


int32_t gkg::Cell::getId() const
{

  try
  {

    return _id;

  }
  GKG_CATCH( "int32_t gkg::Cell::getId() const" );

}


void gkg::Cell::setParent( const gkg::Population* population )
{

  try
  {

    _parent = population;

  }
  GKG_CATCH( "void gkg::Cell::setParent( const gkg::Population* population )" );

}


const gkg::Population* gkg::Cell::getParent() const
{

  try
  {

    return _parent;

  }
  GKG_CATCH( "const gkg::Population* gkg::Cell::getParent() const" );

}


int32_t gkg::Cell::getAtomCount() const
{

  try
  {

    return ( int32_t )_atoms.size();


  }
  GKG_CATCH( "int32_t gkg::Cell::getAtomCount() const" );

}


uint64_t gkg::Cell::getAtomParameterCount() const
{

  try
  {


    uint64_t atomParameterCount = 0U;
    std::vector< gkg::Atom* >::const_iterator
      a = _atoms.begin(),
      ae = _atoms.end();
    while ( a != ae )
    {

      atomParameterCount += ( uint64_t )( *a )->getParameterCount();
      ++ a;

    }    
    return atomParameterCount;


  }
  GKG_CATCH( "uint64_t gkg::Cell::getAtomParameterCount() const" );

}


const std::vector< gkg::Atom* >& gkg::Cell::getAtoms() const
{

  try
  {

    return _atoms;

  }
  GKG_CATCH( "const std::vector< gkg::Atom* >& "
             "gkg::Cell::getAtoms() const" );


}


const gkg::Atom*
gkg::Cell::getAtom( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= ( int32_t )_atoms.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    return _atoms[ index ];

  }
  GKG_CATCH( "const gkg::Cell::Atom* "
             "gkg::Cell::getAtom( int32_t index ) const" );

}


void gkg::Cell::setAtom( int32_t index, gkg::Atom* other )
{

  try
  {

    if ( ( index < 0 ) || ( index >= ( int32_t )_atoms.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    _atoms[ index ] = other;

  }
  GKG_CATCH( "void gkg::Cell::setAtom( "
             "int32_t index, gkg::Atom* other )" );

}


void gkg::Cell::setAtoms( const std::vector< gkg::Atom* >& atoms )
{

  try
  {

    _atoms = atoms;

  }
  GKG_CATCH( "void gkg::Cell::setAtoms( "
             "const std::vector< gkg::Atom* >& atoms )" );

}


int32_t gkg::Cell::getNotBelongingToBranchAtomCount() const
{

  try
  {
  
    return (  int32_t )_notBelongingToBranchAtomIndices.size();
  
  }
  GKG_CATCH( "int32_t "
             "gkg::Cell::getNotBelongingToBranchAtomCount() const" );

}


const std::set< int32_t >&
gkg::Cell::getNotBelongingToBranchAtomIndices() const
{

  try
  {
  
    return _notBelongingToBranchAtomIndices;
  
  }
  GKG_CATCH( "const std::set< int32_t >& "
             "gkg::Cell::getNotBelongingToBranchAtomIndices() const" );

}


void gkg::Cell::fillAtomBranchLabels( uint32_t* atomBranchLabels,
                                      uint32_t& lastLabel ) const
{

  try
  {

    // first, resetting labels to nul
    uint32_t* labelCurrentPointer = atomBranchLabels;
    uint32_t* labelEndPointer = labelCurrentPointer + _atoms.size();

    while ( labelCurrentPointer != labelEndPointer )
    {

      *labelCurrentPointer = 0U;
      ++ labelCurrentPointer;

    }

    // then, setting atom labels corresponding to branches
    std::vector< std::pair< int32_t, int32_t > >::const_iterator
      b = _branches.begin(),
      be = _branches.end();
    while ( b != be )
    {

      labelCurrentPointer = atomBranchLabels + ( uint64_t )b->first;
      labelEndPointer = atomBranchLabels + ( uint64_t )b->second + 1U;

      while ( labelCurrentPointer != labelEndPointer )
      {

        *labelCurrentPointer = lastLabel;
        ++ labelCurrentPointer;
        

      }

      // increasing the label value
      ++ lastLabel;

      ++ b;

    }

  }
  GKG_CATCH( "void gkg::Cell::fillAtomBranchLabels( "
             "uint32_t* atomBranchLabels, "
             "uint32_t& lastLabel ) const" );


}


void gkg::Cell::fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {

    int32_t sphereAtomCount = 0;
    int32_t ellipsoidAtomCount = 0;

    gkg::Cell::const_iterator
      a = _atoms.begin(),
      ae = _atoms.end();
    while ( a != ae )
    {

      if ( ( *a )->getType() == 1U )
      {

        ++ sphereAtomCount;

      }
      else if ( ( *a )->getType() == 2U )
      {

        ++ ellipsoidAtomCount;

      }
      ++ a;

    }

    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      unitSphereForSphereAtom;
    int32_t vertexCountPerSphereAtom = 60;
    const std::list< gkg::Vector3d< float > >* vertexListForSphereAtoms = 0;
    if ( sphereAtomCount )
    {

      unitSphereForSphereAtom = gkg::MeshFactory::getInstance().getSphere(
                                        gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                                        1.0,
                                        vertexCountPerSphereAtom );
      vertexListForSphereAtoms =
                               &unitSphereForSphereAtom->vertices.getSites( 0 );

    }
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      unitSphereForEllipsoidAtom;
    int32_t vertexCountPerEllipsoidAtom = 1200;
    const std::list< gkg::Vector3d< float > >* vertexListForEllipsoidAtoms = 0;
    if ( ellipsoidAtomCount )
    {

      unitSphereForEllipsoidAtom = gkg::MeshFactory::getInstance().getSphere(
                                        gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                                        1.0,
                                        vertexCountPerEllipsoidAtom );
      vertexListForEllipsoidAtoms =
                            &unitSphereForEllipsoidAtom->vertices.getSites( 0 );

    }


    std::vector< gkg::Vector3d< float > >
      pointCloud( sphereAtomCount * vertexCountPerSphereAtom +
                  ellipsoidAtomCount * vertexCountPerEllipsoidAtom );

    a = _atoms.begin();
    std::vector< gkg::Vector3d< float > >::iterator
      p = pointCloud.begin();
    while ( a != ae )
    {

      if ( ( *a )->getType() == 1U )
      {

        const gkg::SphereAtom* 
        sphereAtom = static_cast< const gkg::SphereAtom* >( *a );

        // collecting its center and radius
        gkg::Vector3d< float > center = sphereAtom->getCenter();
        float radius = sphereAtom->getRadius();

        std::list< gkg::Vector3d< float > >::const_iterator
          v = vertexListForSphereAtoms->begin(),
          ve = vertexListForSphereAtoms->end();
        while ( v != ve )
        {

          p->x = center.x + v->x * radius;
          p->y = center.y + v->y * radius;
          p->z = center.z + v->z * radius;

          ++ p;
          ++ v;

        }


      }
      else if ( ( *a )->getType() == 2U )
      {

        const gkg::EllipsoidAtom* 
        ellipsoidAtom = static_cast< const gkg::EllipsoidAtom* >( *a );
       
        // collecting its center, radii and rotation angles
        
        gkg::Vector3d< float > center;
        gkg::Vector3d< float > radii;
        gkg::Vector3d< float > axis1;
        gkg::Vector3d< float > axis2;
        gkg::Vector3d< float > axis3;
        ellipsoidAtom->getStdParameters( center,
                                         radii.x,
                                         radii.y,
                                         radii.z,
                                         axis1,
                                         axis2,
                                         axis3 );

	std::list< gkg::Vector3d< float > >::const_iterator
          v = vertexListForEllipsoidAtoms->begin(),
          ve = vertexListForEllipsoidAtoms->end();
	while ( v != ve )
        {

          p->x = center.x + 
          	v->x * axis1.x * radii.x + 
          	v->y * axis2.x * radii.y + 
          	v->z * axis3.x * radii.z;

          p->y = center.y + 
          	v->x * axis1.y * radii.x +
          	v->y * axis2.y * radii.y +
          	v->z * axis3.y * radii.z;

          p->z = center.z +
          	v->x * axis1.z * radii.x +
          	v->y * axis2.z * radii.y +
          	v->z * axis3.z * radii.z;

          ++ p;
          ++ v;

        }

      }

      ++ a;

    }

    gkg::MeshMap< int32_t, float, 3U > meshMap;
    bool removingOutliers = false;
    int32_t neighborCount = 12;
    float outlierDistanceScale = 1.0;
    bool simplifyingPointSet = true;
    bool smoothingPointSet = false;
    bool subdividingMesh = false;
    int32_t subdivisionIterationCount = 2;

    gkg::PointCloudAlphaShapeMesher pointCloudAlphaShapeMesher;
    pointCloudAlphaShapeMesher.computeAlphaShapeMesh( pointCloud,
                                                      meshMap,
                                                      0,
                                                      removingOutliers,
                                                      neighborCount,
                                                      outlierDistanceScale,
                                                      simplifyingPointSet,
                                                      smoothingPointSet,
                                                      subdividingMesh,
                                                      subdivisionIterationCount,
                                                      "",
                                                      false );

    meshAccumulators[ 0 ].add( meshMap );

  }
  GKG_CATCH( "void gkg::Cell::fillMeshAccumulators( "
             "std::vector< MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Cell::addInteractionWithOtherPopulations()
{

  // nothing to do by default

}


void gkg::Cell::regularizeWithFurtherAtoms()
{

  try
  {

    float oversamplingRatio = _parent->getBranchAtomOversamplingRatio();
    int32_t atomCount = ( int32_t )_atoms.size();
    int32_t branchCount = _branches.size();

    if ( branchCount == 0 )
    {

      return;

    }

    std::vector< std::list< gkg::Atom* > > newAtomsForBranches( branchCount );

    int32_t controlPointIndex = 0;
    int32_t controlPointCount = 0;
    int32_t totalAtomCountInNewBranches = 0;

    // looping over branches
    std::vector< std::pair< int32_t,int32_t > >::const_iterator
      b = _branches.begin(),
      be = _branches.end();
    std::vector< std::list< gkg::Atom* > >::iterator
      nal = newAtomsForBranches.begin();
    while ( b != be )
    {

      const int32_t& startingIndex = b->first;
      const int32_t& endingIndex = b->second;
      controlPointCount = endingIndex - startingIndex + 1;

      // looping over pairs of consecutive control points
      for ( controlPointIndex = 0;
            controlPointIndex < controlPointCount - 1;
            controlPointIndex++ )
      {

        const gkg::Atom* atom1 = _atoms[ startingIndex + controlPointIndex ];
        const gkg::Atom* atom2 = _atoms[ startingIndex + 
                                         controlPointIndex + 1 ];

        // adding the first atom to the new atom list
        nal->push_back( atom1->clone() );

        // interpolating and creating new atoms between atom1 and atom2
        gkg::AtomInterpolatorFactory::AtomInterpolator
          atomInterpolator = 
                gkg::AtomInterpolatorFactory::getInstance().getAtomInterpolator(
                                                             atom1->getType(),
                                                             atom2->getType() );
        ( *atomInterpolator )( *atom1, *atom2, oversamplingRatio, *nal );

        // adding the second atom to the new atom list
        nal->push_back( atom2->clone() );


      }

      // updating the total number of atoms belonging to the new branches
      totalAtomCountInNewBranches += ( int32_t )nal->size();

      ++ nal;
      ++ b;

    }
    
    // now, replacing the existing branches by the new branches
    int32_t atomCountNotBelongingToBranches =
            ( int32_t )_notBelongingToBranchAtomIndices.size();
    std::vector< gkg::Atom* > newAtoms( atomCountNotBelongingToBranches +
                                        totalAtomCountInNewBranches );
    int32_t atomIndex = 0;
    int32_t newAtomIndex = 0;
    int32_t branchIndex = 0;
        
    for ( atomIndex = 0; atomIndex < atomCount; atomIndex++ )
    {
    
      // if the current sphere atom does not belong to a branch
      if ( _notBelongingToBranchAtomIndices.find( atomIndex ) !=
           _notBelongingToBranchAtomIndices.end() )
      {
      
        newAtoms[ newAtomIndex ] = _atoms[ atomIndex ];
        ++ newAtomIndex;
      
      }
      else
      {
      
        std::list< gkg::Atom* >::const_iterator
          na = newAtomsForBranches[ branchIndex ].begin(),
          nae = newAtomsForBranches[ branchIndex ].end();
        while ( na != nae )
        {
      
          newAtoms[ newAtomIndex ] = *na;
          
          ++ newAtomIndex;
          ++ na;
      
        }

        atomIndex = _branches[ branchIndex ].second;
        ++ branchIndex;
        
      }
    
    }
    
    _atoms = newAtoms;

  }
  GKG_CATCH( "void gkg::Cell::regularizeWithFurtherAtoms()" );

}

void gkg::Cell::addDetails()
{

  // nothing to do by default

}
