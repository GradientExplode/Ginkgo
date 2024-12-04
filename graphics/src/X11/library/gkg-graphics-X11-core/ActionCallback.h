#ifndef _gkg_graphics_X11_core_ActionCallback_h_
#define _gkg_graphics_X11_core_ActionCallback_h_


#include <gkg-graphics-X11-core/Action.h>



namespace gkg
{


template < class T >
class ActionCallback : public Action
{

  public:

    typedef void ( T::*MemberFunction )();

    ActionCallback( T* object, MemberFunction function );
    ~ActionCallback();

    virtual void execute();

  private:

    T* _object;
    MemberFunction _function;

};


}


template < class T >
inline
gkg::ActionCallback< T >::ActionCallback(
                             T* object,
                             gkg::ActionCallback< T >::MemberFunction function )
                         : gkg::Action(),
                           _object( object ),
                           _function( function )
{
}


template < class T >
inline
gkg::ActionCallback< T >::~ActionCallback()
{
}


template < class T >
inline
void gkg::ActionCallback< T >::execute()
{

 ( _object->*_function )();

}


#endif
