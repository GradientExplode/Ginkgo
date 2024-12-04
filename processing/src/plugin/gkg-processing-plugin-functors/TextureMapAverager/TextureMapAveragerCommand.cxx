#include <gkg-processing-plugin-functors/TextureMapAverager/TextureMapAveragerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   TextureMapAveragerProcess
//


class TextureMapAveragerProcess : public gkg::Process
{

  public:

    TextureMapAveragerProcess( 
                         const std::vector< std::string >& theInputFileNameList,
                         const std::string& theFileNameOutputTexture,
                         const std::string& theMode,
                         const bool& theAscii,
                         const std::string& theFormat,
                         const bool& theVerbose );

    const std::vector< std::string >& fileNameInputList;
    const std::string& fileNameOutputTexture;
    const std::string& mode;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void average( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


TextureMapAveragerProcess::TextureMapAveragerProcess(
                         const std::vector< std::string >& theInputFileNameList,
                         const std::string& theFileNameOutputTexture,
                         const std::string& theMode,
                         const bool& theAscii,
                         const std::string& theFormat,
                         const bool& theVerbose )
                          : gkg::Process( "TextureMap" ),
                            fileNameInputList( theInputFileNameList ),
                            fileNameOutputTexture( theFileNameOutputTexture ),
                            mode( theMode ),
                            ascii( theAscii ),
                            format( theFormat ),
                            verbose( theVerbose )
{

  registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                   &TextureMapAveragerProcess::average< int8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                   &TextureMapAveragerProcess::average< uint8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                   &TextureMapAveragerProcess::average< int16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                   &TextureMapAveragerProcess::average< uint16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                   &TextureMapAveragerProcess::average< int32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                   &TextureMapAveragerProcess::average< uint32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                   &TextureMapAveragerProcess::average< int64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                   &TextureMapAveragerProcess::average< uint64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                   &TextureMapAveragerProcess::average< float > );
  registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                   &TextureMapAveragerProcess::average< double > );

}


template < class T >
void TextureMapAveragerProcess::average( gkg::Process& process,
                                      const std::string& fileNameInputTexture,
                                      const gkg::AnalyzedObject&,
                                      const std::string& )
{

  try
  {

    TextureMapAveragerProcess&
    textureMapAveragerProcess = static_cast< 
                                        TextureMapAveragerProcess& >( process );

    const std::vector< std::string >& 
      fileNameInputList = textureMapAveragerProcess.fileNameInputList;
    const std::string& 
      fileNameOutputTexture = textureMapAveragerProcess.fileNameOutputTexture;
    const std::string& 
      mode = textureMapAveragerProcess.mode;
    const bool& 
      ascii = textureMapAveragerProcess.ascii;
    const std::string& 
      format = textureMapAveragerProcess.format;
    const bool& 
      verbose = textureMapAveragerProcess.verbose;

    // reading data
    if ( verbose )
    {

      std::cout << "reading '" << fileNameInputTexture << "' : " << std::flush;

    }
    gkg::TextureMap< T > inputTexture;
    gkg::Reader::getInstance().read( fileNameInputTexture, inputTexture );
    int32_t itemCount = inputTexture.getCount();
    if ( verbose )
    {

      std::cout << itemCount << " items" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "building updatable statistics mapper:  " << std::flush;

    }
    std::vector< gkg::UpdatableMeanStandardDeviationMinMax >
      statisticsMapper( itemCount );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "averaging:  " << std::flush;

    }
    int32_t inputTextureCount = ( int32_t )fileNameInputList.size();
    int32_t i = 0;
    for ( i = 0; i < inputTextureCount; i++ )
    {

      gkg::TextureMap< T > currentInputTexture;
      if ( i > 0 )
      {

        gkg::Reader::getInstance().read( fileNameInputList[ i ],
                                         currentInputTexture );
        if ( inputTexture.getCount() != itemCount )
        {

          throw std::runtime_error(
            std::string( "incompatible item count for file '" ) +
            fileNameInputList[ i ] + "'" );

        }

      }
      typename gkg::TextureMap< T >::const_iterator
        t = currentInputTexture.begin(),
        te = currentInputTexture.end();
      std::vector< gkg::UpdatableMeanStandardDeviationMinMax >::iterator
        s = statisticsMapper.begin();
      while ( t != te )
      {

        s->add( ( double )t->second );
        ++ t;
        ++ s;

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "extracting output texture:  " << std::flush;

    }
    gkg::TextureMap< float > outputTexture;
    std::vector< float > items;
    if ( mode == "all" )
    {

      items.resize( itemCount * 4, 0.0 );
      for ( i = 0; i < itemCount; i++ )
      {

        items[ i ] = ( float )statisticsMapper[ i ].getMean();
        items[ i + itemCount ] =
                          ( float )statisticsMapper[ i ].getStandardDeviation();
        items[ i + 2 * itemCount ] =
                                    ( float )statisticsMapper[ i ].getMinimum();
        items[ i + 3 * itemCount ] =
                                    ( float )statisticsMapper[ i ].getMaximum();

      }
      outputTexture.getHeader().addAttribute(
                                       "aims_item_count",
                                       std::vector< int32_t >( 4, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 4 );
      std::vector< int32_t > aimsRank( 4 );
      aimsRank[ 0 ] = 0;
      aimsRank[ 1 ] = 1;
      aimsRank[ 2 ] = 2;
      aimsRank[ 3 ] = 3;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "mean" )
    {

      items.resize( itemCount, 0.0 );
      for ( i = 0; i < itemCount; i++ )
      {

        items[ i ] = ( float )statisticsMapper[ i ].getMean();

      }

    }
    else if ( mode == "stddev" )
    {

      items.resize( itemCount, 0.0 );
      for ( i = 0; i < itemCount; i++ )
      {

        items[ i ] = ( float )statisticsMapper[ i ].getStandardDeviation();

      }

    }
    else if ( mode == "min" )
    {

      items.resize( itemCount, 0.0 );
      for ( i = 0; i < itemCount; i++ )
      {

        items[ i ] = ( float )statisticsMapper[ i ].getMinimum();

      }

    }
    else if ( mode == "max" )
    {

      items.resize( itemCount, 0.0 );
      for ( i = 0; i < itemCount; i++ )
      {

        items[ i ] = ( float )statisticsMapper[ i ].getMaximum();

      }

    }
    outputTexture.addTextures( items );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // writing output texture
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOutputTexture << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTexture,
                                      outputTexture,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void TextureMapAveragerProcess::average( gkg::Process& process, "
             "const std::string& fileNameInputTexture, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   TextureMapAveragerCommand
//


gkg::TextureMapAveragerCommand::TextureMapAveragerCommand( int32_t argc,
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
  GKG_CATCH( "gkg::TextureMapAveragerCommand::TextureMapAveragerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapAveragerCommand::TextureMapAveragerCommand(
                            const std::vector< std::string >& fileNameInputList,
                            const std::string& fileNameOutputTexture,
                            const std::string& mode,
                            bool ascii,
                            const std::string& format,
                            bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameInputList, fileNameOutputTexture, mode, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapAveragerCommand::TextureMapAveragerCommand( "
             "const std::vector< std::string >& fileNameInputList, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& mode, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::TextureMapAveragerCommand::TextureMapAveragerCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameInputList );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputList, fileNameOutputTexture, mode, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapAveragerCommand::TextureMapAveragerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapAveragerCommand::~TextureMapAveragerCommand()
{
}


std::string gkg::TextureMapAveragerCommand::getStaticName()
{

  try
  {

    return "TextureMapAverager";

  }
  GKG_CATCH( "std::string gkg::TextureMapAveragerCommand::getStaticName()" );

}


void gkg::TextureMapAveragerCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameInputList;
    std::string fileNameOutputTexture;
    std::string mode = "all";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Texture map averager",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input texture file name list",
                                 fileNameInputList,
                                 1 );
    application.addSingleOption( "-o",
                                 "Output texture file name (can be 4D and"
                                 "include mean/stddev/min/max information)",
                                 fileNameOutputTexture );
    application.addSingleOption( "-m",
                                 "Output mode: \n"
                                 "- mean -> mean texture\n"
                                 "- stddev -> standard deviation texture\n"
                                 "- min -> minimum texture\n"
                                 "- max -> maximum texture\n"
                                 "- all -> mean/stddev/min/max 4D texture\n"
                                 "(default=all)",
                                 mode,
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

    execute( fileNameInputList, fileNameOutputTexture, mode, ascii, format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapAveragerCommand::parse()" );

}


void gkg::TextureMapAveragerCommand::execute(
                  	    const std::vector< std::string >& fileNameInputList,
                            const std::string& fileNameOutputTexture,
                            const std::string& mode,
                            bool ascii,
                            const std::string& format,
                            bool verbose )
{

  try
  {

    //
    // sanity checks
    //

    if ( !( ( mode == "all" ) ||
            ( mode == "mean" ) ||
            ( mode == "stddev" ) ||
            ( mode == "min" ) ||
            ( mode == "max" ) ) )
    {

      throw std::runtime_error(
                         "output mode must be one of all/mean/stddev/min/max" );

    }
            

    //
    // launching process
    //
    TextureMapAveragerProcess textureMapAveragerProcess( fileNameInputList,
                                                         fileNameOutputTexture,
                                                         mode,
                                                         ascii,
                                                         format,
                                                         verbose );
    textureMapAveragerProcess.execute( fileNameInputList[ 0 ] );

  }
  GKG_CATCH( "void gkg::TextureMapAveragerCommand::execute( "
             "const std::vector< std::string >& fileNameInputList, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& mode, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
                 TextureMapAveragerCommand,
    		 DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameInputList ) +
    		 DECLARE_STRING_PARAMETER_HELP( fileNameOutputTexture ) +
    		 DECLARE_STRING_PARAMETER_HELP( mode ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		 DECLARE_STRING_PARAMETER_HELP( format ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
