#ifndef _gkg_processing_io_GisDiskFormat_i_h_
#define _gkg_processing_io_GisDiskFormat_i_h_


#include <gkg-processing-io/GisDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/RegionInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-exception/WrongFormatException.h>
#include <gkg-core-exception/FormatMismatchException.h>
#include <gkg-core-exception/InconsistentHeaderException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>


template < class T >
inline
gkg::GisDiskFormat< T >::GisDiskFormat()
                        : gkg::TypedDiskFormat< gkg::Volume< T > >()
{

  this->_extensions.insert( ".dim" );
  this->_extensions.insert( ".ima" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class T >
inline
gkg::GisDiskFormat< T >::~GisDiskFormat()
{
}


template < class T >
inline
std::string gkg::GisDiskFormat< T >::getName() const
{

  return "gis";

}


template < class T >
inline
void gkg::GisDiskFormat< T >::readHeader( const std::string& name,
                                          gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all GIS specific attributes
    object.addSemantic( "__gis__", "resolutionX",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gis__", "resolutionY",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gis__", "resolutionZ",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gis__", "resolutionT",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gis__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__gis__", "io_mode",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );

    bool hasStdHeader = false;

    // reading the meta-information header
    try
    {

      this->gkg::DiskFormat::readHeader( name, object );
      hasStdHeader = true;

      // compatibility between AIMS and GKG : adding the following fields 
      // when reading data generated from AIMS command lines
      // - data_type -> item_type
      // - volume_dimension -> sizeX, sizeY, sizeZ, sizeT
      // - voxel_size -> resolutionX, resolutionY, resolutionZ, resolutionT
      if ( object.getHeader().hasAttribute( "data_type" ) &&
           !object.getHeader().hasAttribute( "item_type" ) )
      {

        std::string data_type;
        object.getHeader().getAttribute( "data_type", data_type );
        if ( data_type == getTypeName() )
        {

          object.getHeader().addAttribute( "item_type",
                                           gkg::TypeOf< T >::getName() );

        }
        else
        {

          throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                        getHeaderExtension() );

        }

      }
      if ( object.getHeader().hasAttribute( "volume_dimension" ) &&
           ( !object.getHeader().hasAttribute( "sizeX" ) ||
             !object.getHeader().hasAttribute( "sizeY" ) ||
             !object.getHeader().hasAttribute( "sizeZ" ) ||
             !object.getHeader().hasAttribute( "sizeT" ) ) )
      {

        gkg::GenericObjectList volume_dimension;
        object.getHeader().getAttribute( "volume_dimension", volume_dimension );
        object.getHeader().addAttribute(
                      "sizeX",
                      ( int32_t )( volume_dimension[ 0 ]->getScalar() + 0.5 ) );
        object.getHeader().addAttribute(
                      "sizeY",
                      ( int32_t )( volume_dimension[ 1 ]->getScalar() + 0.5 ) );
        object.getHeader().addAttribute(
                      "sizeZ",
                      ( int32_t )( volume_dimension[ 2 ]->getScalar() + 0.5 ) );
        object.getHeader().addAttribute(
                      "sizeT",
                      ( int32_t )( volume_dimension[ 3 ]->getScalar() + 0.5 ) );

      }
      if ( object.getHeader().hasAttribute( "voxel_size" ) &&
           ( !object.getHeader().hasAttribute( "resolutionX" ) ||
             !object.getHeader().hasAttribute( "resolutionY" ) ||
             !object.getHeader().hasAttribute( "resolutionZ" ) ||
             !object.getHeader().hasAttribute( "resolutionT" ) ) )
      {

        gkg::GenericObjectList voxel_size;
        object.getHeader().getAttribute( "voxel_size", voxel_size );
        object.getHeader().addAttribute(
                      "resolutionX", voxel_size[ 0 ]->getScalar() );
        object.getHeader().addAttribute(
                      "resolutionY", voxel_size[ 1 ]->getScalar() );
        object.getHeader().addAttribute(
                      "resolutionZ", voxel_size[ 2 ]->getScalar() );
        object.getHeader().addAttribute(
                      "resolutionT", voxel_size[ 3 ]->getScalar() );

      }

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

    // checking that it is a volume
    std::string tmpObjectType;
    object.getHeader().getAttribute( "object_type", tmpObjectType );
    if ( tmpObjectType != "Volume" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good item type
    std::string tmpItemType;
    object.getHeader().getAttribute( "item_type", tmpItemType );
    if ( ! gkg::TypeOf< T >::isSameType( tmpItemType ) )
    {

      throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                    getHeaderExtension() );

    }

    // reading the <*.dim> header
    std::ifstream is( ( this->getNameWithoutExtension( name ) + ".dim" ).c_str()
                     );

    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) + 
                                        ".dim" );

    }
    std::string token = "";
    is >> token;

    if ( token.empty() )
    {

      throw gkg::WrongFormatException( this->getNameWithoutExtension( name ) +
                                       ".dim" );

    }

    int32_t sizeX = 1;
    int32_t sizeY = 1;
    int32_t sizeZ = 1;
    int32_t sizeT = 1;

    if ( !gkg::StringConverter::toScalar( token, sizeX ) )
    {

      throw gkg::WrongFormatException( this->getNameWithoutExtension( name ) +
                                       ".dim" );

    }

    is >> token;

    if ( gkg::StringConverter::toScalar( token, sizeY ) )
    {

      is >> token; 
      if ( gkg::StringConverter::toScalar( token, sizeZ ) )
      {

        is >> token;
        if ( gkg::StringConverter::toScalar( token, sizeT ) )
        {

          is >> token;

        }
        else
        {

          sizeT = 1;

        }

      }
      else
      {

        sizeZ = 1;

      }

    }
    else
    {

      sizeY = 1;

    }

    std::string itemType = "";
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    double resolutionT = 1.0;
    gkg::ByteOrder::Type byteOrderType =
      gkg::ByteOrder::getInstance().getCurrentType();
    gkg::IOMode::Type ioModeType = gkg::IOMode::Binary;

    while ( is )
    {

      if ( token == "-type" )
      {

        is >> itemType;

      }
      else if ( token == "-dx" )
      {

        is >> resolutionX;

      }
      else if ( token == "-dy" )
      {

        is >> resolutionY;

      }
      else if ( token == "-dz" )
      {

        is >> resolutionZ;

      }
      else if ( token == "-dt" )
      {

        is >> resolutionT;

      }
      else if ( token == "-bo" )
      {

        std::string byteOrderABCDCode;
        is >> byteOrderABCDCode;
        byteOrderType = gkg::ByteOrder::getInstance().getTypeFromABCDCode( 
                                                         byteOrderABCDCode );

      }
      else if ( token == "-om" )
      {

        std::string ioModeName;
        is >> ioModeName;
        ioModeType = gkg::IOMode::getInstance().getTypeFromName( ioModeName );

      }
      else
      {

        throw gkg::FormatMismatchException(
                        std::string( "GIS header token " ) +
                        token + " not in proper format",
                        this->getNameWithoutExtension( name ) + ".dim" );

      }

      if ( is )
      {

        is >> token;

      }

    }

    is.close();

    // checking that it is the good GIS type
    if ( itemType != getTypeName() )
    {

      throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                    ".dim" );

    }

    // consistency checks between meta-information and <*.dim>
    if ( hasStdHeader )
    {

      int32_t tmpSizeX;
      object.getHeader().getAttribute( "sizeX", tmpSizeX );
      if ( tmpSizeX != sizeX )
      {

        throw gkg::InconsistentHeaderException(
                      "sizeX", this->getNameWithoutExtension( name ) + ".dim" );

      }

      int32_t tmpSizeY;
      object.getHeader().getAttribute( "sizeY", tmpSizeY );
      if ( tmpSizeY != sizeY )
      {

        throw gkg::InconsistentHeaderException(
                      "sizeY", this->getNameWithoutExtension( name ) + ".dim" );

      }

      int32_t tmpSizeZ;
      object.getHeader().getAttribute( "sizeZ", tmpSizeZ );
      if ( tmpSizeZ != sizeZ )
      {

        throw gkg::InconsistentHeaderException(
                      "sizeZ", this->getNameWithoutExtension( name ) + ".dim" );

      }

      int32_t tmpSizeT;
      object.getHeader().getAttribute( "sizeT", tmpSizeT );
      if ( tmpSizeT != sizeT )
      {

        throw gkg::InconsistentHeaderException(
                      "sizeT", this->getNameWithoutExtension( name ) + ".dim" );

      }

      if ( object.getHeader().hasAttribute( "resolutionX" ) )
      {

        double tmpResolutionX;
        object.getHeader().getAttribute( "resolutionX", tmpResolutionX );
        if ( !gkg::equal( tmpResolutionX, resolutionX, 1e-3 ) )
        {

          throw gkg::InconsistentHeaderException(
                "resolutionX", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

      if ( object.getHeader().hasAttribute( "resolutionY" ) )
      {

        double tmpResolutionY;
        object.getHeader().getAttribute( "resolutionY", tmpResolutionY );
        if ( !gkg::equal( tmpResolutionY, resolutionY, 1e-3 ) )
        {

          throw gkg::InconsistentHeaderException(
                "resolutionY", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

      if ( object.getHeader().hasAttribute( "resolutionZ" ) )
      {

        double tmpResolutionZ;
        object.getHeader().getAttribute( "resolutionZ", tmpResolutionZ );
        if ( !gkg::equal( tmpResolutionZ, resolutionZ, 1e-3 ) )
        {

          throw gkg::InconsistentHeaderException(
                "resolutionZ", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

      if ( object.getHeader().hasAttribute( "resolutionT" ) )
      {

        double tmpResolutionT;
        object.getHeader().getAttribute( "resolutionT", tmpResolutionT );
        if ( !gkg::equal( tmpResolutionT, resolutionT, 1e-3 ) )
        {

          throw gkg::InconsistentHeaderException(
                "resolutionT", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

      if ( object.getHeader().hasAttribute( "byte_order" ) )
      {

        std::string tmpByteOrderName;
        object.getHeader().getAttribute( "byte_order", tmpByteOrderName );
        if ( gkg::ByteOrder::getInstance().getTypeFromName( tmpByteOrderName )
             != byteOrderType )
        {

          throw gkg::InconsistentHeaderException(
                 "byte_order", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

      if ( object.getHeader().hasAttribute( "io_mode" ) )
      {

        std::string tmpIOModeName;
        object.getHeader().getAttribute( "io_mode", tmpIOModeName );
        if ( gkg::IOMode::getInstance().getTypeFromName( tmpIOModeName ) !=
             ioModeType )
        {

          throw gkg::InconsistentHeaderException(
                    "io_mode", this->getNameWithoutExtension( name ) + ".dim" );

        }

      }

    }

    // putting the <*.dim> informations into the object header
    if ( !hasStdHeader )
    {

      object.getHeader()[ "sizeX" ] = sizeX;
      object.getHeader()[ "sizeY" ] = sizeY;
      object.getHeader()[ "sizeZ" ] = sizeZ;
      object.getHeader()[ "sizeT" ] = sizeT;

    }
    if ( !object.getHeader().hasAttribute( "resolutionX" ) )
    {

      object.getHeader().addAttribute( "resolutionX", resolutionX );

    }
    if ( !object.getHeader().hasAttribute( "resolutionY" ) )
    {

      object.getHeader().addAttribute( "resolutionY", resolutionY );

    }
    if ( !object.getHeader().hasAttribute( "resolutionZ" ) )
    {

      object.getHeader().addAttribute( "resolutionZ", resolutionZ );

    }
    if ( !object.getHeader().hasAttribute( "resolutionT" ) )
    {

      object.getHeader().addAttribute( "resolutionT", resolutionT );

    }
    if ( !object.getHeader().hasAttribute( "byte_order" ) )
    {

      object.getHeader().addAttribute( "byte_order", 
              gkg::ByteOrder::getInstance().getNameFromType( byteOrderType ) );

    }
    if ( !object.getHeader().hasAttribute( "io_mode" ) )
    {

      object.getHeader().addAttribute( "io_mode",
                    gkg::IOMode::getInstance().getNameFromType( ioModeType ) );

    }

    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

    // writing the <*.dim> header
    std::ofstream os( ( this->getNameWithoutExtension( name ) + ".dim" ).c_str()
                     );

    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".dim" );

    }

    const gkg::VolumeProxy< T >* volumeProxy =
                        dynamic_cast< const gkg::VolumeProxy< T >* >( &object );
    if ( !volumeProxy )
    {

      throw std::runtime_error( "object is not a volume" );

    }

    os << volumeProxy->getSizeX() << " "
       << volumeProxy->getSizeY() << " "
       << volumeProxy->getSizeZ() << " "
       << volumeProxy->getSizeT() << std::endl;
    os << "-type " << getTypeName() << std::endl;
       
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    double resolutionT = 1.0;

    if ( object.getHeader().hasAttribute( "resolutionX" ) )
    {

      object.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( object.getHeader().hasAttribute( "resolutionY" ) )
    {

      object.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( object.getHeader().hasAttribute( "resolutionZ" ) )
    {

      object.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    if ( object.getHeader().hasAttribute( "resolutionT" ) )
    {

      object.getHeader().getAttribute( "resolutionT", resolutionT );

    }
    os << "-dx " << resolutionX << " "
       << "-dy " << resolutionY << " "
       << "-dz " << resolutionZ << " "
       << "-dt " << resolutionT << std::endl;

    gkg::ByteOrder::Type byteOrderType = 
                                 gkg::ByteOrder::getInstance().getCurrentType();
    if ( object.getHeader().hasAttribute( "byte_order" ) )
    {

      std::string tmpByteOrderName;
      object.getHeader().getAttribute( "byte_order", tmpByteOrderName );
      byteOrderType = gkg::ByteOrder::getInstance().getTypeFromName( 
                                                             tmpByteOrderName );

    }

    os << "-bo " 
       << gkg::ByteOrder::getInstance().getABCDCodeFromType( byteOrderType )
       << std::endl;
    if ( object.getHeader().hasAttribute( "io_mode" ) )
    {

      std::string ioModeName;
      object.getHeader().getAttribute( "io_mode", ioModeName );

      switch ( gkg::IOMode::getInstance().getTypeFromName( ioModeName ) )
      {

        case gkg::IOMode::Ascii:

          os << "-om ascii" << std::endl;
          break;

        case gkg::IOMode::Binary:

          os << "-om binar" << std::endl;
          break;

      }

    }
    else
    {

      os << "-om binar" << std::endl;

    }

    os.close();

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::read( const std::string& name,
                                    gkg::Volume< T >& object ) const
{

  try
  {

    // preparing partial binary read
    gkg::VolumeProxy< T > proxy;
    preparePartialBinaryRead( name, proxy );

    // building bounding box
    gkg::BoundingBox< int32_t > boundingBox( 0, proxy.getSizeX() - 1,
                                             0, proxy.getSizeY() - 1,
                                             0, proxy.getSizeZ() - 1,
                                             0, proxy.getSizeT() - 1 );

    // reading whole volume
    partialBinaryRead( proxy, object, boundingBox );

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::read( "
             "const std::string& name, gkg::Volume< T >& object ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::write( const std::string& name,
                                     gkg::Volume< T >& object,
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

    // writing header(s) meta-information and <*.dim>
    writeHeader( name, object );


    // writing core data <*.ima>
    gkg::largefile_ofstream os( this->getNameWithoutExtension( name ) +
                                ".ima" );
    if ( !os.isOpened() )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".ima" );

    }
    gkg::ItemWriter< T >& itemW =
      gkg::DefaultItemIOFactory< T >::getInstance().getWriter( ascii, false );
    writeCore( os, itemW, object );
    os.close();

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::write( "
             "const std::string& name, gkg::Volume< T >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // reading header(s) meta-information and <*.dim>
      readHeader( name, volumeProxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName",
                                        this->getNameWithoutExtension( name ) );

      // getting IO mode
      std::string ioModeName;
      if ( volumeProxy.getHeader().hasAttribute( "io_mode" ) )
      {

        volumeProxy.getHeader().getAttribute( "io_mode", ioModeName );
        if ( gkg::IOMode::getInstance().getTypeFromName( ioModeName ) ==
             gkg::IOMode::Ascii )
        {

          throw std::runtime_error(
                          "partial read is limited to binary I/O mode volume" );

        }
  
      }

      // getting byte order
      std::string byteOrderName;
      if ( volumeProxy.getHeader().hasAttribute( "byte_order" ) )
      {

        volumeProxy.getHeader().getAttribute( "byte_order", byteOrderName );

        int32_t proxyBSwap = 
           ( gkg::ByteOrder::getInstance().getTypeFromName( byteOrderName ) !=
             gkg::ByteOrder::getInstance().getCurrentType() ? 1 : 0 );

        volumeProxy.getHeader().addAttribute( "proxyBSwap", proxyBSwap );
      
      }

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::partialBinaryRead(
                         const gkg::HeaderedObject& proxy,
                         gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {

    if ( &dynamic_cast< const gkg::VolumeProxy< T >& >( proxy ) )
    {

      const gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< const gkg::VolumeProxy< T >& >( proxy );

      if ( &dynamic_cast< gkg::Volume< T >& >( subpart ) )
      {

        gkg::Volume< T >&
          subVolume = dynamic_cast< gkg::Volume< T >& >( subpart );

        if ( &dynamic_cast< const gkg::BoundingBox< int32_t >& >( 
                                         regionInformation ) )
        {

          const gkg::BoundingBox< int32_t >& boundingBox = 
            dynamic_cast< const gkg::BoundingBox< int32_t >& >( 
	                               regionInformation );

          try
          {

            // getting byte swap info and proxy file name
            if ( !volumeProxy.getHeader().hasAttribute( "proxyBSwap" ) )
            {

               throw std::runtime_error( "cannot find 'proxyBSwap' attribute" );

            }
            if ( !volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
            {

               throw std::runtime_error(
                                      "cannot find 'proxyFileName' attribute" );

            }
            int32_t proxyBSwap = 0;
            volumeProxy.getHeader().getAttribute( "proxyBSwap",
                                                  proxyBSwap );
            std::string proxyFileName;
            volumeProxy.getHeader().getAttribute( "proxyFileName",
                                                  proxyFileName );

            // checking bounding box
            gkg::BoundingBox< int32_t >
              onDiskBoundingBox( 0, volumeProxy.getSizeX() - 1,
                                 0, volumeProxy.getSizeY() - 1,
                                 0, volumeProxy.getSizeZ() - 1,
                                 0, volumeProxy.getSizeT() - 1 );
            if ( !onDiskBoundingBox.contains( boundingBox ) )
            {

              throw std::runtime_error(
                                "bounding box is not contained inside volume" );

            }

            int32_t subSizeX = boundingBox.getUpperX() -
                               boundingBox.getLowerX() + 1;
            int32_t subSizeY = boundingBox.getUpperY() -
                               boundingBox.getLowerY() + 1;
            int32_t subSizeZ = boundingBox.getUpperZ() -
                               boundingBox.getLowerZ() + 1;
            int32_t subSizeT = boundingBox.getUpperT() -
                               boundingBox.getLowerT() + 1;

            // reallocating the sub-part
            subVolume.getHeader() = volumeProxy.getHeader();
            subVolume.reallocate( subSizeX, subSizeY, subSizeZ, subSizeT );

            // reading from core data <*.ima>
            gkg::largefile_ifstream
              is( this->getNameWithoutExtension( proxyFileName ) + ".ima" );
            if ( !is.isOpened() )
            {

              throw gkg::FileNotFoundException(
                      this->getNameWithoutExtension( proxyFileName ) + ".ima" );

            }
            gkg::ItemReader< T >& itemR =
              gkg::DefaultItemIOFactory< T >::getInstance().getReader(
                                         false, ( proxyBSwap ? true : false ) );
            readPartialBinaryCore( is,
                                   itemR,
                                   boundingBox,
                                   volumeProxy.getSizeX(),
                                   volumeProxy.getSizeXY(),
                                   volumeProxy.getSizeXYZ(),
                                   subVolume );
            is.close();

          }
          catch ( std::exception& e )
          {

            throw std::runtime_error( std::string( "volume sub-part: " ) +
                                      e.what() );

          }

        }

      }
      else
      {

        throw std::runtime_error( "not a valid sub-part" );

      }

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::"
             "partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::preparePartialBinaryWrite(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName",
                                        this->getNameWithoutExtension( name ) );

      // setting IO mode
      std::string ioModeName =
             gkg::IOMode::getInstance().getNameFromType( gkg::IOMode::Binary );
      if ( volumeProxy.getHeader().hasAttribute( "io_mode" ) )
      {

        volumeProxy.getHeader()[ "io_mode" ] = ioModeName;
      
      }
      else
      {

        volumeProxy.getHeader().addAttribute( "io_mode", ioModeName );

      }

      // setting byte order
      std::string
        byteOrderName = gkg::ByteOrder::getInstance().getCurrentName();
      if ( volumeProxy.getHeader().hasAttribute( "byte_order" ) )
      {

        volumeProxy.getHeader()[ "byte_order" ] = byteOrderName;
      
      }
      else
      {

        volumeProxy.getHeader().addAttribute( "byte_order", byteOrderName );

      }

      // writing header(s) meta-information and <*.dim>
      writeHeader( name, volumeProxy );

      // creating file on disk with suitable size
      gkg::largefile_ofstream os( this->getNameWithoutExtension( name ) +
                                  ".ima" );
      if ( !os.isOpened() )
      {

        throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                     ".ima" );

      }
      os.truncate(
                ( gkg::largefile_ofstream::off_type )volumeProxy.getSizeXYZT() *
                ( gkg::largefile_ofstream::off_type )sizeof( T ) );
      os.close();

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::"
             "preparePartialBinaryWrite( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::partialBinaryWrite(
                         const gkg::HeaderedObject& proxy,
                         const gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {


    if ( &dynamic_cast< const gkg::VolumeProxy< T >& >( proxy ) )
    {

      const gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< const gkg::VolumeProxy< T >& >( proxy );

      if ( &dynamic_cast< const gkg::Volume< T >& >( subpart ) )
      {

        const gkg::Volume< T >&
          subVolume = dynamic_cast< const gkg::Volume< T >& >( subpart );

        if ( &dynamic_cast< const gkg::BoundingBox< int32_t >& >(
                                           regionInformation ) )
        {

          const gkg::BoundingBox< int32_t >& boundingBox = 
            dynamic_cast< const gkg::BoundingBox< int32_t >& >( 
	                                 regionInformation );

          try
          {

            if ( !volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
            {

              throw std::runtime_error(
                                      "cannot find 'proxyFileName' attribute" );

            }
            std::string proxyFileName;
            volumeProxy.getHeader().getAttribute( "proxyFileName",
                                                  proxyFileName );

            // checking bounding box
            gkg::BoundingBox< int32_t >
              onDiskBoundingBox( 0, volumeProxy.getSizeX() - 1,
                                 0, volumeProxy.getSizeY() - 1,
                                 0, volumeProxy.getSizeZ() - 1,
                                 0, volumeProxy.getSizeT() - 1 );
            if ( !onDiskBoundingBox.contains( boundingBox ) )
            {

              throw std::runtime_error(
                                "bounding box is not contained inside volume" );

            }
            if ( volumeProxy.getHeader().hasAttribute( "resolutionX" ) &&
                 subVolume.getHeader().hasAttribute( "resolutionX" ) )
            {

              double onDiskResolutionX, subResolutionX;
              volumeProxy.getHeader().getAttribute( "resolutionX",
                                                    onDiskResolutionX );
              subVolume.getHeader().getAttribute( "resolutionX",
                                                  subResolutionX );
              if ( !gkg::equal( onDiskResolutionX, subResolutionX, 1e-5 ) )
              {

                throw std::runtime_error( "X resolution inconsistency between "
                                          "on disk volume and sub-volume" );

              }

            }
            if ( volumeProxy.getHeader().hasAttribute( "resolutionY" ) &&
                 subVolume.getHeader().hasAttribute( "resolutionY" ) )
            {

              double onDiskResolutionY, subResolutionY;
              volumeProxy.getHeader().getAttribute( "resolutionY",
                                                    onDiskResolutionY );
              subVolume.getHeader().getAttribute( "resolutionY",
                                                  subResolutionY );
              if ( !gkg::equal( onDiskResolutionY, subResolutionY, 1e-5 ) )
              {

                throw std::runtime_error( "Y resolution inconsistency between "
                                          "on disk volume and sub-volume" );

              }

            }
            if ( volumeProxy.getHeader().hasAttribute( "resolutionZ" ) &&
                 subVolume.getHeader().hasAttribute( "resolutionZ" ) )
            {

              double onDiskResolutionZ, subResolutionZ;
              volumeProxy.getHeader().getAttribute( "resolutionZ",
                                                    onDiskResolutionZ );
              subVolume.getHeader().getAttribute( "resolutionZ",
                                                  subResolutionZ );
              if ( !gkg::equal( onDiskResolutionZ, subResolutionZ, 1e-5 ) )
              {

                throw std::runtime_error( "Z resolution inconsistency between "
                                          "on disk volume and sub-volume" );

              }

            }
            if ( volumeProxy.getHeader().hasAttribute( "resolutionT" ) &&
                 subVolume.getHeader().hasAttribute( "resolutionT" ) )
            {

              double onDiskResolutionT, subResolutionT;
              volumeProxy.getHeader().getAttribute( "resolutionT",
                                                    onDiskResolutionT );
              subVolume.getHeader().getAttribute( "resolutionT",
                                                  subResolutionT );
              if ( !gkg::equal( onDiskResolutionT, subResolutionT, 1e-5 ) )
              {

                throw std::runtime_error( "T resolution inconsistency between "
                                          "on disk volume and sub-volume" );

              }

            }

            // sanity checks: subpart should be consistent with bounding box
            if ( ( subVolume.getSizeX() != boundingBox.getUpperX() -
                                           boundingBox.getLowerX() + 1 ) ||
                 ( subVolume.getSizeY() != boundingBox.getUpperY() -
                                           boundingBox.getLowerY() + 1 ) ||
                 ( subVolume.getSizeZ() != boundingBox.getUpperZ() -
                                           boundingBox.getLowerZ() + 1 ) ||
                 ( subVolume.getSizeT() != boundingBox.getUpperT() -
                                           boundingBox.getLowerT() + 1 ) )
            {

              throw std::runtime_error(
                                  "bounding box inconsistent with sub-volume" );

            }

            // writing to core data <*.ima>
            gkg::largefile_fstream
              fs( this->getNameWithoutExtension( proxyFileName ) + ".ima" );
            if ( !fs.isOpened() )
            {

              throw gkg::FileNotFoundException(
                      this->getNameWithoutExtension( proxyFileName ) + ".ima" );

            }
            gkg::ItemWriter< T >& itemW =
              gkg::DefaultItemIOFactory< T >::getInstance().getWriter( false,
                                                                       false );
            writePartialBinaryCore( fs,
                                    itemW,
                                    boundingBox,
                                    volumeProxy.getSizeX(),
                                    volumeProxy.getSizeXY(),
                                    volumeProxy.getSizeXYZ(),
                                    subVolume );
            fs.close();

          }
          catch ( std::exception& e )
          {

            throw std::runtime_error( std::string( "volume sub-part: " ) +
                                      e.what() );

          }

        }

      }
      else
      {

        throw std::runtime_error( "not a valid sub-part" );

      }

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::"
             "partialBinaryWrite( "
             "const gkg::HeaderedObject& proxy, "
             "const gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
inline
bool gkg::GisDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
inline
bool gkg::GisDiskFormat< T >::hasWriter() const
{

  return true;

}


template < class T >
inline
std::string gkg::GisDiskFormat< T >::getHeaderExtension() const
{

  return std::string( ".ima" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


template < class T >
inline
std::string gkg::GisDiskFormat< T >::getTypeName() const
{

  return "";

}


template < class T >
inline
void gkg::GisDiskFormat< T >::writeCore( gkg::largefile_ofstream& os,
                                         const gkg::ItemWriter< T >& itemW,
                                         const gkg::Volume< T >& object ) const
{

  try
  {

    typename gkg::Volume< T >::const_iterator i = object.begin(),
                                              ie = object.end();

    if( object.isMemoryMapped() )
    {

      while ( i != ie )
      {

        itemW.write( os, &( *i ), ( size_t )1 );
        ++ i;

      }

    }
    else
    {

      while ( i != ie )
      {

        itemW.write( os, &( *i ), ( size_t )object.getSizeX() );
        i += ( size_t )object.getSizeX();

      }

    }

  }
  GKG_CATCH( "template < class T > void gkg::GisDiskFormat< T >::writeCore( "
             "gkg::largefile_ofstream& os, *"
             "const gkg::ItemWriter< T >& itemW,"
             "const gkg::Volume< T >& object ) const" );

}


template < class T >
inline
void gkg::GisDiskFormat< T >::readPartialBinaryCore(
                                 gkg::largefile_ifstream& is,
                                 const gkg::ItemReader< T >& itemR,
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 int32_t onDiskSizeX,
                                 int32_t onDiskSizeXY,
                                 int32_t onDiskSizeXYZ,
                                 gkg::Volume< T >& subVolume ) const
{

  typename gkg::Volume< T >::iterator s = subVolume.begin();
  int32_t x, y, z, t;

  for ( t = 0; t < subVolume.getSizeT(); t++ )
  {

    for ( z = 0; z < subVolume.getSizeZ(); z++ )
    {

      for ( y = 0; y < subVolume.getSizeY(); y++ )
      {

        is.seek( ( gkg::largefile_ifstream::off_type )
                 ( ( ( gkg::largefile_ifstream::off_type )
                      boundingBox.getLowerX() +
                 ( gkg::largefile_ifstream::off_type )
                   ( y + boundingBox.getLowerY() ) * 
                 ( gkg::largefile_ifstream::off_type )
                   onDiskSizeX +
                 ( gkg::largefile_ifstream::off_type )
                   ( z + boundingBox.getLowerZ() ) *
                 ( gkg::largefile_ifstream::off_type )
                   onDiskSizeXY +
                 ( gkg::largefile_ifstream::off_type )
                   ( t + boundingBox.getLowerT() ) *
                 ( gkg::largefile_ifstream::off_type )
                   onDiskSizeXYZ ) *
                   ( gkg::largefile_ifstream::off_type )sizeof( T ) ),
                 gkg::IosBase::beg );

        if( subVolume.isMemoryMapped() )
        {

          for ( x = 0; x < subVolume.getSizeX(); ++ x )
          {

            itemR.read( is, &( *s ), ( size_t )1 );
            ++ s;

          }

        }
        else
        {

          itemR.read( is, &( *s ), ( size_t )subVolume.getSizeX() );
          s += ( size_t )subVolume.getSizeX();

        }

      }

    }

  }

}


template < class T >
inline
void gkg::GisDiskFormat< T >::writePartialBinaryCore(
                                 gkg::largefile_fstream& fs,
                                 const gkg::ItemWriter< T >& itemW,
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 int32_t onDiskSizeX,
                                 int32_t onDiskSizeXY,
                                 int32_t onDiskSizeXYZ,
                                 const gkg::Volume< T >& subVolume ) const
{

  typename gkg::Volume< T >::const_iterator s = subVolume.begin();
  int32_t x, y, z, t;

  for ( t = 0; t < subVolume.getSizeT(); t++ )
  {

    for ( z = 0; z < subVolume.getSizeZ(); z++ )
    {

      for ( y = 0; y < subVolume.getSizeY(); y++ )
      {

        fs.seek( ( gkg::largefile_fstream::off_type )
                 ( ( ( gkg::largefile_fstream::off_type )
                      boundingBox.getLowerX() +
                 ( gkg::largefile_fstream::off_type )
                   ( y + boundingBox.getLowerY() ) * 
                 ( gkg::largefile_fstream::off_type )
                   onDiskSizeX +
                 ( gkg::largefile_fstream::off_type )
                   ( z + boundingBox.getLowerZ() ) *
                 ( gkg::largefile_fstream::off_type )
                   onDiskSizeXY +
                 ( gkg::largefile_fstream::off_type )
                   ( t + boundingBox.getLowerT() ) *
                 ( gkg::largefile_fstream::off_type )
                   onDiskSizeXYZ ) *
                   ( gkg::largefile_fstream::off_type )sizeof( T ) ),
                 gkg::IosBase::beg );

        if( subVolume.isMemoryMapped() )
        {

          for ( x = 0; x < subVolume.getSizeX(); ++ x )
          {

            itemW.write( fs, &( *s ), ( size_t )1 );
            ++ s;

          }

        }
        else
        {

          itemW.write( fs, &( *s ), ( size_t )subVolume.getSizeX() );
          s += ( size_t )subVolume.getSizeX();

        }

      }

    }

  }

}



// specializations


// boolean are managed as single bit, so we must get or put boolean items once 
// a time and we cannot use a pointer to boolean items
// boolean are managed as single bit, so we must write boolean items once a time
// and we cannot use a pointer to boolean items

namespace gkg
{


template <>
inline
void GisDiskFormat< bool >::writeCore( largefile_ofstream& os,
                                       const ItemWriter< bool >& itemW,
                                       const Volume< bool >& object ) const
{

  Volume< bool >::const_iterator i = object.begin(),
                                 ie = object.end();
  while ( i != ie )
  {

    itemW.write( os, *i );
    i ++;

  }

}


template <>
inline
void GisDiskFormat< bool >::readPartialBinaryCore(
                                      largefile_ifstream& is,
                                      const ItemReader< bool >& itemR,
                                      const BoundingBox< int32_t >& boundingBox,
                                      int32_t onDiskSizeX,
                                      int32_t onDiskSizeXY,
                                      int32_t onDiskSizeXYZ,
                                      Volume< bool >& subVolume ) const
{

  Volume< bool >::iterator s = subVolume.begin();

  int32_t x, y, z, t;
  for ( t = 0; t < subVolume.getSizeT(); t++ )
  {

    for ( z = 0; z < subVolume.getSizeZ(); z++ )
    {

      for ( y = 0; y < subVolume.getSizeY(); y++ )
      {

        is.seek( ( largefile_ifstream::off_type )
                 ( ( ( largefile_ifstream::off_type )
                      boundingBox.getLowerX() +
                 ( largefile_ifstream::off_type )
                   ( y + boundingBox.getLowerY() ) * 
                 ( largefile_ifstream::off_type )
                   onDiskSizeX +
                 ( largefile_ifstream::off_type )
                   ( z + boundingBox.getLowerZ() ) *
                 ( largefile_ifstream::off_type )
                   onDiskSizeXY +
                 ( largefile_ifstream::off_type )
                   ( t + boundingBox.getLowerT() ) *
                 ( largefile_ifstream::off_type )
                   onDiskSizeXYZ ) *
                   ( largefile_ifstream::off_type )sizeof( bool ) ),
                 gkg::IosBase::beg );
        for ( x = 0; x < subVolume.getSizeX(); x++ )
        {

          bool value;
          itemR.read( is, value );
          *s = value;
          ++ s;

        }

      }

    }

  }

}


template <>
inline
void GisDiskFormat< bool >::writePartialBinaryCore(
                                     largefile_fstream& fs,
                                     const ItemWriter< bool >& itemW,
                                     const BoundingBox< int32_t >& boundingBox,
                                     int32_t onDiskSizeX,
                                     int32_t onDiskSizeXY,
                                     int32_t onDiskSizeXYZ,
                                     const Volume< bool >& subVolume ) const
{

  Volume< bool >::const_iterator s = subVolume.begin();

  int32_t x, y, z, t;
  for ( t = 0; t < subVolume.getSizeT(); t++ )
  {

    for ( z = 0; z < subVolume.getSizeZ(); z++ )
    {

      for ( y = 0; y < subVolume.getSizeY(); y++ )
      {

        fs.seek( ( largefile_fstream::off_type )
                 ( ( ( largefile_fstream::off_type )
                      boundingBox.getLowerX() +
                 ( largefile_fstream::off_type )
                   ( y + boundingBox.getLowerY() ) * 
                 ( largefile_fstream::off_type )
                   onDiskSizeX +
                 ( largefile_fstream::off_type )
                   ( z + boundingBox.getLowerZ() ) *
                 ( largefile_fstream::off_type )
                   onDiskSizeXY +
                 ( largefile_fstream::off_type )
                   ( t + boundingBox.getLowerT() ) *
                 ( largefile_fstream::off_type )
                   onDiskSizeXYZ ) *
                   ( largefile_fstream::off_type )sizeof( bool ) ),
                 gkg::IosBase::beg );
        for ( x = 0; x < subVolume.getSizeX(); x++ )
        {

          bool value = *s;
          itemW.write( fs, value );
          ++ s;

        }

      }

    }

  }

}


template <>
inline
std::string GisDiskFormat< int8_t >::getTypeName() const
{

  return "S8";

}


template <>
inline
std::string GisDiskFormat< uint8_t >::getTypeName() const
{

  return "U8";

}


template <>
inline
std::string GisDiskFormat< int16_t >::getTypeName() const
{

  return "S16";

}


template <>
inline
std::string GisDiskFormat< uint16_t >::getTypeName() const
{

  return "U16";

}


template <>
inline
std::string GisDiskFormat< int32_t >::getTypeName() const
{

  return "S32";

}


template <>
inline
std::string GisDiskFormat< uint32_t >::getTypeName() const
{

  return "U32";

}


template <>
inline
std::string GisDiskFormat< int64_t >::getTypeName() const
{

  return "S64";

}


template <>
inline
std::string GisDiskFormat< uint64_t >::getTypeName() const
{

  return "U64";

}


template <>
inline
std::string GisDiskFormat< float >::getTypeName() const
{

  return "FLOAT";

}


template <>
inline
std::string GisDiskFormat< double >::getTypeName() const
{

  return "DOUBLE";

}


template <>
inline
std::string GisDiskFormat< bool >::getTypeName() const
{

  return "BOOLEAN";

}


template <>
inline
std::string GisDiskFormat< std::complex< float > >::getTypeName() const
{

  return "CFLOAT";

}


template <>
inline
std::string GisDiskFormat< std::complex< double > >::getTypeName() const
{

  return "CDOUBLE";

}


template <>
inline
std::string GisDiskFormat< gkg::RGBComponent >::getTypeName() const
{

  return "RGB";

}


template <>
inline
std::string GisDiskFormat< gkg::Vector3d< float > >::getTypeName() const
{

  return "POINT3DF";

}


template <>
inline
std::string GisDiskFormat< gkg::Vector3d< double > >::getTypeName() const
{

  return "POINT3DD";

}


}


#endif
