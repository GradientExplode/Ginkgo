#include <gkg-processing-plugin-functors/TextureMapMasker/TextureMapMaskerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//  MaskerProcess
//


class MaskerProcess : public gkg::Process
{

  public:

    MaskerProcess( const std::string& theFileNameMaskTextureMap,
                   const std::string& theFileNameOutputTextureMap,
                   const double& theBackground,
                   const bool& theAscii,
                   const std::string& theFormat,
                   const bool& theVerbose );

    const std::string& fileNameMaskTextureMap;
    const std::string& fileNameOutputTextureMap;
    const double& background;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void mask( gkg::Process& process,
                      const std::string& fileNameInputTextureMap,
                      const gkg::AnalyzedObject&,
                      const std::string& );


};


MaskerProcess::MaskerProcess( const std::string& theFileNameMaskTextureMap,
                              const std::string& theFileNameOutputTextureMap,
                              const double& theBackground,
                              const bool& theAscii,
                              const std::string& theFormat,
                              const bool& theVerbose )
              : gkg::Process( "TextureMap" ),
                fileNameMaskTextureMap( theFileNameMaskTextureMap ),
                fileNameOutputTextureMap( theFileNameOutputTextureMap ),
                background( theBackground ),
                ascii( theAscii ),
                format( theFormat ),
                verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &MaskerProcess::mask< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &MaskerProcess::mask< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &MaskerProcess::mask< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &MaskerProcess::mask< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &MaskerProcess::mask< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &MaskerProcess::mask< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &MaskerProcess::mask< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &MaskerProcess::mask< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &MaskerProcess::mask< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &MaskerProcess::mask< double > );

  }
  GKG_CATCH( "MaskerProcess::MaskerProcess( "
             "const std::string& theFileNameMaskTextureMap, "
             "const std::string& theFileNameOutputTextureMap, "
             "const double& theBackground, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void MaskerProcess::mask( gkg::Process& process,
                          const std::string& fileNameInputTextureMap,
                          const gkg::AnalyzedObject&,
                          const std::string& )
{

  try
  {

    MaskerProcess&
      maskerProcess = static_cast< MaskerProcess& >( process );

    const std::string& fileNameMaskTextureMap =
     maskerProcess.fileNameMaskTextureMap;
    const std::string& fileNameOutputTextureMap =
     maskerProcess.fileNameOutputTextureMap;
    const double& background =
     maskerProcess.background;
    const bool& ascii =
     maskerProcess.ascii;
    const std::string& format =
     maskerProcess.format;
    const bool& verbose =
     maskerProcess.verbose;

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
    // reading mask int16_t texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading mask (int16_t) texture map '"
                << fileNameInputTextureMap << "' : "
                << std::flush;

    }
    gkg::TextureMap< int16_t > maskTextureMap;
    gkg::Reader::getInstance().read( fileNameMaskTextureMap,
                                     maskTextureMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////

    if ( inputTextureMap.getCount() % maskTextureMap.getCount() )
    {

      throw std::runtime_error( "input texture map has a number of items that "
                                "is not a multiple of the number of items of "
                                "the mask texture name" );

    }
    int32_t maskItemCount = maskTextureMap.getCount();

    ////////////////////////////////////////////////////////////////////////////
    // masking texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "masking texture map : " << std::flush;

    }

    typename gkg::TextureMap< T >::iterator i = inputTextureMap.begin(),
                                            ie = inputTextureMap.end();
    gkg::TextureMap< int16_t >::const_iterator m = maskTextureMap.begin();

    int32_t currentItemIndex = 0;
    while ( i != ie )
    {

      if ( !m->second )
      {

        i->second =  ( T )background;

      }

      ++ i;
      ++ currentItemIndex;

      if ( currentItemIndex == maskItemCount )
      {

        m = maskTextureMap.begin();
        currentItemIndex = 0;

      }
      else
      {

        ++ m;

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing masked texture map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing masked texture map '"
                << fileNameOutputTextureMap << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTextureMap,
                                      inputTextureMap,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void MaskerProcess::mask( gkg::Process& process, "
             "const std::string& fileNameInputTextureMap, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  TextureMapMaskerCommand
//


gkg::TextureMapMaskerCommand::TextureMapMaskerCommand( int32_t argc,
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
  GKG_CATCH( "gkg::TextureMapMaskerCommand::TextureMapMaskerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapMaskerCommand::TextureMapMaskerCommand(
                             	    const std::string& fileNameInputTextureMap,
                             	    const std::string& fileNameMaskTextureMap,
                             	    const std::string& fileNameOutputTextureMap,
                             	    double background,
                             	    bool ascii,
                             	    const std::string& format,
                             	    bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameInputTextureMap, fileNameMaskTextureMap,
             fileNameOutputTextureMap, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapMaskerCommand::TextureMapMaskerCommand( "
             "const std::string& fileNameInputTextureMap, "
             "const std::string& fileNameMaskTextureMap, "
             "const std::string& fileNameOutputTextureMap, double background, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::TextureMapMaskerCommand::TextureMapMaskerCommand(
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
                              fileNameMaskTextureMap );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameOutputTextureMap );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputTextureMap, fileNameMaskTextureMap,
             fileNameOutputTextureMap, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::TextureMapMaskerCommand::TextureMapMaskerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapMaskerCommand::~TextureMapMaskerCommand()
{
}


std::string gkg::TextureMapMaskerCommand::getStaticName()
{

  try
  {

    return "TextureMapMasker";

  }
  GKG_CATCH( "std::string gkg::TextureMapMaskerCommand::getStaticName()" );

}


void gkg::TextureMapMaskerCommand::parse()
{

  try
  {

    std::string fileNameInputTextureMap;
    std::string fileNameMaskTextureMap;
    std::string fileNameOutputTextureMap;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
                        _argc, _argv,
                        "Mask a texture map container using a texture map mask",
                        _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input texture map file name",
                                 fileNameInputTextureMap );
    application.addSingleOption( "-m",
                                 "Mask (int16_t) texture map file name",
                                 fileNameMaskTextureMap );
    application.addSingleOption( "-o",
                                 "Output masked texture map file name",
                                 fileNameOutputTextureMap );
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

    execute( fileNameInputTextureMap, fileNameMaskTextureMap,
             fileNameOutputTextureMap, background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapMaskerCommand::parse()" );

}


void gkg::TextureMapMaskerCommand::execute(
                  	     	    const std::string& fileNameInputTextureMap,
                             	    const std::string& fileNameMaskTextureMap,
                             	    const std::string& fileNameOutputTextureMap,
                             	    double background,
                             	    bool ascii,
                             	    const std::string& format,
                             	    bool verbose )
{

  try
  {

    MaskerProcess maskerProcess( fileNameMaskTextureMap,
                                 fileNameOutputTextureMap,
                                 background,
                                 ascii,
                                 format,
                                 verbose );
    maskerProcess.execute( fileNameInputTextureMap );

  }
  GKG_CATCH( "void gkg::TextureMapMaskerCommand::execute( "
             "const std::string& fileNameInputTextureMap, "
             "const std::string& fileNameMaskTextureMap, "
             "const std::string& fileNameOutputTextureMap, double background, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                     TextureMapMaskerCommand,
    		     DECLARE_STRING_PARAMETER_HELP( fileNameInputTextureMap ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameMaskTextureMap ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameOutputTextureMap ) +
    		     DECLARE_FLOATING_PARAMETER_HELP( background ) +
    		     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		     DECLARE_STRING_PARAMETER_HELP( format ) +
    		     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
