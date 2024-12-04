#ifndef _gkg_graphics_X11_core_GUIImplementationFactory_h_
#define _gkg_graphics_X11_core_GUIImplementationFactory_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Color.h>


namespace gkg
{


class Canvas;
class CanvasImplementation;
class Display;
class DisplayImplementation;
class Glyph;
class Visual;
class Window;
class WindowImplementation;
class ManagedWindow;
class ManagedWindowImplementation;
class ApplicationWindow;
class ApplicationWindowImplementation;
class TopLevelWindow;
class TopLevelWindowImplementation;
class TransientWindow;
class TransientWindowImplementation;
class PopupWindow;
class PopupWindowImplementation;
class UnvisibleWindow;
class UnvisibleWindowImplementation;
class IconWindow;
class IconWindowImplementation;
class Brush;
class BrushImplementation;
class Color;
class ColorImplementation;
class FontFamily;
class FontFamilyImplementation;
class Font;
class FontImplementation;
class Bitmap;
class BitmapImplementation;
class Raster;
class RasterImplementation;
class Session;
class SessionImplementation;
class Event;
class EventImplementation;
class Cursor;
class CursorImplementation;
class DataCursor;
class DataCursorImplementation;
class BitmapCursor;
class BitmapCursorImplementation;
class FontCursor;
class FontCursorImplementation;
class PredefinedCursor;
class PredefinedCursorImplementation;
class SelectionManager;
class SelectionManagerImplementation;
class Drag;
class DragImplementation;
class DragZone;
class DragZoneImplementation;
class DragZoneSink;
class DragZoneSinkImplementation;
class RequestError;
class RequestErrorImplementation;


class GUIImplementationFactory
{

  public:

    virtual ~GUIImplementationFactory();

    // canvas
    virtual CanvasImplementation* 
              createCanvasImplementation(
                Canvas* canvas,
                Window* window ) const = 0;

    // display
    virtual DisplayImplementation*
              createDisplayImplementation(
                Display* display,
                const std::string& device ) const= 0;

    // window
    virtual WindowImplementation* 
              createWindowImplementation(
                Window* window,
                Glyph* glyph ) const = 0;
    virtual ManagedWindowImplementation* 
              createManagedWindowImplementation( 
                ManagedWindow* managedWindow ) const = 0;
    virtual ApplicationWindowImplementation* 
              createApplicationWindowImplementation( 
                ApplicationWindow* applicationWindow ) const = 0;
    virtual TopLevelWindowImplementation* 
              createTopLevelWindowImplementation( 
                TopLevelWindow* topLevelWindow ) const = 0;
    virtual TransientWindowImplementation* 
              createTransientWindowImplementation( 
                TransientWindow* transientWindow ) const = 0;
    virtual PopupWindowImplementation* 
              createPopupWindowImplementation( 
                PopupWindow* popupWindow ) const = 0;
    virtual UnvisibleWindowImplementation* 
              createUnvisibleWindowImplementation( 
                UnvisibleWindow* unvisibleWindow,
                Display* display ) const = 0;
    virtual IconWindowImplementation* 
              createIconWindowImplementation( 
                IconWindow* iconWindow ) const = 0;

    // brush
    virtual BrushImplementation* 
              createBrushImplementation(
                const int32_t* pattern, int32_t count,
                PixelCoordinate pixelWidth,
                Display* display ) const = 0;
    // color
    virtual ColorImplementation* 
              createColorImplementation(
                Visual* visual,
                FloatColorIntensity r,
                FloatColorIntensity g,
                FloatColorIntensity b,
                float alpha,
                Color::Operation operation ) const = 0;

    // font family and font
    virtual FontFamilyImplementation* 
              createFontFamilyImplementation( 
                FontFamily* fontFamily,
                Display* display,
                const std::string& familyName ) const = 0;
    virtual FontImplementation* 
              createFontImplementation(
                Display* display,
                const std::string& name,
                float scale ) const = 0;

    // bitmap
    virtual BitmapImplementation* 
              createBitmapImplementation(
                Bitmap* bitmap,
                const void* data,
                PixelCoordinate pixelWidth,
                PixelCoordinate pixelHeight,
                PixelCoordinate x0,
                PixelCoordinate y0 ) const = 0;
    virtual BitmapImplementation* 
              createBitmapImplementation(
                Bitmap* bitmap,
                const Font* font,
                int32_t character,
                float scale ) const = 0;
    virtual BitmapImplementation* 
              createBitmapImplementation(
                Bitmap* bitmap,
                BitmapImplementation* other ) const = 0;
    virtual BitmapImplementation* 
              createBitmapImplementation(
                Bitmap* bitmap,
                const std::string& filename ) const = 0;

    // raster
    virtual RasterImplementation* 
              createRasterImplementation(
                Raster* raster,
                PixelCoordinate pixelWidth,
                PixelCoordinate pixelHeight ) const = 0;
    virtual RasterImplementation* 
              createRasterImplementation(
                Raster* raster,
                RasterImplementation* other ) const = 0;

    // session
    virtual SessionImplementation* 
              createSessionImplementation(
                Session* session ) const = 0;

    // event
    virtual EventImplementation*
              createEventImplementation(
                Event* event ) const = 0;
    virtual EventImplementation*
              createEventImplementation(
                Event* event,
                EventImplementation* other ) const = 0;

    // cursor
    virtual CursorImplementation* 
              createCursorImplementation(
                Cursor* cursor,
                const Color* foreground,
                const Color* background ) const = 0;
    virtual DataCursorImplementation* 
              createDataCursorImplementation( 
                DataCursor* dataCursor,
                int16_t xHot, int16_t yHot,
                const int32_t* pattern, const int32_t* mask ) const = 0;
    virtual BitmapCursorImplementation* 
              createBitmapCursorImplementation(
                BitmapCursor* bitmapCursor,
                const Bitmap* pattern,
                const Bitmap* mask ) const = 0;
    virtual FontCursorImplementation* 
              createFontCursorImplementation(
                FontCursor* fontCursor,
                const Font* font,
                int32_t pattern, int32_t mask ) const = 0;
    virtual PredefinedCursorImplementation* 
              createPredefinedCursorImplementation(
                PredefinedCursor* predefinedCursor,
                int32_t code ) const = 0;
    virtual Cursor* createArrowCursor() const = 0;
    virtual Cursor* createMenuCursor() const = 0;
    virtual Cursor* createCrossHairsCursor() const = 0;
    virtual Cursor* createLeftTextCursor() const = 0;
    virtual Cursor* createRightTextCursor() const = 0;
    virtual Cursor* createHourGlassCursor() const = 0;
    virtual Cursor* createUpperLeftCursor() const = 0;
    virtual Cursor* createUpperRightCursor() const = 0;
    virtual Cursor* createLowerLeftCursor() const = 0;
    virtual Cursor* createLowerRightCursor() const = 0;
    virtual Cursor* createNoneCursor() const = 0;

    // selection manager
    virtual SelectionManagerImplementation*
              createSelectionManagerImplementation(
                SelectionManager* selectionManager,
                Display* display,
                const std::string& name ) const = 0;

    // drag and drop
    virtual DragImplementation*
              createDragImplementation(
                Drag* drag ) const = 0;
    virtual DragZoneImplementation*
              createDragZoneImplementation(
                DragZone* dragZone ) const = 0;
    virtual DragZoneSinkImplementation*
              createDragZoneSinkImplementation(
                DragZoneSink* dragZoneSink ) const = 0;

    // window manager-dependent request error handling
    virtual RequestErrorImplementation*
              createRequestErrorImplementation(
                RequestError* requestError ) const = 0;

  protected:

    GUIImplementationFactory();

};


// this function is used to get access to the concrete implementation
// factory and should be implemented in the concrete library
// (in libgkg-gui-X11.so for instance )
GUIImplementationFactory* getGUIImplementationFactory();


}


#endif
