#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::AtomColliderFactory::AtomColliderFactory()
{

  try
  {

    _atomColliders.resize( 16U );

    int32_t i = 0;
    for ( i = 0; i < 16; i++ )
    {

      _atomColliders[ i ].resize( 16U, 0 );

    }

  }
  GKG_CATCH( "gkg::AtomColliderFactory::AtomColliderFactory()" );

}


gkg::AtomColliderFactory::~AtomColliderFactory()
{
}


bool gkg::AtomColliderFactory::registerAtomCollider(
                           uint8_t atomType1,
                           uint8_t atomType2,
                           gkg::AtomColliderFactory::AtomCollider atomCollider )
{

  try
  {

    if ( atomCollider )
    {

      if ( _atomColliders[ ( int32_t )atomType1 ][ ( int32_t )atomType2 ] )
      {

        std::cerr << "gkg::AtomColliderFactory::registerAtomCollider: "
                  << "atom type " << atomType1 << " to "
                  << "atom type " << atomType2 << " collider already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _atomColliders[ ( int32_t )atomType1 ][ ( int32_t )atomType2 ] =
          atomCollider;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomColliderFactory::registerAtomCollider( "
             "uint8_t atomType1, "
             "uint8_t atomType2, "
             "gkg::AtomColliderFactory::AtomCollider atomCollider )" );

}


gkg::AtomColliderFactory::AtomCollider
gkg::AtomColliderFactory::getAtomCollider( uint8_t atomType1,
                                           uint8_t atomType2 ) const
{

  try
  {


    if ( ( atomType1 < 16U ) &&
         ( atomType2 < 16U ) )
    {

      gkg::AtomColliderFactory::AtomCollider
        atomCollider = _atomColliders[ ( int32_t )atomType1 ]
                                     [ ( int32_t )atomType2 ];

      if ( atomCollider )
      {

        return atomCollider;

      }
      else
      {

        throw std::runtime_error( 
          std::string( "no atom collider registered for atom type " ) +
          gkg::StringConverter::toString( ( int32_t )atomType1 ) +
          " and atom type " + 
          gkg::StringConverter::toString( ( int32_t )atomType2 ) );

      }

    }
    else
    {

      throw std::runtime_error( 
                           "atom types out of range, should belong to [0;15]" );

    }

    return 0;

  }
  GKG_CATCH( "gkg::AtomColliderFactory::AtomCollider "
             "gkg::AtomColliderFactory::getAtomCollider( "
             "uint8_t atomType1, "
             "uint8_t atomType2 ) const" );

}

