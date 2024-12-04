#include <gkg-simulation-virtual-tissue/AtomForceApplierFactory.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-core-exception/Exception.h>



static void ellipsoidAtomForceApplier( const float& repulsionForceAlongX,
                                       const float& repulsionForceAlongY,
                                       const float& repulsionForceAlongZ,
                                       float* parameters,
                                       bool compressedRepresentation )
{

  try
  {

    gkg::EllipsoidAtom::
                      getTranslatedVectorRepresentationFromVectorRepresentation(
                                                     parameters,
                                                     compressedRepresentation,
                                                     repulsionForceAlongX,
                                                     repulsionForceAlongY,
                                                     repulsionForceAlongZ,
                                                     parameters,
                                                     compressedRepresentation );

  }
  GKG_CATCH( "static void ellipsoidAtomForceApplier( "
             "const float& repulsionForceAlongX, "
             "const float& repulsionForceAlongY, "
             "const float& repulsionForceAlongZ, "
             "float* parameters, "
             "bool compressedRepresentation )" );


}


//
// adding static methods to atom force applier factory
//


static bool registerToAtomForceApplierFactory __attribute__((unused)) = 
  gkg::AtomForceApplierFactory::getInstance().registerAtomForceApplier(
    gkg::EllipsoidAtom::getStaticType(),
    ellipsoidAtomForceApplier );
