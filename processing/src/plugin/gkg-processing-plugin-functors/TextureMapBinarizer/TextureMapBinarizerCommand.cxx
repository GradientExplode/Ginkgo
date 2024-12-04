#include <gkg-processing-plugin-functors/TextureMapBinarizer/TextureMapBinarizerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
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


class TextureMapBinarizerProcess : public gkg::Process
{

  public:

    TextureMapBinarizerProcess( const std::string& theFileNameInputMeshMap,
                                const std::string& theFileNameOutputTextureMap,
                     	        const std::string& theFileNameOutputSurface,
                     	        const std::string& theOutType,
                     	        const std::string& theMode,
                     	        const double& theThreshold1,
                     	        const double& theThreshold2,
                     	        const double& theBackground,
                     	        const double& theForeground,
                     	        const bool& theAscii,
                     	        const std::string& theFormat,
                     	        const bool& theVerbose );

    const std::string& fileNameInputMeshMap;
    const std::string& fileNameOutputTextureMap;
    const std::string& fileNameOutputSurface;
    const std::string& outType;
    const std::string& mode;
    const double& threshold1;
    const double& threshold2;
    const double& background;
    const double& foreground;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void binarize( gkg::Process& process,
                          const std::string& fileNameInputTextureMap,
                          const gkg::AnalyzedObject&,
                          const std::string& );

    template < class T >
    static std::list< float > getSurfaceAreas( 
                              const gkg::MeshMap< int32_t, float, 3 >& meshMap,
                              const gkg::TextureMap< T >& binarizedTextureMap,
                              const T& background );

};


TextureMapBinarizerProcess::TextureMapBinarizerProcess(
                                 const std::string& theFileNameInputMeshMap,
                                 const std::string& theFileNameOutputTextureMap,
                                 const std::string& theFileNameOutputSurface,
                                 const std::string& theOutType,
                                 const std::string& theMode,
                                 const double& theThreshold1,
                                 const double& theThreshold2,
                                 const double& theBackground,
                                 const double& theForeground,
                                 const bool& theAscii,
                                 const std::string& theFormat,
                                 const bool& theVerbose )
                          : gkg::Process( "TextureMap" ),
                            fileNameInputMeshMap( theFileNameInputMeshMap ),
                            fileNameOutputTextureMap(
                                                  theFileNameOutputTextureMap ),
                            fileNameOutputSurface( theFileNameOutputSurface ),
                    	    outType( theOutType ),
                    	    mode( theMode ),
                    	    threshold1( theThreshold1 ),
                    	    threshold2( theThreshold2 ),
                    	    background( theBackground ),
                    	    foreground( theForeground ),
                    	    ascii( theAscii ),
                    	    format( theFormat ),
                    	    verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &TextureMapBinarizerProcess::binarize< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &TextureMapBinarizerProcess::binarize< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &TextureMapBinarizerProcess::binarize< double > );

  }
  GKG_CATCH( "TextureMapBinarizerProcess::TextureMapBinarizerProcess( "
             "const std::string& theFileNameInputMeshMap, "
             "const std::string& theFileNameOutputTextureMap, "
             "const std::string& theFileNameOutputSurface, "
             "const std::string& theMode, "
             "const double& theThreshold1, "
             "const double& theThreshold2, "
             "const double& theBackground, "
             "const double& theForeground, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void TextureMapBinarizerProcess::binarize(
                                     gkg::Process& process,
                                     const std::string& fileNameInputTextureMap,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    TextureMapBinarizerProcess&
       binarizerProcess = static_cast< TextureMapBinarizerProcess& >( process );

    const std::string& fileNameInputMeshMap =
     binarizerProcess.fileNameInputMeshMap;
    const std::string& fileNameOutputTextureMap =
     binarizerProcess.fileNameOutputTextureMap;
    const std::string& fileNameOutputSurface =
     binarizerProcess.fileNameOutputSurface;
    const std::string& outType =
     binarizerProcess.outType;
    const std::string& mode =
     binarizerProcess.mode;
    const double& threshold1 =
     binarizerProcess.threshold1;
    const double& threshold2 =
     binarizerProcess.threshold2;
    const double& background =
     binarizerProcess.background;
    const double& foreground =
     binarizerProcess.foreground;
    const bool& ascii =
     binarizerProcess.ascii;
    const std::string& format =
     binarizerProcess.format;
    const bool& verbose =
     binarizerProcess.verbose;

    ////////////////////////////////////////////////////////////////////////////
    // reading input texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading input texture map '"
                << fileNameInputTextureMap << "' : "
                << std::flush;

    }
    gkg::TextureMap< T > inputTextureMap;
    gkg::Reader::getInstance().read( fileNameInputTextureMap,
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

      std::cout << "reading input mesh map '" << fileNameInputMeshMap << "' : "
                << std::flush;

    }

    gkg::MeshMap< int32_t, float, 3 > meshMap;
    gkg::Reader::getInstance().read( fileNameInputMeshMap, meshMap );

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
    // binarizing texture map
    ////////////////////////////////////////////////////////////////////////////

    std::list< float > surfaceAreas;
    if ( verbose )
    {

      std::cout << "binarizing texture map : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                                              mode,
	                                                      ( T )threshold1,
	                                                      ( T )threshold2 );
    if ( outType.empty() )
    {

      gkg::TextureMap< T > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< T > >
        binarizer( *testFunction,
                   ( T )foreground,
                   ( T )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
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

    }
    else if ( gkg::TypeOf< int8_t >::isSameType( outType ) )
    {

      gkg::TextureMap< int8_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< int8_t > >
        binarizer( *testFunction,
                   ( int8_t )foreground,
                   ( int8_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( int8_t )background );

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

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::TextureMap< uint8_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< uint8_t > >
        binarizer( *testFunction,
                   ( uint8_t )foreground,
                   ( uint8_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( uint8_t )background );

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

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::TextureMap< int16_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< int16_t > >
        binarizer( *testFunction,
                   ( int16_t )foreground,
                   ( int16_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( int16_t )background );

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

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( outType ) )
    {

      gkg::TextureMap< uint16_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< uint16_t > >
        binarizer( *testFunction,
                   ( uint16_t )foreground,
                   ( uint16_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( uint16_t )background );

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

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( outType ) )
    {

      gkg::TextureMap< int32_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< int32_t > >
        binarizer( *testFunction,
                   ( int32_t )foreground,
                   ( int32_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( int32_t )background );

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

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( outType ) )
    {

      gkg::TextureMap< uint32_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< uint32_t > >
        binarizer( *testFunction,
                   ( uint32_t )foreground,
                   ( uint32_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( uint32_t )background );

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

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( outType ) )
    {

      gkg::TextureMap< int64_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< int64_t > >
        binarizer( *testFunction,
                   ( int64_t )foreground,
                   ( int64_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( int64_t )background );

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

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( outType ) )
    {

      gkg::TextureMap< uint64_t > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< uint64_t > >
        binarizer( *testFunction,
                   ( uint64_t )foreground,
                   ( uint64_t )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( uint64_t )background );

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

    }
    else if ( gkg::TypeOf< float >::isSameType( outType ) )
    {

      gkg::TextureMap< float > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< float > >
        binarizer( *testFunction,
                   ( float )foreground,
                   ( float )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( float )background );

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

    }
    else if ( gkg::TypeOf< double >::isSameType( outType ) )
    {

      gkg::TextureMap< double > binarizedTextureMap;
      gkg::Binarizer< gkg::TextureMap< T >, gkg::TextureMap< double > >
        binarizer( *testFunction,
                   ( double )foreground,
                   ( double )background );
      binarizer.binarize( inputTextureMap, binarizedTextureMap );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing binarized texture map '"
                  << fileNameOutputTextureMap << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                        binarizedTextureMap,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // computing area of thresholded texture map
      if ( verbose )
      {

        std::cout << "computing surface area(s) : " << std::flush;

      }

      surfaceAreas = getSurfaceAreas( meshMap,
                                      binarizedTextureMap,
                                      ( double )background );

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

    }

    delete testFunction;


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
             "void TextureMapBinarizerProcess::binarize( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


template < class T >
std::list< float > TextureMapBinarizerProcess::getSurfaceAreas( 
                               const gkg::MeshMap< int32_t, float, 3 >& meshMap,
                               const gkg::TextureMap< T >& binarizedTextureMap,
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

    int32_t textureItemCount = binarizedTextureMap.getCount();

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

        const T& value1 = binarizedTextureMap.getTexture( t * vertexCount +
                                                          textureIndex1 );
        const T& value2 = binarizedTextureMap.getTexture( t * vertexCount +
                                                          textureIndex2 );
        const T& value3 = binarizedTextureMap.getTexture( t * vertexCount +
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
             "std::list< float > TextureMapBinarizerProcess::getSurfaceAreas( "
             "const gkg::MeshMap< int32_t, float, 3 >& meshMap, "
             "const gkg::TextureMap< T >& binarizedTextureMap, "
             "const T& background )" );


}


//
//   TextureMapBinarizerCommand
//


gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand( int32_t argc,
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
  GKG_CATCH( "gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand(
                                    const std::string& fileNameInputTextureMap,
                                    const std::string& fileNameInputMeshMap,
                                    const std::string& fileNameOutputTextureMap,
                                    const std::string& fileNameOutputSurface,
                                    const std::string& outType,
                                    const std::string& mode,
                                    double threshold1,
                                    double threshold2,
                                    double background,
                                    double foreground,
                                    bool ascii,
                                    const std::string& format,
                                    bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameInputTextureMap, fileNameInputMeshMap,
             fileNameOutputTextureMap, fileNameOutputSurface, outType, mode, 
             threshold1, threshold2, background, foreground, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand( "
             "const std::string& fileNameInputTextureMap, "
             "const std::string& fileNameInputMeshMap, "
             "const std::string& fileNameOutputTextureMap, "
             "const std::string& fileNameOutputSurface, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double background, "
             "double foreground, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameInputTextureMap );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string, 
                              fileNameInputMeshMap );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string, 
                              fileNameOutputTextureMap );
    DECLARE_STRING_PARAMETER( parameters, 
                              std::string, 
                              fileNameOutputSurface );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, foreground );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputTextureMap, fileNameInputMeshMap,
             fileNameOutputTextureMap, fileNameOutputSurface, outType, mode, 
             threshold1, threshold2, background, foreground, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapBinarizerCommand::TextureMapBinarizerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapBinarizerCommand::~TextureMapBinarizerCommand()
{
}


std::string gkg::TextureMapBinarizerCommand::getStaticName()
{

  try
  {

    return "TextureMapBinarizer";

  }
  GKG_CATCH( "std::string gkg::TextureMapBinarizerCommand::getStaticName()" );

}


void gkg::TextureMapBinarizerCommand::parse()
{

  try
  {

    std::string fileNameInputTextureMap;
    std::string fileNameInputMeshMap;
    std::string fileNameOutputTextureMap;
    std::string fileNameOutputSurface;
    std::string outType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double background = 0.0;
    double foreground = 1.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
                              _argc, _argv,
                              "Binarization operation on texture map container",
                              _loadPlugin );
    application.addSingleOption( "-it",
                                 "Input texture map file name",
                                 fileNameInputTextureMap );
    application.addSingleOption( "-im",
                                 "Input mesh map file name",
                                 fileNameInputMeshMap );
    application.addSingleOption( "-ot",
                                 "Output binarized texture map file name",
                                 fileNameOutputTextureMap );
    application.addSingleOption( "-os",
                                 "Output surface of binarized texture "
                                 "file name",
                                 fileNameOutputSurface );
    application.addSingleOption( "-t",
                                 "Output type (default=input type)",
                                 outType,
                                 true );
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
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
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

    execute( fileNameInputTextureMap, fileNameInputMeshMap,
             fileNameOutputTextureMap, fileNameOutputSurface, outType, mode, 
             threshold1, threshold2, background, foreground, ascii, format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapBinarizerCommand::parse()" );

}


void gkg::TextureMapBinarizerCommand::execute(
                  	            const std::string& fileNameInputTextureMap,
                                    const std::string& fileNameInputMeshMap,
                                    const std::string& fileNameOutputTextureMap,
                                    const std::string& fileNameOutputSurface,
                                    const std::string& outType,
                                    const std::string& mode,
                                    double threshold1,
                                    double threshold2,
                                    double background,
                                    double foreground,
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

    if ( !outType.empty() &&
         ( ! gkg::TypeOf< int8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< float >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< double >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    //
    // launching process
    //
    TextureMapBinarizerProcess binarizerProcess( fileNameInputMeshMap,
                                        	 fileNameOutputTextureMap,
                                        	 fileNameOutputSurface,
                                        	 outType,
                                        	 mode,
                                        	 threshold1,
                                        	 threshold2,
                                        	 background,
                                        	 foreground,
                                        	 ascii,
                                        	 format,
                                        	 verbose );
    binarizerProcess.execute( fileNameInputTextureMap );

  }
  GKG_CATCH( "void gkg::TextureMapBinarizerCommand::execute( "
             "const std::string& fileNameInputTextureMap, "
             "const std::string& fileNameInputMeshMap, "
             "const std::string& fileNameOutputTextureMap, "
             "const std::string& fileNameOutputSurface, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double background, "
             "double foreground, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
                     TextureMapBinarizerCommand,
                     DECLARE_STRING_PARAMETER_HELP( fileNameInputTextureMap ) +
                     DECLARE_STRING_PARAMETER_HELP( fileNameInputMeshMap ) +
                     DECLARE_STRING_PARAMETER_HELP( fileNameOutputTextureMap ) +
                     DECLARE_STRING_PARAMETER_HELP( fileNameOutputSurface ) +
                     DECLARE_STRING_PARAMETER_HELP( outType ) +
                     DECLARE_STRING_PARAMETER_HELP( mode ) +
                     DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                     DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                     DECLARE_FLOATING_PARAMETER_HELP( foreground ) +
                     DECLARE_FLOATING_PARAMETER_HELP( background ) +
                     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                     DECLARE_STRING_PARAMETER_HELP( format ) +
                     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
