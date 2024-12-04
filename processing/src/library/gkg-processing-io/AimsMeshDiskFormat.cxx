#include <gkg-processing-io/AimsMeshDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <list>


template < uint32_t D >
gkg::AimsMeshDiskFormat< D >::AimsMeshDiskFormat()
                             : gkg::TypedDiskFormat<
                                 gkg::MeshMap< int32_t, float, D > >()
{

  this->_extensions.insert( ".mesh" );
  this->_extensions.insert( getHeaderExtension() );

}


template < uint32_t D >
gkg::AimsMeshDiskFormat< D >::~AimsMeshDiskFormat()
{
}


template < uint32_t D >
std::string gkg::AimsMeshDiskFormat< D >::getName() const
{

  return "aimsmesh";

}


template < uint32_t D >
void gkg::AimsMeshDiskFormat< D >::readHeader(
                                             const std::string& name,
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
    if ( object.getHeader().hasAttribute( "object_type" ) )
    {

      std::string objectType;
      object.getHeader().getAttribute( "object_type", objectType );
      if ( objectType != "MeshMap" )
      {

        throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                        getHeaderExtension() );

      }

    }
    else
    {

      // the .minf is sometimes empty. In that case set the object type
      // otherwise this disk format will never be used
      object.getHeader().addAttribute( "object_type",
                                       std::string( "MeshMap" ) );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::AimsMeshDiskFormat< D >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < uint32_t D >
void gkg::AimsMeshDiskFormat< D >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::AimsMeshDiskFormat< D >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < uint32_t D >
void gkg::AimsMeshDiskFormat< D >::read(
                               const std::string& name,
                               gkg::MeshMap< int32_t, float, D >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // reading core data <*.mesh>
    std::ifstream is( ( this->getNameWithoutExtension( name ) +
                        ".mesh" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) +
                                        ".mesh" );

    }

    // getting IO mode
    bool ascii = false;
    char ioMode[ 6 ];
    is.read( ioMode, 5U );
    ioMode[ 5 ] = '\0';

    if ( std::string( ioMode ) == "ascii" )
    {

      ascii = true;

    }
    else if ( std::string( ioMode ) == "binar" )
    {

      ascii = false;

    }
    else
    {

       throw std::runtime_error( "wrong I/O mode" );

    }
    if ( object.getHeader().hasAttribute( "io_mode" ) )
    {

      std::string ioModeName;
      object.getHeader().getAttribute( "io_mode", ioModeName );
      if ( ( gkg::IOMode::getInstance().getTypeFromName( ioModeName ) ==
             gkg::IOMode::Ascii ) && !ascii )
      {

       throw std::runtime_error( "incompatible I/O mode in headers" );

      }
  
    }

    // getting byte order
    bool bswap = false;
    if ( !ascii )
    {

      uint32_t magicNumber;
      is.read( ( char* )&magicNumber, sizeof( uint32_t ) );

      if ( magicNumber != 0x41424344 )
      {

        bswap = true;

      }

    }
    if ( object.getHeader().hasAttribute( "byte_order" ) )
    {

      std::string byteOrderName;
      object.getHeader().getAttribute( "byte_order", byteOrderName );

      bool tmpBinarySwap = 
           ( gkg::ByteOrder::getInstance().getTypeFromName( byteOrderName ) !=
             gkg::ByteOrder::getInstance().getCurrentType() ? true : false );

      if ( tmpBinarySwap != bswap )
      {

       throw std::runtime_error( "incompatible byte order in headers" );

      }

    }

    // getting item readers
    gkg::ItemReader< uint32_t >& itemR_uint32_t =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( ascii,
                                                                      bswap );
    gkg::ItemReader< gkg::Vector3d< float > >& itemR_Vector3d_of_float =
      gkg::DefaultItemIOFactory<
                   gkg::Vector3d< float > >::getInstance().getReader( ascii,
                                                                      bswap );
    gkg::ItemReader< gkg::Polygon< D > >& itemR_Polygon =
      gkg::DefaultItemIOFactory<
                        gkg::Polygon< D > >::getInstance().getReader( ascii,
                                                                      bswap );

    // checking that texture is of VOID type
    std::string textureTypeName;
    if ( !ascii )
    {

      uint32_t textureTypeNameSize;
      itemR_uint32_t.read( is, textureTypeNameSize );
      if ( textureTypeNameSize != 4 )
      {

         throw std::runtime_error( "texture should be of VOID type" );

      }

      char textureTypeNameChar[ 5 ];
      is.read( textureTypeNameChar, 4 );
      textureTypeNameChar[ 4 ] = '\0';
      textureTypeName = ( std::string )textureTypeNameChar;

    }
    else
    {

      is >> textureTypeName;

    }
    if ( textureTypeName != "VOID" )
    {

      throw std::runtime_error( "texture should be of VOID type" );

    }



    // reading polygons size
    uint32_t polygonSize;
    itemR_uint32_t.read( is, polygonSize );
    if ( polygonSize != D )
    {

      throw std::runtime_error( "bad polygon size" );

    }

    // reading rank count
    uint32_t rankCount;
    itemR_uint32_t.read( is, rankCount );

    uint32_t r, v, n, p;
 //   uint32_t d, globalVertexCount = 0U;
    for ( r = 0; r < rankCount; r++ )
    {

      // reading rank value
      uint32_t rank;
      itemR_uint32_t.read( is, rank );
   
      // reading vertex count for current rank
      uint32_t vertexCount;
      itemR_uint32_t.read( is, vertexCount );

      // reading vertices
      gkg::Vector3d< float > vertex;
      std::list< gkg::Vector3d< float > > vertices;
      for ( v = 0; v < vertexCount; v++ )
      {

        itemR_Vector3d_of_float.read( is, vertex );
        vertices.push_back( vertex );

      }
      object.vertices.addSites( ( int32_t )rank, vertices );

      // reading norm count
      uint32_t normalCount;
      itemR_uint32_t.read( is, normalCount );
      if ( normalCount > 0 )
      {

        gkg::Vector3d< float > normal;
        for ( n = 0; n < normalCount; n++ )
        {

          // skip it
          itemR_Vector3d_of_float.read( is, normal );

        }

      }

      // reading texture count
      uint32_t textureCount;
      itemR_uint32_t.read( is, textureCount );
      if ( textureCount > 0 )
      {

        throw std::runtime_error( "texture count should be nul" );

      }

      // reading polygon count
      uint32_t polygonCount;
      itemR_uint32_t.read( is, polygonCount );

      // reading vertices
      std::list< gkg::Polygon< D > > polygons;    
      gkg::Polygon< D > polygon;
      for ( p = 0; p < polygonCount; p++ )
      {

        itemR_Polygon.read( is, polygon );
//        for ( d = 0; d < D; d++ )
//        {
//
//          polygon.indices[ d ] += globalVertexCount;
//
//       }
        polygons.push_back( polygon );

      }
      object.polygons.addPolygons( ( int32_t )r, polygons );

//      globalVertexCount += vertexCount;

    }

    is.close();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::AimsMeshDiskFormat< D >::read( "
             "const std::string& name, "
             "gkg::MeshMap< int32_t, float, D >& object ) const" );

}


template < uint32_t D >
void gkg::AimsMeshDiskFormat< D >::write( 
                                      const std::string& name,
                                      gkg::MeshMap< int32_t, float, D >& object,
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

    // writing core data <*.mesh>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".mesh" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".mesh" );

    }

    // getting item writers
    gkg::ItemWriter< uint32_t >& itemW_uint32_t =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( ascii,
                                                                      false );
    gkg::ItemWriter< gkg::Vector3d< float > >& itemW_Vector3d_of_float =
      gkg::DefaultItemIOFactory<
                   gkg::Vector3d< float > >::getInstance().getWriter( ascii,
                                                                      false );
    gkg::ItemWriter< gkg::Polygon< D > >& itemW_Polygon =
      gkg::DefaultItemIOFactory<
                        gkg::Polygon< D > >::getInstance().getWriter( ascii,
                                                                      false );

    if ( ascii )
    {

      os << "ascii" << std::endl;

    }
    else
    {

      os << "binar";

    }

    if ( !ascii )
    {

      uint32_t magicNumber = 0x41424344;
      os.write( ( const char* )&magicNumber, sizeof( uint32_t ) );
      itemW_uint32_t.write( os, ( uint32_t )4 );
      os << "VOID";               // no texture

    }
    else
    {

      os << "VOID" << std::endl;  // no texture

    }

    // D vertices polygons
    itemW_uint32_t.write( os, ( uint32_t )D );

    // writing rank count
    itemW_uint32_t.write( os, ( uint32_t )object.vertices.getRanks().size() );

    // looping over ranks
    typename gkg::SiteMap< int32_t, float >::const_iterator
      rs = object.vertices.begin(),
      rse = object.vertices.end();

//    gkg::Polygon< D > polygon;
//    uint32_t globalVertexCount = ( uint32_t)0, d;
    while ( rs != rse )
    {

      // writing current rank value
      itemW_uint32_t.write( os, ( uint32_t )rs->first );

      // writing vertex count for current rank
      itemW_uint32_t.write( os, ( uint32_t )rs->second.size() );

      // writing vertices
      std::list< gkg::Vector3d< float > >::const_iterator
        v = rs->second.begin(),
        ve = rs->second.end();
      while ( v != ve )
      {

        itemW_Vector3d_of_float.write( os, *v );
        ++ v;

      }

      // writing norm count, i.e. 0 because we let Anatomist processing them
      itemW_uint32_t.write( os, ( uint32_t )0 );

      // writing texture count, i.e. 0 because it is always VOID type
      itemW_uint32_t.write( os, ( uint32_t )0 );

      // getting polygon list
      const std::list< Polygon< D > >&
        polygonList = object.polygons.getPolygons( object.vertices.getIndex(
                                                                  rs->first ) );

      // writing polygon count
      itemW_uint32_t.write( os, ( uint32_t )polygonList.size() );

      // writing polygons
      typename std::list< gkg::Polygon< D > >::const_iterator
        p = polygonList.begin(),
        pe = polygonList.end();
      while ( p != pe )
      {

//        for ( d = 0; d < D; d++ )
//        {
//
//          polygon.indices[ d ] = p->indices[ d ] - globalVertexCount;
//
//        }
//        itemW_Polygon->write( os, polygon );
        itemW_Polygon.write( os, *p );
        ++ p;

      }

//      globalVertexCount += ( uint32_t )rs->second.size();
      ++ rs;

    }

    os.close();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::AimsMeshDiskFormat< D >::write( "
             "const std::string& name, "
             "gkg::MeshMap< int32_t, float, D >& object, "
             "bool ascii ) const" );

}


template < uint32_t D >
bool gkg::AimsMeshDiskFormat< D >::hasReader() const
{

  return true;

}


template < uint32_t D >
bool gkg::AimsMeshDiskFormat< D >::hasWriter() const
{

  return true;

}


template < uint32_t D >
std::string 
gkg::AimsMeshDiskFormat< D >::getHeaderExtension() const
{

  return std::string( ".mesh" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


//
// AimsMesh disk format singleton instanciation
//

template class gkg::AimsMeshDiskFormat< 2U >;
template class gkg::AimsMeshDiskFormat< 3U >;
template class gkg::AimsMeshDiskFormat< 4U >;


//
// registrating AimsMesh disk format(s) for "MeshMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsMeshDiskFormat< 2U >::getInstance().getName(),
         &gkg::AimsMeshDiskFormat< 2U >::getInstance() );

    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsMeshDiskFormat< 3U >::getInstance().getName(),
         &gkg::AimsMeshDiskFormat< 3U >::getInstance() );

    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsMeshDiskFormat< 4U >::getInstance().getName(),
         &gkg::AimsMeshDiskFormat< 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "AimsMesh disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
