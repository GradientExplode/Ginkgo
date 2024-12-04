#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-core-exception/Exception.h>


static bool ellipsoidAtomToSphereAtomCollider(
                                         const float* parameters,
                                         bool compressedRepresentation,
                                         const float* otherParameters,
                                         bool /*otherCompressedRepresentation*/,
                                         float& repulsionForceAlongX,
                                         float& repulsionForceAlongY,
                                         float& repulsionForceAlongZ )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // converting the sphere atom to an ellipsoid atom
    ////////////////////////////////////////////////////////////////////////////

    float ellipsoidOtherParameters[ 10 ];
    gkg::EllipsoidAtom::getVectorRepresentationFromStdParameters(
                                  gkg::Vector3d< float >( otherParameters[ 0 ],
                                                          otherParameters[ 1 ],
                                                          otherParameters[ 2 ] ),
                                  otherParameters[ 3 ],
                                  otherParameters[ 3 ],
                                  otherParameters[ 3 ],
                                  gkg::Vector3d< float >( 1.0f, 0.0f, 0.0f ),
                                  gkg::Vector3d< float >( 0.0f, 1.0f, 0.0f ),
                                  gkg::Vector3d< float >( 0.0f, 0.0f, 1.0f ),
                                  ellipsoidOtherParameters,
                                  true );

    return ( gkg::AtomColliderFactory::getInstance().getAtomCollider(
                                     gkg::EllipsoidAtom::getStaticType(),
                                     gkg::EllipsoidAtom::getStaticType() ) )
                                                     ( parameters,
                                                       compressedRepresentation,
                                                       ellipsoidOtherParameters,
                                                       true,
                                                       repulsionForceAlongX,
                                                       repulsionForceAlongY,
                                                       repulsionForceAlongZ );

  }
  GKG_CATCH( "static bool ellipsoidAtomToSphereAtomCollider( "
             "const float* parameters, "
             "bool compressedRepresentation, "
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
    gkg::EllipsoidAtom::getStaticType(),
    gkg::SphereAtom::getStaticType(),
    ellipsoidAtomToSphereAtomCollider );
