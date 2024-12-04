#ifndef _gkg_graphics_X11_core_Stepper_h_
#define _gkg_graphics_X11_core_Stepper_h_


#include <gkg-graphics-X11-core/Button.h>


namespace gkg
{


class IOHandler;
class TellTaleState;


// button with autorepeat
class Stepper : public Button
{

  public:

    Stepper( Glyph* glyph,
             Style* style,
             TellTaleState* tellTaleState,
             Action* action = 0 );
    virtual ~Stepper();

    virtual void press( const Event& event );
    virtual void release( const Event& event );

    virtual void startStepping();
    virtual void stopStepping();

  protected:

    virtual void adjust() = 0;

  private:

    void tick( int64_t, int64_t );

    int64_t _startDelay;
    int64_t _nextDelay;
    IOHandler* _timer;

};


}


#endif
