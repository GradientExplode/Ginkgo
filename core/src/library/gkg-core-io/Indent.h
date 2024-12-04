#ifndef _gkg_core_io_Indent_h_
#define _gkg_core_io_Indent_h_


#include <string>


namespace gkg
{


class Indent
{

  public:

    Indent();

    void reset();
    void increment();
    void decrement();

    std::string getSpaces() const;

  private:

    size_t _count;

};


}


#endif
