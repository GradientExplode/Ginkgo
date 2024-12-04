#ifndef _gkg_graphics_X11_core_SelectionHandler_h_
#define _gkg_graphics_X11_core_SelectionHandler_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class SelectionManager;


class SelectionHandler : public RCGUIObject
{

  public:

    virtual ~SelectionHandler();

    virtual void handle( SelectionManager* selectionManager ) = 0;

  protected:

    SelectionHandler();    

};


template < class T >
class SelectionCallback : public SelectionHandler
{

  public:

    typedef void ( T::*MemberFunction )( SelectionManager* );

    SelectionCallback( T* object, MemberFunction function );

    virtual void handle( SelectionManager* selectionManager );

  private:

    T* _object;
    MemberFunction _function;
};


}


template < class T >
inline
gkg::SelectionCallback< T >::SelectionCallback(
                          T* object,
                          gkg::SelectionCallback< T >::MemberFunction function )
                            : gkg::SelectionHandler(),
                              _object( object ),
                              _function( function )
{
}


template < class T >
inline
void 
gkg::SelectionCallback< T >::handle( gkg::SelectionManager* selectionManager )
{

  ( _object->*_function )( selectionManager );

}


#endif

