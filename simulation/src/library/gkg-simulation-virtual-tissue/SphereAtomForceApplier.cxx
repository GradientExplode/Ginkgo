#include <gkg-simulation-virtual-tissue/AtomForceApplierFactory.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-core-exception/Exception.h>



static void sphereAtomForceApplier( const float& repulsionForceAlongX,
                                    const float& repulsionForceAlongY,
                                    const float& repulsionForceAlongZ,
                                    float* parameters,
                                    bool /*compressedRepresentation*/ )
{

  try
  {

    parameters[ 0 ] += repulsionForceAlongX;
    parameters[ 1 ] += repulsionForceAlongY;
    parameters[ 2 ] += repulsionForceAlongZ;

  }
  GKG_CATCH( "static void sphereAtomForceApplier( "
             "const float& repulsionForceAlongX, "
             "const float& repulsionForceAlongY, "
             "const float& repulsionForceAlongZ, "
             "float* parameters, "
             "bool /*compressedRepresentation*/ )" );


}


//
// adding static methods to atom force applier factory
//


static bool registerToAtomForceApplierFactory __attribute__((unused)) = 
  gkg::AtomForceApplierFactory::getInstance().registerAtomForceApplier(
    gkg::SphereAtom::getStaticType(),
    sphereAtomForceApplier );
