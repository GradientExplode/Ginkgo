#include <gkg-graphics-X11-core/FileChooserImplementation.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/FileBrowser.h>
#include <gkg-graphics-X11-core/FieldEditor.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Target.h>
#include <gkg-graphics-X11-core/ChoiceItem.h>


gkg::FileChooserImplementation::FileChooserImplementation(
                                     gkg::FileChooser* fileChooser,
                                     const std::string& directory,
                                     gkg::Style* style,
                                     gkg::FileChooserAction* fileChooserAction )
                               : _target( fileChooser ),
                                 _name( directory ),
                                 _fileBrowser( 0 ),
                                 _editor( 0 ),
                                 _filter( 0 ),
                                 _directoryFilter( 0 ),
                                 _filterMap( 0 ),
                                 _fileChooserAction( fileChooserAction ),
                                 _selected( "" ),
                                 _style( new gkg::Style( style ) )
{

  _directory = new gkg::Directory( _name );
  if ( !_directory->isValid() )
  {

    delete _directory;
    _directory = new gkg::Directory();
    // and what if we can't read the current directory?

  }
  if ( _fileChooserAction )
  {

    _fileChooserAction->addReference();

  }
  if ( _style )
  {

    _style->addReference();

  }
  _style->setAlias( "FileChooser" );
  _style->setAlias( "Dialog" );
  _update = new gkg::ActionCallback< gkg::FileChooserImplementation >(
                                       this,
                                       &gkg::FileChooserImplementation::build );
  _style->addTriggerAny( _update );
  build();

}                                    


gkg::FileChooserImplementation::~FileChooserImplementation()
{

  delete _directory;
  delete [] _filterMap;
  if ( _fileChooserAction )
  {

    _fileChooserAction->removeReference();

  }
  _style->removeTriggerAny( _update );
  if ( _style )
  {

    _style->removeReference();

  }

}


gkg::FileChooser* gkg::FileChooserImplementation::getTarget() const
{

  return _target;

}


const std::string& gkg::FileChooserImplementation::getSelected() const
{

  return _selected;

}


void gkg::FileChooserImplementation::reRead()
{

  if ( !chdir( _directory->getPath() ) )
  {

    // should generate an error message

  }

}


void gkg::FileChooserImplementation::dismiss( bool accept )
{

  if ( _fileChooserAction )
  {

    _fileChooserAction->execute( _target, accept );

  }

}


void gkg::FileChooserImplementation::build()
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::DialogFactory*
    dialogFactory = gkg::LookAndFeel::getInstance().getDialogFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  widgetFactory->pushStyle();
  widgetFactory->setStyle( _style );
  std::string caption( "" );
  _style->findAttribute( "caption", caption );
  std::string subcaption( "Enter filename:" );
  _style->findAttribute( "subcaption", subcaption );
  std::string open( "Open" );
  _style->findAttribute( "open", open );
  std::string close( "Cancel" );
  _style->findAttribute( "cancel", close );
  int32_t rows = 10;
  _style->findAttribute( "rows", rows );
  const Font* font = widgetFactory->getFont();
  gkg::FontBoundingBox boundingBox;
  font->getFontBoundingBox( boundingBox );
  gkg::FloatCoordinate height = rows * ( boundingBox.ascent +
                                         boundingBox.descent ) + 1.0;
  gkg::FloatCoordinate width;
  if ( !_style->findAttribute( "width", width ) )
  {

    width = 16 * font->getWidth( 'm' ) + 3.0;

  }

  gkg::Action*
    accept = new gkg::ActionCallback< gkg::FileChooserImplementation >(
                         this, &gkg::FileChooserImplementation::acceptBrowser );
  gkg::Action*
    cancel = new gkg::ActionCallback< gkg::FileChooserImplementation >(
                         this, &gkg::FileChooserImplementation::cancelBrowser );

  if ( !_editor )
  {

    _editor = dialogFactory->createFieldEditor(
               _directory->getPath(),
               _style,
               new gkg::FieldEditorCallback< gkg::FileChooserImplementation >(
                                this,
                                &gkg::FileChooserImplementation::acceptEditor,
                                &gkg::FileChooserImplementation::cancelEditor ),
               false,
               0.0 );

  }
  _fileBrowser = new gkg::FileBrowser( accept, cancel );

  _target->removeAllInputHandlers();
  _target->appendInputHandler( _editor );
  _target->appendInputHandler( _fileBrowser );

  gkg::Glyph* glyph = layoutFactory.createVBox();
  if ( !caption.empty() )
  {

    glyph->append( layoutFactory.createRMargin(
                  widgetFactory->createFancyLabel( caption ), 5.0, fil, 0.0 ) );

  }
  if ( !subcaption.empty() )
  {

    glyph->append( layoutFactory.createRMargin(
               widgetFactory->createFancyLabel( subcaption ), 5.0, fil, 0.0 ) );

  }
  glyph->append( layoutFactory.createVGlue( 5.0, 0.0, 2.0 ) );
  glyph->append( _editor );
  glyph->append( layoutFactory.createVGlue( 15.0, 0.0, 12.0 ) );
  glyph->append( layoutFactory.createHBox(
                   layoutFactory.createVCenter(
                     widgetFactory->createInsetFrame(
                       layoutFactory.createMargin(
                         layoutFactory.createNatural( _fileBrowser,
                                                      width,
                                                      height ),
                         1.0 ) ),
                     1.0 ),
                   layoutFactory.createHSpace( 4.0 ),
                   widgetFactory->createVScrollBar( 
                                    _fileBrowser->getAdjustable() ) ) );
  glyph->append( layoutFactory.createVSpace( 15.0 ) );
  if ( _style->getValueIsOn( "filter" ) )
  {

    gkg::FieldEditorAction* action =
      new gkg::FieldEditorCallback< gkg::FileChooserImplementation >(
                                  this,
                                  &gkg::FileChooserImplementation::acceptFilter,
                                  0 );
    _filter = addFilter( _style,
                         "filterPattern",
                         "",
                         "filterCaption",
                         "Filter:",
                         glyph,
                         action	);
    if ( _style->getValueIsOn( "directoryFilter" ) )
    {

      _directoryFilter = addFilter( _style,
                                    "directoryFilterPattern",
                                    "",
                                    "directoryFilterCaption",
                                    "Directory Filter:",
                                    glyph,
                                    action );

    }
    else
    {

      _directoryFilter = 0;

    }

  } 
  else
  {

    _filter = 0;
    _directoryFilter = 0;

  }
  glyph->append( layoutFactory.createHBox(
                   layoutFactory.createHGlue( 10.0 ),
                   layoutFactory.createVCenter(
                     widgetFactory->createDefaultButton( open, accept ) ),
                     layoutFactory.createHGlue( 10.0, 0.0, 5.0 ),
                     layoutFactory.createVCenter(
                       widgetFactory->createPushButton( close, cancel ) ),
                     layoutFactory.createHGlue( 10.0 )	) );

  _target->setBody( layoutFactory.createBack(
                      layoutFactory.createVCenter(
                        widgetFactory->createOutsetFrame(
                          layoutFactory.createMargin( glyph, 5.0 ) ),
                        1.0 ),
                      new gkg::Target( 0, gkg::Target::PrimitiveHit ) ) );
  _target->focus( _editor );
  widgetFactory->popStyle();
  load();

}


void gkg::FileChooserImplementation::clear()
{

  _fileBrowser->Browser::select( -1 );
  gkg::Glyph::Index count = _fileBrowser->Browser::getCount();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    _fileBrowser->removeSelectable( 0 );
    _fileBrowser->remove( 0 );

  }

}


void gkg::FileChooserImplementation::load()
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory&
    layoutFactory = gkg::LayoutFactory::getInstance();

  widgetFactory->pushStyle();
  widgetFactory->setStyle( _style );

  int32_t directoryCount = _directory->getCount();
  delete [] _filterMap;
  int32_t* index = new int32_t[ directoryCount ];
  _filterMap = index;
  for ( int32_t i = 0; i < directoryCount; i++ )
  {

    const std::string& f = _directory->getName( i );
    bool isDirectory = _directory->isDirectory( i );
    if ( ( isDirectory && isFiltered( f, _directoryFilter ) ) ||
         ( !isDirectory && isFiltered( f, _filter ) ) )
    {

      gkg::Glyph* name = widgetFactory->createLabel( f );
      if ( isDirectory )
      {

        name = layoutFactory.createHBox( name,
                                         widgetFactory->createLabel( 
                                           gkg::getDirectorySeparator() ) );

      }
      gkg::Glyph* label = new gkg::Target(
                              layoutFactory.createHMargin( name,
                                                           3.0, 0.0, 0.0, 15.0,
                                                           fil, 0.0 ),
                              gkg::Target::PrimitiveHit );
      gkg::TellTaleState* tellTaleState = new gkg::TellTaleState(
                                        gkg::TellTaleState::Enabled );
      _fileBrowser->appendSelectable( tellTaleState );
      _fileBrowser->append( new gkg::ChoiceItem(
                                        tellTaleState,
                                        label,
                                        widgetFactory->createBrightInsetFrame(
                                                                    label ) ) );
      *index++ = i;

    }

  }
  _fileBrowser->refresh();
  widgetFactory->popStyle();

}


gkg::FieldEditor* 
gkg::FileChooserImplementation::addFilter( gkg::Style* style,
                                           const std::string& patternAttribute,
                                           const std::string& defaultPattern,
                                           const std::string& captionAttribute,
                                           const std::string& defaultCaption,
                                           gkg::Glyph* body,
                                           gkg::FieldEditorAction* action )
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::DialogFactory*
    dialogFactory = gkg::LookAndFeel::getInstance().getDialogFactory();
  gkg::LayoutFactory&
    layoutFactory = gkg::LayoutFactory::getInstance();

  std::string pattern = defaultPattern;
  style->findAttribute( patternAttribute, pattern );
  std::string caption = defaultCaption;
  style->findAttribute( captionAttribute, caption );
  gkg::FieldEditor*
    fieldEditor = dialogFactory->createFieldEditor( pattern,
                                                    style,
                                                    action,
                                                    false,
                                                    0.0 );

  _target->appendInputHandler( fieldEditor );
  body->append( layoutFactory.createHBox(
                  layoutFactory.createVCenter(
                    widgetFactory->createFancyLabel( caption ),
                    0.5 ),
                  layoutFactory.createHSpace( 2.0 ),
                  layoutFactory.createVCenter( fieldEditor, 0.5 ) ) );
  body->append( layoutFactory.createVSpace( 10.0 ) );
  return fieldEditor;

}


bool gkg::FileChooserImplementation::isFiltered( const std::string& name,
                                                 gkg::FieldEditor* fieldEditor )
{

  if ( !fieldEditor )
  {

    return true;

  }
  const std::string& text = fieldEditor->getText();
  if ( text.empty() )
  {

    return true;

  }
  return gkg::Directory::match( name, text );

}


void gkg::FileChooserImplementation::acceptBrowser()
{

  int32_t i = ( int32_t )_fileBrowser->getSelected();
  if ( i == -1 )
  {

    acceptEditor( _editor );
    return;

  }
  i = _filterMap[ i ];
  std::string fullPath = _directory->getPath() + _directory->getName( i );
  _editor->setText( fullPath );
  _selected = _editor->getText();
  if ( _directory->isDirectory( i ) )
  {

    if ( chdir( fullPath ) )
    {

      _editor->setText( _directory->getPath() );
      _target->focus( _editor );

    }
    else
    {

      // should generate an error message

    }

  }
  else
  {

    _target->dismiss( true );

  }

}


void gkg::FileChooserImplementation::cancelBrowser()
{

  _selected = "";
  _target->dismiss( false );

}


void
gkg::FileChooserImplementation::acceptEditor( gkg::FieldEditor* fieldEditor )
{


  std::string path = gkg::getCanonicalDirectory( fieldEditor->getText() );
  fieldEditor->setText( path );
  if ( !chdir( path ) )
  {

    _selected = path;
    _target->dismiss( true );
//    fieldEditor->select( path.rfind( gkg::getDirectorySeparator() ) + 1,
//                         path.length() );
    fieldEditor->selectAll();

  }

}


void gkg::FileChooserImplementation::cancelEditor( gkg::FieldEditor* )
{

  _target->dismiss( false );

}


void gkg::FileChooserImplementation::acceptFilter( gkg::FieldEditor* )
{

  clear();
  load();

}


bool gkg::FileChooserImplementation::chdir( const std::string& name )
{

  gkg::Directory* directory = new gkg::Directory( name );
  if ( directory->isValid() )
  {

    delete _directory;
    _directory = directory;
    clear();
    load();
    return true;

  }
  return false;

}
