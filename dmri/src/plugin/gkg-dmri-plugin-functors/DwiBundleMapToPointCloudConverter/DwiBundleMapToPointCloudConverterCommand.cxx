#include <gkg-dmri-plugin-functors/DwiBundleMapToPointCloudConverter/DwiBundleMapToPointCloudConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


//
// class BundleMapToPointCloudConverterProcessProcess
//

class BundleMapToPointCloudConverterProcess : public gkg::Process
{

  public:

    BundleMapToPointCloudConverterProcess(
                                 const std::string& theFileNameOut,
                                 float theStep,
                                 bool theSingleFile,
                                 const std::string& theColoringMode,
                                 const std::string& theFileNameColorInformation,
                                 bool theVerbose );

    std::string fileNameOut;
    float step;
    bool singleFile;
    const std::string& coloringMode;
    const std::string& fileNameColorInformation;
    bool verbose;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


BundleMapToPointCloudConverterProcess::BundleMapToPointCloudConverterProcess(
                                 const std::string& theFileNameOut,
                                 float theStep,
                                 bool theSingleFile,
                                 const std::string& theColoringMode,
                                 const std::string& theFileNameColorInformation,
                                 bool theVerbose )
                                      : gkg::Process(),
                                        fileNameOut( theFileNameOut ),
                                        step( theStep ),
                                        singleFile( theSingleFile ),
                                        coloringMode( theColoringMode ),
                                        fileNameColorInformation(
                                                  theFileNameColorInformation ),
                                        verbose( theVerbose )
{

  try
  {


    registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                     &BundleMapToPointCloudConverterProcess::operate<
                                                                std::string > );
    registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                     &BundleMapToPointCloudConverterProcess::operate<
                                                                int16_t > );

  }
  GKG_CATCH( "BundleMapToPointCloudConverterProcess::"
             "BundleMapToPointCloudConverterProcess( "
             "const std::string& theFileNameOut, "
             "float theStep, "
             "bool theSingleFile, "
             "const std::string& theColoringMode, "
             "const std::string& theFileNameColorInformation, "
             "bool theVerbose )" );

}


template < class L >
void BundleMapToPointCloudConverterProcess::operate(
                                                  gkg::Process& process,
                                                  const std::string& fileNameIn,
                                                  const gkg::AnalyzedObject&,
                                                  const std::string& )
{

  try
  {

    BundleMapToPointCloudConverterProcess&
      bundleMapToPointCloudConverterProcess =
        static_cast< BundleMapToPointCloudConverterProcess& >( process );

    const std::string& fileNameOut =
      bundleMapToPointCloudConverterProcess.fileNameOut;
    const float& step =
      bundleMapToPointCloudConverterProcess.step;
    const bool& singleFile = 
      bundleMapToPointCloudConverterProcess.singleFile;
    const std::string& coloringMode =
      bundleMapToPointCloudConverterProcess.coloringMode;
    const std::string& fileNameColorInformation =
      bundleMapToPointCloudConverterProcess.fileNameColorInformation;
    const bool& verbose =
      bundleMapToPointCloudConverterProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    std::map< L, gkg::Vector3d< float > > bundleLabelToColorLut;
    if ( coloringMode == "none" )
    {

      if ( !fileNameColorInformation.empty() )
      {

        throw std::runtime_error( 
                      "no color information file required when using "
                      "'none' coloring mode" );

      }

    }
    else if ( coloringMode == "fiber-direction" )
    {

      if ( !fileNameColorInformation.empty() )
      {

        throw std::runtime_error( 
                      "no color information file required when using "
                      "'fiber-direction' coloring mode" );

      }

    }
    else if ( coloringMode == "bundle-color" )
    {


      std::ifstream is( fileNameColorInformation.c_str() );
      L label;
      gkg::Vector3d< float > color;
      while ( !is.eof() )
      {

        is >> label >> color.x >> color.y >> color.z;
        bundleLabelToColorLut[ label ] = gkg::Vector3d< float >( color.x,
                                                                 color.y,
                                                                 color.z );

      }
      is.close();

    }
    else
    {

      throw std::runtime_error( "not a valid coloring mode; should be "
                                "'fiber-direction' or 'bundle-color'" );

    }

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
    // extracting point cloud for each fiber bundle
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< float > > allBundlePoints;
    std::list< gkg::Vector3d< float > > allBundleColors;

    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      if ( verbose)
      {

        std::cout << "=========================================================" 
                  << std::endl;
        std::cout << "processing bundle '" << b->first << std::endl;
        std::cout << "---------------------------------------------------------" 
                  << std::endl;

      }

      std::list< gkg::Vector3d< float > > currentBundlePoints;
      std::list< gkg::Vector3d< float > > currentBundleColors;

      gkg::Vector3d< float > color;

      if ( coloringMode == "bundle-color" )
      {

        typename std::map< L, gkg::Vector3d< float > >::const_iterator
          c = bundleLabelToColorLut.find( b->first );
        if ( c == bundleLabelToColorLut.end() )
        {

          throw std::runtime_error(
                    "bundle to color LUT inconsistent with bundle map labels" );

        }
        color = c->second;

      }


      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;


      if ( coloringMode == "none" )
      {

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
            p = resampledFiber.begin(),
            pe = resampledFiber.end();
          while ( p != pe )
          {


            currentBundlePoints.push_back( *p );
            ++ p;

          }

          ++ f;

        }

      }
      else if ( coloringMode == "fiber-direction" )
      {

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
            p = resampledFiber.begin(),
            pe = resampledFiber.end();
          gkg::LightCurve3d< float >::const_iterator
            pNext = p;
          ++ pNext;
          while ( p != pe )
          {


            currentBundlePoints.push_back( *p );

            if ( pNext == pe )
            {

              pNext = p;
              -- pNext;

            }
            color = ( *pNext - *p ).absoluteCoordinates();
            color.normalize();
            currentBundleColors.push_back( color );

            ++ pNext;
            ++ p;

          }

          ++ f;

        }

      }
      else if ( coloringMode == "bundle-color" )
      {

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
            p = resampledFiber.begin(),
            pe = resampledFiber.end();
          while ( p != pe )
          {


            currentBundlePoints.push_back( *p );
            currentBundleColors.push_back( color );

            ++ p;

          }

          ++ f;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // writing current bundle point cloud if not in single file mode
      //////////////////////////////////////////////////////////////////////////

      if ( !singleFile )
      {

        std::string 
          fileNameOutForBundle = fileNameOut + '_' +
                                 gkg::StringConverter::toString( b->first ) + 
                                 ".pointcloud";
        if ( verbose )
        {

          std::cout << "writing '" << fileNameOutForBundle << "' : "
                    << std::flush;

        }

        std::ofstream os( fileNameOutForBundle.c_str() );

        if ( coloringMode == "none" )
        {

          std::list< gkg::Vector3d< float > >::const_iterator
            p = currentBundlePoints.begin(),
            pe = currentBundlePoints.end();
           while ( p != pe )
          {

            os << p->x << " " << p->y << " " << p->z << std::endl;
            ++ p;

          }

        }
        else
        {

          std::list< gkg::Vector3d< float > >::const_iterator
            p = currentBundlePoints.begin(),
            pe = currentBundlePoints.end();
          std::list< gkg::Vector3d< float > >::const_iterator
            c = currentBundleColors.begin();
           while ( p != pe )
          {

            os << p->x << " " << p->y << " " << p->z << " "
               << c->x << " " << c->y << " " << c->z << std::endl;
            ++ c;
            ++ p;

          }

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        allBundlePoints.insert( allBundlePoints.end(),
                                currentBundlePoints.begin(),
                                currentBundlePoints.end() );
        allBundleColors.insert( allBundleColors.end(),
                                currentBundleColors.begin(),
                                currentBundleColors.end() );

      }

      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // writing writing bundle map point cloud if single file mode
    //////////////////////////////////////////////////////////////////////////

    if ( singleFile )
    {

      std::string 
        fileNameOutForAllBundles = fileNameOut + ".pointcloud";
      if ( verbose )
      {

        std::cout << "writing '" << fileNameOutForAllBundles << "' : "
                  << std::flush;

      }

      std::ofstream os( fileNameOutForAllBundles.c_str() );

      if ( coloringMode == "none" )
      {

        std::list< gkg::Vector3d< float > >::const_iterator
          p = allBundlePoints.begin(),
          pe = allBundlePoints.end();
        while ( p != pe )
        {

          os << p->x << " " << p->y << " " << p->z << std::endl;
          ++ p;

        }

      }
      else
      {

        std::list< gkg::Vector3d< float > >::const_iterator
          p = allBundlePoints.begin(),
          pe = allBundlePoints.end();
        std::list< gkg::Vector3d< float > >::const_iterator
          c = allBundleColors.begin();
         while ( p != pe )
        {

          os << p->x << " " << p->y << " " << p->z << " "
             << c->x << " " << c->y << " " << c->z << std::endl;
          ++ c;
          ++ p;

        }

      }

      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapToPointCloudConverterProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapToPointCloudConverterCommand
//

gkg::DwiBundleMapToPointCloudConverterCommand::
                                       DwiBundleMapToPointCloudConverterCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapToPointCloudConverterCommand::"
             "DwiBundleMapToPointCloudConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapToPointCloudConverterCommand::
                                       DwiBundleMapToPointCloudConverterCommand(
                                    const std::string& fileNameBundleMap,
                                    const std::string& fileNameOut,
                                    float step,
                                    bool singleFile,
                                    const std::string& coloringMode,
                                    const std::string& fileNameColorInformation,
                                    bool verbose )
                                              : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameOut,
             step,
             singleFile,
             coloringMode,
             fileNameColorInformation,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapToPointCloudConverterCommand::"
             "DwiBundleMapToPointCloudConverterCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "bool singleFile, "
             "const std::string& coloringMode, "
             "const std::string& fileNameColorInformation, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiBundleMapToPointCloudConverterCommand::
                                       DwiBundleMapToPointCloudConverterCommand(
                                             const gkg::Dictionary& parameters )
                                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, singleFile );
    DECLARE_STRING_PARAMETER( parameters, std::string, coloringMode );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameColorInformation );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameOut,
             step,
             singleFile,
             coloringMode,
             fileNameColorInformation,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapToPointCloudConverterCommand::"
             "DwiBundleMapToPointCloudConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapToPointCloudConverterCommand::
                                     ~DwiBundleMapToPointCloudConverterCommand()
{
}


std::string gkg::DwiBundleMapToPointCloudConverterCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapToPointCloudConverter";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapToPointCloudConverterCommand::getStaticName()" );

}


void gkg::DwiBundleMapToPointCloudConverterCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameOut;
    float step = 0.1;
    bool singleFile = false;
    std::string coloringMode = "none";
    std::string fileNameColorInformation = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "converting bundle map to point cloud "
                                  "representation",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-o",
                                 "Output point cloud text file name",
                                 fileNameOut );
    application.addSingleOption( "-s",
                                 "step ",
                                 step,
				 true );
    application.addSingleOption( "-singleFile",
                                 "Create a simple point cloud including all "
                                 "bundles or instead one output point cloud "
                                 "per bundle (default=false)",
                                 singleFile,
                                 true );
    application.addSingleOption( "-coloringMode",
                                 "Color mode being one of:\n"
                                 "- none\n"
                                 "- fiber-direction\n"
                                 "- bundle-color\n"
                                  " (default=none)",
                                 coloringMode,
                                 true );
    application.addSingleOption( "-fileNameColorInformation",
                                 "Text file including the bundle label to "
                                 "color correspondance, being of text type:\n"
                                 "<label1>  <red1> < green1> <blue1> \n"
                                 "...\n"
                                 "<labelN>  <redN> < greenN> <blueN>",
                                 fileNameColorInformation,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 


    execute( fileNameBundleMap,
             fileNameOut,
             step,
             singleFile,
             coloringMode,
             fileNameColorInformation,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapToPointCloudConverterCommand::"
                     "parse()" );

}


void gkg::DwiBundleMapToPointCloudConverterCommand::execute(
                                 const std::string& fileNameBundleMap,
                                 const std::string& fileNameOut,
                                 float step,
                                 bool singleFile,
                                 const std::string& coloringMode,
                                 const std::string& fileNameColorInformation,
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

    BundleMapToPointCloudConverterProcess 
      bundleMapToPointCloudConverterProcess( fileNameOut,
                                             step,
                                             singleFile,
                                             coloringMode,
                                             fileNameColorInformation,
                                             verbose );
    bundleMapToPointCloudConverterProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapToPointCloudConverterCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "bool singleFile, "
             "const std::string& coloringMode, "
             "const std::string& fileNameColorInformation, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapToPointCloudConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( singleFile ) +
    DECLARE_STRING_PARAMETER_HELP( coloringMode ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameColorInformation ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
