#include <gkg-processing-plugin-functors/TextureMapSelector/TextureMapSelectorCommand.h>
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
//   SelectorProcess
//


class SelectorProcess : public gkg::Process
{

  public:

    SelectorProcess( const std::vector< double >& theLabels,
                     const std::string& theFileNameOutputTexture,
                     const double& theBackground,
                     const bool& theAscii,
                     const std::string& theFormat,
                     const bool& theVerbose );

    const std::vector< double >& labels;
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


SelectorProcess::SelectorProcess( const std::vector< double >& theLabels,
                                  const std::string& theFileNameOutputTexture,
                                  const double& theBackground,
                                  const bool& theAscii,
                                  const std::string& theFormat,
                                  const bool& theVerbose )
                : gkg::Process( "TextureMap" ),
                  labels( theLabels ),
                  fileNameOutputTexture( theFileNameOutputTexture ),
                  background( theBackground ),
                  ascii( theAscii ),
                  format( theFormat ),
                  verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &SelectorProcess::select< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &SelectorProcess::select< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &SelectorProcess::select< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &SelectorProcess::select< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &SelectorProcess::select< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &SelectorProcess::select< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &SelectorProcess::select< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &SelectorProcess::select< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &SelectorProcess::select< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &SelectorProcess::select< double > );

  }
  GKG_CATCH( "SelectorProcess::SelectorProcess( "
             "const std::vector< double >& theLabels, "
             "const std::string& theFileNameOutputTexture, "
             "const double& theBackground, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void SelectorProcess::select( gkg::Process& process,
                              const std::string& fileNameInputTexture,
                              const gkg::AnalyzedObject&,
                              const std::string& )
{

  try
  {

    SelectorProcess& 
      selectorProcess = static_cast< SelectorProcess& >( process );

    const std::vector< double >& labels =
     selectorProcess.labels;
    const std::string& fileNameOutputTexture =
     selectorProcess.fileNameOutputTexture;
    const double& background =
     selectorProcess.background;
    const bool& ascii =
     selectorProcess.ascii;
    const std::string& format =
     selectorProcess.format;
    const bool& verbose =
     selectorProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // creating set of labels
    ////////////////////////////////////////////////////////////////////////////
    std::set< T > labelSet;

    std::vector< double >::const_iterator
      l = labels.begin(),
      le = labels.end();
    while ( l != le )
    {

      labelSet.insert( ( T )*l );
      ++ l;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading input texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading texture map '" << fileNameInputTexture << "' : "
                << std::flush;

    }
    gkg::TextureMap< T > textureMap;
    gkg::Reader::getInstance().read( fileNameInputTexture,
                                     textureMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // selecting labels
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "selecting labels : " << std::flush;

    }

    typename gkg::TextureMap< T >::iterator
      t = textureMap.begin(),
      te = textureMap.end();
    while ( t != te )
    {

      if ( labelSet.find( t->second ) == labelSet.end() )
      {

        t->second = ( T )background;

      }
      ++ t;

    }

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
                                      textureMap,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void SelectorProcess::select( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   TextureMapSelectorCommand
//


gkg::TextureMapSelectorCommand::TextureMapSelectorCommand( int32_t argc,
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
  GKG_CATCH( "gkg::TextureMapSelectorCommand::TextureMapSelectorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapSelectorCommand::TextureMapSelectorCommand(
                                       const std::string& fileNameInputTexture,
                                       const std::string& fileNameOutputTexture,
                                       const std::vector< double >& labels,
                                       double background,
                                       bool ascii,
                                       const std::string& format,
                                       bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameInputTexture, fileNameOutputTexture, labels, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapSelectorCommand::TextureMapSelectorCommand( "
             "const std::string& fileNameInputTexture, "
             "const std::string& fileNameOutputTexture, "
             "const std::vector< double >& labels, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::TextureMapSelectorCommand::TextureMapSelectorCommand(
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
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputTexture, fileNameOutputTexture, labels, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapSelectorCommand::TextureMapSelectorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapSelectorCommand::~TextureMapSelectorCommand()
{
}


std::string gkg::TextureMapSelectorCommand::getStaticName()
{

  try
  {

    return "TextureMapSelector";

  }
  GKG_CATCH( "std::string gkg::TextureMapSelectorCommand::getStaticName()" );

}


void gkg::TextureMapSelectorCommand::parse()
{

  try
  {

    std::string fileNameInputTexture;
    std::string fileNameOutputTexture;
    std::vector< double > labels;
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

    execute( fileNameInputTexture, fileNameOutputTexture, labels, background,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapSelectorCommand::parse()" );

}


void gkg::TextureMapSelectorCommand::execute(
                  	               const std::string& fileNameInputTexture,
                                       const std::string& fileNameOutputTexture,
                                       const std::vector< double >& labels,
                                       double background,
                                       bool ascii,
                                       const std::string& format,
                                       bool verbose )
{

  try
  {

    SelectorProcess selectorProcess( labels,
                                     fileNameOutputTexture,
                                     background,
                                     ascii,
                                     format,
                                     verbose );
    selectorProcess.execute( fileNameInputTexture );

  }
  GKG_CATCH( "void gkg::TextureMapSelectorCommand::execute( "
             "const std::string& fileNameInputTexture, "
             "const std::string& fileNameOutputTexture, "
             "const std::vector< double >& labels, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( TextureMapSelectorCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameInputTexture ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOutputTexture ) +
    			DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( labels ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
