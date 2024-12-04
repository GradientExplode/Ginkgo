#include <gkg-dmri-io/TckBundleMapDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <cmath>


gkg::TckBundleMapDiskFormat::TckBundleMapDiskFormat()
                            : gkg::TypedDiskFormat<
                              gkg::BundleMap< std::string > >()
{

  this->_extensions.insert( ".tck" );

}


gkg::TckBundleMapDiskFormat::~TckBundleMapDiskFormat()
{
}


std::string gkg::TckBundleMapDiskFormat::getName() const
{

  return "tckbundlemap";

}


void gkg::TckBundleMapDiskFormat::readHeader(
                                         const std::string& name,
                                         gkg::HeaderedObject& object ) const
{

  try
  {

    // first we add all Tck BUndleMap specific attributes
    object.addSemantic( "__tckbundlemap__", "downsample_factor",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "fod_power",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "init_threshold",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "lmax",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "max_angle",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "max_num_seeds",
                       gkg::Semantic( gkg::TypeOf< uint64_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "max_num_tracks",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "max_seed_attempts",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "max_trials",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "method",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "mrtrix_version",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "output_step_size",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "rk4",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "samples_per_step",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "seed_dynamic",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "sh_precomputed",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "source",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "step_size",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "stop_on_all_include",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "threshold",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "timestamp",
                       gkg::Semantic( gkg::TypeOf< float >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "unidirectional",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "datatype",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "file",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "count",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    object.addSemantic( "__tckbundlemap__", "total_count",
                       gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );

    // opening input file
    std::ifstream is( ( this->getNameWithoutExtension( name ) + 
                      ".tck" ).c_str() );

    std::string tag;

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "mrtrix" )
    {

      throw std::runtime_error( "'mrtrix' tag expected" );

    }

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "tracks" )
    {

      throw std::runtime_error( "'tracks' tag expected" );

    }

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "downsample_factor:" )
    {

      throw std::runtime_error( "'downsample_factor' tag expected" );

    }
    int32_t downsample_factor = 0;
    is >> downsample_factor;
    object.getHeader().addAttribute( "downsample_factor", downsample_factor );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "fod_power:" )
    {

      throw std::runtime_error( "'fod_power' tag expected" );

    }
    float fod_power = 0;
    is >> fod_power;
    object.getHeader().addAttribute( "fod_power", fod_power );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "init_threshold:" )
    {

      throw std::runtime_error( "'init_threshold' tag expected" );

    }
    float init_threshold = 0;
    is >> init_threshold;
    object.getHeader().addAttribute( "init_threshold", init_threshold );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "lmax:" )
    {

      throw std::runtime_error( "'lmax' tag expected" );

    }
    int32_t lmax = 0;
    is >> lmax;
    object.getHeader().addAttribute( "lmax", lmax );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "max_angle:" )
    {

      throw std::runtime_error( "'max_angle' tag expected" );

    }
    float max_angle = 0;
    is >> max_angle;
    object.getHeader().addAttribute( "max_angle", max_angle );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;

    if ( tag == "max_dist:" )
    {

      object.addSemantic( "__tckbundlemap__", "max_dist",
                         gkg::Semantic( gkg::TypeOf< float >::getName() ) );
      float max_dist = 0;
      is >> max_dist;
      object.getHeader().addAttribute( "max_dist", max_dist );
      is >> tag;

    }
    if ( tag != "max_num_seeds:" )
    {

      throw std::runtime_error( "'max_num_seeds' tag expected" );

    }
    uint64_t max_num_seeds = 0;
    is >> max_num_seeds;
    object.getHeader().addAttribute( "max_num_seeds", max_num_seeds );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "max_num_tracks:" )
    {

      throw std::runtime_error( "'max_num_tracks' tag expected" );

    }
    int32_t max_num_tracks = 0;
    is >> max_num_tracks;
    object.getHeader().addAttribute( "max_num_tracks", max_num_tracks );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "max_seed_attempts:" )
    {

      throw std::runtime_error( "'max_seed_attempts' tag expected" );

    }
    int32_t max_seed_attempts = 0;
    is >> max_seed_attempts;
    object.getHeader().addAttribute( "max_seed_attempts", max_seed_attempts );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "max_trials:" )
    {

      throw std::runtime_error( "'max_trials' tag expected" );

    }
    int32_t max_trials = 0;
    is >> max_trials;
    object.getHeader().addAttribute( "max_trials", max_trials );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "method:" )
    {

      throw std::runtime_error( "'method' tag expected" );

    }
    std::string method;
    is >> method;
    object.getHeader().addAttribute( "method", method );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;

    if ( tag == "min_dist:" )
    {

      object.addSemantic( "__tckbundlemap__", "min_dist",
                         gkg::Semantic( gkg::TypeOf< float >::getName() ) );
      float min_dist = 0;
      is >> min_dist;
      object.getHeader().addAttribute( "min_dist", min_dist );
      is >> tag;

    }
    if (tag != "mrtrix_version:" )
    {

      throw std::runtime_error( "'mrtrix_version' tag expected" );

    }
    std::string mrtrix_version;
    is >> mrtrix_version;
    object.getHeader().addAttribute( "mrtrix_version", mrtrix_version );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "output_step_size:" )
    {

      throw std::runtime_error( "'output_step_size' tag expected" );

    }
    float output_step_size = 0;
    is >> output_step_size;
    object.getHeader().addAttribute( "output_step_size", output_step_size );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "rk4:" )
    {

      throw std::runtime_error( "'rk4' tag expected" );

    }
    int32_t rk4 = 0;
    is >> rk4;
    object.getHeader().addAttribute( "rk4", rk4 );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "samples_per_step:" )
    {

      throw std::runtime_error( "'samples_per_step' tag expected" );

    }
    int32_t samples_per_step = 0;
    is >> samples_per_step;
    object.getHeader().addAttribute( "samples_per_step", samples_per_step );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "seed_dynamic:" )
    {

      throw std::runtime_error( "'seed_dynamic' tag expected" );

    }
    std::string seed_dynamic;
    is >> seed_dynamic;
    object.getHeader().addAttribute( "seed_dynamic", seed_dynamic );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "sh_precomputed:" )
    {

      throw std::runtime_error( "'sh_precomputed' tag expected" );

    }
    int32_t sh_precomputed = 0;
    is >> sh_precomputed;
    object.getHeader().addAttribute( "sh_precomputed", sh_precomputed );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "source:" )
    {

      throw std::runtime_error( "'source' tag expected" );

    }
    std::string source;
    is >> source;
    object.getHeader().addAttribute( "source", source );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "step_size:" )
    {

      throw std::runtime_error( "'step_size' tag expected" );

    }
    float step_size = 0;
    is >> step_size;
    object.getHeader().addAttribute( "step_size", step_size );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "stop_on_all_include:" )
    {

      throw std::runtime_error( "'stop_on_all_include' tag expected" );

    }
    int32_t stop_on_all_include = 0;
    is >> stop_on_all_include;
    object.getHeader().addAttribute( "stop_on_all_include", stop_on_all_include );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "threshold:" )
    {

      throw std::runtime_error( "'threshold' tag expected" );

    }
    float threshold = 0;
    is >> threshold;
    object.getHeader().addAttribute( "threshold", threshold );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if ( tag != "timestamp:" )
    {

      throw std::runtime_error( "'timestamp' tag expected" );

    }
    float timestamp = 0;
    is >> timestamp;
    object.getHeader().addAttribute( "timestamp", timestamp );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "unidirectional:" )
    {

      throw std::runtime_error( "'unidirectional' tag expected" );

    }
    int32_t unidirectional = 0;
    is >> unidirectional;
    object.getHeader().addAttribute( "unidirectional", unidirectional );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    int32_t roiCount = 1;
    while (tag == "roi:" )
    {

      std::string inclusionExclusion;
      std::string roi;
      is >> inclusionExclusion >> roi;
      object.getHeader().addAttribute( std::string( "roi" ) +
                                       gkg::StringConverter::toString( roiCount ),
                                       roi );
      ++ roiCount;
      is >> tag;

    }
    std::string datatype;
    is >> datatype;
    object.getHeader().addAttribute( "datatype", datatype );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "file:" )
    {

      throw std::runtime_error( "'file' tag expected" );

    }
    std::string point;
    int32_t binaryFileOffset = 0;
    is >> point >> binaryFileOffset;
    object.getHeader().addAttribute( "binaryFileOffset", binaryFileOffset );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "count:" )
    {

      throw std::runtime_error( "'count' tag expected" );

    }
    int32_t count = 0;
    is >> count;
    object.getHeader().addAttribute( "count", count );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "total_count:" )
    {

      throw std::runtime_error( "'total_count' tag expected" );

    }
    int32_t total_count = 0;
    is >> total_count;
    object.getHeader().addAttribute( "total_count", total_count );

    //////////////////////////////////////////////////////////////////////////////
    is >> tag;
    if (tag != "END" )
    {

      throw std::runtime_error( "'END' tag expected" );

    }

    is.close();

  }
  GKG_CATCH( "void gkg::TckBundleMapDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::TckBundleMapDiskFormat::read(
                                const std::string& name,
                                gkg::BundleMap< std::string >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 


    int32_t binaryFileOffset = 0;
    object.getHeader().getAttribute( "binaryFileOffset", binaryFileOffset );

    int32_t fiberCount = 0;
    object.getHeader().getAttribute( "count", fiberCount );

    std::string dataType;
    object.getHeader().getAttribute( "datatype", dataType );

    std::ifstream is( ( this->getNameWithoutExtension( name ) + 
                      ".tck" ).c_str() );

    is.seekg( binaryFileOffset );


    gkg::ItemReader< float >* itemR_float = 0;
    gkg::ItemReader< double >* itemR_double = 0;
    if ( dataType == "Float32LE" )
    {

      if ( gkg::ByteOrder::getInstance().getCurrentType() == 
           gkg::ByteOrder::LittleEndian )
      {

        itemR_float =
          &gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                        false );

      }
      else
      {

        itemR_float =
          &gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                        true );

      }

    }
    else if ( dataType == "Float32BE" )
    {

      if ( gkg::ByteOrder::getInstance().getCurrentType() == 
           gkg::ByteOrder::BigEndian )
      {

        itemR_float =
          &gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                        false );

      }
      else
      {

        itemR_float =
          &gkg::DefaultItemIOFactory< float >::getInstance().getReader( false,
                                                                        true );

      }

    }
    else if ( dataType == "Float64LE" )
    {

      if ( gkg::ByteOrder::getInstance().getCurrentType() == 
           gkg::ByteOrder::LittleEndian )
      {

        itemR_double =
          &gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                         false );

      }
      else
      {

        itemR_double =
          &gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                         true );

      }

    }
    else if ( dataType == "Float64BE" )
    {

      if ( gkg::ByteOrder::getInstance().getCurrentType() == 
           gkg::ByteOrder::BigEndian )
      {

        itemR_double =
          &gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                         false );

      }
      else
      {

        itemR_double =
          &gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                         true );

      }

    }


    std::vector< gkg::LightCurve3d< float > > fibers( fiberCount );
    int32_t fiberIndex = 0;
    gkg::Vector3d< float > currentPointFloat;
    gkg::Vector3d< double > currentPointDouble;

    for ( fiberIndex = 0; fiberIndex < fiberCount; fiberIndex++ )
    {

      std::list< gkg::Vector3d< float > > points;
      if ( ( dataType == "Float32LE" ) || ( dataType == "Float32BE" ) )
      {

        itemR_float->read( is, currentPointFloat.x );
        itemR_float->read( is, currentPointFloat.y );
        itemR_float->read( is, currentPointFloat.z );

        while ( !std::isnan( currentPointFloat.x ) &&
                !std::isinf( currentPointFloat.x ) )
        {

          points.push_back( currentPointFloat );

          itemR_float->read( is, currentPointFloat.x );
          itemR_float->read( is, currentPointFloat.y );
          itemR_float->read( is, currentPointFloat.z );

        }

      }
      else
      {

        itemR_double->read( is, currentPointDouble.x );
        itemR_double->read( is, currentPointDouble.y );
        itemR_double->read( is, currentPointDouble.z );

        while ( !std::isnan( currentPointDouble.x ) &&
                !std::isinf( currentPointDouble.x ) )
        {

          points.push_back(
                        gkg::Vector3d< float >( ( float )currentPointDouble.x,
                                                ( float )currentPointDouble.y,
                                                ( float )currentPointDouble.z ) );

          itemR_double->read( is, currentPointDouble.x );
          itemR_double->read( is, currentPointDouble.y );
          itemR_double->read( is, currentPointDouble.z );

        }

      }
      fibers[ fiberIndex ].addPoints( points );


    }

    // adding fibers to bundle
    object.addCurve3ds( "1", fibers );
    is.close();

  }
  GKG_CATCH( "void gkg::TckBundleMapDiskFormat::read( "
             "const std::string& name, "
             "gkg::BundleMap< std::string >& object ) const" );

}


bool gkg::TckBundleMapDiskFormat::hasReader() const
{

  return true;

}


//
// registrating MRTrix TCK BundleMap disk format for "BundleMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMap< std::string > 
                         >::getInstance().registerDiskFormat(
         gkg::TckBundleMapDiskFormat::getInstance().getName(),
         &gkg::TckBundleMapDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "MRTrix TCK BundleMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
