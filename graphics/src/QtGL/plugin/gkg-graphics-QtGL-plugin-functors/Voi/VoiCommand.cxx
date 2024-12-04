#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiCommand.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiMainWindow.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>

#include <QApplication>
#include <QStyleFactory>


gkg::VoiCommand::VoiCommand( int32_t argc,
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
  GKG_CATCH( "gkg::VoiCommand::VoiCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VoiCommand::VoiCommand( const std::string& anatName,
                             const std::string& fusionName,
                             const std::string& labelName,
                             const std::string& transfoName )
                : gkg::Command(),
                  _qtApplication( 0 )
{

  try
  {

    execute( anatName, fusionName, labelName, transfoName );

  }
  GKG_CATCH( "gkg::VoiCommand::VoiCommand( const std::string& anatName, "
             "const std::string& fusionName,  const std::string& labelName, "
             "const std::string& transfoName )" );

}


gkg::VoiCommand::VoiCommand( const gkg::Dictionary& parameters )
                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, anatName );
    DECLARE_STRING_PARAMETER( parameters, std::string, fusionName );
    DECLARE_STRING_PARAMETER( parameters, std::string, labelName );
    DECLARE_STRING_PARAMETER( parameters, std::string, transfoName );
    
    execute( anatName, fusionName, labelName, transfoName );

  }
  GKG_CATCH( "gkg::VoiCommand::VoiCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VoiCommand::~VoiCommand()
{

  delete _qtApplication;

}


std::string gkg::VoiCommand::getStaticName()
{

  try
  {

    return "Voi";

  }
  GKG_CATCH( "std::string gkg::VoiCommand::getStaticName()" );

}


void gkg::VoiCommand::parse()
{

  try
  {

    std::string anatName = "";
    std::string fusionName = "";
    std::string labelName = "";
    std::string transfoName = "";

#if QT_VERSION >= 0x050000
    QApplication::setStyle( QStyleFactory::create( "windows" ) );
    QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
#else
    QApplication::setStyle( new QWindowsStyle );
#endif
    _qtApplication = new QApplication( _argc, _argv );
    
    gkg::Application application( _argc, _argv,
                                  "Volume Of Interest",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                  "Input data file name",
				  anatName,
				  true );
    application.addSingleOption( "-f",
                                 "Input second file name (for fusion)",
				 fusionName,
				 true );
    application.addSingleOption( "-l",
                                 "Input label file name",
				 labelName,
				 true );
    application.addSingleOption( "-t",
                                 "Transformation file name",
				 transfoName,
				 true );
    application.initialize();

    execute( anatName, fusionName, labelName, transfoName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VoiCommand::parse()" );

}


void gkg::VoiCommand::execute( const std::string& anatName,
                               const std::string& fusionName,
                               const std::string& labelName,
                               const std::string& transfoName )
{

  try
  {

    if ( !_qtApplication )
    {

#if QT_VERSION >= 0x050000
      QApplication::setStyle( QStyleFactory::create( "windows" ) );
      QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
#else
      QApplication::setStyle( new QWindowsStyle );
#endif
      _qtApplication = new QApplication( _argc, _argv );

    }
    
    gkg::VoiMainWindow window;
    _qtApplication->setFont( QFont( "helvetica", 10 ) );
    _qtApplication->setStyleSheet(
      "QWidget { color: rgb( 200, 200, 200 ); "
                "background-color: rgb( 102, 102, 102 ) }"
      "QToolTip { border: 2px solid rgb( 100, 100, 100 );"
                 "color: rgb( 50, 50, 50 );"
                 "background: rgb( 180, 180, 180 ) }"
      "QLineEdit { selection-background-color: rgb( 140, 140, 140 ) }"
      "QSpinBox { selection-background-color: rgb( 140, 140, 140 ) }"
      "QComboBox { background-color: rgb( 102, 102, 102 );"
                  "selection-background-color: rgb( 102, 102, 102 ) }"
      "QComboBox QAbstractItemView "
                          "{ selection-background-color: rgb( 140, 140, 140 ) }"
      "QScrollBar::sub-page { background: rgb( 150, 150, 150 ) }"
      "QScrollBar::add-page { background: rgb( 150, 150, 150 ) }"
      "QTableWidget { selection-background-color: rgb( 180, 180, 180 ) }"
      "QTableWidget::item:hover { background: rgb( 140, 140, 140 ) }" );
    window.show();
    window.initializeArg( anatName, fusionName, labelName, transfoName );

    _qtApplication->exec();

  }
  GKG_CATCH( "void gkg::VoiCommand::execute( "
             "const std::string& fileNameVolume, "
             "const std::string& fileNameLabel )" );

}


RegisterCommandCreator( 
    VoiCommand,
    DECLARE_STRING_PARAMETER_HELP( anatName ) +
    DECLARE_STRING_PARAMETER_HELP( fusionName ) +
    DECLARE_STRING_PARAMETER_HELP( labelName ) +
    DECLARE_STRING_PARAMETER_HELP( transfoName ) );
