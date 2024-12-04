#include <gkg-dmri-odf-functor/OdfFunctorMDGFAScaledMaxNormalizedMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::
                                      OdfFunctorMDGFAScaledMaxNormalizedMesh()
                                         : gkg::OdfFunctor<
                                           gkg::MeshMap< int32_t, float, 3U > ,
                                           std::vector< float > >()
{
}


gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::
                                     ~OdfFunctorMDGFAScaledMaxNormalizedMesh()
{
}


std::string gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::getName() const
{

  return "mdgfa_scaled_max_normalized_mesh";

}


bool 
gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::process(
                             gkg::MeshMap< int32_t, float, 3U >& output,
                             gkg::OdfCartesianField& odfCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    gkg::Volume< float > normalizedGfa;
    gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getOdfFunctor( "normalized_gfa" ).process( normalizedGfa,
                                                     odfCartesianField,
                                                     scalarParameters,
                                                     stringParameters,
                                                     false );

    gkg::Volume< float > normalizedMeanDiffusivity;
    gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getOdfFunctor( "normalized_mean_diffusivity" ).process(
                                                     normalizedMeanDiffusivity,
                                                     odfCartesianField,
                                                     scalarParameters,
                                                     stringParameters,
                                                     false );

    // getting site count
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // preparing ouput mesh map
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

    // preparing translation
    gkg::Translation3d< float > translation;

    // processing convex hull of output orientation set
    const OrientationSet&
      outputOrientationSet = odfCartesianField.getOutputOrientationSet();
    MeshMap< int32_t, float, 3U > orientationMeshMap;
    ConvexHull::getInstance().addConvexHull( 
                                         outputOrientationSet.getOrientations(),
                                         0,
                                         orientationMeshMap );
    int32_t orientationVertexCount = 
      orientationMeshMap.vertices.getSiteCount( 0 );
    int32_t orientationPolygonCount = 
      orientationMeshMap.polygons.getPolygonCount( 0 );

    // processing textures
    std::vector< float > red( orientationVertexCount );
    std::vector< float > green( orientationVertexCount );
    std::vector< float > blue( orientationVertexCount );

    int32_t o;
    for ( o = 0; o < orientationVertexCount; o++ )
    {

       red[ o ] = std::fabs( outputOrientationSet.getOrientation( o ).x ) *
                  255.0;
       green[ o ] = std::fabs( outputOrientationSet.getOrientation( o ).y ) *
                    255.0;
       blue[ o ] = std::fabs( outputOrientationSet.getOrientation( o ).z ) *
                   255.0;
       if ( red[ o ] > 255.0 )
       {

         red[ o ] = 255.0;

       }
       if ( green[ o ] > 255.0 )
       {

         green[ o ] = 255.0;

       }
       if ( blue[ o ] > 255.0 )
       {

         blue[ o ] = 255.0;

       }

    }
    TextureMap< float > redTextureMap;
    TextureMap< float > greenTextureMap;
    TextureMap< float > blueTextureMap;
    std::vector< std::string > palettes( 3 );
    palettes[ 0 ] = "multitex-geom-red-mask";
    palettes[ 1 ] = "multitex-geom-green-mask";
    palettes[ 2 ] = "multitex-geom-blue-mask";
    redTextureMap.getHeader().addAttribute( "palette", palettes );

    // preparing mesh scaler and transformer
    MeshScaler< int32_t, float, 3U > meshScaler;
    MeshTransformer< int32_t, float, 3U > meshTransformer;

    // preparing mesh accumulator
    MeshAccumulator< int32_t, float, 3U > meshAccumulator;
    meshAccumulator.reserve( 0,
                             orientationVertexCount * siteCount,
                             orientationPolygonCount * siteCount );

    // allocating specific scalar parameter vector
    std::vector< double > specificScalarParameters( 7 );
    specificScalarParameters[ 0 ] = scalarParameters[ 0 ];
    specificScalarParameters[ 1 ] = scalarParameters[ 1 ];
    specificScalarParameters[ 2 ] = scalarParameters[ 2 ];
    specificScalarParameters[ 3 ] = scalarParameters[ 3 ];
    specificScalarParameters[ 4 ] = scalarParameters[ 4 ];
    specificScalarParameters[ 5 ] = 1.0;
    specificScalarParameters[ 6 ] = 1.0;

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::vector< float > scaling;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1 
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      specificScalarParameters[ 5 ] = normalizedGfa( *s - offset );
      specificScalarParameters[ 6 ] = normalizedMeanDiffusivity( *s - offset );
      this->processItem( scaling,
                         *odfCartesianField.getItem( *s ),
                         specificScalarParameters,
                         stringParameters );

      // processing 3D transform
      translation.setDirectTranslation( s->x * resolution.x,
                                        s->y * resolution.y,
                                        s->z * resolution.z );

      // processing local scaled mesh map
      MeshMap< int32_t, float, 3U > localMeshMap( orientationMeshMap );
      meshScaler.scale( localMeshMap, 0, scaling, localMeshMap );
      meshTransformer.transform( localMeshMap, translation,
                                 localMeshMap );

      // accumulating local mesh map
      meshAccumulator.add( localMeshMap );

      // adding red/green/blue texture(s)
      redTextureMap.addTextures( red );
      greenTextureMap.addTextures( green );
      blueTextureMap.addTextures( blue );

      ++ s;
      ++ count;

    }
    output.add( meshAccumulator );

    // in case output file name is stored in the string parameters at first
    // position, backup the texture file(s)
    if ( !stringParameters.empty() && !stringParameters[ 3 ].empty() )
    {
      std::string outputFileName = stringParameters[ 3 ];
      bool ascii = ( scalarParameters[ 4 ] > 0 ? true : false );
      std::string textureMapFormat = stringParameters[ 2 ];
      std::vector< std::string > textureNames( 3 );
      textureNames[ 0 ] = "red";
      textureNames[ 1 ] = "green";
      textureNames[ 2 ] = "blue";
      std::vector< std::string > textureFileNames( 3 );
      textureFileNames[ 0 ] = gkg::File( outputFileName ).getBaseName() +
                              ".red";
      textureFileNames[ 1 ] = gkg::File( outputFileName ).getBaseName() +
                              ".green";
      textureFileNames[ 2 ] = gkg::File( outputFileName ).getBaseName() +
                              ".blue";
      output.getHeader().addAttribute( "texture_names", textureNames );
      output.getHeader().addAttribute( "texture_filenames", textureFileNames );

      gkg::Writer::getInstance().write(
                               outputFileName + ".red",
                               redTextureMap,
                               ascii,
                               textureMapFormat );
      gkg::Writer::getInstance().write(
                               outputFileName + ".green",
                               greenTextureMap,
                               ascii,
                               textureMapFormat );
      gkg::Writer::getInstance().write(
                               outputFileName + ".blue",
                               blueTextureMap,
                               ascii,
                               textureMapFormat );

    }

    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorNormalizedGFA::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


void gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // obtaining threshold GFA
    float gfa = ( float )scalarParameters[ 5 ];
    float lowerGFA = scalarParameters[ 2 ];
    float upperGFA = scalarParameters[ 3 ];
    if ( gfa < lowerGFA )
    {

      gfa = 0.0;

    }
    else if ( gfa > upperGFA )
    {

      gfa = 1.0;

    }
    else
    {

      gfa = ( gfa - lowerGFA ) / ( upperGFA- lowerGFA );

    }

    // obtaining MD
    float meanDiffusivity = ( float )scalarParameters[ 6 ];

    // preparing max filter
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float maximum;

    // processing min-max scaling
    item = odf.getOrientationProbabilities();

    // processing max value
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, meshScale * gfa * ( 1.0 - meanDiffusivity ) );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMDGFAScaledMaxNormalizedMesh
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U > ,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                 gkg::OdfFunctorMDGFAScaledMaxNormalizedMesh::getInstance() );
