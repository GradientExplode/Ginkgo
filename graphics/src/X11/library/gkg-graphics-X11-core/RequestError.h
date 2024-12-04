#ifndef _gkg_graphics_X11_core_RequestError_h_
#define _gkg_graphics_X11_core_RequestError_h_


namespace gkg
{


class RequestErrorImplementation;


// handling errors from window server
class RequestError
{

  public:

    RequestError();
    virtual ~RequestError();

    virtual RequestError* install();
    virtual void error();

    RequestErrorImplementation* getImplementation() const;

  private:

    RequestErrorImplementation* _requestErrorImplementation;

};


}


#endif
