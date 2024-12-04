#include <gkg-dmri-io/TrackvisBundleMapDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
gkg::TrackvisBundleMapDiskFormat< L, Compare >::TrackvisBundleMapDiskFormat()
                                          : gkg::TypedDiskFormat<
                                            gkg::BundleMap< L, Compare > >()
{

  this->_extensions.insert( ".trk" );

}


template < class L, class Compare >
gkg::TrackvisBundleMapDiskFormat< L, Compare >::~TrackvisBundleMapDiskFormat()
{
}


template < class L, class Compare >
std::string gkg::TrackvisBundleMapDiskFormat< L, Compare >::getName() const
{

  return "trkbundlemap";

}


template < class L, class Compare >
void gkg::TrackvisBundleMapDiskFormat< L, Compare >::readHeader(
                                         const std::string& /*name*/,
                                         gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    //throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::TrackvisBundleMapDiskFormat< L, Compare >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class L, class Compare >
void gkg::TrackvisBundleMapDiskFormat< L, Compare >::writeHeader(
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const
{

  // no header in Trackvis BundleMap disk format

}


template < class L, class Compare >
void gkg::TrackvisBundleMapDiskFormat< L, Compare >::read(
                                const std::string& /*name*/,
                                gkg::BundleMap< L, Compare >& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::TrackvisBundleMapDiskFormat< L, Compare >::read( "
             "const std::string& /*name*/, "
             "gkg::BundleMap< L, Compare >& /*object*/ ) const" );

}


template < class L, class Compare >
void gkg::TrackvisBundleMapDiskFormat< L, Compare >::write(
                                         const std::string& name,
                                         gkg::BundleMap< L, Compare >& object,
                                         bool /*ascii*/ ) const
{

  try
  {

    // collecting item writer(s)
    gkg::ItemWriter< int16_t >& itemW_int16_t =
      gkg::DefaultItemIOFactory< int16_t >::getInstance().getWriter( false,
                                                                     false );
    gkg::ItemWriter< int32_t >& itemW_int32_t =
      gkg::DefaultItemIOFactory< int32_t >::getInstance().getWriter( false,
                                                                     false );
    gkg::ItemWriter< float >& itemW_float =
      gkg::DefaultItemIOFactory< float >::getInstance().getWriter( false,
                                                                   false );

    gkg::ItemWriter< int8_t >& itemW_int8_t =
      gkg::DefaultItemIOFactory< int8_t >::getInstance().getWriter( false,
                                                                     false );
    gkg::ItemWriter< uint8_t >& itemW_uint8_t =
      gkg::DefaultItemIOFactory< uint8_t >::getInstance().getWriter( false,
                                                                     false );


    // opening output file
    std::ofstream os( ( this->getNameWithoutExtension( name ) + 
                      ".trk" ).c_str() );

    // writing header from http://www.trackvis.org/docs/?subsect=fileformat
    //
    // id_string[6]                 char          6 TRACK\0
    // dim[3]                       short int     6 Dimension of the volume
    // voxel_size[3]                float        12 Voxel size of the volume
    // origin[3]                    float        12 Origin of the image volume
    // n_scalars                    short int     2 Number of scalars
    // scalar_name[10][20]          char        200 Name of each scalar
    // n_properties                 short int     2 Number of properties
    // property_name[10][20]        char        200 Name of each property
    // vox_to_ras[4][4]             float        64 4x4 matrix voxel to RAS
    // reserved[444]                char        444 Reserved space for futur
    // voxel_order[4]               char          4 Storing order
    // pad2[4]                      char          4 Paddings.
    // image_orientation_patient[6] float        24 Image orientation
    // pad1[2]                      char          2 Paddings.
    // invert_x                     unsigned char 1 Inversion/rotation flags
    // invert_y                     unsigned char 1 As above.
    // invert_x                     unsigned char 1 As above.
    // swap_xy                      unsigned char 1 As above.
    // swap_yz                      unsigned char 1 As above.
    // swap_zx                      unsigned char 1 As above.
    // n_count                      int           4 Number of tracks
    // version                      int           4 Version number
    // hdr_size                     int           4 Size of the header

    itemW_int8_t.write( os, ( int8_t )'T' );
    itemW_int8_t.write( os, ( int8_t )'R' );
    itemW_int8_t.write( os, ( int8_t )'A' );
    itemW_int8_t.write( os, ( int8_t )'C' );
    itemW_int8_t.write( os, ( int8_t )'K' );
    itemW_int8_t.write( os, ( int8_t )'\0' );


    int16_t sizeX = 1;
    if ( object.getHeader().hasAttribute( "sizeX" ) )
    {

      try
      {

        double doubleSizeX = 0.0;
        object.getHeader().getAttribute( "sizeX", doubleSizeX );
        sizeX = ( int16_t )( doubleSizeX + 0.5 );

      }
      catch ( std::exception& )
      {

       
        int32_t integerSizeX = 0.0;
        object.getHeader().getAttribute( "sizeX", integerSizeX );
        sizeX = ( int16_t )( integerSizeX + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "sizeX attribute missing" );

    }

    int16_t sizeY = 1;
    if ( object.getHeader().hasAttribute( "sizeY" ) )
    {

      try
      {

        double doubleSizeY = 0.0;
        object.getHeader().getAttribute( "sizeY", doubleSizeY );
        sizeY = ( int16_t )( doubleSizeY + 0.5 );

      }
      catch ( std::exception& )
      {

       
        int32_t integerSizeY = 0.0;
        object.getHeader().getAttribute( "sizeY", integerSizeY );
        sizeY = ( int16_t )( integerSizeY + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "sizeY attribute missing" );

    }

    int16_t sizeZ = 1;
    if ( object.getHeader().hasAttribute( "sizeZ" ) )
    {

      try
      {

        double doubleSizeZ = 0.0;
        object.getHeader().getAttribute( "sizeZ", doubleSizeZ );
        sizeZ = ( int16_t )( doubleSizeZ + 0.5 );

      }
      catch ( std::exception& )
      {

       
        int32_t integerSizeZ = 0.0;
        object.getHeader().getAttribute( "sizeZ", integerSizeZ );
        sizeZ = ( int16_t )( integerSizeZ + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "sizeZ attribute missing" );

    }

    float resolutionX = 1;
    if ( object.getHeader().hasAttribute( "resolutionX" ) )
    {

      double doubleResolutionX = 0.0;
      object.getHeader().getAttribute( "resolutionX", doubleResolutionX );
      resolutionX = ( float )doubleResolutionX;

    }
    else
    {

      throw std::runtime_error( "resolutionX attribute missing" );

    }

    float resolutionY = 1;
    if ( object.getHeader().hasAttribute( "resolutionY" ) )
    {

      double doubleResolutionY = 0.0;
      object.getHeader().getAttribute( "resolutionY", doubleResolutionY );
      resolutionY = ( float )doubleResolutionY;

    }
    else
    {

      throw std::runtime_error( "resolutionY attribute missing" );

    }

    float resolutionZ = 1;
    if ( object.getHeader().hasAttribute( "resolutionZ" ) )
    {

      double doubleResolutionZ = 0.0;
      object.getHeader().getAttribute( "resolutionZ", doubleResolutionZ );
      resolutionZ = ( float )doubleResolutionZ;

    }
    else
    {

      throw std::runtime_error( "resolutionZ attribute missing" );

    }

    // adding volume size and resolution
    itemW_int16_t.write( os, sizeX );
    itemW_int16_t.write( os, sizeY );
    itemW_int16_t.write( os, sizeZ );
    itemW_float.write( os, resolutionX );
    itemW_float.write( os, resolutionY );
    itemW_float.write( os, resolutionZ );

    // adding origin
    gkg::Vector3d< float > origin( 0.0, 0.0, 0.0 );
    itemW_float.write( os, origin.x );
    itemW_float.write( os, origin.y );
    itemW_float.write( os, origin.z);

    // adding number of scalar at each track point
    itemW_int16_t.write( os, ( int16_t )0 );

    // adding scalar name(s)
    int32_t c = 0;
    for ( c = 0; c < 10 * 20; c++ )
    {

       itemW_int8_t.write( os, ( int8_t )0 );

    }

    // adding number of properties per track
    itemW_int16_t.write( os, ( int16_t )0 );

    // adding property name(s)
    for ( c = 0; c < 10 * 20; c++ )
    {

       itemW_int8_t.write( os, ( int8_t )0 );

    }

    // adding voxel to RAS transform
    itemW_float.write( os, resolutionX );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );

    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, resolutionY );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );

    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, resolutionZ );
    itemW_float.write( os, ( float )0 );

    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )1 );

    // reserved for the future generation(s)
    for ( c = 0; c < 444; c++ )
    {

       itemW_int8_t.write( os, ( int8_t )0 );

    }

    // adding voxel order
    itemW_int8_t.write( os, ( int8_t )'L' );
    itemW_int8_t.write( os, ( int8_t )'A' );
    itemW_int8_t.write( os, ( int8_t )'S' );
    itemW_int8_t.write( os, ( int8_t )'\0' );

    // adding paddings
    itemW_int8_t.write( os, ( int8_t )'L' );
    itemW_int8_t.write( os, ( int8_t )'A' );
    itemW_int8_t.write( os, ( int8_t )'S' );
    itemW_int8_t.write( os, ( int8_t )'\0' );

    // adding image orientation patient
    itemW_float.write( os, ( float )1 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )0 );
    itemW_float.write( os, ( float )-1 );
    itemW_float.write( os, ( float )0 );

    // adding paddings
    itemW_int8_t.write( os, ( int8_t )0 );
    itemW_int8_t.write( os, ( int8_t )0 );

    // adding inversion / rotation flags
    itemW_uint8_t.write( os, ( uint8_t )0 );
    itemW_uint8_t.write( os, ( uint8_t )0 );
    itemW_uint8_t.write( os, ( uint8_t )0 );
    itemW_uint8_t.write( os, ( uint8_t )0 );
    itemW_uint8_t.write( os, ( uint8_t )0 );
    itemW_uint8_t.write( os, ( uint8_t )0 );

    // adding global number of tracks
    int32_t globalFiberCount = 0;
    typename gkg::BundleMap< L, Compare >::const_iterator
      b = object.begin(),
      be = object.end();
    while ( b != be )
    {

      globalFiberCount += b->second.getCurve3dCount();
      ++ b;

    }
    itemW_int32_t.write( os, globalFiberCount );

    // adding Trackvis version
    int32_t version = 2;
    itemW_int32_t.write( os, version );

    // adding header size
    int32_t headerSize = 1000;
    itemW_int32_t.write( os, headerSize );

    // writing track(s)
    b = object.begin();
    while ( b != be )
    {

      // looping over 3D curve(s)
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        c = b->second.begin(),
        ce = b->second.end();
      while ( c != ce )
      {

        const typename gkg::BundleMap< L, Compare >::Fiber& fiber = *c;

        // writing 3D point count
        itemW_int32_t.write( os, fiber.getPointCount() );

        // looping over 3D point(s)
        gkg::LightCurve3d< float >::const_iterator
          p = fiber.begin(),
          pe = fiber.end();
        while ( p != pe )
        {

          // writing current 3D point
          itemW_float.write( os, p->x );
          itemW_float.write( os, p->y );
          itemW_float.write( os, p->z );
          ++ p;

        }
        ++ c;

      }
      ++ b;

    }

    
    // closing output file
    os.close();


  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::TrackvisBundleMapDiskFormat<L, Compare >::write( "
             "const std::string& "
             "name, gkg::BundleMap< L, Compare >& object, "
             "bool /*ascii*/ ) const" );

}


template < class L, class Compare >
bool gkg::TrackvisBundleMapDiskFormat< L, Compare >::hasReader() const
{

  return false;

}


template < class L, class Compare >
bool gkg::TrackvisBundleMapDiskFormat< L, Compare >::hasWriter() const
{

  return true;

}


//
// TRACKVIS BundleMap disk format singleton instanciation
//

template class gkg::TrackvisBundleMapDiskFormat< int8_t >;
template class gkg::TrackvisBundleMapDiskFormat< uint8_t >;
template class gkg::TrackvisBundleMapDiskFormat< int16_t >;
template class gkg::TrackvisBundleMapDiskFormat< uint16_t >;
template class gkg::TrackvisBundleMapDiskFormat< int32_t >;
template class gkg::TrackvisBundleMapDiskFormat< uint32_t >;
template class gkg::TrackvisBundleMapDiskFormat< int64_t >;
template class gkg::TrackvisBundleMapDiskFormat< uint64_t >;
template class gkg::TrackvisBundleMapDiskFormat< std::string >;


//
// registrating TRACKVIS BundleMap disk format for "BundleMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMap< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< std::string > 
                         >::getInstance().registerDiskFormat(
         gkg::TrackvisBundleMapDiskFormat< std::string >::getInstance().getName(),
         &gkg::TrackvisBundleMapDiskFormat< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "TRACKVIS BundleMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
