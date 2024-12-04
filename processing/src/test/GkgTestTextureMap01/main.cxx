#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
/*
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-io/AimsTextureAnalyzer_i.h>
#include <gkg-processing-io/AimsTextureDiskFormat_i.h>
*/
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <iostream>
#include <fstream>

/*
RegisterTextureAnalyzer( gkg::Vector3d< float >, gkg_Vector3d_of_float );
RegisterTextureDiskFormat( gkg::Vector3d< float >, gkg_Vector3d_of_float );
*/

int main()
{

  gkg::SiteMap< int32_t, float > siteMap;
  gkg::TextureMap< gkg::Vector3d< float > > textureMap;


  siteMap.addSite( 2, gkg::Vector3d< float >( 1.5, 2.5, 3.5 ) );
  siteMap.addSite( 5, gkg::Vector3d< float >( 1.5, 2.5, 3.5 ) );
  siteMap.addSite( 5, gkg::Vector3d< float >( 4.5, 5.5, 6.5 ) );

  gkg::RankSiteLut< int32_t, float > rankSiteLut( siteMap );

  textureMap.addTexture( rankSiteLut.getIndex(
                                      2,
                                      gkg::Vector3d< float >( 1.5, 2.5, 3.5 ) ),
                         gkg::Vector3d< float >( 1.0, 2.0, 3.0 ) );
  textureMap.addTexture( rankSiteLut.getIndex(
                                      5,
                                      gkg::Vector3d< float >( 1.5, 2.5, 3.5 ) ),
                         gkg::Vector3d< float >( 7.0, 8.0, 9.0 ) );
  textureMap.addTexture( rankSiteLut.getIndex(
                                      5,
                                      gkg::Vector3d< float >( 4.5, 5.5, 6.5 ) ),
                         gkg::Vector3d< float >( 4.0, 5.0, 6.0 ) );

  std::cout << "site map : "
            << std::endl
            << siteMap
            << std::endl;
  std::cout << "texture map : "
            << std::endl
            << textureMap
            << std::endl;


  // testing TextureMapDiskFormat
  gkg::Writer::getInstance().write( "foo1", siteMap, true );
  gkg::Writer::getInstance().write( "foo1", textureMap, true, "texturemap" );

  gkg::TextureMap< gkg::Vector3d< float > > textureMapBis;
  gkg::Reader::getInstance().read( "foo1", textureMapBis );
  std::cout << "read texture map with TextureMapDiskFormat: "
            << std::endl
            << textureMapBis
            << std::endl;

  // testing AimsTextureDiskFormat
  std::vector< int32_t > rank( 2 );
  rank[ 0 ] = 2;
  rank[ 1 ] = 5;

  std::vector< int32_t > itemCount( 2 );
  itemCount[ 0 ] = 1;
  itemCount[ 1 ] = 2;

  textureMap.getHeader().addAttribute( "aims_rank_count", ( int32_t )2 );
  textureMap.getHeader().addAttribute( "aims_rank", rank );
  textureMap.getHeader().addAttribute( "aims_item_count", itemCount );

  gkg::Writer::getInstance().write( "foo2", textureMap, true, "aimstexture" );


  gkg::TextureMap< gkg::Vector3d< float > > textureMapTer;
  gkg::Reader::getInstance().read( "foo2", textureMapTer );
  std::cout << "read texture map with AimsTextureDiskFormat: "
            << std::endl
            << textureMapTer
            << std::endl;

  return EXIT_SUCCESS;

}
