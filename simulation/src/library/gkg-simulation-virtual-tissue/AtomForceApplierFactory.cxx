#include <gkg-simulation-virtual-tissue/AtomForceApplierFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::AtomForceApplierFactory::AtomForceApplierFactory()
{

  try
  {

    _atomForceAppliers.resize( 16U, 0 );

  }
  GKG_CATCH( "gkg::AtomForceApplierFactory::AtomForceApplierFactory()" );

}


gkg::AtomForceApplierFactory::~AtomForceApplierFactory()
{
}


bool gkg::AtomForceApplierFactory::registerAtomForceApplier(
               uint8_t atomType,
               gkg::AtomForceApplierFactory::AtomForceApplier atomForceApplier )
{

  try
  {

    if ( atomForceApplier )
    {

      if ( _atomForceAppliers[ ( int32_t )atomType ] )
      {

        std::cerr << "gkg::AtomForceApplierFactory::registerAtomForceApplier: "
                  << "atom type " << atomType << " force applier already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _atomForceAppliers[ ( int32_t )atomType ] = atomForceApplier;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomForceApplierFactory::registerAtomForceApplier( "
             "uint8_t atomType1, "
             "gkg::AtomForceApplierFactory::AtomForceApplier "
             "atomForceApplier )" );

}


gkg::AtomForceApplierFactory::AtomForceApplier
gkg::AtomForceApplierFactory::getAtomForceApplier( uint8_t atomType ) const
{

  try
  {


    if ( atomType < 16U )
    {

      gkg::AtomForceApplierFactory::AtomForceApplier
        atomForceApplier = _atomForceAppliers[ ( int32_t )atomType ];

      if ( atomForceApplier )
      {

        return atomForceApplier;

      }
      else
      {

        throw std::runtime_error( 
          std::string( "no atom force applier registered for atom " ) +
          gkg::StringConverter::toString( ( int32_t )atomType ) +
          " type" );

      }

    }
    else
    {

      throw std::runtime_error( 
                            "atom type out of range, should belong to [0;15]" );

    }

    return 0;

  }
  GKG_CATCH( "gkg::AtomForceApplierFactory::AtomForceApplier "
             "gkg::AtomForceApplierFactory::getAtomForceApplier( "
             "uint8_t atomType ) const" );

}

