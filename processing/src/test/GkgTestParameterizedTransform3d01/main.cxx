#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>


#define FLOATING_SIZE_X 100
#define FLOATING_SIZE_Y 100
#define FLOATING_SIZE_Z 100

#define FLOATING_RES_X 1.0
#define FLOATING_RES_Y 1.0
#define FLOATING_RES_Z 1.0

#define REFERENCE_SIZE_X 100
#define REFERENCE_SIZE_Y 100
#define REFERENCE_SIZE_Z 100

#define REFERENCE_RES_X 1.0
#define REFERENCE_RES_Y 1.0
#define REFERENCE_RES_Z 1.0


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    std::vector< float > parameters( 6, 0 );
    parameters[ 0 ] = M_PI / 2;
    parameters[ 1 ] = 0;
    parameters[ 2 ] = 0;
    parameters[ 3 ] = 0;
    parameters[ 4 ] = 0;
    parameters[ 5 ] = 0;

    gkg::RCPointer< gkg::ParameterizedRigidTransform3d< float > >
        parameterizedTransform3d;
    parameterizedTransform3d.reset(
        new gkg::ParameterizedRigidTransform3d< float > );
    parameterizedTransform3d->setDirectParameters( parameters, true );


    gkg::CompositeTransform3d< float > normalizedTransform3d;
    normalizedTransform3d.compose(
            gkg::RCPointer< gkg::Scaling3d< float > > (
                new gkg::Scaling3d< float > (
                                         ( float )FLOATING_RES_X,
                                         ( float )FLOATING_RES_Y,
                                         ( float )FLOATING_RES_Z ) ),
            false );
    normalizedTransform3d.compose(
        gkg::RCPointer< gkg::Translation3d< float > > (
            new gkg::Translation3d< float >(
          ( float )( FLOATING_SIZE_X - 1 ) *
          ( float )FLOATING_RES_X / 2.0,
          ( float )( FLOATING_SIZE_Y - 1 ) *
          ( float )FLOATING_RES_X / 2.0,
          ( float )( FLOATING_SIZE_Z - 1 ) *
          ( float )FLOATING_RES_X / 2.0,
          false ) ),
        false );
    normalizedTransform3d.compose( parameterizedTransform3d, false );
    normalizedTransform3d.compose(
        gkg::RCPointer< gkg::Translation3d< float > >(
            new gkg::Translation3d< float >(
          ( float )( REFERENCE_SIZE_X - 1 ) *
          ( float )REFERENCE_RES_X / 2.0,
          ( float )( REFERENCE_SIZE_Y - 1 ) *
          ( float )REFERENCE_RES_Y / 2.0,
          ( float )( REFERENCE_SIZE_Z - 1 ) *
          ( float )REFERENCE_RES_Z / 2.0 ) ),
        false );
    normalizedTransform3d.compose(
        gkg::RCPointer< gkg::Scaling3d< float > > (
            new gkg::Scaling3d< float >( ( float )REFERENCE_RES_X,
                                         ( float )REFERENCE_RES_Y,
                                         ( float )REFERENCE_RES_Z,
                                         false  ) ),
        false );

    gkg::Vector3d< float > normalizedFloatingPoint, normalizedReferencePoint;

    normalizedFloatingPoint.x = 0.0;
    normalizedFloatingPoint.y = 50.0;
    normalizedFloatingPoint.z = 0.0;
    normalizedTransform3d.getDirect( normalizedFloatingPoint,
                                     normalizedReferencePoint );
    std::cout << "float = " << normalizedFloatingPoint
              << " -> reference = " << normalizedReferencePoint
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

