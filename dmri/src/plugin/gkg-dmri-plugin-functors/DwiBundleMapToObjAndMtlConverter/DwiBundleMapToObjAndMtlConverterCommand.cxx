#include <gkg-dmri-plugin-functors/DwiBundleMapToObjAndMtlConverter/DwiBundleMapToObjAndMtlConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>


//
// class BundleMapToObjAndMtlConverterProcess
//

class BundleMapToObjAndMtlConverterProcess : public gkg::Process
{

  public:

    BundleMapToObjAndMtlConverterProcess( const std::string& theFileNameObj,
	                                  const std::string& theFileNameMtl,
	                                  const float& theStep,
	                                  const float& theRadius,
	                                  const int32_t& theVertexCountPerPlane,
	                                  const bool& theVerbose );

    const std::string& fileNameObj;
    const std::string& fileNameMtl;
    const float& step;
    const float& radius;
    const int32_t& vertexCountPerPlane;
    const bool& verbose;

  private:

    template < class L >
    static void convert( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


BundleMapToObjAndMtlConverterProcess::BundleMapToObjAndMtlConverterProcess(
                                          const std::string& theFileNameObj,
	                                  const std::string& theFileNameMtl,
	                                  const float& theStep,
	                                  const float& theRadius,
	                                  const int32_t& theVertexCountPerPlane,
	                                  const bool& theVerbose )
                                     : gkg::Process( "BundleMap" ),
                                       fileNameObj( theFileNameObj ),
                                       fileNameMtl( theFileNameMtl ),
                                       step( theStep ),
                                       radius( theRadius ),
                                       vertexCountPerPlane(
                                                       theVertexCountPerPlane ),
                                       verbose( theVerbose )
{

  try
  {

    registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                     &BundleMapToObjAndMtlConverterProcess::convert<
                                                                std::string > );
    registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                    &BundleMapToObjAndMtlConverterProcess::convert< int16_t > );

  }
  GKG_CATCH( "BundleMapToObjAndMtlConverterProcess::"
             "BundleMapToObjAndMtlConverterProcess( "
	     "const std::string& theFileNameObj, "
	     "const std::string& theFileNameMtl, "
             "const float& theStep, "
	     "const float& theRadius, "
	     "const int32_t& theVertexCountPerPlane, "
	     "const bool& theVerbose )" );

}


template < class L >
void 
BundleMapToObjAndMtlConverterProcess::convert( gkg::Process& process,
 					       const std::string& fileNameIn,
 					       const gkg::AnalyzedObject&,
 					       const std::string& )
{

  try
  {

    BundleMapToObjAndMtlConverterProcess&
      bundleMapToObjAndMtlConverterProcess =
        static_cast< BundleMapToObjAndMtlConverterProcess& >( process );

    const std::string& 
      fileNameObj = bundleMapToObjAndMtlConverterProcess.fileNameObj;
    const float&
      step = bundleMapToObjAndMtlConverterProcess.step;
    const float&
      radius = bundleMapToObjAndMtlConverterProcess.radius;
    const float&
      vertexCountPerPlane = bundleMapToObjAndMtlConverterProcess.
                                                            vertexCountPerPlane;
    const bool&
      verbose = bundleMapToObjAndMtlConverterProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameIn << "' bundle map : "
                << std::flush;
     
    }
    gkg::BundleMap< L > bundleMap;
    gkg::Reader::getInstance().read( fileNameIn, bundleMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }

    //////////////////////////////////////////////////////////////////////////
    // opening output stream
    //////////////////////////////////////////////////////////////////////////

    std::ofstream os( fileNameObj.c_str() );
    if ( os.fail() )
    {

      throw std::runtime_error( "could not open .obj file" );

    }


    //////////////////////////////////////////////////////////////////////////
    // converting fiber bundles
    //////////////////////////////////////////////////////////////////////////


    uint32_t offsetVertices = 0U;
    int32_t fiberIndex = 0U;
    int32_t fiberCount = 0U;

    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      if ( verbose )
      {

        std::cout << "processing bundle '" << b->first << "' : " << std::flush;

      }
      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      os << "o " << gkg::StringConverter::toString( b->first )
         << std::endl;
      os << std::endl;

      fiberCount = bundle.getCurve3dCount();
      fiberIndex = 0;

      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {


        if ( verbose )
        {

          if ( fiberIndex )
          {

            std::cout << gkg::Eraser( 27 ) << std::flush;

          }
          /*if ( ( fiberIndex == 0 ) ||
               ( fiberIndex == ( fiberCount - 1 ) ) ||
               ( ( fiberIndex + 1 ) % 100 == 0 ) )*/
          {

            std::cout << " fiber[ " << std::setw( 7 ) << fiberIndex + 1 
                      << " / " << std::setw( 7 ) << fiberCount
                      << " ]" << std::flush;

          }

        }

        const gkg::LightCurve3d< float >& fiber = *f;
        float length = fiber.getLength();

        int32_t newPointCount = ( int32_t )( length / step + 0.5 );

        gkg::LightCurve3d< float > 
          resampledFiber = fiber.getEquidistantCurve( newPointCount );

        std::vector< float > radii( newPointCount, radius );
        gkg::RCPointer< gkg::MeshMap< int32_t, float, 3U > >
          meshMap = gkg::MeshFactory::getInstance().getFiber( 
                                                            resampledFiber,
                                                            radii,
                                                            vertexCountPerPlane,
                                                            false );

        const std::list< gkg::Vector3d< float > >&
          sites = meshMap->vertices.getSites( 0 );

        std::list< gkg::Vector3d< float > >::const_iterator
          s = sites.begin(),
          se = sites.end();
        while ( s != se )
        {

          os << "v " << s->x << " " << s->y << " " << s->z << std::endl;
          ++ s;

        }

        os << std::endl;

        const std::list< gkg::Polygon< 3U > >& 
          polygons = meshMap->polygons.getPolygons( 0 );

        std::list< gkg::Polygon< 3U > >::const_iterator
          p = polygons.begin(),
          pe = polygons.end();
        while ( p != pe )
        {

          os << "f " << p->indices[ 0 ] + offsetVertices + 1 << " "
                     << p->indices[ 1 ] + offsetVertices + 1 << " "
                     << p->indices[ 2 ] + offsetVertices + 1 << std::endl;
                     
          ++ p;

        }

        os << std::endl;

        offsetVertices += ( uint32_t )sites.size();
        ++ fiberIndex;

        ++ f;

      }

      if ( verbose )
      {

        std::cout << std::endl;

      }

      ++ b;

    }

    os.close();

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapToObjAndMtlConverterProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapToObjAndMtlConverterCommand
//

gkg::DwiBundleMapToObjAndMtlConverterCommand::
                                        DwiBundleMapToObjAndMtlConverterCommand(
                                 int32_t argc,
                                 char* argv[],
                                 bool loadPlugin,
                                 bool removeFirst )
                                             : gkg::Command( argc, argv,
                                                             loadPlugin,
                                                             removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiBundleMapToObjAndMtlConverterCommand::"
             "DwiBundleMapToObjAndMtlConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapToObjAndMtlConverterCommand::
                                        DwiBundleMapToObjAndMtlConverterCommand(
                                           const std::string& fileNameBundleMap,
                                           const std::string& fileNameObj,
                                           const std::string& fileNameMtl,
                                           float step,
                                           float radius,
                                           int32_t vertexCountPerPlane,
                                           bool verbose )
                                             : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameObj,
             fileNameMtl,
             step,
             radius,
             vertexCountPerPlane,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapToObjAndMtlConverterCommand::"
             "DwiBundleMapToObjAndMtlConverterCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameObj, "
             "const std::string& fileNameMtl, "
             "float step, "
             "float radius, "
             "int32_t vertexCountPerPlane, "
             "bool verbose )" );

}


gkg::DwiBundleMapToObjAndMtlConverterCommand::
                                        DwiBundleMapToObjAndMtlConverterCommand(
                                             const gkg::Dictionary& parameters )
                                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameObj );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMtl );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, vertexCountPerPlane );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameObj,
             fileNameMtl,
             step,
             radius,
             vertexCountPerPlane,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapToObjAndMtlConverterCommand::"
             "DwiBundleMapToObjAndMtlConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapToObjAndMtlConverterCommand::
                                      ~DwiBundleMapToObjAndMtlConverterCommand()
{
}


std::string gkg::DwiBundleMapToObjAndMtlConverterCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapToObjAndMtlConverter";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapToObjAndMtlConverterCommand::getStaticName()" );

}


void gkg::DwiBundleMapToObjAndMtlConverterCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameObj;
    std::string fileNameMtl;
    float step = 0.1;
    float radius = 0.1;
    int32_t vertexCountPerPlane = 8;
    bool verbose = false;

 
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Converting bundle map to .obj/.mtl",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-o",
                                 "Output Wavefront .obj file name",
                                 fileNameObj );
    application.addSingleOption( "-om",
                                 "Output materials .mtl file name",
                                 fileNameMtl );
    application.addSingleOption( "-s",
                                 "step (default=0.1mm)",
                                 step,
				 true );
    application.addSingleOption( "-r",
                                 "radius (default=0.1mm)",
                                 radius,
				 true );
    application.addSingleOption( "-v",
                                 "Vertex count per plane (default=8)",
                                 vertexCountPerPlane,
				 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 

    execute( fileNameBundleMap,
             fileNameObj,
             fileNameMtl,
             step,
             radius,
             vertexCountPerPlane,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiBundleMapToObjAndMtlConverterCommand::parse()" );

}


void gkg::DwiBundleMapToObjAndMtlConverterCommand::execute(
                                           const std::string& fileNameBundleMap,
                                           const std::string& fileNameObj,
                                           const std::string& fileNameMtl,
                                           float step,
                                           float radius,
                                           int32_t vertexCountPerPlane,
                                           bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( step <= 0 )
    {

       throw std::runtime_error( "step must be strictly positive" );

    }

    if ( radius <= 0 )
    {

       throw std::runtime_error( "radius must be strictly positive" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapToObjAndMtlConverterProcess 
      bundleMapToObjAndMtlConverterProcess( fileNameObj,
                                            fileNameMtl,
                                            step,
                                            radius,
                                            vertexCountPerPlane,
                                            verbose );
    bundleMapToObjAndMtlConverterProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapToObjAndMtlConverterCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameObj, "
             "const std::string& fileNameMtl, "
             "float step, "
             "float radius, "
             "int32_t vertexCountPerPlane, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapToObjAndMtlConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameObj ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMtl ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_INTEGER_PARAMETER_HELP( vertexCountPerPlane ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
