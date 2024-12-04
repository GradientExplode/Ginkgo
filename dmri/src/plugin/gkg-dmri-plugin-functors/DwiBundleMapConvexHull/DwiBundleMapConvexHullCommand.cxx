#include <gkg-dmri-plugin-functors/DwiBundleMapConvexHull/DwiBundleMapConvexHullCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-mesh/QuickConvexHull_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>


//
// class BundleMapConvexHullProcess
//

class BundleMapConvexHullProcess : public gkg::Process
{

  public:

    BundleMapConvexHullProcess( const float& step,
                                const std::string& fileNameOut,
                                const std::string& outputFormat,
                                const bool& singleFile,
                                const bool& useThickFascicles,
                                const float& fascicleRadius,
                                const int32_t& sphereOrientationCount,
                                const bool& ascii,
                                const bool& verbose );

    const float& getStep() const;
    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    const bool& getSingleFile() const;
    const bool& useThickFascicles() const;
    const float& getFascicleRadius() const;
    const int32_t& getSphereOrientationCount() const;
    const bool& getAscii() const;
    const bool& getVerbose() const;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const float& _step;
    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    const bool& _singleFile;
    const bool& _useThickFascicles;
    const float& _fascicleRadius;
    const int32_t& _sphereOrientationCount;
    const bool& _ascii;
    const bool& _verbose;

};


BundleMapConvexHullProcess::BundleMapConvexHullProcess(
				    const float& step,
				    const std::string& fileNameOut,
				    const std::string& outputFormat,
				    const bool& singleFile,
                                    const bool& useThickFascicles,
                                    const float& fascicleRadius,
                                    const int32_t& sphereOrientationCount,
				    const bool& ascii,
				    const bool& verbose )
                            : gkg::Process(),
                              _step( step ),
                              _fileNameOut( fileNameOut ),
                              _outputFormat( outputFormat ),
                              _singleFile( singleFile ),
                              _useThickFascicles( useThickFascicles ),
                              _fascicleRadius( fascicleRadius ),
                              _sphereOrientationCount( sphereOrientationCount ),
                              _ascii( ascii ),
                              _verbose( verbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapConvexHullProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapConvexHullProcess::operate< int16_t > );

}


const float& 
BundleMapConvexHullProcess::getStep() const
{

  return _step;

}


const std::string& 
BundleMapConvexHullProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& 
BundleMapConvexHullProcess::getOutputFormat() const
{

  return _outputFormat;

}


const bool& 
BundleMapConvexHullProcess::getSingleFile() const
{

  return _singleFile;

}


const bool& 
BundleMapConvexHullProcess::useThickFascicles() const
{

  return _useThickFascicles;

}


const float& 
BundleMapConvexHullProcess::getFascicleRadius() const
{

  return _fascicleRadius;

}


const int32_t& 
BundleMapConvexHullProcess::getSphereOrientationCount() const
{

  return _sphereOrientationCount;

}


const bool& 
BundleMapConvexHullProcess::getAscii() const
{

  return _ascii;

}


const bool& 
BundleMapConvexHullProcess::getVerbose() const
{

  return _verbose;

}



template < class L >
void BundleMapConvexHullProcess::operate( gkg::Process& process,
 					   const std::string& fileNameIn,
 					   const gkg::AnalyzedObject&,
 					   const std::string& )
{

  try
  {

    BundleMapConvexHullProcess&
      bundleMapConvexHullProcess =
        static_cast< BundleMapConvexHullProcess& >( process );

    bool singleFile = bundleMapConvexHullProcess.getSingleFile();


    ////////////////////////////////////////////////////////////////////////////
    // reading input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( bundleMapConvexHullProcess.getVerbose() )
    {
     
      std::cout << "reading input '" << fileNameIn << "' bundle map : "
                << std::flush;
     
    }
    gkg::BundleMap< L > bundleMap;
    gkg::Reader::getInstance().read( fileNameIn, bundleMap );

    if ( bundleMapConvexHullProcess.getVerbose() )
    {
     
      std::cout << "done " << std::endl;
     
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating and filling density mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::MeshAccumulator< int32_t, float, 3U > >
      meshAccumulator;

    if ( singleFile )
    {

      meshAccumulator.reset( 
                            new  gkg::MeshAccumulator< int32_t, float, 3U > );

    }

    ////////////////////////////////////////////////////////////////////////////
    // selecting fiber bundles
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< float > > sphereDistributedPoints;
    sphereDistributedPoints.push_back(
                                      gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );
    if ( bundleMapConvexHullProcess.useThickFascicles() )
    {

      float fascicleRadius = bundleMapConvexHullProcess.getFascicleRadius();
      int32_t sphereOrientationCount =
                        bundleMapConvexHullProcess.getSphereOrientationCount();
      int32_t currentOrientationCount = 0;
      float radius = 0.0;
      float step = bundleMapConvexHullProcess.getStep();
      for ( radius = step; radius <= fascicleRadius; radius += step )
      {

        currentOrientationCount =
                       std::max( 6,
                                 ( int32_t )( ( float )sphereOrientationCount *
                                              ( radius / fascicleRadius ) *
                                              ( radius / fascicleRadius ) ) );
        if ( currentOrientationCount % 2 )
        {

          ++ currentOrientationCount;

        }
        gkg::AntipodallySymmetricUniformOrientationSet
          antipodallySymmetricUniformOrientationSet( currentOrientationCount );

        gkg::AntipodallySymmetricUniformOrientationSet::const_iterator
          o = antipodallySymmetricUniformOrientationSet.begin(),
          oe = antipodallySymmetricUniformOrientationSet.end();
        while ( o != oe )
        {

          sphereDistributedPoints.push_back( *o * radius );
          ++ o;

        }

      }

    }
  

    ////////////////////////////////////////////////////////////////////////////
    // selecting fiber bundles
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > point;

    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    int32_t bundleIndex = 0;
    while ( b != be )
    {

      //////////////////////////////////////////////////////////////////////////
      // computing convex hull of current bundle
      //////////////////////////////////////////////////////////////////////////

      if ( bundleMapConvexHullProcess.getVerbose() )
      {

        std::cout << "processing bundle '" << b->first << "' : " << std::flush;

      }

      gkg::MeshMap< int32_t, float, 3U > convexHullMeshMap;
      std::list< gkg::Vector3d< float > > points;

      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                                fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        
        float length = fiber.getLength();

        int32_t newPointCount =
         ( int32_t )( length / bundleMapConvexHullProcess.getStep() + 0.5 );

        gkg::LightCurve3d< float > 
          resampledFiber = fiber.getEquidistantCurve( newPointCount );
        gkg::LightCurve3d< float >::const_iterator
          s = resampledFiber.begin(),
          se = resampledFiber.end();
        while ( s != se )
        {

          std::list< gkg::Vector3d< float > >::const_iterator
            o = sphereDistributedPoints.begin(),
            oe = sphereDistributedPoints.end();

          while ( o != oe )
          {

            point.x = s->x + o->x;
            point.y = s->y + o->y;
            point.z = s->z + o->z;

            points.push_back( point );

            ++ o;

          }

          ++ s;

        }

        ++ f;

      }
      std::vector< gkg::Vector3d< float > > 
        pointVector( points.begin(), points.end() );

      gkg::QuickConvexHull::getInstance().addConvexHull(
        pointVector,
        0,
        convexHullMeshMap );

      if ( bundleMapConvexHullProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // writing bundle convex hull if not in single file mode
      //////////////////////////////////////////////////////////////////////////

      if ( !singleFile )
      {

        std::string fileNameOut = bundleMapConvexHullProcess.getFileNameOut() +
                                  '_' +
                                  gkg::StringConverter::toString( b->first );
        if ( bundleMapConvexHullProcess.getVerbose() )
        {

          std::cout << "writing '" << fileNameOut << "' : " << std::flush;

        }

        gkg::Writer::getInstance().write(
 			     fileNameOut,
 			     convexHullMeshMap,
 			     bundleMapConvexHullProcess.getAscii(),
 			     bundleMapConvexHullProcess.getOutputFormat() );

        if ( bundleMapConvexHullProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        meshAccumulator->add( convexHullMeshMap );

      }

      ++ bundleIndex;
      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // writing output density mask
    //////////////////////////////////////////////////////////////////////////

    if ( singleFile )
    {

      if ( bundleMapConvexHullProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << bundleMapConvexHullProcess.getFileNameOut()
                  << "' : " << std::flush;

      }

      gkg::MeshMap< int32_t, float, 3U > allConvexHullMeshMap;
      allConvexHullMeshMap.add( *meshAccumulator );

      gkg::Writer::getInstance().write(
                               bundleMapConvexHullProcess.getFileNameOut(),
                               allConvexHullMeshMap,
                               bundleMapConvexHullProcess.getAscii(),
                               bundleMapConvexHullProcess.getOutputFormat() );

      if ( bundleMapConvexHullProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapConvexHullProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapConvexHullCommand
//

gkg::DwiBundleMapConvexHullCommand::DwiBundleMapConvexHullCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::DwiBundleMapConvexHullCommand::"
             "DwiBundleMapConvexHullCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapConvexHullCommand::DwiBundleMapConvexHullCommand(
                                          const std::string& fileNameBundleMap,
                                          const std::string& fileNameOut,
                                          float step,
                                          const std::string& outputFormat,
                                          bool singleFile,
                                          const bool& useThickFascicles,
                                          const float& fascicleRadius,
                                          const int32_t& sphereOrientationCount,
                                          bool ascii,
                                          bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapConvexHullCommand::"
             "DwiBundleMapConvexHullCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "const std::string& outputFormat, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiBundleMapConvexHullCommand::DwiBundleMapConvexHullCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, singleFile );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, useThickFascicles );
    DECLARE_FLOATING_PARAMETER( parameters, float, fascicleRadius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sphereOrientationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapConvexHullCommand::"
             "DwiBundleMapConvexHullCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapConvexHullCommand::~DwiBundleMapConvexHullCommand()
{
}


std::string gkg::DwiBundleMapConvexHullCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapConvexHull";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapConvexHullCommand::getStaticName()" );

}


void gkg::DwiBundleMapConvexHullCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameOut;
    float step = 0.1;
    std::string outputFormat;
    bool singleFile = false;
    bool useThickFascicles = false;
    float fascicleRadius = 0.5;
    int32_t sphereOrientationCount = 100;
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application(
                                 _argc, _argv,
                                 "computing convex hull meshes of a bundle map",
                                 _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-s",
                                 "step ",
                                 step,
				 true );
    application.addSingleOption( "-o",
                                 "Output convex hull meshes file name",
                                 fileNameOut );
    application.addSingleOption( "-of",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-singleFile",
                                 "Create a simple mesh including all convex "
                                 "hull meshes instead of one output convex hull"
                                 "mesh per bundle(default=false)",
                                 singleFile,
                                 true );
    application.addSingleOption( "-useThickFascicles",
                                 "Take into account the thickness of numerical "
                                 "fibers that correspond to faiscicles "
                                 "(default=false)",
                                 useThickFascicles,
                                 true );
    application.addSingleOption( "-fascicleRadius",
                                 "Fascicle radius (default=0.5mm)",
                                 fascicleRadius,
                                 true );
    application.addSingleOption( "-sphereOrientationCount",
                                 "Maximum sphere orientation count when taking "
                                 "into account fascicle thickness "
                                 "(default=100)",
                                 sphereOrientationCount,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 


    execute( fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapConvexHullCommand::parse()" );

}


void gkg::DwiBundleMapConvexHullCommand::execute(
                                          const std::string& fileNameBundleMap,
                                          const std::string& fileNameOut,
                                          float step,
                                          const std::string& outputFormat,
                                          bool singleFile,
                                          const bool& useThickFascicles,
                                          const float& fascicleRadius,
                                          const int32_t& sphereOrientationCount,
                                          bool ascii,
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

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapConvexHullProcess 
      bundleMapConvexHullProcess( step,
                                  fileNameOut,
                                  outputFormat,
                                  singleFile,
                                  useThickFascicles,
                                  fascicleRadius,
                                  sphereOrientationCount,
                                  ascii,
                                  verbose );
    bundleMapConvexHullProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapConvexHullCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "const std::string& outputFormat, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapConvexHullCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( singleFile ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( useThickFascicles ) +
    DECLARE_FLOATING_PARAMETER_HELP( fascicleRadius ) +
    DECLARE_INTEGER_PARAMETER_HELP( sphereOrientationCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
