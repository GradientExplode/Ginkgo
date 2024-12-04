#include <gkg-processing-plugin-nifti/NiftiDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/RegionInformation.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>

#include <pio/nifti/NiftiIO.h>
#include <pio/nifti/NiftiHeader.h>
#include <pio/container/DataInfoCache.h>
#include <pio/binding/gkg/GkgHeader_i.h>
#include <pio/binding/gkg/PioVolume_i.h>

#include <fstream>
#include <algorithm>


template < class T >
gkg::NiftiDiskFormat< T >::NiftiDiskFormat()
                          : gkg::TypedDiskFormat< gkg::Volume< T > >()
{

  this->_extensions.insert( ".hdr" );
  this->_extensions.insert( ".img" );
  this->_extensions.insert( ".hdr.gz" );
  this->_extensions.insert( ".img.gz" );
  this->_extensions.insert( ".nii" );
  this->_extensions.insert( ".nii.gz" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class T >
gkg::NiftiDiskFormat< T >::~NiftiDiskFormat()
{
}


template < class T >
std::string gkg::NiftiDiskFormat< T >::getName() const
{

  return "nifti";

}


template < class T >
void gkg::NiftiDiskFormat< T >::readHeader( const std::string& name,
                                            gkg::HeaderedObject& object ) const
{

  try
  {

    // checking that it is a volume
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "Volume" )
    {

      throw gkg::ObjectTypeException( name );

    }

    // checking that it has the good item type
    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    if ( ! gkg::TypeOf< T >::isSameType( itemType ) ) 
    {

      throw gkg::ItemTypeException(
    	std::string( "object item type given as argument is different from " )
    	+ gkg::TypeOf< T >::getName() );

    }

    pio::DataInfoCache::getInstance().clear();
    pio::DataInfo& info = pio::DataInfoCache::getInstance().getDataInfo();
    pio::NiftiHeader niftiHeader( info );

    if ( pio::NiftiIO::getInstance().readHeader( name, info, niftiHeader ) )
    {

      if ( ( ( info._depth == 8 ) && ( info._spp == 1 ) && 
             !info._pixelRepresentation &&
             ( !gkg::TypeOf< uint8_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 8 ) && ( info._spp == 1 ) && 
             info._pixelRepresentation &&
             ( !gkg::TypeOf< int8_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 16 ) && ( info._spp == 1 ) && 
             !info._pixelRepresentation &&
             ( !gkg::TypeOf< uint16_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 16 ) && ( info._spp == 1 ) && 
             info._pixelRepresentation &&
             ( !gkg::TypeOf< int16_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 32 ) && ( info._spp == 1 ) && 
             !info._pixelRepresentation && !info._modalityLut &&
             ( !gkg::TypeOf< uint32_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 32 ) && ( info._spp == 1 ) && 
             info._pixelRepresentation && !info._modalityLut &&
             ( !gkg::TypeOf< int32_t >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 32 ) && ( info._spp == 1 ) && info._modalityLut &&  
             ( !gkg::TypeOf< float >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 64 ) && ( info._spp == 1 ) && info._modalityLut && 
             ( !gkg::TypeOf< double >::isSameType( itemType ) ) ) ||
           ( ( info._depth == 8 ) && ( info._spp == 3 ) &&
             ( !gkg::TypeOf< gkg::RGBComponent >::isSameType( itemType ) ) ) )
      {

        throw gkg::ItemTypeException( name );

      }

      if ( !object.getHeader().hasAttribute( "fast_nifti_check" ) )
      {

        object.addSemantic( "__nifti__", "resolutionX",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__nifti__", "resolutionY",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__nifti__", "resolutionZ",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );
        object.addSemantic( "__nifti__", "resolutionT",
                            gkg::Semantic( gkg::TypeOf< double >::getName() ) );

        pio::GkgHeader gkgHeader( object );
        pio::NiftiIO::getInstance().getHeader( gkgHeader, info, niftiHeader );

      }

    }

    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > void gkg::NiftiDiskFormat< T >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  try
  {

    // writing the meta-information header
    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class T > void gkg::NiftiDiskFormat< T >::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::read( const std::string& name,
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
             "void gkg::NiftiDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::write( const std::string& name,
                                       gkg::Volume< T >& object,
                                       bool ascii ) const
{

  try
  {

    if ( ascii == true )
    {

      throw std::runtime_error(
                              "ascii mode not supported in NIFTI disk format" );

    }
    else
    {

      std::string itemType;
      if ( object.getHeader().hasAttribute( "item_type" ) )
      {

        object.getHeader().getAttribute( "item_type", itemType );
        if ( ! gkg::TypeOf< T >::isSameType( itemType ) ) 
        {

          throw gkg::ItemTypeException(
            std::string( 
                       "object item type given as argument is different from " )
            + gkg::TypeOf< T >::getName() );

        }

      }

      pio::DataInfo dataInfo;
      dataInfo._pixelRepresentation = 0;
      dataInfo._spp = 1;
      dataInfo._depth = sizeof( T ) << 3;

      if ( gkg::TypeOf< int8_t >::isSameType( itemType ) ||
           gkg::TypeOf< int16_t >::isSameType( itemType ) ||
           gkg::TypeOf< int32_t >::isSameType( itemType ) )
      {

        dataInfo._pixelRepresentation = 1;

      }

      if ( gkg::TypeOf< float >::isSameType( itemType ) )
      {

        dataInfo._modalityLut = 1;

      }

      if ( gkg::TypeOf< gkg::RGBComponent >::isSameType( itemType ) )
      {

        dataInfo._spp = 3;
        dataInfo._depth = 8;

      }

      gkg::Vector3d< double > resolution;
      double tr;
      object.getResolution( resolution, tr );

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

      pio::VolumeProxy proxy( (uint8_t*)&object( 0 ), &dataInfo );
      proxy.allocate(); // to set up accessors
      pio::NiftiIO::getInstance().write( name, proxy );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::write( "
             "const std::string& name, gkg::Volume< T >& object, "
             "bool ascii ) const" );

}


template < class T >
inline
void gkg::NiftiDiskFormat< T >::preparePartialBinaryRead(
                                              const std::string& name,
                                              gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >&
        volumeProxy = dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // reading header
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
  GKG_CATCH( "template < class T > void gkg::NiftiDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::partialBinaryRead(
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

        if ( &dynamic_cast< const gkg::BoundingBox< int >& >(
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

            // getting file name
            std::string name;
            volumeProxy.getHeader().getAttribute( "proxyFileName", name );

            // transform bounding box into acquisition referential
            pio::DataInfo& info =
                                pio::DataInfoCache::getInstance().getDataInfo();
            pio::BoundingBox< int32_t > bBox( 
                             boundingBox.getLowerX(), boundingBox.getUpperX(),
                             boundingBox.getLowerY(), boundingBox.getUpperY(),
                             boundingBox.getLowerZ(), boundingBox.getUpperZ(),
                             boundingBox.getLowerT(), boundingBox.getUpperT() );

            info._boundingBox =
                         info._patientOrientation.getInverseBoundingBox( bBox );

            // reading data
            pio::PioVolume< T > pioVolume( subVolume, &info );

            pio::NiftiIO::getInstance().read( name, pioVolume );

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
             "void gkg::NiftiDiskFormat< T >::read( "
             "const std::string& name, gkg::Volume< T >& object ) const" );

}


template < class T >
bool gkg::NiftiDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
bool gkg::NiftiDiskFormat< T >::hasWriter() const
{

  return true;

}


template < class T >
std::string gkg::NiftiDiskFormat< T >::getHeaderExtension() const
{

  return std::string( ".nii" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


template < class T >
void gkg::NiftiDiskFormat< T >::getInt32Value(
                                              const std::string& name,
                                              const gkg::HeaderedObject& object,
                                              int32_t& value,
                                              int32_t defaultValue ) const
{

  try
  {

    value = defaultValue;
    if ( object.getHeader().hasAttribute( name ) )
    {

      object.getHeader().getAttribute( name, value );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::getInt32Value( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object, "
             "int32_t& value, "
             "int32_t defaultValue ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::getInt64Value(
                                              const std::string& name,
                                              const gkg::HeaderedObject& object,
                                              int64_t& value,
                                              int64_t defaultValue ) const
{

  try
  {

    value = defaultValue;
    if ( object.getHeader().hasAttribute( name ) )
    {

      object.getHeader().getAttribute( name, value );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::getInt64Value( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object, "
             "int64_t& value, "
             "int64_t defaultValue ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::getFloatValue(
                                              const std::string& name,
                                              const gkg::HeaderedObject& object,
                                              float& value,
                                              float defaultValue ) const
{

  try
  {

    value = defaultValue;
    if ( object.getHeader().hasAttribute( name ) )
    {

      object.getHeader().getAttribute( name, value );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::getFloatValue( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object, "
             "float& value, "
             "float defaultValue ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::getDoubleValue(
                                              const std::string& name,
                                              const gkg::HeaderedObject& object,
                                              double& value,
                                              double defaultValue ) const
{

  try
  {

    value = defaultValue;
    if ( object.getHeader().hasAttribute( name ) )
    {

      object.getHeader().getAttribute( name, value );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::getDoubleValue( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object, "
             "double& value, "
             "double defaultValue ) const" );

}


template < class T >
void gkg::NiftiDiskFormat< T >::getStringValue(
                                         const std::string& name,
                                         const gkg::HeaderedObject& object,
                                         std::string& value,
                                         const std::string& defaultValue ) const
{

  try
  {

    value = defaultValue;
    if ( object.getHeader().hasAttribute( name ) )
    {

      object.getHeader().getAttribute( name, value );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NiftiDiskFormat< T >::getStringValue( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object, "
             "std::string& value, "
             "const std::string& defaultValue ) const" );

}


//
// NIFTI disk format singleton instanciation
//

template class gkg::NiftiDiskFormat< int8_t >;
template class gkg::NiftiDiskFormat< uint8_t >;
template class gkg::NiftiDiskFormat< int16_t >;
template class gkg::NiftiDiskFormat< uint16_t >;
template class gkg::NiftiDiskFormat< int32_t >;
template class gkg::NiftiDiskFormat< uint32_t >;
template class gkg::NiftiDiskFormat< float >;
template class gkg::NiftiDiskFormat< double >;
template class gkg::NiftiDiskFormat< gkg::RGBComponent >;


//
// registrating standard NIFTI disk format(s) for "Volume" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< int8_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< int8_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint8_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int16_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< int16_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint16_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int32_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< int32_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint32_t > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::NiftiDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< float > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< float >::getInstance().getName(),
         &gkg::NiftiDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< double > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< double >::getInstance().getName(),
         &gkg::NiftiDiskFormat< double >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< gkg::RGBComponent > 
                          >::getInstance().registerDiskFormat(
         gkg::NiftiDiskFormat< gkg::RGBComponent >::getInstance().getName(),
         &gkg::NiftiDiskFormat< gkg::RGBComponent >::getInstance() );

    return true;

  }
  GKG_CATCH( "NIFTI disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
