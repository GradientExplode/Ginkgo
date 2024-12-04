#include <gkg-processing-io/StlMeshDiskFormat.h>
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


gkg::StlMeshDiskFormat::StlMeshDiskFormat()
                       : gkg::TypedDiskFormat<
                         gkg::MeshMap< int32_t, float, 3U > >()
{

  this->_extensions.insert( ".stl" );
  this->_extensions.insert( getHeaderExtension() );

}


gkg::StlMeshDiskFormat::~StlMeshDiskFormat()
{
}


std::string gkg::StlMeshDiskFormat::getName() const
{

  return "stl";

}


void gkg::StlMeshDiskFormat::readHeader( const std::string& name,
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
  GKG_CATCH( "void gkg::StlMeshDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::StlMeshDiskFormat::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "void gkg::StlMeshDiskFormat::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


void gkg::StlMeshDiskFormat::read(
                              const std::string& name,
                              gkg::MeshMap< int32_t, float, 3U >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // reading core data <*.mesh>
    std::ifstream is( ( this->getNameWithoutExtension( name ) +
                        ".stl" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) +
                                        ".stl" );

    }

    // getting IO mode
    bool ascii = false;
    char ioMode[ 6 ];
    is.read( ioMode, 5U );
    ioMode[ 5 ] = '\0';

    if ( std::string( ioMode ) == "solid" )
    {

      ascii = true;

    }
    else
    {

      ascii = false;

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


    // a vertex map is required to collect vertices and index them 
    std::map< gkg::Vector3d< float >, int32_t, gkg::Vector3dCompare< float > >
      vertexMap;
    gkg::Vector3d< float > normal;
    gkg::Vector3d< float > vertex;
    gkg::Polygon< 3U > polygon;
    std::list< gkg::Polygon< 3U > > polygons;    

    uint32_t newVertexIndex = 0;
    std::map< gkg::Vector3d< float >, int32_t,
              gkg::Vector3dCompare< float> >::const_iterator vi;

    ///////////////////////////// ASCII ////////////////////////////////////////
    if ( ascii )
    {

      // reading object name
      std::string tmp;
      is >> tmp;

      // reading next keyword
      is >> tmp;
      while ( tmp != "endsolid" )
      {

        // test whether it is a facet keyword
        if ( tmp != "facet" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading normal keyword
        is >> tmp;
        if ( tmp != "normal" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading normal 
        is >> normal.x >> normal.y >> normal.z;

        // reading outer keyword
        is >> tmp;
        if ( tmp != "outer" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading loop keyword
        is >> tmp;
        if ( tmp != "loop" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading vertex keyword
        is >> tmp;
        if ( tmp != "vertex" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading vertex1
        is >> vertex.x >> vertex.y >> vertex.z;
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 0 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 0 ] = vi->second;

        }

        // reading vertex keyword
        is >> tmp;
        if ( tmp != "vertex" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading vertex2
        is >> vertex.x >> vertex.y >> vertex.z;
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 1 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 1 ] = vi->second;

        }

        // reading vertex keyword
        is >> tmp;
        if ( tmp != "vertex" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading vertex3
        is >> vertex.x >> vertex.y >> vertex.z;
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 2 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 2 ] = vi->second;

        }

        // reading endloop keyword
        is >> tmp;
        if ( tmp != "endloop" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // reading endfacet keyword
        is >> tmp;
        if ( tmp != "endfacet" )
        {

          throw std::runtime_error( "STL object corrupted" );

        }

        // filling polygons
        polygons.push_back( polygon );

        // reading next keyword
        is >> tmp;

      }
      
    }
    ///////////////////////////// BINARY ///////////////////////////////////////
    else
    {

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

      // getting item readers
      gkg::ItemReader< uint32_t >& itemR_uint32_t =
        gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                        bswap );
      gkg::ItemReader< gkg::Vector3d< float > >& itemR_Vector3d_of_float =
        gkg::DefaultItemIOFactory<
                   gkg::Vector3d< float > >::getInstance().getReader( false,
                                                                      bswap );
      gkg::ItemReader< int16_t >& itemR_int16_t =
        gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                       bswap );

      // reading the 80-5 remaining uint8_t characters of the header
      // since we have already read 5 of them
      uint8_t header[ 75U ];
      is.read( ( char* )header, 75U );

      // control word
      int16_t controlWord = 0;

      // reading the triangle count
      uint32_t polygonCount = 0U;
      itemR_uint32_t.read( is, polygonCount );

      // reading the polygons
      uint32_t p = 0U;

      for ( p = 0; p < polygonCount; p++ )
      {

        // reading normal that is not stored
        itemR_Vector3d_of_float.read( is, normal );

        // reading vertex1
        itemR_Vector3d_of_float.read( is, vertex );
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 0 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 0 ] = vi->second;

        }

        // reading vertex2
        itemR_Vector3d_of_float.read( is, vertex );
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 1 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 1 ] = vi->second;

        }

        // reading vertex2
        itemR_Vector3d_of_float.read( is, vertex );
        vi = vertexMap.find( vertex );
        if ( vi == vertexMap.end() )
        {

          vertexMap[ vertex ] = newVertexIndex;
          polygon.indices[ 2 ] = newVertexIndex;
          ++ newVertexIndex;

        }
        else
        {

          polygon.indices[ 2 ] = vi->second;

        }

        // reading unused wontrol word
        itemR_int16_t.read( is, controlWord );

        // filling polygons
        polygons.push_back( polygon );

      }

    }

    // now preparing the final vertex vector from the vertex map
    std::vector< gkg::Vector3d< float > > vertices( vertexMap.size() );
    for ( vi = vertexMap.begin(); vi != vertexMap.end(); vi++ )
    {

      vertices[ vi->second ] = vi->first;

    }

    // adding vertices and polygons to the mesh map
    object.vertices.addSites( ( int32_t )0, vertices );
    object.polygons.addPolygons( ( int32_t )0, polygons );

    is.close();


  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::StlMeshDiskFormat::read( "
             "const std::string& name, "
             "gkg::MeshMap< int32_t, float, 3U >& object ) const" );

}


void gkg::StlMeshDiskFormat::write( const std::string& name,
                                    gkg::MeshMap< int32_t, float, 3U >& object,
                                    bool ascii ) const
{

  try
  {


    // STL format are only available for 3D objects, 4D objects are not managed
    if ( object.vertices.getRanks().size() != 1U )
    {

      throw std::runtime_error( "4D mesh are not compatible with STL format" );

    }

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

    // writing core data <*.stl>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".stl" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".stl" );

    }

    // since it is a 3D mesh, pointing to the vertex list
    const int32_t& rank = object.vertices.begin()->first;
    const std::list< gkg::Vector3d< float > >&
      vertexList = object.vertices.begin()->second;

    // copying the vertex list to a vector for easy access to vertices
    std::vector< gkg::Vector3d< float > > vertices( vertexList.begin(),
                                                    vertexList.end() );

    // now pointing to the polygon list
    const std::list< gkg::Polygon< 3U > >&
      polygonList = object.polygons.begin()->second;

    // and computing normals
    std::list< gkg::Vector3d< float > > normals = object.getNormals( rank );

    ///////////////////////////// ASCII ////////////////////////////////////////
    if ( ascii )
    {

      os << "solid ginkgo_object" << std::endl;

      // now writing all triangle(s)
      std::list< gkg::Polygon< 3U > >::const_iterator
        p = polygonList.begin(),
        pe = polygonList.end();
      std::list< gkg::Vector3d< float > >::const_iterator
        n = normals.begin();
      while ( p != pe )
      {

        // writing normal
        os << "  facet normal "
           << n->x << " " << n->y << " " << n->z << std::endl;

        os << "    outer loop" << std::endl;

        // writing vertex 1
        const gkg::Vector3d< float >& v1 = vertices[ p->indices[ 0 ] ];
        os << "      vertex "
           << v1.x << " " << v1.y << " " << v1.z << std::endl;

        // writing vertex 2
        const gkg::Vector3d< float >& v2 = vertices[ p->indices[ 1 ] ];
        os << "      vertex "
           << v2.x << " " << v2.y << " " << v2.z << std::endl;

        // writing vertex 3
        const gkg::Vector3d< float >& v3 = vertices[ p->indices[ 2 ] ];
        os << "      vertex "
           << v3.x << " " << v3.y << " " << v3.z << std::endl;

        os << "    endloop" << std::endl;

        os << "  endfacet" << std::endl;

        ++ n;
        ++ p;

      }

      os << "endsolid" << std::endl;

    }
    ///////////////////////////// BINARY ///////////////////////////////////////
    else
    {

      // getting item writers
      gkg::ItemWriter< uint8_t >& itemW_uint8_t =
        gkg::DefaultItemIOFactory< uint8_t >::getInstance().getWriter( false,
                                                                       false );
      gkg::ItemWriter< uint32_t >& itemW_uint32_t =
        gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                        false );
      gkg::ItemWriter< gkg::Vector3d< float > >& itemW_Vector3d_of_float =
        gkg::DefaultItemIOFactory<
                     gkg::Vector3d< float > >::getInstance().getWriter( false,
                                                                        false );
      gkg::ItemWriter< int16_t >& itemW_int16_t =
        gkg::DefaultItemIOFactory< int16_t >::getInstance().getWriter( false,
                                                                       false );
      int16_t controlWord = 0;

      // writing 80 uint8_t header
      uint8_t value = 0U;
      int32_t h = 0;
      for ( h = 0; h < 80; h++ )
      {

        itemW_uint8_t.write( os, value );

      }

      // writing the number of triangles
      itemW_uint32_t.write( os, ( uint32_t )polygonList.size() );

      // now writing all triangle(s)
      std::list< gkg::Polygon< 3U > >::const_iterator
        p = polygonList.begin(),
        pe = polygonList.end();
      std::list< gkg::Vector3d< float > >::const_iterator
        n = normals.begin();
      while ( p != pe )
      {

        // writing normal
        itemW_Vector3d_of_float.write( os, *n );

        // writing vertex 1
        itemW_Vector3d_of_float.write( os, vertices[ p->indices[ 0 ] ] );

        // writing vertex 2
        itemW_Vector3d_of_float.write( os, vertices[ p->indices[ 1 ] ] );

        // writing vertex 3
        itemW_Vector3d_of_float.write( os, vertices[ p->indices[ 2 ] ] );

        // writing control word
        itemW_int16_t.write( os, controlWord );

        ++ n;
        ++ p;

      }

    }

    os.close();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::StlMeshDiskFormat::write( "
             "const std::string& name, "
             "gkg::MeshMap< int32_t, float, 3U >& object, "
             "bool ascii ) const" );

}


bool gkg::StlMeshDiskFormat::hasReader() const
{

  return true;

}


bool gkg::StlMeshDiskFormat::hasWriter() const
{

  return true;

}


std::string 
gkg::StlMeshDiskFormat::getHeaderExtension() const
{

  return std::string( ".mesh" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


//
// registrating StlMesh disk format for "MeshMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::StlMeshDiskFormat::getInstance().getName(),
         &gkg::StlMeshDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "StlMesh disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
