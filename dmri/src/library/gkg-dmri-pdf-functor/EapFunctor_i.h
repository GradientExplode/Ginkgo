#ifndef _gkg_dmri_pdf_functor_EapFunctor_i_h_
#define _gkg_dmri_pdf_functor_EapFunctor_i_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-pdf-dpi/DpiEap.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-mesh/MeshTransformer_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>

#define EPSILON 1e-03

template < class C, class I >
inline
gkg::EapFunctor< C, I >::EapFunctor()
{
}


template < class C, class I >
inline
gkg::EapFunctor< C, I >::~EapFunctor()
{
}


template < class C, class I >
inline
void gkg::EapFunctor< C, I >::process(
                        C& /*output*/,
                        gkg::EapCartesianField& /*eapCartesianField*/,
                        const std::vector< double >& /*scalarParameters*/,
                        const std::vector< std::string >& /*stringParameters*/,
                        bool /*verbose*/ ) const
{

  try
  {

    throw std::runtime_error( "not a managed output container type" );

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::EapFunctor< C, I >::process( "
             "C& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template < class C, class I >
inline
void gkg::EapFunctor< C, I >::processItem(
                 I& /*item*/,
                 const gkg::OrientationDistributionFunction& /*eap*/,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // does nothing by default

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::EapFunctor< C, I >::processItem( "
             "I& /*item*/, "
             "const gkg::OrientationDistributionFunction& /*eap*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


namespace gkg
{


template <>
inline
void EapFunctor< Volume< float >, float >::process(
                             Volume< float >& output,
                             EapCartesianField& eapCartesianField,
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

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = eapCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = eapCartesianField.getSizeX();
    int32_t sizeY = eapCartesianField.getSizeY();
    int32_t sizeZ = eapCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = eapCartesianField.getResolutionX();
    resolution.y = eapCartesianField.getResolutionY();
    resolution.z = eapCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( 0.0f );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
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
      this->processItem( output( *s - offset ),
                         *eapCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void EapFunctor< Volume< Vector3d< float > >, Vector3d< float > >::process(
                             Volume< Vector3d< float > >& output,
                             EapCartesianField& eapCartesianField,
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

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = eapCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = eapCartesianField.getSizeX();
    int32_t sizeY = eapCartesianField.getSizeY();
    int32_t sizeZ = eapCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = eapCartesianField.getResolutionX();
    resolution.y = eapCartesianField.getResolutionY();
    resolution.z = eapCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( Vector3d< float >( 0, 0, 0 ) );
    
    // looping over site(s) and processing output item(s)
    int32_t count = 0;
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
      this->processItem( output( *s - offset ),
                         *eapCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< "
             "Volume< Vector3d< float > >, Vector3d< float > >::process( "
             "Volume< float >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void EapFunctor< Volume< float >, std::vector< float > >::process(
                             Volume< float >& output,
                             EapCartesianField& eapCartesianField,
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

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = eapCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = eapCartesianField.getSizeX();
    int32_t sizeY = eapCartesianField.getSizeY();
    int32_t sizeZ = eapCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = eapCartesianField.getResolutionX();
    resolution.y = eapCartesianField.getResolutionY();
    resolution.z = eapCartesianField.getResolutionZ();

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    int32_t itemCount = 0;
    int32_t i = 0;
    std::vector< float > item;
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
      this->processItem( item,
                         *eapCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );
      if ( s == sites.begin() )
      {

        itemCount = ( int32_t )item.size();
        output.reallocate( sizeX, sizeY, sizeZ, itemCount );
        output.getHeader().addAttribute( "resolutionX", resolution.x );
        output.getHeader().addAttribute( "resolutionY", resolution.y );
        output.getHeader().addAttribute( "resolutionZ", resolution.z );
        output.fill( 0.0f );

      }
      for ( i = 0; i < itemCount; i++ )
      {

        output( *s - offset, i ) = item[ i ];

      }

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< Volume< float >, "
             "std::vector< float > >::process( "
             "Volume< float >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void EapFunctor< Volume< RGBComponent >, RGBComponent >::process(
                             Volume< RGBComponent >& output,
                             EapCartesianField& eapCartesianField,
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

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = eapCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = eapCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = eapCartesianField.getSizeX();
    int32_t sizeY = eapCartesianField.getSizeY();
    int32_t sizeZ = eapCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = eapCartesianField.getResolutionX();
    resolution.y = eapCartesianField.getResolutionY();
    resolution.z = eapCartesianField.getResolutionZ();

    output.reallocate( sizeX, sizeY, sizeZ );
    output.getHeader().addAttribute( "resolutionX", resolution.x );
    output.getHeader().addAttribute( "resolutionY", resolution.y );
    output.getHeader().addAttribute( "resolutionZ", resolution.z );

    output.fill( RGBComponent( 0, 0, 0 ) );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
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
      this->processItem( output( *s - offset ),
                         *eapCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< "
             "Volume< RGBComponent >, RGBComponent >::process( "
             "Volume< float >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void EapFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< float > >::process(
                             MeshMap< int32_t, float, 3U >& output,
                             EapCartesianField& eapCartesianField,
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


    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

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
	    
	  throw std::runtime_error( 
                             "ODF is null : zero upper limit in the integral" );
	  
	}
	else 
	{ 
	  
	  if( marginalOdf ) 
	  {
	    
	    //std::cout << "ODF marginal\n";
	    eapValuesAtRadiusR0[ n ] *= ( std::pow( odfIntegralUpperLimit,
						      order + 1 ) - 
					  std::pow( odfIntegralLowerLimit,
						    order + 1 ) ) / 
                                        ( order + 1 );

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
						      order - 1 ) ) / 
                                          ( order - 1 );

	    }
	    else
	    {

	      eapValuesAtRadiusR0[ n ] *= ( std::pow( odfIntegralUpperLimit,
						      order - 1 ) - 
					    std::pow( odfIntegralLowerLimit,
						      order - 1 ) ) /
                                          ( order - 1 );

	    }
	    	    
	  }
	 		 
	}

      }
	
      eapAtRadiusR0.setValues( eapValuesAtRadiusR0 );

      // Now we can switch to the standard basis
      eapAtRadiusR0.setStandardBasisType();
      
      this->processItem( scaling,
                         // *eapCartesianField.getItem( *s ),
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
    output.add( meshAccumulator );


    // in case output file name is stored in the string parameters at first
    // position, backup the texture file(s)
    if ( !stringParameters.empty() && !stringParameters[ 3 ].empty() )
    {
      
      std::string outputFileName = stringParameters[ 3 ];
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
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< float > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}

////////////////////////
//  Vector of Meshes
////////////////////////
template <>
inline
void EapFunctor< std::vector< gkg::MeshMap< int32_t, float, 3U > >,
                 std::vector< float > >::process(
                           std::vector< MeshMap< int32_t, float, 3U > >& output,
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
    //////////////////////////////////////////////////////////////////
    // To test normalization, must give a single voxel in the ibox in
    // order to test
    float sum = 0.0;
    int32_t totalpoints = 0;
    int32_t totalnegative = 0;

    Volume < float > meanSquaredDisplacement;
    meanSquaredDisplacement.reallocate(
                         eapCartesianField.getSizeX(),
                         eapCartesianField.getSizeY(),
                         eapCartesianField.getSizeZ() );
    meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionX",
                                    eapCartesianField.getResolutionX() );
    meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionY",
                                    eapCartesianField.getResolutionY() );
    meanSquaredDisplacement.getHeader().addAttribute(
                                    "resolutionZ",
                                    eapCartesianField.getResolutionZ() );
    meanSquaredDisplacement.fill( 0.0 );

    Volume < float > msd;
    msd.reallocate( eapCartesianField.getSizeX(),
		     eapCartesianField.getSizeY(),
		     eapCartesianField.getSizeZ(),
		     radiiCount );
    msd.fill( 0.0 );

    // getting site count
    siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

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
      Vector3d< int32_t >
	offset = eapCartesianField.getOffset();


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
	
	float sum_msd = 0.0;
	
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
	  
	  if( eapSphericalValues[ n ] < 0 )
	  {
	    
	    totalnegative++;

	  }
	  else
	  {
	  
	    sum_msd += eapSphericalValues[ n ]*radius*radius;

	  }

	  //sum_msd += fabs(eapSphericalValues[ n ])*radius*radius;

	  eapSphericalValues[ n ] /= radiiCount;
	  totalpoints++;	    
	   
	  sum += eapSphericalValues[ n ];
	
	}
	//exit(0);

	//std::cout << *s << " " << r << "," << offset << "\t";
	msd( *s - offset, r ) = sum_msd;
		
	eapAtRadiusR0.setValues( eapSphericalValues );

	this->processItem( scaling,
			   // *eapCartesianField.getItem( *s ),
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

    // looping over site(s) and processing output item(s)
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();

    while ( s != se )
    {
      
      float sum_msd = 0.0;

      for ( r = 0; r < radiiCount; r++ )
      {
	
	sum_msd += msd( *s - offset, r ); 
	
      }		      
      
      meanSquaredDisplacement( *s - offset ) = sum_msd;
      //std::cout << "Mean Squared Displacement : " << sum_msd;

      ++ s;
      
    }

    bool ascii = ( scalarParameters[ 2 ] > 0 ? true : false );
    std::string textureMapFormat = stringParameters[ 2 ];
    std::cout << "\nwriting msd-num.ima...\t";
    gkg::Writer::getInstance().write( "msd-num", meanSquaredDisplacement,
				      ascii, textureMapFormat );
    std::cout << "Done\n";
    
    //////////////////////////
    // To test normalization
    //////////////////////////
    //       if ( verbose )
    //       {      
    //    std::cout << "\nSum of all EAP values, for all shells : "
    //    		<< sum/(radiiCount*siteCount) << "\n";
    //   std::cout << "Number of EAP negative values : "<<totalnegative << "\n";
    //         std::cout << "Total points : " << totalpoints << "\n";
    //    
    //       }
      
    if ( verbose )
    {
	  
      std::cout << Eraser( 22 ) << "done" << std::endl;
      
    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< "
             "std::vector< < gkg::MeshMap< int32_t, float, 3U >> , "
             "std::vector< float > >::process( "
             "std::vector<MeshMap< int32_t, float, 3U >>& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}

template <>
inline
void EapFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< std::vector< float > > >::process(
                            MeshMap< int32_t, float, 3U >& output,
                            EapCartesianField& eapCartesianField,
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

    // getting site count
    int32_t siteCount = eapCartesianField.getSiteMap().getSiteCount( 0 );

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

    // allocating textures and palettes
    std::vector< float > red( orientationVertexCount );
    std::vector< float > green( orientationVertexCount );
    std::vector< float > blue( orientationVertexCount );

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

    // allocating scaling(s) scalings[0]->mesh scaling
    //                       scalings[1]->red texture scaling
    //                       scalings[2]->green texture scaling
    //                       scalings[3]->blue texture scaling
    std::vector< std::vector< float > > scalings( 4 );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    int32_t o = 0;
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
      this->processItem( scalings,
                         *eapCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      // processing 3D transform
      translation.setDirectTranslation( s->x * resolution.x,
                                        s->y * resolution.y,
                                        s->z * resolution.z );

      // processing local scaled mesh map
      MeshMap< int32_t, float, 3U > localMeshMap( orientationMeshMap );
      meshScaler.scale( localMeshMap, 0, scalings[ 0 ], localMeshMap );
      meshTransformer.transform( localMeshMap, translation,
                                 localMeshMap );

      // accumulating local mesh map
      meshAccumulator.add( localMeshMap );

      // adding red/green/blue texture(s)
      for ( o = 0; o < orientationVertexCount; o++ )
      {

         red[ o ] = scalings[ 1 ][ o ] * 255.0;
         green[ o ] = scalings[ 2 ][ o ] * 255.0;
         blue[ o ] = scalings[ 3 ][ o ] * 255.0;
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
  GKG_CATCH( "template <> "
             "inline "
             "void EapFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< std::vector< float > > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


}


#endif
