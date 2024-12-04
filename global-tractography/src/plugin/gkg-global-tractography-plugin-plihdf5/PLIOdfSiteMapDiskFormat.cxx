#include <gkg-global-tractography-plugin-plihdf5/PLIOdfSiteMapDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>

#include <filesystem>
#include <numeric>
#include <PLIHDF5/dataset.h>
#include <PLIHDF5/file.h>


gkg::PLIOdfSiteMapDiskFormat::PLIOdfSiteMapDiskFormat()
                            : gkg::TypedDiskFormat<
                                       gkg::SiteMap< int32_t,
                                                     int32_t,
                                                     std::less< int32_t > > >()
{

  this->_extensions.insert( ".h5" );

}


gkg::PLIOdfSiteMapDiskFormat::~PLIOdfSiteMapDiskFormat()
{
}


std::string gkg::PLIOdfSiteMapDiskFormat::getName() const
{

  return "pli-odf-sitemap";

}


void gkg::PLIOdfSiteMapDiskFormat::readHeader(
                                         const std::string& /*name*/,
                                         gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "void gkg::PLIOdfSiteMapDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::PLIOdfSiteMapDiskFormat::writeHeader(
                                       const std::string& /*name*/,
                                       const gkg::HeaderedObject& /*object*/ ) const
{

  // no header in PLIHDF5 SiteMap disk format

}


void gkg::PLIOdfSiteMapDiskFormat::read(
                        const std::string& name,
                        gkg::SiteMap< int32_t,
                                      int32_t,
                                      std::less< int32_t > >& object ) const
{

  try
  {

    // initializing MPI
    int32_t initialized = 0;
    MPI_Initialized( &initialized );
    if ( !initialized )
    {

      MPI_Init( 0, 0 );

    }

    // opening the file
    auto file = PLI::HDF5::openFile( name );

    // getting access to the dataset corresponding to the ODF(s)
    auto dataSet = PLI::HDF5::openDataset( file, "/ODF" );

    // collecting size and resolution
    const std::vector< hsize_t >& dims = dataSet.dims();

    hsize_t sizeX = dims[ 2 ];
    hsize_t sizeY = dims[ 1 ];
    hsize_t sizeZ = dims[ 0 ];
    hsize_t sphCoefficientCount = dims[ 3 ];

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    // read full dataset
    auto data = dataSet.readFullDataset< float >();


    std::list< gkg::Vector3d< int32_t > > sites;
    // looping iover the dataset to extract the positions where an ODF is
    // stored

    hsize_t x = 0U;
    hsize_t y = 0U;
    hsize_t z = 0U;
    hsize_t c = 0U;
    bool isOutside = true;
    std::vector< float >::const_iterator d = data.begin();
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          isOutside = true;
          for ( c = 0; c < sphCoefficientCount; c++ )
          {

            if ( *d != 0.0f )
            {

              isOutside = false;
              d += sphCoefficientCount - c;
              break;

            }
            else
            {

              ++ d;

            }

          }

          if ( !isOutside )
          {

            sites.push_back( gkg::Vector3d< int32_t >( ( int32_t )x,
                                                       ( int32_t )y,
                                                       ( int32_t )z ) );

          }

        }

      }

    }

    object.addSites( 0, sites );

    object.getHeader().addAttribute( "sizeX", ( int32_t )sizeX );
    object.getHeader().addAttribute( "sizeY", ( int32_t )sizeY );
    object.getHeader().addAttribute( "sizeZ", ( int32_t )sizeZ );

    object.getHeader().addAttribute( "resolutionX", resolution.x );
    object.getHeader().addAttribute( "resolutionY", resolution.y );
    object.getHeader().addAttribute( "resolutionZ", resolution.z );

    dataSet.close();
    file.close();

  }
  GKG_CATCH( "void gkg::PLIOdfSiteMapDiskFormat::read( "
             "const std::string& name, "
             "gkg::SiteMap< "
             "int32_t, "
             "int32_t, "
             "std::less< int32_t > >& object ) const" );

}


void gkg::PLIOdfSiteMapDiskFormat::write(
                               const std::string& /*name*/,
                               gkg::SiteMap< int32_t,
                                             int32_t,
                                             std::less< int32_t > >& /*object*/,
                               bool /*ascii*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "void gkg::PLIOdfSiteMapDiskFormat::write( "
             "const std::string& name, "
             "gkg::SiteMap< "
             "int32_t, "
             "int32_t, "
             "std::less< int32_t > >& object, "
             "bool /*ascii*/ ) const" );

}


bool gkg::PLIOdfSiteMapDiskFormat::hasReader() const
{

  return true;

}


bool gkg::PLIOdfSiteMapDiskFormat::hasWriter() const
{

  return false;

}


//
// registrating PLIHDF5 ODF SiteMap disk format for "SiteMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::SiteMap< int32_t,
                                          int32_t,
                                          std::less< int32_t > >
                          >::getInstance().registerDiskFormat(
         gkg::PLIOdfSiteMapDiskFormat::getInstance().getName(),
         &gkg::PLIOdfSiteMapDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "PLI ODF SiteMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
