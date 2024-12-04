#ifndef _gkg_graphics_X11_X11_X11GUIImplementationFactory_h_
#define _gkg_graphics_X11_X11_X11GUIImplementationFactory_h_


#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class X11GUIImplementationFactory :
                                 public GUIImplementationFactory,
                                 public Singleton< X11GUIImplementationFactory >
{

  public:

    ~X11GUIImplementationFactory();

    // canvas
    CanvasImplementation*
      createCanvasImplementation(
        Canvas* canvas,
        Window* window ) const;

    // display
    DisplayImplementation* 
      createDisplayImplementation(
        Display* display,
        const std::string& device ) const;

    // window
    WindowImplementation* 
      createWindowImplementation(
        Window* window,
        Glyph* glyph ) const;
    ManagedWindowImplementation* 
      createManagedWindowImplementation(
        ManagedWindow* managedWindow ) const;
    ApplicationWindowImplementation* 
      createApplicationWindowImplementation(
        ApplicationWindow* applicationWindow ) const;
    TopLevelWindowImplementation* 
      createTopLevelWindowImplementation(
        TopLevelWindow* topLevelWindow ) const;
    TransientWindowImplementation* 
      createTransientWindowImplementation( 
        TransientWindow* transientWindow ) const;
    PopupWindowImplementation* 
      createPopupWindowImplementation(
        PopupWindow* popupWindow ) const;
    UnvisibleWindowImplementation* 
      createUnvisibleWindowImplementation(
        UnvisibleWindow* unvisibleWindow,
        Display* display ) const;
    IconWindowImplementation* 
      createIconWindowImplementation(
        IconWindow* iconWindow ) const;

    // brush
    BrushImplementation* 
      createBrushImplementation(
        const int32_t* pattern,
        int32_t count,
        PixelCoordinate pixelWidth,
        Display* display ) const;

    // color
    ColorImplementation* 
      createColorImplementation(
        Visual* visual,
        FloatColorIntensity r,
        FloatColorIntensity g,
        FloatColorIntensity b,
        float alpha,
        Color::Operation operation ) const;

    // font family and font
    FontFamilyImplementation* 
      createFontFamilyImplementation(
        FontFamily* fontFamily,
        Display* display,
        const std::string& familyName ) const;
    FontImplementation* 
      createFontImplementation(
        Display* display,
        const std::string& name,
        float scale ) const;

    // bitmap
    BitmapImplementation* 
      createBitmapImplementation(
        Bitmap* bitmap,
        const void* data,
        PixelCoordinate pixelWidth,
        PixelCoordinate pixelHeight,
        PixelCoordinate x0,
        PixelCoordinate y0 ) const;
    BitmapImplementation* 
      createBitmapImplementation(
        Bitmap* bitmap,
        const Font* font,
        int32_t character,
        float scale ) const;
    BitmapImplementation* 
      createBitmapImplementation(
        Bitmap* bitmap,
        BitmapImplementation* other ) const;
    BitmapImplementation* 
      createBitmapImplementation(
        Bitmap* bitmap,
        const std::string& filename ) const;

    // raster
    RasterImplementation* 
      createRasterImplementation(
        Raster* raster,
        PixelCoordinate pixelWidth,
        PixelCoordinate pixelHeight ) const;
    RasterImplementation* 
      createRasterImplementation(
        Raster* raster,
        RasterImplementation* other ) const;

    // session
    SessionImplementation* 
      createSessionImplementation(
        Session* session ) const;

    // event
    EventImplementation*
      createEventImplementation(
        Event* event ) const;
    EventImplementation*
      createEventImplementation(
        Event* event,
        EventImplementation* other ) const;

    // cursor
    CursorImplementation* 
      createCursorImplementation(
        Cursor* cursor,
        const Color* foreground,
        const Color* background ) const;
    DataCursorImplementation* 
      createDataCursorImplementation( 
        DataCursor* dataCursor,
        short xHot,
        short yHot,
        const int32_t* pattern,
        const int32_t* mask ) const;
    BitmapCursorImplementation* 
      createBitmapCursorImplementation(
        BitmapCursor* bitmapCursor,
        const Bitmap* pattern,
        const Bitmap* mask ) const;
    FontCursorImplementation* 
      createFontCursorImplementation(
        FontCursor* fontCursor,
        const Font* font,
        int32_t pattern,
        int32_t mask ) const;
    PredefinedCursorImplementation* 
      createPredefinedCursorImplementation(
        PredefinedCursor* predefinedCursor,
        int32_t code ) const;
    Cursor* createMenuCursor() const;
    Cursor* createArrowCursor() const;
    Cursor* createCrossHairsCursor() const;
    Cursor* createLeftTextCursor() const;
    Cursor* createRightTextCursor() const;
    Cursor* createHourGlassCursor() const;
    Cursor* createUpperLeftCursor() const;
    Cursor* createUpperRightCursor() const;
    Cursor* createLowerLeftCursor() const;
    Cursor* createLowerRightCursor() const;
    Cursor* createNoneCursor() const;

    // selection manager
    SelectionManagerImplementation*
      createSelectionManagerImplementation(
        SelectionManager* selectionManager,
        Display* display,
        const std::string& name ) const;

    // drag and drop
    DragImplementation*
      createDragImplementation(
        Drag* drag ) const;
    DragZoneImplementation*
      createDragZoneImplementation(
        DragZone* dragZone ) const;
    DragZoneSinkImplementation*
      createDragZoneSinkImplementation(
        DragZoneSink* dragZoneSink ) const;

    // window manager-dependent request error handling
    RequestErrorImplementation*
      createRequestErrorImplementation(
        RequestError* requestError ) const;

  protected:

    friend class Singleton< X11GUIImplementationFactory >;

    X11GUIImplementationFactory();

};


}


#endif
