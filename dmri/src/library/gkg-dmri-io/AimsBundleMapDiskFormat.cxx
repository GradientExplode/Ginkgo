#include <gkg-dmri-io/AimsBundleMapDiskFormat.h>
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
#include <gkg-core-io/DiskFormatFactory.h>
#include <vector>
#include <fstream>


gkg::AimsBundleMapDiskFormat::AimsBundleMapDiskFormat()
                             : gkg::TypedDiskFormat<
                                 gkg::BundleMap< std::string > >()
{

  this->_extensions.insert( ".bundlesdata" );
  this->_extensions.insert( getHeaderExtension() );

}


gkg::AimsBundleMapDiskFormat::~AimsBundleMapDiskFormat()
{
}


std::string gkg::AimsBundleMapDiskFormat::getName() const
{

  return "aimsbundlemap";

}


void gkg::AimsBundleMapDiskFormat::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all BundleMap specific attributes
    object.addSemantic( "__aimsbundlemap__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__aimsbundlemap__", "io_mode",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );

    object.addSemantic( "__aimsbundlemap__", "binary",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__aimsbundlemap__", "curves_count",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__aimsbundlemap__", "data_file_name",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__aimsbundlemap__", "format",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__aimsbundlemap__", "space_dimension",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );


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
    std::string format;
    object.getHeader().getAttribute( "format", format );
    if ( format != "bundles_1.0" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that 'byte_order' is present and convert it to gkg byte order
    std::string byteOrder;
    object.getHeader().getAttribute( "byte_order", byteOrder );
    object.getHeader()[ "byte_order" ] =
      gkg::ByteOrder::getInstance().getNameFromType(
        gkg::ByteOrder::getInstance().getTypeFromABCDCode( byteOrder ) );

    // checking that 'binary' is present and convert it to gkg 'io_mode'
    double binary;
    object.getHeader().getAttribute( "binary", binary );
    std::string ioMode = ( ( int32_t )( binary + 0.5 ) == 1 ? "binary" :
                                                              "ascii" );
    object.getHeader().addAttribute( "io_mode", ioMode );

    // checking that 'curves_count' is present
    if ( !object.getHeader().hasAttribute( "curves_count" ) )
    {

      throw std::runtime_error( "'curves_count' argument is missing" );

    }
    double curvesCountDouble = 0.0;
    int32_t curvesCount;
    object.getHeader().getAttribute( "curves_count", curvesCountDouble );
    curvesCount = ( int32_t )( curvesCountDouble + 0.5 );
    if ( curvesCount < 0 )
    {

      throw std::runtime_error( "negative curves count" );

    }

    // checking that 'bundles' is present and building gkg 'curve3d_counts'
    // and 'labels' from it
    if ( !object.getHeader().hasAttribute( "bundles" ) )
    {

      throw std::runtime_error( "'bundles' argument is missing" );

    }

    // checking that 'space_dimension' is present
    if ( !object.getHeader().hasAttribute( "space_dimension" ) )
    {

      throw std::runtime_error( "'space_dimension' argument is missing" );

    }
    double spaceDimensionDouble;
    int32_t spaceDimension;
    object.getHeader().getAttribute( "space_dimension", spaceDimensionDouble );
    spaceDimension = ( int32_t )( spaceDimensionDouble + 0.5 );
    if ( spaceDimension != 3 )
    {

      throw std::runtime_error( "space dimension should be equal to 3" );

    }

    // checking that 'data_file_name' is present
    if ( !object.getHeader().hasAttribute( "data_file_name" ) )
    {

      throw std::runtime_error( "'data_file_name' argument is missing" );

    }
    std::string dataFileName;
    object.getHeader().getAttribute( "data_file_name", dataFileName );
    if ( dataFileName != "*.bundlesdata" )
    {

      throw std::runtime_error( "expected 'data_file_name' : '*.bundlesdata'" );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "void gkg::AimsBundleMapDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::AimsBundleMapDiskFormat::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

    // further saving the header for the anatomist plugin with a
    // *.clusters extension
    std::string clusterName = this->getNameWithoutExtension( name ) +
                              ".clusters";
    std::ofstream os( clusterName );

    if ( ! os )
    {

      throw gkg::NotOpenException( clusterName );

    }

    try
    {

      gkg::BaseObjectWriter baseObjectWriter( object.getSyntaxSet(),
                                              object.getWriterHelpers() );
      baseObjectWriter.write( os, object.getHeader() );
      os.close();

    }
    catch ( std::exception& e )
    {

      os.close();
      throw std::runtime_error(
                 std::string( "gkg::AimsBundleMapDiskFormat::writeHeader: " ) +
                 e.what() );

    }


    // further saving the header for the anatomist plugin with a
    // *.iclusters extension
    std::string iclusterName = this->getNameWithoutExtension( name ) +
                               ".iclusters";
    os.open( iclusterName );
    if ( ! os )
    {

      throw gkg::NotOpenException( clusterName );

    }

    try
    {

      gkg::BaseObjectWriter baseObjectWriter( object.getSyntaxSet(),
                                              object.getWriterHelpers() );
      baseObjectWriter.write( os, object.getHeader() );
      os.close();

    }
    catch ( std::exception& e )
    {

      os.close();
      throw std::runtime_error(
                 std::string( "gkg::AimsBundleMapDiskFormat::writeHeader: " ) +
                 e.what() );

    }


  }
  GKG_CATCH( "void gkg::AimsBundleMapDiskFormat::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


void gkg::AimsBundleMapDiskFormat::read(
                                   const std::string& name,
                                   gkg::BundleMap< std::string >& object ) const
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

    // getting 'curves_count' attribute
    double curvesCountDouble = 0.0;
    int32_t curvesCount;
    object.getHeader().getAttribute( "curves_count", curvesCountDouble );
    curvesCount = ( int32_t )( curvesCountDouble + 0.5 );

    // collecting information from 'bundles' attribute
    gkg::GenericObjectList genericObjectList;
    object.getHeader().getAttribute( "bundles", genericObjectList );

    std::vector< std::string > labels( genericObjectList.size() / 2 );
    std::vector< int32_t > curve3dCounts( genericObjectList.size() / 2 );
    std::vector< int32_t > tmp( genericObjectList.size() / 2 + 1 );

    gkg::GenericObjectList::const_iterator
      g = genericObjectList.begin(),
      ge = genericObjectList.end();
    int32_t count = 0;
    while ( g != ge )
    {

      labels[ count ] = ( *g )->getString();
      ++ g;
      tmp[ count ] = ( int32_t )( ( *g )->getScalar() + 0.5 );
      ++ g;

      ++ count;

    }
    tmp[ count ] = curvesCount;
    for ( count = 0; count < ( int32_t )curve3dCounts.size(); count++ )
    {

      curve3dCounts[ count ] = tmp[ count + 1 ] - tmp[ count ];

    }

    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) + ".bundlesdata" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".bundlesdata" );

    }

    if ( ascii )
    {

      int32_t curve, curve3dCount;
      int32_t point, pointCount;
      gkg::Vector3d< float > vector3d;
      char dummy;   // for reading ';' characters

      std::vector< std::string >::const_iterator
        l = labels.begin(),
        le = labels.end();
      std::vector< int32_t >::const_iterator
        c = curve3dCounts.begin();
      while ( l != le )
      {

        // allocating one bundle
        gkg::BundleMap< std::string >::Bundle bundle;

        // reserving memory given the 3D curve count
        curve3dCount = *c;
        bundle.reserve( curve3dCount );

        // looping over 3D curve(s)
        std::list< gkg::LightCurve3d< float > > curve3ds;
        for ( curve = 0; curve < curve3dCount; curve++ )
        {

          // allocating one 3D curve
          gkg::LightCurve3d< float > curve3d;

          // reserving memory given the 3D point count
          is >> pointCount;
          curve3d.reserve( pointCount );

          std::list< Vector3d< float > > points;

          // looping over 3D point(s)
          for ( point = 0; point < pointCount; point++ )
          {

            is >> vector3d.x >> vector3d.y >> vector3d.z >> dummy;
            points.push_back( vector3d );

          }
          // adding point to 3D curve
          curve3d.reserve( pointCount );
          curve3d.addPoints( points );

          // adding 3D curve to curve list
          curve3ds.push_back( curve3d );

        }
        // adding 3D curve to bundle
        bundle.addCurve3ds( curve3ds );

        // adding bundle to bundle map
        object.addBundle( *l, bundle );

        ++ l;
        ++ c;

      }

    }
    else
    {

      gkg::ItemReader< int32_t >& itemR_int32_t =
        gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                       bswap );
      gkg::ItemReader< float >& itemR_float =
        gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                     bswap );

      int32_t curve, curve3dCount;
      int32_t point, pointCount;
      gkg::Vector3d< float > vector3d;

      std::vector< std::string >::const_iterator
        l = labels.begin(),
        le = labels.end();
      std::vector< int32_t >::const_iterator
        c = curve3dCounts.begin();
      while ( l != le )
      {

        // allocating one bundle
        gkg::BundleMap< std::string >::Bundle bundle;

        // reserving memory given the 3D curve count
        curve3dCount = *c;
        bundle.reserve( curve3dCount );

        // looping over 3D curve(s)
        std::list< gkg::LightCurve3d< float > > curve3ds;
        for ( curve = 0; curve < curve3dCount; curve++ )
        {

          // allocating one 3D curve
          gkg::LightCurve3d< float > curve3d;

          // reserving memory given the 3D point count
          itemR_int32_t.read( is, pointCount );
          curve3d.reserve( pointCount );

          std::list< Vector3d< float > > points;

          // looping over 3D point(s)
          for ( point = 0; point < pointCount; point++ )
          {

            itemR_float.read( is, vector3d.x );
            itemR_float.read( is, vector3d.y );
            itemR_float.read( is, vector3d.z );
            points.push_back( vector3d );

          }
          // adding point to 3D curve
          curve3d.reserve( pointCount );
          curve3d.addPoints( points );

          // adding 3D curve to curve list
          curve3ds.push_back( curve3d );

        }
        // adding 3D curves to bundle
        bundle.addCurve3ds( curve3ds );

        // adding bundle to bundle map
        object.addBundle( *l, bundle );

        ++ l;
        ++ c;


      }

    }

    is.close();

  }
  GKG_CATCH( "void gkg::AimsBundleMapDiskFormat::read( "
             "const std::string& name, "
             "gkg::BundleMap< std::string >& object ) const" );

}


void gkg::AimsBundleMapDiskFormat::write( const std::string& name,
                                          gkg::BundleMap< std::string >& object,
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

    // adding 'format' attribute
    if ( !object.getHeader().hasAttribute( "format" ) )
    {

      object.getHeader().addAttribute( "format", std::string( "bundles_1.0" ) );

    }
    else
    {

      object.getHeader()[ "format" ] = std::string( "bundles_1.0" );

    }

    // converting 'byte_order' to AIMS byte order
    std::string byteOrder;
    object.getHeader().getAttribute( "byte_order", byteOrder );
    object.getHeader()[ "byte_order" ] =
      gkg::ByteOrder::getInstance().getABCDCodeFromType(
        gkg::ByteOrder::getInstance().getTypeFromName( byteOrder ) );

    // adding 'binary' attribute from gkg 'io_mode' attribute
    std::string ioMode;
    object.getHeader().getAttribute( "io_mode", ioMode );
    if ( ioMode == "binary" )
    {

      if ( !object.getHeader().hasAttribute( "binary" ) )
      {

        object.getHeader().addAttribute( "binary", ( int32_t )1 );

      }
      else
      {

        object.getHeader()[ "binary" ] = ( int32_t )1;

      }

    }
    else
    {

      if ( !object.getHeader().hasAttribute( "binary" ) )
      {

        object.getHeader().addAttribute( "binary", ( int32_t )0 );

      }
      else
      {

        object.getHeader()[ "binary" ] = ( int32_t )0;

      }

    }

    // adding 'space_dimension' attribute
    if ( !object.getHeader().hasAttribute( "space_dimension" ) )
    {

      object.getHeader().addAttribute( "space_dimension", ( int32_t )3 );

    }
    else
    {

      object.getHeader()[ "space_dimension" ] = ( int32_t )3;

    }

    // adding 'data_file_name' attribute
    if ( !object.getHeader().hasAttribute( "data_file_name" ) )
    {

      object.getHeader().addAttribute( "data_file_name",
                                       std::string( "*.bundlesdata" ) );

    }
    else
    {

      object.getHeader()[ "data_file_name" ] = std::string( "*.bundlesdata" );

    }

    // adding 'curves_count' and 'bundles' attributes
    std::vector< std::string > labels;
    std::vector< int32_t > curve3dCounts;

    object.getHeader().getAttribute( "labels", labels );
    object.getHeader().getAttribute( "curve3d_counts", curve3dCounts );

    gkg::GenericObjectList genericObjectList( labels.size() * 2U );
    int32_t curvesCount = 0;

    std::vector< std::string >::const_iterator
      l = labels.begin(),
      le = labels.end();
    std::vector< int32_t >::const_iterator
      c = curve3dCounts.begin();
    gkg::GenericObjectList::iterator
      g = genericObjectList.begin();
    while ( l != le )
    {


      *g = gkg::GenericObject( *l );
      ++ g;
      *g = gkg::GenericObject( curvesCount );
      ++ g;

      curvesCount += *c;

      ++ l;
      ++ c;

    }

    if ( !object.getHeader().hasAttribute( "bundles" ) )
    {

      object.getHeader().addAttribute( "bundles", genericObjectList );

    }
    else
    {

      object.getHeader()[ "bundles" ] = genericObjectList;

    }
    if ( !object.getHeader().hasAttribute( "curves_count" ) )
    {

      object.getHeader().addAttribute( "curves_count", curvesCount );

    }
    else
    {

      object.getHeader()[ "curves_count" ] = curvesCount;

    }

    // writing header(s) meta-information
    writeHeader( name, object );

    // writing core data
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".bundlesdata" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".bundlesdata" );

    }

    if ( ascii )
    {

      // looping over bundle(s)
      gkg::BundleMap< std::string >::const_iterator
        b = object.begin(),
        be = object.end();
      while ( b != be )
      {

        // looping over 3D curve(s)
        gkg::BundleMap< std::string >::Bundle::const_iterator
          c = b->second.begin(),
          ce = b->second.end();
        while ( c != ce )
        {

          // writing 3D point count
          os << c->getPointCount() << " ";

          // looping over 3D point(s)
          gkg::LightCurve3d< float >::const_iterator
            p = c->begin(),
            pe = c->end();
          while ( p != pe )
          {

            // writing current 3D point
            os << p->x << " "
               << p->y << " "
               << p->z << ";";
            ++ p;

          }
          os << std::endl;
          ++ c;

        }
        ++ b;

      }

    }
    else
    {

      gkg::ItemWriter< int32_t >& itemW_int32_t =
        gkg::DefaultItemIOFactory< int32_t >::getInstance().getWriter( false,
                                                                       false );
      gkg::ItemWriter< float >& itemW_float =
        gkg::DefaultItemIOFactory< float >::getInstance().getWriter( false,
                                                                     false );
      // looping over bundle(s)
      gkg::BundleMap< std::string >::const_iterator
        b = object.begin(),
        be = object.end();
      while ( b != be )
      {

        // looping over 3D curve(s)
        gkg::BundleMap< std::string >::Bundle::const_iterator
          c = b->second.begin(),
          ce = b->second.end();
        while ( c != ce )
        {

          // writing 3D point count
          itemW_int32_t.write( os, c->getPointCount() );

          // looping over 3D point(s)
          gkg::LightCurve3d< float >::const_iterator
            p = c->begin(),
            pe = c->end();
          while ( p != pe )
          {

            // writing current 3D point
            itemW_float.write( os, p->x );
            itemW_float.write( os, p->y );
            itemW_float.write( os, p->z );
            ++ p;

          }
          ++ c;

        }
        ++ b;

      }

    }
 
    os.close();

  }
  GKG_CATCH( "void gkg::AimsBundleMapDiskFormat::write( "
             "const std::string& "
             "name, gkg::BundleMap< std::string& object, "
             "bool ascii ) const" );

}


bool gkg::AimsBundleMapDiskFormat::hasReader() const
{

  return true;

}


bool gkg::AimsBundleMapDiskFormat::hasWriter() const
{

  return true;

}


std::string gkg::AimsBundleMapDiskFormat::getHeaderExtension() const
{

  return ".bundles";

}


//
// registrating Aims BundleMap disk format for "BundleMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMap< std::string > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsBundleMapDiskFormat::getInstance().getName(),
         &gkg::AimsBundleMapDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "Aims BundleMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
