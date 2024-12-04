#ifndef _gkg_graphics_X11_core_FileBrowserImplementation_h_
#define _gkg_graphics_X11_core_FileBrowserImplementation_h_


#include <gkg-graphics-X11-core/Glyph.h>


#define FILE_BROWSER_KEYFUNCTION_SIZE 256


namespace gkg
{


class FileBrowser;
class Cursor;
class IOHandler;
class TBScrollBox;
class Event;
class Adjustable;


class FileBrowserImplementation 
{

  public:

    typedef void ( FileBrowserImplementation::*KeyFunction )();

    enum Mode
    {

      Selecting,
      GrabScrolling,
      RateScrolling

    };

    struct KeyInfo
    {

      char key;
      const char* name;
      KeyFunction function;

    };

    FileBrowserImplementation( FileBrowser* fileBrowser );
    ~FileBrowserImplementation();

    FileBrowser* getTarget() const;

    void press( const Event& event );
    void drag( const Event& event );
    void release( const Event& event );
    void keyStroke( const Event& event );

    void focusIn();
    void focusOut();

    void select( Glyph::Index index );
    Adjustable* getAdjustable() const;

    void refresh();

    void setRateScrollTimer( int64_t, int64_t );

    void open();
    void cancel();
    void scrollToFirst();
    void scrollToLast();
    void selectAll();
    void unselectAll();
    void nextFocus();
    void selectPrevious();
    void selectNext();
    void selectTop();
    void selectBottom();
    void scrollDown();
    void scrollUp();
    void pageDown();
    void pageUp();
    void halfPageDown();
    void halfPageUp();

  private:

    FileBrowser* _target;

    Glyph::Index _selected;
    TBScrollBox* _box;
    Mode _mode;
    FloatCoordinate _scale;
    Cursor* _saveCursor;
    FloatCoordinate _startScrollPointer;
    FloatCoordinate _currentScrollPointer;
    FloatCoordinate _startScrollPosition;
    IOHandler* _rateHandler;
    int32_t _usecRate;
    KeyFunction _keyFunctions[ FILE_BROWSER_KEYFUNCTION_SIZE ];

};


}


#endif
