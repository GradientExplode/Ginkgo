#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassConnection::SpinGlassConnection(
                         uint64_t identity,
                         const std::pair< gkg::RCPointer< gkg::SpinGlass >,
                                          gkg::SpinGlass::ExtremityType >&
                                                    spinGlassAndExtremityType1,
                         const std::pair< gkg::RCPointer< gkg::SpinGlass >,
                                          gkg::SpinGlass::ExtremityType >&
                                                    spinGlassAndExtremityType2 )
                         : _identity( identity ),
                           _spinGlassAndExtremityType1(
                                                   spinGlassAndExtremityType1 ),
                           _spinGlassAndExtremityType2(
                                                   spinGlassAndExtremityType2 )
{

  try
  {

    _spinGlassAndExtremityType1.first->setConnectedSpinGlass(
                                             spinGlassAndExtremityType1.second,
                                             spinGlassAndExtremityType2.first );

    _spinGlassAndExtremityType2.first->setConnectedSpinGlass(
                                             spinGlassAndExtremityType2.second,
                                             spinGlassAndExtremityType1.first );

    // then registering this connection to each spin glass
//    gkg::RCPointer< gkg::SpinGlassConnection >
//      thisSpinGlassConnection( this );
//    spinGlass1->setSpinGlassConnection( spinGlassExtremityType1,
//                                        thisSpinGlassConnection );
//    spinGlass2->setSpinGlassConnection( spinGlassExtremityType2,
//                                        thisSpinGlassConnection );


  }
  GKG_CATCH( "gkg::SpinGlassConnection::SpinGlassConnection( "
             "uint64_t identity, "
             "const std::pair< gkg::RCPointer< gkg::SpinGlass >, "
             "gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType1, "
             "const std::pair< gkg::RCPointer< gkg::SpinGlass >, "
             "gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType2 )" );

}


gkg::SpinGlassConnection::SpinGlassConnection(
                                         const gkg::SpinGlassConnection& other )
                         : _identity( other._identity ),
                           _spinGlassAndExtremityType1(
                                            other._spinGlassAndExtremityType1 ),
                           _spinGlassAndExtremityType2( 
                                            other._spinGlassAndExtremityType2 )
{
}


gkg::SpinGlassConnection::~SpinGlassConnection()
{
}


gkg::SpinGlassConnection&
gkg::SpinGlassConnection::operator=( const gkg::SpinGlassConnection& other )
{

  try
  {

    _identity = other._identity;
    _spinGlassAndExtremityType1 = other._spinGlassAndExtremityType1;
    _spinGlassAndExtremityType2 = other._spinGlassAndExtremityType2;

    return *this;

  }
  GKG_CATCH( "gkg::SpinGlassConnection& "
             "gkg::SpinGlassConnection::operator=( "
             "const gkg::SpinGlassConnection& other )" );

}


uint64_t gkg::SpinGlassConnection::getIdentity() const
{

  try
  {

    return _identity;

  }
  GKG_CATCH( "uint64_t gkg::SpinGlassConnection::getIdentity() const" );

}


const gkg::RCPointer< gkg::SpinGlass >& 
gkg::SpinGlassConnection::getSpinGlass1() const
{

  try
  {

    return _spinGlassAndExtremityType1.first;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlass >& "
             "gkg::SpinGlassConnection::getSpinGlass1() const" );

}


const gkg::SpinGlass::ExtremityType& 
gkg::SpinGlassConnection::getSpinGlassExtremityType1() const
{

  try
  {

    return _spinGlassAndExtremityType1.second;

  }
  GKG_CATCH( "const gkg::SpinGlass::ExtremityType& "
             "gkg::SpinGlassConnection::getSpinGlassExtremityType1() const" );


}


const gkg::RCPointer< gkg::SpinGlass >& 
gkg::SpinGlassConnection::getSpinGlass2() const
{

  try
  {

    return _spinGlassAndExtremityType2.first;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::SpinGlass >& "
             "gkg::SpinGlassConnection::getSpinGlass2() const" );

}


const gkg::SpinGlass::ExtremityType& 
gkg::SpinGlassConnection::getSpinGlassExtremityType2() const
{

  try
  {

    return _spinGlassAndExtremityType2.second;

  }
  GKG_CATCH( "const gkg::SpinGlass::ExtremityType& "
             "gkg::SpinGlassConnection::getSpinGlassExtremityType2() const" );


}


void gkg::SpinGlassConnection::getExtremityTypeOfConnectedSpinGlass(
                            const gkg::RCPointer< gkg::SpinGlass >& spinGlass,
                            gkg::SpinGlass::ExtremityType& extremityType ) const
{

  try
  {

    if ( _spinGlassAndExtremityType1.first == spinGlass )
    {

      extremityType = _spinGlassAndExtremityType2.second;

    }
    else if ( _spinGlassAndExtremityType2.first == spinGlass )
    {

      extremityType = _spinGlassAndExtremityType1.second;

    }
    else
    {

      // should normally never enter this case
      throw std::runtime_error( "invalid connection" );

    }

  }
  GKG_CATCH( "void "
             "gkg::SpinGlassConnection::getExtremityTypeOfConnectedSpinGlass( "
             "const gkg::RCPointer< gkg::SpinGlass >& spinGlass, "
             "gkg::SpinGlass::ExtremityType& extremityType ) const" );

}


gkg::Vector3d< float >
gkg::SpinGlassConnection::getFirstExtremity() const
{

  try
  {

    return _spinGlassAndExtremityType1.first->getExtremity(
                                           _spinGlassAndExtremityType1.second );

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::SpinGlassConnection::getFirstExtremity() const" );

}


gkg::Vector3d< float >
gkg::SpinGlassConnection::getSecondExtremity() const
{

  try
  {

    return _spinGlassAndExtremityType2.first->getExtremity(
                                           _spinGlassAndExtremityType2.second );

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::SpinGlassConnection::getSecondExtremity() const" );

}

/*
float gkg::SpinGlassConnection::getInteractionPotential() const
{

  try
  {

    // retrieving the first spin glass information
    const gkg::Vector3d< float >&
      spinGlassPosition1 = _spinGlassAndExtremityType1.first->getPosition();
    const gkg::Vector3d< float >&
      spinGlassExtremity1 = _spinGlassAndExtremityType1.first->getExtremity(
                                           _spinGlassAndExtremityType1.second );

    float halfLength1 = _spinGlassAndExtremityType1.first->getLength() / 2.0;
    float connectionLikelihood1 = 
      _spinGlassAndExtremityType1.first->getConnectionLikelihood();

    // retrieving the second spin glass information
    const gkg::Vector3d< float >&
      spinGlassPosition2 = _spinGlassAndExtremityType2.first->getPosition();
    const gkg::Vector3d< float >&
      spinGlassExtremity2 = _spinGlassAndExtremityType2.first->getExtremity(
                                           _spinGlassAndExtremityType2.second );

    float halfLength2 = _spinGlassAndExtremityType2.first->getLength() / 2.0;
    float connectionLikelihood2 = 
      _spinGlassAndExtremityType2.first->getConnectionLikelihood();

    // computing the middle point
    gkg::Vector3d< float > 
      middlePoint( ( spinGlassPosition1.x + spinGlassPosition2.x ) / 2.0,
                   ( spinGlassPosition1.y + spinGlassPosition2.y ) / 2.0,
                   ( spinGlassPosition1.z + spinGlassPosition2.z ) / 2.0 );

    // computing the interaction potential
    float interactionPotential = 
                        ( spinGlassExtremity1 - middlePoint ).getNorm2() / 
                        ( halfLength1 * halfLength1 ) +
                        ( spinGlassExtremity2 - middlePoint ).getNorm2() / 
                        ( halfLength2 * halfLength2 ) -
                        ( connectionLikelihood1 + connectionLikelihood2 ) / 2.0;

    return interactionPotential;

  }
  GKG_CATCH( "float "
             "gkg::SpinGlassConnection::getInteractionPotential() const" );

}
*/

void gkg::SpinGlassConnection::disconnect()
{

  try
  {

    // disconnecting the first spin glass and the spin glass connection
    _spinGlassAndExtremityType1.first->resetConnectedSpinGlass(
                                           _spinGlassAndExtremityType1.second );
    _spinGlassAndExtremityType1.first->resetSpinGlassConnection(
                                           _spinGlassAndExtremityType1.second );

    // disconnecting the second spin glass and the spin glass connection
    _spinGlassAndExtremityType2.first->resetConnectedSpinGlass(
                                           _spinGlassAndExtremityType2.second );
    _spinGlassAndExtremityType2.first->resetSpinGlassConnection(
                                           _spinGlassAndExtremityType2.second );

  }
  GKG_CATCH( "void gkg::SpinGlassConnection::disconnect()" );

}


void gkg::SpinGlassConnection::connect(
  const std::pair< gkg::RCPointer< gkg::SpinGlass >,
                   gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType1,
  const std::pair< gkg::RCPointer< gkg::SpinGlass >,
                   gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType2 )
{

  try
  {

    const gkg::RCPointer< gkg::SpinGlass >& spinGlass1 =
                   spinGlassAndExtremityType1.first;
    const gkg::SpinGlass::ExtremityType& 
                   spinGlassExtremityType1 = spinGlassAndExtremityType1.second;

    const gkg::RCPointer< gkg::SpinGlass >& spinGlass2 =
                   spinGlassAndExtremityType2.first;
    const gkg::SpinGlass::ExtremityType& 
                   spinGlassExtremityType2 = spinGlassAndExtremityType2.second;

    // need to check in which order the extremities are given
    if ( _spinGlassAndExtremityType1.first == spinGlass1 )
    {

      // setting back the connected spin glass of the first spin glass and make
      // both point to this spin glass connection again
      _spinGlassAndExtremityType1.first->setConnectedSpinGlass(
                                                        spinGlassExtremityType1,
                                                        spinGlass2 );

      _spinGlassAndExtremityType2.first->setConnectedSpinGlass(
                                                        spinGlassExtremityType2,
                                                        spinGlass1 );

    }
    else
    {

      // setting back the connected spin glass of the second spin glass and make
      // both point to this spin glass connection again
      _spinGlassAndExtremityType1.first->setConnectedSpinGlass(
                                                        spinGlassExtremityType2,
                                                        spinGlass1 );

      _spinGlassAndExtremityType2.first->setConnectedSpinGlass(
                                                        spinGlassExtremityType1,
                                                        spinGlass2 );

    }

    // then registering this connection to each spin glass
//     gkg::RCPointer< gkg::SpinGlassConnection >
//       thisSpinGlassConnection( this );
//     spinGlass1->setSpinGlassConnection( spinGlassExtremityType1,
//                                         thisSpinGlassConnection );
//     spinGlass2->setSpinGlassConnection( spinGlassExtremityType2,
//                                         thisSpinGlassConnection );

  }
  GKG_CATCH( "void gkg::SpinGlassConnection::connect( "
              "const std::pair< gkg::RCPointer< gkg::SpinGlass >, "
              "gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType1, "
              "const std::pair< gkg::RCPointer< gkg::SpinGlass >, "
              "gkg::SpinGlass::ExtremityType >& spinGlassAndExtremityType2 )" );

}
