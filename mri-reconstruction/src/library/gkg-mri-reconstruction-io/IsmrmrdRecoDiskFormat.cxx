#include <gkg-mri-reconstruction-io/IsmrmrdRecoDiskFormat.h>
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
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/FormatMismatchException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/xml.h>
#include <ismrmrd/dataset.h>
#include <list>
#include <fstream>
#include <hdf5.h>

//
// class IsmrmrdRecoDiskFormat
//


template < class T >
gkg::IsmrmrdRecoDiskFormat< T >::IsmrmrdRecoDiskFormat()
                                : gkg::TypedDiskFormat< gkg::Volume< T > >()
{

  try
  {

    // ISMRMRD files are HDF5 files, so ending with .h5
    this->_extensions.insert( ".h5" );

  }
  GKG_CATCH( "template < class T > "
             "gkg::IsmrmrdRecoDiskFormat< T >::IsmrmrdRecoDiskFormat()" );

}


template < class T >
gkg::IsmrmrdRecoDiskFormat< T >::~IsmrmrdRecoDiskFormat()
{
}


template < class T >
std::string gkg::IsmrmrdRecoDiskFormat< T >::getName() const
{

  return "ismrmrd-reco";

}






template < class T >
void gkg::IsmrmrdRecoDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // inspecting the HDF5 file
    hid_t fileAccessPropertyListId = H5Pcreate( H5P_FILE_ACCESS );
    herr_t status = H5Pset_fapl_stdio( fileAccessPropertyListId );

    if ( status )
    {

      throw std::runtime_error( "failed to get HDF5 file access propery list" );

    }

    hid_t h5FileId = H5Fopen( name.c_str(), H5F_ACC_RDONLY,
                              fileAccessPropertyListId );

    if ( H5Fget_obj_count( h5FileId, H5F_OBJ_ALL ) != 1U )
    {

      H5Fclose( h5FileId );
      throw std::runtime_error( "the HDF5 file of an ISMRMRD reconstruction "
                                "must contain only one element" );

    }

    // getting the element key(s)
    std::list< std::pair< std::string, std::string > > elementKeys;
    H5Giterate( h5FileId, "/", NULL, getFileInformation, &elementKeys );

    if ( elementKeys.front().second != "group" )
    {

      H5Fclose( h5FileId );
      throw std::runtime_error( "the single element of an HDF5 ISMRMRD "
                                "reconstructiont must be a 'group'" );

    }

    if ( H5Fget_obj_count( h5FileId, H5F_OBJ_ALL ) != 1U )
    {

      throw std::runtime_error( "the HDF5 file of an ISMRMRD reconstruction "
                                "must contain only one element" );

    }

    H5Fclose( h5FileId );


    std::string hdf5GroupName = elementKeys.front().first;


    // reading the group
    ISMRMRD::Dataset dataset( name.c_str(), hdf5GroupName.c_str(), false );

    std::string labelOfImage;
    if ( dataset.getNumberOfImages( "image_0" ) )
    {

      labelOfImage = "image_0";

    }
    else if ( dataset.getNumberOfImages( "image_1" ) )
    {

      labelOfImage = "image_1";

    }
    else
    {

      throw gkg::FormatMismatchException( name );

    }

    ISMRMRD::Image< T > imageData;
    dataset.readImage( labelOfImage, 0U, imageData );

    if ( ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_USHORT ) &&
             ( gkg::TypeOf< T >::getName() != "uint16_t" ) ) ||
         ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_SHORT ) &&
             ( gkg::TypeOf< T >::getName() != "int16_t" ) ) ||
         ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_UINT ) &&
             ( gkg::TypeOf< T >::getName() != "uint32_t" ) ) ||
         ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_INT ) &&
             ( gkg::TypeOf< T >::getName() != "int32_t" ) ) ||
         ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_FLOAT ) &&
             ( gkg::TypeOf< T >::getName() != "float" ) ) ||
         ( ( imageData.getDataType() == ISMRMRD::ISMRMRD_DOUBLE ) &&
             ( gkg::TypeOf< T >::getName() != "double" ) ) )
    {

      throw gkg::ItemTypeException(
    	std::string( "object item type given as argument is different from " )
    	+ gkg::TypeOf< T >::getName() );

    }

    int32_t sizeX = imageData.getMatrixSizeX();
    int32_t sizeY = imageData.getMatrixSizeY();
    int32_t sizeZ = imageData.getMatrixSizeZ();
    int32_t sizeT = imageData.getNumberOfChannels();

    double resolutionX = imageData.getFieldOfViewX() / ( double )sizeX;
    double resolutionY = imageData.getFieldOfViewY() / ( double )sizeY;
    double resolutionZ = imageData.getFieldOfViewZ() / ( double )sizeZ;
    double resolutionT = 1.0;

    if ( dataset.getNumberOfImages( labelOfImage ) > 1U )
    {

      sizeT = dataset.getNumberOfImages( labelOfImage );

    }

    object.getHeader()[ "sizeX" ] = sizeX;
    object.getHeader()[ "sizeY" ] = sizeY;
    object.getHeader()[ "sizeZ" ] = sizeZ;
    object.getHeader()[ "sizeT" ] = sizeT;

    object.getHeader()[ "resolutionX" ] = resolutionX;
    object.getHeader()[ "resolutionY" ] = resolutionY;
    object.getHeader()[ "resolutionZ" ] = resolutionZ;
    object.getHeader()[ "resolutionT" ] = resolutionT;

    object.getHeader()[ "hdf5GroupName" ] = hdf5GroupName;
    object.getHeader()[ "hdf5LabelOfImage" ] = labelOfImage;

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::IsmrmrdRecoDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::IsmrmrdRecoDiskFormat< T >::read( const std::string& name,
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
             "void gkg::IsmrmrdRecoDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< T >& object ) const" );

}


template < class T >
inline
void gkg::IsmrmrdRecoDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< T >& >( proxy ) )
    {

      gkg::VolumeProxy< T >& volumeProxy =
        dynamic_cast< gkg::VolumeProxy< T >& >( proxy );

      // reading header(s) acq + imnd or method header
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
  GKG_CATCH( "template < class T > void gkg::IsmrmrdRecoDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::IsmrmrdRecoDiskFormat< T >::partialBinaryRead(
                         const gkg::HeaderedObject& proxy,
                         gkg::GenericObject& subpart,
                         const gkg::RegionInformation& regionInformation ) const
{

  try
  {

    if ( &dynamic_cast< const gkg::VolumeProxy< T >& >( proxy ) )
    {

      const gkg::VolumeProxy< T >& volumeProxy =
        dynamic_cast< const gkg::VolumeProxy< T >& >( proxy );

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

            // getting proxy file name
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

            std::string hdf5GroupName;
            volumeProxy.getHeader().getAttribute( "hdf5GroupName",
                                                  hdf5GroupName ); 

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
            ISMRMRD::Dataset dataset( proxyFileName.c_str(), 
                                      hdf5GroupName.c_str(), false );
            readPartialBinaryCore( dataset,
                                   volumeProxy,
                                   subVolume,
                                   boundingBox );

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
             "void gkg::IsmrmrdRecoDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::IsmrmrdRecoDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
herr_t gkg::IsmrmrdRecoDiskFormat< T >::getFileInformation( hid_t loc_id,
                                                            const char* name,
                                                            void* opdata )
{

  try
  {

    // converting the void* to the element key list pointer
    std::list< std::pair< std::string, std::string > >* 
      elementKeys = static_cast< std::list< std::pair< std::string,
                                                       std::string > >* >(
                                                                       opdata );

    // get type of the object and display its name and type.
    H5G_stat_t statbuf;
    H5Gget_objinfo( loc_id, name, 0, &statbuf);

    switch ( statbuf.type )
    {

      case H5G_GROUP: 
 
        elementKeys->push_back( std::pair< std::string, std::string >(
                                                           name, "group" ) );
        break;

      case H5G_DATASET: 
 
        elementKeys->push_back( std::pair< std::string, std::string >(
                                                           name, "dataset" ) );
        break;

      case H5G_TYPE: 

        elementKeys->push_back( std::pair< std::string, std::string >(
                                                           name, "datatype" ) );
        break;

      default:

        throw std::runtime_error(
             std::string( "unable to identify the type of the HDF5 object '" ) +
             name + "'" );

    }
    return 0;

  }
  GKG_CATCH( "template < class T > "
             "herr_t gkg::IsmrmrdRecoDiskFormat< T >::getFileInformation( "
             "hid_t loc_id, "
             "const char* name, "
             "void* opdata )" );

 }




template < class T >
void gkg::IsmrmrdRecoDiskFormat< T >::readPartialBinaryCore(
                          ISMRMRD::Dataset& dataset,
                          const gkg::VolumeProxy< T >& volumeProxy,
                          gkg::Volume< T >& subVolume,
                          const gkg::BoundingBox< int32_t >& boundingBox ) const
{

  try
  {

    int32_t subSizeX = subVolume.getSizeX();
    int32_t subSizeY = subVolume.getSizeY();
    int32_t subSizeZ = subVolume.getSizeZ();
    int32_t subSizeT = subVolume.getSizeT();

    //int32_t onDiskSizeY = volumeProxy.getSizeY();
    //int32_t onDiskSizeZ = volumeProxy.getSizeZ();

    int32_t bboxLowerX = boundingBox.getLowerX();
    int32_t bboxLowerY = boundingBox.getLowerY();
    int32_t bboxLowerZ = boundingBox.getLowerZ();
    int32_t bboxLowerT = boundingBox.getLowerT();

    std::string labelOfImage = 
                     volumeProxy.getHeader()[ "hdf5LabelOfImage" ]->getString();

    if ( dataset.getNumberOfImages( labelOfImage ) == 1U )
    {

      ISMRMRD::Image< T > imageData;
      dataset.readImage( labelOfImage, 0U, imageData );

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t t = 0;
      typename gkg::Volume< T >::iterator s = subVolume.begin();

      for ( t = 0; t < subSizeT; t++ )
      {

        for ( z = 0; z < subSizeZ; z++ )
        {

          for ( y = 0; y < subSizeY; y++ )
          {

            for ( x = 0; x < subSizeX; x++ )
            {

              *s = imageData( ( uint16_t )( x + bboxLowerX ),
                              ( uint16_t )( y + bboxLowerY ),
                              ( uint16_t )( z + bboxLowerZ ),
                              ( uint16_t )( t + bboxLowerT ) );
              ++ s;

            }

          }

        }

      }

    }
    else
    {

      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      int32_t t = 0;
      typename gkg::Volume< T >::iterator s = subVolume.begin();

      for ( t = 0; t < subSizeT; t++ )
      {

        ISMRMRD::Image< T > imageData;
        dataset.readImage( labelOfImage, ( uint32_t )t, imageData );

        for ( z = 0; z < subSizeZ; z++ )
        {

          for ( y = 0; y < subSizeY; y++ )
          {

            for ( x = 0; x < subSizeX; x++ )
            {

              *s = imageData( ( uint16_t )( x + bboxLowerX ),
                              ( uint16_t )( y + bboxLowerY ),
                              ( uint16_t )( z + bboxLowerZ ),
                              ( uint16_t )( 0 + bboxLowerT ) );
              ++ s;

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::IsmrmrdRecoDiskFormat< T >::readPartialBinaryCore( "
             "ISMRMRD::Dataset& dataset, "
             "const gkg::VolumeProxy< T >& volumeProxy, "
             "gkg::Volume< T >& subVolume, "
             "const gkg::BoundingBox< int32_t >& boundingBox ) const" );

}


template class gkg::IsmrmrdRecoDiskFormat< float >;
template class gkg::IsmrmrdRecoDiskFormat< double >;


//
// registrating standard Ismrmrd Reco disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< uint16_t > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int16_t > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< int16_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint32_t > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int32_t > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< int32_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< float > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< float >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< double > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRecoDiskFormat< double >::getInstance().getName(),
         &gkg::IsmrmrdRecoDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Ismrmrd Reco disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();

