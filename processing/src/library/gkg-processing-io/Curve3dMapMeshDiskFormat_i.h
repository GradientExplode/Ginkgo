#ifndef _gkg_processing_io_Curve3dMapMeshDiskFormat_i_h_
#define _gkg_processing_io_Curve3dMapMeshDiskFormat_i_h_


#include <gkg-processing-io/Curve3dMapMeshDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-io/StdVectorItemIOFactory_i.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-exception/WrongFormatException.h>
#include <gkg-core-exception/FormatMismatchException.h>
#include <gkg-core-exception/InconsistentHeaderException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <fstream>


template < class T >
inline
gkg::Curve3dMapMeshDiskFormat< T >::Curve3dMapMeshDiskFormat()
                                   : gkg::TypedDiskFormat<
                                                        gkg::Curve3dMap< T > >()
{

  this->_extensions.insert( ".mesh" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class T >
inline
gkg::Curve3dMapMeshDiskFormat< T >::~Curve3dMapMeshDiskFormat()
{
}


template < class T >
inline
std::string gkg::Curve3dMapMeshDiskFormat< T >::getName() const
{

  return "curve3dmapmesh";

}


template < class T >
inline
void gkg::Curve3dMapMeshDiskFormat< T >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Curve3dMapMeshDiskFormat< T >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class T >
inline
void gkg::Curve3dMapMeshDiskFormat< T >::write( const std::string& name,
                                                gkg::Curve3dMap< T >& object,
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


    // writing core data
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".mesh" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".mesh" );

    }


    if ( ascii )
    {

      os << "ascii" << std::endl;

    }
    else
    {

      os << "binar";

    }

    gkg::ItemWriter< uint32_t >& itemW1 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( ascii,
                                                                      false );
    gkg::ItemWriter< std::vector< gkg::Vector3d< T > > >& itemW2 =
      gkg::StdVectorItemIOFactory< 
                 gkg::Vector3d< T > >::getInstance().getWriter( ascii, false );
    gkg::ItemWriter< std::vector< gkg::Vector2d< uint32_t > > >& itemW3 =
      gkg::StdVectorItemIOFactory< 
          gkg::Vector2d< uint32_t > >::getInstance().getWriter( ascii, false );


    if ( !ascii )
    {

      uint32_t magicNumber = 0x41424344;
      os.write( ( const char* )&magicNumber, sizeof( uint32_t ) );
      itemW1.write( os, ( uint32_t )4 );
      os << "VOID";                       // no texture
      itemW1.write( os, ( uint32_t )2 ); // 2 vertices polygons
      itemW1.write( os, ( uint32_t )1 ); // 1 time
      itemW1.write( os, ( uint32_t )0 ); // time=0

    }
    else
    {

      os << "VOID" << std::endl;          // no texture
      os << "2" << std::endl;             // 2 vertices polygons
      os << "1" << std::endl;             // 1 time
      os << "0" << std::endl;             // time=0

    }


    // estimating global point count
    int32_t globalPointCount = 0;
    int32_t c = 0;
    for ( c = 0; c < object.getCurve3dCount(); c++ )
    {

      globalPointCount += object.getCurve3d( c ).getPointCount();

    }

    // building global vector of points
    std::vector< Vector3d< T > > points( globalPointCount );
    int32_t p = 0;
    int32_t index = 0;
    for ( c = 0; c < object.getCurve3dCount(); c++ )
    {

      for ( p = 0; p < object.getCurve3d( c ).getPointCount(); p++ )
      {

        points[ index ] = object.getCurve3d( c ).getPoint( p );
        ++ index;

      }

    }
    
    // writing global vector of points
    itemW2.write( os, points );

    // adding information about normal and texture
    if ( !ascii )
    {

      itemW1.write( os, ( uint32_t )0 ); // zero normal
      itemW1.write( os, ( uint32_t )0 ); // zero texture

    }
    else
    {

      os << std::endl;
      os << "0" << std::endl;             // zero normal
      os << "0" << std::endl;             // zero texture

    }

    // computing global segment count
    int32_t globalSegmentCount = 0;
    for ( c = 0; c < object.getCurve3dCount(); c++ )
    {

      if ( object.getCurve3d( c ).isClosed() )
      {

        globalSegmentCount += object.getCurve3d( c ).getPointCount();

      }
      else
      {

        globalSegmentCount += object.getCurve3d( c ).getPointCount() - 1;

      }

    }
  
    // building global segment vector
    std::vector< gkg::Vector2d< uint32_t > > segments( globalSegmentCount );

    index = 0;
    int32_t offset = 0;
    for ( c = 0; c < object.getCurve3dCount(); c++ )
    {

      if ( object.getCurve3d( c ).isClosed() )
      {

        for ( p = 0; p < object.getCurve3d( c ).getPointCount(); p++ )
        {

          segments[ index ].x = ( uint32_t )( offset + p );
          segments[ index ].y = ( uint32_t )( offset + ( p + 1 ) %
                                       object.getCurve3d( c ).getPointCount() );
          ++ index;

        }
        offset += object.getCurve3d( c ).getPointCount();

      }
      else
      {

        for ( p = 0; p < object.getCurve3d( c ).getPointCount() - 1; p++ )
        {

          segments[ index ].x = ( uint32_t )( offset + p );
          segments[ index ].y = ( uint32_t )( offset + p + 1 );
          ++ index;

        }
        offset += object.getCurve3d( c ).getPointCount();

      }

    }
    itemW3.write( os, segments );


    os.close();

  }
  GKG_CATCH( "template < class T > void gkg::Curve3dMapMeshDiskFormat< T >::write( "
             "const std::string& name, gkg::Curve3d< T >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
bool gkg::Curve3dMapMeshDiskFormat< T >::hasWriter() const
{

  return true;

}


template < class T >
inline
std::string gkg::Curve3dMapMeshDiskFormat< T >::getHeaderExtension() const
{

  return std::string( ".mesh" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
