#ifndef _gkg_processing_io_SiteMapDiskFormat_i_h_
#define _gkg_processing_io_SiteMapDiskFormat_i_h_


#include <gkg-processing-io/SiteMapDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-exception/RankTypeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <list>
#include <fstream>


template < class R, class S, class Compare >
inline
gkg::SiteMapDiskFormat< R, S, Compare >::SiteMapDiskFormat()
                                        : gkg::TypedDiskFormat<
                                            gkg::SiteMap< R, S, Compare > >()
{

  this->_extensions.insert( ".sitemap" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class R, class S, class Compare >
inline
gkg::SiteMapDiskFormat< R, S, Compare >::~SiteMapDiskFormat()
{
}


template < class R, class S, class Compare >
inline
std::string gkg::SiteMapDiskFormat< R, S, Compare >::getName() const
{

  return "sitemap";

}


template < class R, class S, class Compare >
inline
void gkg::SiteMapDiskFormat< R, S, Compare >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all SiteMap Mesh specific attributes
    object.addSemantic( "__sitemap__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__sitemap__", "io_mode",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );


    // reading the meta-information header
    try
    {

      this->gkg::DiskFormat::readHeader( name, object );

    }
    catch ( gkg::FileNotFoundException& e )
    {

      // std::cerr << e.what() << ", "
      //           << "meta-information not present"
      //           << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( e.what() );

    }

    // checking that it is a site map
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "SiteMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good item type
    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    if ( ! gkg::TypeOf< S >::isSameType( itemType ) )
    {

      throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                    getHeaderExtension() );

    }

    // checking that it has the good rank type
    std::string rankType;
    object.getHeader().getAttribute( "rank_type", rankType );
    if ( ! gkg::TypeOf< R >::isSameType( rankType ) )
    {

      throw gkg::RankTypeException( this->getNameWithoutExtension( name ) +
                                    getHeaderExtension() );

    }

    // checking that rank count exists and is valid
    int32_t rankCount = 0;
    if ( object.getHeader().hasAttribute( "rank_count" ) )
    {

      object.getHeader().getAttribute( "rank_count", rankCount );
      if ( rankCount < 0 )
      {

        throw std::runtime_error( "invalid rank count" );

      }

    }
    else
    {

       throw std::runtime_error( "rank count not present" );

    }

    // checking that item counts exist and are valid
    if ( rankCount > 0 )
    {

      if ( object.getHeader().hasAttribute( "item_count" ) )
      {

        gkg::GenericObjectList
          genericObjectList( rankCount );

        std::vector< int32_t > itemCount( rankCount );
        object.getHeader().getAttribute( "item_count", genericObjectList );

        int32_t i;
        for ( i = 0; i < rankCount; i++ )
        {

          itemCount[ i ] = ( int32_t )genericObjectList[ i ]->getScalar();
          if ( itemCount[ i ] <= 0 )
          {

            throw std::runtime_error( "invalid item count" );

          }

        }

      }
      else
      {

        throw std::runtime_error( "item count not present" );

      }

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "void gkg::SiteMapDiskFormat< R, S, Compare >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMapDiskFormat< R, S, Compare >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "void gkg::SiteMapDiskFormat< R, S, Compare >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMapDiskFormat< R, S, Compare >::read(
                                   const std::string& name,
                                   gkg::SiteMap< R, S, Compare >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // getting byte order
    bool bswap = false;
    if ( object.getHeader().hasAttribute( "byte_order" ) )
    {

      std::string byteOrderName;
      object.getHeader().getAttribute( "byte_order", byteOrderName );

      bswap = 
           ( gkg::ByteOrder::getInstance().getTypeFromName( byteOrderName ) !=
             gkg::ByteOrder::getInstance().getCurrentType() ? true : false );

    }

    // getting IO mode
    bool ascii = false;
    if ( object.getHeader().hasAttribute( "io_mode" ) )
    {

      std::string ioModeName;
      object.getHeader().getAttribute( "io_mode", ioModeName );
      if ( gkg::IOMode::getInstance().getTypeFromName( ioModeName ) ==
           gkg::IOMode::Ascii )
      {

        ascii = true;

      }
  
    }

    // getting rank count
    int32_t rankCount = 0;
    object.getHeader().getAttribute( "rank_count", rankCount );

    // getting item count
    gkg::GenericObjectList genericObjectList( rankCount );
    std::vector< int32_t > itemCount( rankCount );
    object.getHeader().getAttribute( "item_count", genericObjectList );
    int32_t i;
    for ( i = 0; i < rankCount; i++ )
    {

      itemCount[ i ] = ( int32_t )genericObjectList[ i ]->getScalar();

    }

    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) + ".sitemap" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".sitemap" );

    }

    gkg::ItemReader< R >& itemR_R =
      gkg::DefaultItemIOFactory< R >::getInstance().getReader( ascii,
                                                               bswap );
    gkg::ItemReader< gkg::Vector3d< S > >& itemR_S =
      gkg::DefaultItemIOFactory< gkg::Vector3d< S > >::getInstance().getReader(
                                                                ascii,
                                                                bswap );

    int32_t r, s;

    for ( r = 0; r < rankCount; r++ )
    {

      R rank;
      itemR_R.read( is, rank );

      std::list< gkg::Vector3d< S > > sites;
      gkg::Vector3d< S > site;
      for ( s = 0; s < itemCount[ r ]; s++ )
      {

        itemR_S.read( is, site );
        sites.push_back( site );

      }
      object.addSites( rank, sites );

    }

    is.close();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "void gkg::SiteMapDiskFormat< R, S, Compare >::read( "
             "const std::string& name, "
             "gkg::SiteMap< R, S, Compare >& object ) const" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMapDiskFormat< R, S, Compare >::write( 
                                          const std::string& name,
                                          gkg::SiteMap< R, S, Compare >& object,
                                          bool ascii ) const
{

  try
  {

    // setting IO mode
    std::string ioModeName =
    gkg::IOMode::getInstance().getNameFromType( ascii ? gkg::IOMode::Ascii :
                                                        gkg::IOMode::Binary );
    if ( object.getHeader().hasAttribute( "io_mode" ) )
    {

      object.getHeader()[ "io_mode" ] = ioModeName;
      
    }
    else
    {

      object.getHeader().addAttribute( "io_mode", ioModeName );

    }

    // setting byte order
    std::string byteOrderName = gkg::ByteOrder::getInstance().getCurrentName();
    if ( object.getHeader().hasAttribute( "byte_order" ) )
    {

      object.getHeader()[ "byte_order" ] = byteOrderName;
      
    }
    else
    {

      object.getHeader().addAttribute( "byte_order", byteOrderName );

    }

    // writing header(s) meta-information
    writeHeader( name, object );


    // writing core data <*.sitemap>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".sitemap" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".sitemap" );

    }

    // writing ranks/sites
    gkg::ItemWriter< R >& itemW_R =
      gkg::DefaultItemIOFactory< R >::getInstance().getWriter( ascii,
                                                               false );
    gkg::ItemWriter< gkg::Vector3d< S > >& itemW_S =
      gkg::DefaultItemIOFactory< gkg::Vector3d< S > >::getInstance().getWriter(
                                                                ascii,
                                                                false );

    typename std::map< R,
                       std::list< gkg::Vector3d< S > >,
                       Compare >::const_iterator
      i = object.begin(),
      ie = object.end();

    while ( i != ie )
    {

      itemW_R.write( os, i->first );

      typename
        std::list< gkg::Vector3d< S > >::const_iterator
        s = i->second.begin(),
        se = i->second.end();

      while ( s != se )
      {

        itemW_S.write( os, *s );
        ++ s;

      }
      ++ i;

    }

    os.close();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "void gkg::SiteMapDiskFormat< R, S, Compare >::write( "
             "const std::string& name, gkg::SiteMap< R, S, Compare >& object, "
             "bool ascii ) const" );

}


template < class R, class S, class Compare >
inline
bool gkg::SiteMapDiskFormat< R, S, Compare >::hasReader() const
{

  return true;

}


template < class R, class S, class Compare >
inline
bool gkg::SiteMapDiskFormat< R, S, Compare >::hasWriter() const
{

  return true;

}


template < class R, class S, class Compare >
inline
std::string gkg::SiteMapDiskFormat< R, S, Compare >::getHeaderExtension() const
{

  return std::string( ".sitemap" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
