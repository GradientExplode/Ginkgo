#include <gkg-global-tractography-plugin-plihdf5/PLIOdfTextureMapDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-odf-aqbi/AnalyticalQBallOdf.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>
#include <filesystem>
#include <numeric>
#include <PLIHDF5/dataset.h>
#include <PLIHDF5/file.h>


gkg::PLIOdfTextureMapDiskFormat::PLIOdfTextureMapDiskFormat()
                                : gkg::TypedDiskFormat<
                                    gkg::TextureMap<
                                      gkg::OrientationDistributionFunction > >()
{

  this->_extensions.insert( ".h5" );

}


gkg::PLIOdfTextureMapDiskFormat::~PLIOdfTextureMapDiskFormat()
{
}


std::string gkg::PLIOdfTextureMapDiskFormat::getName() const
{

  return "pli-odf-texturemap";

}


void gkg::PLIOdfTextureMapDiskFormat::readHeader(
                                         const std::string& /*name*/,
                                         gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "void gkg::PLIOdfTextureMapDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::PLIOdfTextureMapDiskFormat::writeHeader(
                                       const std::string& /*name*/,
                                       const gkg::HeaderedObject& /*object*/ ) const
{

  // no header in PLIHDF5 SiteMap disk format

}


void gkg::PLIOdfTextureMapDiskFormat::read(
                    const std::string& name,
                    gkg::TextureMap<
                      gkg::OrientationDistributionFunction >& object ) const
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

    // looping over the dataset to extract the positions where an ODF is
    // stored
    hsize_t x = 0U;
    hsize_t y = 0U;
    hsize_t z = 0U;
    hsize_t c = 0U;
    bool isOutside = true;
    std::vector< float >::const_iterator d = data.begin();
    std::vector< float > shCoefficients( sphCoefficientCount );
    std::list< gkg::OrientationDistributionFunction > odfs;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          isOutside = true;
          for ( c = 0; c < sphCoefficientCount; c++ )
          {

            shCoefficients[ c ] = *d;
            if ( *d != 0.0f )
            {

              isOutside = false;

            }
            ++ d;

          }

          if ( !isOutside )
          {

            gkg::OrientationDistributionFunction
              odf( gkg::OrientationDistributionFunction::SphericalHarmonics );
            odf.setValues( shCoefficients );

            odfs.push_back( odf );

          }

        }

      }

    }

    object.addTextures( odfs );

    object.getHeader().addAttribute( "resolutionX", resolution.x );
    object.getHeader().addAttribute( "resolutionY", resolution.y );
    object.getHeader().addAttribute( "resolutionZ", resolution.z );

    object.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::AnalyticalQBallOdf >::getName() );

    dataSet.close();
    file.close();


  }
  GKG_CATCH( "void gkg::PLIOdfTextureMapDiskFormat::read( "
             "const std::string& /*name*/, "
             "gkg::TextureMap< "
             "gkg::OrientationDistributionFunction >& /*object*/ ) const" );

}


void gkg::PLIOdfTextureMapDiskFormat::write(
                           const std::string& /*name*/,
                           gkg::TextureMap<
                             gkg::OrientationDistributionFunction >& /*object*/,
                           bool /*ascii*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "void gkg::PLIOdfTextureMapDiskFormat::write( "
             "const std::string& name, "
             "gkg::TextureMap< "
             "gkg::OrientationDistributionFunction >& object, "
             "bool /*ascii*/ ) const" );

}


bool gkg::PLIOdfTextureMapDiskFormat::hasReader() const
{

  return true;

}


bool gkg::PLIOdfTextureMapDiskFormat::hasWriter() const
{

  return false;

}


//
// registrating PLIHDF5 ODF TextureMap disk format for 
//   "TextureMap< OrientationDistributionFunction>" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::TextureMap<
                                 gkg::OrientationDistributionFunction >
                          >::getInstance().registerDiskFormat(
         gkg::PLIOdfTextureMapDiskFormat::getInstance().getName(),
         &gkg::PLIOdfTextureMapDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "PLI ODF TextureMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
