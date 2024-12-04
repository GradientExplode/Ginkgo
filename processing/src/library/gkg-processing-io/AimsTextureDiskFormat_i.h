#ifndef _gkg_processing_io_AimsTextureDiskFormat_i_h_
#define _gkg_processing_io_AimsTextureDiskFormat_i_h_


#include <gkg-processing-io/AimsTextureDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-io/DefaultItemIOFactoryOfStdList_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <fstream>
#include <list>


template < class T >
inline
gkg::AimsTextureDiskFormat< T >::AimsTextureDiskFormat()
                                : gkg::TypedDiskFormat< gkg::TextureMap< T > >()
{

  this->_extensions.insert( ".tex" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class T >
inline
gkg::AimsTextureDiskFormat< T >::~AimsTextureDiskFormat()
{
}


template < class T >
inline
std::string gkg::AimsTextureDiskFormat< T >::getName() const
{

  return "aimstexture";

}


template < class T >
inline
void gkg::AimsTextureDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all SiteMap Mesh specific attributes
    object.addSemantic( "__aimstexture__", "aims_rank_count",
            gkg::Semantic( gkg::TypeOf< int32_t >::getName(),
                           false ) );
    object.addSemantic( "__aimstexture__", "aims_rank",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(),
                           false ) );
    object.addSemantic( "__aimstexture__", "aims_item_count",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(),
                           false ) );

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
    if ( object.getHeader().hasAttribute( "object_type" ) )
    {

      object.getHeader().getAttribute( "object_type", objectType );
      if ( objectType == "Texture" )
      {

        objectType = "TextureMap";

      }

    }
    if ( objectType != "TextureMap" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    std::ifstream is( ( this->getNameWithoutExtension( name ) + 
                      ".tex" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) +
                                        ".tex" );

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

    // checking that texture is of good type
    std::string textureTypeName;
    if ( !ascii )
    {

      uint32_t textureTypeNameSize;
      itemR_uint32_t.read( is, textureTypeNameSize );
      char textureTypeNameChar[ 128 ];
      is.read( textureTypeNameChar, textureTypeNameSize );
      textureTypeNameChar[ textureTypeNameSize ] = '\0';
      textureTypeName = ( std::string )textureTypeNameChar;

    }
    else
    {

      is >> textureTypeName;

    }

    if ( textureTypeName == "S8" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< int8_t >::getName() );

    }
    else if ( textureTypeName == "U8" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< uint8_t >::getName() );

    }
    else if ( textureTypeName == "S16" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< int16_t >::getName() );

    }
    else if ( textureTypeName == "U16" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< uint16_t >::getName() );

    }
    else if ( textureTypeName == "S32" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< int32_t >::getName() );

    }
    else if ( textureTypeName == "U32" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< uint32_t >::getName() );

    }
    else if ( textureTypeName == "S64" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< int64_t >::getName() );

    }
    else if ( textureTypeName == "U64" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< uint64_t >::getName() );

    }
    else if ( textureTypeName == "FLOAT" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< float >::getName() );

    }
    else if ( textureTypeName == "DOUBLE" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< double >::getName() );

    }
    else if ( textureTypeName == "CFLOAT" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< 
                                           std::complex< float > >::getName() );

    }
    else if ( textureTypeName == "CDOUBLE" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf< 
                                          std::complex< double > >::getName() );

    }
    else if ( textureTypeName == "RGB" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf<
                                               gkg::RGBComponent >::getName() );

    }
    else if ( textureTypeName == "POINT3DF" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf<
                                          gkg::Vector3d< float > >::getName() );

    }
    else if ( textureTypeName == "POINT3DD" )
    {

      object.getHeader().addAttribute( "item_type",
                                       gkg::TypeOf<
                                         gkg::Vector3d< double > >::getName() );

    }


    is.close();

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::AimsTextureDiskFormat< T >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class T >
inline
void gkg::AimsTextureDiskFormat< T >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    // sanity checks
    int32_t rankCount;
    std::vector< int32_t > rank;
    std::vector< int32_t > itemCount;
    int32_t globalItemCount = getRankInformation( object,
                                                  rankCount,
                                                  rank,
                                                  itemCount );

    int32_t tmpGlobalItemCount = 0;
    object.getHeader().getAttribute( "item_count", tmpGlobalItemCount );

    if ( tmpGlobalItemCount != globalItemCount )
    {

      throw std::runtime_error(
        "inconsistent item count between header and rank information" );

    }

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::AimsTextureDiskFormat< T >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class T >
inline
void gkg::AimsTextureDiskFormat< T >::read(
                               const std::string& name,
                               gkg::TextureMap< T >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // reading core data <*.mesh>
    std::ifstream is( ( this->getNameWithoutExtension( name ) + 
                      ".tex" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) +
                                        ".tex" );

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
    gkg::ItemReader< T >& itemR_T =
      gkg::DefaultItemIOFactory< T >::getInstance().getReader( ascii,
                                                               bswap );

    // checking that texture is of good type
    std::string textureTypeName;
    if ( !ascii )
    {

      uint32_t textureTypeNameSize;
      itemR_uint32_t.read( is, textureTypeNameSize );
      char textureTypeNameChar[ 128 ];
      is.read( textureTypeNameChar, textureTypeNameSize );
      textureTypeNameChar[ textureTypeNameSize ] = '\0';
      textureTypeName = ( std::string )textureTypeNameChar;

    }
    else
    {

      is >> textureTypeName;

    }
    if ( textureTypeName != getTypeName() )
    {

      throw std::runtime_error( "incompatible texture item type(s)" );

    }

    // reading rank count
    uint32_t rankCount;
    itemR_uint32_t.read( is, rankCount );


    uint32_t r, t;
    T textureItem;
    std::list< T > textures;
    for ( r = 0; r < rankCount; r++ )
    {

      // reading rank value
      uint32_t rank;
      itemR_uint32_t.read( is, rank );

      // reading item count for current rank
      uint32_t itemCount;
      itemR_uint32_t.read( is, itemCount );

      // reading items
      for ( t = 0; t < itemCount; t++ )
      {

        itemR_T.read( is, textureItem );
        textures.push_back( textureItem );

      }

    }
    object.addTextures( textures );

    is.close();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::AimsTextureDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::TextureMap< T >& object ) const" );

}


template < class T >
inline
void gkg::AimsTextureDiskFormat< T >::write( 
                                      const std::string& name,
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

    // writing core data <*.mesh>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".tex" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".tex" );

    }

    // getting item writers
    gkg::ItemWriter< uint32_t >& itemW_uint32_t =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( ascii,
                                                                      false );
    gkg::ItemWriter< T >& itemW_T =
      gkg::DefaultItemIOFactory< T >::getInstance().getWriter( ascii,
                                                               false );

    if ( ascii )
    {

      os << "ascii" << std::endl;

    }
    else
    {

      os << "binar";

    }

    std::string typeName = getTypeName();

    if ( !ascii )
    {

      uint32_t magicNumber = 0x41424344;
      os.write( ( const char* )&magicNumber, sizeof( uint32_t ) );
      itemW_uint32_t.write( os, ( uint32_t )typeName.length() );
      os << typeName;

    }
    else
    {

      os << typeName << std::endl;

    }

    // collecting rank information
    int32_t rankCount;
    std::vector< int32_t > rank;
    std::vector< int32_t > itemCount;
    getRankInformation( object, rankCount, rank, itemCount );

    // writing rank count
    itemW_uint32_t.write( os, ( uint32_t )rankCount );

    // looping over ranks
    typename gkg::TextureMap< T >::const_iterator it = object.begin();

    int r, t;
    for ( r = 0; r < rankCount; r++ )
    {

      // writing rank value
      itemW_uint32_t.write( os, ( uint32_t )rank[ r ] );

      // writing item count for current rank
      itemW_uint32_t.write( os, ( uint32_t )itemCount[ r ] );

      // writing items
      for ( t = 0; t < itemCount[ r ]; t++ )
      {

        itemW_T.write( os, it->second );
        ++ it;

      }

    }

    os.close();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::AimsTextureDiskFormat< T >::write( "
             "const std::string& name, "
             "gkg::TextureMap< T >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
bool gkg::AimsTextureDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
inline
bool gkg::AimsTextureDiskFormat< T >::hasWriter() const
{

  return true;

}


template < class T >
inline
std::string 
gkg::AimsTextureDiskFormat< T >::getHeaderExtension() const
{

  return std::string( ".tex" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


template < class T >
inline
std::string gkg::AimsTextureDiskFormat< T >::getTypeName() const
{

  return "";

}


template < class T >
inline
int32_t gkg::AimsTextureDiskFormat< T >::getRankInformation(
                                       const gkg::HeaderedObject& object,
                                       int32_t& rankCount,
                                       std::vector< int32_t >& rank,
                                       std::vector< int32_t >& itemCount ) const
{

  try
  {

    int32_t globalItemCount = 0;

    // in case not AIMS info is provided, we assume there is only one rank
    // with zero value, and all items are attributed in the rank
    if ( ( object.getHeader().hasAttribute( "aims_rank_count" ) == false ) &&
         ( object.getHeader().hasAttribute( "aims_rank" ) == false ) &&
         ( object.getHeader().hasAttribute( "aims_item_count" ) == false ) )
    {

      rankCount = 1;
      rank.resize( rankCount, 0 );
      object.getHeader().getAttribute( "item_count", globalItemCount );
      itemCount.resize( rankCount );
      itemCount[ 0 ] = globalItemCount;

    }
    else
    {

      // checking that rank count exists and is valid
      rankCount = 0;
      if ( object.getHeader().hasAttribute( "aims_rank_count" ) )
      {

        try
        {

          object.getHeader().getAttribute( "aims_rank_count", rankCount );

        }
        catch ( std::exception& ) 
        {

          double rankCountDouble = 0.0;
          object.getHeader().getAttribute( "aims_rank_count", rankCountDouble );
          rankCount = ( int32_t )( rankCountDouble + 0.5 );

        }
        if ( rankCount < 0 )
        {

          throw std::runtime_error( "invalid AIMS rank count" );

        }

      }
      else
      {

         throw std::runtime_error( "rank count not present" );

      }

      if ( rankCount > 0 )
      {

        rank.resize( rankCount );
        itemCount.resize( rankCount );

        // checking that rank(s) exist and are valid
        if ( object.getHeader().hasAttribute( "aims_rank" ) )
        {

          try
          {

            object.getHeader().getAttribute( "aims_rank", rank );

          }
          catch( std::exception& )
          {

            gkg::GenericObjectList genericObjectList( rankCount );
            object.getHeader().getAttribute( "aims_rank", genericObjectList );
            int32_t i;
            for ( i = 0; i < rankCount; i++ )
            {

              rank[ i ] = ( int32_t )genericObjectList[ i ]->getScalar();

            }

          }

        }
        else
        {

          throw std::runtime_error( "rank not present" );

        }

        // checking that item counts exist and are valid
        if ( object.getHeader().hasAttribute( "aims_item_count" ) )
        {

          try
          {

            object.getHeader().getAttribute( "aims_item_count", itemCount );

          }
          catch( std::exception& )
          {

            gkg::GenericObjectList genericObjectList( rankCount );
            object.getHeader().getAttribute( "aims_item_count",
                                             genericObjectList );
            int32_t i;
            for ( i = 0; i < rankCount; i++ )
            {

              itemCount[ i ] = ( int32_t )genericObjectList[ i ]->getScalar();

            }

          }

          int32_t i;
          for ( i = 0; i < rankCount; i++ )
          {

            if ( itemCount[ i ] <= 0 )
            {

              throw std::runtime_error( "invalid AIMS item count" );

            }
            globalItemCount += itemCount[ i ];

          }

        }
        else
        {

          throw std::runtime_error( "item count not present" );

        }

      }

    }
    return globalItemCount;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::AimsTextureDiskFormat< T >::getRankInformation( "
             "const gkg::HeaderedObject& object, "
             "int32_t& rankCount, "
             "std::vector< int32_t >& rank, "
             "std::vector< int32_t >& itemCount )" );

}


namespace gkg
{


template <>
inline
std::string 
AimsTextureDiskFormat< int8_t >::getTypeName() const
{

  return "S8";

}


template <>
inline
std::string 
AimsTextureDiskFormat< uint8_t >::getTypeName() const
{

  return "U8";

}


template <>
inline
std::string 
AimsTextureDiskFormat< int16_t >::getTypeName() const
{

  return "S16";

}


template <>
inline
std::string 
AimsTextureDiskFormat< uint16_t >::getTypeName() const
{

  return "U16";

}


template <>
inline
std::string 
AimsTextureDiskFormat< int32_t >::getTypeName() const
{

  return "S32";

}


template <>
inline
std::string 
AimsTextureDiskFormat< uint32_t >::getTypeName() const
{

  return "U32";

}


template <>
inline
std::string 
AimsTextureDiskFormat< int64_t >::getTypeName() const
{

  return "S64";

}


template <>
inline
std::string 
AimsTextureDiskFormat< uint64_t >::getTypeName() const
{

  return "U64";

}


template <>
inline
std::string 
AimsTextureDiskFormat< float >::getTypeName() const
{

  return "FLOAT";

}


template <>
inline
std::string 
AimsTextureDiskFormat< double >::getTypeName() const
{

  return "DOUBLE";

}


template <>
inline
std::string 
AimsTextureDiskFormat< bool >::getTypeName() const
{

  return "BOOLEAN";

}


template <>
inline
std::string 
AimsTextureDiskFormat< std::complex< float > >::getTypeName() const
{

  return "CFLOAT";

}


template <>
inline
std::string 
AimsTextureDiskFormat< std::complex< double > >::getTypeName() const
{

  return "CDOUBLE";

}


template <>
inline
std::string 
AimsTextureDiskFormat< gkg::RGBComponent >::getTypeName() const
{

  return "RGB";

}


template <>
inline
std::string 
AimsTextureDiskFormat< gkg::Vector3d< float > >::getTypeName() const
{

  return "POINT3DF";

}


template <>
inline
std::string 
AimsTextureDiskFormat< gkg::Vector3d< double > >::getTypeName() const
{

  return "POINT3DD";

}


}

#endif
