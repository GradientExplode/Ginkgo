#ifndef _gkg_graphics_X11_core_PredefinedCursorImplementation_h_
#define _gkg_graphics_X11_core_PredefinedCursorImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Display;
class Visual;
class PredefinedCursor;


class PredefinedCursorImplementation
{

  public:

    virtual ~PredefinedCursorImplementation();

    virtual PredefinedCursor* getTarget() const;

    virtual void makeCursor( Display* display, Visual* visual ) = 0;



  protected:

    PredefinedCursorImplementation( PredefinedCursor* predefinedCursor,
                                    int32_t code );

    PredefinedCursor* _target;

    int32_t _code;

};


}


#endif
