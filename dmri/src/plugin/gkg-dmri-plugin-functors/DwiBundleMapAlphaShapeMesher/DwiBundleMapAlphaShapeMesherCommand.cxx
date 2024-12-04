#include <gkg-dmri-plugin-functors/DwiBundleMapAlphaShapeMesher/DwiBundleMapAlphaShapeMesherCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
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
// class BundleMapAlphaShapeMesherProcess
//

class BundleMapAlphaShapeMesherProcess : public gkg::Process
{

  public:

    BundleMapAlphaShapeMesherProcess(
                                 float step,
                                 const std::string& fileNameOut,
                                 const std::string& fileNamePointCloud,
                                 const std::string& fileNamePointCloudAtSurface,
                                 const std::string& outputFormat,
                                 bool singleFile,
                                 bool useThickFascicles,
                                 float fascicleRadius,
                                 int32_t sphereOrientationCount,
                                 bool removingOutliers,
                                 int32_t neighborCount,
                                 float outlierDistanceScale,
                                 bool simplifyingPointSet,
                                 bool smoothingPointSet,
                                 bool subdividingMesh,
                                 int32_t subdivisionIterationCount,
                                 const std::string& intermediateFileName,
                                 bool ascii,
                                 bool verbose );

    const float& getStep() const;
    const std::string& getFileNameOut() const;
    const std::string& getFileNamePointCloud() const;
    const std::string& getFileNamePointCloudAtSurface() const;
    const std::string& getOutputFormat() const;
    const bool& getSingleFile() const;
    const bool& useThickFascicles() const;
    const float& getFascicleRadius() const;
    const int32_t& getSphereOrientationCount() const;
    const bool& getRemovingOutliers() const;
    const int32_t& getNeighborCount() const;
    const float& getOutlierDistanceScale() const;
    const bool& getSimplifyingPointSet() const;
    const bool& getSmoothingPointSet() const;
    const bool& getSubdividingMesh() const;
    const int32_t& getSubdivisionIterationCount() const;
    const std::string& getIntermediateFileName() const;
    const bool& getAscii() const;
    const bool& getVerbose() const;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    float _step;
    std::string _fileNameOut;
    std::string _fileNamePointCloud;
    std::string _fileNamePointCloudAtSurface;
    std::string _outputFormat;
    bool _singleFile;
    bool _useThickFascicles;
    float _fascicleRadius;
    int32_t _sphereOrientationCount;
    bool _removingOutliers;
    int32_t _neighborCount;
    float _outlierDistanceScale;
    bool _simplifyingPointSet;
    bool _smoothingPointSet;
    bool _subdividingMesh;
    int32_t _subdivisionIterationCount;
    std::string _intermediateFileName;
    bool _ascii;
    bool _verbose;

};


BundleMapAlphaShapeMesherProcess::BundleMapAlphaShapeMesherProcess(
                                 float step,
	                         const std::string& fileNameOut,
                                 const std::string& fileNamePointCloud,
                                 const std::string& fileNamePointCloudAtSurface,
                                 const std::string& outputFormat,
                                 bool singleFile,
                                 bool useThickFascicles,
                                 float fascicleRadius,
                                 int32_t sphereOrientationCount,
                                 bool removingOutliers,
                                 int32_t neighborCount,
                                 float outlierDistanceScale,
                                 bool simplifyingPointSet,
                                 bool smoothingPointSet,
                                 bool subdividingMesh,
                                 int32_t subdivisionIterationCount,
                                 const std::string& intermediateFileName,
                                 bool ascii,
                                 bool verbose )
                                 : gkg::Process(),
                                   _step( step ),
                                   _fileNameOut( fileNameOut ),
                                   _fileNamePointCloud( fileNamePointCloud ),
                                   _fileNamePointCloudAtSurface(
                                                  fileNamePointCloudAtSurface ),
                                   _outputFormat( outputFormat ),
                                   _singleFile( singleFile ),
                                   _useThickFascicles( useThickFascicles ),
                                   _fascicleRadius( fascicleRadius ),
                                   _sphereOrientationCount(
                                                       sphereOrientationCount ),
                                   _removingOutliers( removingOutliers ),
                                   _neighborCount( neighborCount ),
                                   _outlierDistanceScale(
                                                         outlierDistanceScale ),
                                   _simplifyingPointSet( simplifyingPointSet ),
                                   _smoothingPointSet( smoothingPointSet ),
                                   _subdividingMesh( subdividingMesh ),
                                   _subdivisionIterationCount(
                                                    subdivisionIterationCount ),
                                   _intermediateFileName( 
                                                         intermediateFileName ),
                                   _ascii( ascii ),
                                   _verbose( verbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapAlphaShapeMesherProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapAlphaShapeMesherProcess::operate< int16_t > );

}


const float& 
BundleMapAlphaShapeMesherProcess::getStep() const
{

  return _step;

}


const std::string& 
BundleMapAlphaShapeMesherProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& 
BundleMapAlphaShapeMesherProcess::getFileNamePointCloud() const
{

  return _fileNamePointCloud;

}


const std::string& 
BundleMapAlphaShapeMesherProcess::getFileNamePointCloudAtSurface() const
{

  return _fileNamePointCloudAtSurface;

}


const std::string& 
BundleMapAlphaShapeMesherProcess::getOutputFormat() const
{

  return _outputFormat;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getSingleFile() const
{

  return _singleFile;

}


const bool& 
BundleMapAlphaShapeMesherProcess::useThickFascicles() const
{

  return _useThickFascicles;

}


const float& 
BundleMapAlphaShapeMesherProcess::getFascicleRadius() const
{

  return _fascicleRadius;

}


const int32_t& 
BundleMapAlphaShapeMesherProcess::getSphereOrientationCount() const
{

  return _sphereOrientationCount;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getRemovingOutliers() const
{

  return _removingOutliers;

}


const int32_t& 
BundleMapAlphaShapeMesherProcess::getNeighborCount() const
{

  return _neighborCount;

}


const float& 
BundleMapAlphaShapeMesherProcess::getOutlierDistanceScale() const
{

  return _outlierDistanceScale;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getSimplifyingPointSet() const
{

  return _simplifyingPointSet;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getSmoothingPointSet() const
{

  return _smoothingPointSet;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getSubdividingMesh() const
{

  return _subdividingMesh;

}


const int32_t& 
BundleMapAlphaShapeMesherProcess::getSubdivisionIterationCount() const
{

  return _subdivisionIterationCount;

}


const std::string&
BundleMapAlphaShapeMesherProcess:: getIntermediateFileName() const
{

  return _intermediateFileName;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getAscii() const
{

  return _ascii;

}


const bool& 
BundleMapAlphaShapeMesherProcess::getVerbose() const
{

  return _verbose;

}



template < class L >
void BundleMapAlphaShapeMesherProcess::operate( gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    BundleMapAlphaShapeMesherProcess&
      bundleMapAlphaShapeMesherProcess =
        static_cast< BundleMapAlphaShapeMesherProcess& >( process );

    const float& step =
      bundleMapAlphaShapeMesherProcess.getStep();
    const std::string& fileNameOut =
      bundleMapAlphaShapeMesherProcess.getFileNameOut();
    const std::string& fileNamePointCloud = 
      bundleMapAlphaShapeMesherProcess.getFileNamePointCloud();
    const std::string& fileNamePointCloudAtSurface =
      bundleMapAlphaShapeMesherProcess.getFileNamePointCloudAtSurface();
    const std::string& outputFormat =
      bundleMapAlphaShapeMesherProcess.getOutputFormat();
    const bool& singleFile = 
      bundleMapAlphaShapeMesherProcess.getSingleFile();
    const bool& useThickFascicles =
      bundleMapAlphaShapeMesherProcess.useThickFascicles();
    const float& fascicleRadius =
      bundleMapAlphaShapeMesherProcess.getFascicleRadius();
    const int32_t& sphereOrientationCount =
      bundleMapAlphaShapeMesherProcess.getSphereOrientationCount();
    const bool& removingOutliers =
      bundleMapAlphaShapeMesherProcess.getRemovingOutliers();
    const int32_t& neighborCount =
      bundleMapAlphaShapeMesherProcess.getNeighborCount();
    const float& outlierDistanceScale =
      bundleMapAlphaShapeMesherProcess.getOutlierDistanceScale();
    const bool& simplifyingPointSet =
      bundleMapAlphaShapeMesherProcess.getSimplifyingPointSet();
    const bool& smoothingPointSet =
      bundleMapAlphaShapeMesherProcess.getSmoothingPointSet();
    const bool& subdividingMesh =
      bundleMapAlphaShapeMesherProcess.getSubdividingMesh();
    const int32_t& subdivisionIterationCount =
      bundleMapAlphaShapeMesherProcess.getSubdivisionIterationCount();
    const std::string& intermediateFileName =
      bundleMapAlphaShapeMesherProcess.getIntermediateFileName();
    const bool& ascii =
      bundleMapAlphaShapeMesherProcess.getAscii();
    const bool& verbose =
      bundleMapAlphaShapeMesherProcess.getVerbose();


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
    if ( useThickFascicles )
    {

      if ( verbose )
      {
     
        std::cout << "using thick fascicles : "
                  << std::flush;
     
      }

      int32_t currentOrientationCount = 0;
      float radius = 0.0;
      float step = bundleMapAlphaShapeMesherProcess.getStep();
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

      if ( verbose )
      {
     
        std::cout << sphereDistributedPoints.size() << " points"
                  << std::endl;
     
      }

    }
  

    ////////////////////////////////////////////////////////////////////////////
    // creating point cloud mesher
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {
    
      std::cout << "creating point cloud alpha shape mesher : "
                << std::flush;
    
    }

    gkg::PointCloudAlphaShapeMesher pointCloudAlphaShapeMesher;

    if ( verbose )
    {
    
      std::cout << "done"
                << std::endl;
    
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
      // computing alpha shape mesh of current bundle
      //////////////////////////////////////////////////////////////////////////

      if ( verbose)
      {

        std::cout << "=========================================================" 
                  << std::endl;
        std::cout << "processing bundle '" << b->first << std::endl;
        std::cout << "---------------------------------------------------------" 
                  << std::endl;

      }

      gkg::MeshMap< int32_t, float, 3U > alphaShapeMeshMap;
      std::list< gkg::Vector3d< float > > points;

      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();
      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        
        float length = fiber.getLength();

        int32_t newPointCount = ( int32_t )( length / step + 0.5 );

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

      pointCloudAlphaShapeMesher.computeAlphaShapeMesh(
                                                      pointVector,
                                                      alphaShapeMeshMap,
                                                      0,
                                                      removingOutliers,
                                                      neighborCount,
                                                      outlierDistanceScale,
                                                      simplifyingPointSet,
                                                      smoothingPointSet,
                                                      subdividingMesh,
                                                      subdivisionIterationCount,
                                                      intermediateFileName,
                                                      verbose );

      //////////////////////////////////////////////////////////////////////////
      // writing bundle alpha shape mesh if not in single file mode
      //////////////////////////////////////////////////////////////////////////

      if ( !singleFile )
      {

        std::string 
          fileNameOutForBundle = fileNameOut + '_' +
                                 gkg::StringConverter::toString( b->first );
        if ( verbose )
        {

          std::cout << "writing '" << fileNameOutForBundle << "' : "
                    << std::flush;

        }

        gkg::Writer::getInstance().write( fileNameOutForBundle,
                                          alphaShapeMeshMap,
                                          ascii,
                                          outputFormat );

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        meshAccumulator->add( alphaShapeMeshMap );

      }

      //////////////////////////////////////////////////////////////////////////
      // writing bundle point cloud files if asked
      //////////////////////////////////////////////////////////////////////////

      if ( !fileNamePointCloud.empty() )
      {


        // saving all points in a .pointcloud file
        std::string fileNamePointCloudForBundle =
                                   fileNamePointCloud + '_' +
                                   gkg::StringConverter::toString( b->first ) + 
                                   ".pointcloud";
        if ( verbose )
        {

          std::cout << "writing '" << fileNamePointCloudForBundle 
                    << "' : " << std::flush;

        }

        std::ofstream os( fileNamePointCloudForBundle.c_str() );

        std::vector< gkg::Vector3d< float > >::const_iterator
          p = pointVector.begin(),
          pe = pointVector.end();
        while ( p != pe )
        {

          os << p->x << " " << p->y << " " << p->z << std::endl;
          ++ p;

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // writing bundle point cloud at surface files if asked
      //////////////////////////////////////////////////////////////////////////

      if ( !fileNamePointCloudAtSurface.empty() )
      {

        // saving vertices of the bundle alpha shape mesh in a .pointcloud file
        std::string fileNameSurfacePointCloudAtSurfaceForBundle =
                                    fileNamePointCloudAtSurface + '_' +
                                    gkg::StringConverter::toString( b->first ) +
                                    ".pointcloud";
        if ( verbose )
        {

          std::cout << "writing '"
                    << fileNameSurfacePointCloudAtSurfaceForBundle
                    << "' : " << std::flush;

        }

        std::ofstream os( fileNameSurfacePointCloudAtSurfaceForBundle.c_str() );

        const std::list< gkg::Vector3d< float > >& 
          vertexList = alphaShapeMeshMap.vertices.getSites( 0 );
        std::list< gkg::Vector3d< float > >::const_iterator
          v = vertexList.begin(),
          ve = vertexList.end();
        while ( v != ve )
        {

          os << v->x << " " << v->y << " " << v->z << std::endl;
          ++ v;

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ bundleIndex;
      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // writing output alpha shape mesh
    //////////////////////////////////////////////////////////////////////////

    if ( singleFile )
    {

      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut
                  << "' : " << std::flush;

      }

      gkg::MeshMap< int32_t, float, 3U > allAlphaShapeMeshMap;
      allAlphaShapeMeshMap.add( *meshAccumulator );

      gkg::Writer::getInstance().write( fileNameOut,
                                        allAlphaShapeMeshMap,
                                        ascii,
                                        outputFormat );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapAlphaShapeMesherProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapAlphaShapeMesherCommand
//

gkg::DwiBundleMapAlphaShapeMesherCommand::DwiBundleMapAlphaShapeMesherCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapAlphaShapeMesherCommand::"
             "DwiBundleMapAlphaShapeMesherCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapAlphaShapeMesherCommand::DwiBundleMapAlphaShapeMesherCommand(
                                 const std::string& fileNameBundleMap,
                                 const std::string& fileNameOut,
                                 const std::string& fileNamePointCloud,
                                 const std::string& fileNamePointCloudAtSurface,
                                 float step,
                                 const std::string& outputFormat,
                                 bool singleFile,
                                 const bool& useThickFascicles,
                                 const float& fascicleRadius,
                                 const int32_t& sphereOrientationCount,
                                 bool removingOutliers,
                                 int32_t neighborCount,
                                 float outlierDistanceScale,
                                 bool simplifyingPointSet,
                                 bool smoothingPointSet,
                                 bool subdividingMesh,
                                 int32_t subdivisionIterationCount,
                                 const std::string& intermediateFileName,
                                 bool ascii,
                                 bool verbose )
                                         : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameOut,
             fileNamePointCloud,
             fileNamePointCloudAtSurface,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             removingOutliers,
             neighborCount,
             outlierDistanceScale,
             simplifyingPointSet,
             smoothingPointSet,
             subdividingMesh,
             subdivisionIterationCount,
             intermediateFileName,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapAlphaShapeMesherCommand::"
             "DwiBundleMapAlphaShapeMesherCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "const std::string& fileNamePointCloud, "
             "const std::string& fileNamePointCloudAtSurface, "
             "float step, "
             "const std::string& outputFormat, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool removingOutliers, "
             "int32_t neighborCount, "
             "float outlierDistanceScale, "
             "bool simplifyingPointSet, "
             "bool smoothingPointSet, "
             "bool subdividingMesh, "
             "int32_t subdivisionIterationCount, "
             "const std::string& intermediateFileName, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiBundleMapAlphaShapeMesherCommand::DwiBundleMapAlphaShapeMesherCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNamePointCloud );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNamePointCloudAtSurface );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, singleFile );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, useThickFascicles );
    DECLARE_FLOATING_PARAMETER( parameters, float, fascicleRadius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sphereOrientationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, removingOutliers );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, neighborCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, outlierDistanceScale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, simplifyingPointSet );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, smoothingPointSet );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, subdividingMesh );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, subdivisionIterationCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, intermediateFileName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameOut,
             fileNamePointCloud,
             fileNamePointCloudAtSurface,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             removingOutliers,
             neighborCount,
             outlierDistanceScale,
             simplifyingPointSet,
             smoothingPointSet,
             subdividingMesh,
             subdivisionIterationCount,
             intermediateFileName,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapAlphaShapeMesherCommand::"
             "DwiBundleMapAlphaShapeMesherCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapAlphaShapeMesherCommand::~DwiBundleMapAlphaShapeMesherCommand()
{
}


std::string gkg::DwiBundleMapAlphaShapeMesherCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapAlphaShapeMesher";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapAlphaShapeMesherCommand::getStaticName()" );

}


void gkg::DwiBundleMapAlphaShapeMesherCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameOut;
    std::string fileNamePointCloud;
    std::string fileNamePointCloudAtSurface;
    float step = 0.1;
    std::string outputFormat;
    bool singleFile = false;
    bool useThickFascicles = false;
    float fascicleRadius = 0.5;
    int32_t sphereOrientationCount = 100;
    bool removingOutliers = true;
    int32_t neighborCount = 24;
    float outlierDistanceScale = 2.0f;
    bool simplifyingPointSet = true;
    bool smoothingPointSet = true;
    bool subdividingMesh = true;
    int32_t subdivisionIterationCount = 1;
    std::string intermediateFileName = "";
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application(
                                 _argc, _argv,
                                 "computing convex alpha shape meshes of a "
                                 "bundle map",
                                 _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-s",
                                 "step ",
                                 step,
				 true );
    application.addSingleOption( "-o",
                                 "Output convex alpha shape meshes file name",
                                 fileNameOut );
    application.addSingleOption( "-p",
                                 "Output point cloud text file name",
                                 fileNamePointCloud,
                                 true );
    application.addSingleOption( "-ps",
                                 "Output point cloud at surface text file name",
                                 fileNamePointCloudAtSurface,
                                 true );
    application.addSingleOption( "-of",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-singleFile",
                                 "Create a simple mesh including all bundle "
                                 "alpha shape meshes instead of one output "
                                 "alpha shape mesh per bundle(default=false)",
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
    application.addSingleOption( "-removingOutliers",
                                 "Removing outliers using a threshold on "
                                 "distance between neighbors (default=true)",
                                 removingOutliers,
                                 true );
    application.addSingleOption( "-neighborCount",
                                 "Neighbor count to compute the distance "
                                 "threshold to remove outliers "
                                 "(default=24)",
                                 neighborCount,
                                 true );
    application.addSingleOption( "-outlierDistanceScale",
                                 "Scale of the average distance (=scale * "
                                 "average distance ) above which to remove a"
                                 "point (default=2.0)",
                                 outlierDistanceScale,
                                 true );
    application.addSingleOption( "-simplifyingPointSet",
                                 "Simplify point set (default=true)",
                                 simplifyingPointSet,
                                 true );
    application.addSingleOption( "-smoothingPointSet",
                                 "Smooth point set (default=true)",
                                 smoothingPointSet,
                                 true );
    application.addSingleOption( "-subdividingMesh",
                                 "Subdivide the computed mesh to get triangles"
                                 "of simular surface at the end (default=true)",
                                 subdividingMesh,
                                 true );
    application.addSingleOption( "-subdivisionIterationCount",
                                 "Subdivision iteration count (default=1)",
                                 subdivisionIterationCount,
                                 true );
    application.addSingleOption( "-intermediateFileName",
                                 "Saving intermediate files (default="", ie"
                                 "not saved)",
                                 intermediateFileName,
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
             fileNamePointCloud,
             fileNamePointCloudAtSurface,
             step,
             outputFormat,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             removingOutliers,
             neighborCount,
             outlierDistanceScale,
             simplifyingPointSet,
             smoothingPointSet,
             subdividingMesh,
             subdivisionIterationCount,
             intermediateFileName,
             ascii,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapAlphaShapeMesherCommand::parse()" );

}


void gkg::DwiBundleMapAlphaShapeMesherCommand::execute(
                                 const std::string& fileNameBundleMap,
                                 const std::string& fileNameOut,
                                 const std::string& fileNamePointCloud,
                                 const std::string& fileNamePointCloudAtSurface,
                                 float step,
                                 const std::string& outputFormat,
                                 bool singleFile,
                                 const bool& useThickFascicles,
                                 const float& fascicleRadius,
                                 const int32_t& sphereOrientationCount,
                                 bool removingOutliers,
                                 int32_t neighborCount,
                                 float outlierDistanceScale,
                                 bool simplifyingPointSet,
                                 bool smoothingPointSet,
                                 bool subdividingMesh,
                                 int32_t subdivisionIterationCount,
                                 const std::string& intermediateFileName,
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

    BundleMapAlphaShapeMesherProcess 
      bundleMapAlphaShapeMesherProcess( step,
                            fileNameOut,
                            fileNamePointCloud,
                            fileNamePointCloudAtSurface,
                            outputFormat,
                            singleFile,
                            useThickFascicles,
                            fascicleRadius,
                            sphereOrientationCount,
                            removingOutliers,
                            neighborCount,
                            outlierDistanceScale,
                            simplifyingPointSet,
                            smoothingPointSet,
                            subdividingMesh,
                            subdivisionIterationCount,
                            intermediateFileName,
                            ascii,
                            verbose );
    bundleMapAlphaShapeMesherProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapAlphaShapeMesherCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "const std::string& fileNamePointCloud, "
             "const std::string& fileNamePointCloudAtSurface, "
             "float step, "
             "const std::string& outputFormat, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool removingOutliers, "
             "int32_t neighborCount, "
             "float outlierDistanceScale, "
             "bool simplifyingPointSet, "
             "bool smoothingPointSet, "
             "bool subdividingMesh, "
             "int32_t subdivisionIterationCount, "
             "const std::string& intermediateFileName," 
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapAlphaShapeMesherCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePointCloud ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePointCloudAtSurface ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( singleFile ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( useThickFascicles ) +
    DECLARE_FLOATING_PARAMETER_HELP( fascicleRadius ) +
    DECLARE_INTEGER_PARAMETER_HELP( sphereOrientationCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( removingOutliers ) +
    DECLARE_INTEGER_PARAMETER_HELP( neighborCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( outlierDistanceScale ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( simplifyingPointSet ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( smoothingPointSet ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( subdividingMesh ) +
    DECLARE_INTEGER_PARAMETER_HELP( subdivisionIterationCount ) +
    DECLARE_STRING_PARAMETER_HELP( intermediateFileName ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
