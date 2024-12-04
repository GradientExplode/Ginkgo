#ifndef _gkg_processing_io_MeshMapDiskFormat_i_h_
#define _gkg_processing_io_MeshMapDiskFormat_i_h_


#include <gkg-processing-io/MeshMapDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-processing-io/SiteMapDiskFormat_i.h>
#include <gkg-processing-io/PolygonMapDiskFormat.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMapDiskFormat< R, S, D, Compare >::MeshMapDiskFormat()
                                           : gkg::TypedDiskFormat<
                                            gkg::MeshMap< R, S, D, Compare > >()
{

  this->_extensions.insert( getHeaderExtension() );

}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMapDiskFormat< R, S, D, Compare >::~MeshMapDiskFormat()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
std::string gkg::MeshMapDiskFormat< R, S, D, Compare >::getName() const
{

  return "meshmap";

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMapDiskFormat< R, S, D, Compare >::readHeader(
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
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "MeshMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshMapDiskFormat< R, S, D, Compare >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMapDiskFormat< R, S, D, Compare >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshMapDiskFormat< R, S, D, Compare >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMapDiskFormat< R, S, D, Compare >::read(
                                const std::string& name,
                                gkg::MeshMap< R, S, D, Compare >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // getting vertex map file name and reading it
    std::string vertexMapFileName;
    if ( object.getHeader().hasAttribute( "vertex_map" ) )
    {

      object.getHeader().getAttribute( "vertex_map", vertexMapFileName );
      gkg::SiteMapDiskFormat< R, S, Compare >::getInstance().read( 
                                                              vertexMapFileName,
                                                              object.vertices );

    }
    else
    {

      std::runtime_error( "missing 'vertex_map' attribute" );

    }

    // getting polygon map file name and reading it
    std::string polygonMapFileName;
    if ( object.getHeader().hasAttribute( "polygon_map" ) )
    {

      object.getHeader().getAttribute( "polygon_map", polygonMapFileName );
      gkg::PolygonMapDiskFormat< D >::getInstance().read( polygonMapFileName,
                                                          object.polygons );

    }
    else
    {

      std::runtime_error( "missing 'polygon_map' attribute" );

    }

    // sanity checks
    // checking that maximum index in polygons is not greater than maximum
    // rank/site LUT index 

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshMapDiskFormat< R, S, D, Compare >::read( "
             "const std::string& name, "
             "gkg::MeshMap< R, S, D, Compare >& object ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMapDiskFormat< R, S, D, Compare >::write( 
                                       const std::string& name,
                                       gkg::MeshMap< R, S, D, Compare >& object,
                                       bool ascii ) const
{

  try
  {

    // setting vertex map file name
    if ( object.getHeader().hasAttribute( "vertex_map" ) )
    {

      object.getHeader()[ "vertex_map" ] = 
        this->getNameWithoutExtension( name );
      
    }
    else
    {

      object.getHeader().addAttribute( "vertex_map",
                                       this->getNameWithoutExtension( name ) );

    }

    // setting polygon map file name
    if ( object.getHeader().hasAttribute( "polygon_map" ) )
    {

      object.getHeader()[ "polygon_map" ] = 
        this->getNameWithoutExtension( name );
      
    }
    else
    {

      object.getHeader().addAttribute( "polygon_map",
                                       this->getNameWithoutExtension( name ) );

    }

    // writing header(s) meta-information
    writeHeader( name, object );

    // writing vertex map
    gkg::SiteMapDiskFormat< R, S, Compare >::getInstance().write( 
                                          this->getNameWithoutExtension( name ),
                                          object.vertices,
                                          ascii );

    // writing polygon map
    gkg::PolygonMapDiskFormat< D >::getInstance().write( 
                                          this->getNameWithoutExtension( name ),
                                          object.polygons,
                                          ascii );

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshMapDiskFormat< R, S, D, Compare >::write( "
             "const std::string& name, "
             "gkg::MeshMap< R, S, D, Compare >& object, "
             "bool ascii ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
bool gkg::MeshMapDiskFormat< R, S, D, Compare >::hasReader() const
{

  return true;

}


template < class R, class S, uint32_t D, class Compare >
inline
bool gkg::MeshMapDiskFormat< R, S, D, Compare >::hasWriter() const
{

  return true;

}


template < class R, class S, uint32_t D, class Compare >
inline
std::string 
gkg::MeshMapDiskFormat< R, S, D, Compare >::getHeaderExtension() const
{

  return std::string( ".meshmap" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
