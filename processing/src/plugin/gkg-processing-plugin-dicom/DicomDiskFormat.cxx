#include <gkg-processing-plugin-dicom/DicomDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/RegionInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-cppext/StringStream.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>

#include <pio/dicom/DicomIO.h>
#include <pio/dicom/DicomDatasetHeader.h>
#include <pio/container/DataInfoCache.h>
#include <pio/binding/gkg/GkgHeader_i.h>
#include <pio/binding/gkg/PioVolume_i.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdict.h>

#include <zlib.h>


// hack to fix a linker error with lz in DCMTK package on Ubuntu-12.04LTS
std::string zLibVersion = zlibVersion();


template < class T >
gkg::DicomDiskFormat< T >::DicomDiskFormat()
                          : gkg::TypedDiskFormat< gkg::Volume< T > >()
{
}


template < class T >
gkg::DicomDiskFormat< T >::~DicomDiskFormat()
{
}


template < class T >
std::string gkg::DicomDiskFormat< T >::getName() const
{

  return "dicom";

}


template < class T >
void gkg::DicomDiskFormat< T >::readHeader( const std::string& name,
                                            gkg::HeaderedObject& object ) const
{

  try
  {

    gkg::File file( name );

    if ( file.isDirectory() )
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

    }
    else
    {

      if ( !file.isExisting() )
        {

          throw gkg::FileNotFoundException( name );

        }

    }

    // checking item type compatibility
    std::string itemType;
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      object.getHeader().getAttribute( "item_type", itemType );
      if ( ! gkg::TypeOf< T >::isSameType( itemType ) ) 
      {

        throw gkg::ItemTypeException(
          std::string( "object item type given as argument is different from " )
          + gkg::TypeOf< T >::getName() );

      }

    }

    pio::DataInfoCache::getInstance().clear();
    pio::DataInfo& info = pio::DataInfoCache::getInstance().getDataInfo();

    if ( pio::DicomIO::getInstance().analyze( name, info, true ) )
    {

      if ( ( info._depth == 24 ) ||  // no supported yet
           ( ( info._depth == 8 ) && !info._pixelRepresentation &&
             ( ! gkg::TypeOf< uint8_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 8 ) && info._pixelRepresentation &&
             ( ! gkg::TypeOf< int8_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 16 ) && !info._pixelRepresentation &&
             ( ! gkg::TypeOf< uint16_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 16 ) && info._pixelRepresentation &&
             ( ! gkg::TypeOf< int16_t >::isSameType( itemType ) ) ) ||
           ( ( info._modalityLut ) && 
             ( ! gkg::TypeOf< float >::isSameType( itemType ) ) ) )
      {

        throw gkg::ItemTypeException( name );

      }

    }
    else
    {

      throw std::runtime_error( name + ": not a DICOM file" );

    }

    if ( !object.getHeader().hasAttribute( "fast_dicom_check" ) )
    {

      pio::DicomDatasetHeader datasetHeader( info );

      if ( object.getHeader().hasAttribute( "no_flip" ) )
      {

        info._noFlip = true;

      }
      if ( object.getHeader().hasAttribute( "no_demosaic" ) )
      {

        info._noDemosaic = true;

      }

      if ( pio::DicomIO::getInstance().check( name, info, datasetHeader ) )
      {

        object.addSemantic( "__dicom__", "resolutionX",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__dicom__", "resolutionY",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__dicom__", "resolutionZ",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__dicom__", "resolutionT",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );

        pio::GkgHeader gkgHeader( object );
        pio::DicomIO::getInstance().getHeader( gkgHeader, info, datasetHeader );

        // adding ordered file names
        object.addSemantic( "__dicom__",
                            "ordered_file_name",
                            gkg::Semantic(
                            gkg::TypeOf< std::vector< std::string > >::getName()
                            ) );
        object.getHeader().addAttribute( "ordered_file_names", 
                                         datasetHeader.getFileList() );
        // adding directory path
        gkg::Directory directory( name );

        object.addSemantic( 
                       "__dicom__",
                       "directory_path",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
        object.getHeader().addAttribute( "directory_path", 
                                         directory.getPath() );

      }

    }

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DicomDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::DicomDiskFormat< T >::writeHeader( 
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "nothing to do" );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DicomDiskFormat< T >::writeHeader( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::DicomDiskFormat< T >::read( const std::string& name,
                                      gkg::Volume< T >& object ) const
{

  try
  {

    // preparing partial binary read
    gkg::VolumeProxy< T > proxy;

    if ( object.getHeader().hasAttribute( "no_flip" ) )
    {

      proxy.getHeader().addAttribute( "no_flip", 1 );

    }
    if ( object.getHeader().hasAttribute( "no_demosaic" ) )
    {

      proxy.getHeader().addAttribute( "no_demosaic", 1 );

    }
    
    preparePartialBinaryRead( name, proxy );

    // building bounding box
    gkg::BoundingBox< int32_t > boundingBox( 0, proxy.getSizeX() - 1,
                                             0, proxy.getSizeY() - 1,
                                             0, proxy.getSizeZ() - 1,
                                             0, proxy.getSizeT() - 1 );

    // reading whole volume
    partialBinaryRead( proxy, object, boundingBox );

    if ( object.getHeader().hasAttribute( "no_flip" ) )
    {

      object.getHeader().removeAttribute( "no_flip" );

    }
    if ( object.getHeader().hasAttribute( "no_demosaic" ) )
    {

      object.getHeader().removeAttribute( "no_demosaic" );

    }
    if ( object.getHeader().hasAttribute( "ordered_file_names" ) )
    {

      object.getHeader().removeAttribute( "ordered_file_names" );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DicomDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
void gkg::DicomDiskFormat< T >::write( const std::string& name,
                                       gkg::Volume< T >& object,
                                       bool /*ascii*/ ) const
{

  // removed until the new I/O is able to write Dicom datasets
  try
  {

    std::string::size_type pos1 = name.rfind( ".dcm" );
    std::string::size_type pos2 = name.rfind( ".dic" );

    // must have a ".dcm" of ".dic" extension to prevent DICOM
    // being the default writer
    if ( ( pos1 == std::string::npos ) && ( pos2 == std::string::npos ) )
    {

      throw std::runtime_error( "must have a .dcm or .dic extension" );

    }

    std::string itemType;
    if ( object.getHeader().hasAttribute( "item_type" ) )
    {

      object.getHeader().getAttribute( "item_type", itemType );
      if ( ! gkg::TypeOf< T >::isSameType( itemType ) ) 
      {

        throw gkg::ItemTypeException(
          std::string( "object item type given as argument is different from " )
          + gkg::TypeOf< T >::getName() );

      }

    }

    pio::DataInfo dataInfo;
    dataInfo._pixelRepresentation = 0;

    if ( gkg::TypeOf< int8_t >::isSameType( itemType ) ||
         gkg::TypeOf< int16_t >::isSameType( itemType ) )
    {

      dataInfo._pixelRepresentation = 1;

    }

    gkg::Vector3d< double > resolution;
    double tr;
    object.getResolution( resolution, tr );

    dataInfo._spp = 1;
    dataInfo._depth = sizeof( T ) << 3;
    dataInfo._invertLut = false;
    dataInfo._width = object.getSizeX();
    dataInfo._height = object.getSizeY();
    dataInfo._slices = object.getSizeZ();
    dataInfo._frames = object.getSizeT();
    dataInfo._resolution.x = resolution.x;
    dataInfo._resolution.y = resolution.y;
    dataInfo._resolution.z = resolution.z;
    dataInfo._repetitionTime = tr;
    dataInfo._noFlip = true;
    dataInfo._boundingBox.setUpperX( dataInfo._width - 1 );
    dataInfo._boundingBox.setUpperY( dataInfo._height - 1 );
    dataInfo._boundingBox.setUpperZ( dataInfo._slices - 1 );
    dataInfo._boundingBox.setUpperT( dataInfo._frames - 1 );
    dataInfo._dataType = pio::Image;

    pio::VolumeProxy data( (uint8_t*)&object( 0 ), &dataInfo );
    data.allocate(); // to set up accessors
    pio::DicomIO::getInstance().write( name, data );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DicomDiskFormat< T >::write( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
void gkg::DicomDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // reading header(s) <*.header> and <*.dim>
      readHeader( name, volumeProxy );

      // adding proxy file name
      if ( volumeProxy.getHeader().hasAttribute( "proxyFileName" ) )
      {

        volumeProxy.getHeader().removeAttribute( "proxyFileName" );

      }
      volumeProxy.getHeader().addAttribute( "proxyFileName", name );

    }
    else
    {

      throw std::runtime_error( "not a volume proxy" );

    }

  }
  GKG_CATCH( "template < class T > void gkg::DicomDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::DicomDiskFormat< T >::partialBinaryRead(
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

            // checking bounding box
            gkg::BoundingBox< int32_t > 
                               fullBoundingBox( 0, volumeProxy.getSizeX() - 1,
                                                0, volumeProxy.getSizeY() - 1,
                                                0, volumeProxy.getSizeZ() - 1,
                                                0, volumeProxy.getSizeT() - 1 );
            if ( !fullBoundingBox.contains( boundingBox ) )
            {

              throw std::runtime_error(
                                "bounding box is not contained inside volume" );

            }

            // reallocating the sub-part
            subVolume.getHeader() = volumeProxy.getHeader();

            // getting ordered file names
            std::vector< std::string > orderedFileNames;
            volumeProxy.getHeader().getAttribute( "ordered_file_names",
                                                  orderedFileNames );

            // transform bounding box into acquisition referential
            pio::DataInfo& info = 
                                pio::DataInfoCache::getInstance().getDataInfo();
            pio::BoundingBox< int32_t > bBox( 
                             boundingBox.getLowerX(), boundingBox.getUpperX(),
                             boundingBox.getLowerY(), boundingBox.getUpperY(),
                             boundingBox.getLowerZ(), boundingBox.getUpperZ(),
                             boundingBox.getLowerT(), boundingBox.getUpperT() );

            if ( info._noFlip )
            {

              info._boundingBox = bBox;

            }
            else
            {

              info._boundingBox =
                         info._patientOrientation.getInverseBoundingBox( bBox );

            }

            // reading data
            pio::DicomDatasetHeader datasetHeader( info );
            pio::PioVolume< T > pioVolume( subVolume, &info );
            std::vector< std::string >& fileList = datasetHeader.getFileList();

            fileList = orderedFileNames;

            pio::DicomIO::getInstance().read( datasetHeader, pioVolume );

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
             "void gkg::DicomDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
inline
void gkg::DicomDiskFormat< T >::preparePartialBinaryWrite(
                                          const std::string& /*name*/,
                                          gkg::HeaderedObject& /*proxy*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "template < class T > void gkg::DicomDiskFormat< T >::"
             "preparePartialBinaryWrite( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
inline
void gkg::DicomDiskFormat< T >::partialBinaryWrite(
                     const gkg::HeaderedObject& /*proxy*/,
                     const gkg::GenericObject& /*subpart*/,
                     const gkg::RegionInformation& /*regionInformation*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "template < class T > void gkg::DicomDiskFormat< T >::"
             "partialBinaryWrite( "
             "const gkg::HeaderedObject& proxy, "
             "const gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
inline
bool gkg::DicomDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
inline
bool gkg::DicomDiskFormat< T >::hasWriter() const
{

  return true;

}


template class gkg::DicomDiskFormat< uint8_t >;
template class gkg::DicomDiskFormat< int8_t >;
template class gkg::DicomDiskFormat< uint16_t >;
template class gkg::DicomDiskFormat< int16_t >;
template class gkg::DicomDiskFormat< float >;


//
// registrating standard Dicom analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< uint8_t > 
                           >::getInstance().registerDiskFormat(
         gkg::DicomDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::DicomDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int8_t > 
                           >::getInstance().registerDiskFormat(
         gkg::DicomDiskFormat< int8_t >::getInstance().getName(),
         &gkg::DicomDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint16_t > 
                           >::getInstance().registerDiskFormat(
         gkg::DicomDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::DicomDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int16_t > 
                           >::getInstance().registerDiskFormat(
         gkg::DicomDiskFormat< int16_t >::getInstance().getName(),
         &gkg::DicomDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< float > 
                           >::getInstance().registerDiskFormat(
         gkg::DicomDiskFormat< float >::getInstance().getName(),
         &gkg::DicomDiskFormat< float >::getInstance() );


    // reading DICOM dictionary
    if ( !dcmDataDict.isDictionaryLoaded() )
    {

      throw std::runtime_error( "no DICOM dictionary loaded, set DCMDICTPATH" );

    }

#ifdef GKG_DEBUG

    const ::DcmDataDictionary& dictionary = dcmDataDict.rdlock();

    std::cout << "dictionary entry count: "
              << dictionary.numberOfEntries()
              << std::endl;
    std::cout << "dictionary duplicate count: "
              << dictionary.numberOfRepeatingTagEntries()
              << std::endl;

#endif

    return true;

  }
  GKG_CATCH( "DICOM disk format registration" )

}

static bool initialized __attribute__((unused)) = initialize();
