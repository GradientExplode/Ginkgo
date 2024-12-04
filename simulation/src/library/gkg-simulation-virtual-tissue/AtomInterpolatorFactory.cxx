#include <gkg-simulation-virtual-tissue/AtomInterpolatorFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::AtomInterpolatorFactory::AtomInterpolatorFactory()
{

  try
  {

    _atomInterpolators.resize( 16U );

    int32_t i = 0;
    for ( i = 0; i < 16; i++ )
    {

      _atomInterpolators[ i ].resize( 16U, 0 );

    }

  }
  GKG_CATCH( "gkg::AtomInterpolatorFactory::AtomInterpolatorFactory()" );

}


gkg::AtomInterpolatorFactory::~AtomInterpolatorFactory()
{
}


bool gkg::AtomInterpolatorFactory::registerAtomInterpolator(
               uint8_t atomType1,
               uint8_t atomType2,
               gkg::AtomInterpolatorFactory::AtomInterpolator atomInterpolator )
{

  try
  {

    if ( atomInterpolator )
    {

      if ( _atomInterpolators[ ( int32_t )atomType1 ]
                             [ ( int32_t )atomType2 ] )
      {

        std::cerr << "gkg::AtomInterpolatorFactory::registerAtomInterpolator: "
                  << "atom type " << atomType1 << " / "
                  << "atom type " << atomType2 << " smoother already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _atomInterpolators[ ( int32_t )atomType1 ]
                          [ ( int32_t )atomType2 ] =
          atomInterpolator;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomInterpolatorFactory::registerAtomInterpolator( "
             "uint8_t atomType1, "
             "uint8_t atomType2, "
             "gkg::AtomInterpolatorFactory::AtomInterpolator "
             "atomInterpolator )" );

}


gkg::AtomInterpolatorFactory::AtomInterpolator
gkg::AtomInterpolatorFactory::getAtomInterpolator( uint8_t atomType1,
                                                   uint8_t atomType2 ) const
{

  try
  {


    if ( ( atomType1 < 16U ) &&
         ( atomType2 < 16U ) )
    {

      gkg::AtomInterpolatorFactory::AtomInterpolator
        atomInterpolator = _atomInterpolators[ ( int32_t )atomType1 ]
                                             [ ( int32_t )atomType2 ];

      if ( atomInterpolator )
      {

        return atomInterpolator;

      }
      else
      {

        throw std::runtime_error( 
          std::string( "no atom smoother registered for atom types " ) +
          gkg::StringConverter::toString( ( int32_t )atomType1 ) + " / " +
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
  GKG_CATCH( "gkg::AtomInterpolatorFactory::AtomInterpolator "
             "gkg::AtomInterpolatorFactory::getAtomInterpolator( "
             "uint8_t atomType1, "
             "uint8_t atomType2 ) const" );

}

