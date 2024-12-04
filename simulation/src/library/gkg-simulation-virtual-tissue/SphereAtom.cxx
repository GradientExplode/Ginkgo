#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::SphereAtom::SphereAtom( const gkg::Vector3d< float >& center, 
                             float radius )
                : gkg::Atom()
{

  try
  {

    this->parameters.resize( 4U );
    this->parameters[ 0 ] = center.x;
    this->parameters[ 1 ] = center.y;
    this->parameters[ 2 ] = center.z;
    this->parameters[ 3 ] = radius;

  }
  GKG_CATCH( "gkg::SphereAtom::SphereAtom( "
             "const gkg::Vector3d< float >& center, " 
             "float radius )" );

}


gkg::SphereAtom::SphereAtom()
                : gkg::Atom()
{

  try
  {

    this->parameters.resize( 4U, 0.0f );


  }
  GKG_CATCH( "gkg::SphereAtom::SphereAtom()" );

}


gkg::SphereAtom::SphereAtom( const gkg::SphereAtom& other )
                : gkg::Atom( other )
{
}


gkg::SphereAtom::~SphereAtom()
{
}


gkg::SphereAtom& 
gkg::SphereAtom::operator=( const gkg::SphereAtom& other )
{

  try
  {

    this->gkg::Atom::operator=( other );
    return *this;

  }
  GKG_CATCH( "gkg::SphereAtom& "
             "gkg::SphereAtom::operator=( const gkg::SphereAtom& other )" );

}


gkg::Atom* gkg::SphereAtom::clone() const
{

  try
  {

    return new gkg::SphereAtom( *this );

  }
  GKG_CATCH( "gkg::Atom* gkg::SphereAtom::clone() const" );

}


uint8_t gkg::SphereAtom::getType() const
{

  try
  {

    return gkg::SphereAtom::getStaticType();

  }
  GKG_CATCH( "uint8_t gkg::SphereAtom::getType() const" );

}


void gkg::SphereAtom::setCenter( const Vector3d< float >& center )
{

  try
  {

    parameters[ 0 ] = center.x;
    parameters[ 1 ] = center.y;
    parameters[ 2 ] = center.z;

  }
  GKG_CATCH( "void gkg::SphereAtom::setCenter( "
             "const Vector3d< float >& center )" );


}


void gkg::SphereAtom::setCenter( float centerX,
                                 float centerY,
                                 float centerZ )
{

  try
  {

    parameters[ 0 ] = centerX;
    parameters[ 1 ] = centerY;
    parameters[ 2 ] = centerZ;

  }
  GKG_CATCH( "void gkg::SphereAtom::setCenter( "
             "float centerX, "
             "float centerY, "
             "float centerZ )" );


}


gkg::Vector3d< float > gkg::SphereAtom::getCenter() const
{

  try
  {

    return gkg::Vector3d< float >( parameters[ 0 ],
                                   parameters[ 1 ],
                                   parameters[ 2 ] );

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::SphereAtom::getCenter() const" );

}


float gkg::SphereAtom::getSpanRadius() const
{

  try
  {

    return parameters[ 3 ];

  }
  GKG_CATCH( "float gkg::SphereAtom::getSpanRadius() const" );

}


void gkg::SphereAtom::setRadius( float radius )
{

  try
  {

    parameters[ 3 ] = radius;

  }
  GKG_CATCH( "void gkg::SphereAtom::setRadius( float radius )" );


}


float gkg::SphereAtom::getRadius() const
{

  try
  {

    return parameters[ 3 ];

  }
  GKG_CATCH( "float gkg::SphereAtom::getRadius() const" );

}


float gkg::SphereAtom::getVolume() const
{

  try
  {

    return 4.0f * M_PI * 
           parameters[ 3 ] * parameters[ 3 ] * parameters[ 3 ] / 3.0f;

  }
  GKG_CATCH( "float gkg::SphereAtom::getVolume() const" );

}


bool gkg::SphereAtom::contains( const gkg::Vector3d< float >& position ) const
{

  try
  {

    float differenceX = ( position.x - parameters[ 0 ] );
    float differenceY = ( position.y - parameters[ 1 ] );
    float differenceZ = ( position.z - parameters[ 2 ] );

    return ( ( differenceX * differenceX +
               differenceY * differenceY +
               differenceZ * differenceZ ) <
             ( parameters[ 3 ] * parameters[ 3 ] ) );

  }
  GKG_CATCH( "bool gkg::SphereAtom::contains( "
             "const gkg::Vector3d< float >& position ) const" );

}


void gkg::SphereAtom::getTranslatedParameters( float* outputParameters,
                                               float translationX,
                                               float translationY,
                                               float translationZ ) const
{

  try
  {

    // copying the translated sphere atom center
    outputParameters[ 0 ] = parameters[ 0 ] + translationX;
    outputParameters[ 1 ] = parameters[ 1 ] + translationY;
    outputParameters[ 2 ] = parameters[ 2 ] + translationZ;

    // copying the sphere atom radius
    outputParameters[ 3 ] = parameters[ 3 ];

  }
  GKG_CATCH( "void gkg::SphereAtom::getTranslatedParameters( "
             "float* outputParameters, "
             "float translationX, "
             "float translationY, "
             "float translationZ ) const" );

}


uint8_t gkg::SphereAtom::getStaticType()
{

  try
  {
  
    return 1U;
     
  }
  GKG_CATCH( "uint8_t gkg::SphereAtom::getStaticType()" );

}


void gkg::SphereAtom::getAtomLutVoxel( const float* inputParameters,
                                       bool /*compressedRepresentation*/,
                                       float lowerX,
                                       float lowerY,
                                       float lowerZ,
                                       float spanX,
                                       float spanY,
                                       float spanZ,
                                       int32_t lutSizeX,
                                       int32_t lutSizeY,
                                       int32_t lutSizeZ,
                                       int32_t& lutVoxelX,
                                       int32_t& lutVoxelY,
                                       int32_t& lutVoxelZ )
{

  try
  {

    lutVoxelX = ( int32_t )( ( ( inputParameters[ 0 ] - lowerX ) / spanX ) *
                             ( lutSizeX - 1 ) );
    lutVoxelY = ( int32_t )( ( ( inputParameters[ 1 ] - lowerY ) / spanY ) *
                             ( lutSizeY - 1 ) );
    lutVoxelZ = ( int32_t )( ( ( inputParameters[ 2 ] - lowerZ ) / spanZ ) *
                             ( lutSizeZ - 1 ) );

  }
  GKG_CATCH( "void gkg::SphereAtom::getAtomLutVoxel( "
             "const float* inputParameters, "
             "bool /*compressedRepresentation*/, "
             "float lowerX, "
             "float lowerY, "
             "float lowerZ, "
             "float spanX, "
             "float spanY, "
             "float spanZ, "
             "int32_t lutSizeX, "
             "int32_t lutSizeY, "
             "int32_t lutSizeZ, "
             "int32_t& lutVoxelX, "
             "int32_t& lutVoxelY, "
             "int32_t& lutVoxelZ )" );

}


void gkg::SphereAtom::getAtomLutVoxelSpan( const float* inputParameters,
                                           bool /*compressedRepresentation*/,
                                           float lowerX,
                                           float lowerY,
                                           float lowerZ,
                                           float gridResolutionInUmX,
                                           float gridResolutionInUmY,
                                           float gridResolutionInUmZ,
                                           int32_t lutSizeX,
                                           int32_t lutSizeY,
                                           int32_t lutSizeZ,
                                           int32_t& infLutVoxelX,
                                           int32_t& supLutVoxelX,
                                           int32_t& infLutVoxelY,
                                           int32_t& supLutVoxelY,
                                           int32_t& infLutVoxelZ,
                                           int32_t& supLutVoxelZ )
{

  try
  {

    infLutVoxelX = std::max( ( int32_t )( ( inputParameters[ 0 ] -
                                            inputParameters[ 3 ] - 
                                            lowerX ) /
                                          gridResolutionInUmX ),
                             0 );
    supLutVoxelX = std::min( ( int32_t )( ( inputParameters[ 0 ] +
                                            inputParameters[ 3 ] -
                                            lowerX ) /
                                          gridResolutionInUmX ),
                             lutSizeX - 1 );

    infLutVoxelY = std::max( ( int32_t )( ( inputParameters[ 1 ] -
                                            inputParameters[ 3 ] - 
                                            lowerY ) /
                                          gridResolutionInUmY ),
                             0 );
    supLutVoxelY = std::min( ( int32_t )( ( inputParameters[ 1 ] +
                                            inputParameters[ 3 ] -
                                            lowerY ) /
                                          gridResolutionInUmY ),
                             lutSizeY - 1 );

    infLutVoxelZ = std::max( ( int32_t )( ( inputParameters[ 2 ] -
                                            inputParameters[ 3 ] - 
                                            lowerZ ) /
                                          gridResolutionInUmZ ),
                             0 );
    supLutVoxelZ = std::min( ( int32_t )( ( inputParameters[ 2 ] +
                                            inputParameters[ 3 ] -
                                            lowerZ ) /
                                          gridResolutionInUmZ ),
                             lutSizeZ - 1 );

  }
  GKG_CATCH( "void gkg::SphereAtom::getAtomLutVoxelSpan( "
             "const float* inputParameters, "
             "bool /*compressedRepresentation*/, "
             "float lowerX, "
             "float lowerY, "
             "float lowerZ, "
             "float gridResolutionInUmX, "
             "float gridResolutionInUmY, "
             "float gridResolutionInUmZ, "
             "int32_t lutSizeX, "
             "int32_t lutSizeY, "
             "int32_t lutSizeZ, "
             "int32_t& infLutVoxelX, "
             "int32_t& supLutVoxelX, "
             "int32_t& infLutVoxelY, "
             "int32_t& supLutVoxelY, "
             "int32_t& infLutVoxelZ, "
             "int32_t& supLutVoxelZ )" );

}


bool gkg::SphereAtom::belongTo( const float* atomParameters,
                                bool /*compressedRepresentation*/,
                                const float& coordinateX,
                                const float& coordinateY,
                                const float& coordinateZ )
{

  try
  {

    const float& centerX = atomParameters[ 0 ];
    const float& centerY = atomParameters[ 1 ];
    const float& centerZ = atomParameters[ 2 ];
    const float& radius = atomParameters[ 3 ];

    float distanceAlongX = coordinateX - centerX;
    float distanceAlongY = coordinateY - centerY;
    float distanceAlongZ = coordinateZ - centerZ;

    float squareOfDistance = distanceAlongX * distanceAlongX +
                             distanceAlongY * distanceAlongY +
                             distanceAlongZ * distanceAlongZ;

    if ( squareOfDistance > radius * radius )
    {

     return false;

    }

    return true;                        

  }
  GKG_CATCH( "bool gkg::SphereAtom::belongTo( "
             "const float* atomParameters, "
             "bool /*compressedRepresentation*/, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ )" );

}


float gkg::SphereAtom::distanceToMembrane( const float* atomParameters,
                                           bool /*compressedRepresentation*/,
                                           const float& coordinateX,
                                           const float& coordinateY,
                                           const float& coordinateZ )
{

  try
  {

    const float& centerX = atomParameters[ 0 ];
    const float& centerY = atomParameters[ 1 ];
    const float& centerZ = atomParameters[ 2 ];
    const float& radius = atomParameters[ 3 ];

    float differenceX = coordinateX - centerX;
    float differenceY = coordinateY - centerY;
    float differenceZ = coordinateZ - centerZ;

    return ( float )std::abs( std::sqrt( differenceX * differenceX +
                                         differenceY * differenceY +
                                         differenceZ * differenceZ ) -
                              radius );

  }
  GKG_CATCH( "float gkg::SphereAtom::distanceToMembrane( "
             "const float* atomParameters, "
             "bool /*compressedRepresentation*/, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ )" );

}


void gkg::SphereAtom::getOutwardNormalAt( const float* atomParameters,
                                          bool /*compressedRepresentation*/,
                                          const float& coordinateX,
                                          const float& coordinateY,
                                          const float& coordinateZ,
                                          float& normalX,
                                          float& normalY,
                                          float& normalZ )
{

  try
  {

    normalX = ( coordinateX - atomParameters[ 0 ] ) / atomParameters[ 3 ];
    normalY = ( coordinateY - atomParameters[ 1 ] ) / atomParameters[ 3 ];
    normalZ = ( coordinateZ - atomParameters[ 2 ] ) / atomParameters[ 3 ];

  }
  GKG_CATCH( "void gkg::SphereAtom::getOutwardNormalAt( "
             "const float* atomParameters, "
             "bool /*compressedRepresentation*/, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ, "
             "float& normalX,"
             "float& normalY,"
             "float& normalZ )" );
}


bool gkg::SphereAtom::isRayCollidingWithAtom( const float* atomParameters,
                                              bool /*compressedRepresentation*/,
                                              const float& coordinateX,
                                              const float& coordinateY,
                                              const float& coordinateZ,
                                              const float& stepX,
                                              const float& stepY,
                                              const float& stepZ,
                                              bool insideAtom,
                                              float& intersectionX,
                                              float& intersectionY,
                                              float& intersectionZ,
                                              float& collisionDistance )
{

  try
  {

    bool result = false;

    float a = stepX * stepX +
              stepY * stepY +
              stepZ * stepZ;

    float b = 2.0f * ( stepX * ( coordinateX - atomParameters[ 0 ] ) +
                       stepY * ( coordinateY - atomParameters[ 1 ] ) +
                       stepZ * ( coordinateZ - atomParameters[ 2 ] ) );

    float centerSquared = atomParameters[ 0 ] * atomParameters[ 0 ] +
                          atomParameters[ 1 ] * atomParameters[ 1 ] +
                          atomParameters[ 2 ] * atomParameters[ 2 ];

    float centerDotPosition = atomParameters[ 0 ] * coordinateX +
                              atomParameters[ 1 ] * coordinateY +
                              atomParameters[ 2 ] * coordinateZ;

    float coordinatesSquared = coordinateX * coordinateX +
                               coordinateY * coordinateY +
                               coordinateZ * coordinateZ;

    float c = centerSquared - 2.0f * centerDotPosition + coordinatesSquared -
              atomParameters[ 3 ] * atomParameters[ 3 ];
 
    float delta = b * b - 4.0f * a * c;

    // there are some collisions
    if ( delta >= 0.0f )
    {

      // collision distances
      float root1 = ( -b - std::sqrt( delta ) ) / ( 2.0f * a );
      float root2 = ( -b + std::sqrt( delta ) ) / ( 2.0f * a );

      // collision conditions
      bool firstCollisionCondition = ( ( root1 <= 1.0f ) && ( root1 > 0.0f ) );
      bool secondCollisionCondition = ( ( root2 <= 1.0f ) && ( root2 > 0.0f ) );

      // if collision(s) detected
      if ( firstCollisionCondition || secondCollisionCondition )
      {

        result = true;
        float ratio = 0.0f;

        if ( firstCollisionCondition && secondCollisionCondition )
        {

          ratio = ( insideAtom ? std::max( root1, root2 ) :
                                 std::min( root1, root2 ) );

        }
        else
        {

          ratio = ( firstCollisionCondition ? root1 : root2 );

        }

        float normOfStep = std::sqrt( a );

        intersectionX = coordinateX + ratio * stepX;
        intersectionY = coordinateY + ratio * stepY;
        intersectionZ = coordinateZ + ratio * stepZ;

        collisionDistance = ratio * normOfStep;

      } 

    }

    return result;

  }
  GKG_CATCH( "bool gkg::SphereAtom::isRayCollidingWithAtom( "
             "const float* atomParameters, "
             "bool /*compressedRepresentation*/, "
             "const float& coordinateX, "
             "const float& coordinateY, "
             "const float& coordinateZ, "
             "const float& stepX, "
             "const float& stepY, "
             "const float& stepZ, "
             "bool insideAtom, "
             "float& intersectionX, "
             "float& intersectionY, "
             "float& intersectionZ, "
             "float& collisionDistance )" );
}


//
// adding static methods to atom method factory
//


static bool registerGetAtomLutVoxelMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerGetAtomLutVoxelMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getAtomLutVoxel );
    
static bool registerGetAtomLutVoxelSpanMethodToFactory __attribute__((unused)) =
  gkg::AtomMethodFactory::getInstance().registerGetAtomLutVoxelSpanMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getAtomLutVoxelSpan );
    
static bool registerBelongToMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerBelongToMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::belongTo );
    
static bool registerDistanceToMembraneMethodToFactory __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerDistanceToMembraneMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::distanceToMembrane );

static bool registerOutwardNormaToMembraneMethodToFactory 
                                                      __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerOutwardNormalToMembraneMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::getOutwardNormalAt );

static bool registerIsRayCollidingWithAtomMethodToFactory
                                                      __attribute__((unused)) = 
  gkg::AtomMethodFactory::getInstance().registerIsRayCollidingWithAtomMethod(
    gkg::SphereAtom::getStaticType(),
    gkg::SphereAtom::isRayCollidingWithAtom );

