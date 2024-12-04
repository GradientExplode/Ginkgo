#include <gkg-processing-plugin-functors/TextureMapThresholder/TextureMapThresholderCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <list>
#include <vector>


//
//   TextureMapThresholderProcess
//


class TextureMapThresholderProcess : public gkg::Process
{

  public:

    TextureMapThresholderProcess( const std::string& theFileNameInputMesh,
                        	  const std::string& theFileNameOutputTexture,
                        	  const std::string& theFileNameOutputSurface,
                        	  const std::string& theMode,
                        	  const double& theThreshold1,
                        	  const double& theThreshold2,
                        	  const double& theBackground,
                        	  const bool& theAscii,
                        	  const std::string& theFormat,
                        	  const bool& theVerbose );

    const std::string& fileNameInputMesh;
    const std::string& fileNameOutputTexture;
    const std::string& fileNameOutputSurface;
    const std::string& mode;
    const double& threshold1;
    const double& threshold2;
    const double& background;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void threshold( gkg::Process& process,
                           const std::string& fileNameInputTexture,
                           const gkg::AnalyzedObject&,
                           const std::string& );

    template < class T >
    static std::list< float > getSurfaceAreas( 
                              const gkg::MeshMap< int32_t, float, 3 >& meshMap,
                              const gkg::TextureMap< T >& thresholdedTextureMap,
                              const T& background );

};


TextureMapThresholderProcess::TextureMapThresholderProcess(
                                    const std::string& theFileNameInputMesh,
                                    const std::string& theFileNameOutputTexture,
                                    const std::string& theFileNameOutputSurface,
                                    const std::string& theMode,
                                    const double& theThreshold1,
                                    const double& theThreshold2,
                                    const double& theBackground,
                                    const bool& theAscii,
                                    const std::string& theFormat,
                                    const bool& theVerbose )
                   	    : gkg::Process( "TextureMap" ),
                   	      fileNameInputMesh( theFileNameInputMesh ),
                   	      fileNameOutputTexture( theFileNameOutputTexture ),
                   	      fileNameOutputSurface( theFileNameOutputSurface ),
                   	      mode( theMode ),
                   	      threshold1( theThreshold1 ),
                   	      threshold2( theThreshold2 ),
                   	      background( theBackground ),
                   	      ascii( theAscii ),
                   	      format( theFormat ),
                   	      verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &TextureMapThresholderProcess::threshold< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &TextureMapThresholderProcess::threshold< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &TextureMapThresholderProcess::threshold< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &TextureMapThresholderProcess::threshold< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &TextureMapThresholderProcess::threshold< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &TextureMapThresholderProcess::threshold< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &TextureMapThresholderProcess::threshold< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &TextureMapThresholderProcess::threshold< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &TextureMapThresholderProcess::threshold< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &TextureMapThresholderProcess::threshold< double > );

  }
  GKG_CATCH( "TextureMapThresholderProcess::TextureMapThresholderProcess( "
             "const std::string& theFileNameInputMesh, "
             "const std::string& theFileNameOutputTexture, "
             "const std::string& theFileNameOutputSurface, "
             "const std::string& theMode, "
             "const double& theThreshold1, "
             "const double& theThreshold2, "
             "const double& theBackground, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void TextureMapThresholderProcess::threshold(
                                        gkg::Process& process,
                                        const std::string& fileNameInputTexture,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    TextureMapThresholderProcess&
      thresholderProcess = static_cast< TextureMapThresholderProcess& >(
                                                                      process );

    const std::string& fileNameInputMesh =
     thresholderProcess.fileNameInputMesh;
    const std::string& fileNameOutputTexture =
     thresholderProcess.fileNameOutputTexture;
    const std::string& fileNameOutputSurface =
     thresholderProcess.fileNameOutputSurface;
    const std::string& mode =
     thresholderProcess.mode;
    const double& threshold1 =
     thresholderProcess.threshold1;
    const double& threshold2 =
     thresholderProcess.threshold2;
    const double& background =
     thresholderProcess.background;
    const bool& ascii =
     thresholderProcess.ascii;
    const std::string& format =
     thresholderProcess.format;
    const bool& verbose =
     thresholderProcess.verbose;

    ////////////////////////////////////////////////////////////////////////////
    // reading input texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading texture map '" << fileNameInputTexture << "' : "
                << std::flush;

    }
    gkg::TextureMap< T > inputTextureMap;
    gkg::Reader::getInstance().read( fileNameInputTexture,
                                     inputTextureMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input mesh map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading input mesh map '" << fileNameInputMesh << "' : "
                << std::flush;

    }

    gkg::MeshMap< int32_t, float, 3 > meshMap;
    gkg::Reader::getInstance().read( fileNameInputMesh, meshMap );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    int32_t meshRankCount = meshMap.vertices.getRankCount();
    if ( meshRankCount != 1 )
    {

      throw std::runtime_error(
                              "only mesh maps with a single rank are allowed" );

    }

    int32_t rank = meshMap.vertices.getRank( 0 );
    int32_t vertexCount = meshMap.vertices.getSiteCount( rank );

    if ( inputTextureMap.getCount() % vertexCount )
    {

      throw std::runtime_error( "input texture map item count must be a "
                                "multiple of the mesh map vertex count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // thresholding texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "thresholding texture map : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                                              mode,
	                                                      ( T )threshold1,
	                                                      ( T )threshold2 );
    
    gkg::TextureMap< T > thresholdedTextureMap;
    gkg::Thresholder< gkg::TextureMap< T > >
      thresholder( *testFunction, ( T )background );
    thresholder.threshold( inputTextureMap, thresholdedTextureMap );

    delete testFunction;

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing thresholded texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing thresholded texture map '"
                << fileNameOutputTexture << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTexture,
                                      thresholdedTextureMap,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing area of thresholded texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "computing surface area(s) : " << std::flush;

    }

    std::list< float > surfaceAreas = getSurfaceAreas( meshMap,
                                                       thresholdedTextureMap,
                                                       ( T )background );

    if ( verbose )
    {

      std::list< float >::const_iterator
        s = surfaceAreas.begin(),
        se = surfaceAreas.end();
      while ( s != se )
      {

        std::cout << *s << "mm2 " << std::flush;
        ++ s;

      }
      std::cout << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing area text file
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing surface area(s) text file '"
                << fileNameOutputSurface << "' : "
                << std::flush;

    }

    std::ofstream os( fileNameOutputSurface.c_str() );
    std::list< float >::const_iterator
      s = surfaceAreas.begin(),
      se = surfaceAreas.end();
    while ( s != se )
    {

      os << *s << " " << std::flush;
      ++ s;

    }
    os << std::endl;
    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void TextureMapThresholderProcess::threshold( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


template < class T >
std::list< float > TextureMapThresholderProcess::getSurfaceAreas( 
                              const gkg::MeshMap< int32_t, float, 3 >& meshMap,
                              const gkg::TextureMap< T >& thresholdedTextureMap,
                              const T& background )
{

  try
  {

    std::list< float > surfaceAreas;

    int32_t rank = meshMap.vertices.getRank( 0 );
    int32_t vertexCount = meshMap.vertices.getSiteCount( rank );
    const std::list< gkg::Vector3d< float > >&
      sites = meshMap.vertices.getSites( rank );

    std::vector< gkg::Vector3d< float > > vectorOfSites( sites.begin(),
                                                         sites.end() );
    const std::list< gkg::Polygon< 3U > >& 
      polygons = meshMap.polygons.getPolygons( 0 );

    int32_t textureItemCount = thresholdedTextureMap.getCount();

    int32_t textureRankCount = textureItemCount / vertexCount;

    gkg::RankSiteLut< int32_t, float > rankSiteLut( meshMap.vertices );

    int32_t t = 0;
    for ( t = 0; t < textureRankCount; t++ )
    {

      float surfaceArea = 0.0;

      std::list< gkg::Polygon< 3U > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        const gkg::Vector3d< float >& site1 = vectorOfSites[ p->indices[ 0 ] ];
        const gkg::Vector3d< float >& site2 = vectorOfSites[ p->indices[ 1 ] ];
        const gkg::Vector3d< float >& site3 = vectorOfSites[ p->indices[ 2 ] ];

        int32_t textureIndex1 = rankSiteLut.getIndex( rank, site1 );
        int32_t textureIndex2 = rankSiteLut.getIndex( rank, site2 );
        int32_t textureIndex3 = rankSiteLut.getIndex( rank, site3 );

        const T& value1 = thresholdedTextureMap.getTexture( t * vertexCount +
                                                            textureIndex1 );
        const T& value2 = thresholdedTextureMap.getTexture( t * vertexCount +
                                                            textureIndex2 );
        const T& value3 = thresholdedTextureMap.getTexture( t * vertexCount +
                                                            textureIndex3 );
                            
        if ( ( value1 != background ) &&
             ( value2 != background ) &&
             ( value3 != background ) )
        {

          surfaceArea += getTriangleArea( site1, site2, site3 );

        }
                  
        ++ p;

      }

      surfaceAreas.push_back( surfaceArea );

    }

    return surfaceAreas;

  }
  GKG_CATCH( "template < class T > "
             "std::list< float > TextureMapThresholderProcess::getSurfaceAreas( "
             "const gkg::MeshMap< int32_t, float, 3 >& meshMap, "
             "const gkg::TextureMap< T >& thresholdedTextureMap, "
             "const T& background )" );


}


//
//   TextureMapThresholderCommand
//


gkg::TextureMapThresholderCommand::TextureMapThresholderCommand(
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
  GKG_CATCH( "gkg::TextureMapThresholderCommand::TextureMapThresholderCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapThresholderCommand::TextureMapThresholderCommand(
                                       const std::string& fileNameInputTexture,
                                       const std::string& fileNameInputMesh,
                                       const std::string& fileNameOutputTexture,
                                       const std::string& fileNameOutputSurface,
                                       const std::string& mode,
                                       double threshold1,
                                       double threshold2,
                                       double background,
                                       bool ascii,
                                       const std::string& format,
                                       bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameInputTexture, fileNameInputMesh, fileNameOutputTexture,
             fileNameOutputSurface, mode, threshold1, threshold2, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapThresholderCommand::TextureMapThresholderCommand( "
             "const std::string& fileNameInputTexture, "
             "const std::string& fileNameInputMesh, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& fileNameOutputSurface, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::TextureMapThresholderCommand::TextureMapThresholderCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputSurface );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputTexture, fileNameInputMesh, fileNameOutputTexture,
             fileNameOutputSurface, mode, threshold1, threshold2, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapThresholderCommand::TextureMapThresholderCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapThresholderCommand::~TextureMapThresholderCommand()
{
}


std::string gkg::TextureMapThresholderCommand::getStaticName()
{

  try
  {

    return "TextureMapThresholder";

  }
  GKG_CATCH( "std::string gkg::TextureMapThresholderCommand::getStaticName()" );

}


void gkg::TextureMapThresholderCommand::parse()
{

  try
  {

    std::string fileNameInputTexture;
    std::string fileNameInputMesh;
    std::string fileNameOutputTexture;
    std::string fileNameOutputSurface;
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
                                 _argc, _argv,
                                 "Threshold operation on texture map container",
                                 _loadPlugin );
    application.addSingleOption( "-it",
                                 "Input texture file name",
                                 fileNameInputTexture );
    application.addSingleOption( "-im",
                                 "Input mesh file name",
                                 fileNameInputMesh );
    application.addSingleOption( "-ot",
                                 "Output thresholded texture file name",
                                 fileNameOutputTexture );
    application.addSingleOption( "-os",
                                 "Output surface of thresholded texture "
                                 "file name",
                                 fileNameOutputSurface );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
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

    execute( fileNameInputTexture, fileNameInputMesh, fileNameOutputTexture,
             fileNameOutputSurface, mode, threshold1, threshold2, background,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapThresholderCommand::parse()" );

}


void gkg::TextureMapThresholderCommand::execute(
                  	               const std::string& fileNameInputTexture,
                                       const std::string& fileNameInputMesh,
                                       const std::string& fileNameOutputTexture,
                                       const std::string& fileNameOutputSurface,
                                       const std::string& mode,
                                       double threshold1,
                                       double threshold2,
                                       double background,
                                       bool ascii,
                                       const std::string& format,
                                       bool verbose )
{

  try
  {

    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid threshold mode" );

    }

    //
    // launching process
    //
    TextureMapThresholderProcess thresholderProcess( fileNameInputMesh,
                                          	     fileNameOutputTexture,
                                          	     fileNameOutputSurface,
                                          	     mode,
                                          	     threshold1,
                                          	     threshold2,
                                          	     background,
                                          	     ascii,
                                          	     format,
                                          	     verbose );
    thresholderProcess.execute( fileNameInputTexture );

  }
  GKG_CATCH( "void gkg::TextureMapThresholderCommand::execute( "
             "const std::string& fileNameInputTexture, "
             "const std::string& fileNameInputMesh, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& fileNameOutputSurface, "
             "const std::string& mode, double threshold1, double threshold2, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( TextureMapThresholderCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameInputTexture ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameInputMesh ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOutputTexture ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOutputSurface ) +
    			DECLARE_STRING_PARAMETER_HELP( mode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
