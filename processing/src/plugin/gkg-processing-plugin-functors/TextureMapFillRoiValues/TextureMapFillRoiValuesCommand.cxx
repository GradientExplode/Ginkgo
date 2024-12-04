#include <gkg-processing-plugin-functors/TextureMapFillRoiValues/TextureMapFillRoiValuesCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <list>
#include <cmath>


//
//  FillRoiValuesProcess
//


class FillRoiValuesProcess : public gkg::Process
{

  public:

    FillRoiValuesProcess( const std::vector< double >& theLabels,
                          const std::vector< double >& theValues,
                          const std::string& theFileNameOutputTexture,
                          const double& theBackground,
                          const bool& theAscii,
                          const std::string& theFormat,
                          const bool& theVerbose );

    const std::vector< double >& labels;
    const std::vector< double >& values;
    const std::string& fileNameOutputTexture;
    const double& background;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void select( gkg::Process& process,
                        const std::string& fileNameInputTexture,
                        const gkg::AnalyzedObject&,
                        const std::string& );


};


FillRoiValuesProcess::FillRoiValuesProcess(
                                  const std::vector< double >& theLabels,
                                  const std::vector< double >& theValues,
                                  const std::string& theFileNameOutputTexture,
                                  const double& theBackground,
                                  const bool& theAscii,
                                  const std::string& theFormat,
                                  const bool& theVerbose )
                     : gkg::Process( "TextureMap" ),
                       labels( theLabels ),
                       values( theValues ),
                       fileNameOutputTexture( theFileNameOutputTexture ),
                       background( theBackground ),
                       ascii( theAscii ),
                       format( theFormat ),
                       verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &FillRoiValuesProcess::select< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &FillRoiValuesProcess::select< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &FillRoiValuesProcess::select< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &FillRoiValuesProcess::select< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &FillRoiValuesProcess::select< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &FillRoiValuesProcess::select< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &FillRoiValuesProcess::select< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &FillRoiValuesProcess::select< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &FillRoiValuesProcess::select< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &FillRoiValuesProcess::select< double > );

  }
  GKG_CATCH( "FillRoiValuesProcess::FillRoiValuesProcess( "
             "const std::vector< double >& theLabels, "
             "const std::vector< double >& theValues, "
             "const std::string& theFileNameOutputTexture, "
             "const double& theBackground, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void FillRoiValuesProcess::select( gkg::Process& process,
                                   const std::string& fileNameInputTexture,
                                   const gkg::AnalyzedObject&,
                                   const std::string& )
{

  try
  {

    FillRoiValuesProcess& 
      fillRoiValuesProcess = static_cast< FillRoiValuesProcess& >( process );

    const std::vector< double >& labels =
     fillRoiValuesProcess.labels;
    const std::vector< double >& values =
     fillRoiValuesProcess.values;
    const std::string& fileNameOutputTexture =
     fillRoiValuesProcess.fileNameOutputTexture;
    const double& background =
     fillRoiValuesProcess.background;
    const bool& ascii =
     fillRoiValuesProcess.ascii;
    const std::string& format =
     fillRoiValuesProcess.format;
    const bool& verbose =
     fillRoiValuesProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // creating set of labels
    ////////////////////////////////////////////////////////////////////////////
    std::set< T > labelSet;
    std::map< T, float > labelToValueLut;

    std::vector< double >::const_iterator
      l = labels.begin(),
      le = labels.end();
    std::vector< double >::const_iterator
      v = values.begin();
    while ( l != le )
    {

      labelSet.insert( ( T )*l );
      labelToValueLut[ ( T )*l ] = ( float )( *v );
      ++ l;
      ++ v;

    }

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
    // selecting labels
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "selecting labels and replacing by values : " << std::flush;

    }

    std::list< float > textures;

    typename gkg::TextureMap< T >::iterator
      t = inputTextureMap.begin(),
      te = inputTextureMap.end();
    while ( t != te )
    {

      if ( labelSet.find( t->second ) == labelSet.end() )
      {

        textures.push_back( ( float )background );

      }
      else
      {

        textures.push_back( labelToValueLut[ t->second ] );

      }
      ++ t;

    }

    gkg::TextureMap< float > outputTextureMap;
    outputTextureMap.addTextures( textures );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing output texture map '"
                << fileNameOutputTexture << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTexture,
                                      outputTextureMap,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void FillRoiValuesProcess::select( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  TextureMapFillRoiValuesCommand
//


gkg::TextureMapFillRoiValuesCommand::TextureMapFillRoiValuesCommand(
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
  GKG_CATCH(
         "gkg::TextureMapFillRoiValuesCommand::TextureMapFillRoiValuesCommand( "
         "int32_t argc, char* argv[], bool loadPlugin, "
         "bool removeFirst )" );

}


gkg::TextureMapFillRoiValuesCommand::TextureMapFillRoiValuesCommand(
                                       const std::string& fileNameInputTexture,
    				       const std::string& fileNameOutputTexture,
    				       const std::vector< double >& labels,
    				       const std::vector< double >& values,
    				       double background,
    				       bool ascii,
    				       const std::string& format,
    				       bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameInputTexture, fileNameOutputTexture, labels, values,
             background, ascii, format, verbose );

  }
  GKG_CATCH( 
         "gkg::TextureMapFillRoiValuesCommand::TextureMapFillRoiValuesCommand( "
         "const std::string& fileNameInputTexture, "
    	 "const std::string& fileNameOutputTexture, "
    	 "const std::vector< double >& labels,  "
         "const std::vector< double >& values, double background, bool ascii, "
    	 "const std::string& format, bool verbose )" );

}


gkg::TextureMapFillRoiValuesCommand::TextureMapFillRoiValuesCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputTexture );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           labels );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           values );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputTexture, fileNameOutputTexture, labels, values,
             background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapFillRoiValuesCommand::"
             "TextureMapFillRoiValuesCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapFillRoiValuesCommand::~TextureMapFillRoiValuesCommand()
{
}


std::string gkg::TextureMapFillRoiValuesCommand::getStaticName()
{

  try
  {

    return "TextureMapFillRoiValues";

  }
  GKG_CATCH( 
           "std::string gkg::TextureMapFillRoiValuesCommand::getStaticName()" );

}


void gkg::TextureMapFillRoiValuesCommand::parse()
{

  try
  {

    std::string fileNameInputTexture;
    std::string fileNameOutputTexture;
    std::vector< double > labels;
    std::vector< double > values;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
                          _argc, _argv,
                          "Selector of labels in a label texture map container",
                          _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input texture file name",
                                 fileNameInputTexture );
    application.addSeriesOption( "-l",
                                 "Series of scalar labels to be selected",
                                 labels,
                                 1 );
    application.addSeriesOption( "-v",
                                 "Series of values corresponding to the labels",
                                 values,
                                 1 );
    application.addSingleOption( "-o",
                                 "Output texture file name",
                                 fileNameOutputTexture );
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

    execute( fileNameInputTexture, fileNameOutputTexture, labels, values,
             background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapFillRoiValuesCommand::parse()" );

}


void gkg::TextureMapFillRoiValuesCommand::execute(
                  	               const std::string& fileNameInputTexture,
    				       const std::string& fileNameOutputTexture,
    				       const std::vector< double >& labels,
    				       const std::vector< double >& values,
    				       double background,
    				       bool ascii,
    				       const std::string& format,
    				       bool verbose )
{

  try
  {

    FillRoiValuesProcess fillRoiValuesProcess( labels,
                                               values,
                                               fileNameOutputTexture,
                                               background,
                                               ascii,
                                               format,
                                               verbose );
    fillRoiValuesProcess.execute( fileNameInputTexture );

  }
  GKG_CATCH( "void gkg::TextureMapFillRoiValuesCommand::execute( "
             "const std::string& fileNameInputTexture, "
    	     "const std::string& fileNameOutputTexture, "
    	     "const std::vector< double >& labels,  "
             "const std::vector< double >& values, double background, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( TextureMapFillRoiValuesCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameInputTexture ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOutputTexture ) +
    			DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( labels ) +
    			DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( values ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
