#include <gkg-mri-reconstruction-io/GehcSignaPFileDiskFormat.h>
#include <gkg-mri-reconstruction-io/GehcSignaPFileHeader.h>
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
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <complex>
#include <vector>
#include <fstream>



//RegisterTemplateType( std::complex, std_complex );

//
// some useful defines and macro(s)
//

#define REV7      1
#define REV8      2
#define REV9      4


#define ENDL \
  { \
    if ( _logStream ) \
    { \
      *_logStream << std::endl; \
    } \
  }

#define BEGIN_SECTION( name ) \
  { \
    if ( _logStream ) \
    { \
      *_logStream << _indent.getSpaces() \
                  << name \
                  << " : " \
                  << std::endl \
                  << std::endl; \
      _indent.increment(); \
    } \
  }

#define END_SECTION \
  { \
    if ( _logStream ) \
    { \
      _indent.decrement(); \
      *_logStream << std::endl; \
    } \
  }


//
// class GehcSignaPFileDiskFormat
//


template < class T >
gkg::GehcSignaPFileDiskFormat< T >::GehcSignaPFileDiskFormat()
                   : gkg::TypedDiskFormat< gkg::Volume< std::complex< T > > >(),
                     _logStream( 0 )
{

  this->_extensions.insert( ".7" );

}


template < class T >
gkg::GehcSignaPFileDiskFormat< T >::~GehcSignaPFileDiskFormat()
{
}


template < class T >
std::string gkg::GehcSignaPFileDiskFormat< T >::getName() const
{

  return "gehc-signa-pfile";

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading the <*.7> header
    std::ifstream is( ( this->getNameWithoutExtension( name ) +
                        ".7" ).c_str() );

    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) + 
                                        ".7" );

    }

    // checking item type compatibility
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      std::string itemType;
      object.getHeader().getAttribute( "item_type", itemType );
      if ( ! gkg::TypeOf< std::complex< T > >::isSameType( itemType ) ) 
      {

        throw gkg::ItemTypeException(
                                 this->getNameWithoutExtension( name ) + ".7" );

      }

    }

    // get byte order: rev. 7, rev. 8 -> SGI host => big endian
    //                 rev. 9 -> Linux host => little endian
    bool bswap = false;
    float revision = -1.0;
    getBSwapAndRevision( is, bswap, revision );


    // item reader of character(s)
    gkg::ItemReader< int8_t >& itemR_INT8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                     bswap );

    BEGIN_SECTION( "POOL_HEADER" );

      BEGIN_SECTION( "RDBM_HEADER" );

        BEGIN_SECTION( "rdb_hdr_rec" );
          readRdbHeaderRec( is, "pool_header.rdb_hdr_rec",
                            object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_per_pass" );
          readRdbPerPassInfoEntryTab( is, "pool_header.rdb_hdr_per_pass",
                                      object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_unlock_raw" );
          readRdbPerPassInfoEntryTab( is, "pool_header.rdb_hdr_unlock_raw",
                                      object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_data_acq_tab" );
          readRdbSliceInfoEntryTab( is, "pool_header.rdb_hdr_data_acq_tab",
                                    object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_nex_tab" );
          readRdbNexType( is, "pool_header.rdb_hdr_nex_tab",
                          object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_nex_abort_tab" );
          readRdbNexType( is, "pool_header.rdb_hdr_nex_abort_tab",
                          object, bswap, revision );
        END_SECTION;

      END_SECTION;


      BEGIN_SECTION( "IMAGE_HEADER" );

        readStringItem( is, itemR_INT8, getToolsDataSize( revision ),
                        "pool_header", "rdb_hdr_tool",
                        object );
        ENDL;

        BEGIN_SECTION( "rdb_hdr_exam" );
          readExamDataType( is, "pool_header.rdb_hdr_exam",
                            object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_series" );
          readSeriesDataType( is, "pool_header.rdb_hdr_series",
                              object, bswap, revision );
        END_SECTION;

        BEGIN_SECTION( "rdb_hdr_image" );
          readMrImageDataType( is, "pool_header.rdb_hdr_image",
                               object, bswap, revision );
        END_SECTION;

      END_SECTION;

    END_SECTION;

    is.close();

    // adding size(s)

    int16_t rdb_hdr_da_xres;
    int16_t rdb_hdr_da_yres;
    int16_t start_rcv, stop_rcv;
    
    object.getHeader().getAttribute( "pool_header.rdb_hdr_rec.rdb_hdr_da_xres",
                                     rdb_hdr_da_xres );
    object.getHeader().getAttribute( "pool_header.rdb_hdr_rec.rdb_hdr_da_yres",
                                     rdb_hdr_da_yres );
    object.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                               start_rcv );
    object.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );

    // need for skipping baseline in some sequences
    std::string psd_iname;
    object.getHeader().getAttribute( "pool_header.rdb_hdr_image.psd_iname",
                                     psd_iname );


    int32_t sizeY = 0, sizeZ = 0, sizeT = 0;
    if ( ( psd_iname == "GkgDwSeEpi" ) ||
         ( psd_iname == "GkgBoldGeEpi" ) ||
         ( psd_iname == "NmrDwSeEpi" ) ||
         ( psd_iname == "NmrBoldGeEpi" ) )
    {

      int16_t se_nacq;
      int32_t se_numimages;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_nacq",
                                      se_nacq );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_numimages",
                                      se_numimages );
      sizeY = ( int32_t )rdb_hdr_da_yres - 1;
      sizeZ = ( int32_t )se_numimages;
      sizeT = ( int32_t )se_nacq * ( int32_t )( stop_rcv - start_rcv + 1 );

    }
    else if ( ( psd_iname == "GkgB0Map" ) ||
              ( psd_iname == "NmrB0Map" ) )
    {

      int16_t rdb_hdr_nechoes;
      int32_t se_numimages;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_rec.rdb_hdr_nechoes",
                                      rdb_hdr_nechoes );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_numimages",
                                      se_numimages );
      sizeY = ( int32_t )rdb_hdr_da_yres - 1;
      sizeZ = ( int32_t )se_numimages / ( int32_t )rdb_hdr_nechoes;
      sizeT = ( int32_t )rdb_hdr_nechoes * ( int32_t )( stop_rcv - 
                                                        start_rcv + 1 );

    }
    else if ( ( psd_iname == "GkgB1Map" ) ||
              ( psd_iname == "NmrB1Map" ) )
    {

      int16_t se_nacq;
      int32_t se_numimages;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_nacq",
                                      se_nacq );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_numimages",
                                      se_numimages );
      sizeY = ( int32_t )rdb_hdr_da_yres - 1;
      sizeZ = ( int32_t )se_numimages;
      sizeT = ( int32_t )se_nacq * ( int32_t )( stop_rcv - start_rcv + 1 );

    }
    else if ( ( psd_iname == "GkgGe" ) ||
              ( psd_iname == "NmrGe" ) )
    {

      int16_t rdb_hdr_nslices, rdb_hdr_npasses;
      float nex;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_rec.rdb_hdr_nslices",
                                      rdb_hdr_nslices );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_rec.rdb_hdr_npasses",
                                      rdb_hdr_npasses );
      object.getHeader().getAttribute( "pool_header.rdb_hdr_image.nex",
                                       nex );

      sizeY = ( int32_t )rdb_hdr_da_yres - 1;
      sizeZ = ( int32_t )rdb_hdr_nslices / ( int32_t )rdb_hdr_npasses;
      sizeT = ( int32_t )nex * ( int32_t )( stop_rcv - start_rcv + 1 );

    }
    else if ( ( psd_iname == "GkgDwSeEvi" ) ||
              ( psd_iname == "NmrDwSeEvi" ) )
    {

      int16_t se_nacq, slquant;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_series.se_nacq",
                                      se_nacq );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_image.slquant",
                                      slquant );
      sizeY = ( int32_t )( ( rdb_hdr_da_yres - 1 ) / slquant );
      sizeZ = ( int32_t )slquant;
      sizeT = ( int32_t )se_nacq * ( int32_t )( stop_rcv - start_rcv + 1 );

    }
    else   /* the default case */
    {

      int16_t rdb_hdr_nslices, rdb_hdr_npasses;
      float nex;
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_rec.rdb_hdr_nslices",
                                      rdb_hdr_nslices );
      object.getHeader().getAttribute(
                                      "pool_header.rdb_hdr_rec.rdb_hdr_npasses",
                                      rdb_hdr_npasses );
      object.getHeader().getAttribute( "pool_header.rdb_hdr_image.nex",
                                       nex );

      sizeY = ( int32_t )rdb_hdr_da_yres - 1;
      sizeZ = ( int32_t )rdb_hdr_nslices / ( int32_t )rdb_hdr_npasses;
      sizeT = ( int32_t )nex * ( int32_t )( stop_rcv - start_rcv + 1 );

    }

    object.getHeader()[ "sizeX" ] = ( int32_t )rdb_hdr_da_xres;
    object.getHeader()[ "sizeY" ] = sizeY;
    object.getHeader()[ "sizeZ" ] = sizeZ;
    object.getHeader()[ "sizeT" ] = sizeT;

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::read(
                                const std::string& name,
                                gkg::Volume< std::complex< T > >& object ) const
{

  try
  {

    // preparing partial binary read
    gkg::VolumeProxy< std::complex< T > > proxy;
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
             "void gkg::GehcSignaPFileDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
inline
void gkg::GehcSignaPFileDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy ) )
    {

      gkg::VolumeProxy< std::complex< T > >& volumeProxy =
        dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy );

      // reading header(s)
      readHeader( name, volumeProxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName",
                                        this->getNameWithoutExtension( name ) );

      // getting byte order and revision
      std::ifstream is( ( this->getNameWithoutExtension( name ) +
                          ".7" ).c_str() );
      if ( !is )
      {

        throw gkg::FileNotFoundException(
                                 this->getNameWithoutExtension( name ) + ".7" );

      }
      bool bswap = false;
      float revision = -1.0;
      getBSwapAndRevision( is, bswap, revision );
      is.close();

      volumeProxy.getHeader().addAttribute( "proxyBSwap", ( bswap ? 1 : 0 ) );
      volumeProxy.getHeader().addAttribute( "proxyRevision", revision );

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::GehcSignaPFileDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::partialBinaryRead(
                         const gkg::HeaderedObject& proxy,
                         gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {

    if ( &dynamic_cast< const gkg::VolumeProxy< std::complex< T > >& >( 
                                                                       proxy ) )
    {

      const gkg::VolumeProxy< std::complex< T > >& volumeProxy =
        dynamic_cast< const gkg::VolumeProxy< std::complex< T > >& >( proxy );

      if ( &dynamic_cast< gkg::Volume< std::complex< T > >& >( subpart ) )
      {

        gkg::Volume< std::complex< T > >& subVolume =
          dynamic_cast< gkg::Volume< std::complex< T > >& >( subpart );

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

            // opening PFile
            std::ifstream is( ( this->getNameWithoutExtension( proxyFileName ) +
                                ".7" ).c_str() );
            if ( !is )
            {

              throw gkg::FileNotFoundException(
                        this->getNameWithoutExtension( proxyFileName ) + ".7" );

            }


            // checking bounding box
            gkg::BoundingBox< int >
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
            gkg::ItemReader< int16_t >& itemR =
            gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader(
                                         false, ( proxyBSwap ? true : false ) );

            readPartialBinaryCore( is,
                                   itemR,
                                   volumeProxy,
                                   subVolume,
                                   boundingBox,
                                   proxyRevision );
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
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::GehcSignaPFileDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::setLogStream( std::ostream& logStream )
{

  _logStream = &logStream;

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::getBSwapAndRevision( 
                                                         std::istream& is,
                                                         bool& bswap,
                                                         float& revision ) const
{

  try
  {

    if ( !is )
    {

      throw std::runtime_error( "bad input stream" );

    }

    // remember current position and go to start of PFile
    off_t position = is.tellg();
    is.seekg( 0U, gkg::IosBase::beg );

    // get byte order: rev. 7, rev. 8 -> SGI host => big endian
    //                 rev. 9 -> Linux host => little endian
    gkg::ItemReader< float >& itemR_FLOAT_swapped =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   true );
    bswap = false;
    revision = -1.0;
    itemR_FLOAT_swapped.read( is, revision );
    if ( ( revision == 7.0 ) ||
         ( revision == 8.0 ) ||
         ( revision == 9.0 ) )
    {

      bswap = true;

    }
    else
    {

      gkg::ItemReader< float >& itemR_FLOAT_unswapped =
        gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                     false );
      is.seekg( 0U, gkg::IosBase::beg );
      itemR_FLOAT_unswapped.read( is, revision );
      if ( ( revision == 7.0 ) ||
           ( revision == 8.0 ) ||
           ( revision == 9.0 ) )
      {

        bswap = false;

      }
      else
      {

        throw std::runtime_error( "not a valid revision number" );

      }

    }

    // rewinding the file to the initial position
    is.seekg( position, gkg::IosBase::beg );


  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::getBSwapAndRevision( "
             "bool& bswap, float& revision )" );

}


template < class T >
size_t 
gkg::GehcSignaPFileDiskFormat< T >::getSliceFactor( float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return 1;

      case 8:
      case 9:

        return 2;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t "
             "gkg::GehcSignaPFileDiskFormat< T >::getSliceFactor( "
             "float revision ) const" );

}


template < class T >
size_t 
gkg::GehcSignaPFileDiskFormat< T >::getRdbHeaderRecSize( float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_RDB_HEADER_REC_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t "
             "gkg::GehcSignaPFileDiskFormat< T >::getRdbHeaderRecSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaPFileDiskFormat< T >::getRdbPerPassTabSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_RDB_PER_PASS_TAB_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t gkg::GehcSignaPFileDiskFormat< T >::getRdbPerPassTabSize( "
             "float revision ) const" );

}


template < class T >
size_t 
gkg::GehcSignaPFileDiskFormat< T >::getRdbNexTypeSize( float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_RDB_NEX_TYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t gkg::GehcSignaPFileDiskFormat< T >::getRdbNexTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t 
gkg::GehcSignaPFileDiskFormat< T >::getToolsDataSize( float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_TOOLSDATA_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t gkg::GehcSignaPFileDiskFormat< T >::getToolsDataSize( "
             "float revision ) const" );

}


template < class T >
size_t 
gkg::GehcSignaPFileDiskFormat< T >::getExamDataTypeSize( float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_EXAMDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaPFileDiskFormat< T >::getExamDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaPFileDiskFormat< T >::getSeriesDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_SERIESDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaPFileDiskFormat< T >::getSeriesDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaPFileDiskFormat< T >::getMrImageDataTypeSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_MRIMAGEDATATYPE_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaPFileDiskFormat< T >::getMrImageDataTypeSize( "
             "float revision ) const" );

}


template < class T >
size_t gkg::GehcSignaPFileDiskFormat< T >::getPoolHeaderSize(
                                                          float revision ) const
{

  try
  {

    switch ( ( int32_t )( revision + 0.5 ) )
    {

      case 7:

        return GEHC_SIGNA_PFILE_HEADER_SIZE_REV_7_x;

      case 8:

        return GEHC_SIGNA_PFILE_HEADER_SIZE_REV_8_x;

      case 9:

        return GEHC_SIGNA_PFILE_HEADER_SIZE_REV_9_x;

    }
    throw std::runtime_error( "not a valid revision number" );
    return 0U;

  }
  GKG_CATCH( "template < class T > "
             "size_t " 
             "gkg::GehcSignaPFileDiskFormat< T >::getPoolHeaderSize( "
             "float revision ) const" );

}


template < class T >
void 
gkg::GehcSignaPFileDiskFormat< T >::skipPadding( std::istream& is,
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
             "gkg::GehcSignaPFileDiskFormat< T >::skipPadding( "
             "std::istream& is, "
             "off_t start, "
             "size_t size ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readPartialBinaryCore(
                       std::istream& is,
                       const gkg::ItemReader< int16_t >& itemR,
                       const gkg::VolumeProxy< std::complex< T > >& volumeProxy,
                       gkg::Volume< std::complex< T > >& subVolume,
                       const gkg::BoundingBox< int32_t >& boundingBox,
                       float revision ) const
{

  try
  {

    typename gkg::Volume< std::complex< T > >::iterator s = subVolume.begin();
    int32_t x, y, z, t, sizeX = subVolume.getSizeX();

    int32_t onDiskSizeX = volumeProxy.getSizeX();
    int32_t onDiskSizeY = volumeProxy.getSizeY();
    int32_t onDiskSizeZ = volumeProxy.getSizeZ();
    int32_t onDiskSizeT = volumeProxy.getSizeT();

    // managing offset to start of image data which is different between 
    // 7.x, 8.x and 9.x RDBM release(s)
    size_t offset = getPoolHeaderSize( revision );

    // need for skipping baseline in some sequences
    std::string psd_iname;
    volumeProxy.getHeader().getAttribute( "pool_header.rdb_hdr_image.psd_iname",
                                          psd_iname );


    if ( ( psd_iname == "GkgDwSeEpi" ) ||
         ( psd_iname == "GkgBoldGeEpi" ) ||
         ( psd_iname == "NmrDwSeEpi" ) ||
         ( psd_iname == "NmrBoldGeEpi" ) )
    {

      // things to be done are different wether we use old 2GB limit acquisition
      // scheme or new real time P-file transfer without memory limitation
      int16_t rdb_hdr_exec_ctrl;
      volumeProxy.getHeader().getAttribute(
                                    "pool_header.rdb_hdr_rec.rdb_hdr_exec_ctrl",
                                    rdb_hdr_exec_ctrl );
      if ( rdb_hdr_exec_ctrl == 27650 )
      {

        //
        // case without memory limitation
        // 27650 = RDB_AUTO_LOCK |                 -->     2
        //         RDB_RTD_XFER_IM_REMOTE |        -->  1024
        //         RDB_RTD_SCAN |                  -->  2048
        //         RDB_DONT_WRITE_OR_INSTALL |     -->  8192
        //         RDB_RTD_XFER_ALL_IM_PER_PASS    --> 16384
        //

        int16_t start_rcv, stop_rcv;
        volumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
        volumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
        int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );

        int32_t rdb_hdr_raw_pass_size;
        volumeProxy.getHeader().getAttribute(
                                "pool_header.rdb_hdr_rec.rdb_hdr_raw_pass_size",
                                rdb_hdr_raw_pass_size );
        int32_t rdb_hdr_sspsave;
        volumeProxy.getHeader().getAttribute(
                                "pool_header.rdb_hdr_rec.rdb_hdr_sspsave",
                                rdb_hdr_sspsave );



        float opuser0;
        volumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user0", opuser0 );
        int32_t sliceOrder = ( int32_t )( opuser0 + 0.5 );
        int32_t trueZ, onDiskZ;

        float start_loc, end_loc;
        volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
        volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
        bool reverseZ = ( start_loc < end_loc );

        int32_t onDiskSizeXY = onDiskSizeX * onDiskSizeY;
        int32_t onDiskSizeXYZ = onDiskSizeXY * onDiskSizeZ;
        int32_t onDiskSizeXZ = onDiskSizeX * onDiskSizeZ;

        // reading data : here, data are arranged in the P-file in such a way
        //                  - all x samples
        //                  - all y lines
        //                  - all z slices
        //                  - all c coils
        //                  - all r repetitions
        std::vector< int16_t > bufferS16( 2 * sizeX );
        for ( t = 0; t < subVolume.getSizeT(); t++ )
        {

          // reversing r and c coordinates
          int32_t repetition = ( t + boundingBox.getLowerT() ) / coilCount;
          int32_t coil = ( t + boundingBox.getLowerT() ) % coilCount;

          for ( z = 0; z < subVolume.getSizeZ(); z++ )
          {

            // managing z reversal
            trueZ = ( z + boundingBox.getLowerZ() );
            if ( reverseZ )
            {

              trueZ = onDiskSizeZ - 1 - trueZ;

            }
            if ( sliceOrder == 0 )
            {

              onDiskZ = trueZ;

            }
            else
            {

              onDiskZ = trueZ / 2 +
                        ( trueZ % 2 ? ( onDiskSizeZ / 2 + onDiskSizeZ % 2 ) :
                                      0 );

            }

            // acquiring a k-space
            for ( y = 0; y < subVolume.getSizeY(); y++ )
            {

              is.seekg( ( off_t )( offset +
                        repetition * ( rdb_hdr_raw_pass_size +
                                       rdb_hdr_sspsave ) +
                        ( coil * ( onDiskSizeXYZ + onDiskSizeXZ ) +
                          onDiskZ * onDiskSizeXY +
                          ( y + boundingBox.getLowerY() ) * onDiskSizeX
                        ) * sizeof( int16_t ) * 2 ),
                        gkg::IosBase::beg );
              itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
              for ( x = 0; x < sizeX; x++ )
              {

                *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                        ( T )bufferS16[ 2 * x + 1 ] );
                ++ s;

              }

            }

          }

        }

      }
      else
      {

        //
        // case with memory limitation
        // 2 = RDB_AUTO_LOCK                   -->     2
        //

        int16_t start_rcv, stop_rcv;
        volumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
        volumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
        int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );
        int32_t repetitionCount = onDiskSizeT / coilCount;


        float opuser0;
        volumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user0", opuser0 );
        int32_t sliceOrder = ( int32_t )( opuser0 + 0.5 );
        int32_t trueZ, onDiskZ;

        float start_loc, end_loc;
        volumeProxy.getHeader().getAttribute(
                                         "pool_header.rdb_hdr_series.start_loc",
                                         start_loc );
        volumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_series.end_loc", end_loc );
        bool reverseZ = ( start_loc < end_loc );

        int32_t onDiskSizeXY = onDiskSizeX * onDiskSizeY;
        int32_t onDiskSizeXYZ = onDiskSizeXY * onDiskSizeZ;

        int32_t extraOffset = onDiskSizeX * onDiskSizeZ * repetitionCount *
                              sizeof( int16_t ) * 2;

        // reading data : here, data are arranged in the P-file in such a way
        //                  - all x samples
        //                  - all y lines
        //                  - all z slices
        //                  - all c coils
        //                  - all r repetitions
        // but we would like to have the following order:
        //                  - all x samples
        //                  - all y lines
        //                  - all z slices
        //                  - all c coils
        //                  - all r repetitions
        // so we need to do some cooking in order to reverse r and c coordinates
        std::vector< int16_t > bufferS16( 2 * sizeX );
        for ( t = 0; t < subVolume.getSizeT(); t++ )
        {

          // reversing r and c coordinates
          int32_t repetition = ( t + boundingBox.getLowerT() ) / coilCount;
          int32_t coil = ( t + boundingBox.getLowerT() ) % coilCount;
          int32_t tprime = repetition + coil * repetitionCount;

          for ( z = 0; z < subVolume.getSizeZ(); z++ )
          {

            // managing z reversal
            trueZ = ( z + boundingBox.getLowerZ() );
            if ( reverseZ )
            {

              trueZ = onDiskSizeZ - 1 - trueZ;

            }
            if ( sliceOrder == 0 )
            {

              onDiskZ = trueZ;

            }
            else
            {

              onDiskZ = trueZ / 2 +
                        ( trueZ % 2 ? ( onDiskSizeZ / 2 + onDiskSizeZ % 2 ) :
                                      0 );

            }

            // acquiring a k-space
            for ( y = 0; y < subVolume.getSizeY(); y++ )
            {

              is.seekg( ( off_t )( offset +
                        coil * extraOffset +
                        ( boundingBox.getLowerX() +
                        ( y + boundingBox.getLowerY() ) * onDiskSizeX +
                        onDiskZ * onDiskSizeXY +
                        tprime * onDiskSizeXYZ )
                        * sizeof( int16_t ) * 2 ),
                        gkg::IosBase::beg );
              itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
              for ( x = 0; x < sizeX; x++ )
              {

                *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                        ( T )bufferS16[ 2 * x + 1 ] );
                ++ s;

              }

            }

          }

        }

      }

    }
    else if ( psd_iname == "EPI" )
    {

      float opuser3;
      volumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user3", opuser3 );
      int32_t sliceOrder = ( ( ( int32_t )( opuser3 + 0.5 ) == 0 ) ? 1 : 0 );

      int32_t trueZ, onDiskZ;

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );

      int32_t onDiskSizeXY = onDiskSizeX * onDiskSizeY;
      int32_t onDiskSizeXYZ = onDiskSizeXY * onDiskSizeZ;


      // reading data
      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          trueZ = ( z + boundingBox.getLowerZ() );
          if ( reverseZ )
          {

            trueZ = onDiskSizeZ - 1 - trueZ;

          }
          if ( sliceOrder == 0 )
          {

            onDiskZ = trueZ;

          }
          else
          {

            onDiskZ = trueZ / 2 +
                      ( trueZ % 2 ? ( onDiskSizeZ / 2 + onDiskSizeZ % 2 ) : 0 );

          }
          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            is.seekg( ( off_t )( offset +
                      ( boundingBox.getLowerX() +
                      ( y + boundingBox.getLowerY() ) * onDiskSizeX +
                      onDiskZ * onDiskSizeXY +
                      ( t + boundingBox.getLowerT() ) * onDiskSizeXYZ )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );
            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else if ( ( psd_iname == "EPI2" ) ||
              ( psd_iname == "SE_2D_Std_Std" ) )
    {

      int32_t trueZ, onDiskZ;

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );

      int32_t onDiskSizeXY = onDiskSizeX * ( onDiskSizeY + 1 /* baseline */ );
      int32_t onDiskSizeXYZ = onDiskSizeXY * onDiskSizeZ;

      // reading data
      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          trueZ = ( z + boundingBox.getLowerZ() );
          if ( reverseZ )
          {

            trueZ = onDiskSizeZ - 1 - trueZ;

          }
          onDiskZ = trueZ / 2 +
                    ( trueZ % 2 ? ( onDiskSizeZ / 2 + onDiskSizeZ % 2 ) : 0 );
          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            is.seekg( ( off_t )( offset +
                      ( boundingBox.getLowerX() +
                      ( y + boundingBox.getLowerY() + 1 /*baseline to skeep*/ )
                                                                * onDiskSizeX +
                      onDiskZ * onDiskSizeXY +
                      ( t + boundingBox.getLowerT() ) * onDiskSizeXYZ )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );
            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else if ( ( psd_iname == "GkgB0Map" ) ||
              ( psd_iname == "NmrB0Map" ) )
    {

      int16_t start_rcv, stop_rcv;
      volumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
      volumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
      int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );
      int32_t echoCount = onDiskSizeT / coilCount;

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );
      int32_t onDiskZ;

      int32_t onDiskSizeXY = onDiskSizeX * ( onDiskSizeY + 1 );

      // reading data : here, data are arranged in the P-file in such a way
      //                  - all x samples
      //                  - all y lines
      //                  - all e echoes
      //                  - all z slices
      //                  - all c coils
      // but we would like to have the following order:
      //                  - all x samples
      //                  - all y lines
      //                  - all z slices
      //                  - all c coils
      //                  - all e echoes
      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        int32_t echo = ( t + boundingBox.getLowerT() ) / coilCount;
        int32_t coil = ( t + boundingBox.getLowerT() ) % coilCount;

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          if ( reverseZ )
          {

            onDiskZ = onDiskSizeZ - 1 - ( z + boundingBox.getLowerZ() ) ;

          }
          else
          {

            onDiskZ = ( z + boundingBox.getLowerZ() ) ;

          }

          // reversing e z, and c coordinates
          int32_t B = echo + onDiskZ * echoCount +
                      coil * echoCount * onDiskSizeZ;
 
          is.seekg( ( off_t )( offset +
                      ( boundingBox.getLowerX() + 
                      ( boundingBox.getLowerY() + 1 ) * onDiskSizeX +
                      B * onDiskSizeXY  )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );

          // acquiring a k-space
          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else if ( ( psd_iname == "GkgB1Map" ) ||
              ( psd_iname == "GkgGe" ) ||
              ( psd_iname == "NmrB1Map" ) ||
              ( psd_iname == "NmrGe" ) )
    {

      int16_t start_rcv, stop_rcv;
      volumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
      volumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
      int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );
      int32_t echoCount = onDiskSizeT / coilCount;

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );
      int32_t onDiskZ;

      int32_t onDiskSizeXY = onDiskSizeX * ( onDiskSizeY + 1 );

      // reading data : here, data are arranged in the P-file in such a way
      //                  - all x samples
      //                  - all y lines
      //                  - all e echoes
      //                  - all z slices
      //                  - all c coils
      // but we would like to have the following order:
      //                  - all x samples
      //                  - all y lines
      //                  - all z slices
      //                  - all c coils
      //                  - all e echoes
      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        int32_t echo = ( t + boundingBox.getLowerT() ) / coilCount;
        int32_t coil = ( t + boundingBox.getLowerT() ) % coilCount;

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          if ( reverseZ )
          {

            onDiskZ = onDiskSizeZ - 1 - ( z + boundingBox.getLowerZ() ) ;

          }
          else
          {

            onDiskZ = ( z + boundingBox.getLowerZ() ) ;

          }

          // reversing e z, and c coordinates
          int32_t B = echo + onDiskZ * echoCount +
                      coil * echoCount * onDiskSizeZ;
 
          is.seekg( ( off_t )( offset +
                      ( boundingBox.getLowerX() + 
                      ( boundingBox.getLowerY() + 1 ) * onDiskSizeX +
                      B * onDiskSizeXY  )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );

          // acquiring a k-space
          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else if ( psd_iname == "FGRE" )
    {

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );
      int32_t onDiskZ;

      int32_t onDiskSizeXY = onDiskSizeX * ( onDiskSizeY + 1 /* baseline */ );
      int32_t onDiskSizeXYT = onDiskSizeXY * onDiskSizeT;

      // reading data
      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            if ( reverseZ )
            {

              onDiskZ = onDiskSizeZ - 1 - ( z + boundingBox.getLowerZ() ) ;

            }
            else
            {

              onDiskZ = ( z + boundingBox.getLowerZ() ) ;

            }
            is.seekg( ( off_t )( offset +
                      ( boundingBox.getLowerX() +
                      ( y + boundingBox.getLowerY() + 1 /*baseline offset*/ ) *
                                                        onDiskSizeX +
                      ( t + boundingBox.getLowerT() ) * onDiskSizeXY +
                      onDiskZ * onDiskSizeXYT )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );
            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else if ( ( psd_iname == "GkgDwSeEvi" ) ||
              ( psd_iname == "NmrDwSeEvi" ) )
    {

      //
      // case with memory limitation
      // 2 = RDB_AUTO_LOCK                   -->     2
      //

      int16_t start_rcv, stop_rcv;
      volumeProxy.getHeader().getAttribute(
                           "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                           start_rcv );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                            stop_rcv );
      int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );
      int32_t repetitionCount = onDiskSizeT / coilCount;

      int32_t onDiskZ;

      float start_loc, end_loc;
      volumeProxy.getHeader().getAttribute(
                                       "pool_header.rdb_hdr_series.start_loc",
                                       start_loc );
      volumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );

      int32_t onDiskSizeXY = onDiskSizeX * onDiskSizeY;
      int32_t onDiskSizeXYZ = onDiskSizeXY * onDiskSizeZ;

      int32_t extraOffset = onDiskSizeX * repetitionCount * 
                            sizeof( int16_t ) * 2;

      // reading data : here, data are arranged in the P-file in such a way
      //                  - all x samples
      //                  - all y lines
      //                  - all z slices
      //                  - all r repetitions
      //                  - all c coils
      // but we would like to have the following order:
      //                  - all x samples
      //                  - all y lines
      //                  - all z slices
      //                  - all c coils
      //                  - all r repetitions
      // so we need to do some cooking in order to reverse r and c coordinates

      std::vector< int16_t > bufferS16( 2 * sizeX );
      for ( t = 0; t < subVolume.getSizeT(); t++ )
      {

        // reversing r and c coordinates
        int32_t repetition = ( t + boundingBox.getLowerT() ) / coilCount;
        int32_t coil = ( t + boundingBox.getLowerT() ) % coilCount;
        int32_t tprime = repetition + coil * repetitionCount;

        for ( z = 0; z < subVolume.getSizeZ(); z++ )
        {

          // managing z reversal
          onDiskZ = ( z + boundingBox.getLowerZ() );
          if ( reverseZ )
          {

            onDiskZ = onDiskSizeZ - 1 - onDiskZ;

          }

          // acquiring a k-space
          for ( y = 0; y < subVolume.getSizeY(); y++ )
          {

            is.seekg( ( off_t )( offset +
                      coil * extraOffset +
                      ( boundingBox.getLowerX() +
                      ( y + boundingBox.getLowerY() ) * onDiskSizeX +
                      onDiskZ * onDiskSizeXY +
                      tprime * onDiskSizeXYZ )
                      * sizeof( int16_t ) * 2 ),
                      gkg::IosBase::beg );
            itemR.read( is, &bufferS16[ 0 ], 2 * sizeX );
            for ( x = 0; x < sizeX; x++ )
            {

              *s = std::complex< T >( ( T )bufferS16[ 2 * x ],
                                      ( T )bufferS16[ 2 * x + 1 ] );
              ++ s;

            }

          }

        }

      }

    }
    else
    {

      throw std::runtime_error( std::string( "'" ) + psd_iname + "' is not" +
                                " a managed pulse sequence type" );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readPartialBinaryCore( "
             "std::istream& is, "
             "const gkg::ItemReader< short >* itemR, "
             "const gkg::VolumeProxy< std::complex< T > >& volumeProxy, "
             "gkg::Volume< std::complex< T > >& subVolume, "
             "const gkg::BoundingBox< int >& boundingBox, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readRdbHeaderRec(
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

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_rdbm_rev", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_run_int", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_scan_seq", object );
    readStringItem( is, itemR_INT8, 6, prefix, "rdb_hdr_run_char", object );
    readStringItem( is, itemR_INT8, 10, prefix, "rdb_hdr_scan_date", object );
    readStringItem( is, itemR_INT8, 8, prefix, "rdb_hdr_scan_time", object );
    readStringItem( is, itemR_INT8, 10, prefix, "rdb_hdr_logo", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_file_contents", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_lock_mode", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dacq_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_recon_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_exec_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_scan_type", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_data_collect_type", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_data_format", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_recon", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_datacq", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_npasses", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_npomp", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_nslices", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_nechoes", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_navs", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_nframes", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_baseline_views", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_hnover", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_frame_size", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_point_size", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_vquant", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_cheart", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ctr", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ctrr", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_initpass", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_incrpass", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_method_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_da_xres", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_da_yres", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_rc_xres", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_rc_yres", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_im_size", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_rc_zres", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_raw_pass_size", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_sspsave", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_udasave", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_fermi_radius", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_fermi_width", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_fermi_ecc", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_clip_min", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_clip_max", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_default_offset", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_xoff", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_yoff", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_nwin", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ntran", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scalei", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scaleq", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_rotation", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_transpose", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_kissoff_views", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_slblank", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_gradcoil", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_ddaover", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_sarr", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_fd_tr", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_fd_te", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_fd_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_algor_num", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_fd_df_dec", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[0].start_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[0].stop_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[1].start_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[1].stop_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[2].start_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[2].stop_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[3].start_rcv", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dab[3].stop_rcv", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user0", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user1", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user2", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user3", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user4", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user5", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user6", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user7", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user8", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user9", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user10", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user11", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user12", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user13", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user14", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user15", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user16", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user17", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user18", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user19", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_v_type", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefxa", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefxb", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefxc", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefxd", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefya", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefyb", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefyc", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefyd", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefza", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefzb", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefzc", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_coefzd", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_vm_coef1", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_vm_coef2", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_vm_coef3", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_vm_coef4", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_v_venc", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "spectral_width", object );
    readItem( is, itemR_INT16, prefix, "csi_dims", object );
    readItem( is, itemR_INT16, prefix, "xcsi", object );
    readItem( is, itemR_INT16, prefix, "ycsi", object );
    readItem( is, itemR_INT16, prefix, "zcsi", object );
    readItem( is, itemR_FLOAT, prefix, "roilenx", object );
    readItem( is, itemR_FLOAT, prefix, "roileny", object );
    readItem( is, itemR_FLOAT, prefix, "roilenz", object );
    readItem( is, itemR_FLOAT, prefix, "roilocx", object );
    readItem( is, itemR_FLOAT, prefix, "roilocy", object );
    readItem( is, itemR_FLOAT, prefix, "roilocz", object );
    readItem( is, itemR_FLOAT, prefix, "numdwell", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_command", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_mps_r1", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_mps_r2", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_mps_tg", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_mps_freq", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_aps_r1", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_aps_r2", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_aps_tg", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_aps_freq", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ps_scalei", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ps_scaleq", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_snr_warning", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_aps_or_mps", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_mps_bitmap", object );
    readStringItem( is, itemR_INT8, 256, prefix, "rdb_hdr_ps_powerspec",
                    object);
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_filler1", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ps_filler2", object );
    readListItem( is, itemR_FLOAT, 16, prefix, "rdb_hdr_rec_noise_mean",
                  object );
    readListItem( is, itemR_FLOAT, 16, prefix, "rdb_hdr_rec_noise_std",
                  object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "halfecho", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_im_size_y", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_data_collect_type1", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_freq_scale", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_phase_scale", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_ovl", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_pclin", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pclinnpts", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pclinorder", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pclinavg", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pccon", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcconnpts", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcconorder", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcextcorr", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcgraph", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcileave", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_hdbestky", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcthrespts", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcdiscbeg", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcdiscmid", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcdiscend", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcthrespct", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcspacial", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pctemporal", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcspare", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_ileaves", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_kydir", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_alt", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_reps", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_ref", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pcconnorm", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pcconfitwt", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pclinnorm", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pclinfitwt", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pcbestky", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_vrgf", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_vrgfxres", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_bp_corr", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_recv_freq_s", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_recv_freq_e", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_hniter", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_fast_rec", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_refframes", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_refframep", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_scnframe", object );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_pasframe", object );

    ENDL;

    readItem( is, itemR_UINT32, prefix, "rdb_hdr_user_usage_tag", object );
    readItem( is, itemR_UINT32, prefix, "rdb_hdr_user_fill_mapMSW", object );
    readItem( is, itemR_UINT32, prefix, "rdb_hdr_user_fill_mapLSW", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user20", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user21", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user22", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user23", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user24", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user25", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user26", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user27", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user28", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user29", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user30", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user31", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user32", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user33", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user34", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user35", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user36", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user37", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user38", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user39", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user40", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user41", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user42", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user43", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user44", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user45", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user46", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user47", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_user48", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcfitorig", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcshotfirst", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcshotlast", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcmultegrp", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pclinfix", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcconfix", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pclinslope", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_pcconslope", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pccoil", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_vvsmode", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_vvsaimgs", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_vvstr", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_vvsgender", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_zip_factor", object );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef1a", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef1b", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef1c", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef1d", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef2a", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef2b", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef2c", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef2d", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef3a", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef3b", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef3c", object );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_maxcoef3d", object );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_ut_ctrl", object );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dp_type", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_arw", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_vps", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_mcReconEnable", object );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_fov", object,
              revision, REV8 | REV9 );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_te", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_te2", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_dfmrbw", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_dfmctrl", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_raw_nex", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_navs_per_pass", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_dfmxres", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_dfmptsize", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_navs_per_view", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_dfmdebug", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_dfmthreshold", object,
              revision, REV8 | REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_grid_control", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_b0map", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_grid_tediff", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_grid_motion_comp", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_radius_a", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_radius_b", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_max_gradient", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_max_slew", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_scan_fov", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_a2d_time", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_density_factor", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_grid_display_fov", object,
              revision, REV8 | REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_fatwater", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_fiestamlf", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_app", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_rhncoilsel", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_rhncoillimit", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_app_option", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_grad_mode", object,
              revision, REV8 | REV9 );

    readItem( is, itemR_INT16, prefix, "rdb_hdr_fiestamlf", object,
              revision, REV8 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pfile_passes", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_asset", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_asset_calthresh", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_asset_R", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_coilno", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_asset_phases", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scancent", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_position", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_entry", object,
              revision, REV8 | REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_lmhor", object,
              revision, REV8 | REV9 );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_last_slice_num", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_asset_slice_R", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_asset_slabwrap", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_dwnav_coeff", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_cor", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_view", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_corecho", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_sview", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_eview", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_sshot", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_dwnav_eshot", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_3dwin_type", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_3dwin_apod", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_3dwin_q", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_ime_scic_enable", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_clariview_type", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ime_scic_edge", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ime_scic_smooth", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_ime_scic_focus", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_clariview_edge", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_clariview_smooth", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_clariview_focus", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scic_reduction", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scic_gauss", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "rdb_hdr_scic_threshold", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT32, prefix, "rdb_hdr_ectricks_no_regions", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "rdb_hdr_ectricks_input_regions", 
              object, revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_psc_reuse", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_left_blank", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_right_blank", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_acquire_type", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_retro_control", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_etl", object,
              revision, REV9 );

    ENDL;

    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcref_start", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_pcref_stop", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_ref_skip", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_extra_frames_top", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "rdb_hdr_extra_frames_bot", object,
              revision, REV9 );

    ENDL;

    skipPadding( is, start, getRdbHeaderRecSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readRdbHeaderRec( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readRdbPerPassInfoEntryTab(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float /*revision*/ ) const
{

  try
  {

    gkg::ItemReader< int32_t >& itemR_INT32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );
    int32_t i, j;
    for ( i = 0; i < RDB_MAX_PASSES; i++ )
    {

      for ( j = 0; j < RDB_MAX_DABS; j++ )
      {

        readItem( is, itemR_INT32,
                  prefix + '[' + gkg::StringConverter::toString( i ) +
                  "].dab_bam[" + gkg::StringConverter::toString( j ) + ']',
                  "bam_modifier", object );
        readItem( is, itemR_INT32,
                  prefix + '[' + gkg::StringConverter::toString( i ) +
                  "].dab_bam[" + gkg::StringConverter::toString( j ) + ']',
                  "bam_address", object );

      }    

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::"
             "readRdbPerPassInfoEntryTab( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap, "
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readRdbSliceInfoEntryTab(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float revision ) const
{

  try
  {

    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< float >& itemR_FLOAT =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   bswap );

    int32_t imax = RDB_MAX_SLICES * ( int32_t )getSliceFactor( revision );

    int32_t i;
    for ( i = 0; i < imax; i++ )
    {

      readItem( is, itemR_INT16,
                prefix + '[' + gkg::StringConverter::toString( i ) + ']',
                "pass_number", object );
      readItem( is, itemR_INT16,
                prefix + '[' + gkg::StringConverter::toString( i ) + ']',
                "slice_in_pass", object );
      readListItem( is, itemR_FLOAT, 3,
                    prefix + '[' + gkg::StringConverter::toString( i ) + ']',
                    "gw_point1", object );
      readListItem( is, itemR_FLOAT, 3,
                    prefix + '[' + gkg::StringConverter::toString( i ) + ']',
                    "gw_point2", object );
      readListItem( is, itemR_FLOAT, 3,
                    prefix + '[' + gkg::StringConverter::toString( i ) + ']',
                    "gw_point3", object );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::"
             "readRdbSliceInfoEntryTab( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap,"
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readRdbNexType(
                                                    std::istream& is, 
                                                    const std::string& prefix,
                                                    gkg::HeaderedObject& object,
                                                    bool bswap,
                                                    float revision ) const
{

  try
  {

    gkg::ItemReader< int16_t >& itemR_INT16 =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getReader( false,
                                                                     bswap );

    readListItem( is, itemR_INT16,
                  getRdbNexTypeSize( revision ) / sizeof( int16_t ),
                  prefix, "rdb_hdr_nex_size", object );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::"
             "readRdbNexType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap,"
             "float revision ) const" );

}



template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readExamDataType(
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

    ENDL;

    readStringItem( is, itemR_INT8, 32, prefix, "refsopcuid", object );
    readStringItem( is, itemR_INT8, 32, prefix, "refsopiuid", object );
    readStringItem( is, itemR_INT8, 65, prefix, "patnameff", object );
    readStringItem( is, itemR_INT8, 65, prefix, "patidff", object );
    readStringItem( is, itemR_INT8, 17, prefix, "reqnumff", object );
    readStringItem( is, itemR_INT8, 9, prefix, "dateofbirth", object );
    readStringItem( is, itemR_INT8, 32, prefix, "mwlstudyuid", object );
    readStringItem( is, itemR_INT8, 16, prefix, "mwlstudyid", object );


    skipPadding( is, start, getExamDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::"
             "readExamDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap,"
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readSeriesDataType(
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

    ENDL;

    readStringItem( is, itemR_INT8, 32, prefix, "refsopcuids", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 32, prefix, "refsopiuids", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "schacitval", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "schacitdesc", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 64, prefix, "schacitmea", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 64, prefix, "schprocstdesc", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "schprocstid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "reqprocstid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "perprocstid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 64, prefix, "perprocstdesc", object,
                    revision, REV9 );

    ENDL;

    readStringItem( is, itemR_INT8, 16, prefix, "landmark_uid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "landmark_uid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "landmark_uid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "landmark_uid", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 32, prefix, "landmark_uid", object,
                    revision, REV9 );
    readItem( is, itemR_INT16, prefix, "table_entry", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "SwingAngle", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "LateralOffset", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "GradientCoil", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "se_subtype", object,
              revision, REV9 );
    readItem( is, itemR_INT16, prefix, "BWRT", object,
              revision, REV9 );
    readStringItem( is, itemR_INT8, 64, prefix, "PdgmStr", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 256, prefix, "PdgmDesc", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 64, prefix, "PdgmUID", object,
                    revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ExpType", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "TrRest", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "TrActive", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "DumAcq", object,
              revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "ApplName", object,
                    revision, REV9 );
    readStringItem( is, itemR_INT8, 16, prefix, "ApplVer", object,
                    revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ExptTimePts", object,
              revision, REV9 );

    skipPadding( is, start, getSeriesDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readSeriesDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap,"
             "float revision ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readMrImageDataType(
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
    gkg::ItemReader< int32_t >& itemR_INT32=
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
    readItem( is, itemR_INT16, prefix, "im_no", object );
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
    readItem( is, itemR_FLOAT, prefix, "loc", object );
    readItem( is, itemR_FLOAT, prefix, "ctr_R", object );
    readItem( is, itemR_FLOAT, prefix, "ctr_A", object );
    readItem( is, itemR_FLOAT, prefix, "ctr_S", object );
    readItem( is, itemR_FLOAT, prefix, "norm_R", object );
    readItem( is, itemR_FLOAT, prefix, "norm_A", object );
    readItem( is, itemR_FLOAT, prefix, "norm_S", object );
    readItem( is, itemR_FLOAT, prefix, "tlhc_R", object );
    readItem( is, itemR_FLOAT, prefix, "tlhc_A", object );
    readItem( is, itemR_FLOAT, prefix, "tlhc_S", object );
    readItem( is, itemR_FLOAT, prefix, "trhc_R", object );
    readItem( is, itemR_FLOAT, prefix, "trhc_A", object );
    readItem( is, itemR_FLOAT, prefix, "trhc_S", object );
    readItem( is, itemR_FLOAT, prefix, "brhc_R", object );
    readItem( is, itemR_FLOAT, prefix, "brhc_A", object );
    readItem( is, itemR_FLOAT, prefix, "brhc_S", object );
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
    readItem( is, itemR_INT32, prefix, "rawrunnum", object );
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
    readItem( is, itemR_UINT32, prefix, "im_checksum", object );
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

    ENDL;

    readItem( is, itemR_INT32, prefix, "slop_int_6", object,
              revision, REV7 );
    readItem( is, itemR_INT32, prefix, "iopt2", object,
              revision, REV7 );
    readItem( is, itemR_INT32, prefix, "slop_int_8", object,
              revision, REV7 );
    readItem( is, itemR_INT32, prefix, "slop_int_9", object,
              revision, REV7 );

    readItem( is, itemR_INT32, prefix, "b_value", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "iopt2", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ihtagging", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ihtagspc", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ihfcineim", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "ihfcinent", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "num_seg", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "oprtarr", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "averages", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "station_index", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "station_total", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "slop_int_17", object,
              revision, REV9 );
    readItem( is, itemR_FLOAT, prefix, "RegressorVal", object,
              revision, REV9 );
    readItem( is, itemR_INT32, prefix, "delAcq", object,
              revision, REV9 );

    skipPadding( is, start, getSeriesDataTypeSize( revision ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readMrImageDataType( "
             "std::istream& is, "
             "const std::string& prefix, "
             "gkg::HeaderedObject& object, "
             "bool bswap,"
             "float revision ) const" );

}


template < class T > template < class U >
void gkg::GehcSignaPFileDiskFormat< T >::readItem(
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

    if ( ( ( ( int32_t )( revision + 0.5 ) == 7 ) && 
           ( revisionMask & REV7 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
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
      U data;
      itemR.read( is, data );

      // adding object to header
      std::string attributeName = prefix + '.' + name;
      object.addSemantic( "__gehc_signa_pfile__",
                          attributeName,
                          gkg::Semantic( gkg::TypeOf< U >::getName() ) );
      object.getHeader().addAttribute( attributeName, data );

      // logging read data
      if ( _logStream )
      {

        *_logStream << _indent.getSpaces()
                    << attributeName
                    << " [" << gkg::TypeOf< U >::getName() << "]"
                    << " = "
                    << data
                    << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readItem( "
             "std::istream& is, "
             "const gkg::ItemReader< U >& itemR, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object,"
             "float revision,"
             "int revisionMask ) const" );

}


template < class T > template < class U >
void gkg::GehcSignaPFileDiskFormat< T >::readListItem(
                                             std::istream& is, 
                                             const gkg::ItemReader< U >& itemR,
                                             size_t count,
                                             const std::string& prefix,
                                             const std::string& name,
                                             gkg::HeaderedObject& object,
                                             float revision,
                                             int32_t revisionMask ) const
{

  try
  {

    if ( ( ( ( int32_t )( revision + 0.5 ) == 7 ) && 
           ( revisionMask & REV7 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {      

      for ( size_t n = 0; n < count; n++ )
      {

        readItem( is, itemR, prefix,
                  name + '[' + gkg::StringConverter::toString( ( uint64_t )n )
                       + ']',
                  object,
                  revision,
                  revisionMask );

      }

    }

  }
  GKG_CATCH( "template < class T > template < class U > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readListItem( "
             "std::istream& is, "
             "const gkg::ItemReader< U >& itemR, "
             "size_t count, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object,"
             "float revision,"
             "int revisionMask ) const" );

}


template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readStringItem(
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

    if ( ( ( ( int32_t )( revision + 0.5 ) == 7 ) && 
           ( revisionMask & REV7 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
           ( revisionMask & REV8 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 9 ) && 
           ( revisionMask & REV9 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 0 ) && 
           ( revisionMask == 0 ) ) )
    {      

      // aligning bytes
      char dummy;
      while ( is.tellg() % sizeof( int8_t ) )
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
      object.addSemantic( "__gehc_signa_pfile__",
                          attributeName,
                      gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      if ( gkg::ConfigurationInfo::getInstance().hasAnonymousName() &&
           ( ( name == "patname" ) || ( name == "patnameff" ) ) )
      {

        data_string = "anonymous";

      }
      object.getHeader().addAttribute( attributeName, data_string );

      // logging read data
      if ( _logStream )
      {

        *_logStream << _indent.getSpaces()
                    << attributeName
                    << " [" << gkg::TypeOf< std::string >::getName() << "]"
                    << " = \""
                    << data_string
                    << "\""
                   << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readStringItem( "
             "std::istream& is, "
             "const gkg::ItemReader< int8_t >& itemR, "
             "size_t count, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object,"
             "float revision,"
             "int revisionMask ) const" );

}



template < class T >
void gkg::GehcSignaPFileDiskFormat< T >::readVarTypeItem(
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

    if ( ( ( ( int32_t )( revision + 0.5 ) == 7 ) && 
           ( revisionMask & REV7 ) ) ||
         ( ( ( int32_t )( revision + 0.5 ) == 8 ) && 
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
      object.addSemantic( "__gehc_signa_pfile__",
                          attributeName,
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
      object.getHeader().addAttribute( attributeName, data_string );

      if ( _logStream )
      {

        *_logStream << _indent.getSpaces()
                    << attributeName
                    << " [var_type]"
                    << " = \""
                    << data_string
                    << "\""
                    << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GehcSignaPFileDiskFormat< T >::readVarTypeItem( "
             "std::istream& is, "
             "const gkg::ItemReader< int8_t >& itemR_INT8, "
             "const gkg::ItemReader< int32_t >& itemR_INT32, "
             "const gkg::ItemReader< uint32_t >& itemR_UINT32, "
             "const std::string& prefix, "
             "const std::string& name, "
             "gkg::HeaderedObject& object,"
             "float revision,"
             "int revisionMask ) const" );

}


template class gkg::GehcSignaPFileDiskFormat< float >;
template class gkg::GehcSignaPFileDiskFormat< double >;


//
// registrating standard GEHC Signa PFile disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > 
                           >::getInstance().registerDiskFormat(
         gkg::GehcSignaPFileDiskFormat< float >::getInstance().getName(),
         &gkg::GehcSignaPFileDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< double > > 
                           >::getInstance().registerDiskFormat(
         gkg::GehcSignaPFileDiskFormat< double >::getInstance().getName(),
         &gkg::GehcSignaPFileDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "GEHC Signa PFile disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();


#undef GEHC_SIGNA_PFILE_HEADER_SIZE
#undef ENDL
#undef BEGIN_SECTION
#undef END_SECTION
#undef REV7
#undef REV8
#undef REV9
