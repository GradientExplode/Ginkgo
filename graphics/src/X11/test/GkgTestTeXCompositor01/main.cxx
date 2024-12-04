// TeX compositor


#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Character.h>
#include <gkg-graphics-X11-core/Composition.h>
#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/TeXCompositor.h>
#include <gkg-graphics-X11-core/LRComposition.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/ScrollBox.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/InputBufferFile.h>
#include <string>
#include <iostream>


//
// class DocumentView
//

class DocumentView : public gkg::MonoGlyph
{

  public:

    DocumentView( gkg::InputBufferFile* inputBufferFile );
    ~DocumentView();

    gkg::Adjustable* getAdjustable() const;

  private:

    gkg::Composition* _page;
    gkg::ScrollBox* _box;
    gkg::Glyph* _beginParagraph;
    gkg::Glyph* _endParagraph;
    gkg::Glyph* _beginLine;
    gkg::Glyph* _endLine;
    gkg::Glyph* _wordSpace;
    gkg::Glyph* _interline;
    gkg::Glyph* _vfilGlue;

    void add( const std::string& text );

};


DocumentView::DocumentView( gkg::InputBufferFile* inputBufferFile )
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Font* font = widgetFactory->getFont();
  const gkg::Color* fgColor = widgetFactory->getForeground();
  _wordSpace = layoutFactory.createSpaces( 2, 0.5, font, fgColor );
  _interline = layoutFactory.createVGlue();
  _vfilGlue = layoutFactory.createVGlue();
  std::string alignment = "justify";
  widgetFactory->getStyle()->findAttribute( "alignment", alignment );
  if ( alignment == "left" )
  {

    _beginLine = layoutFactory.createVStrut( 0 );
    _endLine = layoutFactory.createStrut( font, 0, fil, 0 );
    _beginParagraph = layoutFactory.createVStrut( 0 );
    _endParagraph = layoutFactory.createStrut( font, 0, fil, 0 );

  }
  else if ( alignment == "right" )
  {

    _beginLine = layoutFactory.createVStrut( 0, 0, 0, fil, 0 );
    _endLine = layoutFactory.createStrut( font );
    _beginParagraph = layoutFactory.createVStrut( 0, 0, 0, fil, 0 );
    _endParagraph = layoutFactory.createStrut( font );

  }
  else if ( alignment == "center" )
  {

    _beginLine = layoutFactory.createVStrut( 0, 0, 0, fil, 0 );
    _endLine = layoutFactory.createStrut( font, 0, fil, 0 );
    _beginParagraph = layoutFactory.createVStrut( 0, 0, 0, fil, 0 );
    _endParagraph = layoutFactory.createStrut( font, 0, fil, 0 );

  }
  else if ( alignment == "justify" )
  {

    _beginLine = layoutFactory.createVStrut( 0 );
    _endLine = layoutFactory.createStrut( font );
    _beginParagraph = layoutFactory.createVStrut( 0 );
    _endParagraph = layoutFactory.createStrut( font, 0, fil, 0 );

  }
  else 
  {

    std::cerr << "Unknown alignment: " << alignment << std::endl;
    ::exit( 1 );

  }

  if ( _beginParagraph )
  {

    _beginParagraph->addReference();

  }
  if ( _endParagraph )
  {

    _endParagraph->addReference();

  }
  if ( _beginLine )
  {

    _beginLine->addReference();

  }
  if ( _endLine )
  {

    _endLine->addReference();

  }
  if ( _wordSpace )
  {

    _wordSpace->addReference();

  }
  if ( _interline )
  {

    _interline->addReference();

  }
  if ( _vfilGlue )
  {

    _vfilGlue->addReference();

  }

  _box = layoutFactory.createVScrollBox();
  _box->setSmallScroll( gkg::DimensionY, 2 );
  _page = new gkg::LRComposition( _box,
                                  new gkg::TeXCompositor( 10 ),
                                  0, 6 * 72.0, fil, fil,
                                  inputBufferFile->getLength() );
  const char* buffer = 0;
  for ( ; ; )
  {

    int32_t length = inputBufferFile->read( buffer );
    if ( length <= 0 )
    {

      break;

    }
    add( std::string( buffer ) );

  }
  _page->append( _vfilGlue );
  _page->repair();
  setBody( _page );

}


DocumentView::~DocumentView()
{

  if ( _beginParagraph )
  {

    _beginParagraph->removeReference();

  }
  if ( _endParagraph )
  {

    _endParagraph->removeReference();

  }
  if ( _beginLine )
  {

    _beginLine->removeReference();

  }
  if ( _endLine )
  {

    _endLine->removeReference();

  }
  if ( _wordSpace )
  {

    _wordSpace->removeReference();

  }
  if ( _interline )
  {

    _interline->removeReference();

  }
  if ( _vfilGlue )
  {

    _vfilGlue->removeReference();

  }

}


void DocumentView::add( const std::string& data )
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const char* p = data.c_str();
  const char* end = p + data.length();
  const gkg::Font* font = widgetFactory->getFont();
  const gkg::Color* fgColor = widgetFactory->getForeground();
  gkg::Glyph* glyphs[ 256 ];
  for ( int32_t i = 0; i < 256; i++ )
  {

    glyphs[ i ] = new gkg::Character( i, font, fgColor );

  }

  if ( glyphs[ ( int32_t )'\n' ] )
  {

    glyphs[ ( int32_t )'\n' ]->removeReference();

  }
  glyphs[ ( int32_t )'\n' ] = layoutFactory.createDiscretionary(
                                 gkg::Compositor::PenaltyGood,
                                 _endParagraph,
                                 _endParagraph,
                                 layoutFactory.createDiscretionary( 0, 
                                                                    _interline,
                                                                    _vfilGlue,
                                                                    0,
                                                                    0 ),
	                         _beginParagraph );

  if ( glyphs[ ( int32_t )' ' ] )
  {

    glyphs[ ( int32_t )' ' ]->removeReference();

  }
  glyphs[ ( int32_t )' ' ] = layoutFactory.createDiscretionary(
                                 0,
                                 _wordSpace,
                                 _endLine,
                                 layoutFactory.createDiscretionary( 0, 
                                                                    _interline,
                                                                    _vfilGlue,
                                                                    0,
                                                                    0 ),
	                         _beginLine );

  if ( glyphs[ ( int32_t )'\t' ] )
  {

    glyphs[ ( int32_t )'\t' ]->removeReference();

  }
  glyphs[ ( int32_t )'\t' ] = new gkg::Label( "        ", font, fgColor );

  _page->append( _beginParagraph );
  for ( ; p < end; p++ )
  {

    _page->append( glyphs[ ( int32_t )*( ( unsigned char* )p ) ] );

  }

}


gkg::Adjustable* DocumentView::getAdjustable() const
{

  return _box;

}


//
// main procedure
//


static gkg::PropertyData properties[] = 
{

  { "*alignment", "justify" },
  { 0, 0 }

};


static gkg::OptionDescription options[] =
{

  { "-justify", "*alignment", 
    gkg::OptionDescription::OptionValueImplicit, "justify" },
  { "-center", "*alignment",
    gkg::OptionDescription::OptionValueImplicit, "center" },
  { "-left", "*alignment",
    gkg::OptionDescription::OptionValueImplicit, "left" },
  { "-right", "*alignment",
    gkg::OptionDescription::OptionValueImplicit, "right" },
  { 0, 0, gkg::OptionDescription::OptionNone, 0 }

};



int main( int argc, char* argv[] ) 
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestTeXCompositor01",
                      argc, argv, options, properties );

  if ( argc != 2 )
  {

    std::cerr << "Usage: " << argv[ 0 ] << " file" << std::endl;
    ::exit( 1 );

  }

  gkg::InputBufferFile inputBufferFile;
  inputBufferFile.open( argv[ 1 ] );
  if ( !inputBufferFile )
  {

    std::cerr << "Unable to open '" << argv[ 1 ] << "' file" << std::endl;
    ::exit( 1 );

  }


  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  DocumentView* view = new DocumentView( &inputBufferFile );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow(
                  layoutFactory.createHBox(
                    widgetFactory->createInsetFrame(
                      widgetFactory->createVScrollBar( view->getAdjustable() ) ),
                    new gkg::Background(
                      layoutFactory.createFlexible(
                        layoutFactory.createNatural(
                          layoutFactory.createVCenter( view, 1.0 ),
                          4 * 72.0, 6 * 72.0 ) ),
                      widgetFactory->getBackground() ) ) );

  int result = session.runWindow( aWindow );

  return result;

}
