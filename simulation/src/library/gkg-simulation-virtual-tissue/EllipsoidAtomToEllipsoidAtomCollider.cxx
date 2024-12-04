#include <gkg-simulation-virtual-tissue/AtomColliderFactory.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


static bool ellipsoidAtomToEllipsoidAtomCollider(
                                             const float* parameters,
                                             bool compressedRepresentation,
                                             const float* otherParameters,
                                             bool otherCompressedRepresentation,
                                             float& repulsionForceAlongX,
                                             float& repulsionForceAlongY,
                                             float& repulsionForceAlongZ )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the standard parameters of the first ellipsoid
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > center;
    float radius1 = 0.0f;
    float radius2 = 0.0f;
    float radius3 = 0.0f;
    gkg::Vector3d< float > axis1;
    gkg::Vector3d< float > axis2;
    gkg::Vector3d< float > axis3;
    gkg::EllipsoidAtom::getStdParametersFromVectorRepresentation(
                                                       parameters,
                                                       compressedRepresentation,
                                                       center,
                                                       radius1,
                                                       radius2,
                                                       radius3,
                                                       axis1,
                                                       axis2,
                                                       axis3 );

    float offsetX = std::sqrt( ( ( axis1.x * axis1.x * radius1 * radius1 ) +
                                 ( axis1.y * axis1.y * radius2 * radius2 ) +
                                 ( axis1.z * axis1.z * radius3 * radius3 ) ) );

    float offsetY = std::sqrt( ( ( axis2.x * axis2.x * radius1 * radius1 ) +
                                 ( axis2.y * axis2.y * radius2 * radius2 ) +
                                 ( axis2.z * axis2.z * radius3 * radius3 ) ) );

    float offsetZ = std::sqrt( ( ( axis3.x * axis3.x * radius1 * radius1 ) +
                                 ( axis3.y * axis3.y * radius2 * radius2 ) +
                                 ( axis3.z * axis3.z * radius3 * radius3 ) ) );

    float lowerX = center.x - offsetX;
    float lowerY = center.y - offsetY;
    float lowerZ = center.z - offsetZ;
    float upperX = center.x + offsetX;
    float upperY = center.y + offsetY;
    float upperZ = center.z + offsetZ;


    ////////////////////////////////////////////////////////////////////////////
    // computing the standard parameters of the second ellipsoid
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > otherCenter;
    float otherRadius1 = 0.0f;
    float otherRadius2 = 0.0f;
    float otherRadius3 = 0.0f;
    gkg::Vector3d< float > otherAxis1;
    gkg::Vector3d< float > otherAxis2;
    gkg::Vector3d< float > otherAxis3;

    gkg::EllipsoidAtom::getStdParametersFromVectorRepresentation(
                                                  otherParameters,
                                                  otherCompressedRepresentation,
                                                  otherCenter,
                                                  otherRadius1,
                                                  otherRadius2,
                                                  otherRadius3,
                                                  otherAxis1,
                                                  otherAxis2,
                                                  otherAxis3 );

    float otherOffsetX = std::sqrt(
            ( ( otherAxis1.x * otherAxis1.x * otherRadius1 * otherRadius1 ) +
              ( otherAxis1.y * otherAxis1.y * otherRadius2 * otherRadius2 ) +
              ( otherAxis1.z * otherAxis1.z * otherRadius3 * otherRadius3 ) ) );

    float otherOffsetY = std::sqrt(
            ( ( otherAxis2.x * otherAxis2.x * otherRadius1 * otherRadius1 ) +
              ( otherAxis2.y * otherAxis2.y * otherRadius2 * otherRadius2 ) +
              ( otherAxis2.z * otherAxis2.z * otherRadius3 * otherRadius3 ) ) );

    float otherOffsetZ = std::sqrt(
            ( ( otherAxis3.x * otherAxis3.x * otherRadius1 * otherRadius1 ) +
              ( otherAxis3.y * otherAxis3.y * otherRadius2 * otherRadius2 ) +
              ( otherAxis3.z * otherAxis3.z * otherRadius3 * otherRadius3 ) ) );

    float otherLowerX = otherCenter.x - otherOffsetX;
    float otherLowerY = otherCenter.y - otherOffsetY;
    float otherLowerZ = otherCenter.z - otherOffsetZ;
    float otherUpperX = otherCenter.x + otherOffsetX;
    float otherUpperY = otherCenter.y + otherOffsetY;
    float otherUpperZ = otherCenter.z + otherOffsetZ;


    ////////////////////////////////////////////////////////////////////////////
    // if the bounding boxes of the two ellipsoids do not overlap, then they
    // simply do not collide, so return false
    ////////////////////////////////////////////////////////////////////////////

    if ( ! ( ( ( upperX >= otherLowerX ) && ( otherUpperX >= lowerX ) ) &&
             ( ( upperY >= otherLowerY ) && ( otherUpperY >= lowerY ) ) &&
             ( ( upperZ >= otherLowerZ ) && ( otherUpperZ >= lowerZ ) ) ) )
    {

      return false;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the maximum radius of the first and second ellipsoid
    ////////////////////////////////////////////////////////////////////////////

    float maximumRadius = std::max( std::max( radius1, radius2 ), radius3 );
    float otherMaximumRadius = std::max( std::max( otherRadius1, otherRadius2 ),
                                         otherRadius3 );


    ////////////////////////////////////////////////////////////////////////////
    // computing the matrix representation of the first ellipsoid
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix matrixRepresentation;
    gkg::EllipsoidAtom::getMatrixRepresentationFromVectorRepresentation(
                                                       parameters,
                                                       compressedRepresentation,
                                                       matrixRepresentation );


    ////////////////////////////////////////////////////////////////////////////
    // computing the matrix representation of the second ellipsoid
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix otherMatrixRepresentation;
    gkg::EllipsoidAtom::getMatrixRepresentationFromVectorRepresentation(
                                                  otherParameters,
                                                  otherCompressedRepresentation,
                                                  otherMatrixRepresentation );


    ////////////////////////////////////////////////////////////////////////////
    // getting static pointer to the numerical analysis implementation factory
    ////////////////////////////////////////////////////////////////////////////

    static gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // computing the intersection matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix inverseOtherMatrixRepresentation(4,4);

    factory->getInverse( otherMatrixRepresentation,
                         inverseOtherMatrixRepresentation );

    gkg::Matrix 
      intersectionMatrix = inverseOtherMatrixRepresentation.getComposition(
                                                         matrixRepresentation );


    ////////////////////////////////////////////////////////////////////////////
    // computing the eigensystem of the intersection matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::CVector eigenValues;
    gkg::CMatrix eigenVectors;
    factory->getNonSymmetricEigenSystem( intersectionMatrix,
                                         eigenValues,
                                         eigenVectors );


    ////////////////////////////////////////////////////////////////////////////
    // checking if there is a collision and computing repulsion force
    ////////////////////////////////////////////////////////////////////////////

    bool hasCollision = false;
    if ( ( eigenValues.getImaginaryPart().getNorm() > 0.0 ) ||
         ( factory->getMinimumItem( eigenValues.getRealPart() ) > 0.0 ) )
    {

      hasCollision = true;
      float directionAlongX = center.x - otherCenter.x;
      float directionAlongY = center.y - otherCenter.y;
      float directionAlongZ = center.z - otherCenter.z;

      float norm = std::sqrt( directionAlongX * directionAlongX + 
                              directionAlongY * directionAlongY +
                              directionAlongZ * directionAlongZ ) + 1e-9;
      float overlap = 0.5f * ( maximumRadius + otherMaximumRadius -
                               norm );
      float overlapOverNorm = overlap / norm; 

      repulsionForceAlongX = directionAlongX * overlapOverNorm;
      repulsionForceAlongY = directionAlongY * overlapOverNorm;
      repulsionForceAlongZ = directionAlongZ * overlapOverNorm;

    }

    return hasCollision;

  }
  GKG_CATCH( "static bool ellipsoidAtomToEllipsoidAtomCollider( "
             "const float* parameters, "
             "bool compressedRepresentation, "
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
    gkg::EllipsoidAtom::getStaticType(),
    gkg::EllipsoidAtom::getStaticType(),
    ellipsoidAtomToEllipsoidAtomCollider );
