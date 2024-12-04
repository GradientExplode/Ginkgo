#ifndef _gkg_graphics_X11_core_SelectionManager_h_
#define _gkg_graphics_X11_core_SelectionManager_h_


#include <string>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Display;
class SelectionHandler;
class SelectionManagerImplementation;


// inter client communications mechanism
class SelectionManager : public RCGUIObject
{

  public:

    SelectionManager( Display* display, const std::string& name );
    virtual ~SelectionManager();

    virtual void own( SelectionHandler* convert,
                      SelectionHandler* lose = 0,
                      SelectionHandler* done = 0 );
    virtual void putValue( const void* data, int32_t length, 
                           int32_t format = 8 );
    virtual void retrieve( const std::string& type,
                           SelectionHandler* ok,
                           SelectionHandler* fail = 0 );
    virtual void getValue( std::string& type, void*& data, int32_t& length,
                           int32_t& format );

    SelectionManagerImplementation* getImplementation() const;

  private:

    SelectionManagerImplementation* _selectionManagerImplementation;

};


}


#endif
