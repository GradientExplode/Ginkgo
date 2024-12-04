#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-core-exception/Exception.h>


static bool sphereAtomToEllipsoidAtomCollider(
                                             const float* parameters,
                                             bool /*compressedRepresentation*/,
                                             const float* otherParameters,
                                             bool otherCompressedRepresentation,
                                             float& repulsionForceAlongX,
                                             float& repulsionForceAlongY,
                                             float& repulsionForceAlongZ )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // converting the sphere atom to an ellipsoid atom
    ////////////////////////////////////////////////////////////////////////////

    float ellipsoidParameters[ 10 ];
    gkg::EllipsoidAtom::getVectorRepresentationFromStdParameters(
                                  gkg::Vector3d< float >( parameters[ 0 ],
                                                          parameters[ 1 ],
                                                          parameters[ 2 ] ),
                                  parameters[ 3 ],
                                  parameters[ 3 ],
                                  parameters[ 3 ],
                                  gkg::Vector3d< float >( 1.0f, 0.0f, 0.0f ),
                                  gkg::Vector3d< float >( 0.0f, 1.0f, 0.0f ),
                                  gkg::Vector3d< float >( 0.0f, 0.0f, 1.0f ),
                                  ellipsoidParameters,
                                  true );

    return ( gkg::AtomColliderFactory::getInstance().getAtomCollider(
                                     gkg::EllipsoidAtom::getStaticType(),
                                     gkg::EllipsoidAtom::getStaticType() ) )
                                                ( ellipsoidParameters,
                                                  true,
                                                  otherParameters,
                                                  otherCompressedRepresentation,
                                                  repulsionForceAlongX,
                                                  repulsionForceAlongY,
                                                  repulsionForceAlongZ );

  }
  GKG_CATCH( "static bool sphereAtomToEllipsoidAtomCollider( "
             "const float* parameters, "
             "bool /*compressedRepresentation*/, "
             "const float* otherParameters, "
             "bool otherCompressedRepresentation, "
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
    gkg::EllipsoidAtom::getStaticType(),
    sphereAtomToEllipsoidAtomCollider );
