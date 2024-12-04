#ifndef _gkg_graphics_X11_X11_X11RequestErrorImplementation_h_
#define _gkg_graphics_X11_X11_X11RequestErrorImplementation_h_


#include <string>
#include <gkg-graphics-X11-core/RequestErrorImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11RequestErrorImplementation : public RequestErrorImplementation
{

  public:

    X11RequestErrorImplementation( RequestError* requestError );
    virtual ~X11RequestErrorImplementation();

    RequestError* install();
    void error();

    void setMessageId( uint32_t messageId );
    uint32_t getMessageId() const;

    void setCode( int32_t code );
    int32_t getCode() const;

    void setRequest( int32_t request );
    int32_t getRequest() const;

    void setDetail( int32_t detail );
    int32_t getDetail() const;

    void setId( char* id );
    char* getId() const;

    void setMessage( const std::string& message );
    const std::string& getMessage() const;

    static X11RequestErrorImplementation* 
      getImplementation( RequestError* requestError );

  protected:

    uint32_t _messageId;
    int32_t _code;
    int32_t _request;
    int32_t _detail;
    char* _id;
    std::string _message;

};


}


#endif
