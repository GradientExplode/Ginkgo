#include <gkg-processing-io/AimsBucketDiskFormat.h>
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
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <list>
#include <fstream>


gkg::AimsBucketDiskFormat::AimsBucketDiskFormat()
                         : gkg::TypedDiskFormat<
                                        gkg::SiteMap< int32_t, int32_t,
                                                      std::less< int32_t > > >()
{

  this->_extensions.insert( ".bck" );
  this->_extensions.insert( getHeaderExtension() );

}


gkg::AimsBucketDiskFormat::~AimsBucketDiskFormat()
{
}


std::string gkg::AimsBucketDiskFormat::getName() const
{

  return "aimsbucket";

}


void gkg::AimsBucketDiskFormat::readHeader( const std::string& name,
                                            gkg::HeaderedObject& object ) const
{

  try
  {

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
    if ( objectType != "Bucket" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }
    object.getHeader()[ "object_type" ] = std::string( "SiteMap" );


    // checking that it is of VOID data type
    std::string dataType;
    object.getHeader().getAttribute( "data_type", dataType );
    if ( dataType != "VOID" )
    {

      throw std::runtime_error( "not of VOID data type" );

    }


    // adding item type
    object.getHeader().addAttribute( "item_type",
                                     gkg::TypeOf< int32_t >::getName() );


    // adding rank type
    object.getHeader().addAttribute( "rank_type",
                                     gkg::TypeOf< int32_t >::getName() );


    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "void gkg::AimsBucketDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::AimsBucketDiskFormat::read(
                            const std::string& name,
                            gkg::SiteMap< int32_t, int32_t,
                                          std::less< int32_t > >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 


    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) + ".bck" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".sitemap" );

    }


    // checking I/O mode 
    char ioModeCharacters[ 6 ];
    is.read( &ioModeCharacters[ 0 ], 5U );
    ioModeCharacters[ 5 ] = 0;
    std::string ioMode( ioModeCharacters );
    bool ascii = false;
    if ( ioMode == "ascii" )
    {

      ascii = true;

    }


    // checking byte order
    char byteOrderCharacters[ 5 ];
    is.read( &byteOrderCharacters[ 0 ], 4U );
    byteOrderCharacters[ 4 ] = 0;
    std::string byteOrder( byteOrderCharacters );
    bool bswap = false;
    if ( gkg::ByteOrder::getInstance().getTypeFromABCDCode( byteOrder ) !=
         gkg::ByteOrder::getInstance().getCurrentType() )
    {

      bswap = true;

    }


    // allocating a int32_t, float item reader
    gkg::ItemReader< int16_t >& itemR_Int16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( ascii,
                                                                     bswap );

    gkg::ItemReader< int32_t >& itemR_Int32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( ascii,
                                                                     bswap );
    gkg::ItemReader< uint32_t >& itemR_UInt32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( ascii,
                                                                      bswap );
    gkg::ItemReader< float >& itemR_Float =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( ascii,
                                                                   bswap );

    // reading the void type size
    int32_t voidTypeSize = 0;
    itemR_Int32.read( is, voidTypeSize );

    if ( voidTypeSize != 4 )
    {

      throw std::runtime_error( "VOID type size should be 4" );

    }


    // reading the void type
    char voidTypeCharacters[ 5 ];
    is.read( &voidTypeCharacters[ 0 ], 4U );
    voidTypeCharacters[ 4 ] = 0;
    std::string voidType( voidTypeCharacters );

    if ( voidType != "VOID" )
    {

      throw std::runtime_error( "not a VOID bucket type" );

    }


    // reading resolution(s) and adding them to the object
    float resolutionX = 1.0;
    float resolutionY = 1.0;
    float resolutionZ = 1.0;
    float resolutionT = 1.0;
    itemR_Float.read( is, resolutionX );
    itemR_Float.read( is, resolutionY );
    itemR_Float.read( is, resolutionZ );
    itemR_Float.read( is, resolutionT );

    object.getHeader().addAttribute( "resolutionX",  ( double )resolutionX );
    object.getHeader().addAttribute( "resolutionY",  ( double )resolutionY );
    object.getHeader().addAttribute( "resolutionZ",  ( double )resolutionZ );
    object.getHeader().addAttribute( "resolutionT",  ( double )resolutionT );

    // reading the rank count
    uint32_t rankCount = 0U;
    itemR_UInt32.read( is, rankCount );

    uint32_t rankIndex = 0U;
    uint32_t rank = 0U;
    uint32_t siteCount = 0U;
    uint32_t siteIndex = 0U;
    for ( rankIndex = 0U; rankIndex < rankCount; rankIndex++ )
    {

      itemR_UInt32.read( is, rank );
      itemR_UInt32.read( is, siteCount );

      std::list< gkg::Vector3d< int32_t > > sites;

      int16_t x = 0;
      int16_t y = 0;
      int16_t z = 0;

      for ( siteIndex = 0U; siteIndex < siteCount; siteIndex++ )
      {

        itemR_Int16.read( is, x );
        itemR_Int16.read( is, y );
        itemR_Int16.read( is, z );

        sites.push_back( gkg::Vector3d< int32_t >( ( int32_t )x,
                                                   ( int32_t )y,
                                                   ( int32_t )z ) );

      }
      object.addSites( ( int32_t )rank, sites );

    }

    is.close();

  }
  GKG_CATCH( "void gkg::AimsBucketDiskFormat::read( "
             "const std::string& name, "
             "gkg::SiteMap< int32_t, int32_t, "
             "std::less< int32_t > >& object ) const" );

}


bool gkg::AimsBucketDiskFormat::hasReader() const
{

  return true;

}


std::string gkg::AimsBucketDiskFormat::getHeaderExtension() const
{

  return std::string( ".bck" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}



//
// registrating standard SiteMap disk format(s) for "SiteMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, int32_t > 
                          >::getInstance().registerDiskFormat(
                             gkg::AimsBucketDiskFormat::getInstance().getName(),
                             &gkg::AimsBucketDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "SiteMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
