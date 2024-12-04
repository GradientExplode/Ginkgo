#ifndef _gkg_dmri_io_BundleMapDiskFormat_i_h_
#define _gkg_dmri_io_BundleMapDiskFormat_i_h_


#include <gkg-dmri-io/BundleMapDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapDiskFormat< L, Compare >::BundleMapDiskFormat()
                                       : gkg::TypedDiskFormat<
                                         gkg::BundleMap< L, Compare > >()
{

  this->_extensions.insert( ".bundlemap" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class L, class Compare >
inline
gkg::BundleMapDiskFormat< L, Compare >::~BundleMapDiskFormat()
{
}


template < class L, class Compare >
inline
std::string gkg::BundleMapDiskFormat< L, Compare >::getName() const
{

  return "bundlemap";

}


template < class L, class Compare >
inline
void gkg::BundleMapDiskFormat< L, Compare >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all BundleMap specific attributes
    object.addSemantic( "__bundlemap__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__bundlemap__", "io_mode",
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

    // checking that it is a bundle map
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "BundleMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good label type
    std::string labelType;
    object.getHeader().getAttribute( "label_type", labelType );
    if ( ! gkg::TypeOf< L >::isSameType( labelType ) )
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
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapDiskFormat< L, Compare >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapDiskFormat< L, Compare >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapDiskFormat< L, Compare >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapDiskFormat< L, Compare >::read(
                                    const std::string& name,
                                    gkg::BundleMap< L, Compare >& object ) const
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

    // getting bundle count
    int32_t itemCount = 0;
    object.getHeader().getAttribute( "item_count", itemCount );

    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) + ".bundlemap" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".bundlemap" );

    }

    gkg::ItemReader< L >& itemR_L =
      gkg::DefaultItemIOFactory< L >::getInstance().getReader(
                                                               ascii,
                                                               bswap );

    gkg::ItemReader< gkg::Curve3dMap< float > >& itemR_C =
      gkg::DefaultItemIOFactory< gkg::Curve3dMap< float > >::getInstance().
                                                             getReader( ascii,
                                                                        bswap );

    std::list< L > labels;
    std::list< gkg::Curve3dMap< float > > bundles;

    int32_t i;
    for ( i = 0; i < itemCount; i++ )
    {

      L label;
      typename gkg::BundleMap< L, Compare >::Bundle bundle;
      itemR_L.read( is, label );
      itemR_C.read( is, bundle );

      labels.push_back( label );
      bundles.push_back( bundle );


    }

    object.addBundles( labels, bundles );

    is.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapDiskFormat< L, Compare >::read( "
             "const std::string& name, "
             "gkg::BundleMap< L, Compare >& object ) const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapDiskFormat< L, Compare >::write( const std::string& name,
                                           gkg::BundleMap< L, Compare >& object,
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
                        ".bundlemap" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".bundlemap" );

    }

    // writing ranks/sites
    gkg::ItemWriter< L >& itemW_L =
      gkg::DefaultItemIOFactory< L >::getInstance().getWriter( ascii,
                                                               false );
    gkg::ItemWriter< gkg::Curve3dMap< float > >& itemW_C =
      gkg::DefaultItemIOFactory< gkg::Curve3dMap< float > >::getInstance().
                                                             getWriter( ascii,
                                                                        false );

    typename std::map< L, gkg::Curve3dMap< float > >::const_iterator
      i = object.begin(),
      ie = object.end();

    while ( i != ie )
    {

      itemW_L.write( os, i->first );
      itemW_C.write( os, i->second );
      ++ i;

    }

    os.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapDiskFormat< L, Compare >::write( "
             "const std::string& "
             "name, gkg::BundleMap< L, Compare >& object, "
             "bool ascii ) const" );

}


template < class L, class Compare >
inline
bool gkg::BundleMapDiskFormat< L, Compare >::hasReader() const
{

  return true;

}


template < class L, class Compare >
inline
bool gkg::BundleMapDiskFormat< L, Compare >::hasWriter() const
{

  return true;

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapDiskFormat< L, Compare >::getHeaderExtension() const
{

  return std::string( ".bundlemap" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
