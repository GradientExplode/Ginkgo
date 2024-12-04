#include <gkg-mri-reconstruction-io/IsmrmrdRawDiskFormat.h>
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
#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/FormatMismatchException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/xml.h>
#include <ismrmrd/dataset.h>
#include <complex>
#include <vector>
#include <fstream>


//
// class IsmrmrdRawDiskFormat
//


template < class T >
gkg::IsmrmrdRawDiskFormat< T >::IsmrmrdRawDiskFormat()
                               : gkg::TypedDiskFormat<
                                            gkg::Volume< std::complex< T > > >()
{

  try
  {


    // ISMRMRD files are HDF5 files, so ending with .h5
    this->_extensions.insert( ".h5" );

  }
  GKG_CATCH( "template < class T > "
             "gkg::IsmrmrdRawDiskFormat< T >::IsmrmrdRawDiskFormat()" );

}


template < class T >
gkg::IsmrmrdRawDiskFormat< T >::~IsmrmrdRawDiskFormat()
{
}


template < class T >
std::string gkg::IsmrmrdRawDiskFormat< T >::getName() const
{

  return "ismrmrd-raw";

}


template < class T >
void gkg::IsmrmrdRawDiskFormat< T >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    ISMRMRD::Dataset dataset( name.c_str(), "dataset", false );

    if ( !dataset.getNumberOfAcquisitions() )
    {

      throw gkg::FormatMismatchException( name );

    }
    ISMRMRD::Acquisition acquisitionData;
    dataset.readAcquisition( 0, acquisitionData );


    std::string xmlHeader;
    dataset.readHeader( xmlHeader );
    ISMRMRD::IsmrmrdHeader ismrmrdHeader;
    ISMRMRD::deserialize( xmlHeader.c_str(), ismrmrdHeader );
    ISMRMRD::EncodingSpace 
      encodingSpace = ismrmrdHeader.encoding[ 0 ].encodedSpace;


    int32_t sizeX = encodingSpace.matrixSize.x;
    int32_t sizeY = encodingSpace.matrixSize.y;
    int32_t sizeZ = ( int32_t )( encodingSpace.fieldOfView_mm.z + 0.5 );
                     //encodingSpace.matrixSize.z;
    int32_t coilCount = acquisitionData.active_channels();
    int32_t acquisitionCount = dataset.getNumberOfAcquisitions();



    int32_t repetitionCount = acquisitionCount / ( sizeY * sizeZ );
    int32_t sizeT = coilCount * repetitionCount;

    double resolutionX = encodingSpace.fieldOfView_mm.x / ( double )sizeX;
    double resolutionY = encodingSpace.fieldOfView_mm.y / ( double )sizeY;
    double resolutionZ = encodingSpace.fieldOfView_mm.z / ( double )sizeZ;
    double resolutionT = 1.0;


    object.getHeader()[ "sizeX" ] = sizeX;
    object.getHeader()[ "sizeY" ] = sizeY;
    object.getHeader()[ "sizeZ" ] = sizeZ;
    object.getHeader()[ "sizeT" ] = sizeT;

    object.getHeader()[ "resolutionX" ] = resolutionX;
    object.getHeader()[ "resolutionY" ] = resolutionY;
    object.getHeader()[ "resolutionZ" ] = resolutionZ;
    object.getHeader()[ "resolutionT" ] = resolutionT;


    object.addSemantic( "__ismrmrd_raw__",
                        "coil_count",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader()[ "coil_count" ] = coilCount;

    object.addSemantic( "__ismrmrd_raw__",
                        "repetition_count",
                        gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.getHeader()[ "repetition_count" ] = repetitionCount;

    // notifying observers
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::IsmrmrdRawDiskFormat< T >::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


template < class T >
void gkg::IsmrmrdRawDiskFormat< T >::read(
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
             "void gkg::IsmrmrdRawDiskFormat< T >::read( "
             "const std::string& name, "
             "gkg::Volume< std::complex< T > >& object ) const" );

}


template < class T >
inline
void gkg::IsmrmrdRawDiskFormat< T >::preparePartialBinaryRead(
                                             const std::string& name,
                                             gkg::HeaderedObject& proxy ) const
{

  try
  {

    if ( &dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy ) )
    {

      gkg::VolumeProxy< std::complex< T > >& volumeProxy =
        dynamic_cast< gkg::VolumeProxy< std::complex< T > >& >( proxy );

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
  GKG_CATCH( "template < class T > void gkg::IsmrmrdRawDiskFormat< T >::"
             "preparePartialBinaryRead( "
             "const std::string& name, "
             "gkg::HeaderedObject& proxy ) const" );

}


template < class T >
void gkg::IsmrmrdRawDiskFormat< T >::partialBinaryRead(
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

        gkg::Volume< std::complex< T > >&
          subVolume = dynamic_cast< gkg::Volume< std::complex< T > >& >(
                                                                subpart );

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
            ISMRMRD::Dataset dataset( proxyFileName.c_str(), "dataset", false );
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
             "void gkg::IsmrmrdRawDiskFormat< T >::partialBinaryRead( "
             "const gkg::HeaderedObject& proxy, "
             "gkg::GenericObject& subpart, "
             "const gkg::RegionInformation& regionInformation ) const" );

}


template < class T >
bool gkg::IsmrmrdRawDiskFormat< T >::hasReader() const
{

  return true;

}


template < class T >
void gkg::IsmrmrdRawDiskFormat< T >::readPartialBinaryCore(
                       ISMRMRD::Dataset& dataset,
                       const gkg::VolumeProxy< std::complex< T > >& volumeProxy,
                       gkg::Volume< std::complex< T > >& subVolume,
                       const gkg::BoundingBox< int32_t >& boundingBox ) const
{

  try
  {

    int32_t subSizeX = subVolume.getSizeX();
    int32_t subSizeY = subVolume.getSizeY();
    int32_t subSizeZ = subVolume.getSizeZ();
    int32_t subSizeT = subVolume.getSizeT();

    int32_t onDiskSizeY = volumeProxy.getSizeY();
    int32_t onDiskSizeZ = volumeProxy.getSizeZ();

    int32_t bboxLowerX = boundingBox.getLowerX();
    int32_t bboxLowerY = boundingBox.getLowerY();
    int32_t bboxLowerZ = boundingBox.getLowerZ();
    int32_t bboxLowerT = boundingBox.getLowerT();


    int32_t coilCount = 0;
    volumeProxy.getHeader().getAttribute( "coil_count",
                                          coilCount );
    int32_t repetitionCount = 0;
    volumeProxy.getHeader().getAttribute( "repetition_count",
                                          repetitionCount );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;
    int32_t coilIndex = 0;
    int32_t repetitionIndex = 0;
    int32_t acquisitionIndex = 0;
    typename gkg::Volume< std::complex< T > >::iterator s = subVolume.begin();

    for ( t = 0; t < subSizeT; t++ )
    {

      coilIndex = ( bboxLowerT + t ) % coilCount; 
      repetitionIndex = ( bboxLowerT + t ) / coilCount; 

      for ( z = 0; z < subSizeZ; z++ )
      {

        for ( y = 0; y < subSizeY; y++ )
        {

          acquisitionIndex = ( y + bboxLowerY ) +
                             ( z + bboxLowerZ ) * onDiskSizeY +
                             ( repetitionIndex ) * onDiskSizeY * onDiskSizeZ;

          ISMRMRD::Acquisition acquisitionData;
          dataset.readAcquisition( acquisitionIndex, acquisitionData );

          for ( x = 0; x < subSizeX; x++ )
          {

            *s = std::complex< T >(
                      ( T )std::real( acquisitionData.data( bboxLowerX + x,
                                                            coilIndex ) ),
                      ( T )std::imag( acquisitionData.data( bboxLowerX + x,
                                                            coilIndex ) ) );
            ++ s;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::IsmrmrdRawDiskFormat< T >::readPartialBinaryCore( "
             "gkg::largefile_ifstream& is, "
             "const gkg::ItemReader< int >& itemR, "
             "const gkg::VolumeProxy< std::complex< T > >& volumeProxy, "
             "gkg::Volume< std::complex< T > >& subVolume, "
             "const gkg::BoundingBox< int >& boundingBox ) const" );

}


template class gkg::IsmrmrdRawDiskFormat< float >;
template class gkg::IsmrmrdRawDiskFormat< double >;


//
// registrating standard Ismrmrd Raw disk format(s) for "Volume" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRawDiskFormat< float >::getInstance().getName(),
         &gkg::IsmrmrdRawDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< double > > 
                           >::getInstance().registerDiskFormat(
         gkg::IsmrmrdRawDiskFormat< double >::getInstance().getName(),
         &gkg::IsmrmrdRawDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Ismrmrd Raw disk format registration" );

}

static bool initialized __attribute__((unused)) = initialize();

