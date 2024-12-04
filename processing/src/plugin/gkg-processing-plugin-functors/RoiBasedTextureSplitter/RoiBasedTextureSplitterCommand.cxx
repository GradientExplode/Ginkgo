#include <gkg-processing-plugin-functors/RoiBasedTextureSplitter/RoiBasedTextureSplitterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <iomanip>


//
//  RoiBasedTextureSplitterProcess
//
#include <gkg-core-exception/Exception.h>


class RoiBasedTextureSplitterProcess : public gkg::Process
{

  public:

    RoiBasedTextureSplitterProcess( const std::string& fileNameRois,
                                    const std::string& fileNameOut,
                                    const bool& ascii,
                                    const std::string& format,
                                    const bool& verbose );

    const std::string& fileNameRois;
    const std::string& fileNameOut;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void split( gkg::Process& process,
                       const std::string& fileNameTextureMap,
                       const gkg::AnalyzedObject&,
                       const std::string& );

};


RoiBasedTextureSplitterProcess::RoiBasedTextureSplitterProcess(
                                     const std::string& theFileNameRois,
                                     const std::string& theFileNameOut,
                                     const bool& theAscii,
                                     const std::string& theFormat,
                                     const bool& theVerbose )
                                : gkg::Process( "TextureMap" ),
                                  fileNameRois( theFileNameRois ),
                                  fileNameOut( theFileNameOut ),
                                  ascii( theAscii ),
                                  format( theFormat ),
                                  verbose( theVerbose )
{

  registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< int8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< uint8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< int16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< uint16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< int32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< uint32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< int64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                   &RoiBasedTextureSplitterProcess::split< uint64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                   &RoiBasedTextureSplitterProcess::split< float > );
  registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                   &RoiBasedTextureSplitterProcess::split< double > );

}


template < class T >
void 
RoiBasedTextureSplitterProcess::split( gkg::Process& process,
                                       const std::string& fileNameTextureMap,
                                       const gkg::AnalyzedObject&,
                                       const std::string& )
{

  try
  {

    RoiBasedTextureSplitterProcess&
      roiBasedTextureSplitterProcess =
                     static_cast< RoiBasedTextureSplitterProcess& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // reading input texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "reading input texture map '"
                << fileNameTextureMap
                << "' : " << std::flush;

    }
    
    gkg::TextureMap< T > textureMap;
    gkg::Reader::getInstance().read( fileNameTextureMap, textureMap );

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading ROI texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "reading ROI texture map '"
                << roiBasedTextureSplitterProcess.fileNameRois
                << "' : " << std::flush;

    }
    
    gkg::TextureMap< int16_t > textureMapRois;
    gkg::Reader::getInstance().read(
                                    roiBasedTextureSplitterProcess.fileNameRois,
                                    textureMapRois );

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting vertex count and rank count
    ////////////////////////////////////////////////////////////////////////////

    int32_t vertexCount = textureMapRois.getCount();
    int32_t rankCount = textureMap.getCount() / vertexCount;

    if ( textureMap.getCount() % vertexCount )
    {

      throw std::runtime_error( "incompatible scalar and ROI texture maps" );

    }

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "ROI vertex count :  " << vertexCount << std::endl;
      std::cout << "scalar texture rank count :  " << rankCount << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // analyzing the ROI texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "analyzing the ROI texture map : " << std::flush;

    }

    std::map< int16_t, std::list< int32_t > > roiVertexIndexLut;
    gkg::TextureMap< int16_t >::const_iterator
      r = textureMapRois.begin(),
      re = textureMapRois.end();
    int32_t index = 0;
    while ( r != re )
    {

      if ( r->second )
      {

        roiVertexIndexLut[ r->second ].push_back( index );

      }
      ++ index;
      ++ r;

    }

    int32_t labelCount = ( int32_t )roiVertexIndexLut.size();
    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << labelCount << " labels" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // getting root name of the output texture file name
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameOutRoot = roiBasedTextureSplitterProcess.fileNameOut;
    if ( ( fileNameOutRoot.length() > 4U ) &&
         ( fileNameOutRoot.substr( 
                               fileNameOutRoot.length() - 4U, 4U ) == ".tex" ) )
    {

      fileNameOutRoot = 
                     fileNameOutRoot.substr( 0, fileNameOutRoot.length() - 4U );

    }
    else if ( ( fileNameOutRoot.length() > 11U ) &&
         ( fileNameOutRoot.substr( 
                             fileNameOutRoot.length() - 11U, 11U ) == ".tex" ) )
    {

      fileNameOutRoot = 
                    fileNameOutRoot.substr( 0, fileNameOutRoot.length() - 11U );

    }
    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "output root filename : " << fileNameOutRoot << std::endl;

    }
   

    ////////////////////////////////////////////////////////////////////////////
    // looping over rank(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << "splitting texture map according to ROIs : " << std::flush;

    }

    std::vector< int32_t > aimsItemCount( rankCount, vertexCount );
    std::vector< int32_t > aimsRank( rankCount );
    int32_t rank = 0;
    for ( rank = 0; rank < rankCount; rank++ )
    {

      aimsRank[ rank ] = rank;

    }

    std::map< int16_t, std::list< int32_t > >::const_iterator
      l = roiVertexIndexLut.begin(),
      le = roiVertexIndexLut.end();
    int32_t labelIndex = 0;
    while ( l != le )
    {

      if ( roiBasedTextureSplitterProcess.verbose )
      {

        if ( labelIndex != 0 )
        {

          std::cout << gkg::Eraser( 27 );

        }
        std::cout << "label[ "
                  << std::setw( 4 ) << labelIndex + 1
                  << "(" << std::setw( 4 ) << l->first << ")"
                  << " / " << std::setw( 4 ) << labelCount
                  << " ] " << std::flush;

      }
      const std::list< int32_t >& indices = l->second;

      // building output textures
      std::vector< T > outputTextures( vertexCount * rankCount, ( T )0 );

      for ( rank = 0; rank < rankCount; rank++ )
      {

        std::vector< T > textures( vertexCount );

        // extracting textures corresponding to the rank
        typename gkg::TextureMap< T >::const_iterator
          t = textureMap.lower_bound( rank * vertexCount ),
          te = textureMap.upper_bound( ( rank + 1 ) * vertexCount );
        int32_t vertexIndex = 0;
        while ( t != te )
        {

          textures[ vertexIndex ] = t->second;
          ++ t;
          ++ vertexIndex;

        }

        // filling output textures
        std::list< int32_t >::const_iterator
          i = indices.begin(),
          ie = indices.end();
        while ( i != ie )
        {

          outputTextures[ rank * vertexCount + *i ] = textures[ *i ];

          ++ i;

        }

      }

      // creating output texture map for current ROI label
      gkg::TextureMap< T > outputTextureMap;
      outputTextureMap.addTextures( outputTextures );
      outputTextureMap.getHeader().addAttribute( "aims_item_count",
                                                 aimsItemCount );
      outputTextureMap.getHeader().addAttribute( "aims_rank",
                                                 aimsRank );
      outputTextureMap.getHeader().addAttribute( "aims_rank_count",
                                                 rankCount );

      // writing output texture map for current ROI label
      gkg::Writer::getInstance().write( 
             fileNameOutRoot + '_' + gkg::StringConverter::toString( l->first ),
             outputTextureMap,
             roiBasedTextureSplitterProcess.ascii,
             roiBasedTextureSplitterProcess.format );


      ++ l;
      ++ labelIndex;

    }


    if ( roiBasedTextureSplitterProcess.verbose )
    {

      std::cout << std::endl;

    }


  }
  GKG_CATCH( "template < class T > "
             "void RoiBasedTextureSplitterProcess::split( "
             "gkg::Process& process, "
             "const std::string& fileNameVolume, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  RoiBasedTextureSplitterCommand
//


gkg::RoiBasedTextureSplitterCommand::RoiBasedTextureSplitterCommand(
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
  GKG_CATCH( "gkg::RoiBasedTextureSplitterCommand::RoiBasedTextureSplitterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RoiBasedTextureSplitterCommand::RoiBasedTextureSplitterCommand(
                                    	  const std::string& fileNameTextureMap,
                                    	  const std::string& fileNameRois,
                                    	  const std::string& fileNameOut,
                                    	  bool ascii,
                                    	  const std::string& format,
                                    	  bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameTextureMap, fileNameRois, fileNameOut, ascii, format,
             verbose );

  }
  GKG_CATCH( 
         "gkg::RoiBasedTextureSplitterCommand::RoiBasedTextureSplitterCommand( "
         "const std::string& fileNameTextureMap, "
         "const std::string& fileNameRois, const std::string& fileNameOut, "
         "bool ascii, const std::string& format, bool verbose )" );

}


gkg::RoiBasedTextureSplitterCommand::RoiBasedTextureSplitterCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTextureMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRois );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameTextureMap, fileNameRois, fileNameOut, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::RoiBasedTextureSplitterCommand::"
             "RoiBasedTextureSplitterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RoiBasedTextureSplitterCommand::~RoiBasedTextureSplitterCommand()
{
}


std::string gkg::RoiBasedTextureSplitterCommand::getStaticName()
{

  try
  {

    return "RoiBasedTextureSplitter";

  }
  GKG_CATCH( "std::string gkg::RoiBasedTextureSplitterCommand::getStaticName()" );

}


void gkg::RoiBasedTextureSplitterCommand::parse()
{

  try
  {

    std::string fileNameTextureMap;
    std::string fileNameRois;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Surface ROI based texture splitter",
                                  _loadPlugin );
    application.addSingleOption( "-t",
                                 "Input scalar texture map",
                                 fileNameTextureMap );
    application.addSingleOption( "-r",
                                 "Input ROI int16_t texture map",
                                 fileNameRois );
    application.addSingleOption( "-o",
                                 "Output texture file name",
                                 fileNameOut );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameTextureMap, fileNameRois, fileNameOut, ascii, format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RoiBasedTextureSplitterCommand::parse()" );

}


void gkg::RoiBasedTextureSplitterCommand::execute(
                  	            	  const std::string& fileNameTextureMap,
                                    	  const std::string& fileNameRois,
                                    	  const std::string& fileNameOut,
                                    	  bool ascii,
                                    	  const std::string& format,
                                    	  bool verbose )
{

  try
  {

    RoiBasedTextureSplitterProcess  roiBasedTextureSplitterProcess(
                                        			   fileNameRois,
                                        			   fileNameOut,
                                        			   ascii,
                                        			   format,
                                        			   verbose );
    roiBasedTextureSplitterProcess.execute( fileNameTextureMap );

  }
  GKG_CATCH( "void gkg::RoiBasedTextureSplitterCommand::execute( "
             "const std::string& fileNameTextureMap, "
             "const std::string& fileNameRois, const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( RoiBasedTextureSplitterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameTextureMap ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameRois ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
