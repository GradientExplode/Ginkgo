#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-mesh/MeshTransformer_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define QSPHERE_VERTEX_COUNT 2000


gkg::QSpaceSampling::QSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                    : gkg::RCObject(),
                      _gradientCharacteristics( gradientCharacteristics )
{

  try
  {

    _transform3d = transform3d.clone();

  }
  GKG_CATCH( "gkg::QSpaceSampling::QSpaceSampling( "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::QSpaceSampling::~QSpaceSampling()
{
}


int32_t gkg::QSpaceSampling::getCount() const
{

  try
  {
  
    return ( int32_t )_bValues.size();
  
  }
  GKG_CATCH( "int32_t gkg::QSpaceSampling::getCount() const" );

}


float gkg::QSpaceSampling::getQMagnitude( int32_t rank ) const
{

  try
  {

    return std::sqrt( _bValues[ rank ] / _diffusionTimes[ rank ] );

  }
  GKG_CATCH( "float gkg::QSpaceSampling::getQMagnitude( int32_t rank ) const" );


}


float gkg::QSpaceSampling::getBValue( int32_t rank ) const
{

  try
  {

    return _bValues[ rank ];

  }
  GKG_CATCH( "float gkg::QSpaceSampling::getBValue( int32_t rank ) const" );


}


float gkg::QSpaceSampling::getDiffusionTime( int32_t rank ) const
{

  try
  {

    return _diffusionTimes[ rank ];

  }
  GKG_CATCH( "float gkg::QSpaceSampling::getDiffusionTime( "
             "int32_t rank ) const" );


}


gkg::Vector3d< float >
gkg::QSpaceSampling::getOrientation( int32_t rank ) const
{

  try
  {

    gkg::Vector3d< float > to1;
    _transform3d->getDirect( _orientations[ rank ], to1 );

    gkg::Rotation3d< float > motionRotation3d( _motionRotation3ds[ rank ] );
    gkg::Vector3d< float > to2;
    motionRotation3d.getDirect( to1, to2 );

    return to2;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::QSpaceSampling::getOrientation( int32_t rank ) const" );


}


gkg::Vector3d< float >
gkg::QSpaceSampling::getMotionRotation3d( int32_t rank ) const
{

  try
  {

    return _motionRotation3ds[ rank ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::QSpaceSampling::getMotionRotation3d( int32_t rank ) const" );


}


void gkg::QSpaceSampling::setMotionRotation3d(
                                int32_t rank,
                                const gkg::Vector3d< float >& motionRotation3d )
{

  try
  {

    _motionRotation3ds[ rank ] = motionRotation3d;

  }
  GKG_CATCH( "void gkg::QSpaceSampling::setMotionRotation3d( "
             "int32_t rank, "
             "const gkg::Vector3d< float >& motionRotation3d ) const" );

}


void gkg::QSpaceSampling::addMotionRotation3d(
                                int32_t rank,
                                const gkg::Vector3d< float >& motionRotation3d )
{

  try
  {

    gkg::CompositeTransform3d< float > compositeTransform3d;

    compositeTransform3d.compose( 
                       gkg::Rotation3d< float >( _motionRotation3ds[ rank ] ) );
    compositeTransform3d.compose( 
                       gkg::Rotation3d< float >( motionRotation3d ) );

    const gkg::HomogeneousTransform3d< float >&
      composition = compositeTransform3d.getComposition();

    gkg::Vector3d< float > u;
    gkg::Vector3d< float > v;
    gkg::Vector3d< float > w;

    u.x = composition.getDirectCoefficient( 0, 0 );
    u.y = composition.getDirectCoefficient( 1, 0 );
    u.z = composition.getDirectCoefficient( 2, 0 );

    v.x = composition.getDirectCoefficient( 0, 1 );
    v.y = composition.getDirectCoefficient( 1, 1 );
    v.z = composition.getDirectCoefficient( 2, 1 );

    w.x = composition.getDirectCoefficient( 0, 2 );
    w.y = composition.getDirectCoefficient( 1, 2 );
    w.z = composition.getDirectCoefficient( 2, 2 );

    gkg::Vector3d< float >
      angles = gkg::getXYZAnglesFromTrieder< float >( u, v, w, false );

    _motionRotation3ds[ rank ] = angles;

  }
  GKG_CATCH( "void gkg::QSpaceSampling::setMotionRotation3d( "
             "int32_t rank, "
             "const gkg::Vector3d< float >& motionRotation3d ) const" );

}


const gkg::Dictionary& gkg::QSpaceSampling::getGradientCharacteristics() const
{

  try
  {

    return _gradientCharacteristics;

  }
  GKG_CATCH( "const gkg::Dictionary& "
             "gkg::QSpaceSampling::getGradientCharacteristics() const" );

}




void gkg::QSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // adding b-values
    headeredObject.getHeader().addAttribute( "bvalues", _bValues );

    // adding orientations
    int32_t orientationCount = ( int32_t )_orientations.size();
    gkg::GenericObjectList golOrientations( orientationCount );
    gkg::Vector3d< float > orientation;
    gkg::GenericObjectList golOrientation( 3 );
    int32_t o = 0;
    for ( o = 0; o < orientationCount; o++ )
    {
    
      orientation = getOrientation( o );
      golOrientation[ 0 ] = orientation.x;
      golOrientation[ 1 ] = orientation.y;
      golOrientation[ 2 ] = orientation.z;
      golOrientations[ o ] = golOrientation;
    
    }
    headeredObject.getHeader().addAttribute( "diffusion_gradient_orientations",
                                             golOrientations );

  }
  GKG_CATCH( "void gkg::QSpaceSampling::addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


void gkg::QSpaceSampling::getMeshes(
                 float sphereRadius,
                 int32_t sphereVertexCount,
                 bool withSymmetrical,
                 std::list< gkg::MeshMap< int32_t, float, 3U > >& meshes ) const
{

  try
  {

    ///////////////////////////////////////////////////////////////////////////
    // building the mesh of a sphere
    ///////////////////////////////////////////////////////////////////////////

    gkg::OrientationSet orientationSet(
              gkg::ElectrostaticOrientationSet( sphereVertexCount / 2
                                               ).getSymmetricalOrientations() );
    gkg::MeshMap< int32_t, float, 3U > sphereMeshMap;    
    gkg::ConvexHull::getInstance().addConvexHull( 
                                               orientationSet.getOrientations(),
                                               0,
                                               sphereMeshMap );

    int32_t sphereVertexCount = 
      sphereMeshMap.vertices.getSiteCount( 0 );
    int32_t spherePolygonCount = 
      sphereMeshMap.polygons.getPolygonCount( 0 );
    int32_t orientationCount = getCount();

    ///////////////////////////////////////////////////////////////////////////
    // preparing mesh accumulator
    ///////////////////////////////////////////////////////////////////////////

    gkg::MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             sphereVertexCount * orientationCount,
                             spherePolygonCount * orientationCount );

    ///////////////////////////////////////////////////////////////////////////
    // preparing translation
    ///////////////////////////////////////////////////////////////////////////

    gkg::Translation3d< float > translation;

    ///////////////////////////////////////////////////////////////////////////
    // preparing mesh scaler and transformer
    ///////////////////////////////////////////////////////////////////////////

    gkg::MeshScaler< int32_t, float, 3U > meshScaler;
    gkg::MeshTransformer< int32_t, float, 3U > meshTransformer;

    ///////////////////////////////////////////////////////////////////////////
    // creating the scale vector
    ///////////////////////////////////////////////////////////////////////////

    std::vector< float > scaling( sphereVertexCount, sphereRadius /
                      std::sqrt( this->_diffusionTimes[ 0 ] ) );

    ///////////////////////////////////////////////////////////////////////////
    // adding sphere meshes
    ///////////////////////////////////////////////////////////////////////////

    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      // collecting the orientation o and its Q-space magnitude
      const gkg::Vector3d< float >& orientation = getOrientation( o );
      float qMagnitude = getQMagnitude( o );

      // processing 3D transform
      translation.setDirectTranslation( qMagnitude * orientation.x,
                                        qMagnitude * orientation.y,
                                        qMagnitude * orientation.z );

      // processing local scaled mesh map
      gkg::MeshMap< int32_t, float, 3U > localMeshMap( sphereMeshMap );
      meshScaler.scale( localMeshMap, 0, scaling, localMeshMap );
      meshTransformer.transform( localMeshMap, translation, localMeshMap );

      // accumulating local mesh map
      meshAccumulator.add( localMeshMap );

      if ( withSymmetrical )
      {

        // processing 3D transform
        translation.setDirectTranslation( -qMagnitude * orientation.x,
                                          -qMagnitude * orientation.y,
                                          -qMagnitude * orientation.z );

        // processing symmetrical local scaled mesh map
        localMeshMap = sphereMeshMap;
        meshScaler.scale( localMeshMap, 0, scaling, localMeshMap );
        meshTransformer.transform( localMeshMap, translation, localMeshMap );

        // accumulating symmetrical local mesh map
        meshAccumulator.add( localMeshMap );

      }

    }

    ///////////////////////////////////////////////////////////////////////////
    // adding accumulator to mesh
    ///////////////////////////////////////////////////////////////////////////

    gkg::MeshMap< int32_t, float, 3U > mesh;
    meshes.push_back( mesh ); 

    meshes.back().add( meshAccumulator );

    ///////////////////////////////////////////////////////////////////////////
    // when spherical Q-space sampling, adding q-spheres
    ///////////////////////////////////////////////////////////////////////////

    if ( dynamic_cast< const gkg::SingleShellQSpaceSampling* >( this ) )
    {

      const gkg::SingleShellQSpaceSampling*
        singleShellQSpaceSampling = 
          dynamic_cast< const gkg::SingleShellQSpaceSampling* >( this );

      gkg::OrientationSet qSphereOrientationSet(
        gkg::ElectrostaticOrientationSet( QSPHERE_VERTEX_COUNT / 2 ).
                                                 getSymmetricalOrientations() );
      gkg::MeshMap< int32_t, float, 3U > qSphereMeshMap;    
      gkg::ConvexHull::getInstance().addConvexHull( 
                                        qSphereOrientationSet.getOrientations(),
                                        0,
                                        qSphereMeshMap );
      std::vector< float >
        qSphereScaling(
                    QSPHERE_VERTEX_COUNT,
                    ( float )std::sqrt(
                             singleShellQSpaceSampling->getBValue() /
                             singleShellQSpaceSampling->getDiffusionTime() ) );
      meshScaler.scale( qSphereMeshMap, 0, qSphereScaling, qSphereMeshMap );
      meshes.push_back( qSphereMeshMap );

    }
    else if ( dynamic_cast< const gkg::MultipleShellQSpaceSampling* >( this ) )
    {

      const gkg::MultipleShellQSpaceSampling*
        multipleShellQSpaceSampling = 
          dynamic_cast< const gkg::MultipleShellQSpaceSampling* >( this );

      gkg::OrientationSet qSphereOrientationSet(
        gkg::ElectrostaticOrientationSet( QSPHERE_VERTEX_COUNT / 2 ).
                                                 getSymmetricalOrientations() );

      int32_t shell = 0;
      for ( shell = 0; shell < multipleShellQSpaceSampling->getShellCount();
            shell++ )
      {

        gkg::MeshMap< int32_t, float, 3U > qSphereMeshMap;    
        gkg::ConvexHull::getInstance().addConvexHull( 
                                        qSphereOrientationSet.getOrientations(),
                                        0,
                                        qSphereMeshMap );
        std::vector< float >
          qSphereScaling(
           QSPHERE_VERTEX_COUNT,
           ( float )std::sqrt(
                   multipleShellQSpaceSampling->getBValue( shell ) /
                   multipleShellQSpaceSampling->getDiffusionTime( shell ) ) );
        meshScaler.scale( qSphereMeshMap, 0, qSphereScaling, qSphereMeshMap );
        meshes.push_back( qSphereMeshMap );

      }

    }

  }
  GKG_CATCH( "void gkg::QSpaceSampling::getMesh( "
             "float sphereRadius, "
             "int32_t sphereVertexCount, "
             "gkg::MeshMap< int32_t, float, 3U >& mesh ) const" );

}


const gkg::Dictionary& 
gkg::QSpaceSampling::getRawGradientCharacteristics() const
{

  try
  {

    return _gradientCharacteristics;

  }
  GKG_CATCH( "const gkg::Dictionary& "
             "gkg::QSpaceSampling::getRawGradientCharacteristics() const" );

}


const gkg::RCPointer< gkg::HomogeneousTransform3d< float > >&
gkg::QSpaceSampling::getRawTransform3d() const
{

  try
  {

    return _transform3d;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::HomogeneousTransform3d< float > >& "
             "gkg::QSpaceSampling::getRawTransform3d() const" );

}


const std::vector< float >& gkg::QSpaceSampling::getRawBValues() const
{

  try
  {

    return _bValues;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::QSpaceSampling::getRawBValues() cons" );

}


const std::vector< float >& gkg::QSpaceSampling::getRawDiffusionTimes() const
{

  try
  {

    return _diffusionTimes;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::QSpaceSampling::getRawDiffusionTimes() const" );

}


const std::vector< gkg::Vector3d< float > >& 
gkg::QSpaceSampling::getRawMotionRotation3ds() const
{

  try
  {

    return _motionRotation3ds;

  }
  GKG_CATCH( "const std::vector< gkg::Vector3d< float > >& "
             "gkg::QSpaceSampling::getRawMotionRotation3ds() const" );

}


const std::vector< gkg::Vector3d< float > >& 
gkg::QSpaceSampling::getRawOrientations() const
{

  try
  {

    return _orientations;

  }
  GKG_CATCH( "const std::vector< gkg::Vector3d< float > >& "
             "gkg::QSpaceSampling::getRawOrientations() const" );

}


void gkg::QSpaceSampling::saveBValAndBVecFiles(
                                         const std::string& fileNameBVal,
                                         const std::string& fileNameBVec ) const
{

  try
  {

    // saving the output *.bval file
    std::ofstream os( fileNameBVal );
    if ( !os )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileNameBVal + "'" );

    }

    std::vector< float >::const_iterator
      b = _bValues.begin(),
      be = _bValues.end();
    while ( b != be )
    {

      os << *b << " ";
      ++ b;

    }
    os << std::endl;

    os.close();

    // saving the output *.bvec file
    os.open( fileNameBVec );
    if ( !os )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileNameBVec + "'" );

    }

    int32_t orientationCount = ( int32_t )_orientations.size();
    std::vector< gkg::Vector3d< float > >
      rotatedOrientations( orientationCount );
    int32_t o = 0;
    for ( o = 0; o < orientationCount; o++ )
    {

      rotatedOrientations[ o ] = this->getOrientation( o );

    }
    std::vector< gkg::Vector3d< float > >::const_iterator
      ro = rotatedOrientations.begin(),
      roe = rotatedOrientations.end();
    while ( ro != roe )
    {

      os << ro->x << " ";
      ++ ro;

    }
    os << std::endl;

    ro = rotatedOrientations.begin();
    while ( ro != roe )
    {

      os << ro->y << " ";
      ++ ro;

    }
    os << std::endl;

    ro = rotatedOrientations.begin();
    while ( ro != roe )
    {

      os << ro->z << " ";
      ++ ro;

    }
    os << std::endl;

    os.close();

  }
  GKG_CATCH( "void gkg::QSpaceSampling::saveBValAndBVecFiles( "
             "const std::string& fileNameBVal, "
             "const std::string& fileNameBVec ) const" );

}


void gkg::QSpaceSampling::extractDiffusionTimes()
{

  try
  {

    if ( _gradientCharacteristics[ "type" ]->getString() == "PGSE" )
    {

      gkg::GenericObjectList
        little_delta = _gradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = _gradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      int32_t rankCount = ( int32_t )_bValues.size();

      if ( little_delta.size() != big_delta.size() )
      {

        throw std::runtime_error(
              "little and big delta fields don't have the same item count(s)" );

      }

      if ( ( int32_t )little_delta.size() != rankCount )
      {

        throw std::runtime_error(
          "little/big delta fields and b-values don't have the same item "
          "count(s)" );

      }

      _diffusionTimes.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        _diffusionTimes[ r ] = ( float )big_delta[ r ]->getScalar() -
                               ( float )little_delta[ r ]->getScalar() / 3.0f;

      }

    }
    else if ( _gradientCharacteristics[ "type" ]->getString() == "Sine-OGSE" )
    {

      gkg::GenericObjectList
        little_delta = _gradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = _gradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        period = _gradientCharacteristics[ "period" ]
                                         ->getValue< gkg::GenericObjectList >();


      gkg::GenericObjectList
        phase = _gradientCharacteristics[ "phase" ]
                                         ->getValue< gkg::GenericObjectList >();


      int32_t rankCount = ( int32_t )_bValues.size();

      if ( ( little_delta.size() != big_delta.size() ) || 
           ( little_delta.size() != period.size() ) || 
           ( little_delta.size() != phase.size() ) )
      {

        throw std::runtime_error(
              "little, big delta, period and phase fields don't have the same "
              "item count(s)" );

      }

      if ( ( int32_t )little_delta.size() != rankCount )
      {

        throw std::runtime_error(
          "little/big delta/period/phase fields and b-values don't have the "
          "same item count(s)" );

      }

      _diffusionTimes.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        _diffusionTimes[ r ] = ( float )big_delta[ r ]->getScalar() -
                               ( float )little_delta[ r ]->getScalar() / 3.0f;

      }

    }
    else if ( _gradientCharacteristics[ "type" ]->getString() == "Cosine-OGSE" )
    {

      gkg::GenericObjectList
        little_delta = _gradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = _gradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        period = _gradientCharacteristics[ "period" ]
                                         ->getValue< gkg::GenericObjectList >();


      gkg::GenericObjectList
        phase = _gradientCharacteristics[ "phase" ]
                                         ->getValue< gkg::GenericObjectList >();


      int32_t rankCount = ( int32_t )_bValues.size();

      if ( ( little_delta.size() != big_delta.size() ) || 
           ( little_delta.size() != period.size() ) || 
           ( little_delta.size() != phase.size() ) )
      {

        throw std::runtime_error(
              "little, big delta, period and phase fields don't have the same "
              "item count(s)" );

      }

      if ( ( int32_t )little_delta.size() != rankCount )
      {

        throw std::runtime_error(
          "little/big delta/period/phase fields and b-values don't have the "
          "same item count(s)" );

      }

      _diffusionTimes.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        _diffusionTimes[ r ] = ( float )big_delta[ r ]->getScalar() -
                               ( float )little_delta[ r ]->getScalar() / 3.0f;

      }

    }
    else if ( _gradientCharacteristics[ "type" ]->getString() == "unknown" )
    {

      int32_t rankCount = ( int32_t )_bValues.size();

      _diffusionTimes.resize( rankCount );
      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        _diffusionTimes[ r ] = 1.0f;

      }

    }
    else
    {

      throw std::runtime_error( "invalid gradient characteristics" );

    }

  }
  GKG_CATCH( "void gkg::QSpaceSampling::extractDiffusionTimes()" );

}


void gkg::QSpaceSampling::
                   addGradientCharacteristicsToDiffusionSensitizationParameters(
                       gkg::Dictionary& diffusionSensitizationParameters ) const
{

  try
  {

    diffusionSensitizationParameters[ "gradient-characteristics" ] =
                                                       _gradientCharacteristics;

  }
  GKG_CATCH( "void "
             "gkg::QSpaceSampling::"
             "addGradientCharacteristicsToDiffusionSensitizationParameters( "
             "gkg::Dictionary& diffusionSensitizationParameters ) const" );

}






#undef QSPHERE_VERTEX_COUNT
