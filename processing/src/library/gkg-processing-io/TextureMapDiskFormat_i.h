#ifndef _gkg_processing_io_TextureMapDiskFormat_i_h_
#define _gkg_processing_io_TextureMapDiskFormat_i_h_


#include <gkg-processing-io/TextureMapDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-io/DefaultItemIOFactoryOfStdList_i.h>
#include <gkg-processing-container/TextureMap_i.h>
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
#include <fstream>
#include <list>
#include <boost/algorithm/string.hpp>


template < class T >
inline
gkg::TextureMapDiskFormat< T >::TextureMapDiskFormat()
                               : gkg::TypedDiskFormat<
                                 gkg::TextureMap< T > >()
{

  this->_extensions.insert( ".texturemap" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class T >
inline
gkg::TextureMapDiskFormat< T >::~TextureMapDiskFormat()
{
}


template < class T >
inline
std::string gkg::TextureMapDiskFormat< T >::getName() const
{

  return "texturemap";

}


template < class T >
inline
void gkg::TextureMapDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all TextureMap specific attributes
    object.addSemantic( "__texturemap__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__texturemap__", "io_mode",
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

    // checking that it is a texture map
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "TextureMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good item type
    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );

    // managing the fact that old files could contain ptk_ instead of gkg_
    boost::replace_all( itemType, "ptk", "gkg");

    if ( ! gkg::TypeOf< T >::isSameType( itemType ) )
    {

      throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                    getHeaderExtension() );

    }

    // checking that item count exist and are valid
    int32_t itemCount = 0;
    object.getHeader().getAttribute( "item_count", itemCount );
    if ( itemCount < 0 )
    {

      throw std::runtime_error( "negative item count" );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TextureMapDiskFormat< T >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class T >
inline
void gkg::TextureMapDiskFormat< T >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TextureMapDiskFormat< T >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class T >
inline
void gkg::TextureMapDiskFormat< T >::read(
                                const std::string& name,
                                gkg::TextureMap< T >& object ) const
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

    // getting item count
    int32_t itemCount = 0;
    object.getHeader().getAttribute( "item_count", itemCount );

    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) + ".texturemap" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".texturemap" );

    }

    gkg::ItemReader< T >& itemR_T =
      gkg::DefaultItemIOFactory< T >::getInstance().getReader( ascii,
                                                               bswap );

    int32_t i;
    std::list< T > textures;
    T texture;
    for ( i = 0; i < itemCount; i++ )
    {

      itemR_T.read( is, texture );
      textures.push_back( texture );

    }
    object.addTextures( textures );

    is.close();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TextureMapDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::TextureMap< T >& object ) const" );

}


template < class T >
inline
void gkg::TextureMapDiskFormat< T >::write( const std::string& name,
                                            gkg::TextureMap< T >& object,
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
                        ".texturemap" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".texturemap" );

    }

    // writing ranks/sites
    gkg::ItemWriter< T >& itemW_T =
      gkg::DefaultItemIOFactory< T >::getInstance().getWriter( ascii,
                                                               false );

    typename std::map< int32_t, T >::const_iterator
      i = object.begin(),
      ie = object.end();

    while ( i != ie )
    {

      itemW_T.write( os, i->second );
      ++ i;

    }

    os.close();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TextureMapDiskFormat< T >::write( "
             "const std::string& "
             "name, gkg::TextureMap< T >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
bool gkg::TextureMapDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
inline
bool gkg::TextureMapDiskFormat< T >::hasWriter() const
{

  return true;

}


template < class T >
inline
std::string 
gkg::TextureMapDiskFormat< T >::getHeaderExtension() const
{

  return std::string( ".texturemap" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
