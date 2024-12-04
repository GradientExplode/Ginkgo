#include <gkg-dmri-plugin-functors/MeshMapSlicer/MeshMapSlicerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/MeshSlicer.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>



gkg::MeshMapSlicerCommand::MeshMapSlicerCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                          : gkg::Command( argc, argv, 
                                          loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MeshMapSlicerCommand::MeshMapSlicerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MeshMapSlicerCommand::MeshMapSlicerCommand(
                                             const std::string& fileNameMeshMap,
                                             const std::string& fileNameOut,
                                             const std::string& bundleMapFormat,
                                             float epsilon,
                                             float minimumThickness,
                                             float maximumThickness,
                                             bool adaptive,
                                             bool clockwise,
                                             bool verbose )
                          : gkg::Command()
{

  try
  {

    execute( fileNameMeshMap,
             fileNameOut,
             bundleMapFormat,
             epsilon,
             minimumThickness,
             maximumThickness,
             adaptive,
             clockwise,
             verbose );

  }
  GKG_CATCH( "gkg::MeshMapSlicerCommand::MeshMapSlicerCommand( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "const std::string& bundleMapFormat, "
             "float epsilon, "
             "float minimumThickness, "
             "float maximumThickness, "
             "bool adaptive, "
             "bool clockwise, "
             "bool verbose )" );

}


gkg::MeshMapSlicerCommand::MeshMapSlicerCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, bundleMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, double, epsilon );
    DECLARE_FLOATING_PARAMETER( parameters, double, minimumThickness );
    DECLARE_FLOATING_PARAMETER( parameters, double, maximumThickness );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, adaptive );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, clockwise );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameMeshMap,
             fileNameOut,
             bundleMapFormat,
             epsilon,
             minimumThickness,
             maximumThickness,
             adaptive,
             clockwise,
             verbose );

  }
  GKG_CATCH( "gkg::MeshMapSlicerCommand::MeshMapSlicerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MeshMapSlicerCommand::~MeshMapSlicerCommand()
{
}


std::string gkg::MeshMapSlicerCommand::getStaticName()
{

  try
  {

    return "MeshMapSlicer";

  }
  GKG_CATCH( "std::string gkg::MeshMapSlicerCommand::getStaticName()" );

}


void gkg::MeshMapSlicerCommand::parse()
{

  try
  {

    std::string fileNameMeshMap;
    std::string fileNameOut;
    std::string bundleMapFormat = "aimsbundlemap";
    float epsilon = 0.004;
    float minimumThickness = 0.032;
    float maximumThickness = 1.0;
    bool adaptive = false;
    bool clockwise = true;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Bundle map slicer along the z-axis",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input mesh map",
                                 fileNameMeshMap );
    application.addSingleOption( "-o",
                                 "Output bundle map",
                                 fileNameOut );
    application.addSingleOption( "-f",
                                 "Bundle map format (default=aimsbundlemap)",
                                 bundleMapFormat,
                                 true );
    application.addSingleOption( "-epsilon",
                                 "Epsilon precision on the plane positions "
                                 "(default=0.004mm)",
                                 epsilon,
                                 true );
    application.addSingleOption( "-minimumThickness",
                                 "Minimum slice thickness, only used for "
                                 "adaptive slicing (default=0.016mm)",
                                 minimumThickness,
                                 true );
    application.addSingleOption( "-maximumThickness",
                                 "Maximum slice thickness "
                                 "(default=1.0mm)",
                                 maximumThickness,
                                 true );
    application.addSingleOption( "-adaptive",
                                 "Use adaptibe versus uniform slicing "
                                 "(default=false)",
                                 adaptive,
                                 true ); 
    application.addSingleOption( "-clockwise",
                                 "Force clockwise contour polygons when true "
                                 "and counter-clockwise when set to false "
                                 "(default=true)",
                                 clockwise,
                                 true ); 
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 

    application.initialize(); 			 

    execute( fileNameMeshMap,
             fileNameOut,
             bundleMapFormat,
             epsilon,
             minimumThickness,
             maximumThickness,
             adaptive,
             clockwise,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MeshMapSlicerCommand::parse()" );

}


void gkg::MeshMapSlicerCommand::execute( const std::string& fileNameMeshMap,
                                         const std::string& fileNameOut,
                                         const std::string& bundleMapFormat,
                                         float epsilon,
                                         float minimumThickness,
                                         float maximumThickness,
                                         bool adaptive,
                                         bool clockwise,
                                         bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameMeshMap << "' mesh map : "
                << std::flush;
     
    }
    gkg::MeshMap< int32_t, float, 3U > meshMap;
    std::string format = gkg::Reader::getInstance().read( fileNameMeshMap,
                                                          meshMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // creating mesh slicer
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "creating mesh slicer : "
                << std::flush;
     
    }
    gkg::MeshSlicer meshSlicer( epsilon,
                                minimumThickness,
                                maximumThickness,
                                adaptive,
                                clockwise,
                                verbose );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // slicing
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "slicing ... "
                << std::endl;
     
    }
    std::map< int32_t, gkg::Curve3dMap< float > > curve3dMaps;
    meshSlicer.slicing( meshMap, curve3dMaps );



    ////////////////////////////////////////////////////////////////////////////
    // saving results to a bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "writing results to '" << fileNameOut
                << "' bundle map : "
                << std::flush;
     
    }
    gkg::BundleMap< std::string > bundleMap;
    std::map< int32_t, gkg::Curve3dMap< float > >::const_iterator
      cm = curve3dMaps.begin(),
      cme = curve3dMaps.end();
    while ( cm != cme )
    {

      bundleMap.addBundle( gkg::StringConverter::toString( cm->first ),
                           cm->second );
      ++ cm;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      bundleMap,
                                      false,
                                      bundleMapFormat );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }



  }
  GKG_CATCH( "void gkg::MeshMapSlicerCommand::execute( "
             "const std::string& fileNameMeshMap, "
             "const std::string& fileNameOut, "
             "const std::string& bundleMapFormat, "
             "float epsilon, "
             "float minimumThickness, "
             "float maximumThickness, "
             "bool adaptive, "
             "bool clockwise, "
             "bool verbose )" );

}


RegisterCommandCreator( MeshMapSlicerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameMeshMap ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( bundleMapFormat ) +
    			DECLARE_FLOATING_PARAMETER_HELP( epsilon ) +
    			DECLARE_FLOATING_PARAMETER_HELP( minimumThickness ) +
    			DECLARE_FLOATING_PARAMETER_HELP( maximumThickness ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( adaptive ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( clockwise ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
