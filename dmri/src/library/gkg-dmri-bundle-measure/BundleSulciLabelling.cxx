#include <gkg-dmri-bundle-measure/BundleSulciLabelling.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// class SulciPart
////////////////////////////////////////////////////////////////////////////////

gkg::SulciPart::SulciPart()
               : label( -1 ),
                 boundingBox( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f ),
                 gravityCenter( 0.0f, 0.0f, 0.0f ),
                 direction( 0.0f, 0.0f, 0.0f )
{
}


gkg::SulciPart::SulciPart( int16_t theLabel,
                           const gkg::BoundingBox< float >& theBoundingBox,
                           const gkg::Vector3d< float >& theGravityCenter,
                           const gkg::Vector3d< float >& theDirection )
               : label( theLabel ),
                 boundingBox( theBoundingBox ),
                 gravityCenter( theGravityCenter ),
                 direction( theDirection )
{
}


gkg::SulciPart::SulciPart( const gkg::SulciPart& other )
               : label( other.label ),
                 boundingBox( other.boundingBox ),
                 gravityCenter( other.gravityCenter ),
                 direction( other.direction )
{
}


gkg::SulciPart::~SulciPart()
{
}


gkg::SulciPart&
gkg::SulciPart::operator=( const gkg::SulciPart& other )
{

  try
  {

    label = other.label;
    boundingBox = other.boundingBox;
    gravityCenter = other.gravityCenter;
    direction = other.direction;

    return *this;

  }
  GKG_CATCH( "gkg::SulciPart& "
             "gkg::SulciPart::operator=( "
             "const gkg::SulciPart& other )" );

}


////////////////////////////////////////////////////////////////////////////////
// class SulciInformation
////////////////////////////////////////////////////////////////////////////////

gkg::SulciInformation::SulciInformation()
                      : label( -1 )
{
}


gkg::SulciInformation::SulciInformation(
                              const std::string& theName,
                              int16_t theLabel,
                              const std::list< gkg::SulciPart >& theSulciParts )
                      : name( theName ),
                        label( theLabel ),
                        sulciParts( theSulciParts )
{
}


gkg::SulciInformation::SulciInformation( const gkg::SulciInformation& other )
               : name( other.name ),
                 label( other.label ),
                 sulciParts( other.sulciParts )
{
}


gkg::SulciInformation::~SulciInformation()
{
}


gkg::SulciInformation&
gkg::SulciInformation::operator=( const gkg::SulciInformation& other )
{

  try
  {

    name = other.name;
    label = other.label;
    sulciParts = other.sulciParts;

    return *this;

  }
  GKG_CATCH( "gkg::SulciInformation& "
             "gkg::SulciInformation::operator=( "
             "const gkg::SulciInformation& other )" );

}


////////////////////////////////////////////////////////////////////////////////
// class BundleSulciLabelling
////////////////////////////////////////////////////////////////////////////////

gkg::BundleSulciLabelling::BundleSulciLabelling(
       gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > pialMesh,
       gkg::RCPointer< std::vector< gkg::SulciInformation > > sulciInformations,
       gkg::RCPointer< gkg::Volume< int16_t > > sulciMask,
       gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromPialMeshToSulciMask,
       gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromBundleToSulciMask,
       float resamplingStep,
       float maximumDisplacement,
       float displacementStep,
       int32_t neighborhoodSize,
       double extremityTukeyApodizationAlphaWeight,
       double orthogonalityTukeyApodizationAlphaWeight,
       double extremityTukeyApodizationPower,
       double orthogonalityTukeyApodizationPower )
                          : gkg::BundleMeasure< std::vector< float > >(),
                           _pialMesh( pialMesh ),
                           _sulciInformations( sulciInformations ),
                           _sulciMask( sulciMask ),
                           _transform3dFromPialMeshToSulciMask(
                                           transform3dFromPialMeshToSulciMask ),
                           _transform3dFromBundleToSulciMask(
                                             transform3dFromBundleToSulciMask ),
                           _resamplingStep( resamplingStep ),
                           _maximumDisplacement( maximumDisplacement ),
                           _displacementStep( displacementStep ),
                           _neighborhoodSize( neighborhoodSize ),
                           _extremityTukeyApodizationAlphaWeight(
                                         extremityTukeyApodizationAlphaWeight ),
                           _orthogonalityTukeyApodizationAlphaWeight(
                                     orthogonalityTukeyApodizationAlphaWeight ),
                           _extremityTukeyApodizationPower(
                                               extremityTukeyApodizationPower ),
                           _orthogonalityTukeyApodizationPower(
                                           orthogonalityTukeyApodizationPower ),
                           _sulciMaximumLabel( 0 ),
                           _sulciMaskSize( 1, 1, 1 ),
                           _sulciMaskResolution( 1.0, 1.0, 1.0 )


{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( _pialMesh.isNull() )
    {

      throw std::runtime_error( "empty pial mesh pointer" );

    }


    if ( _sulciInformations.isNull() )
    {

      throw std::runtime_error( "empty sulci informations pointer" );

    }

    if ( _sulciMask.isNull() )
    {

      throw std::runtime_error( "empty sulci mask pointer" );

    }

    if ( _resamplingStep <= 0.0f )
    {

      throw std::runtime_error( "resamling step must be strictly positive" );

    }

    if ( _maximumDisplacement <= 0.0f )
    {

      throw std::runtime_error(
                             "maximum displacement must be strictly positive" );

    }

    if ( _displacementStep <= 0.0f )
    {

      throw std::runtime_error(
                                "displacement step must be strictly positive" );

    }

    if ( _neighborhoodSize < 1 )
    {

      throw std::runtime_error(
                            "neighborhood size must be greater or equal to 1" );

    }

    if ( ( _extremityTukeyApodizationAlphaWeight < 0.0 ) ||
         ( _extremityTukeyApodizationAlphaWeight > 1.0 ) )
    {

      throw std::runtime_error( "extremity Tukey apodization alpha weight "
                                "must lie within [ 0.0; 1.0 ]" );

    }

    if ( ( _orthogonalityTukeyApodizationAlphaWeight < 0.0 ) ||
         ( _orthogonalityTukeyApodizationAlphaWeight > 1.0 ) )
    {

      throw std::runtime_error( "orthogonality Tukey apodization alpha weight "
                                "must lie within [ 0.0; 1.0 ]" );

    }

    if ( _extremityTukeyApodizationPower <= 0.0 )
    {

      throw std::runtime_error( "extremity Tukey apodization power "
                                "must be strictly positive" );

    }

    if ( _orthogonalityTukeyApodizationPower <= 0.0 )
    {

      throw std::runtime_error( "orthogonality Tukey apodization power "
                                "must be strictly positive" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // identifying rotation present in _transform3dFromPialMeshToSulciMask
    ////////////////////////////////////////////////////////////////////////////

    // if set by default to identity, there is no rotation
    const gkg::Rotation3d< float >* rotation3dFromPialMeshToSulciMask = 0;
    if ( dynamic_cast< gkg::RigidTransform3d< float >* >(
                                  _transform3dFromPialMeshToSulciMask.get() ) )
    {

      rotation3dFromPialMeshToSulciMask = &( static_cast<
                                              gkg::RigidTransform3d< float >* >(
                 _transform3dFromPialMeshToSulciMask.get() )->getRotation3d() );

    }
    else if ( dynamic_cast< gkg::AffineTransform3d< float >* >(
                                  _transform3dFromPialMeshToSulciMask.get() ) )
    {

      rotation3dFromPialMeshToSulciMask = &( static_cast<
                                             gkg::AffineTransform3d< float >* >(
                 _transform3dFromPialMeshToSulciMask.get() )->getRotation3d() );

    }

    ////////////////////////////////////////////////////////////////////////////
    // collecting mask information
    ////////////////////////////////////////////////////////////////////////////

    _sulciMaximumLabel = 0;
    gkg::Volume< int16_t >::const_iterator
      s = _sulciMask->begin(),
      se = _sulciMask->end();
    while ( s != se )
    {

      if ( *s > _sulciMaximumLabel )
      {

        _sulciMaximumLabel = *s;

      }
      ++ s;

    }

    _sulciMask->getSize( _sulciMaskSize );
    _sulciMask->getResolution( _sulciMaskResolution );
    _boundingBoxSulciMask = gkg::BoundingBox< int32_t >( *_sulciMask );


    ////////////////////////////////////////////////////////////////////////////
    // collecting pial mesh information and resizing vertex and normal vector(s)
    ////////////////////////////////////////////////////////////////////////////

    // collecting the vertices and normals, and storing them into vectors to 
    // allow the storage of the index to a vertex and to a normal instead of 
    // duplicating them
    const std::list< gkg::Vector3d< float > >&
      vertexList = _pialMesh->vertices.getSites( 0 );
    _vertices.resize( vertexList.size() );
    _boundingBoxPialMesh = _pialMesh->vertices.getBoundingBox( 0 );

    // collecting the outward normals to all the vertices
    std::list< gkg::Vector3d< float > >
      normalList = _pialMesh->getNormals( 0 ); 
    _normals.resize( normalList.size() );


    ////////////////////////////////////////////////////////////////////////////
    // computing a vertex LUT to speed-up the computation: the LUT stores for 
    // each voxel the list of indices to the vertex of the pial mesh
    ////////////////////////////////////////////////////////////////////////////

    _meshVertexLut.reset( new gkg::Volume< std::list< int32_t >* >(
                                                             _sulciMaskSize ) );
    _meshVertexLut->setResolution( _sulciMaskResolution );
    _meshVertexLut->fill( 0 );

    int32_t vertexIndex = 0;
    std::list< gkg::Vector3d< float > >::const_iterator
      iv = vertexList.begin(),
      ive = vertexList.end();
    std::list< gkg::Vector3d< float > >::const_iterator
      in = normalList.begin();
    std::vector< gkg::Vector3d< float > >::iterator
      ov = _vertices.begin();
    std::vector< gkg::Vector3d< float > >::iterator
      on = _normals.begin();
    gkg::Vector3d< int32_t > voxelInSulciMaskFrame;
    while ( iv != ive )
    {

      // getting the vertex position into the sulci mask frame
      _transform3dFromPialMeshToSulciMask->getDirect( *iv, *ov );

      // the normal direction should only be rotated, but not translated
      if ( !rotation3dFromPialMeshToSulciMask )
      {

        *on = *in;

      }
      else
      {

        rotation3dFromPialMeshToSulciMask->getDirect( *in, *on );

      }

      // getting the voxel position in the sulci mask frame
      voxelInSulciMaskFrame.x = ( int32_t )( ov->x / _sulciMaskResolution.x );
      voxelInSulciMaskFrame.y = ( int32_t )( ov->y / _sulciMaskResolution.y );
      voxelInSulciMaskFrame.z = ( int32_t )( ov->z / _sulciMaskResolution.z );

      // checking that voxel lies within the sulci mask bounding box
      if ( _boundingBoxSulciMask.contains( voxelInSulciMaskFrame ) )
      {

        std::list< int32_t >*& 
          vertexIndexListPointer = ( *_meshVertexLut )( voxelInSulciMaskFrame );
        // if list not allocated yet, just allocate it
        if ( vertexIndexListPointer == 0 )
        {

          vertexIndexListPointer = new std::list< int32_t >;

        }
        // adding the vertex index to the list
        vertexIndexListPointer->push_back( vertexIndex );

      }
      ++ vertexIndex;
      ++ iv;
      ++ in;
      ++ ov;
      ++ on;

    }
        
  }
  GKG_CATCH( "gkg::BundleSulciLabelling::BundleSulciLabelling( "
             "gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > > pialMesh, "
             "gkg::RCPointer< gkg::Volume< int16_t > > sulciMask, "
             "const std::vector< std::string >& sulciNames, "
             "const std::vector< gkg::SulciInformation >& sulciInformations, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromPialMeshToSulciMask, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromBundleToSulciMask, "
             "float resamplingStep, "
             "float maximumDisplacement, "
             "float displacementStep, "
             "int32_t neighborhoodSize, "
             "double extremityTukeyApodizationAlphaWeight, "
             "double orthogonalityTukeyApodizationAlphaWeight, "
             "double extremityTukeyApodizationPower, "
             "double orthogonalityTukeyApodizationPower )" );
}


gkg::BundleSulciLabelling::~BundleSulciLabelling()
{

  gkg::Volume< std::list< int32_t >* >::iterator
    l = _meshVertexLut->begin(),
    le = _meshVertexLut->end();
  while ( l != le )
  {

    if ( *l )
    {

      delete *l;

    }
    ++ l;

  }

}


std::vector< float >
gkg::BundleSulciLabelling::get(
                         const gkg::BundleSulciLabelling::Bundle& bundle ) const
{

  try
  {  

    ////////////////////////////////////////////////////////////////////////////
    // allocating the output vector giving for each sulci label
    // the number of fiber points that were close to it
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > matchedSulciHistogram( _sulciMaximumLabel + 1, 0 );

    int32_t closestVertexIndex = -1;


    ////////////////////////////////////////////////////////////////////////////
    // looping over fiber(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > fiberPointInSulciMaskFrame;
    gkg::Vector3d< int32_t > voxelInSulciMaskFrame;
    gkg::Vector3d< int32_t > voxelNeighborInSulciMaskFrame;
    int32_t nx = 0;
    int32_t ny = 0;
    int32_t nz = 0;
    float distance = 0.0f;
    float minimumDistance = 0.0f;
    float angleBetweenFiberAndSulciInDegrees = 0.0f;
    gkg::BundleSulciLabelling::Bundle::const_iterator
      f = bundle.begin(),
      fe = bundle.end();
    while ( f != fe )
    {  

      // resampling the fiber wrt the resampling step
      const gkg::LightCurve3d< float >& fiber = *f;
      float fiberLength = fiber.getLength();
      int32_t resamplingPointCount = ( int32_t )( fiberLength / 
                                                        _resamplingStep + 0.5 );
      gkg::LightCurve3d< float >
          resampledFiber = f->getEquidistantCurve( resamplingPointCount );

      // computing the local direction at each point of the fiber 
      std::vector< gkg::Vector3d< float > >
        directionProfile = resampledFiber.getDirectionProfile();

      // looping over the fiber point(s)
      gkg::Vector3d< float > fiberExtremity1;
      gkg::Vector3d< float > fiberExtremity2;
      _transform3dFromBundleToSulciMask->getDirect( *resampledFiber.begin(),
                                                    fiberExtremity1 );
      _transform3dFromBundleToSulciMask->getDirect( *resampledFiber.rbegin(),
                                                    fiberExtremity2 );

      gkg::LightCurve3d< float >::const_iterator
        p = resampledFiber.begin(),
        pe = resampledFiber.end();
      std::vector< gkg::Vector3d< float > >::const_iterator
        d = directionProfile.begin();
      int32_t pointIndex = 0;
      while ( p != pe )
      {

        _transform3dFromBundleToSulciMask->getDirect(
                                                  *p,
                                                  fiberPointInSulciMaskFrame );
        voxelInSulciMaskFrame.x = ( int32_t )( fiberPointInSulciMaskFrame.x /
                                               _sulciMaskResolution.x );
        voxelInSulciMaskFrame.y = ( int32_t )( fiberPointInSulciMaskFrame.y /
                                               _sulciMaskResolution.y );
        voxelInSulciMaskFrame.z = ( int32_t )( fiberPointInSulciMaskFrame.z /
                                               _sulciMaskResolution.z );                        


        minimumDistance = 1e38;
        closestVertexIndex = -1;
        for ( nz = -_neighborhoodSize; nz <= _neighborhoodSize; nz++ )
        {

          for ( ny = -_neighborhoodSize; ny <= _neighborhoodSize; ny++ )
          {

            for ( nx = -_neighborhoodSize; nx <= _neighborhoodSize; nx++ )
            {

              voxelNeighborInSulciMaskFrame.x = voxelInSulciMaskFrame.x + nx;
              voxelNeighborInSulciMaskFrame.y = voxelInSulciMaskFrame.y + ny;
              voxelNeighborInSulciMaskFrame.z = voxelInSulciMaskFrame.z + nz;

              if ( _boundingBoxSulciMask.contains(
                                            voxelNeighborInSulciMaskFrame ) )
              {

                const std::list< int32_t >* vertexIndexList =
                           ( *_meshVertexLut )( voxelNeighborInSulciMaskFrame );
                if ( vertexIndexList )
                {

                  std::list< int32_t >::const_iterator
                    vi = vertexIndexList->begin(),
                    vie = vertexIndexList->end();
                  while ( vi != vie )
                  {

                    const gkg::Vector3d< float >& vertex = _vertices[ *vi ];

                    if ( ( ( fiberExtremity1 - vertex ).dot(
                                           fiberExtremity2 - vertex ) < 0.0 ) &&
                         ( ( vertex - fiberPointInSulciMaskFrame ).dot(
                                 fiberExtremity1 -
                                 fiberPointInSulciMaskFrame ) > 0.0 ) &&
                         ( ( vertex - fiberPointInSulciMaskFrame ).dot(
                                 fiberExtremity2 -
                                 fiberPointInSulciMaskFrame ) > 0.0 ) )
                    {

                      distance = ( vertex -
                                   fiberPointInSulciMaskFrame ).getNorm();
                      if ( distance < minimumDistance )
                      {

                        minimumDistance = distance;
                        closestVertexIndex = *vi;
            
                      }

                    }

                    ++ vi;
                
                  }

                }

              }

            }
      
          }
      
        }
        if (  closestVertexIndex != -1 )
        {

          gkg::Vector3d< float >
            currentPointAlongNormal = _vertices[ closestVertexIndex ];
          const gkg::Vector3d< float >& normal = _normals[ closestVertexIndex ];

          gkg::Vector3d< int32_t > currentVoxelInSulciMaskFrame;
          float currentDisplacement = 0.0f;

          while ( ( currentDisplacement < _maximumDisplacement ) &&
                  ( _boundingBoxPialMesh.contains( currentPointAlongNormal ) ) )
          {

            currentDisplacement += _displacementStep;
            currentPointAlongNormal += normal * _displacementStep;
            currentVoxelInSulciMaskFrame.x =
              ( int32_t )( currentPointAlongNormal.x / _sulciMaskResolution.x );
            currentVoxelInSulciMaskFrame.y =
              ( int32_t )( currentPointAlongNormal.y / _sulciMaskResolution.y );
            currentVoxelInSulciMaskFrame.z =
              ( int32_t )( currentPointAlongNormal.z / _sulciMaskResolution.z );

            if ( _boundingBoxSulciMask.contains(
                                                currentVoxelInSulciMaskFrame ) )
            {

              int16_t sulciLabel = 
                               ( *_sulciMask )( currentVoxelInSulciMaskFrame );

              int32_t sulciIndex = ( int32_t )sulciLabel - 1;
              if ( sulciLabel != 0 )
              {

                const gkg::SulciInformation& sulciInformation =
                                          ( *_sulciInformations )[ sulciIndex ];

                // getting the closest sulci part
                float minimumDistance = 1e38;
                std::list< gkg::SulciPart >::const_iterator bestSulciPart;
                std::list< gkg::SulciPart >::const_iterator
                  sp = sulciInformation.sulciParts.begin(),
                  spe = sulciInformation.sulciParts.end();
                while ( sp != spe )
                {

                  float distance = ( sp->gravityCenter -
                                     currentPointAlongNormal ).getNorm();
                  if ( distance < minimumDistance )
                  {

                    bestSulciPart = sp;
                    minimumDistance = distance;

                  }
                  ++ sp;

                }


                if ( bestSulciPart == sulciInformation.sulciParts.end() )
                {

                  std::cout << "ya un pb" << std::endl;

                }

                // computing the angle between the bundle direction and the
                // sulci direction
                angleBetweenFiberAndSulciInDegrees = gkg::getLineAngles(
                  *d,
                  bestSulciPart->direction ) *
                  180.0f / M_PI;


                // adding contribution of the sulci that takes into account
                // the angle it forms with the bundle and the distance of the 
                // current point to the bundle extremities
                matchedSulciHistogram[ ( int32_t )sulciLabel ] +=
                  std::min( getTukeyWindowFunction(
                                  pointIndex,
                                  _extremityTukeyApodizationAlphaWeight,
                                  _extremityTukeyApodizationPower,
                                  resamplingPointCount ),
                            getTukeyWindowFunction(
                                  ( int32_t )angleBetweenFiberAndSulciInDegrees,
                                  _orthogonalityTukeyApodizationAlphaWeight,
                                  _orthogonalityTukeyApodizationPower,
                                  180 ) );
                break; 

              }

            }

          }

        }
        ++ p;
        ++ pointIndex;
        ++ d;

      }
   
      ++ f;

    }

    return matchedSulciHistogram;

  }
  GKG_CATCH( "std::vector< float >"
             "gkg::BundleSulciLabelling::get( "   
             "const "
             "gkg::BundleSulciLabelling::Bundle& "
             "bundle ) const" );

}


template < class L, class Compare >
void gkg::BundleSulciLabelling::spreadSheetFunctor(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )  
                      
{

  try
  {

    // stringParameters[ 0 ] : input mesh filename
    // stringParameters[ 1 ] : input sulci graph filename
    // stringParameters[ 2 ] : input pial mesh to sulci mask map 3D transform
    //                         filename
    // stringParameters[ 3 ] : input bundle map to sulci mask 3D transform
    //                         filename
    // stringParameters[ 4 ] : directory where to store intermediate results
    // stringParameters[ 5 ] : unwanted sulci name list filename 

    // scalarParameters[ 0 ] : resampling step 
    // scalarParameters[ 1 ] : maximum displacement
    // scalarParameters[ 2 ] : displacement step
    // scalarParameters[ 3 ] : neighborhood size
    // scalarParameters[ 4 ] : extremity Tukey apodization alpha weight
    // scalarParameters[ 5 ] : orthogonality Tukey apodization alpha weight
    // scalarParameters[ 6 ] : extremity Tukey apodization power
    // scalarParameters[ 7 ] : orthogonality Tukey apodization power
    // scalarParameters[ 8 ] : verbose


    ////////////////////////////////////////////////////////////////////////////
    // collecting string information
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameInputMesh = stringParameters[ 0 ];
    std::string fileNameInputSulciGraph = stringParameters[ 1 ];
    std::string fileNameInputPialMeshToSulciMaskTransform3d =
                                                          stringParameters[ 2 ];
    std::string fileNameInputBundleToSulciMaskTransform3d =
                                                          stringParameters[ 3 ];
    std::string directoryNameOutputIntermediateResults = stringParameters[ 4 ];
    std::string fileNameInputUnwantedSulci = stringParameters[ 5 ];


    ////////////////////////////////////////////////////////////////////////////
    // collecting scalar information
    ////////////////////////////////////////////////////////////////////////////

    float resamplingStep = ( float )scalarParameters[ 0 ];
    float maximumDisplacement = ( float )scalarParameters[ 1 ];
    float displacementStep = ( float )scalarParameters[ 2 ];
    int32_t neighborhoodSize = ( int32_t )( scalarParameters[ 3 ] + 0.5 );
    double extremityTukeyApodizationAlphaWeight =
                                                 ( float )scalarParameters[ 4 ];
    double orthogonalityTukeyApodizationAlphaWeight =
                                                 ( float )scalarParameters[ 5 ];
    double extremityTukeyApodizationPower = ( float )scalarParameters[ 6 ];
    double orthogonalityTukeyApodizationPower = ( float )scalarParameters[ 7 ];
    bool verbose = ( ( ( int32_t )( scalarParameters[ 8 ] + 0.5 ) ) ?
                      true : false );


    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameInputMesh
                << "' pial mesh : "
                << std::flush;

    }
    gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
      pialMesh( new gkg::MeshMap< int32_t, float, 3U > );
    gkg::Reader::getInstance().read( fileNameInputMesh, *pialMesh );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading input sulci graph
    ////////////////////////////////////////////////////////////////////////////

    std::string graphNameWithoutExtension = fileNameInputSulciGraph;
    if ( stringParameters[ 1 ].length() > 4U )
    {

      if ( stringParameters[ 1 ].substr( fileNameInputSulciGraph.length() - 4U,
                                         4U ) == ".arg" )
      {

        graphNameWithoutExtension = fileNameInputSulciGraph.substr(
                                        0U,
                                        fileNameInputSulciGraph.length() - 4U );

      }

    }
    std::string graphArgFileName = graphNameWithoutExtension + ".arg";
    std::string graphDataDirectoryName = graphNameWithoutExtension + ".data";

    if ( verbose )
    {

      std::cout << "reading '" << graphArgFileName
                << "' sulci graph : "
                << std::flush;

    }

    gkg::RCPointer< std::vector< gkg::SulciInformation > >
      sulciInformations( new std::vector< gkg::SulciInformation > );
    gkg::RCPointer< gkg::Volume< int16_t > >
      sulciMask( new gkg::Volume< int16_t > );
    std::vector< std::string > keptSulciNames;
    std::set< std::string > unwantedSulciNames;
    readSulciGraphInformation( graphArgFileName,
                               graphDataDirectoryName,
                               stringParameters[ 4 ],
                               stringParameters[ 5 ],
                               *sulciInformations,
                               *sulciMask,
                               keptSulciNames,
                               unwantedSulciNames,
                               verbose  );
    int32_t keptSulciCount = ( int32_t )sulciInformations->size();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading transform from mesh to sulci mask
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Transform3d< float > > 
     transform3dFromPialMeshToSulciMask;

    if ( fileNameInputPialMeshToSulciMaskTransform3d == "id" )
    {

      if ( verbose )
      {

        std::cout << "setting input mesh to sulci 3D transform to identity"
                  << std::endl;

      }
      transform3dFromPialMeshToSulciMask.reset(
                                        new gkg::IdentityTransform3d< float > );

    }
    else
    {

      if ( verbose )
      {

        std::cout << "reading input mesh to sulci 3D transform : "
                  << std::flush;

      }

      std::ifstream is( fileNameInputPialMeshToSulciMaskTransform3d.c_str() );
      if ( is.fail() )
      {

        throw std::runtime_error( "error while opening " +
                                  stringParameters[ 2 ] );

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        rigidTransform3d->readTrm( is );
        transform3dFromPialMeshToSulciMask.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          affineTransform3d->readTrm( is );
          transform3dFromPialMeshToSulciMask.reset( affineTransform3d );

        }
        catch ( std::exception& )
        {

          throw std::runtime_error(
            "unable to read mesh to sulci 3D transformation" );

        }

      }

      is.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    

    ////////////////////////////////////////////////////////////////////////////
    // reading bundle to sulci mask transform 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::RCPointer< gkg::Transform3d< float > > 
     transform3dFromBundleToSulciMask;

    if ( fileNameInputBundleToSulciMaskTransform3d == "id" )
    {

      if ( verbose )
      {

        std::cout << "setting bundle map to sulci 3D transform to identity"
                  << std::endl;

      }
      transform3dFromBundleToSulciMask.reset(
                                        new gkg::IdentityTransform3d< float > );

    }
    else
    {

      if ( verbose )
      {

        std::cout << "reading bundle map to sulci 3D transform : "
                  << std::flush;

      }

      std::ifstream is( fileNameInputBundleToSulciMaskTransform3d.c_str() );
      if ( is.fail() )
      {

        throw std::runtime_error( "error while opening " +
                                  stringParameters[ 3 ] );

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        rigidTransform3d->readTrm( is );
        transform3dFromBundleToSulciMask.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          affineTransform3d->readTrm( is );
          transform3dFromBundleToSulciMask.reset( affineTransform3d );

        }
        catch ( std::exception& )
        {

          throw std::runtime_error(
            "unable to read bundles to sulci 3D transformation" );

        }

      }

      is.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }



    ////////////////////////////////////////////////////////////////////////////
    // looping over bundle(s)
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleSulciLabelling
      bundleSulciLabelling( pialMesh,
                            sulciInformations,
                            sulciMask,
                            transform3dFromPialMeshToSulciMask,
                            transform3dFromBundleToSulciMask,
                            resamplingStep,
                            maximumDisplacement,
                            displacementStep,
                            neighborhoodSize,
                            extremityTukeyApodizationAlphaWeight,
                            orthogonalityTukeyApodizationAlphaWeight,
                            extremityTukeyApodizationPower,
                            orthogonalityTukeyApodizationPower );

    int32_t keptBundleCount = bundleMap->getBundleCount();


    std::vector< std::string > keptBundleNames;
    keptBundleNames.reserve( keptBundleCount );

    // needs to add 1, because labels starts from 1 instead of 0!
    gkg::Matrix bundleToSulciAffinities( keptBundleCount + 1,
                                         keptSulciCount + 1 );
    bundleToSulciAffinities.fill( 0.0 );


    std::map< std::string, L > nameToLabelLut;

    int32_t bundleIndex = 1;
    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      nameToLabelLut[ gkg::StringConverter::toString( label ) ] = label;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      keptBundleNames.push_back( gkg::StringConverter::toString( label ) );

      if ( verbose )
      {

        std::cout << gkg::StringConverter::toString( label ) << " "
                  << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // collecting measure
      //////////////////////////////////////////////////////////////////////////
    
      std::vector< float >
        matchedSulciHistogram = bundleSulciLabelling.get( bundle );
      
      int32_t sulciLabelCount = ( int32_t )matchedSulciHistogram.size();
      gkg::Matrix measure( 1, sulciLabelCount );
      int32_t p = 0;
      for ( p = 0; p < sulciLabelCount; p++ )
      {

        measure( 0, p ) = ( double )matchedSulciHistogram[ p ];

      }

      measures[ label ][ "bundle_sulci_labelling" ] = measure;


      //////////////////////////////////////////////////////////////////////////
      // filling the affinity matrix for the current bundle
      //////////////////////////////////////////////////////////////////////////
    
      bundleToSulciAffinities.fill( bundleIndex, 0, measure );


      ++ bundleIndex;
      ++ l; 

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing bundle index table
    ////////////////////////////////////////////////////////////////////////////

    std::ofstream os( ( directoryNameOutputIntermediateResults + 
                        gkg::getDirectorySeparator() +
                        "bundle_table.txt" ).c_str() );


    for ( bundleIndex = 0; bundleIndex < keptBundleCount; bundleIndex++ )
    {

      os << keptBundleNames[ bundleIndex ] << "  " 
         << bundleIndex + 1 << std::endl;

    }

    os.close();


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // writing results from the bundle point of view
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing results from the bundle point of view : "
                << std::flush;

    }
    
    std::string directoryNameBundleToSulci =
                                        directoryNameOutputIntermediateResults +
                                        gkg::getDirectorySeparator() +
                                        "bundle-to-sulci";
    gkg::Directory directoryBundleToSulci( directoryNameBundleToSulci );
    directoryBundleToSulci.mkdir();


    ////////////////////////////////////////////////////////////////////////////
    // writing results for all bundle(s) in a json file
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameJsonBundleToSulciAffinities =
                                        directoryNameBundleToSulci +
                                        gkg::getDirectorySeparator() +
                                        "bundle-to-sulci-affinities.json";
    os.open( fileNameJsonBundleToSulciAffinities.c_str() );


    os << "bundleToSulciAffinities = {" << std::endl;

    int32_t sulciIndex = 1;
    for ( bundleIndex = 1; bundleIndex < keptBundleCount + 1; bundleIndex++ )
    {

      std::list< std::string > sulciNames;
      std::list< double > affinities;

      double affinity = 0.0;
      for ( sulciIndex = 1; sulciIndex < keptSulciCount + 1; sulciIndex++ )
      {

        affinity = bundleToSulciAffinities( bundleIndex, sulciIndex );
        if ( affinity > 0.0 )
        {

          sulciNames.push_back( keptSulciNames[ sulciIndex - 1 ] );
          affinities.push_back( affinity );

        }

      }

      os << "  \"" << keptBundleNames[ bundleIndex - 1 ]
         << "\" : {" << std::endl;

      os << "    \"sulci_count\" : " << sulciNames.size() << "," << std::endl;
      os << "    \"sulci_names\" : [ ";

      std::list< std::string >::const_iterator
        s = sulciNames.begin(),
        se = sulciNames.end(),
        seMinusOne = sulciNames.end();
      seMinusOne --;
      while ( s != se )
      {

        if ( s == seMinusOne )
        {

          os << "\"" << *s;

        }
        else
        {

          os << "\"" << *s << "\",";

        }
        ++ s;

      }

      os << " ]," << std::endl;

      os << "    \"sulci_affinities\" : [ ";

      std::list< double >::const_iterator
        a = affinities.begin(),
        ae = affinities.end(),
        aeMinusOne = affinities.end();
      aeMinusOne --;
      while ( a != ae )
      {

        if ( a == aeMinusOne )
        {

          os << "\"" << *a;

        }
        else
        {

          os << "\"" << *a << "\",";

        }
        ++ a;

      }

      os << " ]" << std::endl;

      if ( bundleIndex == keptBundleCount )
      {

        os << "  }" << std::endl;
 
      }
      else
      {

        os << "  }," << std::endl;
 
      }

      ++ bundleIndex;
      ++ l; 

    }

    os << "}" << std::endl;
    os.close();


    ////////////////////////////////////////////////////////////////////////////
    // writing results for each bundle
    ////////////////////////////////////////////////////////////////////////////

    gkg::MaximumFilter< gkg::Vector, float > maximumFilter;
    int32_t r = 0;
    int32_t g = 0;
    int32_t b = 0;
    float ratio = 0.0f;

    bundleIndex = 1;
    l = bundleMap->begin();
    while ( l != le )
    {

      //////////////////////////////////////////////////////////////////////////
      // collecting label and bundle
      //////////////////////////////////////////////////////////////////////////
    
      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;


      //////////////////////////////////////////////////////////////////////////
      // creating the sub-diorectory for the current bundle
      //////////////////////////////////////////////////////////////////////////
    
      std::string directoryNameBundle = directoryNameBundleToSulci + 
                                        gkg::getDirectorySeparator() +
                                        gkg::StringConverter::toString( label );
      gkg::Directory directoryBundle( directoryNameBundle );
      directoryBundle.mkdir();


      //////////////////////////////////////////////////////////////////////////
      // writing bundle
      //////////////////////////////////////////////////////////////////////////
    
      std::string fileNameBundle = directoryNameBundle + 
                                   gkg::getDirectorySeparator() +
                                   gkg::StringConverter::toString( label ) +
                                   ".bundles";

      gkg::BundleMap< L, Compare > individualBundleMap;
      individualBundleMap.addBundle( label, bundle );

      gkg::Writer::getInstance().write( fileNameBundle,
                                        individualBundleMap );


      //////////////////////////////////////////////////////////////////////////
      // writing sulci hierarchy
      //////////////////////////////////////////////////////////////////////////
    
      std::string fileNameSulciHierarchy = directoryNameBundle + 
                                           gkg::getDirectorySeparator() +
                                           gkg::StringConverter::toString(
                                                                       label ) +
                                           "_sulci.hie";

      float maximumAffinity = 0.0f;
      maximumFilter.filter( bundleToSulciAffinities.getRow( bundleIndex ),
                            maximumAffinity );

      std::ofstream os( fileNameSulciHierarchy.c_str() );

      os << "# tree 1.0" << std::endl;
      os << std::endl;

      os << "*BEGIN TREE hierarchy" << std::endl;
      os << "graph_syntax CorticalFoldArg" << std::endl;
      os << std::endl;

      for ( sulciIndex = 1; sulciIndex < keptSulciCount + 1; sulciIndex++ )
      {

        ratio = ( float )bundleToSulciAffinities( bundleIndex, sulciIndex ) /
                ( maximumAffinity ? maximumAffinity : 1.0f );
        r = 127 + ( int32_t )( 127.0 * ratio );
        g = 127;
        b = 127 + ( int32_t )( 127.0 * ( 1.0f - ratio ) );

        os << "*BEGIN TREE fold_name" << std::endl;
        os << "name  " << keptSulciNames[ sulciIndex - 1 ]
           << std::endl;
        os << "color " << r << " " << g << " " << b << std::endl;
        os << "*END" << std::endl;
        os << std::endl;

      }
      std::set< std::string >::const_iterator
        us = unwantedSulciNames.begin(),
        use = unwantedSulciNames.end();
      while ( us != use )
      {

        os << "*BEGIN TREE fold_name" << std::endl;
        os << "name  " << *us << std::endl;
        os << "color 255 255 255 0" << std::endl;
        os << "*END" << std::endl;
        os << std::endl;
        ++ us;

      }
      os << "*END" << std::endl;


      os.close();

      ++ bundleIndex;
      ++ l; 

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // writing results from the sulci point of view
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing results from the sulci point of view : "
                << std::flush;

    }
    
    std::string directoryNameSulciToBundle =
                                        directoryNameOutputIntermediateResults +
                                        gkg::getDirectorySeparator() +
                                        "sulci-to-bundle";
    gkg::Directory directorySulciToBundle( directoryNameSulciToBundle );
    directorySulciToBundle.mkdir();


    ////////////////////////////////////////////////////////////////////////////
    // writing results for all sulci in a json file
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameJsonSulciToBundleAffinities =
                                        directoryNameSulciToBundle +
                                        gkg::getDirectorySeparator() +
                                        "sulci-to-bundle-affinities.json";
    os.open( fileNameJsonSulciToBundleAffinities.c_str() );


    os << "sulciToBundleAffinities = {" << std::endl;


    for ( sulciIndex = 1; sulciIndex < keptSulciCount + 1; sulciIndex++ )
    {

      std::list< std::string > bundleNames;
      std::list< double > affinities;

      double affinity = 0.0;
      for ( bundleIndex = 1; bundleIndex < keptBundleCount + 1; bundleIndex++ )
      {

        affinity = bundleToSulciAffinities( bundleIndex, sulciIndex );
        if ( affinity > 0.0 )
        {

          bundleNames.push_back( keptBundleNames[ bundleIndex - 1 ] );
          affinities.push_back( affinity );

        }

      }


      // creating sulcus directory
      std::string directoryNameSulcus = directoryNameSulciToBundle +
                                        gkg::getDirectorySeparator() +
                                        keptSulciNames[ sulciIndex - 1 ];
      gkg::Directory directorySulcus( directoryNameSulcus );
      directorySulcus.mkdir();


      // writing sulci hierarchy 
      std::string fileNameBundleForSulciHierarchy = directoryNameSulcus + 
                                             gkg::getDirectorySeparator() +
                                             keptSulciNames[ sulciIndex - 1 ] +
                                             ".hie";
      std::ofstream os2( fileNameBundleForSulciHierarchy.c_str() );

      os2 << "# tree 1.0" << std::endl;
      os2 << std::endl;

      os2 << "*BEGIN TREE hierarchy" << std::endl;
      os2 << "graph_syntax CorticalFoldArg" << std::endl;
      os2 << std::endl;

      int32_t sulciIndexBis = 1;
      for ( sulciIndexBis = 1; sulciIndexBis < keptSulciCount + 1;
            sulciIndexBis++ )
      {

        if ( sulciIndexBis == sulciIndex )
        {

          r = 254;
          g = 127;
          b = 127;

        }
        else
        {

          r = 127;
          g = 127;
          b = 254;

        }
        os2 << "*BEGIN TREE fold_name" << std::endl;
        os2 << "name  " << keptSulciNames[ sulciIndexBis - 1 ]
            << std::endl;
        os2 << "color " << r << " " << g << " " << b << std::endl;
        os2 << "*END" << std::endl;
        os2 << std::endl;

      }
      std::set< std::string >::const_iterator
        us = unwantedSulciNames.begin(),
        use = unwantedSulciNames.end();
      while ( us != use )
      {

        os2 << "*BEGIN TREE fold_name" << std::endl;
        os2 << "name  " << *us << std::endl;
        os2 << "color 255 255 255 0" << std::endl;
        os2 << "*END" << std::endl;
        os2 << std::endl;
        ++ us;

      }
      os2 << "*END" << std::endl;


      os2.close();



      if ( !bundleNames.empty() )
      {

        // writing the bundle map of bundles with affinity with the current
        // sulcus
        std::string fileNameBundleForSulci = directoryNameSulcus + 
                                             gkg::getDirectorySeparator() +
                                             keptSulciNames[ sulciIndex - 1 ] +
                                             ".bundles";

        gkg::BundleMap< L, Compare > bundleMapForSulci;

        std::list< std::string >::const_iterator
          b = bundleNames.begin(),
          be = bundleNames.end();
        while ( b != be )
        {

          const L& label = nameToLabelLut[ *b ];
          bundleMapForSulci.addBundle( label,
                                       bundleMap->getBundle( label ) );

          ++ b;

        }
        gkg::Writer::getInstance().write( fileNameBundleForSulci,
                                          bundleMapForSulci );

      }

      os << "  \"" << keptSulciNames[ sulciIndex - 1 ] << "\" : {" << std::endl;

      os << "    \"bundle_count\" : " << bundleNames.size() << "," << std::endl;
      os << "    \"bundle_names\" : [ ";

      std::list< std::string >::const_iterator
        b = bundleNames.begin(),
        be = bundleNames.end(),
        beMinusOne = bundleNames.end();
      beMinusOne --;
      while ( b != be )
      {

        if ( b == beMinusOne )
        {

          os << "\"" << *b;

        }
        else
        {

          os << "\"" << *b << "\",";

        }

        ++ b;

      }

      os << " ]," << std::endl;

      os << "    \"bundle_affinities\" : [ ";

      std::list< double >::const_iterator
        a = affinities.begin(),
        ae = affinities.end(),
        aeMinusOne = affinities.end();
      aeMinusOne --;
      while ( a != ae )
      {

        if ( a == aeMinusOne )
        {

          os << "\"" << *a;

        }
        else
        {

          os << "\"" << *a << "\",";

        }

        ++ a;

      }

      os << " ]" << std::endl;

      os << "  }," << std::endl;

    }

    os << "}" << std::endl;
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "template < class L, class Compare > > "
             "void gkg::BundleSulciLabelling::"
             "spreadSheetFunctor( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );


}


void gkg::BundleSulciLabelling::readSulciGraphInformation(
                        const std::string& graphArgFileName,
                        const std::string& graphDataDirectoryName,
                        const std::string& intermediateDirectoryName,
                        const std::string& unwantedSulciFileName,
                        std::vector< gkg::SulciInformation >& sulciInformations,
                        gkg::Volume< int16_t >& sulciMask,
                        std::vector< std::string >& keptSulciNames,
                        std::set< std::string >& unwantedSulciNames,
                        bool verbose )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // reading list of unwanted sulci
    ////////////////////////////////////////////////////////////////////////////

    std::ifstream is;
    if ( unwantedSulciFileName != "none" )
    {

      std::ifstream is( unwantedSulciFileName.c_str() );
      if ( !is )
      {

        throw std::runtime_error(
                         std::string( "could not open unwanted sulci list '" ) +
                         unwantedSulciFileName + "'" );

      }

      std::string unwantedSulciName;
      while ( !is.eof() )
      {

        is >> unwantedSulciName;
        unwantedSulciNames.insert( unwantedSulciName );

        std::cout << "++++++++++++++++> unwantedSulciName="
                  << unwantedSulciName << std::endl;

      }

      is.close();

    }


    ////////////////////////////////////////////////////////////////////////////
    // opening the sulci graph
    ////////////////////////////////////////////////////////////////////////////

    is.open( graphArgFileName.c_str() );
    if ( !is )
    {

      throw std::runtime_error( std::string( "could not open sulci graph '" ) +
                                graphArgFileName + "'" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // parsing graph arg file
    ////////////////////////////////////////////////////////////////////////////

    seekKey( is, "CorticalFoldArg" );

    // collecting the sulci mask size
    seekKey( is, "boundingbox_max" );
    gkg::Vector3d< int32_t > size;
    is >> size.x >> size.y >> size.z;

    // collecting the sulci mask resolution
    seekKey( is, "voxel_size" );
    gkg::Vector3d< double > resolution;
    is >> resolution.x >> resolution.y >> resolution.z;

    // creating structures to store the various int16_t labels corresponding
    // to a sulci name as well as their center of mass and main direction
    std::map< std::string, gkg::SulciInformation > sulciInformationMap;

    std::string sulciName;
    int16_t bottomLabel = 0;
    int16_t otherLabel = 0;
    int16_t ssLabel = 0;
    float bBoxLowerX = 0.0f;
    float bBoxLowerY = 0.0f;
    float bBoxLowerZ = 0.0f;
    float bBoxUpperX = 0.0f;
    float bBoxUpperY = 0.0f;
    float bBoxUpperZ = 0.0f;
    gkg::Vector3d< float > gravityCenter;
    gkg::Vector3d< float > direction;
    int16_t maximumLabel = 0;
    while ( !is.eof() )
    {

      // collecting sulci name
      seekKey( is, "NODE" );

      if ( is )
      {

        if ( seekKey( is, "name" ) )
        {

          // collecting sulci name
          is >> sulciName;

          if ( seekKey( is, "bottom_label" ) )
          {

            // collecting sulci part label
            is >> bottomLabel;

            if ( seekKey( is, "boundingbox_max" ) )
            {

              // collecting upper corner of the bounding box
              is >> bBoxUpperX >> bBoxUpperY >> bBoxUpperZ;
              bBoxUpperX *= resolution.x;
              bBoxUpperY *= resolution.y;
              bBoxUpperZ *= resolution.z;


              if ( seekKey( is, "boundingbox_min" ) )
              {

                // collecting lower corner of the bounding box
                is >> bBoxLowerX >> bBoxLowerY >> bBoxLowerZ;
                bBoxLowerX *= resolution.x;
                bBoxLowerY *= resolution.y;
                bBoxLowerZ *= resolution.z;

                if ( seekKey( is, "direction" ) )
                {

                  // collecting sulci part main direction
                  is >> direction.x >> direction.y >> direction.z;

                  if ( seekKey( is, "gravity_center" ) )
                  {

                    // collecting sulci part gravity center
                    is >> gravityCenter.x >> gravityCenter.y >> gravityCenter.z;
                    gravityCenter.x *= resolution.x;
                    gravityCenter.y *= resolution.y;
                    gravityCenter.z *= resolution.z;

                    if ( seekKey( is, "other_label" ) )
                    {

                      // collecting sulci part other label
                      is >> otherLabel;

                      if ( seekKey( is, "ss_label" ) )
                      {

                        // collecting sulci part ss label
                        is >> ssLabel;

                        if ( ssLabel > maximumLabel )
                        {

                          maximumLabel = ssLabel;

                        }

                        //std::cout << sulciName << " : " << ssLabel << " "
                        //          << gravityCenter << "  "
                        //          << direction
                        //        << std::endl;
 
                        if ( unwantedSulciNames.find( sulciName ) ==
                             unwantedSulciNames.end() )
                        {

                          sulciInformationMap[ sulciName ].name = sulciName;
                          sulciInformationMap[ sulciName ].sulciParts.push_back(
                            gkg::SulciPart( ssLabel,
                                        gkg::BoundingBox< float >( bBoxLowerX,
                                                                   bBoxUpperX,
                                                                   bBoxLowerY,
                                                                   bBoxUpperY,
                                                                   bBoxLowerZ,
                                                                   bBoxUpperZ ),
                                        gravityCenter,
                                        direction ) );

                        }

                      }

                    }

                  }

                }

              }

            }

          }

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // closing the sulci graph
    ////////////////////////////////////////////////////////////////////////////

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // putting the sulci information map to a vector
    ////////////////////////////////////////////////////////////////////////////

    sulciInformations.resize( sulciInformationMap.size() );
    std::map< int16_t, int16_t > sulciPartLabelToSulciLabel;
    keptSulciNames.reserve( sulciInformationMap.size() );

    if ( verbose )
    {

      std::cout << std::endl;

    }

    std::map< std::string, gkg::SulciInformation >::const_iterator
      sim = sulciInformationMap.begin(),
      sime = sulciInformationMap.end();
    std::vector< gkg::SulciInformation >::iterator
      si = sulciInformations.begin();
    int16_t sulciLabel = 1;
    while ( sim != sime )
    {

      *si = sim->second;
      si->label = sulciLabel;
      keptSulciNames.push_back( si->name );

      if ( verbose )
      {

        std::cout << "sulci #" << si->label << " '" << si->name << "':"
                  << std::endl;

      }

      std::list< gkg::SulciPart >::const_iterator
        sp = si->sulciParts.begin(),
        spe = si->sulciParts.end();
      while ( sp != spe )
      {

        if ( verbose )
        {

          std::cout << "  label: " << sp->label
                    << "  gravity_center: " << sp->gravityCenter
                    << "  direction: " << sp->direction
                    << std::endl;

        }

        sulciPartLabelToSulciLabel[ sp->label ] = sulciLabel;

        ++ sp;

      }

      ++ sulciLabel;
      ++ si;
      ++ sim;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving a bundle map of sulci part direction vectors
    ////////////////////////////////////////////////////////////////////////////

    gkg::BundleMap< std::string > sulciPartDirectionVectors;

    gkg::LightCurve3d< float > directionVector( 2 );

    std::ofstream os( intermediateDirectoryName + 
                      gkg::getDirectorySeparator() +
                      "sulci_part_gravity_centers.pointcloud" );

    gkg::Vector3d< float > lowerExtremity;
    gkg::Vector3d< float > upperExtremity;
    si = sulciInformations.begin();
    std::vector< gkg::SulciInformation >::const_iterator
      sie = sulciInformations.end();
    while ( si != sie )
    {

      std::list< gkg::LightCurve3d< float > > directionVectors;

      std::list< gkg::SulciPart >::const_iterator
        sp = si->sulciParts.begin(),
        spe = si->sulciParts.end();
      while ( sp != spe )
      {


        rayToBoundingBoxIntersection( sp->boundingBox,
                                      sp->gravityCenter,
                                      sp->direction,
                                      lowerExtremity,
                                      upperExtremity );
        directionVector.setPoint( 0, lowerExtremity );
        directionVector.setPoint( 1, upperExtremity );

        //directionVector.setPoint( 0, sp->gravityCenter - sp->direction * 5.0f );
        //directionVector.setPoint( 1, sp->gravityCenter + sp->direction * 5.0f );

        directionVectors.push_back( directionVector );

        os << sp->gravityCenter.x << " "
           << sp->gravityCenter.y << " "
           << sp->gravityCenter.z << std::endl;
           

        ++ sp;

      }

      sulciPartDirectionVectors.addBundle( si->name, directionVectors );

      ++ si;

    }

    os.close();

    gkg::Writer::getInstance().write( intermediateDirectoryName + 
                                      gkg::getDirectorySeparator() +
                                      "sulci_part_direction_vectors.bundles",
                                      sulciPartDirectionVectors );


    ////////////////////////////////////////////////////////////////////////////
    // creating sulci mask
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameSSBucket = graphDataDirectoryName + 
                                   gkg::getDirectorySeparator() +
                                   "ss_Bucket.bck";
    gkg::SiteMap< int32_t, int32_t > ssSiteMap;
    gkg::Reader::getInstance().read( fileNameSSBucket,
                                     ssSiteMap );

    std::string fileNameBottomBucket = graphDataDirectoryName + 
                                       gkg::getDirectorySeparator() +
                                       "bottom_Bucket.bck";
    gkg::SiteMap< int32_t, int32_t > bottomSiteMap;
    gkg::Reader::getInstance().read( fileNameBottomBucket,
                                     bottomSiteMap );

    std::string fileNameOtherBucket = graphDataDirectoryName + 
                                      gkg::getDirectorySeparator() +
                                      "other_Bucket.bck";
    gkg::SiteMap< int32_t, int32_t > otherSiteMap;
    gkg::Reader::getInstance().read( fileNameOtherBucket,
                                     otherSiteMap );



    sulciMask.reallocate( size );
    sulciMask.setResolution( resolution );
    sulciMask.fill( 0 );


    gkg::SiteMap< int32_t, int32_t >::const_iterator
      s = ssSiteMap.begin(),
      se = ssSiteMap.end();
    while ( s != se )
    {

      const int16_t& sulciPartLabel = ( int16_t )s->first;

      if ( sulciPartLabelToSulciLabel.find( sulciPartLabel ) !=
           sulciPartLabelToSulciLabel.end() )
      {

        const std::list< gkg::Vector3d< int32_t > >& voxels = s->second;

        std::list< gkg::Vector3d< int32_t > >::const_iterator
          v = voxels.begin(),
          ve = voxels.end();
        while ( v != ve )
        {

          sulciMask( *v ) = sulciPartLabelToSulciLabel[ sulciPartLabel ];
          ++ v;

        }

      }
      ++ s;

    }

    gkg::SiteMap< int32_t, int32_t >::const_iterator
      b = bottomSiteMap.begin(),
      be = bottomSiteMap.end();
    while ( b != be )
    {

      const int16_t& sulciPartLabel = ( int16_t )b->first;

      if ( sulciPartLabelToSulciLabel.find( sulciPartLabel ) !=
           sulciPartLabelToSulciLabel.end() )
      {

        const std::list< gkg::Vector3d< int32_t > >& voxels = b->second;

        std::list< gkg::Vector3d< int32_t > >::const_iterator
          v = voxels.begin(),
          ve = voxels.end();
        while ( v != ve )
        {

          sulciMask( *v ) = sulciPartLabelToSulciLabel[ sulciPartLabel ];
          ++ v;

        }

      }
      ++ b;

    }

    gkg::SiteMap< int32_t, int32_t >::const_iterator
      o = otherSiteMap.begin(),
      oe = otherSiteMap.end();
    while ( o != oe )
    {

      const int16_t& sulciPartLabel = ( int16_t )o->first;

      if ( sulciPartLabelToSulciLabel.find( sulciPartLabel ) !=
           sulciPartLabelToSulciLabel.end() )
      {

        const std::list< gkg::Vector3d< int32_t > >& voxels = o->second;

        std::list< gkg::Vector3d< int32_t > >::const_iterator
          v = voxels.begin(),
          ve = voxels.end();
        while ( v != ve )
        {

          sulciMask( *v ) = sulciPartLabelToSulciLabel[ sulciPartLabel ];
          ++ v;

        }

      }
      ++ o;

    }

    gkg::Writer::getInstance().write( intermediateDirectoryName + 
                                      gkg::getDirectorySeparator() +
                                      "sulciMask.ima",
                                      sulciMask );


    ////////////////////////////////////////////////////////////////////////////
    // writing sulci index table
    ////////////////////////////////////////////////////////////////////////////

    os.open( ( intermediateDirectoryName + 
             gkg::getDirectorySeparator() +
             "sulci_table.txt" ).c_str() );


    si = sulciInformations.begin();
    while ( si != sie )
    {

      os << si->name << "  " << si->label << std::endl;
      ++ si;

    }

    os.close();


  }
  GKG_CATCH( "void gkg::BundleSulciLabelling::readSulciGraphInformation( "
             "const std::string& graphArgFileName, "
             "const std::string& graphDataDirectoryName, "
             "const std::string& intermediateDirectoryName, "
             "const std::string& unwantedSulciFileName, "
             "std::vector< SulciInformation >& sulciInformations, "
             "Volume< int16_t >& sulciMask, "
             "std::vector< std::string >& keptSulciNames, "
             "std::set< std::string >& unwantedSulciNames, "
             "bool verbose )" );

}


void gkg::BundleSulciLabelling::rayToBoundingBoxIntersection(
                                   const gkg::BoundingBox< float >& boundingBox,
                                   const gkg::Vector3d< float >& gravityCenter,
                                   const gkg::Vector3d< float >& direction,
                                   gkg::Vector3d< float >& lowerExtremity,
                                   gkg::Vector3d< float >& upperExtremity )
{

  try
  {

    float lowerT = ( boundingBox.getLowerX() - gravityCenter.x ) / direction.x;
    float upperT = ( boundingBox.getUpperX() - gravityCenter.x ) / direction.x;
 
    if ( lowerT > upperT )
    {

      std::swap( lowerT, upperT );

    } 
 

    float lowerYT = ( boundingBox.getLowerY() - gravityCenter.y ) / direction.y;
    float upperYT = ( boundingBox.getUpperY() - gravityCenter.y ) / direction.y;
 
    if ( lowerYT > upperYT )
    {

      std::swap( lowerYT, upperYT );

    } 
 
    if ( lowerYT > lowerT )
    {

      lowerT = lowerYT;

    }

    if ( upperYT < upperT )
    {

      upperT = upperYT;

    }

    float lowerZT = ( boundingBox.getLowerZ() - gravityCenter.z ) / direction.z;
    float upperZT = ( boundingBox.getUpperZ() - gravityCenter.z ) / direction.z;
 
    if ( lowerZT > upperZT )
    {

      std::swap( lowerZT, upperZT );

    } 
 
    if ( lowerZT > lowerT )
    {

      lowerT = lowerZT;

    }

    if ( upperZT < upperT )
    {

      upperT = upperZT;

    }


    lowerExtremity = gravityCenter + direction * lowerT;  
    upperExtremity = gravityCenter + direction * upperT;  

  }
  GKG_CATCH( "void gkg::BundleSulciLabelling::rayToBoundingBoxIntersection( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& gravityCenter, "
             "const gkg::Vector3d< float >& direction, "
             "gkg::Vector3d< float >& lowerExtremity, "
             "gkg::Vector3d< float >& upperExtremity )" );


}


bool gkg::BundleSulciLabelling::seekKey( std::ifstream& is, 
                                         const std::string& key )
{

  try
  {

    bool isOk = true;

    std::string tmp;
    do
    {

      is >> tmp;

    }
    while ( ( tmp != key ) && ( tmp != "*END" ) && !is.eof() );

    if ( tmp == "*END" )
    {

      isOk = false;

    }
    return isOk;

  }
  GKG_CATCH( "bool gkg::BundleSulciLabelling::seekKey( "
             "std::ifstream& is, "
             "const std::string& key )" );

}


float gkg::BundleSulciLabelling::getTukeyWindowFunction( int32_t index,
                                                         float alpha,
                                                         float power,
                                                         int32_t count )
{

  try
  {

    if ( index < 0 )
    {

      throw std::runtime_error( "index should be positive" );

    }

    if ( index >= count )
    {

      throw std::runtime_error( "index should be strictly lower than count" );

    }

    float threshold = alpha * ( float )count / 2.0;
    int32_t threshold1 = ( int32_t )threshold;
    int32_t threshold2 = count - 1 - threshold1;

    float value = 0.0f;

    if ( index < threshold1 )
    {

      value =  0.5f * ( 1.0f - std::cos( M_PI * ( float )index /
                                         threshold1 ) );

    }
    else if ( index > threshold2 )
    {

      value =  0.5f * ( 1.0f - std::cos( M_PI * ( float )( count - 1 - index ) /
                                         threshold1 ) );

    }
    else
    {

      value = 1.0f;

    }

    return ( float )std::pow( ( double )value, ( double )power );

  }
  GKG_CATCH( "float gkg::BundleSulciLabelling::getTukeyWindowFunction( "
             "int32_t index, "
             "float alpha, "
             "int32_t count )" );

}


template 
void gkg::BundleSulciLabelling::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                       std::map< std::string, gkg::Matrix >
                       >& measures );

template 
void gkg::BundleSulciLabelling::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                       std::map< std::string, gkg::Matrix >
                       >& measures );


////////////////////////////////////////////////////////////////////////////////
// adding check information to BundleMeasureChecker
////////////////////////////////////////////////////////////////////////////////

static bool initializeBundleSulciLabellingChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "bundle_sulci_labelling",

      ".  <P1>: input mesh filename\n"
      ".  <P2>: input sulci graph filename\n"
      ".  <P3>: input pial mesh->sulci mask 3D transform filename\n"
      ".  <P4>: input bundles->sulci mask 3D transform filename\n"
      ".  <P5>: directory where to store intermediate results\n"
      ".  <P6>: unwanted sulci name list filename (write \"none\" for default)",

      ".  <P1>: resampling step\n"
      ".  <P2>: maximum displacement\n"
      ".  <P3>: displacement step\n"
      ".  <P4>: neighborhood size\n"
      ".  <P5>: extremity Tukey apodization alpha weight within [0.0;1.0]\n"
      ".  <P6>: orthogonality Tukey apodization alpha weight within [0.0;1.0]\n"
      ".  <P7>: extremity Tukey apodization power > 0.0\n"
      ".  <P8>: orthogonality Tukey apodization power > 0.0\n"
      ".  <P9>: verbosity (1->true, 0-> false)",
      6,
      9,
      0 );

    return true;

  }
  GKG_CATCH( "static bool "
             "initializeBundleSulciLabellingChecker()" );

}


static bool initialized __attribute__((unused)) =
                                      initializeBundleSulciLabellingChecker();
