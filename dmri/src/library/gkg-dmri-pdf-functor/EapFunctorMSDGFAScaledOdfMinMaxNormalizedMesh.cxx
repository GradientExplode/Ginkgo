#include <gkg-dmri-pdf-functor/EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::
                                   EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh()
                                         : gkg::EapFunctor<
                                           gkg::MeshMap< int32_t, float, 3U > ,
                                           std::vector< float > >()
{
}


gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::
                                   ~EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh()
{
}


std::string gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::getName() const
{

  return "msdgfa_scaled_odf_mesh";

}


bool 
gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::process(
                             gkg::MeshMap< int32_t, float, 3U >& output,
                             gkg::EapCartesianField& eapCartesianField,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool verbose ) const
{

  try
  {

    float odfIntegralLowerLimit = scalarParameters[ 4 ];
    float odfIntegralUpperLimit = scalarParameters[ 5 ];
    bool marginalOdf = scalarParameters[3];

    if ( verbose )
    {

      std::cout << "processing ";
      
      if ( marginalOdf )
	std::cout << "marginal ";
      else
	std::cout << "tuch's ";
      
      std::cout << this->getName() << " between radius " 
		<< odfIntegralLowerLimit << " and " 
		<< odfIntegralUpperLimit << " : "  
		<< std::flush;

    }

    gkg::Volume< float > normalizedGfa;
    gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getEapFunctor( "normalized_gfa" ).process( normalizedGfa,
                                                     eapCartesianField,
                                                     scalarParameters,
                                                     stringParameters,
                                                     false );

    gkg::Volume< float > normalizedMeanSquaredDisplacement;
    gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getEapFunctor( "normalized_mean_squared_displacement" ).process(
                                                 normalizedMeanSquaredDisplacement,
						 eapCartesianField,
						 scalarParameters,
						 stringParameters,
						 false );

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = eapCartesianField.getOffset();

    // preparing ouput mesh map
    gkg::Vector3d< double > resolution;
    resolution.x = eapCartesianField.getResolutionX();
    resolution.y = eapCartesianField.getResolutionY();
    resolution.z = eapCartesianField.getResolutionZ();

    // preparing translation
    gkg::Translation3d< float > translation;

    // processing convex hull of output orientation set
    const OrientationSet&
      outputOrientationSet = eapCartesianField.getOutputOrientationSet();
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
      specificScalarParameters[ 6 ] = 
	normalizedMeanSquaredDisplacement( *s - offset );


            /////////////////////////////////////////////////////////////////////
      // Get SH values of the EapCartesianField
      ////////////////////////////////////////////////////////////////////
      gkg::OrientationDistributionFunction eapAtRadiusR0 = 
	                                         *eapCartesianField.getItem( *s );

      // need to do this because EAP as set the basis to standard
      std::vector< float > eapValuesAtRadiusR0 = eapAtRadiusR0.getValues();

      int32_t n;
      int32_t order;
      int32_t N = (int32_t)eapValuesAtRadiusR0.size();
      // if marginalOdf
      // true int_0^R P(R) R^2 dR as in Wedeen et al
      // false int_0^R P(R) dR as in Tuch 

      // At this point, we are still in SH basis, we set the SH coefficients
      // of the ODF computed between 0 and infty
      for ( n = 0; n < N; n++ )
      {
	  
	order = 0;

	while ( std::abs( n - ( order * order + order ) / 2 ) > order )
	{
	    
	  order += 2;
	  
	}

	if( odfIntegralUpperLimit == 0 ) 
	{
	    
	  throw std::runtime_error( "ODF is null : zero upper limit in \
the integral" );	  
	  
	}
	else 
	{ 
	  
	  if( marginalOdf ) 
	  {
	    
	    //std::cout << "ODF marginal\n";
	    eapValuesAtRadiusR0[ n ] *= ( std::pow( odfIntegralUpperLimit,
						      order + 1 ) - 
					  std::pow( odfIntegralLowerLimit,
						    order + 1 ) ) / (order + 1) ;

	  }
	  else 
	  {
	    
	    //std::cout << "ODF Tuch\n";
	    // Need the EPSILON because otherwise, 
	    // there is a change of sign when order = 0
	    // which dramatically changes the shape of the ODFs
	    if ( odfIntegralLowerLimit == 0 )
	    {
	    
	      eapValuesAtRadiusR0[ n ] *= ( std::pow( odfIntegralUpperLimit,
						      order - 1 ) - 
					    std::pow( EPSILON,
						      order - 1 ) ) / (order - 1) ;

	    }
	    else
	    {

	      eapValuesAtRadiusR0[ n ] *= ( std::pow( odfIntegralUpperLimit,
						      order - 1 ) - 
					    std::pow( odfIntegralLowerLimit,
						      order - 1 ) ) / (order - 1) ;

	    }
	    	    
	  }
	 		 
	}

      }
	
      eapAtRadiusR0.setValues( eapValuesAtRadiusR0 );

      // Now we can switch to the standard basis
      eapAtRadiusR0.setStandardBasisType();

      this->processItem( scaling,
                         eapAtRadiusR0,
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
  GKG_CATCH( "void gkg::EapFunctorNormalizedGFA::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


void gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& eap,
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

    // obtaining MSD
    float msd = ( float )scalarParameters[ 6 ];

    // preparing min max filter(s)
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    float minimum, maximum;

    // processing min-max scaling
    item = eap.getOrientationProbabilities();


    int32_t o = 0;
    int32_t orientationCount = ( int32_t )item.size();
    int32_t count = 0;
    for ( o = 0; o < orientationCount; o++ )
    {
	
      if ( item[ o ] < 0 )
	{
	  
	  item[ o ] = 0.0;
	  count++;
	  
	}
      
    }

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, meshScale * gfa * ( 1.0 - msd ) );
    rescaler.rescale( item, item );

  }
 GKG_CATCH(
        "void gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::processItem( "
	"std::vector< float >& item, "
	"const gkg::OrientationDistributionFunction& eap, "
	"const std::vector< double >& scalarParameters, "
	"const std::vector< std::string >& /*stringParameters*/ ) "
	"const" );

}


////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////
static bool register_gkg_EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U > ,
                        std::vector< float > >::getInstance().
                                                           registerEapFunctor(
	    gkg::EapFunctorMSDGFAScaledOdfMinMaxNormalizedMesh::getInstance() );


