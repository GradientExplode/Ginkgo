#ifndef _gkg_graphics_X11_core_SessionImplementation_h_
#define _gkg_graphics_X11_core_SessionImplementation_h_


#include <string>
#include <list>
#include <gkg-communication-dispatcher/IOHandler.h>


namespace gkg
{


class Session;
class Style;
class Display;
class Event;
struct PropertyData;
struct OptionDescription;


class SessionImplementation
{

  public:

    virtual ~SessionImplementation();

    virtual const std::string& getClassName() const;
    virtual const std::string& getName() const;
    virtual int32_t getArgc() const;
    virtual char** getArgv() const;
    virtual Style* getStyle() const;

    virtual void setDefaultDisplay( Display* display );
    virtual Display* getDefaultDisplay() const;

    virtual void connect( Display* display );
    virtual void disconnect( Display* display );

    virtual int32_t run();
    virtual void quit();
    virtual bool isDone() const;

    virtual bool pending() const;
    virtual void read( Event& event );
    virtual bool read( int64_t sec, int64_t usec, Event& event );

    virtual void handleDisplayInput( Display* display );

    virtual void initialize( const std::string& className,
                             int32_t& argc, char* argv[],
                             const OptionDescription* options,
                             const PropertyData* properties );

  protected:

    SessionImplementation( Session* session,
                           const PropertyData* defaultProperties,
                           const OptionDescription* defaultOptions  );

    virtual void parseArgs( int32_t& argc, char** argv,
                            const OptionDescription* options );
    virtual bool match( const std::string& arg,
                        const OptionDescription& option,
                        int32_t& i, int32_t argc, char** argv );
    virtual void extract( const std::string& arg,
                          const OptionDescription& option,
                          int32_t& i, int32_t argc, char** argv,
                          std::string& name, std::string& value );
    virtual void badArg( const char* format, const std::string& arg );
    virtual std::string nextArg( int32_t& i, int32_t argc, char** argv,
                                 const char* message,
                                 const std::string& arg );
    virtual bool findArg( const std::string& name, std::string& value );

    virtual void initializeStyle( const std::string& className,
                                  const PropertyData* properties );
    virtual std::string findName();
    virtual void loadProperties( Style* style, const PropertyData* properties,
                                 int32_t priority );
    virtual void loadGkgDefaults( Style* style, int32_t priority ) = 0;
    virtual void loadApplicationDefaults( Style* style, int32_t priority ) = 0;
    virtual void loadDisplayDefaults( Style* style, Display* display,
                                      int32_t priority ) = 0;
    virtual void loadEnvironment( Style* style, int32_t priority ) = 0;

    virtual void initializeDisplay();
    virtual void setStyle( Display* display );
    virtual bool check( Event& event ) const;


    Session* _target;

    int32_t _argc;
    char** _argv;
    bool _done;
    bool _readInput;
    std::string _className;
    std::string _name;
    Style* _style;
    const PropertyData* _properties;
    Display* _defaultDisplay;
    std::list< Display* > _displays;

    const PropertyData* _defaultProperties;
    const OptionDescription* _defaultOptions;

};


// SessionIOHandler is a helper class to translate input ready for a
// display fd into a call to:
//     SessionImplementation::handleDisplayInput( Display* );
class SessionIOHandler : public IOHandler
{

  public:

    SessionIOHandler( SessionImplementation* sessionImplementation,
                      Display* display );
    virtual ~SessionIOHandler();

    virtual int32_t isInputReady( int32_t );

  private:

    SessionImplementation* _sessionImplementation;
    Display* _display;

};


};


#endif
