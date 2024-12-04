#include <gkg-processing-plugin-functors/DiffeomorphismVolumeToMesh/DiffeomorphismVolumeToMeshCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::DiffeomorphismVolumeToMeshCommand::DiffeomorphismVolumeToMeshCommand(
                                                             int32_t argc,
                                                             char* argv[],
                                                             bool loadPlugin,
                                                             bool removeFirst )
                                       : gkg::Command( argc, argv, loadPlugin,
                                                       removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DiffeomorphismVolumeToMeshCommand::"
             "DiffeomorphismVolumeToMeshCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DiffeomorphismVolumeToMeshCommand::DiffeomorphismVolumeToMeshCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 float scale,
                                                 bool ascii,
                                                 const std::string& format,
                                                 bool verbose )
                                       : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             scale,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DiffeomorphismVolumeToMeshCommand::DiffeomorphismVolumeToMeshCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "float scale, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::DiffeomorphismVolumeToMeshCommand::DiffeomorphismVolumeToMeshCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             scale,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DiffeomorphismVolumeToMeshCommand::"
             "DiffeomorphismVolumeToMeshCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DiffeomorphismVolumeToMeshCommand::~DiffeomorphismVolumeToMeshCommand()
{
}


std::string gkg::DiffeomorphismVolumeToMeshCommand::getStaticName()
{

  try
  {

    return "DiffeomorphismVolumeToMesh";

  }
  GKG_CATCH( "std::string "
             "gkg::DiffeomorphismVolumeToMeshCommand::getStaticName()" );

}


void gkg::DiffeomorphismVolumeToMeshCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    float scale = 1.0f;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D transform inverter (restricted to "
                                  "homogeneous 3D transforms)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input float volume filename",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output mesh filename",
                                 fileNameOut );
    application.addSingleOption( "-s",
                                 "Scale to be applied to displacements "
                                 "(default=1)",
                                 scale,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             scale,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DiffeomorphismVolumeToMeshCommand::parse()" );

}


void gkg::DiffeomorphismVolumeToMeshCommand::execute(
                                               const std::string& fileNameIn,
                                               const std::string& fileNameOut,
                                               float scale,
                                               bool ascii,
                                               const std::string& format,
                                               bool verbose )
{

  try
  {

    //
    // reading input float volume 
    //
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn 
                << "' : "
                << std::flush;

    }
    gkg::Volume< float > volume;
    gkg::Reader::getInstance().read( fileNameIn,
                                     volume );
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    if ( sizeT != 1 )
    {

      throw std::runtime_error( "input volume is 4D and not 3D" );

    }


    gkg::Vector3d< double > resolution;
    volume.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    

    //
    // creating mesh
    //

    if ( verbose )
    {

      std::cout << "creating mesh : " << std::flush;

    }

    gkg::MeshMap< int32_t, float, 2U > meshMap;

    std::list< gkg::Vector3d< float > > vertices;
    std::list< gkg::Polygon< 2U > > polygons;

    uint32_t vertexCount = 0;

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    gkg::Vector3d< float > position;
    for ( z = 0; z < size.z; z++ )
    {

      for ( y = 0; y < size.y; y++ )
      {

        for ( x = 0; x < size.x; x++ )
        {

          position.x = ( float )x * ( float )resolution.x;
          position.y = ( float )y * ( float )resolution.y;
          position.z = ( float )z * ( float )resolution.z;

          gkg::Vector3d< float > orientation( volume( x, y, z, 0 ),
                                              volume( x, y, z, 1 ),
                                              volume( x, y, z, 2 ) );
          vertices.push_back( position - orientation * scale / 2.0f );
          vertices.push_back( position + orientation * scale / 2.0f );
      
          polygons.push_back( gkg::Polygon< 2U >( vertexCount,
                                                  vertexCount + 1U ) );

          vertexCount += 2U;


        }

      }

    }

    meshMap.vertices.addSites( 0, vertices );
    meshMap.polygons.addPolygons( 0, polygons );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // writing composition
    //

    if ( verbose )
    {

      std::cout << "wrting  '" << fileNameOut << "' : "
                << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOut, meshMap, ascii, format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DiffeomorphismVolumeToMeshCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "float scale, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( DiffeomorphismVolumeToMeshCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_FLOATING_PARAMETER_HELP( scale ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
