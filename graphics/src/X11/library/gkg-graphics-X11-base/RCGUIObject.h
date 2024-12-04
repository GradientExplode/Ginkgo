#ifndef _gkg_graphics_X11_base_RCGUIObject_h_
#define _gkg_graphics_X11_base_RCGUIObject_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class RCGUIObject
{

  public:

    virtual ~RCGUIObject();

    void addReference() const;
    void removeReference() const;
    void removeReferenceDeferred() const;

    // this function is used to cleanup specific stuff allocated within shared
    // object (actually, only Font class inheritates this function)
    virtual void cleanup();

    int32_t getCount() const;

    static bool defer( bool value );
    static void flush();

  protected:

    RCGUIObject();
    RCGUIObject( const RCGUIObject& );
    RCGUIObject& operator=( const RCGUIObject& );

  private:

    mutable int32_t _count;

};


}


#endif
