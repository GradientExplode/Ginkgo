#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-core-exception/Exception.h>



static bool sphereAtomToSphereAtomCollider(
                                         const float* parameters,
                                         bool /*compressedRepresentation*/,
                                         const float* otherParameters,
                                         bool /*otherCompressedRepresentation*/,
                                         float& repulsionForceAlongX,
                                         float& repulsionForceAlongY,
                                         float& repulsionForceAlongZ )
{

  try
  {

    float directionAlongX = parameters[ 0 ] - 
                            otherParameters[ 0 ];
    float directionAlongY = parameters[ 1 ] - 
                            otherParameters[ 1 ];
    float directionAlongZ = parameters[ 2 ] - 
                            otherParameters[ 2 ];

    float norm = std::sqrt( directionAlongX * directionAlongX + 
                            directionAlongY * directionAlongY +
                            directionAlongZ * directionAlongZ ) + 1e-9;
    float overlap = 0.5f * ( parameters[ 3 ] + otherParameters[ 3 ] -
                             norm );

    bool hasCollision = false;

    if ( overlap > 0.0f )
    {

      hasCollision = true;

      float overlapOverNorm = overlap / norm; 

      repulsionForceAlongX = directionAlongX * overlapOverNorm;
      repulsionForceAlongY = directionAlongY * overlapOverNorm;
      repulsionForceAlongZ = directionAlongZ * overlapOverNorm;

    }
    
    return hasCollision;


  }
  GKG_CATCH( "static bool sphereAtomToSphereAtomCollider( "
             "const float* parameters, "
             "bool /*compressedRepresentation*/, "
             "const float* otherParameters, "
             "bool /*otherCompressedRepresentation*/, "
             "float& repulsionForceAlongX, "
             "float& repulsionForceAlongY, "
             "float& repulsionForceAlongZ )" );


}


//
// adding static methods to atom collider factory
//


static bool registerToAtomColliderFactory __attribute__((unused)) = 
  gkg::AtomColliderFactory::getInstance().registerAtomCollider(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getStaticType(),
    sphereAtomToSphereAtomCollider );
