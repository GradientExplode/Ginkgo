#include <gkg-mri-reconstruction-io/GehcSignaGenesisDiskFormat.h>
#include <gkg-mri-reconstruction-io/GehcSignaGenesisHeader.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/RegionInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <complex>
#include <vector>
#include <fstream>
#include <map>


//
// some useful defines and macro(s)
//

#define REV8      1
#define REV9      2
#define REV_NONE  4


//
// class GehcSignaGenesisDiskFormat
//


template < class T >
gkg::GehcSignaGenesisDiskFormat< T >::GehcSignaGenesisDiskFormat()
                                   : gkg::TypedDiskFormat< gkg::Volume< T > >()
{
}


template < class T >
gkg::GehcSignaGenesisDiskFormat< T >::~GehcSignaGenesisDiskFormat()
{
}


template < class T >
std::string gkg::GehcSignaGenesisDiskFormat< T >::getName() const
{

  return "gehc-signa-genesis";

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // browsing and checking directory coherence
    gkg::Directory directory( name );

    if ( !directory.isValid() )
    {

      throw gkg::FileNotFoundException( name );

    }

    if ( directory.getCount() == 0 )
    {

      throw std::runtime_error( std::string( "no file in '" ) +
                                directory.getPath() + "' directory" );

    }

    // checking item type compatibility
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      std::string itemType;
      object.getHeader().getAttribute( "item_type", itemType );
      if ( ! gkg::TypeOf< T >::isSameType( itemType ) ) 
      {

        throw gkg::ItemTypeException(
          std::string( "object item type given as argument is different from " )
          + gkg::TypeOf< T >::getName() );

      }

    }

    // get byte order: rev. 7, rev. 8, rev. 9 -> SGI host => big endian
    bool bswap = false;
    float revision = -1.0;
    getBSwapAndRevision( name, bswap, revision );

    // looping over file, checking inter-location header consistency
    // and collecting slice information
    std::set< std::string > fileNames = directory.getFiles();
    std::set< std::string >::const_iterator f = fileNames.begin(),
                                            fe = fileNames.end();

    gkg::HeaderedObject commonHeaderObject;
    std::set< float > locations;
    std::multimap< float, std::string > locationToNameLut;
    std::map< std::string,
              typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo >
      nameToSliceInfoLut;

    while ( f != fe )
    {

      gkg::HeaderedObject headerObject;
      std::ifstream is( ( directory.getPath() + gkg::getDirectorySeparator() +
                          *f ).c_str() );

      typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo sliceInfo;

      if ( f == fileNames.begin() )
      {

        readCoreHeader( directory.getPath() + gkg::getDirectorySeparator() + *f,
                        commonHeaderObject,
                        sliceInfo, bswap, revision );

      }
      else
      {

        readCoreHeader( directory.getPath() + gkg::getDirectorySeparator() + *f,
                        headerObject,
                        sliceInfo, bswap, revision );
        if ( headerObject != commonHeaderObject )
        {

          throw std::runtime_error( std::string( "'" ) + *f +
                                    "' header is inconsistent with '" +
                                    *fileNames.begin() + "' header" );

        }

      }
      locations.insert( sliceInfo.loc );
      locationToNameLut.insert( std::pair< float, std::string >(
                            sliceInfo.loc,
                            *f ) );
      nameToSliceInfoLut[ *f ] = sliceInfo;
      ++ f;

    }

    // filling object header once consistency has been checked
    typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo dummySliceInfo;
    readCoreHeader( directory.getPath() + gkg::getDirectorySeparator() +
                    *fileNames.begin(), object,
                    dummySliceInfo, bswap, revision );

    // checking item type compatibility with header
    int32_t imgDepth;
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_pixhdr.img_depth",
                                     imgDepth );
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      std::string itemType;
      object.getHeader().getAttribute( "item_type", itemType );
      if ( ( ( imgDepth == 8 ) &&
             ( ! gkg::TypeOf< int8_t >::isSameType( itemType ) ) ) ||
           ( ( imgDepth == 8 ) &&
             ( ! gkg::TypeOf< uint8_t >::isSameType( itemType ) ) ) ||
           ( ( imgDepth == 16 ) &&
             ( ! gkg::TypeOf< int16_t >::isSameType( itemType ) ) ) )
      {

        throw gkg::ItemTypeException( name );

      }

    }

    // collecting slice count and rank count while doing sanity checks
    int32_t sizeZ, sizeT;

    sizeZ = ( int32_t )locations.size();
    sizeT = ( int32_t )locationToNameLut.count( *locations.begin() );

    std::set< float >::const_iterator l = locations.begin(),
                                      le = locations.end();

    std::vector< float > ctr_R( sizeZ );
    std::vector< float > ctr_A( sizeZ );
    std::vector< float > ctr_S( sizeZ );

    std::vector< float > tlhc_R( sizeZ );
    std::vector< float > tlhc_A( sizeZ );
    std::vector< float > tlhc_S( sizeZ );

    std::vector< float > trhc_R( sizeZ );
    std::vector< float > trhc_A( sizeZ );
    std::vector< float > trhc_S( sizeZ );

    std::vector< float > brhc_R( sizeZ );
    std::vector< float > brhc_A( sizeZ );
    std::vector< float > brhc_S( sizeZ );

    std::vector< std::string > orderedFileNames( sizeZ * sizeT );

    int32_t location = 0, rank = 0;
    while ( l != le )
    {

      if ( ( int32_t )locationToNameLut.count( *l ) != sizeT )
      {

        throw std::runtime_error( "inconsistent sizeT between locations" );

      }
      std::multimap< float, std::string >::const_iterator
        n = locationToNameLut.lower_bound( *l ),
        ne = locationToNameLut.upper_bound( *l );

      typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo
        sliceInfo = nameToSliceInfoLut[ n->second ];

      rank = 0;
      while ( n != ne )
      {

        typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo
          tmpSliceInfo = nameToSliceInfoLut[ n->second ];

        if ( !gkg::equal( tmpSliceInfo.ctr_R, sliceInfo.ctr_R, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.ctr_A, sliceInfo.ctr_A, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.ctr_S, sliceInfo.ctr_S, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.tlhc_R, sliceInfo.tlhc_R, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.tlhc_A, sliceInfo.tlhc_A, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.tlhc_S, sliceInfo.tlhc_S, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.trhc_R, sliceInfo.trhc_R, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.trhc_A, sliceInfo.trhc_A, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.trhc_S, sliceInfo.trhc_S, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.brhc_R, sliceInfo.brhc_R, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.brhc_A, sliceInfo.brhc_A, 1e-4f ) ||
             !gkg::equal( tmpSliceInfo.brhc_S, sliceInfo.brhc_S, 1e-4f ) )
        {

          throw std::runtime_error( std::string( "inconsistent slice info " ) +
                                    "between ranks of location " +
                  gkg::StringConverter::toString( *l, 'f', 5, 1 ) );

        }
        // we need to use sizeZ - 1 - location because z = 0 correspond to
        // top of the brain
        orderedFileNames[ rank * sizeZ + ( sizeZ - 1 - location ) ] = n->second;
        ++ rank;
        ++ n;

      }
      ctr_R[ sizeZ - 1 - location ] = sliceInfo.ctr_R;
      ctr_A[ sizeZ - 1 - location ] = sliceInfo.ctr_A;
      ctr_S[ sizeZ - 1 - location ] = sliceInfo.ctr_S;

      tlhc_R[ sizeZ - 1 - location ] = sliceInfo.tlhc_R;
      tlhc_A[ sizeZ - 1 - location ] = sliceInfo.tlhc_A;
      tlhc_S[ sizeZ - 1 - location ] = sliceInfo.tlhc_S;

      trhc_R[ sizeZ - 1 - location ] = sliceInfo.trhc_R;
      trhc_A[ sizeZ - 1 - location ] = sliceInfo.trhc_A;
      trhc_S[ sizeZ - 1 - location ] = sliceInfo.trhc_S;

      brhc_R[ sizeZ - 1 - location ] = sliceInfo.brhc_R;
      brhc_A[ sizeZ - 1 - location ] = sliceInfo.brhc_A;
      brhc_S[ sizeZ - 1 - location ] = sliceInfo.brhc_S;

      ++ location;
      ++ l;

    }

    // adding ctr RAS information for all locations
    object.addSemantic( "__gehc_signa_genesis__",
                        "ctr_R",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "ctr_R", ctr_R );
    object.addSemantic( "__gehc_signa_genesis__",
                        "ctr_A",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "ctr_A", ctr_A );
    object.addSemantic( "__gehc_signa_genesis__",
                        "ctr_S",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "ctr_S", ctr_S );

    // adding tlhc RAS information for all locations
    object.addSemantic( "__gehc_signa_genesis__",
                        "tlhc_R",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "tlhc_R", tlhc_R );
    object.addSemantic( "__gehc_signa_genesis__",
                        "tlhc_A",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "tlhc_A", tlhc_A );
    object.addSemantic( "__gehc_signa_genesis__",
                        "tlhc_S",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "tlhc_S", tlhc_S );

    // adding trhc RAS information for all locations
    object.addSemantic( "__gehc_signa_genesis__",
                        "trhc_R",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "trhc_R", trhc_R );
    object.addSemantic( "__gehc_signa_genesis__",
                        "trhc_A",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "trhc_A", trhc_A );
    object.addSemantic( "__gehc_signa_genesis__",
                        "trhc_S",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "trhc_S", trhc_S );

    // adding brhc RAS information for all locations
    object.addSemantic( "__gehc_signa_genesis__",
                        "brhc_R",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "brhc_R", brhc_R );
    object.addSemantic( "__gehc_signa_genesis__",
                        "brhc_A",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "brhc_A", brhc_A );
    object.addSemantic( "__gehc_signa_genesis__",
                        "brhc_S",
                        gkg::Semantic(
                             gkg::TypeOf< std::vector< float > >::getName() ) );
    object.getHeader().addAttribute( "brhc_S", brhc_S );

    // adding ordered file names
    object.addSemantic( "__gehc_signa_genesis__",
                        "ordered_file_name",
                        gkg::Semantic(
                            gkg::TypeOf< std::vector< std::string > >::getName()
                                      ) );
    object.getHeader().addAttribute( "ordered_file_names", 
                                     orderedFileNames );
    // adding ordered file names
    object.addSemantic( "__gehc_signa_genesis__",
                        "directory_path",
                        gkg::Semantic( gkg::TypeOf< std::string >::getName()) );
    object.getHeader().addAttribute( "directory_path", 
                                     directory.getPath() );

    // adding size(s) and resolution(s)

    int16_t imatrix_X;
    int16_t imatrix_Y;
    float pixsize_X;
    float pixsize_Y;
    float slthick;
    float scanspacing;

    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.imatrix_X",
                                     imatrix_X );
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.imatrix_Y",
                                     imatrix_Y );
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.pixsize_X",
                                     pixsize_X );
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.pixsize_Y",
                                     pixsize_Y );
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.slthick",
                                     slthick );
    object.getHeader().getAttribute( "genesis_header.rdb_hdr_image.scanspacing",
                                     scanspacing );

    object.getHeader()[ "sizeX" ] = ( int32_t )imatrix_X;
    object.getHeader()[ "sizeY" ] = ( int32_t )imatrix_Y;
    object.getHeader()[ "sizeZ" ] = sizeZ;
    object.getHeader()[ "sizeT" ] = sizeT;

    object.addSemantic( "__gehc_signa_genesis__", "resolutionX",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gehc_signa_genesis__", "resolutionY",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gehc_signa_genesis__", "resolutionZ",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.addSemantic( "__gehc_signa_genesis__", "resolutionT",
                        gkg::Semantic( gkg::TypeOf< double >::getName() ) );
    object.getHeader()[ "resolutionX" ] = ( double )pixsize_X;
    object.getHeader()[ "resolutionY" ] = ( double )pixsize_Y;
    object.getHeader()[ "resolutionZ" ] = ( double )( slthick + scanspacing );
    object.getHeader()[ "resolutionT" ] = 1.0;

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::read(
                                                const std::string& name,
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
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< T >& object ) const" );

}


template < class T >
inline
void gkg::GehcSignaGenesisDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // reading header(s)
      readHeader( name, volumeProxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName", name );

      // get byte order: rev. 7, rev. 8, rev. 9 -> SGI host => big endian
      // and revision
      bool bswap = false;
      float revision = -1.0;
      getBSwapAndRevision( name, bswap, revision );

      volumeProxy.getHeader().addAttribute( "proxyBSwap", ( bswap ? 1 : 0 ) );
      volumeProxy.getHeader().addAttribute( "proxyRevision", revision );

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GehcSignaGenesisDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::partialBinaryRead(
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

        gkg::Volume< T >& subVolume =
          dynamic_cast< gkg::Volume< T >& >( subpart );

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
            if ( !volumeProxy.getHeader().hasAttribute( "proxyRevision" ) )
            {

               throw std::runtime_error(
                                      "cannot find 'proxyRevision' attribute" );

            }
            int32_t proxyBSwap = 0;
            volumeProxy.getHeader().getAttribute( "proxyBSwap",
                                                  proxyBSwap );
            std::string proxyFileName;
            volumeProxy.getHeader().getAttribute( "proxyFileName",
                                                  proxyFileName );
            float proxyRevision;
            volumeProxy.getHeader().getAttribute( "proxyRevision",
                                                  proxyRevision );

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

            // reading from core data
            gkg::ItemReader< T >& itemR =
              gkg::DefaultItemIOFactory< T >::getInstance().getReader(
                                         false, ( proxyBSwap ? true : false ) );

            readPartialBinaryCore( itemR,
                                   volumeProxy,
                                   subVolume,
                                   boundingBox,
                                   proxyRevision );

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
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::GehcSignaGenesisDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::getBSwapAndRevision( 
                                                        const std::string& name,
                                                        bool& bswap,
                                                        float& revision ) const
{

  try
  {

    // browsing and checking directory coherence
    gkg::Directory directory( name );

    if ( !directory.isValid() )
    {

      throw gkg::FileNotFoundException( name );

    }

    if ( directory.getCount() == 0 )
    {

      throw std::runtime_error( std::string( "no file in '" ) +
                                directory.getPath() + "' directory" );

    }

    std::string fileName = directory.getPath() +
                           gkg::getDirectorySeparator() +
                           *directory.getFiles().begin();

    std::ifstream is( fileName.c_str() );

    if ( !is )
    {

      throw std::runtime_error( "bad input stream" );

    }

    int8_t revisionChar[ 3 ];

    // go to start of first Genesis file
    is.seekg( 20U, gkg::IosBase::beg );

    // get byte order: rev. 7, rev. 8, rev. 9 -> SGI host => big endian
    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    false );
    bswap = false;
    revision = -1.0;
    itemR_INT8.read( is, revisionChar, 2 );
    revisionChar[ 2 ] = '\0';
    if ( std::string( ( char* )revisionChar ) == "08" )
    {

      revision = 8.0;
      bswap = ( gkg::ByteOrder::getInstance().getCurrentType() ==
                gkg::ByteOrder::LittleEndian );

    }
    else if ( std::string( ( char* )revisionChar ) == "09" )
    {

      revision = 9.0;
      bswap = ( gkg::ByteOrder::getInstance().getCurrentType() ==
                gkg::ByteOrder::LittleEndian );

    }
    else
    {

      throw std::runtime_error( "not a valid revision number" );

    }

    // closing the file
    is.close();


  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::getBSwapAndRevision( "
             "bool& bswap, float& revision )" );

}


template < class T >
size_t 
gkg::GehcSignaGenesisDiskFormat< T >::getSuiteDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_SUITEDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_SUITEDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t "
             "gkg::GehcSignaGenesisDiskFormat< T >::getSuiteDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t 
gkg::GehcSignaGenesisDiskFormat< T >::getExamDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_EXAMDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_EXAMDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaGenesisDiskFormat< T >::getExamDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaGenesisDiskFormat< T >::getSeriesDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_SERIESDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_SERIESDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaGenesisDiskFormat< T >::getSeriesDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaGenesisDiskFormat< T >::getMrImageDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_MRIMAGEDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_MRIMAGEDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaGenesisDiskFormat< T >::getMrImageDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaGenesisDiskFormat< T >::getPixHdrSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_PIXHDR_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_PIXHDR_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaGenesisDiskFormat< T >::getMrImageDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaGenesisDiskFormat< T >::getGenesisHeaderSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 8:

        return GEHC_SIGNA_GENESIS_HEADER_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_GENESIS_HEADER_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaGenesisDiskFormat< T >::getGenesisHeaderSize( "
             "float revision ) const" );

}


template < class T >
void 
gkg::GehcSignaGenesisDiskFormat< T >::skipPadding( std::istream& is,
                                                   off_t start,
                                                   size_t size ) const
{

  try
  {

    off_t stop = is.tellg();
    is.seekg( size - ( stop - start ), gkg::IosBase::cur );

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::GehcSignaGenesisDiskFormat< T >::skipPadding( "
             "std::istream& is, "
             "off_t start, "
             "size_t size ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readCoreHeader(
            const std::string& name,
            gkg::HeaderedObject& object,
            typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo& sliceInfo,
            bool bswap,
            float revision ) const
{

  try
  {

    std::ifstream is( name.c_str() );

    if ( !is )
    {

      throw gkg::FileNotFoundException( name );

    }

    readSuiteDataType( is, "genesis_header.rdb_hdr_suite",
                       object, bswap, revision );
    readExamDataType( is, "genesis_header.rdb_hdr_exam",
                      object, bswap, revision );
    readSeriesDataType( is, "genesis_header.rdb_hdr_series",
                        object, bswap, revision );
    readMrImageDataType( is, "genesis_header.rdb_hdr_image",
                         object, sliceInfo, bswap, revision );
    readPixHdr( is, "genesis_header.rdb_hdr_pixhdr",
                object, sliceInfo, bswap, revision );

    is.close();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readCoreHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object, "
             "typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo& "
             "sliceInfo, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readPartialBinaryCore(
                                 const gkg::ItemReader< T >& itemR,
                                 const gkg::VolumeProxy< T >& volumeProxy,
                                 gkg::Volume< T >& subVolume,
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 float /* revision */ ) const
{

  try
  {

    // getting directory path and ordered file names
    std::string directoryPath;
    volumeProxy.getHeader().getAttribute( "directory_path", directoryPath );
    std::vector< std::string > orderedFileNames;
    volumeProxy.getHeader().getAttribute( "ordered_file_names",
                                          orderedFileNames );

    // getting header length
    int32_t genesisHeaderLength;
    volumeProxy.getHeader().getAttribute( "genesis_header_length",
                                          genesisHeaderLength );

    int32_t y, z, t;
    int32_t sizeX = subVolume.getSizeX();

    // reading data
    for ( t = 0; t < subVolume.getSizeT(); t++ )
    {

      for ( z = 0; z < subVolume.getSizeZ(); z++ )
      {

        std::string name = directoryPath + gkg::getDirectorySeparator() +
                           orderedFileNames[ ( t + boundingBox.getLowerT() ) *
                                             volumeProxy.getSizeZ() +
                                             ( z + boundingBox.getLowerZ() ) ];
        std::ifstream is( name.c_str() );
        if ( !is )
        {

          throw gkg::FileNotFoundException( name );

        }

        for ( y = 0; y < subVolume.getSizeY(); y++ )
        {

          is.seekg( genesisHeaderLength +
                    ( boundingBox.getLowerX() +
                    ( y + boundingBox.getLowerY() ) * volumeProxy.getSizeX() )
                    * sizeof( T ),
                    gkg::IosBase::beg );
          itemR.read( is, &subVolume( 0, y, z, t ), sizeX );

        }
        is.close();

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::"
             "readPartialBinaryCore( "
             "const gkg::ItemReader< INT16 >& itemR, "
             "const gkg::VolumeProxy< T >& volumeProxy, "
             "gkg::Volume< T >& subVolume, "
             "const gkg::BoundingBox< int >& boundingBox, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readSuiteDataType(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float revision ) const
{

  try
  {


    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );
    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_UINT32 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );

    size_t start = is.tellg();

    readStringItem( is, itemR_INT8, 4, prefix, "su_id", object );
    readItem( is, itemR_INT16, prefix, "su_uniq", object );
    readItem( is, itemR_INT8, prefix, "su_diskid", object );
    readStringItem( is, itemR_INT8, 13, prefix, "prodid", object );
    readStringItem( is, itemR_INT8, 2, prefix, "su_verscre", object );
    readStringItem( is, itemR_INT8, 2, prefix, "su_verscur", object );
    readItem( is, itemR_UINT32, prefix, "su_checksum", object );

    skipPadding( is, start, getSuiteDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::"
             "readSuiteDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readExamDataType(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float revision ) const
{

  try
  {


    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );
    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_UINT16 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int32_t >& itemR_INT32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint32_t >& itemR_UINT32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< float >& itemR_FLOAT =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   bswap );
    gkg::ItemReader< double >& itemR_DOUBLE =
      gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                    bswap );

    size_t start = is.tellg();

    readStringItem( is, itemR_INT8, 4, prefix, "ex_suid", object );
    readItem( is, itemR_INT16, prefix, "ex_uniq", object );
    readItem( is, itemR_INT8, prefix, "ex_diskid", object );
    readItem( is, itemR_UINT16, prefix, "ex_no", object );
    readStringItem( is, itemR_INT8, 33, prefix, "hospname", object );
    readItem( is, itemR_INT16, prefix, "detect", object );
    readItem( is, itemR_INT32, prefix, "numcells", object );
    readItem( is, itemR_FLOAT, prefix, "zerocell", object );
    readItem( is, itemR_FLOAT, prefix, "cellspace", object );
    readItem( is, itemR_FLOAT, prefix, "srctodet", object );
    readItem( is, itemR_FLOAT, prefix, "srctoiso", object );
    readItem( is, itemR_INT16, prefix, "tubetyp", object );
    readItem( is, itemR_INT16, prefix, "dastyp", object );
    readItem( is, itemR_INT16, prefix, "num_dcnk", object );
    readItem( is, itemR_INT16, prefix, "dcn_len", object );
    readItem( is, itemR_INT16, prefix, "dcn_density", object );
    readItem( is, itemR_INT16, prefix, "dcn_stepsize", object );
    readItem( is, itemR_INT16, prefix, "dcn_shiftcnt", object );
    readItem( is, itemR_INT32, prefix, "magstrength", object );
    readStringItem( is, itemR_INT8, 13, prefix, "patid", object );
    readStringItem( is, itemR_INT8, 25, prefix, "patname", object );
    readItem( is, itemR_INT16, prefix, "patage", object );
    readItem( is, itemR_INT16, prefix, "patian", object );
    readItem( is, itemR_INT16, prefix, "patsex", object );
    readItem( is, itemR_INT32, prefix, "patweight", object );
    readItem( is, itemR_INT16, prefix, "trauma", object );
    readStringItem( is, itemR_INT8, 61, prefix, "hist", object );
    readStringItem( is, itemR_INT8, 13, prefix, "reqnum", object );
    readItem( is, itemR_INT32, prefix, "ex_datetime", object );
    readStringItem( is, itemR_INT8, 33, prefix, "refphy", object );
    readStringItem( is, itemR_INT8, 33, prefix, "diagrad", object );
    readStringItem( is, itemR_INT8, 4, prefix, "op", object );
    readStringItem( is, itemR_INT8, 23, prefix, "ex_desc", object );
    readStringItem( is, itemR_INT8, 3, prefix, "ex_typ", object );
    readItem( is, itemR_INT16, prefix, "ex_format", object );
    readItem( is, itemR_DOUBLE, prefix, "firstaxtime", object );

    // need for a dummy string with 4 characters
    readStringItem( is, itemR_INT8, 4, prefix, "dummy1", object );

    readStringItem( is, itemR_INT8, 9, prefix, "ex_sysid", object );
    readItem( is, itemR_INT32, prefix, "ex_lastmod", object );
    readItem( is, itemR_INT16, prefix, "protocolflag", object );
    readStringItem( is, itemR_INT8, 13, prefix, "ex_alloc_key", object );
    readItem( is, itemR_INT32, prefix, "ex_delta_cnt", object );
    readStringItem( is, itemR_INT8, 2, prefix, "ex_verscre", object );
    readStringItem( is, itemR_INT8, 2, prefix, "ex_verscur", object );
    readItem( is, itemR_UINT32, prefix, "ex_checksum", object );

    readItem( is, itemR_INT32, prefix, "ex_complete", object );
    readItem( is, itemR_INT32, prefix, "ex_seriesct", object );
    readItem( is, itemR_INT32, prefix, "ex_numarch", object );
    readItem( is, itemR_INT32, prefix, "ex_numseries", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "ex_series", object );
    readItem( is, itemR_INT32, prefix, "ex_numunser", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "ex_unseries", object );
    readItem( is, itemR_INT32, prefix, "ex_toarchcnt", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "ex_toarchive", object );
    readItem( is, itemR_INT32, prefix, "ex_prospcnt", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "ex_prosp", object );
    readItem( is, itemR_INT32, prefix, "ex_modelnum", object );
    readItem( is, itemR_INT32, prefix, "ex_modelcnt", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "ex_models", object );
    readItem( is, itemR_INT16, prefix, "ex_stat", object );
    readStringItem( is, itemR_INT8, 16, prefix, "uniq_sys_id", object );
    readStringItem( is, itemR_INT8, 16, prefix, "service_id", object );
    readStringItem( is, itemR_INT8, 4, prefix, "mobile_loc", object );
    readStringItem( is, itemR_INT8, 32, prefix, "study_uid", object );
    readItem( is, itemR_INT16, prefix, "study_status", object );   

    skipPadding( is, start, getExamDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::"
             "readExamDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readSeriesDataType(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float revision ) const
{

  try
  {


    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );
    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_UINT16 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int32_t >& itemR_INT32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint32_t >& itemR_UINT32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< float >& itemR_FLOAT =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   bswap );

    size_t start = is.tellg();

    readStringItem( is, itemR_INT8, 4, prefix, "se_suid", object );
    readItem( is, itemR_INT16, prefix, "se_uniq", object );   
    readItem( is, itemR_INT8, prefix, "se_diskid", object );
    readItem( is, itemR_UINT16, prefix, "se_exno", object );
    readItem( is, itemR_INT16, prefix, "se_no", object );
    readItem( is, itemR_INT32, prefix, "se_datetime", object );
    readItem( is, itemR_INT32, prefix, "se_actual_dt", object );
    readStringItem( is, itemR_INT8, 30, prefix, "se_desc", object );
    readStringItem( is, itemR_INT8, 9, prefix, "pr_sysid", object );
    readStringItem( is, itemR_INT8, 9, prefix, "pansysid", object );
    readItem( is, itemR_INT16, prefix, "se_typ", object );
    readItem( is, itemR_INT16, prefix, "se_source", object );
    readItem( is, itemR_INT16, prefix, "se_plane", object );
    readItem( is, itemR_INT16, prefix, "scan_type", object );
    readItem( is, itemR_INT32, prefix, "position", object );
    readItem( is, itemR_INT32, prefix, "entry", object );
    readStringItem( is, itemR_INT8, 3, prefix, "anref", object );
    readItem( is, itemR_FLOAT, prefix, "lmhor", object );
    readStringItem( is, itemR_INT8, 25, prefix, "prtcl", object );
    readItem( is, itemR_INT16, prefix, "se_contrast", object );
    readItem( is, itemR_INT8, prefix, "start_ras", object );
    readItem( is, itemR_FLOAT, prefix, "start_loc", object );
    readItem( is, itemR_INT8, prefix, "end_ras", object );
    readItem( is, itemR_FLOAT, prefix, "end_loc", object );
    readItem( is, itemR_INT16, prefix, "se_pseq", object );
    readItem( is, itemR_INT16, prefix, "se_sortorder", object );
    readItem( is, itemR_INT32, prefix, "se_lndmrkcnt", object );
    readItem( is, itemR_INT16, prefix, "se_nacq", object );
    readItem( is, itemR_INT16, prefix, "xbasest", object );
    readItem( is, itemR_INT16, prefix, "xbaseend", object );
    readItem( is, itemR_INT16, prefix, "xenhst", object );
    readItem( is, itemR_INT16, prefix, "xenhend", object );
    readItem( is, itemR_INT32, prefix, "se_lastmod", object );
    readStringItem( is, itemR_INT8, 13, prefix, "se_alloc_key", object );
    readItem( is, itemR_INT32, prefix, "se_delta_cnt", object );
    readStringItem( is, itemR_INT8, 2, prefix, "se_verscre", object );
    readStringItem( is, itemR_INT8, 2, prefix, "se_verscur", object );
    readItem( is, itemR_FLOAT, prefix, "se_pds_a", object );
    readItem( is, itemR_FLOAT, prefix, "se_pds_c", object );
    readItem( is, itemR_FLOAT, prefix, "se_pds_u", object );
    readItem( is, itemR_UINT32, prefix, "se_checksum", object );
    readItem( is, itemR_INT32, prefix, "se_complete", object );
    readItem( is, itemR_INT32, prefix, "se_numarch", object );
    readItem( is, itemR_INT32, prefix, "se_imagect", object );
    readItem( is, itemR_INT32, prefix, "se_numimages", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "se_images", object );
    readItem( is, itemR_INT32, prefix, "se_numunimg", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "se_unimages", object );
    readItem( is, itemR_INT32, prefix, "se_toarchcnt", object );
    readVarTypeItem( is, itemR_INT8, itemR_INT32, itemR_UINT32,
                     prefix, "se_toarchive", object );
    readItem( is, itemR_FLOAT, prefix, "echo1_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo1_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo1_window", object );
    readItem( is, itemR_INT16, prefix, "echo1_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo2_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo2_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo2_window", object );
    readItem( is, itemR_INT16, prefix, "echo2_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo3_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo3_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo3_window", object );
    readItem( is, itemR_INT16, prefix, "echo3_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo4_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo4_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo4_window", object );
    readItem( is, itemR_INT16, prefix, "echo4_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo5_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo5_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo5_window", object );
    readItem( is, itemR_INT16, prefix, "echo5_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo6_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo6_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo6_window", object );
    readItem( is, itemR_INT16, prefix, "echo6_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo7_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo7_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo7_window", object );
    readItem( is, itemR_INT16, prefix, "echo7_level", object );
    readItem( is, itemR_FLOAT, prefix, "echo8_alpha", object );
    readItem( is, itemR_FLOAT, prefix, "echo8_beta", object );
    readItem( is, itemR_UINT16, prefix, "echo8_window", object );
    readItem( is, itemR_INT16, prefix, "echo8_level", object );
    readStringItem( is, itemR_INT8, 32, prefix, "series_uid", object );
    readStringItem( is, itemR_INT8, 32, prefix, "landmark_uid", object );
    readStringItem( is, itemR_INT8, 32, prefix, "equipmnt_uid", object );

    skipPadding( is, start, getSeriesDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readSeriesDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readMrImageDataType(
            std::istream& is, 
            const std::string& prefix,
            gkg::HeaderedObject& object,
            typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo& sliceInfo,
            bool bswap,
            float revision ) const
{

  try
  {


    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );
    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_UINT16 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int32_t >& itemR_INT32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint32_t >& itemR_UINT32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< float >& itemR_FLOAT =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   bswap );

    size_t start = is.tellg();

    readStringItem( is, itemR_INT8, 4, prefix, "im_suid", object );
    readItem( is, itemR_INT16, prefix, "im_uniq", object );
    readItem( is, itemR_INT8, prefix, "im_diskid", object );
    readItem( is, itemR_UINT16, prefix, "im_exno", object );
    readItem( is, itemR_INT16, prefix, "im_seno", object );
    readBaseItem( is, itemR_INT16, sliceInfo.im_no );
    readItem( is, itemR_INT32, prefix, "im_datetime", object );
    readItem( is, itemR_INT32, prefix, "im_actual_dt", object );
    readItem( is, itemR_FLOAT, prefix, "sctime", object );
    readItem( is, itemR_FLOAT, prefix, "slthick", object );
    readItem( is, itemR_INT16, prefix, "imatrix_X", object );
    readItem( is, itemR_INT16, prefix, "imatrix_Y", object );
    readItem( is, itemR_FLOAT, prefix, "dfov", object );
    readItem( is, itemR_FLOAT, prefix, "dfov_rect", object );
    readItem( is, itemR_FLOAT, prefix, "dim_X", object );
    readItem( is, itemR_FLOAT, prefix, "dim_Y", object );
    readItem( is, itemR_FLOAT, prefix, "pixsize_X", object );
    readItem( is, itemR_FLOAT, prefix, "pixsize_Y", object );
    readStringItem( is, itemR_INT8, 14, prefix, "pdid", object );
    readStringItem( is, itemR_INT8, 17, prefix, "contrastIV", object );
    readStringItem( is, itemR_INT8, 17, prefix, "contrastOral", object );
    readItem( is, itemR_INT16, prefix, "contmode", object );
    readItem( is, itemR_INT16, prefix, "serrx", object );
    readItem( is, itemR_INT16, prefix, "imgrx", object );
    readItem( is, itemR_INT16, prefix, "screenformat", object );
    readItem( is, itemR_INT16, prefix, "plane", object );
    readItem( is, itemR_FLOAT, prefix, "scanspacing", object );
    readItem( is, itemR_INT16, prefix, "im_compress", object );
    readItem( is, itemR_INT16, prefix, "im_scouttype", object );
    readItem( is, itemR_INT8, prefix, "loc_ras", object );
    readBaseItem( is, itemR_FLOAT, sliceInfo.loc );
    readBaseItem( is, itemR_FLOAT, sliceInfo.ctr_R );
    readBaseItem( is, itemR_FLOAT, sliceInfo.ctr_A );
    readBaseItem( is, itemR_FLOAT, sliceInfo.ctr_S );
    readItem( is, itemR_FLOAT, prefix, "norm_R", object );
    readItem( is, itemR_FLOAT, prefix, "norm_A", object );
    readItem( is, itemR_FLOAT, prefix, "norm_S", object );
    readBaseItem( is, itemR_FLOAT, sliceInfo.tlhc_R );
    readBaseItem( is, itemR_FLOAT, sliceInfo.tlhc_A );
    readBaseItem( is, itemR_FLOAT, sliceInfo.tlhc_S );
    readBaseItem( is, itemR_FLOAT, sliceInfo.trhc_R );
    readBaseItem( is, itemR_FLOAT, sliceInfo.trhc_A );
    readBaseItem( is, itemR_FLOAT, sliceInfo.trhc_S );
    readBaseItem( is, itemR_FLOAT, sliceInfo.brhc_R );
    readBaseItem( is, itemR_FLOAT, sliceInfo.brhc_A );
    readBaseItem( is, itemR_FLOAT, sliceInfo.brhc_S );
    readStringItem( is, itemR_INT8, 4, prefix, "forimgrev", object );
    readItem( is, itemR_INT32, prefix, "tr", object );
    readItem( is, itemR_INT32, prefix, "ti", object );
    readItem( is, itemR_INT32, prefix, "te", object );
    readItem( is, itemR_INT32, prefix, "te2", object );
    readItem( is, itemR_INT16, prefix, "numecho", object );
    readItem( is, itemR_INT16, prefix, "echonum", object );
    readItem( is, itemR_FLOAT, prefix, "tbldlta", object );
    readItem( is, itemR_FLOAT, prefix, "nex", object );
    readItem( is, itemR_INT16, prefix, "contig", object );
    readItem( is, itemR_INT16, prefix, "hrtrate", object );
    readItem( is, itemR_INT32, prefix, "tdel", object );
    readItem( is, itemR_FLOAT, prefix, "saravg", object );
    readItem( is, itemR_FLOAT, prefix, "sarpeak", object );
    readItem( is, itemR_INT16, prefix, "monsar", object );
    readItem( is, itemR_INT16, prefix, "trgwindow", object );
    readItem( is, itemR_FLOAT, prefix, "reptime", object );
    readItem( is, itemR_INT16, prefix, "imgpcyc", object );
    readItem( is, itemR_INT16, prefix, "xmtgain", object );
    readItem( is, itemR_INT16, prefix, "rcvgain1", object );
    readItem( is, itemR_INT16, prefix, "rcvgain2", object );
    readItem( is, itemR_INT16, prefix, "mr_flip", object );
    readItem( is, itemR_INT32, prefix, "mindat", object );
    readItem( is, itemR_INT16, prefix, "cphase", object );
    readItem( is, itemR_INT16, prefix, "swappf", object );
    readItem( is, itemR_INT16, prefix, "pauseint", object );
    readItem( is, itemR_FLOAT, prefix, "pausetime", object );
    readItem( is, itemR_INT32, prefix, "obplane", object );
    readItem( is, itemR_INT32, prefix, "slocfov", object );
    readItem( is, itemR_INT32, prefix, "xmtfreq", object );
    readItem( is, itemR_INT32, prefix, "autoxmtfreq", object );
    readItem( is, itemR_INT16, prefix, "autoxmtgain", object );
    readItem( is, itemR_INT16, prefix, "prescan_r1", object );
    readItem( is, itemR_INT16, prefix, "prescan_r2", object );
    readItem( is, itemR_INT32, prefix, "user_bitmap", object );
    readItem( is, itemR_INT16, prefix, "cenfreq", object );
    readItem( is, itemR_INT16, prefix, "imode", object );
    readItem( is, itemR_INT32, prefix, "iopt", object );
    readItem( is, itemR_INT16, prefix, "pseq", object );
    readItem( is, itemR_INT16, prefix, "pseqmode", object );
    readStringItem( is, itemR_INT8, 33, prefix, "psdname", object );
    readItem( is, itemR_INT32, prefix, "psd_datetime", object );
    readStringItem( is, itemR_INT8, 13, prefix, "psd_iname", object );
    readItem( is, itemR_INT16, prefix, "ctyp", object );
    readStringItem( is, itemR_INT8, 17, prefix, "cname", object );
    readItem( is, itemR_INT16, prefix, "surfctyp", object );
    readItem( is, itemR_INT16, prefix, "surfcext", object );
    readBaseItem( is, itemR_INT32, sliceInfo.rawrunnum );
    readItem( is, itemR_UINT32, prefix, "cal_fldstr", object );
    readItem( is, itemR_INT16, prefix, "supp_tech", object );
    readItem( is, itemR_FLOAT, prefix, "vbw", object );
    readItem( is, itemR_INT16, prefix, "slquant", object );
    readItem( is, itemR_INT16, prefix, "gpre", object );
    readItem( is, itemR_INT32, prefix, "intr_del", object );
    readItem( is, itemR_FLOAT, prefix, "user0", object );
    readItem( is, itemR_FLOAT, prefix, "user1", object );
    readItem( is, itemR_FLOAT, prefix, "user2", object );
    readItem( is, itemR_FLOAT, prefix, "user3", object );
    readItem( is, itemR_FLOAT, prefix, "user4", object );
    readItem( is, itemR_FLOAT, prefix, "user5", object );
    readItem( is, itemR_FLOAT, prefix, "user6", object );
    readItem( is, itemR_FLOAT, prefix, "user7", object );
    readItem( is, itemR_FLOAT, prefix, "user8", object );
    readItem( is, itemR_FLOAT, prefix, "user9", object );
    readItem( is, itemR_FLOAT, prefix, "user10", object );
    readItem( is, itemR_FLOAT, prefix, "user11", object );
    readItem( is, itemR_FLOAT, prefix, "user12", object );
    readItem( is, itemR_FLOAT, prefix, "user13", object );
    readItem( is, itemR_FLOAT, prefix, "user14", object );
    readItem( is, itemR_FLOAT, prefix, "user15", object );
    readItem( is, itemR_FLOAT, prefix, "user16", object );
    readItem( is, itemR_FLOAT, prefix, "user17", object );
    readItem( is, itemR_FLOAT, prefix, "user18", object );
    readItem( is, itemR_FLOAT, prefix, "user19", object );
    readItem( is, itemR_FLOAT, prefix, "user20", object );
    readItem( is, itemR_FLOAT, prefix, "user21", object );
    readItem( is, itemR_FLOAT, prefix, "user22", object );
    readItem( is, itemR_FLOAT, prefix, "user23", object );
    readItem( is, itemR_FLOAT, prefix, "user24", object );
    readStringItem( is, itemR_INT8, 13, prefix, "im_alloc_key", object );
    readItem( is, itemR_INT32, prefix, "im_lastmod", object );
    readStringItem( is, itemR_INT8, 2, prefix, "im_verscre", object );
    readStringItem( is, itemR_INT8, 2, prefix, "im_verscur", object );
    readItem( is, itemR_INT32, prefix, "im_pds_a", object );
    readItem( is, itemR_INT32, prefix, "im_pds_c", object );
    readItem( is, itemR_INT32, prefix, "im_pds_u", object );
    readBaseItem( is, itemR_UINT32, sliceInfo.im_checksum );
    readItem( is, itemR_INT32, prefix, "im_archived", object );
    readItem( is, itemR_INT32, prefix, "im_complete", object );
    readItem( is, itemR_INT16, prefix, "satbits", object );
    readItem( is, itemR_INT16, prefix, "scic", object );
    readItem( is, itemR_INT16, prefix, "satxloc1", object );
    readItem( is, itemR_INT16, prefix, "satxloc2", object );
    readItem( is, itemR_INT16, prefix, "satyloc1", object );
    readItem( is, itemR_INT16, prefix, "satyloc2", object );
    readItem( is, itemR_INT16, prefix, "satzloc1", object );
    readItem( is, itemR_INT16, prefix, "satzloc2", object );
    readItem( is, itemR_INT16, prefix, "satxthick", object );
    readItem( is, itemR_INT16, prefix, "satythick", object );
    readItem( is, itemR_INT16, prefix, "satzthick", object );
    readItem( is, itemR_INT16, prefix, "flax", object );
    readItem( is, itemR_INT16, prefix, "venc", object );
    readItem( is, itemR_INT16, prefix, "thk_disclmr", object );
    readItem( is, itemR_INT16, prefix, "ps_flag", object );
    readItem( is, itemR_INT16, prefix, "ps_status", object );
    readItem( is, itemR_INT16, prefix, "image_type", object );
    readItem( is, itemR_INT16, prefix, "vas_collapse", object );
    readItem( is, itemR_FLOAT, prefix, "user23n", object );
    readItem( is, itemR_FLOAT, prefix, "user24n", object );
    readItem( is, itemR_INT16, prefix, "proj_alg", object );
    readStringItem( is, itemR_INT8, 13, prefix, "proj_name", object );
    readItem( is, itemR_FLOAT, prefix, "x_axis_rot", object );
    readItem( is, itemR_FLOAT, prefix, "y_axis_rot", object );
    readItem( is, itemR_FLOAT, prefix, "z_axis_rot", object );
    readItem( is, itemR_INT32, prefix, "thresh_min1", object );
    readItem( is, itemR_INT32, prefix, "thresh_max1", object );
    readItem( is, itemR_INT32, prefix, "thresh_min2", object );
    readItem( is, itemR_INT32, prefix, "thresh_max2", object );
    readItem( is, itemR_INT16, prefix, "echo_trn_len", object );
    readItem( is, itemR_INT16, prefix, "frac_echo", object );
    readItem( is, itemR_INT16, prefix, "prep_pulse", object );
    readItem( is, itemR_INT16, prefix, "cphasenum", object );
    readItem( is, itemR_INT16, prefix, "var_echo", object );
    readItem( is, itemR_INT8, prefix, "ref_img", object );
    readItem( is, itemR_INT8, prefix, "sum_img", object );
    readItem( is, itemR_UINT16, prefix, "img_window", object );
    readItem( is, itemR_INT16, prefix, "img_level", object );
    readItem( is, itemR_INT32, prefix, "slop_int_1", object );
    readItem( is, itemR_INT32, prefix, "slop_int_2", object );
    readItem( is, itemR_INT32, prefix, "slop_int_3", object );
    readItem( is, itemR_INT32, prefix, "slop_int_4", object );
    readItem( is, itemR_INT32, prefix, "slop_int_5", object );
    readItem( is, itemR_FLOAT, prefix, "slop_float_1", object );
    readItem( is, itemR_FLOAT, prefix, "slop_float_2", object );
    readItem( is, itemR_FLOAT, prefix, "slop_float_3", object );
    readItem( is, itemR_FLOAT, prefix, "slop_float_4", object );
    readItem( is, itemR_FLOAT, prefix, "slop_float_5", object );
    readStringItem( is, itemR_INT8, 16, prefix, "slop_str_1", object );
    readStringItem( is, itemR_INT8, 16, prefix, "slop_str_2", object );
    readItem( is, itemR_INT16, prefix, "scanactno", object );
    readItem( is, itemR_INT16, prefix, "vasflags", object );
    readItem( is, itemR_FLOAT, prefix, "vencscale", object );
    readItem( is, itemR_INT16, prefix, "integrity", object );
    readItem( is, itemR_INT32, prefix, "fphase", object );
    readItem( is, itemR_INT16, prefix, "freq_dir", object );
    readItem( is, itemR_INT16, prefix, "vas_mode", object );
    readStringItem( is, itemR_INT8, 32, prefix, "image_uid", object );
    readStringItem( is, itemR_INT8, 32, prefix, "sop_uid", object );
    readItem( is, itemR_INT16, prefix, "dont_use_1", object );
    readItem( is, itemR_INT16, prefix, "dont_use_2", object );
    readItem( is, itemR_INT16, prefix, "dont_use_3", object );
    readItem( is, itemR_INT16, prefix, "pscopts", object );
    readItem( is, itemR_INT16, prefix, "asoffsetx", object );
    readItem( is, itemR_INT16, prefix, "asoffsety", object );
    readItem( is, itemR_INT16, prefix, "asoffsetz", object );
    readItem( is, itemR_INT16, prefix, "unoriginal", object );
    readItem( is, itemR_INT16, prefix, "interleaves", object );
    readItem( is, itemR_INT16, prefix, "effechospace", object );
    readItem( is, itemR_INT16, prefix, "viewsperseg", object );
    readItem( is, itemR_INT16, prefix, "rbpm", object );
    readItem( is, itemR_INT16, prefix, "rtpoint", object );
    readItem( is, itemR_INT16, prefix, "rcvrtype", object );
    readItem( is, itemR_FLOAT, prefix, "dbdt", object );
    readItem( is, itemR_FLOAT, prefix, "dbdtper", object );
    readItem( is, itemR_FLOAT, prefix, "estdbdtper", object );
    readItem( is, itemR_FLOAT, prefix, "estdbdtts", object );
    readItem( is, itemR_FLOAT, prefix, "saravghead", object );
    readItem( is, itemR_FLOAT, prefix, "neg_scanspacing", object );
    readItem( is, itemR_INT32, prefix, "offsetfreq", object );
    readItem( is, itemR_UINT32, prefix, "user_usage_tag", object );
    readItem( is, itemR_UINT32, prefix, "user_fill_mapMSW", object );
    readItem( is, itemR_UINT32, prefix, "user_fill_mapLSW", object );
    readItem( is, itemR_FLOAT, prefix, "user25", object );
    readItem( is, itemR_FLOAT, prefix, "user26", object );
    readItem( is, itemR_FLOAT, prefix, "user27", object );
    readItem( is, itemR_FLOAT, prefix, "user28", object );
    readItem( is, itemR_FLOAT, prefix, "user29", object );
    readItem( is, itemR_FLOAT, prefix, "user30", object );
    readItem( is, itemR_FLOAT, prefix, "user31", object );
    readItem( is, itemR_FLOAT, prefix, "user32", object );
    readItem( is, itemR_FLOAT, prefix, "user33", object );
    readItem( is, itemR_FLOAT, prefix, "user34", object );
    readItem( is, itemR_FLOAT, prefix, "user35", object );
    readItem( is, itemR_FLOAT, prefix, "user36", object );
    readItem( is, itemR_FLOAT, prefix, "user37", object );
    readItem( is, itemR_FLOAT, prefix, "user38", object );
    readItem( is, itemR_FLOAT, prefix, "user39", object );
    readItem( is, itemR_FLOAT, prefix, "user40", object );
    readItem( is, itemR_FLOAT, prefix, "user41", object );
    readItem( is, itemR_FLOAT, prefix, "user42", object );
    readItem( is, itemR_FLOAT, prefix, "user43", object );
    readItem( is, itemR_FLOAT, prefix, "user44", object );
    readItem( is, itemR_FLOAT, prefix, "user45", object );
    readItem( is, itemR_FLOAT, prefix, "user46", object );
    readItem( is, itemR_FLOAT, prefix, "user47", object );
    readItem( is, itemR_FLOAT, prefix, "user48", object );
    readItem( is, itemR_INT32, prefix, "slop_int_6", object );
    readItem( is, itemR_UINT32, prefix, "iopt2", object );
    readItem( is, itemR_INT32, prefix, "slop_int_8", object );
    readItem( is, itemR_INT32, prefix, "slop_int_9", object );

    skipPadding( is, start, getMrImageDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readMrImageDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readPixHdr(
            std::istream& is, 
            const std::string& prefix,
            gkg::HeaderedObject& object,
            typename gkg::GehcSignaGenesisDiskFormat< T >::SliceInfo& sliceInfo,
            bool bswap,
            float /*revision*/ ) const
{

  try
  {

    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_UINT16 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int32_t >& itemR_INT32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );

    size_t start = is.tellg();

    readItem( is, itemR_INT32, prefix, "img_magic", object );
    readItem( is, itemR_INT32, prefix, "img_hdr_length", object );
    readItem( is, itemR_INT32, prefix, "img_width", object );
    readItem( is, itemR_INT32, prefix, "img_height", object );
    readItem( is, itemR_INT32, prefix, "img_depth", object );
    readItem( is, itemR_INT32, prefix, "img_compress", object );
    readBaseItem( is, itemR_INT32, sliceInfo.img_dwindow );
    readBaseItem( is, itemR_INT32, sliceInfo.img_dlevel );
    readItem( is, itemR_INT32, prefix, "img_bgshade", object );
    readItem( is, itemR_INT32, prefix, "img_ovrflow", object );
    readItem( is, itemR_INT32, prefix, "img_undflow", object );
    readItem( is, itemR_INT32, prefix, "img_top_offset", object );
    readItem( is, itemR_INT32, prefix, "img_bot_offset", object );
    readItem( is, itemR_INT16, prefix, "img_version", object );
    readItem( is, itemR_UINT16, prefix, "img_checksum", object );
    readItem( is, itemR_INT32, prefix, "img_p_id", object );
    readItem( is, itemR_INT32, prefix, "img_l_id", object );
    readItem( is, itemR_INT32, prefix, "img_p_unpack", object );
    readItem( is, itemR_INT32, prefix, "img_l_unpack", object );
    readItem( is, itemR_INT32, prefix, "img_p_compress", object );
    readItem( is, itemR_INT32, prefix, "img_l_compress", object );
    readItem( is, itemR_INT32, prefix, "img_p_histo", object );
    readItem( is, itemR_INT32, prefix, "img_l_histo", object );
    readItem( is, itemR_INT32, prefix, "img_p_text", object );
    readItem( is, itemR_INT32, prefix, "img_l_text", object );
    readItem( is, itemR_INT32, prefix, "img_p_graphics", object );
    readItem( is, itemR_INT32, prefix, "img_l_graphics", object );
    readItem( is, itemR_INT32, prefix, "img_p_dbHdr", object );
    readItem( is, itemR_INT32, prefix, "img_l_dbHdr", object );
    readItem( is, itemR_INT32, prefix, "img_levelOffset", object );
    readItem( is, itemR_INT32, prefix, "img_p_user", object );
    readItem( is, itemR_INT32, prefix, "img_l_user", object );

    // storing directly Genesis header length
    int pixhdr_length;
    object.getHeader().getAttribute( prefix + ".img_hdr_length",
                                     pixhdr_length );
    object.addSemantic( "__gehc_signa_genesis__",
                        "genesis_header_length",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader().addAttribute( "genesis_header_length", 
                                     ( int32_t )start + pixhdr_length );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readPixHdr( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T > template < class U >
void gkg::GehcSignaGenesisDiskFormat< T >::readBaseItem(
                                       std::istream& is, 
                                       const gkg::ItemReader< U >& itemR,
                                       U& u,
                                       float revision,
                                       int32_t revisionMask ) const
{

  try
  {

    if ( ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {

      // aligning bytes
      char dummy;
      while ( is.tellg() % sizeof( U ) )
      {

        is.read( &dummy, 1 );

      }

      // reading data
      itemR.read( is, u );

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readBaseItem( "
             "std::istream& is, "
             "const gkg::ItemReader< U >& itemR, "
             "U& u, "
             "float revision, "
             "int revisionMask ) const" );

}


template < class T > template < class U >
void gkg::GehcSignaGenesisDiskFormat< T >::readItem(
                                             std::istream& is, 
                                             const gkg::ItemReader< U >& itemR,
                                             const std::string& prefix,
                                             const std::string& name,
                                             gkg::HeaderedObject& object,
                                             float revision,
                                             int32_t revisionMask ) const
{

  try
  {

    if ( ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {

      U data;
      readBaseItem( is, itemR, data );

      // adding object to header
      std::string attributeName = prefix + '.' + name;
      object.addSemantic( "__gehc_signa_genesis__",
                          attributeName,
                          gkg::Semantic( gkg::TypeOf< U >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readItem( "
             "std::istream& is, "
             "const gkg::ItemReader< U >& itemR, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object, "
             "float revision, "
             "int revisionMask ) const" );

}


template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readStringItem(
                                         std::istream& is, 
                                         const gkg::ItemReader< int8_t >& itemR,
                                         size_t count,
                                         const std::string& prefix,
                                         const std::string& name,
                                         gkg::HeaderedObject& object,
                                         float revision,
                                         int32_t revisionMask ) const
{

  try
  {

    if ( ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {

      // aligning bytes
      char dummy;
      while ( is.tellg() % sizeof( char ) )
      {

        is.read( &dummy, 1 );

      }

      // reading data
      int8_t* data = new int8_t[ count + 1 ];
      itemR.read( is, data, count );
      data[ count ] = '\0';

      for ( size_t n = 0; n < count; n++ )
      {

        if ( !isprint( data[ n ] ) )
        {

          data[ n ] = '\0';

        }
        if ( data[ n ] == '\\' )
        {

          data[ n ] = '_';

        }

      }

      std::string data_string = std::string( ( char* )data );
      delete [] data;

      // adding object to header
      std::string attributeName = prefix + '.' + name;
      object.addSemantic(
                       "__gehc_signa_genesis__",
                       attributeName,
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      if ( gkg::ConfigurationInfo::getInstance().hasAnonymousName() &&
           ( name == "patname" ) )
      {

        data_string = "anonymous";

      }
      object.getHeader().addAttribute( attributeName, data_string );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readStringItem( "
             "std::istream& is, "
             "const gkg::ItemReader< int8_t >& itemR, "
             "size_t count, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object, "
             "float revision, "
             "int revisionMask ) const" );

}



template < class T >
void gkg::GehcSignaGenesisDiskFormat< T >::readVarTypeItem(
                            std::istream& is, 
                            const gkg::ItemReader< int8_t >& itemR_INT8,
                            const gkg::ItemReader< int32_t >& itemR_INT32,
                            const gkg::ItemReader< uint32_t >& itemR_UINT32,
                            const std::string& prefix,
                            const std::string& name,
                            gkg::HeaderedObject& object,
                            float revision,
                            int32_t revisionMask ) const
{

  try
  {

    if ( ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {

      // aligning bytes
      char dummy1;
      while ( is.tellg() % sizeof( int32_t ) )
      {

        is.read( &dummy1, 1 );

      }

      // reading data
      uint32_t length;
      itemR_UINT32.read( is, length );

      std::string data_string = "";
      if ( length > 0 )
      {

        int8_t* data = new int8_t[ length + 1 ];
        itemR_INT8.read( is, data, length );
        data[ length ] = '\0';
        for ( size_t n = 0; n < length; n++ )
        {

          if ( !isprint( data[ n ] ) )
          {

            data[ n ] = '\0';

          }

        }
        data_string = std::string( ( char* )data );
        delete [] data;

      }
      else
      {

        int32_t dummy2;
        itemR_INT32.read( is, dummy2 );

      }

      // adding object to header
      std::string attributeName = prefix + '.' + name;
      object.addSemantic( "__gehc_signa_genesis__",
                          attributeName,
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader().addAttribute( attributeName, data_string );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaGenesisDiskFormat< T >::readVarTypeItem( "
             "std::istream& is, "
             "const gkg::ItemReader< int8_t >& itemR_INT8, "
             "const gkg::ItemReader< int32_t >& itemR_INT32, "
             "const gkg::ItemReader< uint32_t >& itemR_UINT32, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object, "
             "float revision, "
             "int revisionMask ) const" );

}


template class gkg::GehcSignaGenesisDiskFormat< uint8_t >;
template class gkg::GehcSignaGenesisDiskFormat< int16_t >;


//
// registrating standard GEHC Signa Genesis disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< uint8_t > 
                           >::getInstance().registerDiskFormat(
         gkg::GehcSignaGenesisDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::GehcSignaGenesisDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int16_t > 
                           >::getInstance().registerDiskFormat(
         gkg::GehcSignaGenesisDiskFormat< int16_t >::getInstance().getName(),
         &gkg::GehcSignaGenesisDiskFormat< int16_t >::getInstance() );

    return true;

  }
  GKG_CATCH( "GEHC Signa Genesis disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();

