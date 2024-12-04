#include <gkg-dmri-plugin-functors/DwiBundleMapHierarchy/DwiBundleMapHierarchyCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>


//
// class HierarchyProcess
//

class BundleMapHierarchyProcess : public gkg::Process
{

  public:

    BundleMapHierarchyProcess( const std::string& theFileNameOut,
                    				   const bool& theVerbose );

    const std::string& fileNameOut;
    const bool& verbose;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


BundleMapHierarchyProcess::BundleMapHierarchyProcess( 
                           const std::string& theFileNameOut,
				                   const bool& theVerbose )
                          : gkg::Process(),
                            fileNameOut( theFileNameOut ),
                            verbose( theVerbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapHierarchyProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapHierarchyProcess::operate< int16_t > );

}


template < class L >
void BundleMapHierarchyProcess::operate( gkg::Process& process,
                         					       const std::string& fileNameIn,
                         					       const gkg::AnalyzedObject&,
                         					       const std::string& )
{

  try
  {

    BundleMapHierarchyProcess&
      bundleMapHierarchyProcess =
        static_cast< BundleMapHierarchyProcess& >( process );

    const std::string&
      fileNameOut = bundleMapHierarchyProcess.fileNameOut;
    const bool& 
      verbose = bundleMapHierarchyProcess.verbose;

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
    int32_t bundleCount = bundleMap.getBundleCount();

    if ( verbose )
    {
     
      std::cout << "done " << std::endl;
     
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating and writing hierarchy file
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }

    gkg::RandomColorGenerator randomColorGenerator;

    std::ofstream os( fileNameOut.c_str() );

    os << "# tree 1.0" << std::endl;
    os << std::endl;

    os << "*BEGIN TREE hierarchy" << std::endl;
    os << "graph_syntax RoiArg" << std::endl;
    os << std::endl;


    os << "*BEGIN TREE fold_name" << std::endl;
    os << "name all" << std::endl;
    os << std::endl;

    int32_t colorIndex = 0;
    gkg::RGBComponent rgbComponent;
    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    while ( b != be )
    {

      rgbComponent = randomColorGenerator.getRandomRainbowColor( colorIndex,
                                                                 bundleCount );

      os << "*BEGIN TREE fold_name" << std::endl;
      os << "name " << b->first << std::endl;
      os << "color "
         << ( int32_t )rgbComponent.r << " "
         << ( int32_t )rgbComponent.g << " "
         << ( int32_t )rgbComponent.b << std::endl;
      os << "*END" << std::endl;
      os << std::endl;

      ++ b;
      ++ colorIndex;

    }

    os << "*END" << std::endl;
    os << std::endl;

    os << "*END" << std::endl;
    os << std::endl;

    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapHierarchyProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapHierarchyCommand
//

gkg::DwiBundleMapHierarchyCommand::DwiBundleMapHierarchyCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapHierarchyCommand::"
             "DwiBundleMapHierarchyCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapHierarchyCommand::DwiBundleMapHierarchyCommand(
                                           const std::string& fileNameBundleMap,
                                           const std::string& fileNameOut,
                                           bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( fileNameBundleMap,
             fileNameOut,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapHierarchyCommand::"
             "DwiBundleMapHierarchyCommand( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "bool verbose  )" );

}


gkg::DwiBundleMapHierarchyCommand::DwiBundleMapHierarchyCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMap,
             fileNameOut,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapHierarchyCommand::"
             "DwiBundleMapHierarchyCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapHierarchyCommand::~DwiBundleMapHierarchyCommand()
{
}


std::string gkg::DwiBundleMapHierarchyCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapHierarchy";

  }
  GKG_CATCH( "std::string gkg::DwiBundleMapHierarchyCommand::getStaticName()" );

}


void gkg::DwiBundleMapHierarchyCommand::parse()
{

  try
  {

    std::string fileNameBundleMap;
    std::string fileNameOut;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "creating hierarchy file from bundle map ",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-o",
                                 "Output hierarchy file name (.hie)",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 


    execute( fileNameBundleMap,
             fileNameOut,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapHierarchyCommand::parse()" );

}


void gkg::DwiBundleMapHierarchyCommand::execute(
                                           const std::string& fileNameBundleMap,
                                           const std::string& fileNameOut,
                                           bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapHierarchyProcess bundleMapHierarchyProcess( fileNameOut,
                                                         verbose );
    bundleMapHierarchyProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapHierarchyCommand::execute( "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator(  DwiBundleMapHierarchyCommand,
                         DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
                         DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                         DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
