#ifndef _gkg_dmri_pdf_functor_PdfFunctor_i_h_
#define _gkg_dmri_pdf_functor_PdfFunctor_i_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
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
gkg::PdfFunctor< C, I >::PdfFunctor()
{
}


template < class C, class I >
inline
gkg::PdfFunctor< C, I >::~PdfFunctor()
{
}


template < class C, class I >
inline
void gkg::PdfFunctor< C, I >::process(
                        C& /*output*/,
                        gkg::PdfCartesianField& /*pdfCartesianField*/,
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
             "void gkg::PdfFunctor< C, I >::process( "
             "C& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template < class C, class I >
inline
void gkg::PdfFunctor< C, I >::processItem(
                 I& /*item*/,
                 const gkg::ProbabilityDensityFunction& /*pdf*/,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // does nothing by default

  }
  GKG_CATCH( "template < class C, class I > "
             "inline "
             "void gkg::PdfFunctor< C, I >::processItem( "
             "I& /*item*/, "
             "const gkg::ProbabilityDensityFunction& /*pdf*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


namespace gkg
{


template <>
inline
void PdfFunctor< Volume< float >, float >::process(
                             Volume< float >& output,
                             PdfCartesianField& pdfCartesianField,
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
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing output volume
    int32_t sizeX = pdfCartesianField.getSizeX();
    int32_t sizeY = pdfCartesianField.getSizeY();
    int32_t sizeZ = pdfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

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
                         *pdfCartesianField.getItem( *s ),
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
             "void PdfFunctor< Volume< float >, float >::process( "
             "Volume< float >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void PdfFunctor< Volume< Vector3d< float > >, Vector3d< float > >::process(
                             Volume< Vector3d< float > >& output,
                             PdfCartesianField& pdfCartesianField,
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
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = pdfCartesianField.getSizeX();
    int32_t sizeY = pdfCartesianField.getSizeY();
    int32_t sizeZ = pdfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

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
                         *pdfCartesianField.getItem( *s ),
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
             "void PdfFunctor< "
             "Volume< Vector3d< float > >, Vector3d< float > >::process( "
             "Volume< float >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void PdfFunctor< Volume< float >, std::vector< float > >::process(
                             Volume< float >& output,
                             PdfCartesianField& pdfCartesianField,
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
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = pdfCartesianField.getSizeX();
    int32_t sizeY = pdfCartesianField.getSizeY();
    int32_t sizeZ = pdfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

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
                         *pdfCartesianField.getItem( *s ),
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
             "void PdfFunctor< Volume< float >, "
             "std::vector< float > >::process( "
             "Volume< float >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void PdfFunctor< Volume< RGBComponent >, RGBComponent >::process(
                             Volume< RGBComponent >& output,
                             PdfCartesianField& pdfCartesianField,
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
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput volume
    int32_t sizeX = pdfCartesianField.getSizeX();
    int32_t sizeY = pdfCartesianField.getSizeY();
    int32_t sizeZ = pdfCartesianField.getSizeZ();
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

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
                         *pdfCartesianField.getItem( *s ),
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
             "void PdfFunctor< "
             "Volume< RGBComponent >, RGBComponent >::process( "
             "Volume< float >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void PdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< float > >::process(
                       gkg::MeshMap< int32_t, float, 3U >& /* output */,
                       gkg::PdfCartesianField& /* pdfCartesianField */,
                       const std::vector< double >& /* scalarParameters */,
                       const std::vector< std::string >& /* stringParameters */,
                       bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }
/*
    // getting site count
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput mesh map
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

    // preparing translation
    gkg::Translation3d< float > translation;

    // processing convex hull of output orientation set
    const OrientationSet&
      outputOrientationSet = pdfCartesianField.getOutputOrientationSet();
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
      this->processItem( scaling,
                         *pdfCartesianField.getItem( *s ),
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
*/
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void PdfFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< float > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


template <>
inline
void PdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                 std::vector< std::vector< float > > >::process(
                       gkg::MeshMap< int32_t, float, 3U >& /* output */,
                       gkg::PdfCartesianField& /* pdfCartesianField */,
                       const std::vector< double >& /* scalarParameters */,
                       const std::vector< std::string >& /* stringParameters */,
                       bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }
/*
    // getting site count
    int32_t siteCount = pdfCartesianField.getSiteMap().getSiteCount( 0 );

    // getting site(s)
    const std::list< Vector3d< int32_t > >&
      sites = pdfCartesianField.getSiteMap().getSites( 0 );

    // getting offset
    Vector3d< int32_t > 
      offset = pdfCartesianField.getOffset();

    // preparing ouput mesh map
    gkg::Vector3d< double > resolution;
    resolution.x = pdfCartesianField.getResolutionX();
    resolution.y = pdfCartesianField.getResolutionY();
    resolution.z = pdfCartesianField.getResolutionZ();

    // preparing translation
    gkg::Translation3d< float > translation;

    // processing convex hull of output orientation set
    const OrientationSet&
      outputOrientationSet = pdfCartesianField.getOutputOrientationSet();
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
                         *pdfCartesianField.getItem( *s ),
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
*/
    if ( verbose )
    {

      std::cout << Eraser( 22 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "void PdfFunctor< "
             "Volume< gkg::MeshMap< int32_t, float, 3U >, "
             "std::vector< std::vector< float > > >::process( "
             "MeshMap< int32_t, float, 3U >& output, "
             "PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}


}


#endif
