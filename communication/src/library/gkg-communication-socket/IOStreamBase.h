#ifndef _gkg_communication_socket_IOStreamBase_h_
#define _gkg_communication_socket_IOStreamBase_h_


#include <gkg-core-cppext/StdInt.h>
#include <iostream>


namespace gkg
{


// modify ios to store extra state information for binary I/O.
class IOStreamBase : public virtual std::ios
{

  public:

    // Make sure that these assumptions about the sizes of integral types
    // hold for your machine.  If your machine does not have 32-bit and
    // 16-bit integers, then you cannot use binary I/O because other
    // machines will expect the integers to be these sizes.  If your
    // machine does not have 8-bit characters, then I don't know how
    // you're going to communicate with other machines!
    typedef int64_t INT64;
    typedef int32_t INT32;
    typedef int16_t INT16;
    typedef uint64_t UINT64;
    typedef uint32_t UINT32;
    typedef uint16_t UINT16;

    IOStreamBase();
    ~IOStreamBase();

    bool getBinary() const;
    void setBinary( bool b );
    bool getSwapped() const;
    void setSwapped( bool b );

  protected:

    bool _binary;      // are my peer and I performing binary I/O?
    bool _swapped;     // does my peer have a swapped endian?

};


}


#endif
