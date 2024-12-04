#include <gkg-graphics-X11-X11/X11GUIImplementationFactory.h>
#include <gkg-graphics-X11-X11/X11CanvasImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11ManagedWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11ApplicationWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11TransientWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11PopupWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11UnvisibleWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11IconWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11BrushImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-X11/X11FontFamilyImplementation.h>
#include <gkg-graphics-X11-X11/X11FontImplementation.h>
#include <gkg-graphics-X11-X11/X11BitmapImplementation.h>
#include <gkg-graphics-X11-X11/X11RasterImplementation.h>
#include <gkg-graphics-X11-X11/X11SessionImplementation.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DataCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11BitmapCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11FontCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11PredefinedCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11SelectionManagerImplementation.h>
#include <gkg-graphics-X11-X11/X11DragImplementation.h>
#include <gkg-graphics-X11-X11/X11DragZoneImplementation.h>
#include <gkg-graphics-X11-X11/X11DragZoneSinkImplementation.h>
#include <gkg-graphics-X11-X11/X11RequestErrorImplementation.h>
#include <gkg-graphics-X11-core/PredefinedCursor.h>
#include <gkg-graphics-X11-core/DataCursor.h>
#include <X11/cursorfont.h>


//
// access to factory
//

gkg::GUIImplementationFactory* gkg::getGUIImplementationFactory()
{

  return &gkg::X11GUIImplementationFactory::getInstance();

}

//
// X11GUIImplementationFactory methods
//


static const gkg::CursorPattern _textPattern =
{

  0x0000, 0x4400, 0x2800, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 
  0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x2800, 0x4400, 0x0000

};


static const gkg::CursorPattern _textMask =
{

  0x0000, 0xCC00, 0x7800, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 
  0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x7800, 0xCC00, 0x0000, 

};


static const gkg::CursorPattern _noPattern =
{

  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0

};


gkg::X11GUIImplementationFactory::X11GUIImplementationFactory()
                                 : gkg::GUIImplementationFactory()
{
}


gkg::X11GUIImplementationFactory::~X11GUIImplementationFactory()
{
}


gkg::CanvasImplementation*
gkg::X11GUIImplementationFactory::createCanvasImplementation( 
                                                     gkg::Canvas* canvas,
                                                     gkg::Window* window ) const
{

  return new gkg::X11CanvasImplementation( canvas, window );

}


gkg::DisplayImplementation*
gkg::X11GUIImplementationFactory::createDisplayImplementation( 
                                               gkg::Display* display,
                                               const std::string& device ) const
{

  return new gkg::X11DisplayImplementation( display, device );

}


gkg::WindowImplementation*
gkg::X11GUIImplementationFactory::createWindowImplementation( 
                                                       gkg::Window* window,
                                                       gkg::Glyph* glyph ) const
{

  return new gkg::X11WindowImplementation( window, glyph );

}


gkg::ManagedWindowImplementation*
gkg::X11GUIImplementationFactory::createManagedWindowImplementation( 
                                       gkg::ManagedWindow* managedWindow ) const
{

  return new gkg::X11ManagedWindowImplementation( managedWindow );

}


gkg::ApplicationWindowImplementation*
gkg::X11GUIImplementationFactory::createApplicationWindowImplementation( 
                               gkg::ApplicationWindow* applicationWindow ) const
{

  return new gkg::X11ApplicationWindowImplementation( applicationWindow );

}


gkg::TopLevelWindowImplementation*
gkg::X11GUIImplementationFactory::createTopLevelWindowImplementation( 
                                     gkg::TopLevelWindow* topLevelWindow ) const
{

  return new gkg::X11TopLevelWindowImplementation( topLevelWindow );

}


gkg::TransientWindowImplementation*
gkg::X11GUIImplementationFactory::createTransientWindowImplementation( 
                                   gkg::TransientWindow* transientWindow ) const
{

  return new gkg::X11TransientWindowImplementation( transientWindow );

}


gkg::PopupWindowImplementation*
gkg::X11GUIImplementationFactory::createPopupWindowImplementation( 
                                           gkg::PopupWindow* popupWindow ) const
{

  return new gkg::X11PopupWindowImplementation( popupWindow );

}


gkg::UnvisibleWindowImplementation*
gkg::X11GUIImplementationFactory::createUnvisibleWindowImplementation( 
                                          gkg::UnvisibleWindow* unvisibleWindow,
                                          gkg::Display* display ) const
{

  return new gkg::X11UnvisibleWindowImplementation( unvisibleWindow,
                                                    display );

}


gkg::IconWindowImplementation*
gkg::X11GUIImplementationFactory::createIconWindowImplementation( 
                                             gkg::IconWindow* iconWindow ) const
{

  return new gkg::X11IconWindowImplementation( iconWindow );

}


gkg::BrushImplementation*
gkg::X11GUIImplementationFactory::createBrushImplementation( 
                                                const int32_t* pattern,
                                                int32_t count,
                                                gkg::PixelCoordinate pixelWidth,
                                                gkg::Display* display ) const
{

  return new gkg::X11BrushImplementation( pattern,
                                          count,
                                          pixelWidth,
                                          display );

}


gkg::ColorImplementation*
gkg::X11GUIImplementationFactory::createColorImplementation( 
                                         gkg::Visual* visual,
                                         gkg::FloatColorIntensity r,
                                         gkg::FloatColorIntensity g,
                                         gkg::FloatColorIntensity b,
                                         float alpha,
                                         gkg::Color::Operation operation ) const
{

  return new gkg::X11ColorImplementation( visual, r, g, b, alpha, operation );

}


gkg::FontFamilyImplementation* 
gkg::X11GUIImplementationFactory::createFontFamilyImplementation(
                                           gkg::FontFamily* fontFamily,
                                           gkg::Display* display,
                                           const std::string& familyName ) const
{

  return new gkg::X11FontFamilyImplementation( fontFamily,
                                               display, 
                                               familyName );

}


gkg::FontImplementation* 
gkg::X11GUIImplementationFactory::createFontImplementation(
                                                        gkg::Display* display,
                                                        const std::string& name,
                                                        float scale ) const
{

  return new gkg::X11FontImplementation( display, name, scale );

}


gkg::BitmapImplementation* 
gkg::X11GUIImplementationFactory::createBitmapImplementation(
                                               gkg::Bitmap* bitmap,
                                               const void* data,
                                               gkg::PixelCoordinate pixelWidth,
                                               gkg::PixelCoordinate pixelHeight,
                                               gkg::PixelCoordinate x0,
                                               gkg::PixelCoordinate y0 ) const
{

  return new gkg::X11BitmapImplementation( bitmap, data, 
                                           pixelWidth, pixelHeight,
                                           x0, y0 );

}


gkg::BitmapImplementation* 
gkg::X11GUIImplementationFactory::createBitmapImplementation(
                                                          gkg::Bitmap* bitmap,
                                                          const gkg::Font* font,
                                                          int32_t character,
                                                          float scale ) const
{

  return new gkg::X11BitmapImplementation( bitmap, font, character, scale );

}


gkg::BitmapImplementation* 
gkg::X11GUIImplementationFactory::createBitmapImplementation(
                                        gkg::Bitmap* bitmap,
                                        gkg::BitmapImplementation* other ) const
{

  return new gkg::X11BitmapImplementation( bitmap,
                        static_cast< gkg::X11BitmapImplementation* >( other ) );

}


gkg::BitmapImplementation* 
gkg::X11GUIImplementationFactory::createBitmapImplementation(
                                             gkg::Bitmap* bitmap,
                                             const std::string& filename ) const
{

  return new gkg::X11BitmapImplementation( bitmap, filename );

}


gkg::RasterImplementation* 
gkg::X11GUIImplementationFactory::createRasterImplementation(
                                        gkg::Raster* raster,
                                        gkg::PixelCoordinate pixelWidth,
                                        gkg::PixelCoordinate pixelHeight ) const
{

  return new gkg::X11RasterImplementation( raster, pixelWidth, pixelHeight );

}


gkg::RasterImplementation* 
gkg::X11GUIImplementationFactory::createRasterImplementation(
                                        gkg::Raster* raster,
                                        gkg::RasterImplementation* other ) const
{

  return new gkg::X11RasterImplementation( raster,
                        static_cast< gkg::X11RasterImplementation* >( other ) );

}


gkg::SessionImplementation* 
gkg::X11GUIImplementationFactory::createSessionImplementation(
                                                   gkg::Session* session ) const
{

  return new gkg::X11SessionImplementation( session );

}


gkg::EventImplementation*
gkg::X11GUIImplementationFactory::createEventImplementation(
                                                       gkg::Event* event ) const
{

  return new gkg::X11EventImplementation( event );

}


gkg::EventImplementation*
gkg::X11GUIImplementationFactory::createEventImplementation(
                                         gkg::Event* event,
                                         gkg::EventImplementation* other ) const
{

  return new gkg::X11EventImplementation( event, 
                         static_cast< gkg::X11EventImplementation* >( other ) );

}


gkg::CursorImplementation* 
gkg::X11GUIImplementationFactory::createCursorImplementation(
                                            gkg::Cursor* cursor,
                                            const gkg::Color* foreground,
                                            const gkg::Color* background ) const
{

  return new gkg::X11CursorImplementation( cursor, foreground, background );

}


gkg::DataCursorImplementation* 
gkg::X11GUIImplementationFactory::createDataCursorImplementation( 
                                                    gkg::DataCursor* dataCursor,
                                                    int16_t xHot, int16_t yHot,
                                                    const int32_t* pattern,
                                                    const int32_t* mask ) const
{

  return new gkg::X11DataCursorImplementation( dataCursor,
                                               xHot, yHot, pattern, mask );

}


gkg::BitmapCursorImplementation* 
gkg::X11GUIImplementationFactory::createBitmapCursorImplementation(
                                                gkg::BitmapCursor* bitmapCursor,
                                                const gkg::Bitmap* pattern,
                                                const gkg::Bitmap* mask ) const
{

  return new gkg::X11BitmapCursorImplementation( bitmapCursor, pattern, mask );

}


gkg::FontCursorImplementation* 
gkg::X11GUIImplementationFactory::createFontCursorImplementation(
                                                    gkg::FontCursor* fontCursor,
                                                    const gkg::Font* font,
                                                    int32_t pattern,
                                                    int32_t mask ) const
{

  return new gkg::X11FontCursorImplementation( fontCursor, font,
                                               pattern, mask );

}


gkg::PredefinedCursorImplementation* 
gkg::X11GUIImplementationFactory::createPredefinedCursorImplementation(
                                        gkg::PredefinedCursor* predefinedCursor,
                                        int32_t code ) const
{

  return new gkg::X11PredefinedCursorImplementation( predefinedCursor, code );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createMenuCursor() const
{

  return new gkg::PredefinedCursor( XC_arrow );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createArrowCursor() const
{

  return new gkg::PredefinedCursor( XC_left_ptr );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createCrossHairsCursor() const
{

  return new gkg::PredefinedCursor( XC_crosshair );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createLeftTextCursor() const
{

  return new gkg::DataCursor( 4, 8, _textPattern, _textMask );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createRightTextCursor() const
{

  return new gkg::DataCursor( 0, 8, _textPattern, _textMask );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createHourGlassCursor() const
{

  return new gkg::PredefinedCursor( XC_watch );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createUpperLeftCursor() const
{

  return new gkg::PredefinedCursor( XC_ul_angle );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createUpperRightCursor() const
{

  return new gkg::PredefinedCursor( XC_ur_angle );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createLowerLeftCursor() const
{

  return new gkg::PredefinedCursor( XC_ll_angle );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createLowerRightCursor() const
{

  return new gkg::PredefinedCursor( XC_lr_angle );

}


gkg::Cursor* gkg::X11GUIImplementationFactory::createNoneCursor() const
{

  return new gkg::DataCursor( 0, 0, _noPattern, _noPattern );

}


gkg::SelectionManagerImplementation*
gkg::X11GUIImplementationFactory::createSelectionManagerImplementation(
                                        gkg::SelectionManager* selectionManager,
                                        gkg::Display* display,
                                        const std::string& name ) const
{

  return new gkg::X11SelectionManagerImplementation( selectionManager,
                                                     display,
                                                     name );

}


gkg::DragImplementation*
gkg::X11GUIImplementationFactory::createDragImplementation(
                                                         gkg::Drag* drag ) const
{

  return new gkg::X11DragImplementation( drag );

}


gkg::DragZoneImplementation*
gkg::X11GUIImplementationFactory::createDragZoneImplementation(
                                                 gkg::DragZone* dragZone ) const
{

  return new gkg::X11DragZoneImplementation( dragZone );

}


gkg::DragZoneSinkImplementation*
gkg::X11GUIImplementationFactory::createDragZoneSinkImplementation(
                                         gkg::DragZoneSink* dragZoneSink ) const
{

  return new gkg::X11DragZoneSinkImplementation( dragZoneSink );

}


gkg::RequestErrorImplementation*
gkg::X11GUIImplementationFactory::createRequestErrorImplementation(
                                         gkg::RequestError* requestError ) const
{

  return new gkg::X11RequestErrorImplementation( requestError );

}
