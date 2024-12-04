#ifndef _gkg_graphics_X11_core_SelectionManagerImplementation_h_
#define _gkg_graphics_X11_core_SelectionManagerImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class SelectionManager;
class SelectionHandler;
class Display;
class UnvisibleWindow;


class SelectionManagerImplementation
{

  public:

    virtual ~SelectionManagerImplementation();

    SelectionManager* getTarget() const;

    virtual void own() = 0;
    virtual void putValue( const void* data, int32_t length, 
                           int32_t format ) = 0;
    virtual void retrieve( const std::string& type,
                           SelectionHandler* ok,
                           SelectionHandler* fail );
    virtual void getValue( std::string& type, void*& data, int32_t& length,
                           int32_t& format );

    virtual const std::string& getName() const;

    virtual void doReferencing( SelectionHandler* convert,
                                SelectionHandler* lose,
                                SelectionHandler* done );

  protected:

    SelectionManagerImplementation( SelectionManager* selectionManager,
                                    Display* display,
                                    const std::string& name );

    SelectionManager* _target;
    std::string _name;
    UnvisibleWindow* _owner;
    SelectionHandler* _convert;
    SelectionHandler* _lose;
    SelectionHandler* _done;
    SelectionHandler* _ok;
    SelectionHandler* _fail;

};


}


#endif
