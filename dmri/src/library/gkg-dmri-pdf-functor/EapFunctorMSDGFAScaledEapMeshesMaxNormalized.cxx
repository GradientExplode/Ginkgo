#include <gkg-dmri-pdf-functor/EapFunctorMSDGFAScaledEapMeshesMaxNormalized.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::
EapFunctorMSDGFAScaledEapMeshesMaxNormalized()
                                         : gkg::EapFunctor<
                                           gkg::MeshMap< int32_t, float, 3U > ,
                                           std::vector< float > >()
{
}


gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::
~EapFunctorMSDGFAScaledEapMeshesMaxNormalized()
{
}


std::string gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::getName() const
{

  return "msdgfa_scaled_eap_meshes_max_normalized";

}


bool 
gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::isStandardBasisRequired() const
{

  return true;

}


void gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::process(
			std::vector< gkg::MeshMap< int32_t, float, 3U > >& output,
			gkg::EapCartesianField& eapCartesianField,
			const std::vector< double >& scalarParameters,
			const std::vector< std::string >& stringParameters,
			bool verbose ) const
{

  try
  {

    int32_t radiiCount = scalarParameters.size() - 7;
    int32_t r;

    int32_t siteCount;

    // getting site count
    siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    //Vector3d< int32_t > 
    //  offset = eapCartesianField.getOffset();

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
    for ( r = 0; r < radiiCount; r++ )
    {

      char filename[300];
      sprintf( filename, "%s0%d", stringParameters[3].c_str(), r );
      
      double radius = scalarParameters[ 7 + r ];
      
      if ( verbose )
      {

	std::cout << "\nprocessing " << this->getName() 
		  << " at radius " << radius
		  << " : " << std::flush;

      }

      
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

      // getting offset
      //Vector3d< int32_t >
      //  offset = eapCartesianField.getOffset();


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
	
	/////////////////////////////////////////////////////////////////////
	// Get SH values of the EapCartesianField
	////////////////////////////////////////////////////////////////////
	gkg::OrientationDistributionFunction eapAtRadiusR0 = 
	  *eapCartesianField.getItem( *s );
	
	std::vector< float > eapValuesAtRadiusR0 = eapAtRadiusR0.getValues();
	
	int32_t n;
	int32_t order;
	int32_t N = (int32_t)eapValuesAtRadiusR0.size();

	/////////////////////////////////////////////////////////////////////
	//  In order to have a normalized EAP,
	//  i.e. int P(r) dr = 1 
	//
	//  we can derive the normalization factor analytically, given by
	//  2*sqrt(pi)*r_0*R0max, 
	//  where r_0 is the first term of the EAP SH representation
	/////////////////////////////////////////////////////////////////////
	//float R0max = scalarParameters[ scalarParameters.size() - 1 ];
	//float r0 = eapValuesAtRadiusR0[ 0 ];
	//float eapNormalizationFactor = 2*std::sqrt(M_PI)*r0*R0max;

	// At this point, we are still in SH basis, we set the SH coefficients
	// of the EAP at order R0
	for ( n = 0; n < N; n++ )
	{
	    
	  order = 0;
	  while ( std::abs( n - ( order * order + order ) / 2 ) > order )
	  {
		
	    order += 2;
	    
	  }
	  
	  if(radius == 0 ) 
	  {
	    
	    // this should be the getAverageNormalizedDwSignal() from DpiEap
	    // or getAnalyticEapAtZero()
	    eapValuesAtRadiusR0[ n ] *= //1/eapNormalizationFactor*
	      std::pow( EPSILON,
			( double ) order - 2.0 );
	    
	  }
	  else
	    eapValuesAtRadiusR0[ n ] *= //1/eapNormalizationFactor*
	      std::pow( radius,
			( double ) order - 2.0 );

	}

	eapAtRadiusR0.setValues( eapValuesAtRadiusR0 );
      
	// Now we can switch to the standard basis
	//eapAtRadiusR0.setUnnormalizedStandardBasisType();
	eapAtRadiusR0.setStandardBasisType();
	
	// Now we must divide by the number of shells
	std::vector< float > eapSphericalValues = eapAtRadiusR0.getValues();
	int32_t countValues = eapSphericalValues.size();

	for( n = 0; n < countValues; n++)
	{
	  
	  eapSphericalValues[ n ] /= radiiCount;
	
	}
		
	eapAtRadiusR0.setValues( eapSphericalValues );

	this->processItem( scaling,
			   //*eapCartesianField.getItem( *s ),
			   eapAtRadiusR0,
			   scalarParameters,
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

      output[r].add( meshAccumulator );

      // position, backup the texture file(s)
      std::string outputFileName = filename;
      bool ascii = ( scalarParameters[ 2 ] > 0 ? true : false );
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
      output[r].getHeader().addAttribute( "texture_names", textureNames );
      output[r].getHeader().addAttribute( "texture_filenames", 
					  textureFileNames );
      
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
  GKG_CATCH( "void gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );


}


void gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::processItem(
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
    float maximum;

    // processing min-max scaling
    item = eap.getOrientationProbabilities();

    // setting negative value(s) to zero
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
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, meshScale * gfa * ( 1.0 - msd ) );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( 
         "void gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::processItem( "
         "std::vector< float >& item, "
         "const gkg::OrientationDistributionFunction& eap, "
         "const std::vector< double >& scalarParameters, "
         "const std::vector< std::string >& /*stringParameters*/ ) "
         "const" );

}


////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorMSDGFAScaledEapMeshesMaxNormalized 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U > ,
                        std::vector< float > >::getInstance().
                                                           registerEapFunctor(
             gkg::EapFunctorMSDGFAScaledEapMeshesMaxNormalized::getInstance() );

