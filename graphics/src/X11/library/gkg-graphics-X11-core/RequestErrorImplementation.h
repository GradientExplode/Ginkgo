#ifndef _gkg_graphics_X11_core_RequestErrorImplementation_h_
#define _gkg_graphics_X11_core_RequestErrorImplementation_h_


namespace gkg
{


class RequestError;


class RequestErrorImplementation
{

  public:

    virtual ~RequestErrorImplementation();

    virtual RequestError* getTarget() const;

    virtual RequestError* install() = 0;
    virtual void error() = 0;

  protected:

    RequestErrorImplementation( RequestError* requestError );

    RequestError* _target;

};


}


#endif
