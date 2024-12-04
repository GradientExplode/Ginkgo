#ifndef _gkg_graphics_X11_core_Session_h_
#define _gkg_graphics_X11_core_Session_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class Display;
class Event;
class Window;
class Style;
class SessionImplementation;


struct PropertyData
{

  const char* path;       // class/instance(s) property name
  const char* value;      // property value

};




struct OptionDescription
{

  enum OptionStyle
  {

    OptionPropertyNext,     // property and value are in argv[ i + 1 ]
    OptionValueNext,        // argv[ i + 1 ]
    OptionValueImplicit,    // OptionDesc.value
    OptionValueIsArg,       // argv[ i ]
    OptionValueAfter,       // &argv[ i ][ strlen( optionDesc.name ) ]
    OptionNone

  };
  const char* name;
  const char* path;
  OptionStyle style;
  const char* value;

};


// typically, at the beginning of a software main procedure, call:
// gkg::Session::getInstance().initialize( "MyClassName", argc, argv );

class Session : public Singleton< Session >
{

  public:

    virtual ~Session();

    virtual const std::string& getClassName() const;
    virtual const std::string& getName() const;
    virtual int32_t getArgc() const;
    virtual char** getArgv() const;
    virtual Style* getStyle() const;

    virtual void setDefaultDisplay( Display* display );
    virtual Display* getDefaultDisplay() const;

    virtual Display* connect( const std::string& displayName );
    virtual void disconnect( Display* display );

    virtual int32_t run();
    virtual int32_t runWindow( Window* window );
    virtual void quit();
    virtual bool isDone() const;

    virtual bool pending() const;
    virtual void read( Event& event );
    virtual bool read( int32_t sec, int32_t usec, Event& event );
    virtual void unread( Event& event );
    virtual void poll( Event& event );

    virtual void initialize( const std::string& className,
                            int32_t& argc, char* argv[],
                            const OptionDescription* options = 0,
                            const PropertyData* properties = 0 );

    SessionImplementation* getImplementation() const;

  protected:

    friend class Singleton< Session >;

    Session();

  private:

    SessionImplementation* _sessionImplementation;

};


}


#endif
