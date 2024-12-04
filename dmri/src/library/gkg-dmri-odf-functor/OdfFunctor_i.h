#ifndef _gkg_dmri_odf_functor_OdfFunctor_i_h_
#define _gkg_dmri_odf_functor_OdfFunctor_i_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
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


template < class C, class I >
inline
gkg::OdfFunctor< C, I >::OdfFunctor()
{
}


template < class C, class I >
inline
gkg::OdfFunctor< C, I >::~OdfFunctor()
{
}


template < class C, class I >
inline
void gkg::OdfFunctor< C, I >::process(
                        C& /*output*/,
                        gkg::OdfCartesianField& /*odfCartesianField*/,
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
             "void gkg::OdfFunctor< C, I >::process( "
             "C& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template < class C, class I >
inline
void gkg::OdfFunctor< C, I >::processItem(
                 I& /*item*/,
                 const gkg::OrientationDistributionFunction& /*odf*/,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // does nothing by default

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::OdfFunctor< C, I >::processItem( "
             "I& /*item*/, "
             "const gkg::OrientationDistributionFunction& /*odf*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


namespace gkg
{


template <>
inline
void OdfFunctor< Volume< float >, float >::process(
                             Volume< float >& output,
                             OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // preparing output volume
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *odfCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< Volume< Vector3d< float > >, Vector3d< float > >::process(
                             Volume< Vector3d< float > >& output,
                             OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *odfCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< "
             "Volume< Vector3d< float > >, Vector3d< float > >::process( "
             "Volume< float >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< Volume< float >, std::vector< float > >::process(
                             Volume< float >& output,
                             OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( item,
                         *odfCartesianField.getItem( *s ),
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

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< Volume< float >, "
             "std::vector< float > >::process( "
             "Volume< float >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< Volume< RGBComponent >, RGBComponent >::process(
                             Volume< RGBComponent >& output,
                             OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = odfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = odfCartesianField.getSizeX();
    int32_t sizeY = odfCartesianField.getSizeY();
    int32_t sizeZ = odfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = odfCartesianField.getResolutionX();
    resolution.y = odfCartesianField.getResolutionY();
    resolution.z = odfCartesianField.getResolutionZ();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( output( *s - offset ),
                         *odfCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< "
             "Volume< RGBComponent >, RGBComponent >::process( "
             "Volume< float >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< float > >::process(
                             MeshMap< int32_t, float, 3U >& output,
                             OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    //Vector3d< int32_t > 
    //  offset = odfCartesianField.getOffset();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( scaling,
                         *odfCartesianField.getItem( *s ),
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

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< float > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< std::vector< float > > >::process(
                            MeshMap< int32_t, float, 3U >& output,
                            OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    //Vector3d< int32_t > 
    //  offset = odfCartesianField.getOffset();

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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( scalings,
                         *odfCartesianField.getItem( *s ),
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

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< std::vector< float > > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void OdfFunctor< TextureMap< std::vector< Vector3d< float > > >,
                 std::vector< Vector3d< float > > >::process(
                         TextureMap< std::vector< Vector3d< float > > >& output,
                         OdfCartesianField& odfCartesianField,
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
    int32_t siteCount = odfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = odfCartesianField.getSiteMap().getSites( 0 );

    // preparing ouput texture map
    std::vector< std::vector< Vector3d< float > > > items( siteCount );
    output.addTextures( items );

    // looping over site(s) and processing output item(s)
    int32_t count = 0;
    std::list< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    gkg::TextureMap< std::vector< Vector3d< float > > >::iterator
      t = output.begin();
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

            std::cout << Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1 
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      this->processItem( t->second,
                         *odfCartesianField.getItem( *s ),
                         scalarParameters,
                         stringParameters );

      ++ s;
      ++ t;
      ++ count;

    }
    if ( verbose )
    {

      std::cout << Eraser( 28 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void OdfFunctor< TextureMap< std::vector< Vector3d< float > > >, "
             "std::vector< Vector3d< float > > >::process( "
             "TextureMap< std::vector< Vector3d< float > > >& output, "
             "OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


}


#endif
