#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-algobase/Math.h>


#define MAXIMUM_SPIN_GLASS_COUNT 100000000U
#define MAXIMUM_SPIN_GLASS_CONNECTION_COUNT 100000000U


gkg::SpinGlassAndConnectionMap::SpinGlassAndConnectionMap()
                               : gkg::HeaderedObject()
{

  try
  {

    _spinGlasses.resize( MAXIMUM_SPIN_GLASS_COUNT );
    _spinGlassConnections.resize( MAXIMUM_SPIN_GLASS_CONNECTION_COUNT );

  }
  GKG_CATCH( "gkg::SpinGlassAndConnectionMap::SpinGlassAndConnectionMap()" );

}


gkg::SpinGlassAndConnectionMap::SpinGlassAndConnectionMap(
                                   const gkg::SpinGlassAndConnectionMap& other )
                               : gkg::HeaderedObject( other ),
                                 _spinGlasses( other._spinGlasses ),
                                 _spinGlassConnections(
                                                  other._spinGlassConnections ),
                                 _boundingBox( other._boundingBox ),
                                 _roiResolution( other._roiResolution ),
                                 _spinGlassIdentity( other._spinGlassIdentity )
{
}


gkg::SpinGlassAndConnectionMap::~SpinGlassAndConnectionMap()
{
}


gkg::SpinGlassAndConnectionMap&
gkg::SpinGlassAndConnectionMap::operator=(
                                   const gkg::SpinGlassAndConnectionMap& other )
{

  try
  {

    //this->gkg::HeaderedObject::operator=( other );
    _spinGlasses = other._spinGlasses;
    _spinGlassConnections = other._spinGlassConnections;
    _boundingBox = other._boundingBox;
    _roiResolution = other._roiResolution;
    _spinGlassIdentity = other._spinGlassIdentity;

    return *this;

  }
  GKG_CATCH( "gkg::SpinGlassAndConnectionMap& operator=( "
             "const gkg::SpinGlassAndConnectionMap& other )" );

}


void gkg::SpinGlassAndConnectionMap::reset()
{

  try
  {

    _spinGlasses.clear();
    _spinGlassConnections.clear();

    _spinGlasses.resize( MAXIMUM_SPIN_GLASS_COUNT );
    _spinGlassConnections.resize( MAXIMUM_SPIN_GLASS_CONNECTION_COUNT );

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::reset()" );

}


void gkg::SpinGlassAndConnectionMap::setBoundingBox(
                                  const gkg::Vector3d< int32_t >& lowerCorner,
                                  const gkg::Vector3d< int32_t >& upperCorner,
                                  const gkg::Vector3d< double >& roiResolution )
{

  try
  {

    // upating bounding box
    _boundingBox.setLowerX( lowerCorner.x * roiResolution.x );
    _boundingBox.setUpperX( upperCorner.x * roiResolution.x );

    _boundingBox.setLowerY( lowerCorner.y * roiResolution.y );
    _boundingBox.setUpperY( upperCorner.y * roiResolution.y );

    _boundingBox.setLowerZ( lowerCorner.z * roiResolution.z );
    _boundingBox.setUpperZ( upperCorner.z * roiResolution.z );

    // storing the ROI resolution needed for the futur voxel information
    _roiResolution = roiResolution;

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::setBoundingBox( "
             "const gkg::Vector3d< int32_t >& lowerCorner, "
             "const gkg::Vector3d< int32_t >& upperCorner, "
             "const gkg::Vector3d< double >& roiResolution )" );

}


void gkg::SpinGlassAndConnectionMap::setRoiResolution(
                                   const gkg::Vector3d< double >& roiResolution)
{

  try
  {

    _roiResolution = roiResolution;

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::setRoiResolution( "
             "const gkg::Vector3d< double >& roiResolution)" );


}


void gkg::SpinGlassAndConnectionMap::setCurrentSpinGlass(
                                                    uint64_t spinGlassIdentity )
{

  try
  {

    _spinGlassIdentity = spinGlassIdentity;

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::setCurrentSpinGlass( "
             "uint64_t spinGlassIdentity )" );

}


void gkg::SpinGlassAndConnectionMap::addSpinGlass(
                             uint64_t spinGlassIdentity,
                             const gkg::Vector3d< float >& spinGlassPosition,
                             const gkg::Vector3d< float >& spinGlassOrientation,
                             float spinGlassLength )
{

  try
  {

    if ( spinGlassIdentity >= MAXIMUM_SPIN_GLASS_COUNT )
    {

      throw std::runtime_error( 
      "spin glass identity greater than the maximum allowed spin glass count" );

    }

    if ( !_spinGlasses[ spinGlassIdentity ].isNull() )
    {

      throw std::runtime_error( 
         "spin glass with same identity already present in spin glass vector" );

    }

    // creating the spin glass
    gkg::SpinGlass* spinGlass = new gkg::SpinGlass( spinGlassIdentity,
                                                    spinGlassPosition,
                                                    spinGlassOrientation,
                                                    spinGlassLength );

    // collecting forward and backward extremities
    gkg::Vector3d< float > forwardExtremity = spinGlass->getExtremity(
                                                      gkg::SpinGlass::Forward );
    gkg::Vector3d< float > backwardExtremity = spinGlass->getExtremity(
                                                     gkg::SpinGlass::Backward );

    // tuning the bounding box along X axis from the forward extremity
    if ( forwardExtremity.x < _boundingBox.getLowerX() )
    {

      _boundingBox.setLowerX( forwardExtremity.x );

    }
    else if ( forwardExtremity.x > _boundingBox.getUpperX() )
    {

      _boundingBox.setUpperX( forwardExtremity.x );

    }

    // tuning the bounding box along Y axis from the forward extremity
    if ( forwardExtremity.y < _boundingBox.getLowerY() )
    {

      _boundingBox.setLowerY( forwardExtremity.y );

    }
    else if ( forwardExtremity.y > _boundingBox.getUpperY() )
    {

      _boundingBox.setUpperY( forwardExtremity.y );

    }

    // tuning the bounding box along Z axis from the forward extremity
    if ( forwardExtremity.z < _boundingBox.getLowerZ() )
    {

      _boundingBox.setLowerZ( forwardExtremity.z );

    }
    else if ( forwardExtremity.z > _boundingBox.getUpperZ() )
    {

      _boundingBox.setUpperZ( forwardExtremity.z );

    }

    // tuning the bounding box along X axis from the backward extremity
    if ( backwardExtremity.x < _boundingBox.getLowerX() )
    {

      _boundingBox.setLowerX( backwardExtremity.x );

    }
    else if ( backwardExtremity.x > _boundingBox.getUpperX() )
    {

      _boundingBox.setUpperX( backwardExtremity.x );

    }

    // tuning the bounding box along Y axis from the backward extremity
    if ( backwardExtremity.y < _boundingBox.getLowerY() )
    {

      _boundingBox.setLowerY( backwardExtremity.y );

    }
    else if ( backwardExtremity.y > _boundingBox.getUpperY() )
    {

      _boundingBox.setUpperY( backwardExtremity.y );

    }

    // tuning the bounding box along Z axis from the backward extremity
    if ( backwardExtremity.z < _boundingBox.getLowerZ() )
    {

      _boundingBox.setLowerZ( backwardExtremity.z );

    }
    else if ( backwardExtremity.z > _boundingBox.getUpperZ() )
    {

      _boundingBox.setUpperZ( backwardExtremity.z );

    }

    // resetting the reference counted pointer
    _spinGlasses[ spinGlassIdentity ].reset( spinGlass );

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::addSpinGlass( "
             "uint64_t spinGlassIdentity, "
             "const gkg::Vector3d< float >& spinGlassPosition, "
             "const gkg::Vector3d< float >& spinGlassOrientation, "
             "float spinGlassLength )" );

}


void gkg::SpinGlassAndConnectionMap::removeSpinGlass(
                                                    uint64_t spinGlassIdentity )
{

  try
  {

    if ( spinGlassIdentity >= MAXIMUM_SPIN_GLASS_COUNT )
    {

      throw std::runtime_error( 
      "spin glass identity greater than the maximum allowed spin glass count" );

    }

    if ( _spinGlasses[ spinGlassIdentity ].isNull() )
    {

      throw std::runtime_error( "no spin glass with the given identity "
                                "present in the spin glass vector" );

    }

    _spinGlasses[ spinGlassIdentity ].reset();

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::removeSpinGlass( "
             "uint64_t spinGlassIdentity )" );

}


void gkg::SpinGlassAndConnectionMap::moveSpinGlass(
                         uint64_t spinGlassIdentity,
                         const gkg::Vector3d< float >& newSpinGlassPosition,
                         const gkg::Vector3d< float >& newSpinGlassOrientation )
{

  try
  {

    if ( spinGlassIdentity >= MAXIMUM_SPIN_GLASS_COUNT )
    {

      throw std::runtime_error( 
      "spin glass identity greater than the maximum allowed spin glass count" );

    }


    if ( _spinGlasses[ spinGlassIdentity ].isNull() )
    {

      throw std::runtime_error( "no spin glass with the given identity "
                                "present in the spin glass vector" );

    }

    gkg::SpinGlass& spinGlass = *_spinGlasses[ spinGlassIdentity ];
    spinGlass.setPosition( newSpinGlassPosition );
    spinGlass.setOrientation( newSpinGlassOrientation );

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::moveSpinGlass( "
             "uint64_t spinGlassIdentity, "
             "const gkg::Vector3d< float >& newSpinGlassPosition, "
             "const gkg::Vector3d< float >& newSpinGlassOrientation )" );

}


void gkg::SpinGlassAndConnectionMap::addSpinGlassConnection(
                         uint64_t spinGlassConnectionIdentity,
                         uint64_t spinGlass1Identity,
                         gkg::SpinGlass::ExtremityType spinGlassExtremityType1,
                         uint64_t spinGlass2Identity,
                         gkg::SpinGlass::ExtremityType spinGlassExtremityType2 )
{

  try
  {

    if ( spinGlassConnectionIdentity >= MAXIMUM_SPIN_GLASS_CONNECTION_COUNT )
    {

      throw std::runtime_error( 
              "spin glass connection identity greater than the maximum allowed "
              "spin glass connection count" );

    }

    if ( !_spinGlassConnections[ spinGlassConnectionIdentity ].isNull() )
    {

      throw std::runtime_error( 
                  "spin glass connection with same identity already present in "
                  "the spin glass connection vector" );

    }

    if ( spinGlass1Identity >= MAXIMUM_SPIN_GLASS_COUNT )
    {

      throw std::runtime_error( 
                       "spin glass 1 identity greater than the maximum allowed "
                       "spin glass count" );

    }

    if ( _spinGlasses[ spinGlass1Identity ].isNull() )
    {

      throw std::runtime_error( 
                       "spin glass 1 is not present in the spin glass vector" );

    }

    if ( spinGlass2Identity >= MAXIMUM_SPIN_GLASS_COUNT )
    {

      throw std::runtime_error( 
                       "spin glass 2 identity greater than the maximum allowed "
                       "spin glass count" );

    }

    if ( _spinGlasses[ spinGlass2Identity ].isNull() )
    {

      throw std::runtime_error( 
                       "spin glass 2 is not present in the spin glass vector" );

    }

    _spinGlassConnections[ spinGlassConnectionIdentity ].reset( 
                   new gkg::SpinGlassConnection(
                             spinGlassConnectionIdentity,
                             std::make_pair( _spinGlasses[ spinGlass1Identity ],
                                             spinGlassExtremityType1 ),
                             std::make_pair( _spinGlasses[ spinGlass2Identity ],
                                             spinGlassExtremityType2 ) ) );

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::addSpinGlassConnection( "
             "uint64_t spinGlassConnectionIdentity, "
             "uint64_t spinGlass1Identity, "
             "int32_t spinGlassExtremityType1, "
             "uint64_t spinGlass2Identity, "
             "int32_t spinGlassExtremityType2  )" );

}


void gkg::SpinGlassAndConnectionMap::removeSpinGlassConnection(
                                          uint64_t spinGlassConnectionIdentity )
{

  try
  {

    if ( spinGlassConnectionIdentity >= MAXIMUM_SPIN_GLASS_CONNECTION_COUNT )
    {

      throw std::runtime_error( 
              "spin glass connection identity greater than the maximum allowed "
              "spin glass connection count" );

    }

    if ( _spinGlassConnections[ spinGlassConnectionIdentity ].isNull() )
    {

      throw std::runtime_error(
                             "no spin glass connection with the given identity "
                             "present in the spin glass connection vector" );

    }

    _spinGlassConnections[ spinGlassConnectionIdentity ].reset();

  }
  GKG_CATCH( "void gkg::SpinGlassAndConnectionMap::removeSpinGlassConnection( "
             "uint64_t spinGlassConnectionIdentity )" );

}


const std::vector< gkg::RCPointer< gkg::SpinGlass > >& 
gkg::SpinGlassAndConnectionMap::getSpinGlasses() const
{

  try
  {

    return _spinGlasses;

  }
  GKG_CATCH( "const std::vector< gkg::RCPointer< gkg::SpinGlass > >& "
             "gkg::SpinGlassAndConnectionMap::getSpinGlasses() const" );

}


const std::vector< gkg::RCPointer< gkg::SpinGlassConnection > >&
gkg::SpinGlassAndConnectionMap::getSpinGlassConnections() const
{

  try
  {

    return _spinGlassConnections;

  }
  GKG_CATCH( "const std::vector< gkg::RCPointer< gkg::SpinGlassConnection > >& "
             "gkg::SpinGlassAndConnectionMap::getSpinGlassConnections() "
             "const" );

}


const gkg::BoundingBox< float >& 
gkg::SpinGlassAndConnectionMap::getBoundingBox() const
{

  try
  {

    return _boundingBox;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::SpinGlassAndConnectionMap::getBoundingBox() const" );


}


const gkg::Vector3d< double >& 
gkg::SpinGlassAndConnectionMap::getRoiResolution() const
{

  try
  {

    return _roiResolution;

  }
  GKG_CATCH( "const gkg::Vector3d< double >& "
             "gkg::SpinGlassAndConnectionMap::getRoiResolution() const" );


}


const gkg::RCPointer< gkg::SpinGlass >& 
gkg::SpinGlassAndConnectionMap::getCurrentSpinGlass() const
{

  try
  {

    return _spinGlasses[ _spinGlassIdentity ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlass >& "
             "gkg::SpinGlassAndConnectionMap::getCurrentSpinGlass() const" );


}


float gkg::SpinGlassAndConnectionMap::getLowerX() const
{

  try
  {

    return ( _boundingBox.getLowerX() - 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getLowerX() const" );

}


float gkg::SpinGlassAndConnectionMap::getLowerY() const
{

  try
  {

    return ( _boundingBox.getLowerY() - 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getLowerY() const" );

}


float gkg::SpinGlassAndConnectionMap::getLowerZ() const
{

  try
  {

    return ( _boundingBox.getLowerZ() - 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getLowerZ() const" );

}


float gkg::SpinGlassAndConnectionMap::getUpperX() const
{

  try
  {

    return ( _boundingBox.getUpperX() + 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getUpperX() const" );

}


float gkg::SpinGlassAndConnectionMap::getUpperY() const
{

  try
  {

    return ( _boundingBox.getUpperY() + 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getUpperY() const" );

}


float gkg::SpinGlassAndConnectionMap::getUpperZ() const
{

  try
  {

    return ( _boundingBox.getUpperZ() + 0.5 );

  }
  GKG_CATCH( "float gkg::SpinGlassAndConnectionMap::getUpperZ() const" );

}


int32_t gkg::SpinGlassAndConnectionMap::getValidSpinGlassCount() const
{

  try
  {

    int32_t validSpinGlassCount = 0;

    std::vector< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
      s = _spinGlasses.begin(),
      se = _spinGlasses.end();
    while ( s != se )
    {

      if ( !s->isNull() )
      {

        ++ validSpinGlassCount;

      }
      ++ s;

    }
    return validSpinGlassCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::SpinGlassAndConnectionMap::getValidSpinGlassCount() const" );

}


void gkg::SpinGlassAndConnectionMap::getValidSpinGlasses(
              std::list< gkg::RCPointer< gkg::SpinGlass > >& spinGlasses ) const
{

  try
  {

    // emptying the list
    spinGlasses.clear();

    // filling the list with valid spin glasses
    std::vector< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
      s = _spinGlasses.begin(),
      se = _spinGlasses.end();
    while ( s != se )
    {

      if ( !s->isNull() )
      {

        spinGlasses.push_back( *s );

      }
      ++ s;

    }

  }
  GKG_CATCH( "void "
             "gkg::SpinGlassAndConnectionMap::getValidSpinGlasses( "
             "std::list< gkg::RCPointer< gkg::SpinGlass > >& "
             "spinGlasses ) const" );

}


int32_t gkg::SpinGlassAndConnectionMap::getValidSpinGlassConnectionCount() const
{

  try
  {

    int32_t validSpinGlassConnectionCount = 0;

    std::vector< gkg::RCPointer< gkg::SpinGlassConnection > >::const_iterator
      c = _spinGlassConnections.begin(),
      ce = _spinGlassConnections.end();
    while ( c != ce )
    {

      if ( !c->isNull() )
      {

        ++ validSpinGlassConnectionCount;

      }
      ++ c;

    }
    return validSpinGlassConnectionCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::SpinGlassAndConnectionMap::"
             "getValidSpinGlassConnectionCount() const" );

}


void gkg::SpinGlassAndConnectionMap::getValidSpinGlassConnections(
              std::list< gkg::RCPointer< gkg::SpinGlassConnection > >&
                                                    spinGlassConnections ) const
{

  try
  {

    // emptying the list
    spinGlassConnections.clear();

    // filling the list with valid spin glass connections
    std::vector< gkg::RCPointer< gkg::SpinGlassConnection > >::const_iterator
      c = _spinGlassConnections.begin(),
      ce = _spinGlassConnections.end();
    while ( c != ce )
    {

      if ( !c->isNull() )
      {

        spinGlassConnections.push_back( *c );

      }
      ++ c;

    }

  }
  GKG_CATCH( "void "
             "gkg::SpinGlassAndConnectionMap::getValidSpinGlassConnections( "
             "std::list< gkg::RCPointer< gkg::SpinGlassConnection > >& "
             "spinGlassConnections ) const" );

}



#undef MAXIMUM_SPIN_GLASS_COUNT
#undef MAXIMUM_SPIN_GLASS_CONNECTION_COUNT

