#include <gkg-simulation-virtual-tissue/AtomSmootherFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::AtomSmootherFactory::AtomSmootherFactory()
{

  try
  {

    _atomSmoothers.resize( 16U );

    int32_t i = 0;
    int32_t j = 0;
    for ( i = 0; i < 16; i++ )
    {

      _atomSmoothers[ i ].resize( 16U );
      for ( j = 0; j < 16; j++ )
      {

        _atomSmoothers[ i ][ j ].resize( 16U, 0 );

      }

    }

  }
  GKG_CATCH( "gkg::AtomSmootherFactory::AtomSmootherFactory()" );

}


gkg::AtomSmootherFactory::~AtomSmootherFactory()
{
}


bool gkg::AtomSmootherFactory::registerAtomSmoother(
                           uint8_t atomType1,
                           uint8_t atomType2,
                           uint8_t atomType3,
                           gkg::AtomSmootherFactory::AtomSmoother atomSmoother )
{

  try
  {

    if ( atomSmoother )
    {

      if ( _atomSmoothers[ ( int32_t )atomType1 ]
                         [ ( int32_t )atomType2 ]
                         [ ( int32_t )atomType3 ] )
      {

        std::cerr << "gkg::AtomSmootherFactory::registerAtomSmoother: "
                  << "atom type " << atomType1 << " / "
                  << "atom type " << atomType2 << " / "
                  << "atom type " << atomType3 << " smoother already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _atomSmoothers[ ( int32_t )atomType1 ]
                      [ ( int32_t )atomType2 ]
                      [ ( int32_t )atomType3 ] =
          atomSmoother;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomSmootherFactory::registerAtomSmoother( "
             "uint8_t atomType1, "
             "uint8_t atomType2, "
             "uint8_t atomType3, "
             "gkg::AtomSmootherFactory::AtomSmoother atomSmoother )" );

}


gkg::AtomSmootherFactory::AtomSmoother
gkg::AtomSmootherFactory::getAtomSmoother( uint8_t atomType1,
                                           uint8_t atomType2,
                                           uint8_t atomType3 ) const
{

  try
  {


    if ( ( atomType1 < 16U ) &&
         ( atomType2 < 16U ) &&
         ( atomType3 < 16U ) )
    {

      gkg::AtomSmootherFactory::AtomSmoother
        atomSmoother = _atomSmoothers[ ( int32_t )atomType1 ]
                                     [ ( int32_t )atomType2 ]
                                     [ ( int32_t )atomType3 ];

      if ( atomSmoother )
      {

        return atomSmoother;

      }
      else
      {

        throw std::runtime_error( 
          std::string( "no atom smoother registered for atom types " ) +
          gkg::StringConverter::toString( ( int32_t )atomType1 ) + " / " +
          gkg::StringConverter::toString( ( int32_t )atomType2 ) + " / " +
          gkg::StringConverter::toString( ( int32_t )atomType3 ) );

      }

    }
    else
    {

      throw std::runtime_error( 
                           "atom types out of range, should belong to [0;15]" );

    }

    return 0;

  }
  GKG_CATCH( "gkg::AtomSmootherFactory::AtomSmoother "
             "gkg::AtomSmootherFactory::getAtomSmoother( "
             "uint8_t atomType1, "
             "uint8_t atomType2, "
             "uint8_t atomType3 ) const" );

}

