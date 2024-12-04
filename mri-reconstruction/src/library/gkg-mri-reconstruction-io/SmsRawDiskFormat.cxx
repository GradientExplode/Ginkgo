#include <gkg-mri-reconstruction-io/SmsRawDiskFormat.h>
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
#include <iomanip>


//
// class SmsRawDiskFormat
//


template < class T >
gkg::SmsRawDiskFormat< T >::SmsRawDiskFormat()
                           : gkg::TypedDiskFormat<
                                            gkg::Volume< std::complex< T > > >()
{

  this->_extensions.insert( ".dat" );

}


template < class T >
gkg::SmsRawDiskFormat< T >::~SmsRawDiskFormat()
{
}


template < class T >
std::string gkg::SmsRawDiskFormat< T >::getName() const
{

  return "sms-raw";

}


template < class T >
void gkg::SmsRawDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // determining byte swapping (we know that data are saved using a
    // little endian memory model
    bool bswap = false;
    if ( gkg::ByteOrder::getInstance().getCurrentType() !=
         gkg::ByteOrder::LittleEndian )
    {

      bswap = true;

    }

    // reading the <*.dat> header
    std::ifstream is( ( this->getNameWithoutExtension( name ) +
                        ".dat" ).c_str() );

    if ( !is )
    {

      throw gkg::FileNotFoundException( this->getNameWithoutExtension( name ) + 
                                        ".dat" );

    }

    int32_t sizeX = 1;
    int32_t sizeY = 1;
    int32_t sizeZ = 1;
    int32_t sizeT = 1;

    // reading MrProt and Yaps headers
    gkg::ItemReader< uint32_t >& itemR_Uint32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );

    gkg::ItemReader< int8_t >& itemR_Int8 =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getReader( false,
                                                                    bswap );

    uint32_t headerSize = 0U;
    itemR_Uint32.read( is, headerSize );
    object.getHeader()[ "sms_header_size" ] = headerSize;

    std::string ascConv = "xxxxxxxxxxxxxxxxxxxxx";
    int32_t c = 0;
    int8_t ch = 0;
    for ( c = 0; c < ( int32_t )ascConv.length(); c++ )
    {

      itemR_Int8.read( is, ch );
      ascConv[ c ] = ( char )ch;

    }

    while ( ascConv != "### ASCCONV BEGIN ###" )
    {

      itemR_Int8.read( is, ch );
      ascConv = ascConv.substr( 1, ascConv.length() - 1 ) + ( char )ch;

    }
    // skipping line return
    itemR_Int8.read( is, ch );

    char lineChar[ 256 ];
    std::string line;
    std::string name;
    std::string equal;
    std::string value;
    do
    {

      is.getline( lineChar, 256 );

      line = std::string( lineChar );

      if ( line != "### ASCCONV END ###" )
      {

        std::istringstream iss( lineChar );
        iss >> name >> equal;

        if ( getType( name ) == gkg::TypeOf< int32_t >::getName() )
        {

           int32_t value = 0;
           iss >> value;
           object.getHeader()[ name ] = value;

        }
        else if ( getType( name ) == gkg::TypeOf< uint32_t >::getName() )
        {

           uint32_t value = 0U;
           iss >> std::setbase( 16 ) >> value;
           object.getHeader()[ name ] = ( int32_t )value;

        }
        else if ( getType( name ) == gkg::TypeOf< double >::getName() )
        {

           double value = 0.0;
           iss >> value;
           object.getHeader()[ name ] = value;

        }
        else if ( getType( name ) == gkg::TypeOf< std::string >::getName() )
        {

           std::string value;
           iss >> value;
           object.getHeader()[ name ] = value.substr( 1U, value.length() - 2U );

        }

      }

    }
    while ( line != "### ASCCONV END ###" );

    // collecting pulse sequence name
    std::string pulseSequenceName;
    object.getHeader().getAttribute( "tSequenceFileName", pulseSequenceName );

    // collecting channel count
    int32_t channelCount = 0;
    while ( object.getHeader().hasAttribute(
              std::string( "asCoilSelectMeas[0].aFFT_SCALE[" ) +
              gkg::StringConverter::toString( channelCount ) +
              "].lRxChannel" ) )
    {

      ++ channelCount;

    }

    // filling raw volume size according to pulse sequence type
    if ( pulseSequenceName == "%CustomerSeq%\\evi_simple" )
    {

      object.getHeader().getAttribute( "sKSpace.lBaseResolution",
                                       sizeX );
      object.getHeader().getAttribute( "sKSpace.lPhaseEncodingLines",
                                       sizeY );
      object.getHeader().getAttribute( "sKSpace.lPartitions",
                                       sizeZ );
      if ( object.getHeader().hasAttribute( "lRepetitions" ) )
      {

        object.getHeader().getAttribute( "lRepetitions",
                                         sizeT );
        ++ sizeT;

      }

      // default oversampling factor of 2
      sizeX *= 2;
      object.getHeader()[ "sizeX" ] = sizeX;
      object.getHeader()[ "sizeY" ] = sizeY;
      object.getHeader()[ "sizeZ" ] = sizeZ;
      object.getHeader()[ "sizeT" ] = sizeT * channelCount;

    }
    else if ( pulseSequenceName == "%SiemensSeq%\\gre" )
    {

      object.getHeader().getAttribute( "sKSpace.lBaseResolution",
                                       sizeX );
      object.getHeader().getAttribute( "sKSpace.lPhaseEncodingLines",
                                       sizeY );
      object.getHeader().getAttribute( "sKSpace.lPartitions",
                                       sizeZ );
      if ( object.getHeader().hasAttribute( "lRepetitions" ) )
      {

        object.getHeader().getAttribute( "lRepetitions",
                                         sizeT );
        ++ sizeT;

      }

      // default oversampling factor of 2
      sizeX *= 2;

      object.getHeader()[ "sizeX" ] = sizeX;
      object.getHeader()[ "sizeY" ] = sizeY;
      object.getHeader()[ "sizeZ" ] = sizeZ;
      object.getHeader()[ "sizeT" ] = sizeT * channelCount;

    }
    else
    {

      throw std::runtime_error( "not a managed pulse sequence" );

    }

    // closing file
    is.close();

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SmsRawDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::SmsRawDiskFormat< T >::read(
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
             "void gkg::SmsRawDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
inline
void gkg::SmsRawDiskFormat< T >::preparePartialBinaryRead(
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

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::SmsRawDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::SmsRawDiskFormat< T >::partialBinaryRead(
                         const gkg::HeaderedObject& proxy,
                         gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {

    // determining byte swapping (we know that data are saved using a
    // little endian memory model
    bool bswap = false;
    if ( gkg::ByteOrder::getInstance().getCurrentType() !=
         gkg::ByteOrder::LittleEndian )
    {

      bswap = true;

    }

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

            if ( !volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
            {

               throw std::runtime_error(
                                      "cannot find 'proxyFileName' attribute" );

            }

            std::string proxyFileName;
            volumeProxy.getHeader().getAttribute( "proxyFileName",
                                                  proxyFileName );

            // opening PFile
            std::ifstream is( ( this->getNameWithoutExtension( proxyFileName ) +
                                ".dat" ).c_str() );
            if ( !is )
            {

              throw gkg::FileNotFoundException(
                      this->getNameWithoutExtension( proxyFileName ) + ".dat" );

            }


            // reading Mdh to build a y,z,t to index correspondance
            gkg::Volume< uint32_t >
              fidOffsets = getFidOffsets( is, volumeProxy );

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

            // reading from core data
            gkg::ItemReader< float >& itemR =
            gkg::DefaultItemIOFactory< float >::getInstance().getReader(
                                                                 false, bswap );


            uint32_t offsetX = boundingBox.getLowerX() * sizeof( float ) * 2U;

            std::vector< float > bufferFloat( 2 * subSizeX );

            typename gkg::Volume< std::complex< T > >::iterator
              s = subVolume.begin();
            int32_t x, y, z, t;
            for ( t = 0; t < subSizeT; t++ )
            {

              for ( z = 0; z < subSizeZ; z++ )
              {

                for ( y = 0; y < subSizeY; y++ )
                {

                  // moving to the first element of the current fid
                  is.seekg( fidOffsets( boundingBox.getLowerY() + y,
                                        boundingBox.getLowerZ() + z,
                                        boundingBox.getLowerT() + t ) + offsetX,
                            gkg::IosBase::beg );

                  // reading the binary data
                  itemR.read( is, &bufferFloat[ 0 ], 2 * subSizeX  );

                  // converting to the adequate complex type
                  for ( x = 0; x < subSizeX; x++ )
                  {

                    *s = std::complex< T >( ( T )bufferFloat[ 2 * x ],
                                            ( T )bufferFloat[ 2 * x + 1 ] );
                    ++ s;

                  }

                }

              }

            }

            // closing the stream
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
             "void gkg::SmsRawDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::SmsRawDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
std::string gkg::SmsRawDiskFormat< T >::getType( const std::string& name ) const
{

  try
  {

    std::string::size_type position = 0U;
    if ( name.rfind( '.' ) != std::string::npos )
    {

      position = name.rfind( '.' ) + 1;

    }

    std::string type;
    if ( name.substr( position, 1U ) == "t" )
    {

      type = gkg::TypeOf< std::string >::getName();

    }
    else if ( name.substr( position, 2U ) == "ul" )
    {

      type = gkg::TypeOf< uint32_t >::getName();

    }
    else if ( name.substr( position, 1U ) == "l" )
    {

      type = gkg::TypeOf< int32_t >::getName();

    }
    else if ( name.substr( position, 2U ) == "ui" )
    {

      type = gkg::TypeOf< uint32_t >::getName();

    }
    else if ( name.substr( position, 1U ) == "i" )
    {

      type = gkg::TypeOf< int32_t >::getName();

    }
    else if ( name.substr( position, 2U ) == "uc" )
    {

      type = gkg::TypeOf< uint32_t >::getName();

    }
    else if ( name.substr( position, 1U ) == "c" )
    {

      type = gkg::TypeOf< int32_t >::getName();

    }
    else if ( name.substr( position, 1U ) == "d" )
    {

      type = gkg::TypeOf< double >::getName();

    }

    return type;

  }
  GKG_CATCH( "template < class T > "
             "std::string gkg::SmsRawDiskFormat< T >::getType( "
             "const std::string& name ) const" );

}


template < class T >
void 
gkg::SmsRawDiskFormat< T >::readMdh( std::istream& is, gkg::Mdh& mdh ) const
{

  try
  {

    // determining byte swapping (we know that data are saved using a
    // little endian memory model
    bool bswap = false;
    if ( gkg::ByteOrder::getInstance().getCurrentType() !=
         gkg::ByteOrder::LittleEndian )
    {

      bswap = true;

    }

    gkg::ItemReader< uint32_t >& itemR_Uint32 =
      gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< int32_t >& itemR_Int32 =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader( false,
                                                                     bswap );
    gkg::ItemReader< uint16_t >& itemR_Uint16 =
      gkg::DefaultItemIOFactory< uint16_t >::getInstance().getReader( false,
                                                                      bswap );
    gkg::ItemReader< float >& itemR_Float =
      gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                   bswap );

    itemR_Uint32.read( is, mdh.ulFlagsAndDMALength );
    itemR_Int32.read( is, mdh.lMeasUID );
    itemR_Uint32.read( is, mdh.ulScanCounter );
    itemR_Uint32.read( is, mdh.ulTimeStamp );
    itemR_Uint32.read( is, mdh.ulPMUTimeStamp );

    int32_t i = 0;
    for ( i = 0; i < MDH_NUMBEROFEVALINFOMASK; i++ )
    {

      itemR_Uint32.read( is, mdh.aulEvalInfoMask[ i ] );

    }

    itemR_Uint16.read( is, mdh.ushSamplesInScan );
    itemR_Uint16.read( is, mdh.ushUsedChannels );


    itemR_Uint16.read( is, mdh.sLoopCounter.ushLine );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushAcquisition );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushSlice );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushPartition );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushEcho );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushPhase );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushRepetition );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushSet );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushSeg );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushIda );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushIdb );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushIdc );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushIdd );
    itemR_Uint16.read( is, mdh.sLoopCounter.ushIde );

    itemR_Uint16.read( is, mdh.sCutOff.ushPre );
    itemR_Uint16.read( is, mdh.sCutOff.ushPost );

    itemR_Uint16.read( is, mdh.ushKSpaceCentreColumn );
    itemR_Uint16.read( is, mdh.ushCoilSelect );

    itemR_Float.read( is, mdh.fReadOutOffcentre );

    itemR_Uint32.read( is, mdh.ulTimeSinceLastRF );

    itemR_Uint16.read( is, mdh.ushKSpaceCentreLineNo );
    itemR_Uint16.read( is, mdh.ushKSpaceCentrePartitionNo );

    for ( i = 0; i < MDH_NUMBEROFICEPROGRAMPARA; i++ )
    {

      itemR_Uint16.read( is, mdh.aushIceProgramPara[ i ] );

    }

    for ( i = 0; i < MDH_FREEHDRPARA; i++ )
    {

      itemR_Uint16.read( is, mdh.aushFreePara[ i ] );

    }

    itemR_Float.read( is, mdh.sSliceData.sSlicePosVec.flSag );
    itemR_Float.read( is, mdh.sSliceData.sSlicePosVec.flCor );
    itemR_Float.read( is, mdh.sSliceData.sSlicePosVec.flTra );
    itemR_Float.read( is, mdh.sSliceData.aflQuaternion[ 0 ] );
    itemR_Float.read( is, mdh.sSliceData.aflQuaternion[ 1 ] );
    itemR_Float.read( is, mdh.sSliceData.aflQuaternion[ 2 ] );
    itemR_Float.read( is, mdh.sSliceData.aflQuaternion[ 3 ] );

    itemR_Uint16.read( is, mdh.ushChannelId );
    itemR_Uint16.read( is, mdh.ushPTABPosNeg );

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::SmsRawDiskFormat< T >::readMdh( "
             "std::istream& is, gkg::Mdh& mdh ) const" );

}


template < class T >
gkg::Volume< uint32_t >
gkg::SmsRawDiskFormat< T >::getFidOffsets(
                                       std::istream& is,
                                       const gkg::HeaderedObject& object ) const
{

  try
  {


    // skipping the header
    uint32_t headerSize = 0U;
    object.getHeader().getAttribute( "sms_header_size", headerSize );
    is.seekg( headerSize, gkg::IosBase::beg );

    // collecting size(s)
    int32_t sizeX = 1;
    int32_t sizeY = 1;
    int32_t sizeZ = 1;
    int32_t sizeT = 1;
    object.getHeader().getAttribute( "sizeX", sizeX );
    object.getHeader().getAttribute( "sizeY", sizeY );
    object.getHeader().getAttribute( "sizeZ", sizeZ );
    object.getHeader().getAttribute( "sizeT", sizeT );


    uint32_t fidCount = ( uint32_t )sizeY *
                        ( uint32_t )sizeZ *
                        ( uint32_t )sizeT;

    // allocating offset map for any line
    gkg::Volume< uint32_t > fidOffsets( sizeY, sizeZ, sizeT );

    // filling FID offset volume
    uint32_t fid = 0;
    uint32_t offset = headerSize;
    uint32_t lineMemory = ( uint32_t )sizeX * sizeof( float ) * 2U;
    for ( fid = 0; fid < fidCount; fid++ )
    {

      gkg::Mdh  mdh;
      readMdh( is, mdh );
      offset += 128U;

      fidOffsets( ( int32_t )mdh.sLoopCounter.ushLine,
                  ( int32_t )mdh.sLoopCounter.ushPartition,
                  ( ( int32_t )mdh.sLoopCounter.ushRepetition * 
                    ( int32_t )mdh.ushUsedChannels +
                    ( int32_t )mdh.ushChannelId ) ) = offset;

      is.seekg( lineMemory, gkg::IosBase::cur );
      offset += lineMemory;

    }

    return fidOffsets;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< uint32_t > "
             "gkg::SmsRawDiskFormat< T >::getFidOffsets( "
             "std::istream& is, "
             "const gkg::HeaderedObject& object ) const" );

}


template class gkg::SmsRawDiskFormat< float >;
template class gkg::SmsRawDiskFormat< double >;


//
// registrating standard SMS raw disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > 
                           >::getInstance().registerDiskFormat(
         gkg::SmsRawDiskFormat< float >::getInstance().getName(),
         &gkg::SmsRawDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< double > > 
                           >::getInstance().registerDiskFormat(
         gkg::SmsRawDiskFormat< double >::getInstance().getName(),
         &gkg::SmsRawDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "SMS raw disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();


