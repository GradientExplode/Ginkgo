#include <gkg-dmri-plugin-functors/DwiBundleMapTransform3d/DwiBundleMapTransform3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
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
// class BundleMapTransform3dProcess
//

class BundleMapTransform3dProcess : public gkg::Process
{

  public:

    BundleMapTransform3dProcess( const std::string& theFileNameOut,
                                 const std::string& theFileNameTransform3d,
                                 const bool& theVerbose );

    const std::string& fileNameOut;
    const std::string& fileNameTransform3d;
    const bool& verbose;

  private:

    template < class L >
    static void transform( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& );


};


BundleMapTransform3dProcess::BundleMapTransform3dProcess(
                                      const std::string& theFileNameOut,
                                      const std::string& theFileNameTransform3d,
                                      const bool& theVerbose )
                            : gkg::Process( "BundleMap" ),
                             fileNameOut( theFileNameOut ),
                             fileNameTransform3d( theFileNameTransform3d ),
                             verbose( theVerbose )
{

  try
  {

    registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                     &BundleMapTransform3dProcess::transform< std::string > );
    registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                    &BundleMapTransform3dProcess::transform< int16_t > );

  }
  GKG_CATCH( "BundleMapTransform3dProcess::"
             "BundleMapTransform3dProcess( "
	     "const std::string& theFileNameOut, "
	     "const std::string& theFileNameTransform3d, "
	     "const bool& theVerbose )" );

}


template < class L >
void 
BundleMapTransform3dProcess::transform( gkg::Process& process,
 					const std::string& fileNameIn,
 					const gkg::AnalyzedObject&,
 					const std::string& )
{

  try
  {

    BundleMapTransform3dProcess&
      bundleMapTransform3dProcess =
        static_cast< BundleMapTransform3dProcess& >( process );

    const std::string& 
      fileNameOut = bundleMapTransform3dProcess.fileNameOut;
    const std::string& 
      fileNameTransform3d = bundleMapTransform3dProcess.fileNameTransform3d;
    const bool&
      verbose = bundleMapTransform3dProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "reading input '" << fileNameIn << "' bundle map : "
                << std::flush;
     
    }
    gkg::BundleMap< L > bundleMap;
    std::string format = gkg::Reader::getInstance().read( fileNameIn,
                                                          bundleMap );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    ////////////////////////////////////////////////////////////////////////////
    // reading 3D transform
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {
     
      std::cout << "reading '" << fileNameTransform3d
                << "' 3D transform : "
                << std::flush;
     
    }

    gkg::Transform3d< float >* transform3d = 0;
    try
    {

      gkg::RigidTransform3d< float >*
        rigidTransform3d = new gkg::RigidTransform3d< float >;
      std::ifstream is( fileNameTransform3d.c_str() );
      rigidTransform3d->readTrm( is );
      is.close();
      transform3d = rigidTransform3d;

    }
    catch ( std::exception& )
    {

      try
      {

        gkg::AffineWoShearingTransform3d< float >*
          affineWoShearingTransform3d =
            new gkg::AffineWoShearingTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
        affineWoShearingTransform3d->readTrm( is );
        is.close();
        transform3d = affineWoShearingTransform3d;

      }
      catch ( std::exception& )
      {

        gkg::AffineTransform3d< float >*
          affineTransform3d =
            new gkg::AffineTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3d = affineTransform3d;

      }

    }

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }


    //////////////////////////////////////////////////////////////////////////
    // transforming fiber bundles
    //////////////////////////////////////////////////////////////////////////


    int32_t fiberIndex = 0;
    int32_t fiberCount = 0;

    gkg::Vector3d< float > tp;

    typename gkg::BundleMap< L >::iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      if ( verbose )
      {

        std::cout << "transforming bundle '" 
                  << b->first << "' : " << std::flush;

      }
      typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      fiberCount = bundle.getCurve3dCount();
      fiberIndex = 0;

      gkg::Curve3dMap< float >::iterator f = bundle.begin(),
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

        gkg::LightCurve3d< float >& fiber = *f;
        gkg::LightCurve3d< float >::iterator p = fiber.begin(),
                                             pe = fiber.end();
        while ( p != pe )
        {

          transform3d->getDirect( *p, tp );
          *p = tp;
          ++ p;

        }

        ++ fiberIndex;
        ++ f;

      }

      if ( verbose )
      {

        std::cout << std::endl;

      }

      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // deleting 3D transform 
    //////////////////////////////////////////////////////////////////////////

    if ( transform3d )
    {

      delete transform3d;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing transformed bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
     
      std::cout << "writing output '" << fileNameOut << "' bundle map : "
                << std::flush;
     
    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      bundleMap,
                                      false,
                                      format );

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }




  }
  GKG_CATCH( "template < class L > "
             "void BundleMapTransform3dProcess::transform( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapTransform3dCommand
//

gkg::DwiBundleMapTransform3dCommand::DwiBundleMapTransform3dCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapTransform3dCommand::"
             "DwiBundleMapTransform3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapTransform3dCommand::DwiBundleMapTransform3dCommand(
                                         const std::string& fileNameBundleMap,
                                         const std::string& fileNameOut,
                                         const std::string& fileNameTransform3d,
                                         bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameOut,
             fileNameTransform3d,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapTransform3dCommand::"
             "DwiBundleMapTransform3dCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameTransform3d, "
             "bool verbose )" );

}


gkg::DwiBundleMapTransform3dCommand::DwiBundleMapTransform3dCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransform3d );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameOut,
             fileNameTransform3d,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapTransform3dCommand::"
             "DwiBundleMapTransform3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapTransform3dCommand::~DwiBundleMapTransform3dCommand()
{
}


std::string gkg::DwiBundleMapTransform3dCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapTransform3d";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapTransform3dCommand::getStaticName()" );

}


void gkg::DwiBundleMapTransform3dCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameOut;
    std::string fileNameTransform3d;
    bool verbose = false;

 
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "Bundle map 3D transformer",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-o",
                                 "Output bundle map",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "3D transform *.trm filename",
                                 fileNameTransform3d );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 

    execute( fileNameBundleMap,
             fileNameOut,
             fileNameTransform3d,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapTransform3dCommand::parse()" );

}


void gkg::DwiBundleMapTransform3dCommand::execute(
                                         const std::string& fileNameBundleMap,
                                         const std::string& fileNameOut,
                                         const std::string& fileNameTransform3d,
                                         bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapTransform3dProcess 
      bundleMapTransform3dProcess( fileNameOut,
                                   fileNameTransform3d,
                                   verbose );
    bundleMapTransform3dProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapTransform3dCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameTransform3d, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiBundleMapTransform3dCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTransform3d ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
