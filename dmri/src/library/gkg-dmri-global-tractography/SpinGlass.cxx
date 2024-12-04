#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-dmri-global-tractography/AnatomicalPriors.h>
#include <gkg-dmri-global-tractography/Log.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-exception/Exception.h>


gkg::SpinGlass::SpinGlass( uint64_t identity,
                           const gkg::Vector3d< float >& position,
                           const gkg::Vector3d< float >& orientation,
                           float length )
               : _identity( identity ),
                 _position( position ),
                 _orientation( orientation ),
                 _length( length )
{

  try
  {

    this->updateExtremities();

  }
  GKG_CATCH( "gkg::SpinGlass::SpinGlass( uint64_t identity, "
             "const gkg::Vector3d< float >& position, "
             "const gkg::Vector3d< float >& orientation, float length )" );

}


gkg::SpinGlass::SpinGlass( const gkg::SpinGlass& other )
{

  try
  {

    _identity = other._identity;
    _position = other._position;
    _orientation = other._orientation;
    _length = other._length;
    _backwardExtremity = other._backwardExtremity;
    _forwardExtremity = other._forwardExtremity;
    _connectedSpinGlasses[ gkg::SpinGlass::Forward ] = 
                        other._connectedSpinGlasses[ gkg::SpinGlass::Forward ];
    _connectedSpinGlasses[ gkg::SpinGlass::Backward ] = 
                        other._connectedSpinGlasses[ gkg::SpinGlass::Backward ];
    _spinGlassConnections[ gkg::SpinGlass::Forward ] =
                        other._spinGlassConnections[ gkg::SpinGlass::Forward ];
    _spinGlassConnections[ gkg::SpinGlass::Backward ] =
                        other._spinGlassConnections[ gkg::SpinGlass::Backward ];

  }
  GKG_CATCH( "gkg::SpinGlass::SpinGlass( const gkg::SpinGlass& other )" );

}


gkg::SpinGlass::~SpinGlass()
{
}


gkg::SpinGlass& gkg::SpinGlass::operator=( const gkg::SpinGlass& other )
{

  try
  {

    _identity = other._identity;
    _position = other._position;
    _orientation = other._orientation;
    _length = other._length;
    _backwardExtremity = other._backwardExtremity;
    _forwardExtremity = other._forwardExtremity;
    _connectedSpinGlasses[ gkg::SpinGlass::Forward ] = 
                        other._connectedSpinGlasses[ gkg::SpinGlass::Forward ];
    _connectedSpinGlasses[ gkg::SpinGlass::Backward ] = 
                        other._connectedSpinGlasses[ gkg::SpinGlass::Backward ];
    _spinGlassConnections[ gkg::SpinGlass::Forward ] =
                        other._spinGlassConnections[ gkg::SpinGlass::Forward ];
    _spinGlassConnections[ gkg::SpinGlass::Backward ] =
                        other._spinGlassConnections[ gkg::SpinGlass::Backward ];

    return *this;

  }
  GKG_CATCH( "gkg::Vector3d< float >& gkg::SpinGlass::operator=( "
             "const gkg::SpinGlass& other )" );

}


uint64_t gkg::SpinGlass::getIdentity() const
{

  try
  {

    return _identity;

  }
  GKG_CATCH( "uint64_t gkg::SpinGlass::getIdentity() const" );

}


const gkg::Vector3d< float >& gkg::SpinGlass::getPosition() const
{

  try
  {

    return _position;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::SpinGlass::getPosition() const" );

}


const gkg::Vector3d< float >& gkg::SpinGlass::getOrientation() const
{

  try
  {

    return _orientation;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::SpinGlass::getOrientation() const" );

}


float gkg::SpinGlass::getLength() const
{

  try
  {

    return _length;

  }
  GKG_CATCH( "float gkg::SpinGlass::getLength() const" );

}


const gkg::RCPointer< gkg::SpinGlass >&
gkg::SpinGlass::getConnectedSpinGlass(
                      const gkg::SpinGlass::ExtremityType& extremityType ) const
{

  try
  {

    return _connectedSpinGlasses[ extremityType ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlass >& "
             "gkg::SpinGlass::getConnectedSpinGlass( "
             "const gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


const gkg::RCPointer< gkg::SpinGlassConnection >& 
gkg::SpinGlass::getSpinGlassConnection( 
                      const gkg::SpinGlass::ExtremityType& extremityType ) const
{

  try
  {

    return _spinGlassConnections[ extremityType ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlassConnection >& "
             "gkg::SpinGlass::getSpinGlassConnection( "
             "const gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


const gkg::Vector3d< float >&
gkg::SpinGlass::getExtremity(
                      const gkg::SpinGlass::ExtremityType& extremityType ) const
{

  try
  {

    if ( extremityType == gkg::SpinGlass::Forward )
    {

      return _forwardExtremity;

    }
    return _backwardExtremity;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& gkg::SpinGlass::getExtremity( "
             "const gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


const gkg::Vector3d< float >&
gkg::SpinGlass::getExtremityAndOppositeExtremityType(
                    const gkg::SpinGlass::ExtremityType& extremityType,
                    gkg::SpinGlass::ExtremityType& oppositeExtremityType ) const
{

  try
  {

    if ( extremityType == gkg::SpinGlass::Forward )
    {

      oppositeExtremityType = gkg::SpinGlass::Backward;
      return _forwardExtremity;

    }
    oppositeExtremityType = gkg::SpinGlass::Forward;
    return _backwardExtremity;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& gkg::SpinGlass::getExtremity( "
             "const gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


void gkg::SpinGlass::getOptimalPositionAndOrientation(
                         gkg::Vector3d< float >& newSpinGlassPosition,
                         gkg::Vector3d< float >& newSpinGlassOrientation ) const
{

  try
  {

    // computing the current spin glass half length
    float currentSpinGlassHalfLength = _length / 2.0;

    gkg::SpinGlass::ExtremityType forwardSpinGlassExtremityType;
    gkg::SpinGlass::ExtremityType backwardSpinGlassExtremityType;

    // if the spin glass is fully connected
    if ( !_connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull() &&
         !_connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull() )
    {

      // storing the forward spin glass position, orientation and half length
      const gkg::Vector3d< float >& forwardSpinGlassPosition = 
          _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getPosition();
      const gkg::Vector3d< float >& forwardSpinGlassOrientation =
          _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getOrientation();
      float forwardSpinGlassHalfLength = 
          _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getLength() / 2.0;

      // storing the backward spin glass position and orientation
      const gkg::Vector3d< float >& backwardSpinGlassPosition = 
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getPosition();
      const gkg::Vector3d< float >& backwardSpinGlassOrientation =
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getOrientation();
      float backwardSpinGlassHalfLength = 
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getLength() / 2.0;

      // computing the delta position between the forward and backward spin
      // glasses
      gkg::Vector3d< float > deltaPosition(
                     forwardSpinGlassPosition.x - backwardSpinGlassPosition.x,
                     forwardSpinGlassPosition.y - backwardSpinGlassPosition.y,
                     forwardSpinGlassPosition.z - backwardSpinGlassPosition.z );

      // finding the extremity type of the forward spin glass
      if ( _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->
                                getConnectedSpinGlass( gkg::SpinGlass::Forward )
           == this )
      {

        forwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

      }
      else if ( _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Backward )
                == this )
      {

        forwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

      }
      else
      {

        throw std::runtime_error( "invalid forward spin glass connection" );

      }

      // finding the extremity type of the backward spin glass
      if ( _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Forward )
           == this )
      {

        backwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

      }
      else if ( _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Backward )
                == this )
      {

        backwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

      }
      else
      {

        throw std::runtime_error( "invalid backward spin glass connection" );

      }

      // computing sign to avoid reversal of the spin glass
      float sign1 = ( ( forwardSpinGlassExtremityType == 
                        gkg::SpinGlass::Forward ) ? +1.0 : -1.0 );
      float sign2 = ( ( backwardSpinGlassExtremityType == 
                        gkg::SpinGlass::Forward ) ? +1.0 : -1.0 );

      // computing the optimal position of the current spin glass

      float a = sign1 * ( forwardSpinGlassHalfLength +
                          currentSpinGlassHalfLength );
      float b = sign2 * ( backwardSpinGlassHalfLength +
                          currentSpinGlassHalfLength );

      newSpinGlassPosition.x = ( forwardSpinGlassPosition.x +
                                 a * forwardSpinGlassOrientation.x +
                                 backwardSpinGlassPosition.x +
                                 b * backwardSpinGlassOrientation.x ) /
                               2.0;
      newSpinGlassPosition.y = ( forwardSpinGlassPosition.y +
                                 a * forwardSpinGlassOrientation.y +
                                 backwardSpinGlassPosition.y +
                                 b * backwardSpinGlassOrientation.y ) /
                               2.0;
      newSpinGlassPosition.z = ( forwardSpinGlassPosition.z +
                                 a * forwardSpinGlassOrientation.z +
                                 backwardSpinGlassPosition.z +
                                 b * backwardSpinGlassOrientation.z ) /
                               2.0;

      // computing the optimal orientation of the current spin glass
      if ( deltaPosition.getNorm() != 0.0f )
      {

        deltaPosition.normalize();
        newSpinGlassOrientation.x = deltaPosition.x;
        newSpinGlassOrientation.y = deltaPosition.y;
        newSpinGlassOrientation.z = deltaPosition.z;

      }
      else
      {

        newSpinGlassOrientation.x = _orientation.x;
        newSpinGlassOrientation.y = _orientation.y;
        newSpinGlassOrientation.z = _orientation.z;

      }

    }
    else
    {

      // if the spin glass is only attached at the forward extremity
      if ( !_connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull() )
      {

        // storing the forward spin glass position, orientation and half length
        const gkg::Vector3d< float >& forwardSpinGlassPosition =
             _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getPosition();
        const gkg::Vector3d< float >& forwardSpinGlassOrientation =
             _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getOrientation();
        float forwardSpinGlassHalfLength =
            _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getLength() / 2.0;

        // finding the extremity type of the forward spin glass
        if ( _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Forward ) 
             == this )
        {

          forwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

        }
        else if ( _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->
                              getConnectedSpinGlass(  gkg::SpinGlass::Backward )
                  == this )
        {

          forwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

        }
        else
        {

          throw std::runtime_error( "invalid forward spin glass connection" );

        }

        // computing sign to avoid reversal of the spin glass
        float sign = ( ( forwardSpinGlassExtremityType == 
                         gkg::SpinGlass::Forward ) ? +1.0 : -1.0 );

        float a = sign * ( forwardSpinGlassHalfLength +
                           currentSpinGlassHalfLength );

        // computing the optimal position of the current spin glass
        newSpinGlassPosition.x = forwardSpinGlassPosition.x + 
                                 a * forwardSpinGlassOrientation.x;
        newSpinGlassPosition.y = forwardSpinGlassPosition.y +
                                 a * forwardSpinGlassOrientation.y;
        newSpinGlassPosition.z = forwardSpinGlassPosition.z +
                                 a * forwardSpinGlassOrientation.z;

        // computing the optimal orientation of the current spin glass
        if ( forwardSpinGlassOrientation.dot( _orientation ) >= 0 )
        {

          newSpinGlassOrientation.x = forwardSpinGlassOrientation.x;
          newSpinGlassOrientation.y = forwardSpinGlassOrientation.y;
          newSpinGlassOrientation.z = forwardSpinGlassOrientation.z;

        }
        else
        {

          newSpinGlassOrientation.x = -forwardSpinGlassOrientation.x;
          newSpinGlassOrientation.y = -forwardSpinGlassOrientation.y;
          newSpinGlassOrientation.z = -forwardSpinGlassOrientation.z;

        }

      }
      // if the spin glass is only attached at the backward extremity
      else if ( !_connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull() )
      {

        // storing the backward spin glass position and orientation
        const gkg::Vector3d< float >& backwardSpinGlassPosition =
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getPosition();
        const gkg::Vector3d< float >& backwardSpinGlassOrientation =
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getOrientation();
        float backwardSpinGlassHalfLength =
           _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getLength() / 2.0;

        // finding the extremity type of the backward spin glass
        if ( _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Forward )
             == this )
        {

          backwardSpinGlassExtremityType = gkg::SpinGlass::Forward;

        }
        else if ( _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->
                               getConnectedSpinGlass( gkg::SpinGlass::Backward )
                  == this )
        {

          backwardSpinGlassExtremityType = gkg::SpinGlass::Backward;

        }
        else
        {

          throw std::runtime_error( "invalid backward spin glass connection" );

        }

        // computing sign to avoid reversal of the spin glass
        float sign = ( ( backwardSpinGlassExtremityType == 
                         gkg::SpinGlass::Forward ) ? +1.0 : -1.0 );

        float a = sign * ( backwardSpinGlassHalfLength +
                           currentSpinGlassHalfLength );

        // computing the optimal position of the current spin glass
        newSpinGlassPosition.x = backwardSpinGlassPosition.x + 
                                 a * backwardSpinGlassOrientation.x;
        newSpinGlassPosition.y = backwardSpinGlassPosition.y + 
                                 a * backwardSpinGlassOrientation.y;
        newSpinGlassPosition.z = backwardSpinGlassPosition.z + 
                                 a * backwardSpinGlassOrientation.z;

        // computing the optimal orientation of the current spin glass
        if ( backwardSpinGlassOrientation.dot( _orientation ) >= 0 )
        {

          newSpinGlassOrientation = backwardSpinGlassOrientation;

        }
        else
        {

          newSpinGlassOrientation = -backwardSpinGlassOrientation;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::SpinGlass::getOptimalPositionAndOrientation( "
             "gkg::Vector3d< float >& newSpinGlassPosition, "
             "gkg::Vector3d< float >& newSpinGlassOrientation ) const" );

}


void gkg::SpinGlass::setPosition( const gkg::Vector3d< float >& position )
{

  try
  {     

    _position = position;
    this->updateExtremities();

  }
  GKG_CATCH( "void gkg::SpinGlass::setPosition( "
             "const gkg::Vector3d< float >& position )" );

}


void gkg::SpinGlass::setOrientation( const gkg::Vector3d< float >& orientation )
{

  try
  {     
 
    _orientation = orientation;
    this->updateExtremities();

  }
  GKG_CATCH( "void gkg::SpinGlass::setOrientation( "
             "const gkg::Vector3d< float >& orientation )" );

}


void gkg::SpinGlass::setLength( float length )
{

  try
  {

    _length = length;
    this->updateExtremities();

  }
  GKG_CATCH( "void setLength( float length )" );

}


void gkg::SpinGlass::setConnectedSpinGlass( 
                    const gkg::SpinGlass::ExtremityType& extremityType,
                    const gkg::RCPointer< gkg::SpinGlass >& connectedSpinGlass )
{

  try
  {

    _connectedSpinGlasses[ extremityType ] = connectedSpinGlass;

  }
  GKG_CATCH( "void gkg::SpinGlass::setConnectedSpinGlass( "
             "const gkg::SpinGlass::ExtremityType& extremityType,"
             "const gkg::RCPointer< gkg::SpinGlass >& connectedSpinGlass ) )" );

}


void gkg::SpinGlass::setSpinGlassConnection( 
         const gkg::SpinGlass::ExtremityType& extremityType,
         const gkg::RCPointer< gkg::SpinGlassConnection >& spinGlassConnection )
{

  try
  {

    _spinGlassConnections[ extremityType ] = spinGlassConnection;

  }
  GKG_CATCH( "void gkg::SpinGlass::setSpinGlassConnection( "
             "const gkg::SpinGlass::ExtremityType& extremityType, "
             "const gkg::RCPointer< gkg::SpinGlassConnection >& "
             "spinGlassConnection )" );

}


void gkg::SpinGlass::resetConnectedSpinGlass(
                            const gkg::SpinGlass::ExtremityType& extremityType )
{

  try
  {     
 
    _connectedSpinGlasses[ extremityType ].reset();

  }
  GKG_CATCH( "void gkg::SpinGlass::resetConnectedSpinGlass( "
             "const gkg::SpinGlass::ExtremityType& extremityType )" );

}


void gkg::SpinGlass::resetSpinGlassConnection(
                            const gkg::SpinGlass::ExtremityType& extremityType )
{

  try
  {     

    _spinGlassConnections[ extremityType ].reset();

  }
  GKG_CATCH( "void gkg::SpinGlass::resetSpinGlassConnection( "
             "const gkg::SpinGlass::ExtremityType& extremityType )" );

}


bool gkg::SpinGlass::isIsolated() const
{

  try
  {

    return ( _connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull() &&
             _connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull() );

  }
  GKG_CATCH( "bool gkg::SpinGlass::isIsolated() const" );

}


bool gkg::SpinGlass::isConnected(
                      const gkg::SpinGlass::ExtremityType& extremityType ) const
{

  try
  {

    return !_connectedSpinGlasses[ extremityType ].isNull();

  }
  GKG_CATCH( "bool gkg::SpinGlass::isConnected( "
             "const gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


bool gkg::SpinGlass::isAnExtremity(
                        gkg::SpinGlass::ExtremityType* freeExtremityType ) const
{

  try
  {

    bool forwardExtremityAvailability = 
                     _connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull();
    bool backwardExtremityAvailability = 
                     _connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull();
    if ( forwardExtremityAvailability != backwardExtremityAvailability )
    {

      if ( freeExtremityType )
      {

        if ( forwardExtremityAvailability )
        {

          *freeExtremityType = gkg::SpinGlass::Forward;

        }
        else
        {

          *freeExtremityType = gkg::SpinGlass::Backward;

        }

      }
      return true;

    }

    return false;

  }
  GKG_CATCH( "bool gkg::SpinGlass::isAnExtremity( "
             "gkg::SpinGlass::ExtremityType* freeExtremityType ) const" );

}


void gkg::SpinGlass::checkConsistency() const
{

  try
  {

    //--------------------------------------------------------------------------
    BEGIN_DEBUG

    // sanity check at the forward extremity
    if ( _spinGlassConnections[ gkg::SpinGlass::Forward ].isNull() &&
         _connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull() )
    {

      os << "nothing at the forward extremity" << std::endl;

    }
    else if ( !_spinGlassConnections[ gkg::SpinGlass::Forward ].isNull() &&
              !_connectedSpinGlasses[ gkg::SpinGlass::Forward ].isNull() )
    {

      os << "----------------------------------------------------" << std::endl;
      os << "consistency OK : forward connection ("
         << _spinGlassConnections[ gkg::SpinGlass::Forward ]->getIdentity()
         << ") and forward connected spin glass ("
         << _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getIdentity()
         << ")" << std::endl;
      os << "spin glass position : " << _position << std::endl;
      os << "spin glass orientation : " << _orientation << std::endl;
      os << "spin glass length : " << _length << std::endl;
      os << "connected spin glass position : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getPosition()
         << std::endl;
      os << "connected spin glass orientation : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getOrientation()
         << std::endl;
      os << "connected spin glass length : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Forward ]->getLength()
         << std::endl;

    }
    else
    {

      os << "problem with forward extremity" << std::endl;

    }

    // sanity check at the backward extremity
    if ( _spinGlassConnections[ gkg::SpinGlass::Backward ].isNull() &&
         _connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull() )
    {

      os << "nothing at the backward extremity" << std::endl;

    }
    else if ( !_spinGlassConnections[ gkg::SpinGlass::Backward ].isNull() &&
              !_connectedSpinGlasses[ gkg::SpinGlass::Backward ].isNull() )
    {

      os << "----------------------------------------------------" << std::endl;
      os << "consistency OK : backward connection ("
         << _spinGlassConnections[ gkg::SpinGlass::Backward ]->getIdentity()
         << ") and backward connected spin glass ("
         << _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getIdentity()
         << ")" << std::endl;
      os << "spin glass position : " << _position << std::endl;
      os << "spin glass orientation : " << _orientation << std::endl;
      os << "spin glass length : " << _length << std::endl;
      os << "connected spin glass position : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getPosition()
         << std::endl;
      os << "connected spin glass orientation : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getOrientation()
         << std::endl;
      os << "connected spin glass length : "
         << _connectedSpinGlasses[ gkg::SpinGlass::Backward ]->getLength()
         << std::endl;

    }
    else
    {

      os << "problem with backward extremity" << std::endl;

    }

    END_DEBUG
    //--------------------------------------------------------------------------

  }
  GKG_CATCH( "void gkg::SpinGlass::checkConsistency() const" );

}


float gkg::SpinGlass::getInteractionPotentialWithOther(
               const gkg::SpinGlass::ExtremityType& spinGlassExtremityType,
               float connectionLikelihood,
               const gkg::RCPointer< gkg::SpinGlass >& otherSpinGlass,
               const gkg::SpinGlass::ExtremityType& otherSpinGlassExtremityType,
               float otherConnectionLikelihood,
               float spinGlassDistanceToCortex,
               float otherSpinGlassDistanceToCortex,
               float minimumDistance,
               float maximumDistance ) const
{

  try
  {

    // computing the spin glass position, extremity and its half length
    const gkg::Vector3d< float >&
      spinGlassExtremity = this->getExtremity( spinGlassExtremityType );
    float squaredSpinGlassHalfLength = _length * _length / 4.0f;

    // getting the other spin glass position, extremity and length
    const gkg::Vector3d< float >&
      otherSpinGlassPosition = otherSpinGlass->getPosition();
    const gkg::Vector3d< float >&
      otherSpinGlassExtremity = otherSpinGlass->getExtremity(
                                                  otherSpinGlassExtremityType );
    float squaredOtherSpinGlassHalfLength = ( otherSpinGlass->getLength() /
                                              2.0f );
    squaredOtherSpinGlassHalfLength *= squaredOtherSpinGlassHalfLength;

    // computing the middle point
    gkg::Vector3d< float > 
      middlePoint( ( _position.x + otherSpinGlassPosition.x ) / 2.0f,
                   ( _position.y + otherSpinGlassPosition.y ) / 2.0f,
                   ( _position.z + otherSpinGlassPosition.z ) / 2.0f );

    // computing the interaction potential

    float ax = spinGlassExtremity.x - middlePoint.x;
    float ay = spinGlassExtremity.y - middlePoint.y;
    float az = spinGlassExtremity.z - middlePoint.z;

    float bx = otherSpinGlassExtremity.x - middlePoint.x;
    float by = otherSpinGlassExtremity.y - middlePoint.y;
    float bz = otherSpinGlassExtremity.z - middlePoint.z;

    float interactionPotential = 
      ( ax * ax + ay * ay + az * az ) / squaredSpinGlassHalfLength +
      ( bx * bx + by * by + bz * bz ) / squaredOtherSpinGlassHalfLength -
      ( connectionLikelihood + otherConnectionLikelihood ) / 2.0f;


    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "---------------spin glass orientation = "
       << _orientation << std::endl;
    os << "---------------spin glass position = "
       << _position << std::endl;
    os << "---------------spin glass extremity = "
       << spinGlassExtremity << std::endl;
    os << "---------------spin glass norm = "
       << ( spinGlassExtremity - middlePoint ).getNorm2() << std::endl;
    os << "---------------squaredSpinGlassHalfLength = "
       << squaredSpinGlassHalfLength << std::endl;
    os << "---------------middlePoint : " << middlePoint << std::endl;
    os << "---------------other spin glass orientation = "
       << otherSpinGlass->getOrientation() << std::endl;
    os << "---------------other spin glass position = "
       << otherSpinGlassPosition << std::endl;
    os << "---------------other spin glass extremity = "
       << otherSpinGlassExtremity << std::endl;
    os << "---------------other spin glass norm = "
       << ( otherSpinGlassExtremity - middlePoint ).getNorm2() << std::endl;
    os << "---------------squaredOtherSpinGlassHalfLength = "
       << squaredOtherSpinGlassHalfLength << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------


    float averageDistance = ( spinGlassDistanceToCortex + 
                              otherSpinGlassDistanceToCortex ) / 2.0f;


    return interactionPotential * 
           std::min( 1.0f,
                     std::max( averageDistance / maximumDistance, 
                               minimumDistance / maximumDistance ) );

  }
  GKG_CATCH( "float gkg::SpinGlass::getInteractionPotentialWithOther( "
             "const gkg::SpinGlass::ExtremityType& spinGlassExtremityType, "
             "float& connectionLikelihood, "
             "const gkg::RCPointer< gkg::SpinGlass >& otherSpinGlass, "
             "const gkg::SpinGlass::ExtremityType& "
             "otherSpinGlassExtremityType, "
             "float& otherConnectionLikelihood ) const" );

}


const gkg::RCPointer< gkg::SpinGlass >& 
gkg::SpinGlass::getFollower( 
   const gkg::SpinGlass::ExtremityType& spinGlassExtremityType,
   gkg::SpinGlass::ExtremityType& followerConnectedSpinGlassExtremityType,
   gkg::SpinGlass::ExtremityType& followerOppositeSpinGlassExtremityType ) const
{

  try
  {

    const gkg::RCPointer< gkg::SpinGlass >& 
      followerSpinGlass = _connectedSpinGlasses[ spinGlassExtremityType ];

    if ( !followerSpinGlass.isNull() )
    {

      const RCPointer< SpinGlass >& 
        spinGlassForward = followerSpinGlass->getConnectedSpinGlass(
                                                      gkg::SpinGlass::Forward );
      if ( !spinGlassForward.isNull() &&
           ( spinGlassForward->getIdentity() == _identity ) )
      {

        followerConnectedSpinGlassExtremityType = gkg::SpinGlass::Forward;
        followerOppositeSpinGlassExtremityType = gkg::SpinGlass::Backward;

      }
           
      const RCPointer< SpinGlass >& 
        spinGlassBackward = followerSpinGlass->getConnectedSpinGlass(
                                                     gkg::SpinGlass::Backward );
      if ( !spinGlassBackward.isNull() &&
           ( spinGlassBackward->getIdentity() == _identity ) )
      {

        followerConnectedSpinGlassExtremityType = gkg::SpinGlass::Backward;
        followerOppositeSpinGlassExtremityType = gkg::SpinGlass::Forward;

      }

    }

    return followerSpinGlass;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlass >& "
             "gkg::SpinGlass::getFollower( "
             "const gkg::SpinGlass::ExtremityType& spinGlassExtremityType, "
             "gkg::SpinGlass::ExtremityType& "
             "followerConnectedSpinGlassExtremityType, "
             "gkg::SpinGlass::ExtremityType& "
             "followerOppositeSpinGlassExtremityType ) const" );

}


void gkg::SpinGlass::track( gkg::LightCurve3d< float >& curve3d,
                            int32_t neighborCount ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a list to store the curve 3D points
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< float > > points;
    int32_t neighborIndex = 0;


    ////////////////////////////////////////////////////////////////////////////
    // initializing current spin glass and current extremity type
    ////////////////////////////////////////////////////////////////////////////

    const gkg::SpinGlass* currentSpinGlass = 0;
    gkg::SpinGlass::ExtremityType 
      currenSpinGlassExtremityType = gkg::SpinGlass::Forward;


    ////////////////////////////////////////////////////////////////////////////
    // allocating pointer to follower spin glass and its extremity
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SpinGlass > followerSpinGlass;
    gkg::SpinGlass::ExtremityType 
      followerConnectedSpinGlassExtremityType = gkg::SpinGlass::Forward;
    gkg::SpinGlass::ExtremityType 
      followerOppositeSpinGlassExtremityType = gkg::SpinGlass::Forward;


    ////////////////////////////////////////////////////////////////////////////
    // tracking forward
    ////////////////////////////////////////////////////////////////////////////

    currentSpinGlass = this;
    currenSpinGlassExtremityType = gkg::SpinGlass::Forward;
    points.push_back( currentSpinGlass->getExtremity(
                                               currenSpinGlassExtremityType ) );

    neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        points.push_back( followerSpinGlass->getExtremity(
                                    followerConnectedSpinGlassExtremityType ) );
        points.push_back( followerSpinGlass->getExtremity(
                                    followerOppositeSpinGlassExtremityType ) );

        currentSpinGlass = followerSpinGlass.get();
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // tracking backward
    ////////////////////////////////////////////////////////////////////////////

    currentSpinGlass = this;
    currenSpinGlassExtremityType = gkg::SpinGlass::Backward;
    points.push_front( currentSpinGlass->getExtremity(
                                               currenSpinGlassExtremityType ) );

    neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        points.push_front( followerSpinGlass->getExtremity(
                                    followerConnectedSpinGlassExtremityType ) );
        points.push_front( followerSpinGlass->getExtremity(
                                    followerOppositeSpinGlassExtremityType ) );

        currentSpinGlass = followerSpinGlass.get();
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // creating 3D curve
    ////////////////////////////////////////////////////////////////////////////

    curve3d.clear();
    curve3d.addPoints( points );

  }
  GKG_CATCH( "void gkg::SpinGlass::track( gkg::LightCurve3d< float >& curve3d, "
             "int32_t neighborCount ) const" );

}


void gkg::SpinGlass::trackFromMiddle( gkg::LightCurve3d< float >& curve3d,
                                      int32_t neighborCount ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a list to store the curve 3D points
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< float > > points;
    int32_t neighborIndex = 0;


    ////////////////////////////////////////////////////////////////////////////
    // initializing current spin glass and current extremity type
    ////////////////////////////////////////////////////////////////////////////

    const gkg::SpinGlass* currentSpinGlass = 0;
    gkg::SpinGlass::ExtremityType 
      currenSpinGlassExtremityType = gkg::SpinGlass::Forward;


    ////////////////////////////////////////////////////////////////////////////
    // allocating pointer to follower spin glass and its extremity
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SpinGlass > followerSpinGlass;
    gkg::SpinGlass::ExtremityType 
      followerConnectedSpinGlassExtremityType = gkg::SpinGlass::Forward;
    gkg::SpinGlass::ExtremityType 
      followerOppositeSpinGlassExtremityType = gkg::SpinGlass::Forward;


    ////////////////////////////////////////////////////////////////////////////
    // tracking forward
    ////////////////////////////////////////////////////////////////////////////

    currentSpinGlass = this;
    currenSpinGlassExtremityType = gkg::SpinGlass::Forward;
    points.push_back( currentSpinGlass->getPosition() );
    bool followerSpinGlassExtremityInitialized = false;
    gkg::Vector3d< float > followerSpinGlassExtremity;

    neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        points.push_back( followerSpinGlass->getPosition() );
        followerSpinGlassExtremity = followerSpinGlass->getExtremity(
                                       followerOppositeSpinGlassExtremityType );
        followerSpinGlassExtremityInitialized = true;

        currentSpinGlass = followerSpinGlass.get();
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );

    if ( followerSpinGlassExtremityInitialized )
    {

      points.push_back( followerSpinGlassExtremity );

    }


    ////////////////////////////////////////////////////////////////////////////
    // tracking backward
    ////////////////////////////////////////////////////////////////////////////

    currentSpinGlass = this;
    currenSpinGlassExtremityType = gkg::SpinGlass::Backward;
    points.push_front( currentSpinGlass->getPosition() );
    followerSpinGlassExtremityInitialized = false;

    neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        points.push_front( followerSpinGlass->getPosition() );
        followerSpinGlassExtremity = followerSpinGlass->getExtremity(
                                       followerOppositeSpinGlassExtremityType );
        followerSpinGlassExtremityInitialized = true;

        currentSpinGlass = followerSpinGlass.get();
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );

    if ( followerSpinGlassExtremityInitialized )
    {

      points.push_front( followerSpinGlassExtremity );

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating 3D curve
    ////////////////////////////////////////////////////////////////////////////

    curve3d.clear();
    curve3d.addPoints( points );

  }
  GKG_CATCH( "void gkg::SpinGlass::trackFromMiddle( "
             "gkg::LightCurve3d< float >& curve3d, "
             "int32_t neighborCount ) const" );

}


gkg::SpinGlass::ExtremityType 
gkg::SpinGlass::getOppositeExtremityType(
                                   gkg::SpinGlass::ExtremityType extremityType )
{

  try
  {

    if ( extremityType == gkg::SpinGlass::Forward )
    {

      return gkg::SpinGlass::Backward;

    }

    return gkg::SpinGlass::Forward;

  }
  GKG_CATCH( "gkg::SpinGlass::ExtremityType "
             "gkg::SpinGlass::getOppositeExtremityType( "
             "gkg::SpinGlass::ExtremityType extremityType )" );

}


std::list< gkg::RCPointer< gkg::SpinGlass > >
gkg::SpinGlass::getNeighborhood(
                              const gkg::RCPointer< gkg::SpinGlass >& spinGlass,
                              int32_t neighborCount )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a list to store the spin glass followers
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::RCPointer< gkg::SpinGlass > > neighborhood;


    ////////////////////////////////////////////////////////////////////////////
    // allocating pointer to follower spin glass and its extremity
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SpinGlass > followerSpinGlass;
    gkg::SpinGlass::ExtremityType 
      followerConnectedSpinGlassExtremityType = gkg::SpinGlass::Forward;
    gkg::SpinGlass::ExtremityType 
      followerOppositeSpinGlassExtremityType = gkg::SpinGlass::Forward;


    ////////////////////////////////////////////////////////////////////////////
    // looking forward
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::SpinGlass > currentSpinGlass = spinGlass;
    gkg::SpinGlass::ExtremityType
      currenSpinGlassExtremityType = gkg::SpinGlass::Forward;
    neighborhood.push_back( currentSpinGlass );

    int32_t neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        neighborhood.push_back( followerSpinGlass );

        currentSpinGlass = followerSpinGlass;
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // looking backward
    ////////////////////////////////////////////////////////////////////////////

    currentSpinGlass = spinGlass;
    currenSpinGlassExtremityType = gkg::SpinGlass::Backward;

    neighborIndex = 0;
    do
    {

      followerSpinGlass = currentSpinGlass->getFollower(
                                       currenSpinGlassExtremityType,
                                       followerConnectedSpinGlassExtremityType,
                                       followerOppositeSpinGlassExtremityType );

      if ( !followerSpinGlass.isNull() )
      {

        neighborhood.push_front( followerSpinGlass );

        currentSpinGlass = followerSpinGlass;
        currenSpinGlassExtremityType = followerOppositeSpinGlassExtremityType;
        ++ neighborIndex;

      }

    }
    while ( !followerSpinGlass.isNull() &&
            ( ( neighborCount < 0 ) || ( neighborIndex <= neighborCount ) ) );

    return neighborhood;

  }
  GKG_CATCH( "std::list< gkg::RCPointer< gkg::SpinGlass > > "
             "gkg::SpinGlass::getNeighborhood( "
             "const gkg::RCPointer< gkg::SpinGlass >& spinGlass, "
             "int32_t neighborCount )" );

}


void gkg::SpinGlass::updateExtremities()
{

  try
  {

    float lengthOver2 = _length / 2.0f;
    
    _backwardExtremity = _position - _orientation * lengthOver2;
    _forwardExtremity = _position + _orientation * lengthOver2;

  }
  GKG_CATCH( "void gkg::SpinGlass::updateExtremities()" );

}
