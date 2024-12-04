#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-core-exception/Exception.h>


gkg::Population::Population( int32_t id, const gkg::VirtualTissue* parent )
                : _id( id ),
                  _typeName( "unknown" ),
                  _parent( parent ),
                  _branchAtomOversamplingRatio( 20.0f )
{
}


gkg::Population::Population(
                       int32_t id,
                       const std::string& typeName,
                       const VirtualTissue* parent,
                       const std::vector< gkg::RCPointer< gkg::Cell > >& cells )
                : _id( id ),
                  _typeName( typeName ),
                  _parent( parent ),
                  _branchAtomOversamplingRatio( 20.0f ),
                  _cells( cells )
{

  try
  {

    std::vector< gkg::RCPointer< gkg::Cell > >::iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->setParent( this );
      ++ c;

    }

  }
  GKG_CATCH( "gkg::Population::Population( "
             "int32_t id, "
             "const std::string& typeName, "
             "const VirtualTissue* parent, "
             "const std::vector< gkg::RCPointer< gkg::Cell > >& cells )" );

}


gkg::Population::Population( const gkg::Population& other )
                : _id( other._id ),
                  _typeName( other._typeName ),
                  _parent( other._parent ),
                  _branchAtomOversamplingRatio(
                                           other._branchAtomOversamplingRatio ),
                  _cells( other._cells )
{

  try
  {

    std::vector< gkg::RCPointer< gkg::Cell > >::iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->setParent( this );
      ++ c;

    }

  }
  GKG_CATCH( "const gkg::Population& other" );

}


gkg::Population::~Population()
{

}


gkg::Population& 
gkg::Population::operator=( const gkg::Population& other )
{

  try
  {

    _id = other._id;
    _typeName = other._typeName;
    _parent = other._parent;
    _branchAtomOversamplingRatio = other._branchAtomOversamplingRatio;
    _cells = other._cells;

    std::vector< gkg::RCPointer< gkg::Cell > >::iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->setParent( this );
      ++ c;

    }

    return *this;

  }
  GKG_CATCH( "gkg::Population& "
             "gkg::Population::operator=( const gkg::Population& other ))" );

}


gkg::Population::const_iterator gkg::Population::begin() const
{

  try
  {

    return _cells.begin();

  }
  GKG_CATCH( "inline gkg::Population::const_iterator "
             "gkg::Population::begin() const" );

}


gkg::Population::const_iterator gkg::Population::end() const
{

  try
  {

    return _cells.end();

  }
  GKG_CATCH( "inline gkg::Population::const_iterator "
             "gkg::Population::end() const" );

}


gkg::Population::iterator gkg::Population::begin()
{

  try
  {

    return _cells.begin();

  }
  GKG_CATCH( "inline gkg::Population::iterator gkg::Population::begin()" );

}


gkg::Population::iterator gkg::Population::end()
{

  try
  {

    return _cells.end();

  }
  GKG_CATCH( "inline gkg::Population::iterator gkg::Population::end()" );

}



int32_t gkg::Population::getId() const
{

  try
  {

    return _id;

  }
  GKG_CATCH( "int32_t gkg::Population::getId() const" );

}


std::string gkg::Population::getTypeName() const
{

  try
  {

    return _typeName;

  }
  GKG_CATCH( "std::string gkg::Population::getTypeName() const" );

}


void gkg::Population::setParent( const gkg::VirtualTissue* parent )
{

  try
  {

    _parent = parent;

  }
  GKG_CATCH( "void gkg::Population::setParent( "
             "const gkg::VirtualTissue* parent )" );

}


const gkg::VirtualTissue* gkg::Population::getParent() const
{

  try
  {

    return _parent;

  }
  GKG_CATCH( "const gkg::VirtualTissue* gkg::Population::getParent() const" );

}


float gkg::Population::getBranchAtomOversamplingRatio() const
{

  try
  {

    return _branchAtomOversamplingRatio;


  }
  GKG_CATCH( "float gkg::Population::getBranchAtomOversamplingRatio() const" );

}


void gkg::Population::setBranchAtomOversamplingRatio(
                                             float branchAtomOversamplingRatio )
{

  try
  {


    if ( branchAtomOversamplingRatio <= 0.0f )
    {

      throw std::runtime_error(
                   "branch atom oversampling ratio must be strictly positive" );

    }
    _branchAtomOversamplingRatio = branchAtomOversamplingRatio;


  }
  GKG_CATCH( "void gkg::Population::setBranchAtomOversamplingRatio( "
             "float branchAtomOversamplingRatio )" );

}


int32_t gkg::Population::getCellCount() const
{

  try
  {

    return ( int32_t )_cells.size();


  }
  GKG_CATCH( "int32_t gkg::Population::getCellCount() const" );

}


const std::vector< gkg::RCPointer< gkg::Cell > >& 
gkg::Population::getCells() const
{

  try
  {

    return _cells;

  }
  GKG_CATCH( "const std::vector< gkg::RCPointer< gkg::Cell > >& "
             "gkg::Population::getCells() const" );

}


const gkg::RCPointer< gkg::Cell >& 
gkg::Population::getCell( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= ( int32_t )_cells.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    return _cells[ index ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::Cell >& "
             "gkg::Cell::getCell( int32_t index ) const" );

}


void gkg::Population::setCell( int32_t index,
                               const gkg::RCPointer< gkg::Cell >& other )
{

  try
  {

    if ( ( index < 0 ) || ( index >= ( int32_t )_cells.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    _cells[ index ] = other;
    _cells[ index ]->setParent( this );

  }
  GKG_CATCH( "void gkg::Cell::setCell( "
             "int32_t index, const gkg::RCPointer< gkg::Cell >& other )" );

}


int32_t gkg::Population::getAtomCount() const
{

  try
  {

    int32_t atomCount = 0;
    gkg::Population::const_iterator
      c = this->begin(),
      ce = this->end();
    while ( c != ce )
    {

      atomCount += ( *c )->getAtomCount();
      ++ c;

    }

    return atomCount;

  }
  GKG_CATCH( "int32_t gkg::Population::getAtomCount() const" );

}


uint64_t gkg::Population::getAtomParameterCount() const
{

  try
  {

    uint64_t atomParameterCount = 0U;
    gkg::Population::const_iterator
      c = this->begin(),
      ce = this->end();
    while ( c != ce )
    {

      atomParameterCount += ( *c )->getAtomParameterCount();
      ++ c;

    }

    return atomParameterCount;

  }
  GKG_CATCH( "uint64_t gkg::Population::getAtomParameterCount() const" );

}


void gkg::Population::fillMeshAccumulators( 
                      std::vector< gkg::MeshAccumulator< int32_t, float, 3U > >&
                                                        meshAccumulators ) const
{

  try
  {


    gkg::Population::const_iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->fillMeshAccumulators( meshAccumulators );
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::Population::fillMeshAccumulators( "
             "std::vector< gkg::MeshAccumulator< int32_t, float, 3U > >& "
             "meshAccumulators ) const" );

}


void gkg::Population::addInteractionWithOtherPopulations()
{

  // nothing to do by default

}


void gkg::Population::regularizeWithFurtherAtoms()
{

  try
  {

    gkg::Population::iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->regularizeWithFurtherAtoms();
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::Population::regularizeWithFurtherAtoms()" );

}


void gkg::Population::addDetails()
{

  try
  {

    gkg::Population::iterator
      c = _cells.begin(),
      ce = _cells.end();
    while ( c != ce )
    {

      ( *c )->addDetails();
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::Population::addDetails()" );

}
