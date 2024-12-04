#include <gkg-processing-io/PolygonMapDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/PolygonMap.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/PolygonSizeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <fstream>


template < uint32_t D >
gkg::PolygonMapDiskFormat< D >::PolygonMapDiskFormat()
                               : gkg::TypedDiskFormat<
                                 gkg::PolygonMap< D > >()
{

  this->_extensions.insert( ".polygonmap" );
  this->_extensions.insert( getHeaderExtension() );

}


template < uint32_t D >
gkg::PolygonMapDiskFormat< D >::~PolygonMapDiskFormat()
{
}


template < uint32_t D >
std::string gkg::PolygonMapDiskFormat< D >::getName() const
{

  return "polygonmap";

}


template < uint32_t D >
void gkg::PolygonMapDiskFormat< D >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all PolygonMap Mesh specific attributes
    object.addSemantic( "__polygonmap__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__polygonmap__", "io_mode",
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

    // checking that it is a polygon map
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "PolygonMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good polygon size
    uint32_t polygonSize;
    object.getHeader().getAttribute( "polygon_size", polygonSize );
    if ( polygonSize != D )
    {

      throw gkg::PolygonSizeException( this->getNameWithoutExtension( name ) +
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
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMapDiskFormat< D >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < uint32_t D >
void gkg::PolygonMapDiskFormat< D >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMapDiskFormat< D >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < uint32_t D >
void gkg::PolygonMapDiskFormat< D >::read(
                                   const std::string& name,
                                   gkg::PolygonMap< D >& object ) const
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
      is( ( this->getNameWithoutExtension( name ) + ".polygonmap" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".polygonmap" );

    }

    gkg::ItemReader< int32_t >& itemR_R =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( ascii,
                                                                     bswap );
    gkg::ItemReader< gkg::Polygon< D > >& itemR_P =
      gkg::DefaultItemIOFactory< gkg::Polygon< D > >::getInstance().getReader(
                                                                      ascii,
                                                                      bswap );

    int32_t r, s;
    for ( r = 0; r < rankCount; r++ )
    {

      int32_t rank;
      itemR_R.read( is, rank );

      std::list< gkg::Polygon< D > > polygons;    
      for ( s = 0; s < itemCount[ r ]; s++ )
      {

        gkg::Polygon< D > polygon;
        itemR_P.read( is, polygon );
        polygons.push_back( polygon );

      }
      object.addPolygons( rank, polygons );

    }

    is.close();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMapDiskFormat< D >::read( "
             "const std::string& name, "
             "gkg::PolygonMap< D >& object ) const" );

}


template < uint32_t D >
void gkg::PolygonMapDiskFormat< D >::write( 
                                          const std::string& name,
                                          gkg::PolygonMap< D >& object,
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


    // writing core data <*.polygonmap>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".polygonmap" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".polygonmap" );

    }

    // writing ranks/polygons
    gkg::ItemWriter< int32_t >& itemW_R =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getWriter( ascii,
                                                                    false );
    gkg::ItemWriter< gkg::Polygon< D > >& itemW_P =
      gkg::DefaultItemIOFactory< gkg::Polygon< D > >::getInstance().getWriter(
                                                                     ascii,
                                                                     false );

    typename std::map< int32_t,
                       std::list< gkg::Polygon< D > > >::const_iterator
      i = object.begin(),
      ie = object.end();

    while ( i != ie )
    {

      itemW_R.write( os, i->first );

      typename
        std::list< gkg::Polygon< D > >::const_iterator
        p = i->second.begin(),
        pe = i->second.end();

      while ( p != pe )
      {

        itemW_P.write( os, *p );
        ++ p;

      }
      ++ i;

    }

    os.close();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMapDiskFormat< D >::write( "
             "const std::string& name, gkg::PolygonMap< D >& object, "
             "bool ascii ) const" );

}


template < uint32_t D >
bool gkg::PolygonMapDiskFormat< D >::hasReader() const
{

  return true;

}


template < uint32_t D >
bool gkg::PolygonMapDiskFormat< D >::hasWriter() const
{

  return true;

}


template < uint32_t D >
std::string gkg::PolygonMapDiskFormat< D >::getHeaderExtension() const
{

  return std::string( ".polygonmap" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


template class gkg::PolygonMapDiskFormat< 2U >;
template class gkg::PolygonMapDiskFormat< 3U >;
template class gkg::PolygonMapDiskFormat< 4U >;


//
// registrating standard PolygonMap disk format(s) for "PolygonMap" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::PolygonMap< 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::PolygonMapDiskFormat< 2U >::getInstance().getName(),
         &gkg::PolygonMapDiskFormat< 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::PolygonMap< 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::PolygonMapDiskFormat< 3U >::getInstance().getName(),
         &gkg::PolygonMapDiskFormat< 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::PolygonMap< 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::PolygonMapDiskFormat< 4U >::getInstance().getName(),
         &gkg::PolygonMapDiskFormat< 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "SiteMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
