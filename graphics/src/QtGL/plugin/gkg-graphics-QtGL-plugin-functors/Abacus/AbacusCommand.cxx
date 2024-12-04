#include <gkg-graphics-QtGL-plugin-functors/Abacus/AbacusCommand.h>
#include <gkg-graphics-QtGL-plugin-functors/Abacus/AbacusMainWindow.h>
#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::AbacusCommand::AbacusCommand( int32_t argc,
                                   char* argv[],      
                                   bool loadPlugin,   
                                   bool removeFirst ) 
                  : gkg::Command( argc, argv, loadPlugin, removeFirst ),
                    _qtApplication( 0 )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::AbacusCommand::AbacusCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AbacusCommand::AbacusCommand( const std::string& fileName )
                  : gkg::Command(),
                    _qtApplication( 0 )
{

  try
  {

    execute( fileName );

  }
  GKG_CATCH( "gkg::AbacusCommand::AbacusCommand( "
             "const std::string& fileName )" );

}


gkg::AbacusCommand::AbacusCommand( const gkg::Dictionary& parameters )
                  : gkg::Command( parameters ),
                    _qtApplication( 0 )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileName );
    
    execute( fileName );

  }
  GKG_CATCH( "gkg::AbacusCommand::AbacusCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AbacusCommand::~AbacusCommand()
{

  delete _qtApplication;

}


std::string gkg::AbacusCommand::getStaticName()
{

  try
  {

    return "Abacus";

  }
  GKG_CATCH( "std::string gkg::AbacusCommand::getStaticName()" );

}


void gkg::AbacusCommand::parse()
{

  try
  {

    std::string fileName;

#if QT_VERSION >= 0x050000
    QApplication::setStyle( QStyleFactory::create( "windows" ) );
#else
    QApplication::setStyle( new QWindowsStyle );
#endif
    _qtApplication = new QApplication( _argc, _argv );
    
    gkg::Application application( _argc, _argv,
                                  "Abacus display",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                  "Input abacus file name",
				  fileName,
				  true );
    application.initialize();
 
    execute( fileName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AbacusCommand::parse()" );

}


void gkg::AbacusCommand::execute( const std::string& fileName )
{

  try
  {

    if ( !_qtApplication )
    {

#if QT_VERSION >= 0x050000
      QApplication::setStyle( QStyleFactory::create( "windows" ) );
#else
      QApplication::setStyle( new QWindowsStyle );
#endif
      _qtApplication = new QApplication( _argc, _argv );

    }

    gkg::AbacusMainWindow window;
    _qtApplication->setFont( QFont( "helvetica", 10 ) );
    window.show();
    window.initializeArg( fileName );

    _qtApplication->exec();

  }
  GKG_CATCH( "void gkg::AbacusCommand::execute( "
             "const std::string& fileName )" );

}


RegisterCommandCreator( AbacusCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileName ) );
